#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#define MAXLINE 8192
#define RIO_BUFSIZE 8192

typedef struct {
   int rio_fd;
   int rio_cnt;
   char *rio_bufptr;
   char rio_buf[RIO_BUFSIZE];
} rio_t;

static ssize_t rio_read(rio_t *rp,char *usrbuf, size_t n)
{
   int cnt;
   while(rp->rio_cnt <= 0) {
      rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
      if(rp->rio_cnt < 0) {
         if(errno != EINTR)
            return -1;
      }
      else if(rp->rio_cnt == 0)
         return 0;
      else
         rp->rio_bufptr = rp->rio_buf;
   }

   cnt = n;
   if(rp->rio_cnt < n)
      cnt = rp->rio_cnt;

   memcpy(usrbuf, rp->rio_bufptr, cnt);
   rp->rio_bufptr += cnt;
   rp->rio_cnt -= cnt;
   return cnt;
}

void rio_readinitb(rio_t *rp, int fd) 
{
   rp->rio_fd = fd;
   rp->rio_cnt = 0;
   rp->rio_bufptr = rp->rio_buf;
}
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t n)
{
   size_t nleft = n;
   ssize_t nread;
   char *bufp = usrbuf;

   while(nleft > 0) {
      if ((nread = rio_read(rp, bufp, nleft)) < 0) {
         if(errno == EINTR)
            nread = 0;
         else 
            return -1;
      }
      else if(nread = 0)
         break;
      nleft -= nread;
      bufp += nread;
   }
   return (n - nleft);
}

ssize_t rio_writen(int fd, char *usrbuf, size_t n)
{
   size_t nleft = n;
   ssize_t nwritten;
   char *bufp = usrbuf;

   while(nleft > 0 )
   {
      if((nwritten = write(fd, bufp, nleft)) <= 0) {
         if(errno = EINTR)
            nwritten = 0;
         else 
            return -1;
      }
      nleft -= nwritten;
      bufp += nwritten;
   }
   return n;
}


int open_listenfd(int port)
{
   struct sockaddr_in serveraddr;
   int listenfd, optval = 1;
   
   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      return -1;

   if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int)) < 0)
      return -1;

   bzero((char *)&serveraddr, sizeof(serveraddr));
   serveraddr.sin_family = AF_INET;
   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
   serveraddr.sin_port = htons((unsigned short) port);

   if(bind(listenfd, (void *) &serveraddr, sizeof(serveraddr)) < 0)
      return -1;

   if(listen(listenfd, 1024) < 0)
      return -1;
   return listenfd;
}

void echo(int fd)
{
   int n;
   char buf[MAXLINE];

   rio_t rio;
   rio_readinitb(&rio, fd);
   while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
      printf("server received %d bytes\n", n);
      rio_writen(fd, buf, n);
   }
}

void command()
{
   char buf[MAXLINE];
   if(!fgets(buf, MAXLINE, stdin))
      exit(0);
   printf("%s\n", buf);
}

int main(int argc, char **argv)
{
   int listenfd, connfd, port, clientlen = sizeof(struct sockaddr_in);
   struct sockaddr_in clientaddr;
   fd_set read_set, ready_set;

   if(argc != 2)
   {
      fprintf(stderr, "usage: ./select <port>");
      exit(0);
   }

   port = atoi(argv[1]);
   listenfd = open_listenfd(port);

   FD_ZERO(&read_set);
   FD_SET(STDIN_FILENO, &read_set);
   FD_SET(listenfd, &read_set);
   printf("[*] while coming...\n");
   while(1) {
      ready_set = read_set;
      select(listenfd+1, &ready_set, NULL, NULL, NULL);
      printf("[*] inside while\n");
      if(FD_ISSET(STDIN_FILENO, &ready_set))
         command();
      if(FD_ISSET(listenfd, &ready_set)) {
         connfd = accept(listenfd, (void *)&clientaddr, &clientlen);
         echo(connfd);
      }
   }
   return 0;
}
