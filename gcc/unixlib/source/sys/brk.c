/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/brk.c,v $
 * $Date: 2002/02/14 15:56:37 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/****************************************************************************
 *
 * Memory is layed out in one of two ways, depending whether we are using
 * a RISC OS 3.5+ dynamic area for the heap. If a dynamic area is being
 * used then __dynamic_num != -1.
 *
 * Case 1: No dynamic area
 *
 *    +-------+-------------+         +--------+.....+
 *    |       | heap ->     | ....... |        |     | ......
 *    +-------+-------------+         +--------+.....+
 *    ^       ^             ^->     <-^        ^     ^->
 * __base  __rwlimit     __break   __stack     |  __real_himem
 *         __lomem       __stack_limit      __himem
 *
 *
 * Case 2: Heap in dynamic area
 *                                                    /
 *    +-------+          +--------+.....+            /      +--------+.....+
 *    |       |  ....... |        |     | ......     \      | heap ->|     | ......
 *    +-------+          +--------+.....+            /      +--------+.....+
 *    ^       ^        <-^        ^     ^->         /       ^        ^     ^->
 * __base  __rwlimit  __stack     |  __real_himem        __lomem     |  __real_break
 *         __stack_limit       __himem                            __break
 *
 * The stack initially decends (in chunks) downto __stack_limit, then
 * increases (in chunks) by increasing the wimpslot. If the malloc heap is
 * also in the wimpslot then it can also cause the wimpslot to extend.
 ***************************************************************************/

/* sys/brk.c: Complete rewrite by Peter Burwood, June 1997  */

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: brk.c,v 1.3 2002/02/14 15:56:37 admin Exp $";
#endif

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/resource.h>
#include <unixlib/os.h>
#include <swis.h>
#include <sys/types.h>
#include <unixlib/unix.h>

/* #define DEBUG */

#ifdef DEBUG
#include <unixlib/os.h>
#endif

#define align(x) ((void *)(((unsigned int)(x) + 3) & ~3))

/* This file should be compiled without stack checking, as it is could
   confuse malloc if the stack extension caused __himem to move whilst
   malloc is trying to sbrk a region */
#ifdef __CC_NORCROFT
#pragma -s1
#endif

/* __real_break is the top limit of the dynamic area allocated.  */
extern void * __real_break;

