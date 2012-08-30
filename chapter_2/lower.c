#include <string.h>
#include <stdio.h>

int main()
{
  int c;
  while ((c = getchar()) != EOF)
    printf("%c", (c == ' ' || c == '\t' || c == '\n' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')) ? c : c + 32);

  return 0;
}

