/* Compatibility functions for the libscl.
   Implementation taken from UnixLib.  */

#include <stdlib.h>

/* ISOC99: Terminate the program with `status' without calling any of
   the functions registered with `atexit'.  */
void _Exit (int status)
{
  /* FIXME.  We might need to do some other cleanups.   */
  _exit (status);
}
