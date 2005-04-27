/* Change data segment size.
   Copyright (c) 2002, 2003, 2004, 2005 UnixLib Developers.  */

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
 * The stack initially decends (in chunks) downto __unixlib_stack_limit, then
 * increases (in chunks) by increasing the wimpslot. If the malloc heap is
 * also in the wimpslot then it can also cause the wimpslot to extend.
 ***************************************************************************/

/* sys/brk.c: Complete rewrite by Peter Burwood, June 1997  */


#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/resource.h>
#include <unixlib/os.h>
#include <swis.h>
#include <sys/types.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* #define DEBUG */

#ifdef DEBUG
#include <unixlib/os.h>
#endif

#define align(x) ((void *)(((unsigned int)(x) + 3) & ~3))

/* This file should be compiled without stack checking, as it is could
   confuse malloc if the stack extension caused __image_rw_himem to move
   whilst malloc is trying to sbrk a region */
#ifdef __CC_NORCROFT
#pragma -s1
#endif

/* __unixlib_real_break is the top limit of the dynamic area allocated.  */
extern void * __unixlib_real_break;

static int
__internal_brk (void *addr, int internalcall)
{
  struct ul_memory *mem = &__ul_memory;
  struct ul_global *gbl = &__ul_global;
  addr = align (addr);

#ifdef DEBUG
  debug_printf ("brk: addr=%08x, image_rw_lomem=%08x"
		", unixlib_break=%08x, unixlib_stack=%08x\n",
		addr, mem->__image_rw_lomem,
		mem->__unixlib_break, mem->__unixlib_stack);
#endif

  /* Check new limit isn't below minimum brk limit, i.e., __image_rw_lomem.
     Return EINVAL, because it doesn't make sense to return ENOMEM.  */
  if (addr < mem->__image_rw_lomem)
    return __set_errno (EINVAL);

  /* We can't call getrlimit until the process is up and running.
     Unfortunately there is currently no method for safely determining
     whether the resource limits have been initialised yet.  Thus, we
     assume that we can allocate a minimum of 32K before we test the
     data limit resource.  */
  if (addr > mem->__unixlib_break)
    {
      if ((u_char *) addr - (u_char *) mem->__image_rw_lomem > (32 * 1024))
	{
	  /* struct rlimit rlim; */
	  /* Inline version of
	     if (getrlimit (RLIMIT_DATA, &rlim) >= 0)
	     if ((u_char *) addr - (u_char *) __image_rw_lomem > rlim.rlim_cur) */
	  if ((u_char *) addr - (u_char *) mem->__image_rw_lomem
	      > __u->limit[RLIMIT_DATA].rlim_cur)
	    {
#ifdef DEBUG
       	      debug_printf ("brk: addr - __image_rw_lomem > RLIMIT_DATA (%08x)\n",
			    __u->limit[RLIMIT_DATA].rlim_cur);
#endif
	      /* Need to increase the resource limit.  */
	      return __set_errno (ENOMEM);
	    }
	}
    }

  /* Is heap in a dynamic area ?  */
  if (gbl->__dynamic_num != -1)
    {
      int regs[10];

      regs[0] = gbl->__dynamic_num;
      if (addr > mem->__unixlib_real_break)
	{
	  regs[1] = (int) ((u_char *) addr
			   - (u_char *) mem->__unixlib_real_break);
	  /* Align size to multiple of 32K to reduce number of expensive sbrk
	     calls.  This is done because OS_ChangeDynamicArea can be
	     expensive, so smaller [s]brk increments will fit inside
	     __unixlib_real_break.  */
	  regs[1] = ((regs[1] + __DA_WIMPSLOT_ALIGNMENT)
		     & ~__DA_WIMPSLOT_ALIGNMENT);
	  if (__os_swi (OS_ChangeDynamicArea, regs))
	    {
#ifdef DEBUG
       	      debug_printf ("brk: OS_ChangeDynamicArea failed\n");
#endif
	      /* Failed to allocate the memory, so return an error.  */
	      return __set_errno (ENOMEM);
	    }
	  mem->__unixlib_real_break = ((u_char *) mem->__unixlib_real_break
				       + (unsigned int) regs[1]);
	  mem->__unixlib_break = addr;
	}
      else if (addr > mem->__unixlib_break)
	mem->__unixlib_break = addr;
      else if (addr < mem->__unixlib_break)
	{
	  /* New alloc system can cope with userland calling sbrk aswell
	     as the library.  Thus, we should honour a request to reduce
	     the brk limit.  Align the new limit to a page boundary.  */
	  mem->__unixlib_break = addr;
	  regs[1] = (int) ((u_char *) mem->__unixlib_real_break
			   - (u_char *) addr);
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
	      mem->__unixlib_real_break = ((u_char *) mem->__unixlib_real_break
					   - (unsigned int) regs[1]);
	    }
	}
    }

  /* heap is below stack, so make sure we don't run into the stack */
  else
    {
      if (addr > mem->__unixlib_stack)
        {
#ifdef DEBUG
           __os_print ("-- brk: addr > __unixlib_stack\r\n");
#endif

           /* No space before stack, so try to increase wimpslot
              If this is a userland call then increasing the wimpslot is
              likely to give unexpected results so don't bother */
           if (! internalcall
	       || __stackalloc_incr_wimpslot ((u_char *)addr
					      - (u_char *)mem->__image_rw_himem) == 0)
             return __set_errno (ENOMEM);
        }
      else
        {
          /* Adjust stack limit.*/
          mem->__unixlib_stack_limit = addr;
        }
      /* Adjust break limit.
	 This allows +ve or -ve sbrk increments.  */
      mem->__unixlib_real_break = mem->__unixlib_break = addr;
    }

  return 0;
}

