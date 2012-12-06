#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"
#include <errno.h>

#define PORT 7890

int main() {
   int sockfd, new_sockfd;
   struct sockaddr_in host_addr, client_addr;
   socklen_t sin_size;
   int recv_l=1, yes=1;
   char buffer[1024];

   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
      perror("in socket");

   if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
      perror("setting socket option SO_REUSEADDR");
 
   host_addr.sin_family = AF_INET;
   host_addr.sin_port = htons(PORT);
   host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   memset(&(host_addr.sin_zero), '\0', 8); //zero the rest of the struct

   if(bind(sockfd, (struct sockaddr *)&host_addr, sizeof(host_addr)) < 0)
      perror("binding the socket");
   
   if(listen(sockfd, 5) == -1)
      perror("listening on socket");

   while(1) {    // Accept loop
      sin_size = sizeof(struct sockaddr_in);
      new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
      if(new_sockfd == -1)
         perror("accepting connection");
      printf("server: got connection from %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
      send(new_sockfd, "Hello World!\n", 13, 0);
      recv_l = recv(new_sockfd, &buffer, 1024, 0);
      while(recv_l > 0) {
         printf("RECV: %d bytes\n", recv_l);
         dump(buffer, recv_l);
         recv_l = recv(new_sockfd, &buffer, 1024, 0);
      }
      close(new_sockfd);
   }
   return 0;
}
