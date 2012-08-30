#include <stdio.h>

#define SIZE 100
int getint(int *pn);

int main()
{
  int n, v, array[SIZE];

  for(n = 0; n < SIZE && getint(&v) != EOF; n++)
    array[n] = v;

  int i;
  for(i = 0; array[i] != 0; i++)
    printf("%d\n", array[i]);
  return 0;
}

float getint(int *pn)
{
  int sign;
  float c;
  
  while((c = getch()) == ' ' || c == '\n' || c == '\t')
    ;
  sign = 1;
  
  if (c == '+' || c == '-') {
    sign = (c == '+') ? 1 : -1;
    c = getch();
  }

  for(*pn = 0; c >= '0' && c <= '9'; c = getch())
    *pn = 10 * *pn + c - '0';

  *pn *= sign;
  return c;
}