#include <strings.h>
#include <stdio.h>
int getbits(unsigned int x, unsigned int p, unsigned int n);
int main()
{
  unsigned int c = getbits(158, 25, 3);
  printf("%u",c);
  return 0;
}

int getbits(unsigned int x, unsigned int p, unsigned int n)
{
  int i1 = x >> (32 - p - n - 1);
  int i2 = ~(~0 << n);
  int i3 = i1 & i2;
  int i4 = ~i3;
  if (i1 || i2 || i3 || i4)
    ;
  return ((x >> (32 - p - n - 1)) & ~(~0 << n));
}


/*

for(; *s != '\0'; s++)
  *s = tolower(*s);
*/