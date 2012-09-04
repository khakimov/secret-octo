#include <stdio.h>
#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

getch()
{
  return((bufp > 0) ? buf[--bufp] : getchar());
}
ungetch(int c)
{
  if (bufp > BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}

