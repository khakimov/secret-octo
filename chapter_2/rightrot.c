#include <stdio.h>
#include <strings.h>

int rightrot(int n, int b);

int main()
{
  int res;
  res = rightrot(123, 4);
  return 0;
}

int rightrot(int n, int b)
{
  int t;
  t = n;
  n = n >> b;
  t = t << (sizeof(int) - b);
  return n | t;
}