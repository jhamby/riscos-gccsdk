/****************************************************************************
 *
 * $Source: $
 * $Date: $
 * $Revision: $
 * $State: $
 * $Author: $
 *
 ***************************************************************************/

/* stackalloc.c: Written by Alex Waugh, January 2002. */

/****************************************************************************
 * This file implements a heap that sits in application space and is intended
 * for allocating stack chunks.
 * The heap is split into blocks, and all memory is allocated in multiples of
 * the block size.
 * Each block has room for 4K of data, plus 8 bytes of overhead.
 * The top and bottom blocks in the heap are dummy blocks of 4 bytes each,
 * which give the appearance of being allocated blocks. This simplifies the
 * checking needed when trying to coalesce blocks.
 * Free blocks are coalesced where possible into groups of consecutive free
 * blocks. The linked list of free blocks contains only the first block in
 * each such group. The final block in each group contains a pointer to the
 * first block in the group.
 *
 * Warning: __stackalloc and __stackfree must not use more than 256 bytes
 *          of stack space
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: $";
#endif

#include <stddef.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <unixlib/features.h>
#include <pthread.h>

#define BLOCK_FREE 0
#define BLOCK_DATA_SIZE 4096

/* This file should be compiled without stack checking, as it is not reentrant */
#ifdef __CC_NORCROFT
#pragma -s1
#endif

struct block
{
  int size; /* number of consecutive blocks allocated, or BLOCK_FREE for a free block */
  union
  {
    struct
    {
      struct block *nextfreeblock; /* Next non-consecutive block in the freelist linked list */
      int numconsecutiveblocks; /* Number of consecutive free blocks */
    } free;
    char allocated[BLOCK_DATA_SIZE]; /* The actual payload space returned to the user */
  } contents;
  struct block *startofcon; /* First block in this group of consecutive free blocks.
                               NULL if this block is allocated.
                               Only valid in the last block in the group */
};


static struct block *freelist; /* Head of linked list of free blocks */
static struct block *dummybottomblock; /* dummy block header at the top of the heap */


/* Remove a block from the freelist */
#define REMOVE_FROM_FREELIST(remove) \
{ \
  struct block *blocklist = freelist; \
\
  if (freelist == remove) \
    { \
      freelist = remove->contents.free.nextfreeblock; \
    } \
  else \
    { \
      while (blocklist && blocklist->contents.free.nextfreeblock != remove) \
        blocklist = blocklist->contents.free.nextfreeblock; \
\
      if (blocklist) \
        blocklist->contents.free.nextfreeblock = remove->contents.free.nextfreeblock; \
    } \
}

/* Add a block to the start of the freelist */
#define ADD_TO_FREELIST(add) \
{ \
  add->contents.free.nextfreeblock = freelist; \
  freelist = add; \
}

/* The adding to the freelist could perhaps be a bit more intelligent and
   add free blocks in an order such that when the freelist is searched
   blocks higher in the heap will be found first, which should help reduce
   fragmentation in a multithreaded program */


/* Increase the amount of memory allocated to the heap */
/* Returns the new value of __stack, or NULL if unable to increase */
static void *
__stackalloc_incr (int incr)
{
  char *new__stack;

#ifdef DEBUG
  __os_print ("-- __stackalloc_incr: incr = "); __os_prhex (incr); __os_print ("\r\n");
  __os_print ("-- __stackalloc_incr: __stack = "); __os_prhex ((int)__stack); __os_print ("\r\n");
  __os_print ("-- __stackalloc_incr: __stack_limit = "); __os_prhex ((int)__stack_limit); __os_print ("\r\n");
#endif

  /* TODO: increase wimpslot if needed (won't be possible if we are a child process though) */
  new__stack = (char *)__stack - incr;
  if (new__stack < (char *)__stack_limit)
    return NULL;

  __stack = new__stack;

  return __stack;
}

