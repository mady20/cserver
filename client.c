#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

void error(char *msg){
  printf("[ERROR] %s\n", msg);
  exit(1);
}

void info(char *msg){
  printf("[Info]  %s\n", msg);
}

void prepareServerAddress(struct sockaddr_in* serverAddress, int port){
  serverAddress->sin_family = AF_INET;
  serverAddress->sin_port = htons(port); 
  if(inet_pton(AF_INET, SERVER_IP, &serverAddress->sin_addr) <= 0) {
    error("ERROR address not supported");
  }
}


bool createSocket(int* fd){
  *fd = socket(AF_INET, SOCK_STREAM, 0);
  if(*fd < 0) return false;
  return true;
}


int main(int argc, char **argv){
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];
    
    if(argc < 2){
        error("USAGE ./client <port>\n");
    }
    int SERVER_PORT = atoi(argv[1]);
    if(!createSocket(&clientSocket)){
      error("Failed to open socket!");
    }
    info("Client socket created!");

    prepareServerAddress(&serverAddress, SERVER_PORT);

    if(connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1){
        error("Connection Failed");
    }
    info("Connected to server!");
    while (true) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            --len;
        }

        if (strcmp(buffer, "end") == 0) {
            if (send(clientSocket, "end\n", 4, 0) < 0) {
                perror("send");
            }
            break;
        }

        if (len == 0) {
            continue;
        }

        ssize_t sent = send(clientSocket, buffer, len, 0);
        if (sent < 0) {
            perror("send");
            break;
        }
    }
    // receive response from server
    //int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
    
    //if (bytes_received > 0) {
    //    buffer[bytes_received] = '\0';
    //    printf("Server response: %s\n", buffer);
    //}

    close(clientSocket);
    return 0;
}
