/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/initialise.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: initialise.c,v 1.2.2.3 2001/09/04 16:32:04 admin Exp $";
#endif

#include <sys/resource.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <swis.h>
#include <sys/types.h>

/* Initialise the resource limits to calculated initial values.
   Best guesses are taken where information is hard to obtain. :-(  */

void
__resource_initialise (struct proc *p)
{
  int regs[10];

  /* The maximum amount of cpu time the process can use.  */
  p->limit[RLIMIT_CPU].rlim_cur = RLIM_INFINITY;
  p->limit[RLIMIT_CPU].rlim_max = RLIM_INFINITY;

  /* The maximum size of a file that the process can create.  */
  p->limit[RLIMIT_FSIZE].rlim_cur = RLIM_INFINITY;
  p->limit[RLIMIT_FSIZE].rlim_max = RLIM_INFINITY;

  /* The maximum size core file that this process can create.  */
  p->limit[RLIMIT_CORE].rlim_cur = RLIM_INFINITY;
  p->limit[RLIMIT_CORE].rlim_max = RLIM_INFINITY;

  /* The maximum size of data memory for the process.
     Reference src.sys.c.brk for a clear example.

     This should be the same for both cases of dynamic area or not.
     Data area should lie between __lomem and __break. However, __break
     can gradually grow as more memory is requested. For non dynamic
     areas, the data area can lie between __lomem and __stack (yes
     __stack can become less, but stack checking accounts for this).

     For dynamic areas the limit is current available memory size or
     the limit imposed by any virtual memory system such as Virtualise.  */

  if (__dynamic_num == -1)	/* No dynamic area */
    p->limit[RLIMIT_DATA].rlim_max = (u_char *) __stack - (u_char *) __lomem;
  else
    {
      regs[0] = __dynamic_num;
      if (__os_swi (OS_ReadDynamicArea, regs))
	p->limit[RLIMIT_DATA].rlim_max = (u_char *) __break - (u_char *) __lomem;
      else
	p->limit[RLIMIT_DATA].rlim_max = regs[2];
    }
  p->limit[RLIMIT_DATA].rlim_cur = p->limit[RLIMIT_DATA].rlim_max;


  /* The maximum stack size for the process. This lies between
     __himem and __stack_limit.  */
  p->limit[RLIMIT_STACK].rlim_max = (u_char *) __himem - (u_char *) __stack_limit;
  /* Maximum (soft limit) stack size for the process lies between __stack
     and __himem.  */
  p->limit[RLIMIT_STACK].rlim_cur = p->limit[RLIMIT_STACK].rlim_max;


  /* Maximum amount of physical memory that this process should get.
     This process should get as much memory as possible.

     Again, for RISC OS 3.5+ dynamic areas and without, both cases
     should be treated differently.

     I think that maximum physical memory is the area from __base to
     __himem (no dynamic area). Also included is from __lomem to __break
     and beyond for dynamic areas.  */
  if (__dynamic_num == -1)	/* No dynamic area */
    {
      p->limit[RLIMIT_RSS].rlim_max = (u_char *) __himem - (u_char *) __base;
      p->limit[RLIMIT_RSS].rlim_cur = p->limit[RLIMIT_RSS].rlim_max;
    }
  else
    {
      /* Area 6 is the free pool for RISC OS 3.5+ and setting bit 7 gets
         the maximum size.  */
      regs[0] = 6 + 128;
      if (__os_swi (OS_ReadDynamicArea, regs))
	p->limit[RLIMIT_RSS].rlim_max += (u_char *) __break - (u_char *) __lomem;
      else
	/* rlim_max is all of physical memory ?  */
	p->limit[RLIMIT_RSS].rlim_max = regs[2];
      /* rlim_cur is available free memory + __base to __himem.  */
      p->limit[RLIMIT_RSS].rlim_cur += regs[1];
    }


  /* The maximum amount of memory that can be locked into physical
     memory (so it will never be paged out).  Virtual Memory on
     RISC OS (when supported) does not really implement memory locks.
     Virtual Memory isn't really supported on RISC OS anyway.  */
  p->limit[RLIMIT_MEMLOCK].rlim_cur = 0;
  p->limit[RLIMIT_MEMLOCK].rlim_max = 0;

  /* The maximum number of processes that can be created with the
     same user ID.  For RISC OS this is zero.  */
  p->limit[RLIMIT_NPROC].rlim_cur = CHILD_MAX;
  p->limit[RLIMIT_NPROC].rlim_max = CHILD_MAX;

  /* The maximum number of files that the process can open.  */
  p->limit[RLIMIT_NOFILE].rlim_cur = FOPEN_MAX;
  p->limit[RLIMIT_NOFILE].rlim_max = FOPEN_MAX;
}
