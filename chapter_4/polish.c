#include <stdio.h>

#define MAXOP 20
#define NUMBER '0'
#define TOOBIG '9'

int getop(char s[], int lim);

int main()
{
  int type;
  char s[MAXOP];
  double op2, atof(), pop(), push();



  while((type = getop(s, MAXOP)) != EOF)
    switch(type) {

      case NUMBER:
        push(atof(s));
        break;
      case '+':
        push(pop() + pop());
        break;
      case '*':
        push(pop() * pop());
        break;
      case '-':
        op2 = pop();
        push(pop() - op2);
        break;
      case '/':
        op2 = pop();
        if(op2 != 0.0)
          push(pop() / op2);
        else
          printf("zero devisor popped\n");
        break;
      case '=':
        printf("\t%f\n", push(pop()));
        break;
      case 'c':
        clear();
        break;
      case 'g':
        ungets();
        break;
      case TOOBIG:
        printf("%.20s ... is to long\n", s);
        break;
      default:
        printf("unknown command %c\n", type);
        break;
    }
}

int getop(char s[], int lim)
{
  int i, c;

  while((c = getch()) == ' ' || c == '\t' || c == '\n')
    ;
  if(c != '.' && (c < '0' || c > '9'))
    return(c);
  s[0] = c;
  for(i = 1; (c = getchar()) >= '0' && c <= '9'; i++)
    if(i < lim)
      s[i] = c;
  if(c == '.') 
  {
    if (i < lim)
      s[i] = c;
    for(i++; (c = getchar()) >= '0' && c <= '9'; i++)
      if(i<lim)
        s[i] = c;
  }
  if(i < lim)
  {
    ungetch(c);
    s[i] = '\0';
    return(NUMBER);
  } else {
    while(c != '\n' && c != EOF)
      c = getchar();
    s[lim - 1] = '\0';
    return(TOOBIG);
  }
}

ungets()
{
  printf("a");
}