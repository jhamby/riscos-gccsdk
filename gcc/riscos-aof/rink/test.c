/* *********** test program for rink ****************** */

#include <stdlib.h>
#include <stdio.h>

#include "os.h"

#include "rink.h"


os_error err_block;

rink_seghandle code;
rink_check check = { "TestRIN", 100, 1 };

int integer = 856;
char stringz[] = { "This is a string in the main program\n\n" };
char *string = stringz;

typedef void *(*fn0) (void);
typedef void *(*fn1) (int);
typedef void *(*fn2) (int, int);
typedef void *(*fn3) (int, int, int);
typedef void *(*fn4) (int, int, int, int);
typedef void *(*fn5) (int, int, int, int, int);
typedef void *(*fn6) (int, int, int, int, int, int);

os_error *
test_errtrans (int errn)
{
  err_block.errnum = errn;
  sprintf (err_block.errmess, "rink internal error, number %d", errn);

  return &err_block;
}

void *
test_alloc (int s)
{
  printf ("test_alloc called\n\n");

  return malloc (s);
}

void
test_free (void *p)
{
  printf ("test_free called\n\n");

  free (p);
}

int
main (void)
{
  os_error *error;
  int l;
  char *name;
  _rink_fn fn;
  rink_version *v;

  check.id[7] = 'K';

  if ((error = rink_load ("code", "links", &code, &check)) != 0)
    {
      printf ("%s\n\n", error->errmess);
      return 1;
    }

  v = rink_readversion (code);
  printf ("main version of segment %d\ncode version of segment %d\n", v->main,
	  v->code);

  ((fn0) rink_fn (code, 0)) ();
  ((fn1) rink_fn (code, 1)) (1);
  ((fn2) rink_fn (code, 2)) (1, 2);
  ((fn3) rink_fn (code, 3)) (1, 2, 3);
  ((fn4) rink_fn (code, 4)) (1, 2, 3, 4);
  ((fn5) rink_fn (code, 5)) (1, 2, 3, 4, 5);
  ((fn6) rink_fn (code, 6)) (1, 2, 3, 4, 5, 6);

  printf ("test_function1 returned %d\n\n",
	  (int) ((fn2) rink_fn (code, 7)) (128, (int) "Wibble zonk"));

  printf ("test_function2 returned %d\n\n",
	  (int) ((fn2) rink_fn (code, 8)) (21, (int) "Another string..."));

  printf ("*********** named functions... *************\n\n");

  l = 0;
  do
    {
      fn = rink_enum_named (code, &l, &name);
      printf ("*** %s\n", name);
      ((fn0) fn) ();
    }
  while (l >= 0);

  rink_unload (code);

  printf ("integer: %d\nstring: %s\n\n", integer, string);

  return 0;
}
