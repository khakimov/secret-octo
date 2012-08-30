#include <string.h>
#include <stdio.h>

#define MAXLINE 1000
char line[MAXLINE];
char save[MAXLINE];
int max;

int getfline();
void copy();

int main()
{
  int len;
  extern int max;
  extern char save[];

  max = 0;
  while((len = getfline()) > 0)
    if (len > max) {
      max = len;
      copy();
    }
  if(max > 0)
    printf("%s", save);

  return 0;
}

int getfline()
{
  int c, i;
  extern char line[];
  i = 0;
  while((c = getchar()) != EOF) {
    line[i] = c;
    if (c == '\n')
      break;
    ++i;
  }
  line[i] = '\0';
  return(i);
}

void copy()
{
  int i;
  extern char line[], save[];

  i = 0;
  while((save[i] = line[i]) != '\0')
    ++i;
}