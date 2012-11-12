#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

#define MAXFILES           100
#define GET_CMD            "GET / HTTP/1.0\r\n\r\n"
char hostname[] =          "fb.com";

#define F_CONNECTING       1
#define F_READING          2
#define F_DONE             4

typedef struct {
   char *f_name;
   int f_fd[MAXFILES];
   int f_flags[MAXFILES];
   fd_set rset, wset;
   int maxfd;
} pool;

void emit(pool *p, fd_set *t, char *msg)
{
   int i;
   printf("%s [ ", msg);
   for(i = 0; i < MAXFILES; i++) {
      if(p->f_fd[i] < 0)
         continue;
      if(FD_ISSET(p->f_fd[i], t))
         printf("1 ");
      else
         printf("0 ");
   }
   printf("]\n");
}

void setnonblocking(int *fd)
{
   int flags;
   flags = fcntl(*(int *)fd, F_GETFL, 0);
   fcntl(*(int *)fd, F_SETFL, flags | O_NONBLOCK);
}

void write_get_command(int client_id, pool *p)
{
   write(p->f_fd[client_id], GET_CMD, strlen(GET_CMD));
   fprintf(stderr, "write GET\n");
   p->f_flags[client_id] = F_READING;
   FD_SET(p->f_fd[client_id], &p->rset);
   if(p->f_fd[client_id] > p->maxfd)
      p->maxfd = p->f_fd[client_id];
}


int add_client(int connfd, pool *p)
{
   int i;
   for(i = 0; i < MAXFILES; i++) {
      if(p->f_fd[i] < 0) {
         p->f_fd[i] = connfd;
         return i;
      }
   }
   return 0;
}

void init_pool(pool *p)
{
   int i;
   for(i = 0; i < MAXFILES; i++) {
      p->f_fd[i] = -1;
      p->f_flags[i] = 0;
   }
   FD_ZERO(&p->rset);
   FD_ZERO(&p->wset);

}


void start_connect(pool *p)
{
   int fd, n, client_id;
   struct sockaddr_in sin;
   struct hostent *h;

   h = gethostbyname(hostname);
   if(!h) {
      fprintf(stderr, "Coulndt resolve %s\n", hostname);
      exit(0);
   }

   fd = socket(AF_INET, SOCK_STREAM, 0);
   if(fd < 0) {
      perror("socket");
      exit(0);
   }
   setnonblocking(&fd);
   sin.sin_family = AF_INET;
   sin.sin_port = htons(80);
   sin.sin_addr = *(struct in_addr *) h->h_addr;
   client_id = add_client(fd, p); // add to array of clients
   
   if(!client_id) {
      fprintf(stderr, "Too many connection (select doesnt have space)\n");
      return;
   }

   fprintf(stderr, "start_connect for %s, fd %d\n", hostname, fd);

   if (n = connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
      if(errno != EINPROGRESS)
         fprintf(stderr, "nonblocking connect error\n");
      p->f_flags[client_id] = F_CONNECTING;
      FD_SET(fd, &p->rset);
      FD_SET(fd, &p->wset);
      if(fd > p->maxfd)
         p->maxfd = fd;
   }
   else if (n >= 0)
      write_get_command(client_id, p);
}

int main(int argc, char **argv)
{
   static pool pool;
   init_pool(&pool);
   char buf[1024];
   int w = 0, lefttoread = 10, i, flags, fd, error, n;
   fd_set rs, ws;
   FD_ZERO(&rs);
   FD_ZERO(&ws);

   while(lefttoread > 0) {
      while(w++ < 40)  
         start_connect(&pool);
      fprintf(stderr, "maxfd %d\n", pool.maxfd);
      // select(pool.maxfd + 1, &pool.rset, &pool.wset, NULL, NULL);
      printf("Before select() \t\t");
      emit(&pool, &pool.rset, "read pool.rset");
      emit(&pool, &pool.wset, "\t\t\t\twrite pool.wset");
      emit(&pool, &rs, "\t\t\t\tread rs");
      emit(&pool, &ws, "\t\t\t\twrite ws");

      rs = pool.rset;
      ws = pool.wset;


      select(pool.maxfd + 1, &rs, &ws, NULL, NULL);
      printf("\nAfter select() \t\t\t");
      emit(&pool, &pool.rset, "read pool.rset");
      emit(&pool, &pool.wset, "\t\t\t\twrite pool.wset");
      emit(&pool, &rs, "\t\t\t\tread rs");
      emit(&pool, &ws, "\t\t\t\twrite ws");

      for(i = 0; i < MAXFILES; i++) {
         flags = pool.f_flags[i];
         if(flags == 0 || flags & F_DONE)
            continue;
         fd = pool.f_fd[i];
         fprintf(stderr, "[%d] fd %d\n", i, pool.f_fd[i] );

         if(flags & F_CONNECTING && (FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {
            n = sizeof(error);
            if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &n) || error != 0) {
               fprintf(stderr, "non-blocking failed\n");
            }
            fprintf(stderr, "[connecting][%d] established connection! write!\n", i);
            FD_CLR(fd, &pool.wset);
            emit(&pool, &pool.wset, "FD_CLR ws");
            write_get_command(i, &pool);
         } else if (flags & F_READING && FD_ISSET(fd, &rs)) {
            fprintf(stderr, "[reading][%d] read \n", i);
            if ((n = read(fd, buf, sizeof(buf))) == 0) {
               close(fd);
               fprintf(stderr, "%s\n", buf);
               pool.f_flags[i] = F_DONE;
               FD_CLR(fd, &pool.rset);
               emit(&pool, &pool.rset, "FD_CLR rs");
               lefttoread--;
            } else {
               fprintf(stderr, "read %d bytes\n", n);
            }
         }
      }
      sleep(1);
   }
   return 0;
}
