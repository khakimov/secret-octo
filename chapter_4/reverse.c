#include <string.h>
#include <stdio.h>
#define swap(x, y) x ^= y; y ^= x; x ^= y;

void reverse(char *s, int);

int main()
{
  int a = 10, b = 3;
  swap(a,b);
  printf("%d and %d\n", a, b);
  char *s = "Test";
  reverse(s, 0);
  return 0;
}

void reverse(char s[], int n)
{
  if(n != strlen(s) - 1)
    reverse(s, n+1);
  printf("%c\n", s[n]);
}