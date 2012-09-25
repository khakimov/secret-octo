#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#define RIO_BUFSIZE 8192

typedef struct {
   int rio_fd;
   int rio_cnt;
   char *rio_bufptr;
   char buf[RIO_BUFSIZE];
} rio_t;

typedef struct {
   int maxfd;
   fd_set read_set;
   fd_set ready_set;
   int nready;
   int maxi;
   int clientfd[FD_SETSIZE];
   rio_t clientrio[FD_SETSIZE];
} pool;


int byte_cnt = 0; // counts total bytes received by server

int open_listenfd(int port)
{
   int listenfd, optval = 1;
   struct sockaddr_in serveraddr;

   if(socket(AF_INET, SOCK_STREAM, 0) < 0)
      return -1;
   
   if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*) &optval, sizeof(int)) < 0)
      return -1;

   bzero((void *)&serveraddr, sizeof(serveraddr));
   serveraddr.sin_family = AF_INET;
   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
   serveraddr.sin_port = htons((unsigned int)port);
   
   if(bind(listenfd, (void *)&serveraddr, sizeof(serveraddr)) < 0)
      return -1;

   if(listen(listenfd, 1024) < 0)
      return -1;

   return listenfd;
}

void init_pool(int listenfd, pool *p)
{
   int i;
   p->maxi = -1;
   for(i=0; i < FD_SETSIZE; i++)
      p->clientfd[i] = -1;

   p->maxfd = listenfd;
   FD_ZERO(&p->read_set);
   FD_SET(listenfd, &p->read_set);
}

int main(int argc, char** argv)
{
   struct sockaddr_in clientaddr;
   static pool pool;

   int listenfd, connfd, port, clientlen = sizeof(struct sockaddr_in);

   if(argc != 2)
   {
      fprintf(stderr, "usage: ./multi <port>\n");
      exit(0);
   }
   
   port = atoi(argv[1]);
   listenfd = open_listenfd(port);
   
   init_pool(listenfd, &pool);
   while(1) {
   
   }
   

   return 0;
}
