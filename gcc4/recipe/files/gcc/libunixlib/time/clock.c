/* Determine processor time.
   Copyright (c) 2011 UnixLib Developers.  */

#include <errno.h>
#include <time.h>

#include <internal/os.h>

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_ReadMonotonicTime (clock_t *resultp)
{
  register const _kernel_oserror *err __asm ("r0");
  register clock_t result __asm ("r1");
  __asm__ volatile ("SWI\t%[SWI_OS_ReadMonotonicTime]\n\t"
		    "MOV\tr1, r0\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (result)
		    : [SWI_OS_ReadMonotonicTime] "i" (OS_ReadMonotonicTime | (1<<17))
		    : "r14", "cc");
  *resultp = result;
  return err;
}

/* Return the elapsed processor time. The base time is arbitary, but doesn't
   change.  */
clock_t
clock (void)
{
  clock_t result;
  const _kernel_oserror *err;
  if ((err = SWI_OS_ReadMonotonicTime (&result)) != NULL)
    return __ul_seterr (err, ENOSYS);
  return result;
}

