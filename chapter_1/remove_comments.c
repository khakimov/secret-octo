#include <stdio.h>
#include <string.h>

int is_comment(char []);
int main()
{
  char test_comment[] = "superCfunction(\'privet//asd\'); // privet";
  char test_comment2[] = "superCfunction(\"privet//asd\"); // privet";
  char test_comment3[] = "superCfunction("/**/"); /* commment */ asd";

  is_comment(test_comment);
  is_comment(test_comment2);
  is_comment(test_comment3);


  return 0;
}
int is_comment(char str[])
{

  int len = strlen(str);
  int quoted_flag = 0, slash = 0;
  char buf[len];

  for (int i = 0; i < len; i++)
  {
    if ((str[i] == '\"' || str[i] == '\'') && quoted_flag == 0) {
      quoted_flag = 1;
      i++;
    }
    if ((str[i] == '\"' || str[i] == '\'') && quoted_flag == 1)
      quoted_flag = 0;
    
    if (str[i] == '/' && quoted_flag == 0) {
      if(str[i + 1] == '/')
        printf("comment detected \"%s\"\n", str+i);
      if(str[i + 1] == '*') {
        slash = (i > 0) ? i : 1;
        i += 2;
      }
    }
    if (str[i] == '*' && slash > 0) 
    {
      strncpy(buf, str + slash - 1, i - slash + 3);
      printf("comment detected %s\n", buf);
    }
  }
  return 0;
}
