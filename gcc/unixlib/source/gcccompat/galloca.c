/* Implementation of the alloca function for GCC 4
   Copyright (c) 2005 UnixLib Developers
   Written by Nick Burrett <nick@sqrt.co.uk>  */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <unixlib/local.h>

//#define DEBUG 1
//#define DUMMY_IMPLEMENTATION 1

/* These prototypes need to exist to stop the compiler complaining,
   but they are never referenced directly, so they aren't going to appear
   in any header file.  */
extern int __gcc_alloca_save (void);
extern void __gcc_alloca_restore (unsigned int fp, unsigned int block);
extern void __gcc_alloca_free (void);
extern unsigned long __gcc_alloca_free_1 (unsigned long fp);
extern void *__gcc_alloca (size_t size);
extern void __gcc_alloca_thread_free_all (void);

#ifndef DUMMY_IMPLEMENTATION

struct alloca_chunk
{
  unsigned long fp; /* Frame pointer.  */
  unsigned int lr; /* Return address for real function.  */
  unsigned int block; /* If nested, then nesting level.  */
  void *ptr; /* Pointer to malloc'ed memory.  */
  struct alloca_chunk *next;
};

#if ! __UNIXLIB_FEATURE_PTHREADS
/* Head node of alloca list for single threaded programs.  */
static struct alloca_chunk *head = NULL;
#endif

/* Return head node.  Multi-threaded applications store the head node
   in a private structure.  */
static inline struct alloca_chunk *chunk_head (void)
{
#if __UNIXLIB_FEATURE_PTHREADS
  return (struct alloca_chunk *) __pthread_running_thread->alloca[0];
#else
  return head;
#endif
}


/* Called by GCC in the middle of a function, denoting the start of a
   block.  i.e.

   char foo(void)
   {
     int x;
     printf ("hello\n");
     {
                         <==== alloca_save called here
       char bar[x];
       ... do something ...
                         <==== alloca_restore called here
     }
     .. do something ...
                         <==== alloca_free called here
   }
*/

int __gcc_alloca_save (void)
{
  struct alloca_chunk *list = chunk_head ();
#ifdef DEBUG
  printf ("__gcc_alloca_save: block=%d\n", (list == NULL) ? 0 : list->block);
#endif

  return (list == NULL) ? 0 : list->block + 1;
}

/* This function is used by GCC to destory allocated blocks at the
   end of a C block.  This function is also used by calls to
   __gcc_alloca_free.  */

/* Walk the linked list of allocated chunks and delete any that match
   the frame pointer or block number.  If 'fix_lr' is set to true, then
   additionally restore the link-return address in the stack frame to
   that of the returning function, therefore undoing the work of
   __gcc_alloca.  */
static void alloca_restore (unsigned int fp, unsigned int block, int fix_lr)
{
  struct alloca_chunk *list = chunk_head ();
  struct alloca_chunk *prev_chunk, *chunk;
  unsigned long return_address = 0;

  if (list == NULL)
    return;

  prev_chunk = list;
  chunk = prev_chunk->next;
  while (chunk)
    {
      /* Delete any allocated chunk that matches the frame pointer
	 of the function, or appeared in some function lower down the
	 call frame.  */
      if (chunk->fp == fp || chunk->block > /* >= */ block)
	{
#ifdef DEBUG
	  printf ("alloca_restore: fp=%08x, ptr=%08x, chunk->block=%d, block=%d, lr=%08x\n",
		  fp, chunk->ptr, chunk->block, block, chunk->lr);
#endif

	  if (chunk->fp == fp && chunk->lr)
	    return_address = chunk->lr;

	  prev_chunk->next = chunk->next;
	  free (chunk->ptr);
	  free (chunk);
	  chunk = prev_chunk;
	}
      prev_chunk = chunk;
      chunk = chunk->next;
    }

#ifdef DEBUG
  __os_print ("here: list="); __os_prhex (list); __os_nl ();
#endif

  /* Finally test the head node.  */
  if (list->fp == fp || list->block > /* >= */ block)
    {
#ifdef DEBUG
      printf ("alloca_restore: fp=%08x, ptr=%08x, chunk->block=%d, block=%d, lr=%08x\n",
	      fp, list->ptr, list->block, block, list->lr);
#endif
      if (! return_address && list->fp == fp && list->lr)
	return_address = list->lr;

#if __UNIXLIB_FEATURE_PTHREADS
      __pthread_running_thread->alloca[0] = list->next;
#else
      head = list->next;
#endif

      free (list->ptr);
      free (list);
    }

#ifdef DEBUG
  __os_print ("here 2\r\n");
#endif

  if (fix_lr)
    {
      /* Re-scan list, if we have deleted all blocks for this function,
	 then we must fix the return address in the stack frame.  */
      chunk = chunk_head ();
      while (chunk)
	{
	  if (chunk->fp == fp)
	    return;
	  chunk = chunk->next;
	}

      if (return_address)
	{
#ifdef DEBUG
	  printf ("alloca_restore: fixing return address to %08x\n",
		  return_address);
#endif
	  /* No matching chunks found, fix lr in stack frame.  */
	  ((unsigned int *)fp)[-1] = return_address;
	}
    }
}

