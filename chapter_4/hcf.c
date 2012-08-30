#include <stdio.h>
int HCF(int n, int m)
{
  int Remainder;
  Remainder = n % m;
  if (Remainder != 0)
    return HCF(m, Remainder);
  else
    return m;
}

int Factorial(int n)
{
  printf("%d\n", n);
  if (n == 1)
    return n;
  else 
    return n * Factorial(n - 1);
}

int Reverse(int a[], int n)
{
  (n == 0) ? printf("%d\n", a[n]) : printf("%d,", a[n]);
  if (n == 0)
    return 0;
  else 
    return Reverse(a, n -1);
}

int main()
{
  int i;
  int a[] = {1,2,3,4,5,6,7,8,9};
  printf("Reverse to display an array:\n");
  Reverse(a, 8);
  i = HCF(1470, 693);
  printf("Factorial 5! = %d\n", Factorial(5));
  printf("Greates common devisor 1470 and 693 %d\n", i);
  int n;
  n = 3, i;
  printf("%d\n", n);
  return 0;
}