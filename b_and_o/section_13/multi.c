#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#define RIO_BUFSIZE 8192
#define MAXLINE 8192
typedef struct {
   int rio_fd;
   int rio_cnt;
   char *rio_bufptr;
   char rio_buf[RIO_BUFSIZE];
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

void rio_readinitb(rio_t *p, int connfd)
{
   p->rio_fd = connfd;
   p->rio_cnt = 0;
   p->rio_bufptr = p->rio_buf;
}
ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
   size_t nleft = n;
   ssize_t nwritten;
   char *bufp = usrbuf;

   while(nleft > 0) {
      if((nwritten = write(fd, bufp, nleft)) <= 0) {
         if(errno == EINTR)
            nwritten = 0;
         else
            return -1;
      }
      nleft -= nwritten;
      bufp += nwritten;
   }
   return n;
}

static ssize_t rio_read(rio_t *p, char *buf, size_t n)
{
   int cnt;

   while(p->rio_cnt <= 0) {
      p->rio_cnt = read(p->rio_fd, p->rio_buf, sizeof(p->rio_buf));
      if(p->rio_cnt < 0) {
         if(errno != EINTR) /* interrupted by sig handler return */
            return -1;
      }
      else if(p->rio_cnt == 0) /* EOF */
         return 0;
      else
         p->rio_bufptr = p->rio_buf;
   }
   cnt = n;
   if(p->rio_cnt < n)
      cnt = p->rio_cnt;
   memcpy(buf, p->rio_bufptr, cnt);
   p->rio_bufptr += cnt;
   p->rio_cnt -= cnt;
   return cnt;
}

ssize_t rio_readlineb(rio_t *p, char *usrbuf, int maxlen)
{
   int n, rc;
   char c, *bufp = usrbuf;

   for(n = 1; n < maxlen; n++) {
     if((rc = rio_read(p, &c, 1)) == 1) {
        *bufp++ = c;
        if(c == '\n')
           break;
     } else if(rc == 0) {
        if (n == 1)
           return 0;
        else 
           break;
     } else
        return -1;
   }
   *bufp = '\0';
   return n;
}

int open_listenfd(int port)
{
   int listenfd, optval = 1;
   struct sockaddr_in serveraddr;

   if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

void app_error(char *msg)
{
   fprintf(stderr, "%s\n", msg);
}
void init_pool(int listenfd, pool *p)
{
   int i;
   p->maxi = -1;
   for(i=0; i < FD_SETSIZE; i++)
   {
      // printf("[+] init pool number %d\n", i);
      p->clientfd[i] = -1;
   }

   p->maxfd = listenfd;
   FD_ZERO(&p->read_set);
   FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool *p)
{
   int i;
   p->nready--;

   for(i=0; i < FD_SETSIZE; i++)
      if(p->clientfd[i] < 0) {
         p->clientfd[i] = connfd;
         rio_readinitb(&p->clientrio[i], connfd);
         FD_SET(connfd, &p->read_set);

         if(connfd > p->maxfd)
            p->maxfd = connfd;
         if(i > p->maxi)
            p->maxi = i;
         break;
      }
   if(i == FD_SETSIZE)
      app_error("add_client error: Too many clients");
}

void check_clients(pool *p)
{
   int i, connfd, n;
   char buf[MAXLINE];
   rio_t rio;

   for(i=0; (i <= p->maxi) && (p->nready > 0); i++) {
      connfd = p->clientfd[i];
      rio = p->clientrio[i];

      if((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) {
         p->nready--;
         if((n= rio_readlineb(&rio, buf, MAXLINE)) != 0) {
            byte_cnt += n;
            printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
            rio_writen(connfd, buf, n);
         }
         else {
            close(connfd);
            FD_CLR(connfd, &p->read_set);
            p->clientfd[i] = -1;
         }
      }
   }
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
      pool.ready_set = pool.read_set;
      pool.nready = select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

      if(FD_ISSET(listenfd, &pool.ready_set)) {
         connfd = accept(listenfd, (void *) &clientaddr, &clientlen);
         add_client(connfd, &pool);  
      }
      check_clients(&pool);
   }  
   return 0;
}
