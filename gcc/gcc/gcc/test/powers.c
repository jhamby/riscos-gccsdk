/* powers
   print a power e.g. 2^69382 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int ipower (int, int);
void rase (void);
int begin, expon, numb;
char *fig;

int
main (int argc, char *argv[])
{
  int dupnumb, dupexpon, len, speedup, rest, temp, f;
  extern int begin, expon, numb;
  extern char *fig;
  double lp, lg;
  int start_time = clock ();

  if (argc == 1 || argc == 2)
    {
      printf ("Usage: Powers <number> <raise>\n");
      return 0;
    }
  numb = atoi (argv[1]);
  expon = atoi (argv[2]);
  if (numb < 2 || numb > 199526131)
    {
      printf ("<number> is between 2 and 199526131 only\n");
      return 0;
    }
  if (expon < 2)
    {
      printf ("<raise> is between 2 and 2147483647 only\n");
      return 0;
    }
  lg = log10 (numb);
  lp = lg + 0.000000002;
  dupnumb = numb;
  dupexpon = expon;
  len = (int) (expon * lp) + 1;
  fig = (char *) malloc (len + 1);
  printf ("len = %d, numb = %d, expon = %d, lp = %.12f, lg = %.12f\n",
  	  len, numb, expon, lp, lg);

  for (temp = 1; temp <= len; temp++)
    fig[temp] = 0;

  fig[1] = 1;
  begin = 1;
  speedup = (int) (8.3 / lg);
  if (speedup > expon)
    speedup = 1;
  rest = expon % speedup;
  numb = ipower (numb, speedup);
  expon = expon / speedup;
  rase ();
  if (rest != 0)
    {
      numb = dupnumb;
      begin = dupexpon - rest + 1;
      expon = dupexpon;
      rase ();
    }
  printf ("%d ^ %d is exactly\n\n", dupnumb, dupexpon);
  for (f = len; f != 0; f = f - 1)
    printf ("%d", fig[f]);
  printf ("\n\n(which is about %d x 10^%d)\n", fig[len], len - 1);
  printf ("Time taken: %d csecs\n\n", clock () - start_time);
  return 0;
}

void
rase (void)
{
  int raise, help, place, temp, carry = 0;
  float lp;

  lp = (float) log10 (numb);
  for (raise = begin; raise <= expon; raise++)
    {
      temp = (int) (raise * lp) + 1;
      for (place = 1; place <= temp; place++)
	{
	  help = (numb * fig[place]) + carry;
	  carry = help / 10;
	  fig[place] = help % 10;
	}
    }
}

int
ipower (int r, int n)
{
  int p = 1;
loop:
  if (n <= 0)
    return p;
  if (n & 1)
    p *= r;
  r *= r;
  n = n / 2;
  goto loop;
}
