#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LINES 1000
#define MAXLEN 1000

#define ALLOCSIZE 1000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;


int readlines(char *lineptr[], int maxlines);
int Getline(char s[], int lim);
void writelines(char *lineptr[], int nlines);
void sort(char *v[], int n);
char *alloc(int n);


int main(int argc, char const *argv[])
{
  char *lineptr[LINES]; 
  int nlines;

  if ((nlines = readlines(lineptr, LINES)) >= 0)
  {
    sort(lineptr, nlines);
    writelines(lineptr, nlines);
  }
  else
    printf("input too big to sort\n");
  return 0;
}

int readlines(char *lineptr[], int maxlines)
{
  int len, nlines;
  char *p, line[MAXLEN];
  nlines = 0;

  while((len = Getline(line, MAXLEN)) > 0)
    if (nlines >= maxlines)
      return -1;
    else if ((p = alloc(len)) == NULL)
      return -1;
    else {
      line[len - 1] = '\0'; 
      strcpy(p, line);
      lineptr[nlines++] = p;
    }
    return nlines;
}

int Getline(char s[], int lim)
{
  int c, i;
  i = 0;
  while(--lim > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;

  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

void writelines(char *lineptr[], int nlines)
{
  while(--nlines >= 0)
    printf("%s\n", *lineptr++);
}

void sort(char *v[], int n)
{
  int gap, i, j;
  char *temp;

  for(gap = n/2; gap > 0; gap /= 2)
    for(i = gap; i < n; i++)
      for(j = i - gap; j >= 0; j -= gap){
        if(strcmp(v[j], v[j+gap]) <= 0)
          break;
        temp = v[j];
        v[j] = v[j+gap];
        v[j+gap] = temp;
      }
}

char *alloc(int n)
{
  if(allocp + n <= allocbuf + ALLOCSIZE) {
    allocp += n;
    return (allocp - n);
  } else 
    return NULL;
}