/* Release any free memory at the bottom of the heap */
/* Returns non-zero if it managed to free anything */
int
__stackalloc_trim (void)
{
  struct block *bottomblock;

#if __FEATURE_PTHREADS
  if (__pthread_system_running)
    __pthread_disable_ints ();
#endif

  /* Find start of the last real block in the heap */
  bottomblock = dummybottomblock + 1;

  if (bottomblock->size != BLOCK_FREE)
    {
#if __FEATURE_PTHREADS
      if (__pthread_system_running)
        __pthread_enable_ints ();
#endif
      return 0; /* Unable to free anything */
    }

  REMOVE_FROM_FREELIST (bottomblock);

  /* Move dummy block up to where the end of the bottom block was */
  dummybottomblock = bottomblock + bottomblock->contents.free.numconsecutiveblocks - 1;
  dummybottomblock->startofcon = NULL;
  __stack = (char *)dummybottomblock + sizeof(struct block) - 4;

#ifdef DEBUG
  __os_print ("-- __stackalloc_trim: __stack = "); __os_prhex ((int)__stack); __os_print ("\r\n");
#endif

#if __FEATURE_PTHREADS
  if (__pthread_system_running)
    __pthread_enable_ints ();
#endif

  return 1;
}

/* Initialise the stackalloc heap */
/* No need for thread-safety, as only called before threads initialised */
void
__stackalloc_init (void)
{
  struct block *initialblock;

  /* The initial stack chunk is set up in _syslib.s */
  /* __stack points 8 bytes below the base of the initial chunk */

#ifdef DEBUG
  __os_print ("-- __stackalloc_init: __stack = "); __os_prhex ((int)__stack); __os_print ("\r\n");
#endif

  initialblock = (struct block *)((char *)__stack + 4);
  initialblock->size = 1;

  dummybottomblock = initialblock - 1;
  dummybottomblock->startofcon = NULL;

  freelist = NULL;
}

/* Allocate some space from the heap - equivalent to malloc() */
void *
__stackalloc (size_t size)
{
  int blocksneeded, blocksleft;
  struct block *blocklist;
  struct block *blocktoalloc;
  struct block *lastblocktoalloc;
  void *returnptr;

#if __FEATURE_PTHREADS
  if (__pthread_system_running)
    __pthread_disable_ints ();
#endif

#ifdef DEBUG
  __os_print ("-- __stackalloc: size = "); __os_prhex (size); __os_print ("\r\n");
#endif
  /* Convert size into number of blocks */
  blocksneeded = size + BLOCK_DATA_SIZE - 1;
  blocksneeded &= ~(BLOCK_DATA_SIZE - 1);
  blocksneeded /= BLOCK_DATA_SIZE; /* BLOCK_DATA_SIZE should be a power of two so this should optimise to a shift */
#ifdef DEBUG
  __os_print ("-- __stackalloc: blocksneeded = "); __os_prhex (blocksneeded); __os_print ("\r\n");
#endif

  /* Search through freelist to find the first smallest block that is large enough for this request*/
  blocklist = freelist;
  blocktoalloc = NULL;
  while (blocklist)
    {
      if (blocklist->contents.free.numconsecutiveblocks == blocksneeded)
        {
          /* We have found a block of exactly the right size, so no need to search any further */
          blocktoalloc = blocklist;
          break;
        }
      else
        {
          if (blocklist->contents.free.numconsecutiveblocks > blocksneeded)
            {
              if (blocktoalloc == NULL || blocklist->contents.free.numconsecutiveblocks < blocktoalloc->contents.free.numconsecutiveblocks)
                {
                  /* We have found a block that is smaller then the best so far,
                     but still bigger than needed so carry on searching */
                  blocktoalloc = blocklist;
                }
            }
        }
      blocklist = blocklist->contents.free.nextfreeblock;
    }

  if (blocktoalloc == NULL)
    {
      int realblocksneeded = blocksneeded;
      struct block *bottomblock = dummybottomblock + 1;

      /* No suitable block found, so try to increase the size of the heap */
      if (bottomblock->size == BLOCK_FREE)
        {
          /* We can use the bottom block to provide part of the new block */
          realblocksneeded -= bottomblock->contents.free.numconsecutiveblocks;
          REMOVE_FROM_FREELIST (bottomblock);
        }

      blocktoalloc = bottomblock - realblocksneeded;

      if (__stackalloc_incr (realblocksneeded * sizeof(struct block)) == NULL)
        {
#ifdef DEBUG
          __os_print ("-- __stackalloc: no free memory"); __os_print ("\r\n");
#endif
#if __FEATURE_PTHREADS
          if (__pthread_system_running)
            __pthread_enable_ints ();
#endif
          return NULL;
        }

      /* Setup a new dummy block at the bottom of the heap */
      dummybottomblock = blocktoalloc - 1;
      dummybottomblock->startofcon = NULL;
    }
  else
    {
      /* A suitable block was found, so use it */
      REMOVE_FROM_FREELIST (blocktoalloc);
      blocksleft = blocktoalloc->contents.free.numconsecutiveblocks - blocksneeded;
      if (blocksleft > 0)
        {
          /* The block is bigger than we need, so add the remainder back to the freelist */
          struct block *newfreeblock;
          struct block *lastfreeblock;

          newfreeblock = blocktoalloc + blocksneeded;
          lastfreeblock = blocktoalloc + blocktoalloc->contents.free.numconsecutiveblocks - 1;

          ADD_TO_FREELIST (newfreeblock);
          newfreeblock->contents.free.numconsecutiveblocks = blocksleft;
          newfreeblock->size = BLOCK_FREE;
          lastfreeblock->startofcon = newfreeblock;
        }
    }

  /* At this point, blocktoalloc contains a block of exactly the right size, and is not in the freelist */
  blocktoalloc->size = blocksneeded;
  lastblocktoalloc = blocktoalloc + blocksneeded - 1;
  lastblocktoalloc->startofcon = NULL;

  returnptr = ((char*)blocktoalloc) + offsetof (struct block, contents.allocated);
#ifdef DEBUG
  __os_print ("-- __stackalloc: returning "); __os_prhex ((int)returnptr); __os_print ("\r\n");
#endif

#if __FEATURE_PTHREADS
  if (__pthread_system_running)
    __pthread_enable_ints ();
#endif

  return returnptr;
}

