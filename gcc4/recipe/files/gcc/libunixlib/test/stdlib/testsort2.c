#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int count;
static int
cmp (const int *i1, const int *i2)
{
 count++;
 return (*i1 - *i2);
}

static int array[100000];

static void
report (void)
{
  qsort (array, 100000, sizeof (int),
         (int (*) (const void *, const void *))cmp);

  printf ("Number of compares = %d\n", count) ;
  printf ("%d %d %d %d %d %d\n",
          array[0], array[1], array[2], array[3], array[4], array[5]) ; 
  printf ("%d %d %d %d %d %d\n",
          array[99994], array[99995], array[99996], array[99997], array[99998], array[99999]) ;              
}

int
main ( int argc, char **argv )
{
  int i;

  printf ("1) Random\n");
  for (i = 0 ; i < 100000; i++)
    array [i] = (int) lrand48 ();
  count = 0;
  report ();

  printf ("2) 0 1 2 ... 99998 99999\n");
  for (i = 0 ; i < 100000; i++)
    array [i] = i;
  count = 0;
  report ();

  printf ("3) 99999 99998 ... 2 1 0\n");
  for (i = 0 ; i < 100000; i++)
    array [i] = 100000 - i;
  count = 0;
  report ();

  printf ("4) 100000 1 100002 ... 199998 99999\n");
  for (i = 0 ; i < 100000; i++)
   array [i] = (i % 2) ? i : i + 100000;
  count = 0;
  report ();

  return EXIT_SUCCESS;
}
