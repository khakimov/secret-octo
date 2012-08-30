#include <string.h>
#include <stdio.h>

void reverse(char s[])
{
  // printf("%s, %d", s, (int)strlen(s) -1);
  int len;
  len = (int)strlen(s) -1;
  for (int i = len; i > 0; i--)
    printf("%c", s[i]);
  printf("\n");
}

int main() 
{
  int c, i;
  char s[1000];
  i = 0;

  while((c = getchar()) != EOF) {
    if (c == '\n') {
      s[i] = '\0';
      reverse(s);
      i = 0;
    }
    s[i] = c;
    i++;
  }
  return 0;
}