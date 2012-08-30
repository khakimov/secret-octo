#include <strings.h>
#include <stdio.h>

void expand(char s[], char t[]);

int main()
{
  char s[] = "Hello\n\tAmigo Bore\n";
  char t[] = "";
  expand(s,t);
  return 0;
}

void expand(char s[], char t[])
{
  int i, x;
  for(i = 0, x = 0; s[i] != EOF; i++, x++)
  {
    switch(s[i]) {
      case '\n':
        t[i] = '|';
        break;
      case '\t':
        t[i] = '\\';
        break;
      default:
        t[i] = s[i];
        break;
    }
  }
  t[++x] = '\0';
  printf("%s\n", t);
}
