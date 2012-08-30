#include <stdio.h>

// double atof(char s[]);

// int main()
// {
//   double res;
//   res = atof(" \n\t-1");
//   printf("%.2f\n", res);
//   return 0;
// }

double atof(char s[])
{
  double val, power;
  int i, sign;
  for(i = 0; s[i] == ' ' || s[i] == '\n' || s[i] == '\t'; i++)
    ; // wtf and how it works?!
  sign = 1;
  if (s[i] == '+' || s[i] == '-') // sign
    sign = (s[i++] == '+') ? 1: -1;
  for(val = 0; s[i] >= '0' && s[i] <= '9'; i++)
    val = 10 * val + s[i] - '0';
  if (s[i] == '.')
    i++;
  for(power = 1; s[i] >= '0' && s[i] <= '9'; i++){
    val = 10 * val + s[i] - '0';
    power *= 10;
  }  

  return (sign * val / power);
}
