#include <stdio.h>

void itob(unsigned int n);

int main()
{
  itob(100);
  return 0;
}

void itob(unsigned int n)
{
  int i;
  char buf[32];

  for(i =0; i < 32; i++)
    buf[i] = '0';

  for(i = 31; n != 0; i--, n = n >> 1)
    buf[i] = (n & 1) ? '1' : '0';
  printf("%s\n", buf);
}


/*
100
oringinal 00000000000000000000000001100100
itob      00000000000000000000000001100100
*/