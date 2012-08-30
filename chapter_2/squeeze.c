#include <stdio.h>
#include <strings.h>
#include <unistd.h>
void squeeze(char s[], int c);
void squeeze2(char s1[], char s2[]);


int main()
{
  char s1[] = "amigoooo";
  char s2[] = "oi";
  squeeze2(s1, s2);
  return 0;
}

void squeeze(char s[], int c)
{
  int i, j;
  for(i = j = 0; s[i] != '\0'; i++)
    if (s[i] != c)
      s[j++] = s[i];
  s[j] = '\0';
  printf("%s\n", s);
}

void squeeze2(char s1[], char s2[])
{
  for(int i = 0; s2[i] != '\0'; i++) {
    int j =0; 
    for(int x = 0; s1[x] != '\0'; x++) {
      if(s2[i] != s1[x])
        s1[j++] = s1[x];
    }
    s1[j] = '\0';
  }

  printf("%s\n", s1);
}