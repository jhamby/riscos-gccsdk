/* Implementation of all alloca related functions for GCC 4
   Copyright (c) 2005, 2006, 2007 UnixLib Developers
   Written by Nick Burrett <nick@sqrt.co.uk>
   Reworked by John Tytgat <John.Tytgat@aaug.net>  */

/* At the moment, the assert tests are enabled.  Just to be sure we got
   it right during the development.  */
//#define NDEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <unixlib/local.h>

//#define DEBUG_LOG 1

/* These prototypes need to exist to stop the compiler complaining,
   but they are never referenced directly, so they aren't going to appear
   in any header file.  */
extern unsigned int __gcc_alloca_save (void);
extern void __gcc_alloca_restore (unsigned int fp, unsigned int block);
extern void __gcc_alloca_free (void);
extern unsigned int __gcc_alloca_free_1 (void);
extern void *__gcc_alloca (size_t size);
extern void __gcc_alloca_thread_free_all (void);
extern unsigned int __gcc_alloca_return_address (unsigned int fp);

struct alloca_chunk
{
  unsigned int fp; /* Frame pointer of the *parent* function which called alloca().  */
  unsigned int lr; /* Return address for function where alloca() has been called for the first time (and not yet free'd), or 0 for the next alloca() calls in that same function.  */
  unsigned int block; /* Nesting level starting from 1.  */
  struct alloca_chunk *next;
  char data[1]; /* Start of the claimed alloca block */
};

/* FIXME: when thread context switching is happening during these routines, we have a big problem.  We need some synchronisation code.  */

/* Called by GCC in the middle of a function, denoting the start of a
   block.  i.e.

   char foo(void)
   {
     int x;
     printf ("hello\n");
     {
                         <==== __gcc_alloca_save called here
       char bar[x];      <==== __gcc_alloca called here
       ... do something ...
                         <==== __gcc_alloca_restore called here
     }
     .. do something ...
                         <==== __gcc_alloca_free called here
   }
*/

/* Return head node.  Multi-threaded applications store the head node
   in a private structure.  */
static inline struct alloca_chunk *chunk_head (void)
{
  return (struct alloca_chunk *) __pthread_running_thread->alloca[0];
}


unsigned int __gcc_alloca_save (void)
{
  struct alloca_chunk *list = chunk_head ();
#ifdef DEBUG_LOG
  printf ("__gcc_alloca_save: returning block=%d\n", (list == NULL) ? 1 : list->block + 1);
#endif

  return (list == NULL) ? 1 : list->block + 1;
}


void __gcc_alloca_restore (unsigned int fp, unsigned int block)
{
  unsigned int callee_fp = ((unsigned int *)fp)[-3];
  unsigned int return_address;
  struct alloca_chunk *chunk, *next_chunk;

#ifdef DEBUG_LOG
  printf ("__gcc_alloca_restore: fp=%08x, callee fp=%08x, block=%u\n", fp, callee_fp, block);
#endif
  /* Run over the chunk list until we find block. */
  chunk = chunk_head ();
  while (chunk != NULL && chunk->block > block)
    {
      struct alloca_chunk *next_chunk = chunk->next;
#ifdef DEBUG_LOG
      printf("  ** free (step 1): block %u, ptr %p, callee_fp %08x\n", chunk->block, chunk->data, chunk->fp);
#endif
      free (chunk);
      chunk = next_chunk;
    }
  assert (chunk != NULL);
  assert (chunk->block == block);
  assert (chunk->fp == callee_fp);

  /* Delete chunk with id block.  */
  next_chunk = chunk->next;
  return_address = chunk->lr;
#ifdef DEBUG_LOG
  printf("  ** free (step 1): block %u, ptr %p, callee_fp %08x\n", chunk->block, chunk->data, chunk->fp);
#endif
  free (chunk);
  __pthread_running_thread->alloca[0] = next_chunk;

  if (return_address)
    {
#if !defined(NDEBUG)
      unsigned int alloca_free = (unsigned int) &__gcc_alloca_free;
#endif
#ifdef DEBUG_LOG
      printf ("  ** fixing return address %08x to %08x\n", ((unsigned int *)fp)[-1], return_address);
#endif
      assert (((unsigned int *)fp)[-1] == alloca_free);
      ((unsigned int *)fp)[-1] = return_address;

      assert (next_chunk == NULL || callee_fp != next_chunk->fp);
    }
}


/* Free any pre-allocated alloca chunks for the current function which have
   not yet free'd.
   Called in function epilogue. Returns the real return address for
   caller of __gcc_alloca_free.  */
