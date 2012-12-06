#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
int count = 1;
int clientfd[FD_SETSIZE];
fd_set read_set, write_set;
#define     GET_CMD           "GET %s HTTP/1.0\r\n\r\n"

char hostname[] = "fb.com";

int         nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
fd_set      rset, wset;

#define MAXFILES 100
#define MAXLINE 1024

#define F_CONNECTING          1
#define F_READING             2
#define F_DONE                4

struct file {
      char *f_name;
      int f_fd;
      int f_flags;
} file[MAXFILES];

ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
      ssize_t           n;

      if ( (n = read(fd, ptr, nbytes)) == -1)
            fprintf(stderr, "read error\n");
      return(n);
}

ssize_t                                   /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
      size_t            nleft;
      ssize_t           nwritten;
      const char  *ptr;

      ptr = vptr;
      nleft = n;
      while (nleft > 0) {
            if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
                  if (nwritten < 0 && errno == EINTR)
                        nwritten = 0;           /* and call write() again */
                  else
                        return(-1);             /* error */
            }

            nleft -= nwritten;
            ptr   += nwritten;
      }
      return(n);
}
/* end writen */

void
Writen(int fd, void *ptr, size_t nbytes)
{
      if (writen(fd, ptr, nbytes) != nbytes)
            printf("writen error\n");
}

void write_get_cmd(struct file *fptr)
{
      int n;
      char line[MAXLINE];

      // n = sprintf(line, sizeof(line), GET_CMD, fptr->f_name);
      n = snprintf(line, sizeof(line), GET_CMD, fptr->f_name);
      fprintf(stderr, "%s\n", line);
      Writen(fptr->f_fd, line, n);
      fprintf(stderr, "wrote %d bytes for %s\n", n, hostname);
      fptr->f_flags = F_READING; // clear F_CONNECTING
      FD_SET(fptr->f_fd, &rset); // will read server's reply
      if(fptr->f_fd > maxfd)
            maxfd = fptr->f_fd;
}

void start_connect(struct file *fptr)
{
      int fd, flags, n;
      struct sockaddr_in sin;
      struct hostent *h;
      h = gethostbyname(hostname);
      if(!h) {
            fprintf(stderr, "Coulndt resolv %s\n", hostname);
            exit(0);
      }

      fd = socket(AF_INET, SOCK_STREAM, 0);
      if (fd < 0){
            perror("socket");
            exit(0);
      }

      sin.sin_family = AF_INET;
      sin.sin_port = htons(80);
      sin.sin_addr = *(struct in_addr *)h->h_addr;
      fptr->f_fd = fd;
      fprintf(stderr, "start_connect for %s, fd %d\n", hostname, fd);

      /* set non-block */
      flags = fcntl(fd, F_GETFL, 0);
      fcntl(fd, F_SETFL, flags | O_NONBLOCK);

      if ( n = connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
            if (errno != EINPROGRESS)
                  fprintf(stderr, "nonblocking connect error\n");
            fptr->f_flags = F_CONNECTING;
            FD_SET(fd, &rset);
            FD_SET(fd, &wset);
            if(fd > maxfd)
                  maxfd = fd;
      } else if (n >= 0) // connect if already done
            write_get_cmd(fptr); // write the GET command to http serv
}


int main(int argc, char **argv)
{
      int maxnconn, i, error, n, fd, flags;
      char buf[MAXLINE];
      fd_set rs, ws;

      if(argc < 2) {
            printf("usage: %s #conns\n", argv[0]);
            exit(0);
      }
      maxnconn = atoi(argv[1]);
      
      nfiles = MAXFILES;

      for( i = 0; i < MAXFILES; i++) {
            file[i].f_name = "/";
            file[i].f_flags = 0;
      }

      FD_ZERO(&rs);
      FD_ZERO(&ws);

      maxfd = -1;
      nlefttoread = nlefttoconn = nfiles;
      nconn = 0;
      while(nlefttoread > 0) {
            while(nconn < maxnconn && nlefttoconn > 0) {
                  for(i = 0; i < nfiles; i++)
                        if(file[i].f_flags == 0)
                              break;
                  if(i == nfiles)
                        fprintf(stderr, "nlefttoconn = %d but nothing found\n", nlefttoconn);
                  start_connect(&file[i]);
                  //FD_SET(fd, &rset);
                  //FD_SET(fd, &wset);
                  nconn++;
                  nlefttoconn--;
            }

            rs = rset;
            ws = wset;
            n = select(maxfd + 1, &rs, &ws, NULL, NULL);

            for(i = 0; i < nfiles; i++) {
                  flags = file[i].f_flags;
                  if(flags == 0 || flags & F_DONE)
                        continue;
                  fd = file[i].f_fd;
                  if(flags & F_CONNECTING && (FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {
                        n = sizeof(error);
                        if ( getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &n) < 0 || error != 0) {
                              fprintf(stderr, "nonblocking connect failed\n");
                        }
                        fprintf(stderr, "connection established\n");
                        FD_CLR(fd, &wset); // no more writeability test
                        write_get_cmd(&file[i]);
                        //      FD_SET(fptr->f_fd, &rset); // will read server's reply
                  } else if (flags & F_READING && FD_ISSET(fd, &rs)) {
                        if((n = Read(fd, buf, sizeof(buf))) == 0) {
                              fprintf(stderr, "end-of-file\n");
                              close(fd);
                              file[i].f_flags = F_DONE; // clear F_READING
                              FD_CLR(fd, &rset);
                              nconn--;
                              nlefttoread--;
                        } else {
                              fprintf(stderr, "read %d bytes\n", n);
                        }
                  }
            }

      }
      return 0;
}