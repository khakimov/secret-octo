#include <strings.h>
#include <stdio.h>

int invert(int x, int p, int n);

int main()
{
  int res;
  res = invert(3242343, 7, 4);
  return 0;
}

int invert(int x, int p, int n)
{
  int i,t;
  i = ~(~0 << p);
  t = x & i;      
  t = t | i;      
  t = (t >> (p - n)) << (p - n); 
  return x ^ t;
}

/* test
0000000000110001011110010.1100.111
0000000000110001011110010.0011.111 should

0000000000110001011110010.0011.111 got
 */