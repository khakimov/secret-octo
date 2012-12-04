#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   if ( argc < 3 ) {
      printf("Usage: ./getenvaddr <env var> <target programm name>\n");
      exit(0);
   }

   void *ptr;
   ptr = getenv(argv[1]);
   ptr += (strlen(argv[0]) - strlen(argv[2])) * 2;
   printf("%s will be at %p\n", argv[1], ptr);

   return 0;
}
