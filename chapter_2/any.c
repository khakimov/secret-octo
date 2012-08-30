#include <stdio.h>
#include <strings.h>
#include <unistd.h>
int any(char s1[], char s2[]);


int main()
{
  char s1[] = "amigoooo";
  char s2[] = "o";
  int res;
  res = any(s1, s2);
  if (res == -1)
    printf("not found\n");
  else
    printf("first location in the string - %d\n", res);
  return 0;
}


int any(char s1[], char s2[])
{
  for(int i = 0; s1[i] != '\0'; i++) {
    for(int x = 0; s2[x] != '\0'; x++){
      if(s1[i] == s2[x])
        return i;
    }
  }
  return -1;
}