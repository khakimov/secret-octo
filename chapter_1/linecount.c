#include <string.h>
#include <stdio.h>

int main()
{
  int c, t = 0, s = 0, nl = 0;
  while((c = getchar()) != EOF) {
    if (c == '\n')
      ++nl;
    if (c == ' ')
      ++s;
    if (c == '\t')
      ++t;
  }
  printf("New lines %d, tabs %d and spaces %d", nl, t, s);
  return 0;
}