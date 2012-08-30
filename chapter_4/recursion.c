#include <stdio.h>

printd(int n)
{
  int i;
  if (n < 0)
  {
    putchar('-');
    n = -n;
  }
  if ((i = n/10) != 0)
    printd(i);
  putchar(n % 10 + '0');
}

int main()
{
  printd(-123);
  return 0;
}