unsigned int __gcc_alloca_free_1 (void)
{
  register unsigned int *current_fp __asm ("fp");
  unsigned int fp = current_fp[-3];
  unsigned int return_address;
  struct alloca_chunk *chunk;

#ifdef DEBUG_LOG
  printf ("__gcc_alloca_free_1: callee fp=%08x\n", fp);
#endif

  /* Run over the chunk list.  We expect to find at least one chunk holding the
     given fp value and of all chunks holding that fp value, only the last one
     should have a non-zero lr value.  */
  chunk = chunk_head ();
  while (chunk != NULL && chunk->fp != fp)
    {
      struct alloca_chunk *next_chunk = chunk->next;
#ifdef DEBUG_LOG
      printf("  ** free (step 1): block %u, ptr %p, callee fp %08x\n", chunk->block, chunk->data, chunk->fp);
#endif
      free (chunk);
      chunk = next_chunk;
    }
  assert (chunk != NULL);
  assert (chunk->fp == fp);

  /* Delete all chunks with fp */
  while (chunk != NULL && chunk->fp == fp)
    {
      struct alloca_chunk *next_chunk = chunk->next;
      assert (chunk->lr != 0 && (next_chunk == NULL || next_chunk->fp != fp) || chunk->lr == 0 && next_chunk != NULL && next_chunk->fp == fp);
      return_address = chunk->lr;
#ifdef DEBUG_LOG
      printf("  ** free (step 2): block %u, ptr %p, callee fp %08x\n", chunk->block, chunk->data, chunk->fp);
#endif
      free (chunk);
      chunk = next_chunk;
    }
  assert (return_address != 0);
  __pthread_running_thread->alloca[0] = chunk;

  /* This is the link return address that we will be restoring the program
     counter to.  The wrapper function, __gcc_alloca_free, will perform
     this task for us.  */
  return return_address;
}


void *__gcc_alloca (size_t size)
{
  /* Here, 'fp' points to the frame pointer of the current function
     i.e. that of __gcc_alloca.  */
  register unsigned int *fp __asm ("fp");

  /* This is the frame pointer of the function that called __gcc_alloca.  */
  unsigned int *callee_fp = (unsigned int *) fp[-3];
  struct alloca_chunk *chunk;
  struct alloca_chunk *list;

  if (size == 0)
    return NULL;

  chunk = (struct alloca_chunk *) malloc (offsetof (struct alloca_chunk, data) + size);
  /* FIXME: need to check for chunk == NULL like we did in the AOF case and panic ? */

  list = chunk_head ();
#ifdef DEBUG_LOG
  printf ("__gcc_alloca: list=%08x chunk=%08x size=%d fp=%p callee_fp=%p\n",
	  list, chunk, size, fp, callee_fp);
#endif

  chunk->fp = callee_fp[-3];
  chunk->block = (list == NULL) ? 1 : list->block + 1;
  chunk->next = NULL;

  {
    /* This is the return address of the function that called
       __gcc_alloca.  */
    unsigned int callee_func_lr = ((unsigned int *)callee_fp)[-1];
    /* Point to an assembler wrapper that saves any call-clobbered
       registers.  Otherwise we risk corrupting the return address
       of any function.  */
    unsigned int alloca_free = (unsigned int) &__gcc_alloca_free;

#ifdef DEBUG_LOG
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
  chunk->next = (struct alloca_chunk *) __pthread_running_thread->alloca[0];
  __pthread_running_thread->alloca[0] = chunk;

#ifdef DEBUG_LOG
  printf ("__gcc_alloca: fp=%08x, size=%08x, ptr=%08x, block=%d, lr=%08x\n",
	  chunk->fp, size, chunk->data, chunk->block, chunk->lr);
#endif

  return chunk->data;
}


/* Called exclusively by __pthread_exit, this function frees all
   alloca chunks for that thread.  */
void __gcc_alloca_thread_free_all (void)
{
  struct alloca_chunk *list = chunk_head ();
  while (list)
    {
      struct alloca_chunk *next_list = list->next;
      free (list);
      list = next_list;
    }
}


unsigned int __gcc_alloca_return_address (unsigned int fp)
{
  unsigned int callee_fp = ((unsigned int *) fp)[-3];
  unsigned int old_return_address = ((unsigned int *) fp)[-1];
  struct alloca_chunk *chunk;

#ifdef DEBUG_LOG
  printf ("__gcc_alloca_return_address: fp=%08x, callee fp=%08x, rtrn addr=%08x\n", fp, callee_fp, old_return_address);
#endif

  assert (old_return_address == (unsigned int) &__gcc_alloca_free);
  for (chunk = chunk_head(); chunk != NULL && chunk->fp != callee_fp; chunk = chunk->next)
    /* */;
  assert (chunk != NULL);
  for ( ; chunk->fp == callee_fp && chunk->next != NULL; chunk = chunk->next)
    assert ((chunk->lr == 0) ^ (chunk->next->fp != callee_fp));
  assert (chunk->lr != 0);

#ifdef DEBUG_LOG
  printf ("  ** rtrn addr=%08x\n", chunk->lr);
#endif

  return chunk->lr;
}
