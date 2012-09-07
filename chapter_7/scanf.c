#include <stdio.h>

int main()
{
   int i;
   float x;
   char name[10];

   scanf("%2d %f %*d %2s", &i, &x, name); 
   printf("Got 2 digits %d, float %f and name %s\n", i, x, name);
   return 0;
}   
