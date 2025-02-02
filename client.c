#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

// in case of error, print msg and terminate
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    if(argc < 3){
        error("USAGE ./client <ip> <port>\n");
    }

    char* SERVER_IP = argv[1];
    int SERVER_PORT = atoi(argv[2]);
    // creating a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("ERROR opening socket");
    }
    printf("client socket created successfully!\n");

    // configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        error("ERROR address not supported");
    }

    // connect to the server
    if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
        error("Connection Failed");
    }
    printf("Connected to server.\n");

    while(true){
        bzero(buffer,BUFFER_SIZE);
        scanf("%s", buffer);
        if(strcmp(buffer, "end")){
            break;
        }
        // send data to the server
        send(sockfd, buffer, strlen(buffer), 0);
    }

    
    // receive response from server
    int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
    
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