/* Free a chunk - equivalent to free() */
void
__stackfree (void *ptr)
{
  struct block *blocktofree;
  struct block *prevblock;
  struct block *startblock;
  struct block *nextblock;
  struct block *lastblock;

#if __FEATURE_PTHREADS
  if (__pthread_system_running)
    __pthread_disable_ints ();
#endif

#ifdef DEBUG
  __os_print ("-- __stackfree: ptr = "); __os_prhex ((int)ptr); __os_print ("\r\n");
#endif

  /* Find start of block this ptr refers to */
  blocktofree = (struct block *)((char *)ptr - offsetof (struct block, contents.allocated));

  prevblock = blocktofree - 1;
  startblock = prevblock->startofcon;
  if (startblock == NULL)
    {
      /* Block below this is not free, so we need to add this block to the freelist */
#ifdef DEBUG
      __os_print("-- __stackfree: adding to free list"); __os_print ("\r\n");
#endif
      startblock = blocktofree;
      startblock->contents.free.numconsecutiveblocks = 0;
      ADD_TO_FREELIST (startblock);
    }
#ifdef DEBUG
  else
    {
      /* Block below this is free, so we can coalesce with it and hence don't need
         to add this block to the freelist */
      __os_print ("-- __stackfree: coalescing with lower block"); __os_print ("\r\n");
    }
#endif

  startblock->contents.free.numconsecutiveblocks += blocktofree->size;

  nextblock = blocktofree + blocktofree->size;
  if (nextblock->size == BLOCK_FREE)
    {
      /* Block above this is free, so coalesce with it.
         This block has already been added to the freelist,
         so we need to remove the nextblock from the freelist */
#ifdef DEBUG
      __os_print ("-- __stackfree: coalescing with higher block"); __os_print ("\r\n");
#endif
      REMOVE_FROM_FREELIST (nextblock);
      startblock->contents.free.numconsecutiveblocks += nextblock->contents.free.numconsecutiveblocks;
    }

  lastblock = startblock + startblock->contents.free.numconsecutiveblocks - 1;
  lastblock->startofcon = startblock;
  blocktofree->size = BLOCK_FREE;

#if __FEATURE_PTHREADS
  if (__pthread_system_running)
    __pthread_enable_ints ();
#endif
}

