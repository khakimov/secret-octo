#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket functions
#include <netdb.h> // gethostbyname

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
   const char query[] = "GET / HTTP/1.1\r\n"
                        "Host: www.google.com\r\n"
                        "\r\n";
   const char *hostname = "www.google.com";
   struct sockaddr_in sin;
   struct hostent *h;
   const char *cp;
   int fd;

   ssize_t n_written, remaining;
   char buf[1024];

   h = gethostbyname(hostname);
   if(!h) {
      fprintf(stderr, "Couldnt lookup %s: %s", hostname, hstrerror(h_errno));
      exit(0);
   }

   if (h->h_addrtype != AF_INET) {
      fprintf(stderr, "No ipv6 support\n");
      exit(0);
   }

   fd = socket(AF_INET, SOCK_STREAM, 0);
   if (fd < 0) {
      perror("socket");
      exit(0);
   }

   sin.sin_family = AF_INET;
   sin.sin_port = htons(80);
   sin.sin_addr = *(struct in_addr *) h->h_addr;
   if(connect(fd, (struct sockaddr*) &sin, sizeof(sin))) {
      perror("connect");
      close(fd);
      exit(0);
   }

   cp = query;
   remaining = strlen(query);
   while(remaining) {
      n_written = send(fd, cp, remaining, 0);
      if ( n_written <= 0) {
         perror("send");
         exit(0);
      }
      remaining -= n_written;
      cp += n_written;
   }

   while(1) {
      ssize_t result = recv(fd, buf, sizeof(buf), 0);
      fprintf(stderr, "Result is %d\n", (int)result);
      if (result == 0) {
         fprintf(stderr, "NULL\n");
         break;
      } else if (result < 0) {
         perror("recv");
         close(fd);
         exit(0);
      }
      fwrite(buf, 1, result, stdout);
   }
   close(fd);
   return 0;
}