static int
__internal_brk (void *addr, int internalcall)
{
  addr = align (addr);

#ifdef DEBUG
  __os_print ("-- brk: addr = ");    __os_prhex ((int)addr);    __os_print ("\r\n");
  __os_print ("-- brk: __lomem = "); __os_prhex ((int)__lomem); __os_print ("\r\n");
  __os_print ("-- brk: __break = "); __os_prhex ((int)__break); __os_print ("\r\n");
  __os_print ("-- brk: __stack = "); __os_prhex ((int)__stack); __os_print ("\r\n");
#endif

  /* Check new limit isn't below minimum brk limit, i.e., __lomem.
     Return EINVAL, because it doesn't make sense to return ENOMEM.  */
  if (addr < __lomem)
    return __set_errno (EINVAL);

  /* We can't call getrlimit until the process is up and running.
     Unfortunately there is currently no method for safely determining
     whether the resource limits have been initialised yet.  Thus, we
     assume that we can allocate a minimum of 32K before we test the
     data limit resource.  */
  if (addr > __break)
    {
      if ((u_char *) addr - (u_char *) __lomem > (32 * 1024))
	{
	  /* struct rlimit rlim; */
	  /* Inline version of
	       if (getrlimit (RLIMIT_DATA, &rlim) >= 0)
	         if ((u_char *) addr - (u_char *) __lomem > rlim.rlim_cur) */
	  if ((u_char *) addr - (u_char *) __lomem > __u->limit[RLIMIT_DATA].rlim_cur)
	    {
#ifdef DEBUG
       	      __os_print ("-- brk: addr - __lomem > RLIMIT_DATA (");
       	      __os_prhex (__u->limit[RLIMIT_DATA].rlim_cur);
	      __os_print (")\r\n");
#endif
	      /* Need to increase the resource limit.  */
	      return __set_errno (ENOMEM);
	    }
	}
    }

  /* Is heap in a dynamic area ?  */
  if (__dynamic_num != -1)
    {
      int regs[10];

      regs[0] = __dynamic_num;
      if (addr > __real_break)
	{
	  regs[1] = (int) ((u_char *) addr - (u_char *) __real_break);
	  /* Align size to multiple of 32K to reduce number of expensive sbrk
	     calls.  This is done because OS_ChangeDynamicArea can be expensive,
	     so smaller [s]brk increments will fit inside __real_break.  */
	  regs[1] = (regs[1] + __DA_WIMPSLOT_ALIGNMENT) & ~__DA_WIMPSLOT_ALIGNMENT;
	  if (__os_swi (OS_ChangeDynamicArea, regs))
	    {
#ifdef DEBUG
       	      __os_print ("-- brk: OS_ChangeDynamicArea failed\r\n");
#endif
	      /* Failed to allocate the memory, so return an error.  */
	      return __set_errno (ENOMEM);
	    }
	  __real_break = (u_char *) __real_break + (unsigned int) regs[1];
	  __break = addr;
	}
      else if (addr > __break)
	__break = addr;
      else if (addr < __break)
	{
	  /* New alloc system can cope with userland calling sbrk aswell
	     as the library.  Thus, we should honour a request to reduce
	     the brk limit.  Align the new limit to a page boundary.  */
	  __break = addr;
	  regs[1] = (int) ((u_char *) __real_break - (u_char *) addr);
	  /* Align size down to multiple of 32K */
	  regs[1] = regs[1] & ~__DA_WIMPSLOT_ALIGNMENT;
	  /* Trim dynamic area by 32K multiples if enough unused memory.  */
	  if (regs[1] > 0)
	    {
	      regs[1] = -regs[1];
	      /* Ignore any error from __os_swi, since it can happen with a
		 request to reduce the size of the area which is only partially
		 satisfied.  Either way, regs[1] should have the +ve amount of
		 memory returned to the system.  */
	      __os_swi (OS_ChangeDynamicArea, regs);
	      __real_break = (u_char *) __real_break - (unsigned int) regs[1];
	    }
	}
    }

  /* heap is below stack, so make sure we don't run into the stack */
  else
    {
      if (addr > __stack)
        {
#ifdef DEBUG
           __os_print ("-- brk: addr > __stack\r\n");
#endif

           /* No space before stack, so try to increase wimpslot
              If this is a userland call then increasing the wimpslot is
              likely to give unexpected results so don't bother */
           if (!internalcall || __stackalloc_incr_wimpslot((u_char *)addr - (u_char *)__himem) == 0)
             return __set_errno (ENOMEM);
        }
      else
        {
          /* Adjust stack limit.*/
          __stack_limit = addr;
        }
      /* Adjust break limit.
	 This allows +ve or -ve sbrk increments.  */
      __real_break = __break = addr;
    }

  return 0;
}

int
brk (void *addr)
{
  return __internal_brk (addr, 0);
}

/* External calls to sbrk can only increase __break upto __stack,
   and cannot increase the wimplot as the stack will be in the way. */
void *
sbrk (int incr)
{
  void *oldbrk = __break;

#ifdef DEBUG
  __os_print ("-- sbrk: incr = "); __os_prdec (incr); __os_print ("\r\n");
#endif

  if (incr != 0 && __internal_brk ((u_char *) oldbrk + incr, 0) < 0)
    return ((void *)-1);

  return (__dynamic_num == -1 && oldbrk > __stack_limit) ? __stack_limit : oldbrk;
}

/* sbrk for internal UnixLib callers (i.e. malloc) that are aware that
   space allocated may not be contiguous */
void *
__internal_sbrk (int incr)
{
  void *oldbrk;
  /* Record once we have returned some memory above the stack, to ensure all
     subsequent memory is also from above the stack, otherwise malloc would
     get confused */
  static int overstack = 0;

#ifdef DEBUG
  __os_print ("-- __internal_sbrk: incr = ");
  __os_prdec (incr); __os_print ("\r\n");
#endif

  if (incr < 0)
    return ((void *)-1);

  if (__dynamic_num == -1
      && (overstack || ((u_char *)__break + incr >= (u_char *)__stack)))
    {
      oldbrk = __himem;
      overstack = 1;
    }
  else
    oldbrk = __break;

  if (incr != 0 && __internal_brk ((u_char *) oldbrk + incr, 1) < 0)
    return ((void *)-1);

  return oldbrk;
}
