#include <string.h>
#include <stdio.h>

int main()
{
  int c;
  int flag, nwords, nchars;
  int hist[10];

  flag = nwords = nchars = 0;
  while((c = getchar()) != EOF)
  {
    if (c == ' ' || c == '\n' || c == '\t')
      flag = 0;
    else if (flag == 0) {
      flag = 1;
      nwords++; 
      hist[nwords] = 1;
    }
    else 
      hist[nwords] += 1; 
  }
  printf("words %d\n\n", nwords);
  for(int i = 1; i <= nwords; i++)
  {
    printf("[%d] word - ", i);
    for(int p = 0; p < hist[i]; p++)
      printf(".");
    printf("\n");
  }
  return 0;
}

