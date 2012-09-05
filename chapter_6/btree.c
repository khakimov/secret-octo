#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXWORD 20
#define LETTER 'a'
#define DIGIT '0'

struct tnode {
  char *word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

void treeprint(struct tnode *p);
struct tnode *tree(struct tnode *p, char *w);
int getword(char *w, int lim);
char *strsave(char *s);

int main()
{
  struct tnode *root, *tree();
  char word[MAXWORD];
  int t;

  root = NULL;
  while((t = getword(word, MAXWORD)) != EOF)
    if (t == LETTER)
      root = tree(root, word);
  treeprint(root);

  return 0;
}

struct tnode *tree(struct tnode *p, char *w)
{
  struct tnode *talloc();
  char *strsave();
  int cond;

  if (p == NULL) {
    p = talloc();
    p->word = strsave(w);
    p->count = 1;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(w, p->word)) == 0)
      p->count++;
    else if (cond < 0)
      p->left = tree(p->left, w);
    else
      p->right = tree(p->right, w);
    return p;
}

void treeprint(struct tnode *p)
{
  if(p != NULL){
    treeprint(p->left);
    printf("%4d %s\n", p->count, p->word);
    treeprint(p->right);
  }
}

char *strsave(char *s)   /* notice type is BELOW header; this is common in C */
{
 char *p;
 if ((p = (char *) malloc(strlen(s)+1)) != NULL)
     strcpy(p, s);
 return p;
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


struct tnode *talloc()
{
  char *alloc();
  return ((struct tnode *) alloc (sizeof(struct tnode)));
}