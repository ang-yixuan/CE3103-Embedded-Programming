#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/stat.h>

volatile int g_is_pb_pressed = 0; // volatile because it is used in multiple threads

struct leds {
    struct gpiohandle_request request;
    struct gpiohandle_data data;
};

typedef struct leds LEDs;

struct push_button {
    struct gpioevent_request request;
    struct gpioevent_data event;
};

typedef struct push_button PushButton;

// Function to initialize LEDs
LEDs init_leds(int fd) {
    LEDs ryg_leds;

    ryg_leds.request.lines = 3;
    ryg_leds.request.lineoffsets[0] = 27; // Red LED
    ryg_leds.request.lineoffsets[1] = 4;  // Yellow LED
    ryg_leds.request.lineoffsets[2] = 17; // Green LED
    ryg_leds.request.flags = GPIOHANDLE_REQUEST_OUTPUT;

    ryg_leds.data.values[0] = 0; // Red LED initially off
    ryg_leds.data.values[1] = 0; // Yellow LED initially off
    ryg_leds.data.values[2] = 1; // Green LED initially on

    ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &(ryg_leds.request));

    return ryg_leds;
}

// Function to initialize the push button
PushButton init_push_button(int fd) {
    PushButton pb;

    pb.request.lineoffset = 11; // Push Button
    pb.request.handleflags = GPIOHANDLE_REQUEST_INPUT;
    pb.request.eventflags = GPIOEVENT_EVENT_RISING_EDGE | GPIOEVENT_EVENT_FALLING_EDGE;
    strcpy(pb.request.consumer_label, "Push Button event");
    ioctl(fd, GPIO_GET_LINEEVENT_IOCTL, &(pb.request));

    return pb;
}

// Thread function to blink the red LED when the button is pressed
void* blink_red_led(void* args) {
    LEDs* ryg_leds = (LEDs*) args;

    while (1) {
        if (g_is_pb_pressed) {
            ryg_leds->data.values[0] = !ryg_leds->data.values[0]; // Toggle red LED
            ioctl(ryg_leds->request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &(ryg_leds->data));
            usleep(500000); // Blink interval of 0.5 seconds
        } else {
            if (ryg_leds->data.values[0]) {
                ryg_leds->data.values[0] = 0; // Ensure red LED is off if button is not pressed
                ioctl(ryg_leds->request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &(ryg_leds->data));
            }
            usleep(100000); // Small delay to prevent busy waiting
        }
    }
}

// Thread function to alternate blinking of yellow and green LEDs
void* toggle_yellow_green_leds(void* args) {
    LEDs* ryg_leds = (LEDs*) args;

    while (1) {
        ryg_leds->data.values[1] = !ryg_leds->data.values[1]; // Toggle yellow LED
        ryg_leds->data.values[2] = !ryg_leds->data.values[2]; // Toggle green LED
        ioctl(ryg_leds->request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &(ryg_leds->data));
        usleep(500000); // Blink interval of 0.5 seconds
    }
}

// Thread function to listen to push button events
void* listen_push_button(void* args) {
    struct push_button* pb = (struct push_button*) args;

    while (1) {
        int read_result = read(pb->request.fd, &(pb->event), sizeof(pb->event));

        if (read_result == -1) {
            perror("Error reading push button");
        } else if (pb->event.id == GPIOEVENT_EVENT_RISING_EDGE) {
            printf("Button pressed\n");
            g_is_pb_pressed = 1;
        } else if (pb->event.id == GPIOEVENT_EVENT_FALLING_EDGE) {
            printf("Button released\n");
            g_is_pb_pressed = 0;
        }
    }
}

int main(int argc, char* argv[]) {
    int fd0 = open("/dev/gpiochip0", O_RDWR);
    if (fd0 < 0) {
        perror("Failed to open /dev/gpiochip0");
        return EXIT_FAILURE;
    }

    // Initialize LEDs and Push Button
    LEDs ryg_leds = init_leds(fd0);
    PushButton pb = init_push_button(fd0);

    // Start threads
    pthread_t red_blink_thread, yellow_green_toggle_thread, pb_thread;
    pthread_create(&red_blink_thread, NULL, blink_red_led, (void*)&ryg_leds);
    pthread_create(&yellow_green_toggle_thread, NULL, toggle_yellow_green_leds, (void*)&ryg_leds);
    pthread_create(&pb_thread, NULL, listen_push_button, (void*)&pb);

    // Wait for threads to finish (they won't in this case due to while(1))
    pthread_join(red_blink_thread, NULL);
    pthread_join(yellow_green_toggle_thread, NULL);
    pthread_join(pb_thread, NULL);

    // Cleanup
    close(pb.request.fd);
    close(ryg_leds.request.fd);
    close(fd0);

    return EXIT_SUCCESS;
}

