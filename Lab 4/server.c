// server.c
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// sem_t semaphore; //Global semaphore

void error(char *m) {
    perror(m);
    exit(1);
}

void *handle_socket(void *newsockfd) { //Handle individual sockets

    // sem_wait(&semaphore); //Wait for semaphore to ensure that not more than 3 threads are active simultaneously

    int sock = *(int *)newsockfd; //Initialise socket
    char buffer[256];
    int n;

    memset(buffer, 0, 256);
    n = read(sock, buffer, 255); //Read message from socket (client)
    if (n < 0)
        error("ERROR reading from socket");

    printf("Message received: %s\n", buffer);

    sleep(5); //Allow program to allow more connections before exiting
    int num = 5 * atoi(buffer); //Process message and multiply num by 5
    sprintf(buffer, "%d", num);

    n = write(sock, buffer, 255); //Write back to socket
    if (n < 0)
        error("ERROR writing back to socket");

    close(sock); //Close socket
    // sem_post(&semaphore); //Unlock semaphore
    free(newsockfd); //Free memory
    pthread_exit(NULL); //Exit thread
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, port, clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) //Check that port number is argument
        error("ERROR, no port provided\n");

    port = atoi(argv[1]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //Create server socket
    if (sockfd < 0)
        error("ERROR opening socket");
    
    //Configure server address
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port); //Host to network

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) //Bind server to port
        error("ERROR binding to socket");

    listen(sockfd, 3); //Listen for client connections, maximum 3
    clilen = sizeof(cli_addr);
    // sem_init(&semaphore, 0, 3);  //Initialize semaphore to allow maximum 3 concurrent threads

    while (1) { //Keeps accepting client connections and spawns a new thread for each client
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        //Create a new thread to handle client socket
        int *new_sock = malloc(1 * sizeof(int));
        *new_sock = newsockfd;
        pthread_t t;

        if (pthread_create(&t, NULL, handle_socket, (void *)new_sock) < 0) {
            error("ERROR creating thread");
        }
    }

    close(sockfd); //Close main socket
    // sem_destroy(&semaphore); //Destroy semaphore

    return 0;
}
