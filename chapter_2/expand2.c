#include <strings.h>
#include <stdio.h>

void expand(char []);

int main()
{
  char s1[] = "a-x0-9";
  expand(s1);
  return 0;
}

void expand(char s1[])
{
  char buf[1024];
  int t = 0;
  for(int i = 0; s1[i] != EOF; i++)
  {
    if (s1[i] == '-')
      if (i > 0) {
        printf("from %c to %c\n", s1[i-1], s1[i+1]);
        for(int c = s1[i-1]; c <= s1[i+1]; c++) {
          if (buf[t-1] != c)
            buf[t++] = c;
        }
      }
  }
  buf[t] = '\0';
  printf("%s\n", buf);
}
