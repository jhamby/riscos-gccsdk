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
 * __base  __rwlimit     __break   __stack     |  appspace_limit
 *         __rwlomem     __stack_limit     appspace_himem
 *
 *
 * Case 2: Heap in dynamic area
 *                                                    /
 *    +-------+          +--------+.....+            /      +--------+.....+
 *    |       |  ....... |        |     | ......     \      | heap ->|     | ......
 *    +-------+          +--------+.....+            /      +--------+.....+
 *    ^       ^        <-^        ^     ^->         /       ^        ^     ^->
 * __base  __rwlimit  __stack     |  appspace_limit    __rwlomem     |  __dalimit
 *         __stack_limit      appspace_himem                     __break
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

/*#define DEBUG*/

#ifdef DEBUG
#include <sys/debug.h>
#include <unixlib/os.h>
#endif

#define align(x) ((void *)(((unsigned int)(x) + 3) & ~3))

/* This file should be compiled without stack checking, as it is could
   confuse malloc if the stack extension caused 'appspace_himem' to move
   whilst malloc is trying to sbrk a region */
#ifdef __CC_NORCROFT
#pragma -s1
#endif

static int
__internal_brk (void *addr, int internalcall)
{
  struct ul_memory *mem = &__ul_memory;
  struct ul_global *gbl = &__ul_global;

  /* Ensure requested address is aligned to a 4 byte boundry.  */
  addr = align (addr);

#ifdef DEBUG
  debug_printf ("brk: addr=%08x rwlomem=%08x"
		" unixlib_break=%08x unixlib_stack=%08x\n",
		addr, mem->__rwlomem,
		mem->__unixlib_break, mem->__unixlib_stack);
#endif

  /* Check new limit isn't below minimum brk limit, i.e., __rwlomem.
     Return EINVAL, because it doesn't make sense to return ENOMEM.  */
  if (addr < mem->__rwlomem)
    return __set_errno (EINVAL);

  /* We can't call getrlimit until the process is up and running.
     Unfortunately there is currently no method for safely determining
     whether the resource limits have been initialised yet.  Thus, we
     assume that we can allocate a minimum of 32K before we test the
     data limit resource.  */
  if (addr > mem->__unixlib_break)
    {
      /* struct rlimit rlim; */
      /* Inline version of
	 if (getrlimit (RLIMIT_DATA, &rlim) >= 0)
	   if ((u_char *) addr - (u_char *) __rwlomem > rlim.rlim_cur) */
      if ((unsigned int) addr - (unsigned int) mem->__rwlomem
	  > __u->limit[RLIMIT_DATA].rlim_cur)
	{
#ifdef DEBUG
	  debug_printf ("brk: addr (%08x) - __rwlomem (%08x) [%08x]"
			" > RLIMIT_DATA (%08x)\n",
			addr, mem->__rwlomem,
			addr - mem->__rwlomem,
			__u->limit[RLIMIT_DATA].rlim_cur);
#endif
	  /* Need to increase the resource limit.  */
	  return __set_errno (ENOMEM);
	}
    }

  /* Is heap in a dynamic area ?  */
  if (gbl->__dynamic_num != -1)
    {
      /* The heap is in a dynamic area.  */
      int regs[10];

      regs[0] = gbl->__dynamic_num;

      /* If the new address exceeds our current allocation from the
	 dynamic area, then we must attempt to claim more memory for
	 the dynamic area.  */
      if ((unsigned int) addr > mem->dalimit)
	{
	  /* Calculate the amount we want to increase the dynamic area
	     by.  */
	  regs[1] = (int) addr - mem->dalimit;

	  /* Align that amount up to a multiple of 32K to reduce number
	     of expensive sbrk calls.

	     This is done because OS_ChangeDynamicArea can be expensive,
	     so smaller [s]brk increments will fit inside 'dalimit'.  */
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

	  /* Record the new maximum address space.  */
	  mem->dalimit += regs[1];

	  /* Record the new break value.  Note that because we aligned
	     'dalimit', '__unixlib_break' will never equal it.  */
	  mem->__unixlib_break = addr;
	}
      else if (addr > mem->__unixlib_break)
	{
	  /* The user is claiming more memory and we have enough space in
	     our dynamic area to cope.  */
	  mem->__unixlib_break = addr;
	}
      else if (addr < mem->__unixlib_break)
	{
	  /* The user is freeing memory.  */

	  /* New alloc system can cope with userland calling sbrk aswell
	     as the library.  Thus, we should honour a request to reduce
	     the brk limit.  Align the new limit to a page boundary.  */
	  mem->__unixlib_break = addr;

	  /* See if we can give some memory back to the system by
	     reducing the size of our dynamic area.  */
	  regs[1] = mem->dalimit - (unsigned int) addr;

	  /* Align size down to multiple of 32K, thereby sticking to the
	     rule that we provide a small buffer space to reduce the number
	     of calls to OS_ChangeDynamicArea.  */
	  regs[1] = regs[1] & ~__DA_WIMPSLOT_ALIGNMENT;

	  /* If regs[1] is non-zero and positive, then we have found
	     memory to give back.  */
	  if (regs[1] > 0)
	    {
	      /* OS_ChangeDynamicArea takes a signed integer, with negative
		 values meaning that memory is to be released.  */
	      regs[1] = -regs[1];

	      /* Ignore any error from __os_swi, since it can happen with a
		 request to reduce the size of the area which is only partially
		 satisfied.  Either way, regs[1] should have the +ve amount of
		 memory returned to the system.  */
	      __os_swi (OS_ChangeDynamicArea, regs);
	      mem->dalimit -= regs[1];
	    }
	}
    }
  else
    {
      /* Heap is not in a dynamic area and is therefore below the stack.
	 Make sure we don't run into the stack */
      if (addr > mem->__unixlib_stack)
        {
#ifdef DEBUG
	  debug_printf ("brk: addr > __unixlib_stack\n");
#endif
	  /* No space before stack, so try to increase wimpslot
	     If this is a userland call then increasing the wimpslot is
	     likely to give unexpected results so don't bother */
	  if (! internalcall)
	    return __set_errno (ENOMEM);

	  if (__stackalloc_incr_wimpslot ((unsigned int) addr
					  - mem->appspace_himem) == NULL)
	    return __set_errno (ENOMEM);
        }
      else
        {
          /* Adjust stack limit.*/
          mem->__unixlib_stack_limit = addr;
        }

      /* Adjust break limit.
	 This allows +ve or -ve sbrk increments.  */
      mem->__unixlib_break = addr;
    }

  return 0;
}

int
brk (void *addr)
{
  PTHREAD_UNSAFE

  return __internal_brk (addr, 0);
}

/* External calls to sbrk can only increase __unixlib_break up
   to __unixlib_stack, and cannot increase the wimpslot as the
   stack will be in the way. */
void *
sbrk (intptr_t delta)
{
  struct ul_memory *mem = &__ul_memory;
  struct ul_global *gbl = &__ul_global;
  void *oldbrk = mem->__unixlib_break;

  PTHREAD_UNSAFE

#ifdef DEBUG
  debug_printf ("sbrk: incr=%d\n", delta);
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
      oldbrk = mem->appspace_himem;
      overstack = 1;
    }
  else
    oldbrk = mem->__unixlib_break;

  if (incr != 0 && __internal_brk ((u_char *) oldbrk + incr, 1) < 0)
    return ((void *)-1);

  return oldbrk;
}
