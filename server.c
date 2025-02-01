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


// in case of error, print msg and terminate
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {

  // declarations
  int sockfd, newsockfd, portno, clilen, n;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in serv_addr, cli_addr;

  // port is required
  if (argc < 2) {
    error("no port provided!\n");
  }
  // takes address domain, type of socket, protocol, returns a fd
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // if the socket call fails it returns -1
  if (sockfd < 0) {
    close(sockfd);
    error("ERROR opening socket");
  }

  // initialize the data to '\0' at n (2nd arg) bytes starting from char* (1st
  // arg)
  bzero((char *)&serv_addr, sizeof(serv_addr));

  // cli args are strings
  portno = atoi(argv[1]);
  // server address of which address family
  serv_addr.sin_family = AF_INET;
  // which port on server will be open
  // unsigned short int to network byte order
  serv_addr.sin_port = htons(portno);
  // ip address of the machine
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  // bind the socket to the address struct of some sizez
  // 2nd args is defined to take a ptr to sockaddr struct, casting was needed
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    close(sockfd);
    error("ERROR on binding");
  }
  // start listen on the socket for connections
  // 2nd arg is backlog queue, i.e no of waiting connections while server is
  // handling current 5 is max permited by most systems
  listen(sockfd, 5);

  // len of the client address
  clilen = sizeof(cli_addr);

  // block the exec until a client connects
  // creates a socket and return its fd
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                     (socklen_t *restrict)&clilen);
  if (newsockfd < 0) {
    close(newsockfd);
    close(sockfd);
    error("ERROR on accept");
  }

  // initiallize the buffer to zero
  bzero(buffer, BUFFER_SIZE);


  while (true) {

    // read 255 bytes from socket into the buffer
    // n is no of bytes read
    n = read(newsockfd, buffer, BUFFER_SIZE-1);
    if (n < 0) {
      close(newsockfd);
      close(sockfd);
      error("ERROR reading from socket");
    }

    if(strcmp(buffer, "end")){

    }
    printf("Bytes: %i Content: %s\n", n, buffer);
  }

 

  char msg[] = "i got your message\n";

  // write msg of some bytes to the new socket
  // n is no of bytes written
  n = write(newsockfd, msg, sizeof(msg));
  if (n < 0) {
    close(newsockfd);
    close(sockfd);
    error("ERROR writing to socket");
  }
  // program terminates after writing to socket
  close(newsockfd);
  close(sockfd);

  return 0;
}
