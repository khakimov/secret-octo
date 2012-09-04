#include <string.h>
#include <stdio.h>

#define MAXWORD 20
#define NKEYS (sizeof(tab) / sizeof(struct key))
#define LETTER 'a'
#define DIGIT '0'

struct key {
  char *keyword;
  int keycount;
} tab[] = {
  "break", 0,
  "case", 0,
  "char", 0,
  "continue", 0
};


int binary(char *word, struct key tab[], int n);
int getword(char *w, int lim);
int type(int c);

int main()
{
  int n, t;
  char word[MAXWORD];

  while((t = getword(word, MAXWORD)) != EOF)
    if(t == LETTER)
      if((n = binary(word, tab, NKEYS)) >= 0)
        tab[n].keycount++;
  for(n = 0; n < NKEYS; n++)
    if(tab[n].keycount > 0)
      printf("%4d %s\n", tab[n].keycount, tab[n].keyword);

  return 0;
}

int binary(char *word, struct key tab[], int n)
{
  int low, high, mid, cond;
  low = 0;
  high = n - 1;

  while(low <= high){
    mid = (low + high) /2;
    if((cond = strcmp(word, tab[mid].keyword)) < 0)
      high = mid - 1;
    else if (cond > 0)
      low = mid + 1;
    else
      return mid;
  }
  return -1;
}

int getword(char *w, int lim)
{
  int c, t;
  if(type(c = *w++ = getch()) != LETTER)
  {
    *w = '\0';
    return c;
  }
  while(--lim > 0) 
  {
    t = type(c = *w++ = getch());
    if(t != LETTER && t != DIGIT)
    {
      ungetch(c);
      break;
    }
  }
  *(w - 1) = '\0';
  return LETTER;
}

int type(int c)
{
  if( c >= 'a' && c <= 'z' || c >='A' && c <= 'Z')
    return LETTER;
  if(c >= '0' & c<= '9')
    return DIGIT;
  else
    return c;
}