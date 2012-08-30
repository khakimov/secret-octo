#include <stdio.h>

void qsort(int []);
void printIntArray(int []);
int main()
{
  int s[] = {1,23,5,6,22,4};
  printIntArray(s);
  printf("done\n");

  qsort(s);
  return 0;
}

void qsort(int s[])
{
  int i, j, lowest, temp;

  for(i = 0; i < sizeof(s); i++){
    for(j = 0; j < sizeof(s); j++)
    {
      if(s[i] > s[j]) 
        lowest = j;
    }
    temp = s[i];
    s[i] = s[lowest];
    s[lowest] = temp;
  }

  printIntArray(s);
}

void printIntArray(int s[])
{
  for(int i = 0; i< sizeof(s) - 1; i++)
    printf("%d\n", s[i]);
}
