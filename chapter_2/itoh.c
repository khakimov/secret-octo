/*

To convert a decimal number x to hexadecimal, we can repeatedly divide x by 16,
giving a quotient q and a remainder r, such that x = q * 16 + r.

*/

#include <stdio.h>

void itoh(int n);
int convert(int s);

int main()
{

  itoh(123);
  return 0;
}

void itoh(int n)
{
  int i = 0, j =0;
  char buf[100];
  while(n != 0)
  {
    i = n % 16;
    if (i > 0)
    {
      buf[j++] = convert(i);
      n = (n - i) / 16;
    }
  }
  buf[j] = '\0';
  printf("%s\n", buf);
}

int convert(int s)
{
  int c;
  char hex[] = {'A', 'B', 'C', 'D', 'E', 'F'};
  c = (s > 0 && s < 9) ? s + '0' : hex[s - 10];
  return c;
}