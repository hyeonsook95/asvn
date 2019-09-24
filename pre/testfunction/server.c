#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>

void server(int port)
{
  int sock;
  int reuse = 1;

  // server address
  struct sockaddr_in server_address = (struct sockaddr_in) {
    AF_INET,
    htons(port),
    (struct in_addr){INADDR_ANY}
  };

  // SOCKET 생성
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot open socket");
    exit(EXIT_FAILURE);
  }
  
  /* Address can be reused instantly after program exits */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

  /* Bind socket to server address */
  if(bind(sock,(struct sockaddr*) &server_address, sizeof(server_address)) < 0){
    fprintf(stderr, "Cannot bind socket to address");
    exit(EXIT_FAILURE);
  }

  listen(sock,5);

  printf("hello world!\n");
}

int main()
{
  server(6096);
  return 0;
}
