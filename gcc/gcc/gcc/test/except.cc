#include <stdio.h>
#include <stdlib.h>

struct smart {
  int month, day, year;
};

class date {
private:
  int month, day, year;
  char *name;
public:
  int foo (void);
  int set (int, int, int);
  void get (int *, int *, int *);
};

void do_something (date d)
{
  printf ("do_something\n");
  exit (0);
}

void something_else (char *s)
{
  printf ("something_else\n");
  exit (1);
}

int foo (void)
{
  return 5;
}

int date::set (int a, int b, int c)
{
  if (a = 52)
    return b+c;
  throw foo ();
}

void f(date d)
{
  try {
    do_something (d);
  }

  catch (foo) {
    something_else ("you twat!");
  }
}


int main (void)
{
  set (2,5,6);
}
