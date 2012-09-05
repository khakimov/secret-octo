#include <stdio.h>

#define ALLOCSIZE 1000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char *alloc(int n)
{
  if (allocp + n <= allocbuf + ALLOCSIZE) {
    allocp += n;
    return(allocp - n);
  } else 
      return NULL;
}