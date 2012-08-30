#include <stdio.h>
#include <string.h>

int htoi(char s[]);
int hex(char s);

int main()
{
  char buf[] = "7b";
  printf("hex %x and int %d\n", htoi(buf), htoi(buf));

  return 0;
}

int htoi(char s[])
{
  int i, n;
  n = 0;
  for(i = 0; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >= 'A' && s[i] <= 'F'); i++)
    n = 16 * n + hex(s[i]);

  return n;
}

int hex(char s)
{
  if (s == 'A' || s == 'a')
    return 10;
  if (s == 'B' || s == 'b')
    return 11;
  if (s == 'C' || s == 'c')
    return 12;
  if (s == 'D' || s == 'd')
    return 13;
  if (s == 'E' || s == 'e')
    return 14;
  if (s == 'F' || s == 'f')
    return 15;

  return s - '0';
}