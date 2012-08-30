#include <stdio.h>

// static int day_tab[2][13] = {
//   {0,31,28,31,30,31,30,31,31,30,31,30,31},
//   {0,31,29,31,30,31,30,31,31,30,31,30,31}
// };

static int day_tab[13] = 
  {0,31,28,31,30,31,30,31,31,30,31,30,31}
;

int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);

int main(int argc, char const *argv[])
{
  int m, d;
  month_day(2000, 60, &m, &d);
  printf("Month is %d and day %d\n", m, d);
  printf("Day of the year %d", day_of_year(2000, 3, 1));
  return 0;
}

int day_of_year(int year, int month, int day)
{
  int i, leap;
  leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  for(i = 1; i < month; i++){
    if(i == 2)
      day += day_tab[i] + leap;
    else
      day += day_tab[i];
  }
  return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
  int i, leap, t;
  leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  for(i = 1; yearday > (t = i == 2 ? day_tab[i] + 1 : day_tab[i]); i++){
    if (i == 2)
      yearday -= day_tab[i] + leap;
    else
      yearday -= day_tab[i];
  }
  *pmonth = i;
  *pday = yearday;
}