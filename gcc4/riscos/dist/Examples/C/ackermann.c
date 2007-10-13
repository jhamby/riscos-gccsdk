/* Ackermann function */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static int ack (int, int, int);

int
main (int argc, char *argv[])
{
  int a, b;

  if (argc == 1 || argc == 2)
    {
      printf ("Error. Too few parameters passed on command line\n");
      printf ("Syntax: ackermann <x> <y>\n");
      return 1;
    }

  /* Convert from strings to numbers, the two arguments passed on the
     command line.  */
  a = atoi (argv[1]);
  b = atoi (argv[2]);

  printf ("ack(%d,%d) = %d\n", a, b, ack (a, b, 0));
  printf ("Time = %d\n", clock ());
  return 0;
}

int
ack (int x, int y, int acker)
{
  if (x == 0)
    {
      acker = y + 1;
    }
  else
    {
      if (y == 0)
	{
	  acker = ack (x - 1, 1, 0);
	}
      else
	{
	  acker = ack (x - 1, ack (x, y - 1, 0), 0);
	}
    }

  return acker;
}
