#include <stdio.h>

#define ALLOCSIZE 1000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char *alloc(int n);
void free(char *p);
int main()
{
  char *p;
  p = alloc(10);
    printf("address is: %p\n", p, *p);
  p = alloc(100); 
    printf("address is: %p\n", p, *p);
  p = alloc(900); 
  if (p == NULL){
    printf("Not enought space. Sorry\n");
  } else {
    printf("address is: %p\n", p, *p);
  }

  free(p);
  p = alloc(800); 
    printf("address is: %p\n", p, *p);



  return 0;
}

char *alloc(int n)
{
  if(allocp + n <= allocbuf + ALLOCSIZE) {
    allocp += n;
    return (allocp - n);
  } else 
    return NULL;
}

void free(char *p)
{
  if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
    allocp = p;
}