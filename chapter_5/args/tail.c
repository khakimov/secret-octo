#include <stdio.h>

int atoi(char *s);

int main(int argc, char *argv[])
{
  char c, *s;
  int n = 10, i = 0;
  FILE *fp;
  // printf("argc %d and %c", argc, );
  while(--argc > 0 && *(++argv)[0] == '-') {
    for(s = argv[0] + 1; *s != '\0'; s++)
      switch(*s) {
        case 'n':
          if (argv[1] > 0)
            if (atoi(argv[1]) > 0){
              n = atoi(argv[1]);
              *++argv;
            }
          break;
        default:
          printf("Default");
          break;
      }
    }
  if (argc <= 1)
    printf("Usage: tailuy [-n lines, 10 by default] filename\n");

  fp = fopen(*argv, "r");
  if (fp == NULL) {
    printf("Error: can't find file %s\n", *argv);
    return (1);
  }

  fseek(fp, -1, SEEK_END);

  while (n > 0){
    while((c = getc(fp)) != '\n') 
      fseek(fp, -(i++), SEEK_END);
    n--;
  }

  while((c = getc(fp)) != EOF)
    printf("%c", c);

  fclose(fp);
  return 0;
}

int atoi(char *s)
{
  int i = 0, n;
  for(n = 0; s[i] >= '0' && s[i] <= '9'; i++)
    n = 10 * n + s[i] - '0';
  return n;
}