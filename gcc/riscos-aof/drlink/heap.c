/*
** Drlink AOF linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** This module contains the functions concerned with heap
** management.
**
** Memory management is complicated by the fact that the linker
** basically carries out its own. It acquires large chunks via 'malloc'
** and then hands out lumps as required. Free storage is returned
** to the heap managed by this code.
**
** Details of any free storage is kept in two places:
** a) There is an array of pointers to lists of free areas
**    of each size from one to ten words in length
** b) Larger areas than this are kept in a single linked list.
**
** When a request is made for an area of storage of less than
** ten words, the free list array is checked first. For larger
** requests or cases where the array contains no suitable
** entries, the free list is checked and space taken from the
** first area large enough. If the request still cannot be
** fulfilled, memory is acquired using 'malloc'. Under RISC OS,
** the size of the wimpslot is automatically increased if
** necessary by the Shared C Library.
*/

#include "config.h"

#include <stdlib.h>

#include "drlhdr.h"
#include "procdefs.h"

/* #define TRACE	Not tracing calls for now */

typedef struct blockinfo {
  struct blockinfo *blocknext;	/* Address of next block */
  unsigned int blocksize;	/* Size of this block */
} blockinfo;

#define MINALLOC (sizeof(blockinfo))	/* Smallest block allocated */
#define MALLOCSIZE 4096			/* Size of block malloc'ed each time */
#define MAXFREESIZE 10			/* Individual word size free lists */
#define MAXFREEBYTES (MAXFREESIZE*sizeof(int))

#ifdef TARGET_RISCOS
#define MEMLIMIT 0		/* OS_ChangeEnvironment calls */
#define APPLIMIT 14
#endif

/* Private declarations */

static char
  *low_heapnext,		/* Next free address in current heap block */
  *low_heaptop;			/* Top of current heap block */

static blockinfo
  *malloclist,			/* List of blocks acquired using malloc */
  *blocklist,			/* Free memory block list */
  *blocklast;			/* Last entry in free memory block list */

static blockinfo *freelist[MAXFREESIZE+1];	/* Small block free memory lists */

static unsigned int
  mallocsize;			/* Amount of memory acquired via malloc */

static bool
  libs_gone;			/* TRUE if libraries have been discarded as memory is very low */

/*
** 'allocmem' is the memory allocation routine. It assigns memory
** from two areas, a list of free memory slots and the heap.
** Memory for the heap is acquired (in 4K chunks) by using 'malloc'.
** If no memory is available, the function tries to free some by
** discarding any libraries that have been loaded. This can only be
** attempted if the libraries have not been referenced, that is, at
** the time AOF files are being hoiked into memory. If this fails,
** there is not a lot that can be done... The proc returns a pointer
** to the memory allocated or NIL if the request could not be fulfilled.
** It is up to the routines that call 'allocmem' to deal with this
** condition.
*/
void *allocmem(unsigned int size) {
  char *area, *newnext;
  blockinfo *lp, *mp, *np, *p;
  unsigned int wordsize, bitleft;
  size = (size+sizeof(int)-1) & -sizeof(int);	/* Round size up to next word boundary */
  if (size<MINALLOC) size = MINALLOC;
  if (size<=MAXFREEBYTES) {	/* Look for entry in small block free list array */
    wordsize = size/sizeof(unsigned int);
    if ((mp = freelist[wordsize])!=NIL) {	/* Found one */
      freelist[wordsize] = mp->blocknext;
      return mp;
    }
  }
  if ((p = blocklist)!=NIL) {   /* Check free block list */
    lp = NIL;
    while (p!=NIL && p->blocksize<size) {
      lp = p;
      p = p->blocknext;
    }
    if (p!=NIL) {  /* Found one big enough */
#ifdef TRACE
      printf("....  freelist allocate %x at %x\n", size, p);
#endif
      area = COERCE(p, char *);
      bitleft = p->blocksize-size;
      if (bitleft>=sizeof(blockinfo)) {	/* Block is too big - Release end of it */
        np = COERCE(COERCE(p, char *)+size, blockinfo *);
        if (bitleft<=MAXFREEBYTES) {	/* Add chunk to one of the small block lists */
          wordsize = bitleft/sizeof(unsigned int);
          np->blocknext = freelist[wordsize];
          freelist[wordsize] = np;
/* Remove entire block from free memory chain */
          if (lp==NIL) {	/* Allocated block was first in list */
            blocklist = p->blocknext;
          }
          else {
            lp->blocknext = p->blocknext;
          }
          if (blocklast==p) {	/* Allocated old last block in chain? */
            blocklast = lp;
          }
        }
        else {	/* Remainder is too big for small chunk lists */
          np->blocksize = bitleft;
          np->blocknext = p->blocknext;
          if (lp==NIL) {	/* Allocated block was first in list */
            blocklist = np;
          }
          else {
            lp->blocknext = np;
          }
          if (p==blocklast) {	/* Area allocated was from last block in chain */
           blocklast = np;
          }
        }
      }
      else {	/* Use entire block */
        if (lp==NIL) {  /* Allocated block was first in list */
          blocklist = p->blocknext;
        }
        else {
          lp->blocknext = p->blocknext;
        }
        if (blocklast==p) {	/* Allocated old last block in chain? */
          blocklast = lp;
        }
      }
      return area;
    }
  }
/*
** There is nothing suitable on the free memory lists. Allocate from heap
*/
  if (size>MALLOCSIZE) {	/* Want more memory than is available in malloc'ed blocks */
    mp = malloc(size+sizeof(blockinfo));
    if (mp!=NIL) {	/* Acquired memory okay */
      mp->blocknext = malloclist;
      malloclist = mp;
      mallocsize+=size+sizeof(blockinfo);
      return COERCE(mp+1, void *);
    }
  }
  else {
    area = low_heapnext;
    newnext = area+size;
    if (newnext<low_heaptop) {	/* No problem. Allocate memory from heap */
#ifdef TRACE
    printf("....  Allocate %x at %x\n", size, area);
#endif
      low_heapnext = newnext;
      return area;
    }
    else {	/* Need to acquire new block of memory */
      mp = malloc(MALLOCSIZE+sizeof(blockinfo));
      if (mp!=NIL) {	/* Acquired block okay. Add it to malloc'ed list and allocate space from it */
        mp->blocknext = malloclist;
        malloclist = mp;
        mallocsize+=MALLOCSIZE+sizeof(blockinfo);
        area = COERCE(mp+1, void *);
        low_heaptop = area+MALLOCSIZE;
        low_heapnext = area+size;
        return area;
      }
    }
  }
/*
** There was insufficient memory available. Throw away any libraries
** loaded as a last gasp attempt to continue
*/
  if (!libs_gone && link_state==READ_AOF) {
    if (opt_verbose) error("Drlink: Linker is running very short of memory");
    low_memory = libs_gone = TRUE;
    resize_filebuffer();
    discard_libraries();
    return allocmem(size);
  }
  return NIL;	/* There is no memory left */
}

