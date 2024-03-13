// client.c
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void error(char *m) {
    perror(m);
    exit(0);
}

//Manage client socket connection and communication
void process_request(const char *hostname, int port, int num) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    //Create client socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    //Get server details
    server = gethostbyname(hostname);
    if (server == NULL)
        error("ERROR, no such host\n");

    //Configure server address
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    //Connect to server
    if (connect(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    //Send random number to server and receive reply
    memset(buffer, 0, 256);
    sprintf(buffer, "%d", num); 
    printf("Child process %d: The number sent is: %s\n", getpid(), buffer);

    if (write(sockfd, buffer, strlen(buffer)) < 0)
        error("ERROR writing to socket");

    if (read(sockfd, buffer, 255) < 0)
        error("ERROR reading from socket");

    printf("Child process %d: %s\n", getpid(), buffer);

    close(sockfd); //Close client socket
}

//Fork multiple child processes to send requests to server
int main(int argc, char *argv[]) {
    if (argc < 3)
        error("usage client [hostname] [port]\n");

    const char *hostname = argv[1];
    int port = atoi(argv[2]);

    srand(time(0));

    for (int i = 0; i < 3; i++) {
        int r = (rand() % 100);
        pid_t pid = fork();

        if (pid < 0) {
            error("ERROR on fork");
        }

        if (pid == 0) {
            process_request(hostname, port, r);
            exit(0);
        }
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0)
        ;

    return 0;
}