void __gcc_alloca_restore (unsigned int fp, unsigned int block)
{
#ifdef DEBUG
  printf ("__gcc_alloca_restore: fp=%08x, block=%u\n", fp, block);
#endif
  alloca_restore (fp, block, true);
}

/* Free any pre-allocated alloca chunks for the current function.
   Called in function epilogue.  */
unsigned long __gcc_alloca_free_1 (unsigned long fp)
{
  /* As we will already have setup a stack frame on entrance to this
     function, we need to get the frame pointer of the calling function.  */
  unsigned long return_address = 0;
  struct alloca_chunk *chunk;
  unsigned int block = 0xffffffff;
  struct alloca_chunk *list = chunk_head ();

#ifdef DEBUG
  printf ("__gcc_alloca_free: fp=%08x\n", fp);
#endif

  /* Make two passes over the chunk list.  */

  /* The first pass is to deal with calls to setjmp/longjmp.  If we
     have jumped to a different part of the program, then we need
     to ensure that any calls to alloca further down the call frame
     have been freed.  */
  chunk = list;
  while (chunk)
    {
      if (chunk->fp == fp)
	{
	  if (chunk->block < block)
	    block = chunk->block;
	  if (chunk->lr)
	    return_address = chunk->lr;
	}
      chunk = chunk->next;
    }
  if (block != 0xffffffff)
    {
#ifdef DEBUG
      printf ("__gcc_alloca_free: fp=%08x, block=%u, lr=%08x\n",
	      fp, block, return_address);
#endif
      alloca_restore (fp, block, false);
    }

  /* This is the link return address that we will be restoring the program
     counter to.  The wrapper function, __gcc_alloca_free, will perform
     this task for us.  */
  return return_address;
}

void *__gcc_alloca (size_t size)
{
  /* Here, 'fp' points to the frame pointer of the current function
     i.e. that of __gcc_alloca.  */
#ifdef __GNUC__
  register unsigned long *fp __asm ("fp");
#else
  unsigned long *fp = __backtrace_getfp ();
#endif
  /* This is the frame pointer of the function that called __gcc_alloca.  */
  unsigned long *callee_fp = (unsigned long *) fp[-3];
  struct alloca_chunk *chunk;
  struct alloca_chunk *list;

  if (size == 0)
    return NULL;

  list = chunk_head ();

  chunk = (struct alloca_chunk *) malloc (sizeof (struct alloca_chunk));

#ifdef DEBUG
  printf ("__gcc_alloca: list=%08x chunk=%08x size=%d\n",
	  list, chunk, size);
#endif

  chunk->fp = callee_fp[-3];
  chunk->block = (list == NULL) ? 0 : list->block + 1;
  chunk->ptr = (void *) malloc (size); /* Real allocation of memory.  */

  {
    /* This is the return address of the function that called
       __gcc_alloca.  */
    unsigned int callee_func_lr = ((unsigned int *)callee_fp)[-1];
    /* Point to an assembler wrapper that saves any call-clobbered
       registers.  Otherwise we risk corrupting the return address
       of any function.  */
    unsigned int alloca_free = (unsigned int) &__gcc_alloca_free;

#ifdef DEBUG
    printf ("__gcc_alloca: callee_func_lr=%08x, alloca_free=%08x\n",
	    callee_func_lr, alloca_free);
#endif

    if (callee_func_lr == alloca_free)
      {
	/* In this case, a previous call to __gcc_alloca has already
	   modified the return address of the calling function.  */
	chunk->lr = 0;
      }
    else
      {
	/* Modify the return address of the calling function to
	   call __gcc_alloca_free before finally returning.  */
	chunk->lr = callee_func_lr;
	((unsigned int *)callee_fp)[-1] = alloca_free;
      }
  }

  /* Add alloca chunk to the head of the linked list.  */
#if __UNIXLIB_FEATURE_PTHREADS
  chunk->next = (struct alloca_chunk *) __pthread_running_thread->alloca[0];
  __pthread_running_thread->alloca[0] = chunk;
#else
  chunk->next = head;
  head = chunk;
#endif

#ifdef DEBUG
  printf ("__gcc_alloca: fp=%08x, size=%08x, ptr=%08x, block=%d, lr=%08x\n",
	  chunk->fp, size, chunk->ptr, chunk->block, chunk->lr);
#endif

  return chunk->ptr;
}


/* Called exclusively by __pthread_exit, this function frees all
   alloca chunks.  */
void __gcc_alloca_thread_free_all (void)
{
  struct alloca_chunk *list = chunk_head ();
  while (list)
    {
      struct alloca_chunk *chunk = list;
      free (chunk->ptr);
      free (chunk);
      list = list->next;
    }
}


#else

/* Dummy functions, just for paranoid testing.  */

void *__gcc_alloca (size_t size)
{
  return malloc (size);
}

unsigned long __gcc_alloca_free_1 (unsigned long fp)
{
  return 0;
}

void __gcc_alloca_restore (unsigned int fp, unsigned int block)
{
}

int __gcc_alloca_save (void)
{
  return 0;
}

void __gcc_alloca_thread_free_all (void)
{
}

#endif /* DUMMY_IMPLEMENTATION.  */
