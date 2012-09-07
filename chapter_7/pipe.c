#include <stdio.h>

int main()
{
   int c;

   while((c = getchar()) != EOF)
      putchar(isupper(c) ? tolower(c) : c);

   return 0;
}
