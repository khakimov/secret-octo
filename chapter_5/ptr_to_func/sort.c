#include <stdio.h>
#include <string.h>

#define LINES 1000

void sort(char *v[], int n, int r, int d, int(*comp)(), void(*exch)());
char *alloc(int n);
int Getline(char s[], int lim);
int numcmp(char *s1, char *s2);
void swap(char *px[], char *py[]);
double atof(char *s);
void writelines(char *lineptr[], int nlines);
int readlines(char *lineptr[], int maxlines);

int main(int argc, char const *argv[])
{
  char *lineptr[LINES];
  int nlines;
  int numeric = 0, reverse = 0, dictionary = 0;

  while(argc > 0) 
  {
    if (*argv[0] == '-' && argv[0][1] == 'n'){
      numeric = 1;
    }

    if (*argv[0] == '-' && argv[0][1] == 'r'){
      reverse = 1;
    }

    if (*argv[0] == '-' && argv[0][1] == 'd'){
      dictionary = 1;
    }

    argc--;
    argv++;
  }

  if((nlines = readlines(lineptr, LINES)) >= 0) {
    if(numeric)
      sort(lineptr, nlines, reverse, dictionary, numcmp, swap);
    else
      sort(lineptr, nlines, reverse, dictionary, strcmp, swap);
    writelines(lineptr, nlines);
  } else {
    printf("input too big to sort\n");
  }

  return 0;
}



int numcmp(char *s1, char *s2)
{
  double v1, v2;
  v1 = atof(s1);
  v2 = atof(s2);

  if(v1 < v2)
    return(-1);
  else if (v1 > v2)
    return 1;
  else
    return 0;
}

void swap(char *px[], char *py[])
{
  char *temp;

  temp = *px;
  *px = *py;
  *py = temp;
}

// convert string to float
double atof(char *s)
{
  double val, power;
  int i, sign;

  for(i=0; s[i] == ' ' || s[i] == '\n' || s[i] == '\t'; i++)
    ;

  sign = 1;
  
  if(s[i] == '+' || s[i] == '-')
    sign = (s[i++] == '+') ? 1 : -1;
  
  for(val=0; s[i] >= '0' && s[i] <= '9'; i++)
    val = 10 * val + s[i] - '0'; // calc till . if it's float
  
  if(s[i] == '.')
    i++; // increase i and keep calculation

  for(power = 1; s[i] >= '0' && s[i] <= '9'; i++) {
    val = 10 * val + s[i] - '0';
    power *= 10; // calc power and then divide
  }

  return (sign * val / power);
}

void writelines(char *lineptr[], int nlines)
{
  while(--nlines >= 0)
    printf("%s\n", *lineptr++);
}

int readlines(char *lineptr[], int maxlines)
{
  int len, nlines;
  char *p, line[LINES];

  nlines = 0;
  while((len = Getline(line, LINES)) > 0)
    if (nlines >= maxlines)
      return -1;
    else if ((p = alloc(len)) == NULL)
      return -1;
    else {
      line[len - 1] = '\0';
      strcpy(p, line);
      lineptr[nlines++] = p;
    }
    return nlines;
}

int Getline(char *s, int lim)
{
  int c, i;
  i = 0;
  while(--lim > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if(c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

void sort(char *v[], int n, int r, int d, int(*comp)(), void(*exch)())
{
  int gap, i, j;

  for(gap = n/2; gap > 0; gap /= 2)
    for(i = gap; i < n; i++)
      for(j = i - gap; j >= 0; j -= gap) {
        if (r == 1) {
          if ((*comp)(v[j], v[j+gap]) > 0)
            break;
        }
        
        if (d == 1) {
          if (*v[j] >= '0' && *v[j] <= '9' && *v[j] >= 'a' && *v[j] <= 'Z' && *v[j] == ' '){
            if ((*comp)(v[j], v[j+gap]) <= 0) { break; }
          } else {
            break;
          }
        } 
        
        if ((*comp)(v[j], v[j+gap]) <= 0)
          break;
        
        (*exch)(&v[j], &v[j+gap]);
      }
}