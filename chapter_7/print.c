/*
 * fprintf printed immediately when
 * printf buffered and then flushed
 * that's probaly why for error messages
 * people using fprintf(stderr, "err");
 */

#include <stdio.h>

int main()
{
   int i;
   fprintf(stderr, "[");
   for(i=0; i < 10; i++)
   {
      fprintf(stderr, "=");
      sleep (1);
   }
   printf("]\n");
   return 0;
}
