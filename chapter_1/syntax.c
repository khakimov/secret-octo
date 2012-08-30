/*

printf("{[()]}"); 
but if {"} - err
if ["] - err
"[" - ok
"{" - ok
"everything inside quotes is ok, right? except one more " ;) right?"

*/


#include <stdio.h>
#include <string.h>

#define MAXPROG 10000

int is_start_symbol(char);
int is_end_symbol(char t);
int which_symbol(char t);

int main()
{
/* 
#include <stdio.h>
#include <string.h>

#define MAXPROG 10000

int main()
{
  char buf[MAXPROG];
  int c, i = 0;

  while((c = getchar()) != EOF) 
  {
    buf[i] = c;
    i++;
  }
  printf(%s\n\", buf);
  return 0;'''''
}

*/

  char buf[MAXPROG];
  char symbols[100];
  // 1 []
  // 2 ()
  // 3 {}
  // 4 ''
  // 0 ""
  // 
  int err[5], quote = 0, dquote = 0;
  int line_comment = 0, multiline = 0;
  int c, len, i = 0;

  // init err array
  for (int x = 0; x < 5; x++)
    err[x] = 0;


  while((c = getchar()) != EOF) 
  {
    buf[i] = c;
    i++;
  }

  len = (int)strlen(buf);
  for(int x = 0; x < len; x++)
  {
    if (buf[x] == '\'' && quote == 0){
      if(buf[x-1] == '\\'){
        quote = 0;
      } else {
        quote = 1;
        err[which_symbol(buf[x])] += 1;
      }
    }
    else if (buf[x] == '\'' && (quote == 1 || dquote == 1)) {
      quote = 0;
      if(buf[x-1] == '\\')
        quote = 1;
      if (quote == 0)
        err[which_symbol(buf[x])] -= 1;
    }

    if (buf[x] == '\"' && dquote == 0){
      if(buf[x-1] == '\\'){
        dquote = 0;
      } else {
        dquote = 1;
        err[which_symbol(buf[x])] += 1;
      }
    }
    else if (buf[x] == '\"' && dquote == 1) {
      dquote = 0;
      if(buf[x-1] == '\\')
        dquote = 1;
      if (dquote == 0)
        err[which_symbol(buf[x])] -= 1;
    }

    if (is_start_symbol(buf[x]) && quote == 0 && dquote == 0 && line_comment == 0 && multiline == 0){
      err[which_symbol(buf[x])] += 1;
    }

    if (is_end_symbol(buf[x]) && quote == 0 && dquote == 0 && line_comment == 0 && multiline == 0){
      err[which_symbol(buf[x])] -= 1;
    }

// if line comment
    if (buf[x] == '/' && buf[x+1] == '/')
      line_comment = 1;
    if (line_comment == 1 && buf[x] == '\n')
      line_comment = 0;
// if multiline
    if (buf[x] == '/' && buf[x+1] == '*')
      multiline = 1;
    if (buf[x] == '*' && buf[x+1] == '/' && multiline == 1)
      multiline = 0;

  }
  printf("%s\n", symbols);

  for (int x = 0; x < 5; x++)
    printf("%d\n", err[x]);
  if (multiline)
    printf("forget to close /* */ comment\n");
  return 0;
}
int which_symbol(char t)
{
  int num = 0;
  switch(t) {
  case ']':
    num = 1;
    break;

  case '[':
    num = 1;
    break;

  case '{':
    num = 3;
    break;

  case '}':
    num = 3;
    break;

  case '(':
    num = 2;
    break;

  case ')':
    num = 2;
    break;
  case '\'':
    num = 4;
    break;
  case '\"':
    num = 0;
    break;
  }
  return num;
}

int is_start_symbol(char t)
{
  if (t == '[' || t == '(' || t == '{')
    return 1;
  return 0;
}

int is_end_symbol(char t)
{
  if (t == ']' || t == ')' || t == '}')
    return 1;
  return 0;
}