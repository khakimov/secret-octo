#include <string.h>
#include <stdio.h>

#define MAXLINE 1000

int getstaticline(char line[], int s);
void copy(char s1[], char s2[]);

int main()
{
  int len;
  int max;
  char line[MAXLINE];
  char save[MAXLINE];

  max = 0;

  while((len = getstaticline(line, MAXLINE)) > 0)
    if (len < 80){
      printf("Too small...\n");
    }
    else if (len > max) {
      printf("%s\n", line);

      max = len;
      copy(line, save);
    }

  if(max > 80)
    printf("%s\n", save);

  return 0;
}

int getstaticline(char line[], int s)
{
  int c, i;
  for(i = 0; i< s && (c = getchar()) != EOF && c != '\n'; ++i)
    line[i] = c;
  if (c == '\n') {
    line[i] = c;
    ++i;
  }
  line[i] = '\0';
  return i;
}


void copy(char s1[], char s2[])
{
  int i;
  i = 0;
  while((s2[i] = s1[i]) != '\0')
    ++i;
}