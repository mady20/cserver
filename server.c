#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024


/*
 * Prints a message to the stdout and then terminate the program.
 * msg: The message to print to the console.
 * Returns: Nothing.
 */
void error(char *msg){
  printf("[ERROR] %s\n", msg);
  exit(1);
}

void info(char *msg){
  printf("[Info]  %s\n", msg);
}

/*
 * Creates a stream socket
 * fd: File descriptor, on which will read from and write to
 * Returns: false if fd < 0, otherwise true 
 */

bool createSocket(int* fd){
  *fd = socket(AF_INET, SOCK_STREAM, 0);
  if(*fd < 0) return false;
  return true;
} 

bool bindSocket(int fd, struct sockaddr_in* address){
  if(bind(fd, (struct sockaddr *) address, sizeof(*address)) >= 0) return true;
  close(fd);
  return false;
}

bool acceptConnection(int* serverSocket, int* clientSocket, struct sockaddr_in* clientAddress){
  int addressLength = sizeof(*clientAddress);
  *clientSocket = accept(*serverSocket, (struct sockaddr *)clientAddress, (socklen_t *restrict) &addressLength);
  if(*clientSocket > 0) return true;
  close(*serverSocket);
  close(*clientSocket);
  return false;
}

void prepareServerAddress(struct sockaddr_in* address, int port){
  address->sin_family = AF_INET;
  address->sin_port = htons(port);
  address->sin_addr.s_addr = INADDR_ANY;
}

bool readBytes(int clientSocket, int serverSocket, char* buffer){
  int bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
  if (bytesRead == 0){
    info("Client Disconnected!");
    return false;
  }else if(bytesRead < 0){
    close(clientSocket);
    close(serverSocket);
    error("Failed to read from client socket!");
  }
  buffer[bytesRead] = '\0';
  if (strcmp(buffer, "end\n") == 0 || strcmp(buffer, "end") == 0) {
    info("Client Disconnected!");
    return false;
  }
  printf("%d %s\n", bytesRead, buffer);
  return true;
}

int main(int argc, char **argv) {

  if (argc < 2) error("no port provided!\n");

  int serverSocket, clientSocket, portno, n;
  char buffer[BUFFER_SIZE];
  int totalBytesReceived = 0;
  struct sockaddr_in serverAddress, clientAddress;


  if(!createSocket(&serverSocket)) error("Failed to Create Socket!");

  info("Server socket created successfully!");
  
  bzero((char *)&serverAddress, sizeof(serverAddress));
  bzero(buffer, BUFFER_SIZE);
  

  portno = atoi(argv[1]);
  prepareServerAddress(&serverAddress, portno);

  bindSocket(serverSocket, &serverAddress);
  info("Address binded successfully!");


  // start listen on the socket for connections
  // 2nd arg is backlog queue, i.e no of waiting connections while server is
  // handling current 5 is max permited by most systems
  listen(serverSocket, 5);
  info("Server started listening!");

  while(true){
    if(!acceptConnection(&serverSocket, &clientSocket, &clientAddress)) error("Unable to accept connections!");
    info("Client socket created!");
    while(true){
      if(!readBytes(clientSocket, serverSocket, buffer)) break;
    }
  }

  close(clientSocket);
  close(serverSocket);
  return 0;
}
