/* srand (), rand ()
 * Copyright (c) 2002-2008 UnixLib Developers
 */

#include <stdlib.h>

#include <pthread.h>
#include <internal/unix.h>

/* This is a BAD random number generator.  Use the random functions
   instead.  */

static unsigned long __state[32] =
{
  0x6fdb9cb7, 0x9de8dc3d, 0x093bf9e4, 0x47528c2b, 0xfc263867, 0x53cbf1bf,
  0x13618c92, 0x9e0f31b1, 0xcd651ab0, 0x2b52a7e5, 0x2ccdd9bf, 0x30052e2e,
  0xb278be81, 0xd634a58b, 0x0a33d2c1, 0xfd42f052, 0xcb2f06f8, 0xa57bb730,
  0x4ca963ac, 0x84bf5532, 0xd67ab9e6, 0x6e2d017b, 0x1e17cd99, 0x5891173a,
  0x39384a29, 0xe0a0282e, 0x2e5512fc, 0x3093f269, 0x3a6983e6, 0x6b9fdaf3,
  0x38b6bbd1, 0xb5e23046
};
static int __st1 = 0, __st2 = 3;

void
srand (long seed)
{
  int i;

  PTHREAD_UNSAFE

  for (i = 0; i < 32; i++)
    seed = __state[i] = (seed * 1103515245 + 12345);
  __st1 = 0;
  __st2 = 3;
  for (i = 0; i < ((rand () ^ seed) & 255); i++);
  for (i = 0; i < ((rand () ^ seed) & 255); i++)
    rand ();
}

int
rand (void)
{
  long i, j;
  int k, l;

  PTHREAD_UNSAFE

  i = *((long *) (__state + (k = __st1)));
  j = *((long *) (__state + (l = __st2)));
  if (i < 0 && j < 0)
    i = -i;
  __state[l] = (i += j);
  __st1 = (k + 1) & 31;
  __st2 = (l + 1) & 31;
  return ((i >> 1) & RAND_MAX);
}
