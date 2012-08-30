#include <stdio.h>

int sizeOfInt();

int main()
{
  printf("sizeof (int) on this machine is %d\n", sizeOfInt());
  return 0;
}

int sizeOfInt()
{
  int i, buf = 0;
  buf = buf | ~0;

  while(buf != 0)
  {
    buf <<= 1;
    i++;
  }
  return i;
}