int
brk (void *addr)
{
  PTHREAD_UNSAFE

  return __internal_brk (addr, 0);
}

/* External calls to sbrk can only increase __unixlib_break upto __unixlib_stack,
   and cannot increase the wimplot as the stack will be in the way. */
void *
sbrk (intptr_t delta)
{
  struct ul_memory *mem = &__ul_memory;
  struct ul_global *gbl = &__ul_global;
  void *oldbrk = mem->__unixlib_break;

  PTHREAD_UNSAFE

#ifdef DEBUG
  debug_printf ("sbrk: incr=%d\\n", delta);
#endif

  if (delta != NULL && __internal_brk ((u_char *) oldbrk + (int)delta, 0) < 0)
    return ((void *)-1);

  return ((gbl->__dynamic_num == -1
	   && oldbrk > mem->__unixlib_stack_limit)
	  ? mem->__unixlib_stack_limit : oldbrk);
}

/* sbrk for internal UnixLib callers (i.e. malloc) that are aware that
   space allocated may not be contiguous.  No need to be thread safe
   as the internal callers should already have ensured that context switching
   is disabled.  */
void *
__internal_sbrk (int incr)
{
  struct ul_memory *mem = &__ul_memory;
  struct ul_global *gbl = &__ul_global;
  void *oldbrk;
  /* Record once we have returned some memory above the stack, to ensure all
     subsequent memory is also from above the stack, otherwise malloc would
     get confused */
  static int overstack = 0;

#ifdef DEBUG
  debug_printf ("__internal_sbrk: incr=%d\n", incr);
#endif

  if (incr < 0)
    return ((void *)-1);

  if (gbl->__dynamic_num == -1
      && (overstack
	  || ((u_char *)mem->__unixlib_break + incr
	      >= (u_char *)mem->__unixlib_stack)))
    {
      oldbrk = mem->__image_rw_himem;
      overstack = 1;
    }
  else
    oldbrk = mem->__unixlib_break;

  if (incr != 0 && __internal_brk ((u_char *) oldbrk + incr, 1) < 0)
    return ((void *)-1);

  return oldbrk;
}
