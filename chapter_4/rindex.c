#include <stdio.h>

#define MAXLINE 1000

int index(char [], char []);
int getlines(char [], int);

int main()
{
  char line[MAXLINE];
  int res;
  while(getlines(line, MAXLINE) > 0){
    if((res = index(line, "the")) >= 0)
      printf("Position %d\n", res);
  }
  return 0;
}

int getlines(char s[], int lim)
{
  int c, i;
  i = 0;
  while(--lim > 0 && (c=getchar()) != EOF && c != '\n')
    s[i++] = c;
  if(c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

int index(char s[], char t[])
{
  int i, j, k, flag = 0;

  for (i = 0; s[i] != '\0'; i++)
  {
    for(j=i, k=0; t[k] != '\0' && s[j] == t[k]; j++, k++)
      ;
    if(t[k] == '\0')
      flag = i;
  }
  if (flag)
    return flag;
  return -1;
}
