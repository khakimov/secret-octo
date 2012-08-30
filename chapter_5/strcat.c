#include <stdio.h>
#define STRCTY(x,y) \
 while(x++ = y++) \
 ; 
void strcaty(char *s, char *t);

int main(int argc, char const *argv[])
{
  char s[100] = "Hello World!";
  char t[] = " Yahoo!";

  strcaty(s, t);
  printf("S is %s\n", s);
  STRCTY(s,t);
  return 0;
}

void strcaty(char *s, char *t)
{
  int i, j;
  i = j = 0;
  while(t[j] != '\0') 
  {
    if(s[++i] == '\0') {
      s[i] = t[j];
      j++;
    } else {
      continue;
    }
  }
  // while(s[i] != '\0')
  //   i++;
  // while((s[i++] = t[j++]) != '\0')
  //   ;
}