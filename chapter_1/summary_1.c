#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define TAB_STOP 50

void ReplaceTabs(char str[], int len);
void header();

int main()
{
  int c, i;
  char buf[1000];
  
  i = 0;

  while((c = getchar()) != EOF) {
    if (c == '\n' && i > 0) {
      buf[i] = '\0';
      ReplaceTabs(buf, i);
      i = 0;
    } else {
      buf[i] = c; i++;
    }
  }

  return 0;
}

void ReplaceTabs(char str[], int len)
{
  header();
  if (len < TAB_STOP) {
    int tabs;
    tabs = 0;
    for(int i = 0; i < len; i++) 
      if (str[i] == '\t' || str[i] == ' ')
        tabs++;
    int left;
    left = TAB_STOP - len;
    left = left/tabs;
    for(int i = 0; i < len; i++) {
      if (str[i] == ' '){
        for(int i = 0; i < left; i++)
          printf("_");
      }
      putchar(str[i]);
    }
  printf("\n\nLeft chars %d, replace spaces by tabs %d, * %d\n", TAB_STOP - len, (TAB_STOP - len)/4, ((TAB_STOP - len)/4)/tabs);
  header();
  }

  if (len > TAB_STOP){
    char buf[1000];

    for(int i = 0; i < len; i++){
      if (!(i % TAB_STOP)) {
        strncpy(buf+i, str+i, TAB_STOP);
        if(i != 0)
          buf[i] = '\n';
      }
    }
    printf("%s\n", buf);
  }

}

void header()
{
  for(int i = 0; i < TAB_STOP; i++)
    printf("*");
  printf("\n");
}