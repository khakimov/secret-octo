#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void *errorchecking_malloc(unsigned int);

int main(int argc, char *argv[])
{
   char *char_ptr, *char_ptr2;
   int *int_ptr;
   int mem_size;

   if (argc < 2)
      mem_size = 50;
   else
      mem_size = atoi(argv[1]);

   printf("\t[+] allocation %d bytes of memory on the heap for char_ptr\n", mem_size);
   char_ptr = (char *) errorchecking_malloc(mem_size);
   // test
   //
   char_ptr2 = (char *) errorchecking_malloc(mem_size);

   strcpy(char_ptr, "This is memory is located on the heap.");
   printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);
   printf("char_ptr (%p) --> '%s'\n", char_ptr2, char_ptr2);
   printf("\t[+] allocation 12 bytes of memory on the heap for int_ptr\n");
   int_ptr = (int *) errorchecking_malloc(12);

   *int_ptr = 31337;
   printf("int_ptr (%p) --> %d\n", int_ptr, *int_ptr);

   printf("\t[-] freeing char_ptr's heap memory...\n");
   free(char_ptr);

   printf("\t[+] allocation another 15 bytes for char_ptr\n");
   char_ptr = (char *) errorchecking_malloc(15);

   strcpy(char_ptr, "new memory");
   printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);
   printf("\t[-] freeing int_ptr's heap memory...\n");
   free(int_ptr);
   printf("\t[-] freeing char_ptr's heap memory...\n");
   free(char_ptr);
   free(char_ptr2);
   return 0;
}

void *errorchecking_malloc(unsigned int p)
{  
   void *ptr;
   ptr = malloc(p);
   if(ptr == NULL)
   {
      fprintf(stderr, "Eroor: could not allocate heap memory.\n");
      exit(-1);
   }   
   return ptr;
}
