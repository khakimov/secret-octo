#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define RIO_BUFSIZE 8192
#define MAXLINE 1024

typedef struct {
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
} rio_t;

void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    while(rp->rio_cnt <= 0) {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if(rp->rio_cnt < 0) {
            if(errno != EINTR)
                return -1;
        }
        else if (rp->rio_cnt == 0)
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

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = usrbuf;

    for(n = 1; n < maxlen; n++){
        if((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if(c == '\n')
                break;
        } else if (rc == 0) {
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

ssize_t rio_writen(int fd, char *usrbuf, size_t n)
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

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXLINE];

    sprintf(body, "<html><title>Tiny Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Tiny Web Server</em>\r\n", body);

    sprintf(buf, "HTTP/2.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    rio_readlineb(rp, buf, MAXLINE);
    while(strcmp(buf, "\r\n"))
        rio_readlineb(rp, buf, MAXLINE);
    return;
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if(!strstr(uri, "dyn")) {
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if(uri[strlen(uri) - 1] == '/')
            strcat(filename, "home.html");
        return 1;
    }
    else {
        ptr = index(uri, '?');
        if(ptr) {
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        }
        else 
            strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcpy(filename, uri);
        return 0;
    }
}
void get_filetype(char *filename, char *filetype)
{
    if(strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if(strstr(filename, ".gif"))
        strcpy(filetype, "text/gif");
    else if(strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}

void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXLINE];

    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/2.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    rio_writen(fd, buf, strlen(buf));

    srcfd = open(filename, O_RDONLY, 0);
    srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    close(srcfd);
    rio_writen(fd, srcp, filesize);
    munmap(srcp, filesize);
}

void serve_dynamic(int fd, char *filename, char *args)
{
    int add = 0;
    add = atoi(args);
    add += 100; 
    char body[MAXLINE];
    sprintf(body, "%d", add);
    printf("add %d args %s and sizeof %lu\n", add, args, sizeof(args));
    char buf[MAXLINE];
    sprintf(buf, "HTTP/2.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-type: text/html\r\n\r\n", buf);
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}


void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    rio_readinitb(&rio, fd);
    rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }
    read_requesthdrs(&rio);

    is_static = parse_uri(uri, filename, cgiargs);
    
    if((stat(filename, &sbuf) < 0) && is_static){
        clienterror(fd, filename, "404", "Not found", "Tiny coulnd't find this file");
        return;
    }
    
    if(is_static) {
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Tiny Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size);
    }
    else {
        serve_dynamic(fd, filename, cgiargs);
    }
}

/*
    Create socket
    Eliminates error if address already in use
    Listen
    Return listenfd
*/
int open_listenfd(int port)
{
    int listenfd, optval = 1;
    struct sockaddr_in serveraddr;

    /* Create socket */
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* If already in use than error */
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0)
        return -2;

    bzero((void *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if(bind(listenfd, (void *)&serveraddr, sizeof(serveraddr)) < 0)
        return -3;

    if(listen(listenfd, 1024) < 0)
        return -4;
    return listenfd;
}

int main(int argc, char **argv)
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    if(argc != 2) {
        fprintf(stderr, "usage: ./tiny <port>\n");
        exit(1);
    }

    port = atoi(argv[1]);

    listenfd = open_listenfd(port);
    if(listenfd < 0) {
        fprintf(stderr, "something wrong (error code is %d)\n", listenfd);
        exit(1);
    }

    
    while(1) {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd, (void *)&clientaddr, &clientlen);
        printf("[connected] %s\n", inet_ntoa(clientaddr.sin_addr));
        doit(connfd);
        close(connfd);
    }
    return 0;
}