/*
** 'freemem' returns memory to the heap. For blocks of up to
**'MAXFREESIZE' words in size,  a separate list is kept for each
** word size. Anything larger is added to a single free memory
** list. Note that new areas are added to the end of this list.
** The idea here is that as  most chunks requested are twenty bytes
** or less in size, the chunks allocated will all be taken from one
** or two blocks at the start of the list, leaving larger ones
** further down the list relatively intact and available for the
** occasional large request.
*/
void freemem(void *where, unsigned int size) {
  blockinfo *mp;	/* Just to stop having to coerce everything in sight */
  mp = where;
#ifdef TRACE
  printf(">>>>  Freeing %x at %x\n", size, where);
#endif
  if (size<=MAXFREEBYTES) {	/* Small area. Add area to free list array */
    size = size/sizeof(unsigned int);
    mp->blocknext = freelist[size];
    freelist[size] = mp;
  }
  else {	/* Large area. Add to large area list */
    mp->blocknext = NIL;
    mp->blocksize = size;
    if (blocklast==NIL) {
      blocklist = mp;
    }
    else {
      blocklast->blocknext = mp;
    }
    blocklast = mp;
  }
}

/*
** 'initheap' is called at the start of the run to establish the heap
*/
bool initheap(void) {
  int n;
  malloclist = malloc(MALLOCSIZE+sizeof(blockinfo));
  if (malloclist==NIL) {
    error("Fatal: There is not enough memory to run the linker");
  }
  malloclist->blocknext = NIL;
  mallocsize = MALLOCSIZE+sizeof(blockinfo);
  low_heapnext = COERCE(malloclist+1, char *);
  low_heaptop = low_heapnext + MALLOCSIZE;
  for (n = 0; n<=MAXFREESIZE; n++) freelist[n] = NIL;
  blocklist = blocklast = NIL;
  low_memory = libs_gone = FALSE;
  #ifdef TRACE
  printf("First heap block is at: %x ends at: %x\n",
   low_heapnext, low_heaptop);
#endif
  return TRUE;
}

/*
** 'release heap' is called at the end to free all memory malloc'ed
** by the linker
*/
void release_heap(void) {
  blockinfo *p, *np;
  p = malloclist;
  while (p!=NIL) {
    np = p->blocknext;
    free(p);
    p = np;
  }
}

/*
** 'print_heapstats' prints some information on the amount
** of memory used by the program
*/
void print_heapstats(void) {
  error("Drlink: Linking the program required %dK bytes of memory",
   mallocsize/1024);
}
