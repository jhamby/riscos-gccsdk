/* *** test file 3 for rtl *** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int integer;
extern char *string;

int test_function1 (int number, char *string);


/*void fn0(void)
{
  printf("fn0, no args\n\n");

}*/

void
fn1 (int n1)
{
  printf ("fn1, args: %d\n\n", n1);
  printf ("integer: %d\nstring: %s\n\n", integer, string);

  printf ("&integer = 0x%x, fn1 = 0x%x\n", (int) &integer, (int) fn1);
}

void
fn2 (int n1, int n2)
{
  printf ("fn2, args: %d %d\n\n", n1, n2);
}

void
fn3 (int n1, int n2, int n3)
{
  printf ("fn3, args: %d %d %d\n\n", n1, n2, n3);
}

void
fn4 (int n1, int n2, int n3, int n4)
{
  printf ("fn4, args: %d %d %d %d\n\n", n1, n2, n3, n4);

  printf ("calling test_function1\n");
  test_function1 (542, "string consisting of zonks\n\n");
}

void
fn5 (int n1, int n2, int n3, int n4, int n5)
{
  printf ("fn5, args: %d %d %d %d %d\n\n", n1, n2, n3, n4, n5);
}

void
fn6 (int n1, int n2, int n3, int n4, int n5, int n6)
{
  printf ("fn6, args: %d %d %d %d %d %d\n\n", n1, n2, n3, n4, n5, n6);

  integer = 44;
  strcpy (string, "This is a different string\n\n");
}
