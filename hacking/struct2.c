/*
Desicription:

struct tm {
   int tm_sec;
   int tm_min; 
   int tm_hour; 
   int tm_mday; 
   int tm_mon; 
   int tm_year; 
   int tm_wday; 
   int tm_yday; 
   int tm_isdst;
};
sizeof(tm) is 56

18 00 00 00 2d 00 00 00 11 00 00 00 06 00 00 00 
08 00 00 00 70 00 00 00 04 00 00 00 f9 00 00 00 
01 00 00 00 00 00 00 00 90 9d ff ff ff ff ff ff 
18 35 80 39 86 7f 00 00 

int_ptr @ 0x140734847612168 : 24 <- 18 00 00 00 little endian is 00 00 00 18 (1*16 + 8)
int_ptr @ 0x140734847612172 : 45 <- 2d 00 00 00 l-endian 00 00 00 2d (2*16 + 13)
int_ptr @ 0x140734847612176 : 17 <- 11 00 00 00 l-endian 00 00 00 11 (1*16 + 1)

*/
#include <stdio.h>
#include <time.h>

void dump_time_struct_bytes(struct tm *time_ptr, int size)
{
   int i;
   unsigned char *raw_ptr;
   printf("bytes of struct located at %p or 0x%lx and sizeof %d\n", time_ptr, (long unsigned int) time_ptr, size);
   raw_ptr = (unsigned char *) time_ptr;

   for(i=0; i < size; i++)
   {
      printf("%02x ", raw_ptr[i]);
      if(i%16 == 15)
         printf("\n");
   }
   printf("\n");
}

int main()
{
   long int seconds_since_epoch;
   struct tm current_time, *time_ptr;
   int hour, minute, second, i, *int_ptr;

   seconds_since_epoch = time(0);
   printf("time() - seconds since epoch: %ld\n", seconds_since_epoch);
   time_ptr = &current_time;

   localtime_r(&seconds_since_epoch, time_ptr);

   hour = current_time.tm_hour;
   minute = time_ptr->tm_min;
   second = *((int *) time_ptr);

   printf("Current tim is: %02d:%02d:%02d\n", hour, minute, second);

   dump_time_struct_bytes(time_ptr, sizeof(struct tm));

   minute = hour = 0;
   int_ptr = (int *) time_ptr;

   for(i=0; i < 3; i++)
   {
      printf("int_ptr @ 0x%12ld : %d\n", (long unsigned int) int_ptr, *int_ptr);
      int_ptr++;
   }
}
