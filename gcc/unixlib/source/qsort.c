/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <unixlib/unix.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N_INSERT	8

static char *__t;

static size_t __z;
static int (*__c) (const void *, const void *);

/* fast insertion sort - used for n <= N_INSERT */

static void
__isort (register char *b, register size_t n)

{
  register size_t z = __z;
  register int (*c) (const void *, const void *) = __c;
  register char *m, *e, *p;
  register char *t;

#define swap(x,y) (memcpy(t,x,z),memcpy(x,y,z),memcpy(y,t,z))
#define move(x,y,z) memmove(x,y,z)
#define push(x) memcpy(t,x,z)
#define pull(x) memcpy(x,t,z)

  t = __t;

  e = b + (n * z);		/* past end */

/* find minimum */

  for (m = p = b; (p += z) < e;)
    if ((*c) (m, p) > 0)
      m = p;

/* swap minimum into base */

  if (m != b)
    swap (m, b);

/* standard insertion sort */

  for (m = b; (p = m += z) < e;)
    {
      while ((*c) (p -= z, m) > 0);
      if ((p += z) != m)
	push (m), move (p + z, p, m - p), pull (p);
    }

#undef swap
#undef move
#undef push
#undef pull
}

/* quicksort - used for n > N_INSERT */

static void
__qsort (register char *b, register size_t n)

{
  register size_t z = __z;
  register int (*c) (const void *, const void *) = __c;
  register char *m, *e, *p, *t;
  register int i, j, k;

#define swap(x,y) (memcpy(t,x,z),memcpy(x,y,z),memcpy(y,t,z))

loop:

  t = __t;

  m = b + ((n >> 1) * z);	/* middle */
  e = b + ((n - 1) * z);	/* end */

/* find pivot - median of b,m,e */

  if ((*c) (b, m) >= 0)
    p = b;
  else
    p = m, m = b;
  if ((*c) (p, e) > 0)
    p = ((*c) (m, e) >= 0) ? m : e;

/* swap pivot into base */

  if (p != b)
    swap (p, b);

/* standard quicksort & check for flat partition */

  m = b;
  i = 0;
  j = 1;
  for (p = b; (p += z) <= e;)
    {
      if (!(k = (*c) (p, b)))
	j++;
      if (k < 0)
	{
	  if ((m += z) != p)
	    swap (m, p);
	  i++;
	}
    }

  if (j == n)
    return;			/* exit if flat */

  if (b != m)
    swap (b, m);

  m += z;

/* sort smallest partition first */

  if (i < (n >> 1))
    {
      if (i > N_INSERT)
	__qsort (b, i);
      else if (i > 1)
	__isort (b, i);
      i = n - i - 1;
    }
  else
    {
      i = n - i - 1;
      if (i > N_INSERT)
	__qsort (m, i);
      else if (i > 1)
	__isort (m, i);
      i = n - i - 1;
      m = b;
    }

  if (i > N_INSERT)
    {
      b = m;
      n = i;
      goto loop;
    }				/* iterate larger partition */
  else if (i > 1)
    __isort (m, i);

#undef swap
}

void
qsort (register void *v, register size_t n, register size_t z,
       register int (*c) (const void *, const void *))

{
  static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  /* Use a mutex so that threads aren't disabled while calling the user function */

  if (n < 2)
    return;

  pthread_mutex_lock (&mutex);

  if (!(__t = malloc (z)))
    {
      pthread_mutex_unlock (&mutex);
      return;
    }

  __z = z;
  __c = c;

#ifdef __PARANOID
  /* check function pointer */
  __funcall ((*c), (v, v));
#endif

  if (n > N_INSERT)
    __qsort ((char *) v, n);
  else if (n > 1)
    __isort ((char *) v, n);

  free (__t);

  pthread_mutex_unlock (&mutex);
}
