#include <string.h>
#include <stdio.h>
int lower(int);

int main()
{
  int c;
  while ((c = getchar()) != EOF)
    printf("%c", lower(c));

  return 0;
}

int lower(int c)
{
  if (c == ' ' || c == '\t' || c == '\n' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z'))
    return c;
  return c + 32;
}