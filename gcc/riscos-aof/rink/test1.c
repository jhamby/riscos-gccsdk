/* *** test file 1 for rtl *** */

#include <stdlib.h>
#include <stdio.h>

static void wibble (void);

int
test_function1 (int number, char *string)
{
  printf ("test_function1 called\n\n");
  printf ("number: %d\nstring: %s\n\n", number, string);

  wibble ();

  return 42;
}

static void
wibble (void)
{
  printf ("wibble called\n\n");
}
