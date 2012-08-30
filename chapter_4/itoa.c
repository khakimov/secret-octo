#include <stdio.h>
#include <strings.h>

itoa(int n)
{
  int i;

  if (n < 0)
  {
    putchar('-');
    n = -n;
  }
  if ((i = n/10) != 0)
    itoa(i);
  putchar( n % 10 + '0');
}

reverse(char s[], int b)
{
  if(b < strlen(s) -1 )
    reverse(s, b + 1);
  putchar(s[b]);
}


int main()
{
  // itoa(123);
  char s[] = "123";
  reverse(s, 0);
  return 0;
}

// 72*(256^0)  +    141*(256^1)  +   77*(256^2)   +   244 *(256^3)

// 866 2772958e 01688926805