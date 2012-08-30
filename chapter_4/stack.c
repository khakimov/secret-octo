#include <stdio.h>

#define MAXVAL 100

int sp = 0;
double val[MAXVAL];

double push(double f)
{

  if (sp < MAXVAL)
    return(val[sp++] = f);
  else {
    printf("erro: stack full\n");
    clear();
    return(0);
  }
}

double pop()
{
  if (sp > 0)
    return(val[--sp]);
  else {
    printf("error: stack empty\n");
    clear();
    return 0;
  }
}

clear()
{
  sp = 0;
}

