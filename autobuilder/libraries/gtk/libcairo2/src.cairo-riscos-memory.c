/*
 * cairo-riscos-memory.c
 * 
 * Author:
 * 	Lee Noar
 *
 * Memory allocator using a dynamic area for large blocks of memory.
 * Uses a doubly linked list to maintain a list of free blocks.
 * The free blocks are stored in order of ascending address.
 * The caller can free just part of a block of memory as long as it's
 * fully contained within a previously allocated block.
 */

#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cairo-riscos.h"

#include "oslib/os.h"

//#define SANITY_CHECK 1

#if defined (STANDALONE_TEST) && defined (DEBUG_MEMORY)
#define SANITY_CHECK 1
#endif

#define MAX_DA_SIZE (200 * 1024 * 1024)
#define INIT_DA_SIZE (4 * 1024)

typedef struct dynamic_area
{
  int handle;
  byte *base_addr;
  size_t size;
} dynamic_area;

typedef struct memory_node
{
  struct memory_node *prev, *next;
  
  byte *addr;
  size_t size;
} memory_node;

typedef struct memory_node_list
{
  memory_node *first;
  memory_node *last;
  int count;
} memory_node_list;

static memory_node_list free_list;

static dynamic_area cairo_da = { -1, NULL, 0 };

#ifdef CAIRO_RISCOS_DEBUG_MEMORY
void dump_memory_node_list (const char *title)
{
    memory_node *mn;
    if (title)
	printf ("%s\n", title);
    else
	printf ("Free list\n");
    if (!free_list.first)
	printf ("  <Empty>\n");
    else
    {
	for (mn = free_list.first; mn; mn = mn->next)
		printf (">   %p -> %p, %lX\n", mn->addr, mn->addr + mn->size, mn->size);
	putchar ('\n');
    }
}
#endif

void
cairo_riscos_finalise (void)
{
    if (cairo_da.handle != -1)
      {
	os_error *err = xosdynamicarea_delete (cairo_da.handle);
	cairo_da.handle = -1;
	if (err)
	  printf ("%s\n",err->errmess);
      }
}

static memory_node *
memory_node_create (void *addr, int len)
{
    memory_node *mn = malloc (sizeof (memory_node));
    if (!mn)
	return NULL;

    mn->prev = mn->next = NULL;
    mn->addr = addr;
    mn->size = len;

    return mn;
}

static void
memory_node_add_to_end (memory_node_list *list, memory_node *link)
{
    memory_node *oldlast;

    oldlast = list->last;
    link->next = NULL;
    link->prev = oldlast;

    if (oldlast == NULL)
	list->first = link;
    else
	oldlast->next = link;

    list->last = link;
    list->count++;
}

static void
memory_node_add_to_front (memory_node_list *list, memory_node *link)
{
    memory_node *oldfirst;

    oldfirst = list->first;
    link->next = oldfirst;
    link->prev = NULL;

    if (oldfirst == NULL)
	list->last = link;
    else
	oldfirst->prev = link;

    list->first = link;
    list->count++;
}

static void
memory_node_add_before (memory_node_list *list, memory_node *at, memory_node *link)
{
    memory_node *oldprev;

    if (at == NULL)
    {
	memory_node_add_to_end (list, link);
	return;
    }

    oldprev = at->prev;
    if (oldprev == NULL)
	memory_node_add_to_front (list, link);
    else
    {
	oldprev->next = link;
	link->prev = oldprev;
	at->prev = link;
	link->next = at;
	list->count++;
    }
}

static void
memory_node_remove (memory_node_list *list, memory_node *link)
{
    memory_node *prev, *next;

    next = link->next;
    prev = link->prev;

    if (next == NULL)
	list->last = prev;
    else
	next->prev = prev;

    if (prev == NULL)
	list->first = next;
    else
	prev->next = next;

    link->next = link->prev = NULL;
    list->count--;
}

static const char *get_da_name (void)
{
    static char name [24];

#ifdef CAIRO_RISCOS_DEBUG_MEMORY
    sprintf (name, "Cairo %d\n", os_read_monotonic_time());
#else
    strcpy (name, "Cairo");
#endif
	
    return name;
}

static int
cairo_riscos_memory_allocator_init (void)
{
    os_error *err;
    memory_node *free_block;

    err = xosdynamicarea_create (-1, INIT_DA_SIZE, (byte *)-1, os_AREA_NO_USER_DRAG, MAX_DA_SIZE, 0, 0,
				 get_da_name (), &cairo_da.handle, &cairo_da.base_addr, NULL);
    if (err)
	return -1;

    cairo_da.size = INIT_DA_SIZE;

    /* Create a free block for the initial DA allocation.  */
    if ((free_block = memory_node_create (cairo_da.base_addr, INIT_DA_SIZE)) == NULL)
    {
	xosdynamicarea_delete (cairo_da.handle);
	return -1;
    }

    atexit(cairo_riscos_finalise);

    memory_node_add_to_end (&free_list, free_block);

    return 0;
}

/* cairo_riscos_memory_alloc
 *
 * Allocate a block of memory of the given size.
 * 
 * First the free list is searched for a block of exact size. If that fails,
 * then the first block that is big enough is used.
 */
void *
cairo_riscos_memory_alloc (size_t size)
{
    byte *ptr = NULL;
    memory_node *mn;

    /* Initialise on first use.  */
    if (cairo_da.handle < 0)
	if (cairo_riscos_memory_allocator_init () < 0)
	    return NULL;

#ifdef CAIRO_RISCOS_DEBUG_MEMORY
    printf ("* %s (%lX)\n", __func__, size);
    dump_memory_node_list ("> Free list before allocation");
#endif

    /* Search for block of exact size.  */
    for (mn = free_list.first;
	 mn && size != mn->size;
	 mn = mn->next)
	  /* Empty loop. */;

    if (mn)
    {
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	printf ("> Requested size same as free block (%p -> %p, %lX)\n", mn->addr, mn->addr + mn->size, mn->size);
#endif
	ptr = mn->addr;
	memory_node_remove (&free_list, mn);
	free (mn);
    }
    else
    {
	/* Search for block that is big enough for the requested size.  */
	for (mn = free_list.first;
	     mn && size > mn->size;
	     mn = mn->next)
	  /* Empty loop.  */;

	if (mn)
	{
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    printf ("> Requested size fits within free block (%p -> %p, %lX)\n", mn->addr, mn->addr + mn->size, mn->size);
#endif
	    ptr = mn->addr;
	    mn->addr += size;
	    mn->size -= size;
	}
    }

    /* If allocation from free list failed, then extend DA.  */
    if (ptr == NULL)
    {
	os_error *err;
	size_t da_inc;
	memory_node *last = free_list.last;

	if (last && (last->addr + last->size) == cairo_da.base_addr + cairo_da.size)
	{
	    /* There's a free block at the end of the DA, but it's not big enough.
	     * Extend the DA so the block becomes big enough.  */
	    da_inc = ((size - last->size) + 0xfff) & ~0xfff;

	    if ((err = xos_change_dynamic_area (cairo_da.handle, da_inc, NULL)) != NULL)
		return NULL;

	    cairo_da.size += da_inc;
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    printf ("> No suitable free blocks; extend DA by 0x%lX to 0x%lX\n", da_inc, cairo_da.size);
	    printf ("> Use free block (%p -> %p, %lX) at end of (extended) DA\n", last->addr, last->addr + last->size, last->size);
#endif
	    /* There's a free block at the end, and now that the DA has been extended, it's
	     * big enough for this allocation.  */
	    ptr = last->addr;

	    if (last->size + da_inc - size == 0)
	    {
		/* This allocation consumed the whole of the last free block; remove it
		 * from the list.  */
		memory_node_remove (&free_list, last);
		free (last);
	    }
	    else
	    {
		last->addr += size;
		last->size += da_inc - size;
	    }
	}
	else
	{
	    size_t old_da_size = cairo_da.size;

	    da_inc = (size + 0xfff) & ~0xfff;

	    if ((err = xos_change_dynamic_area (cairo_da.handle, da_inc, NULL)) != NULL)
		return NULL;

	    cairo_da.size += da_inc;
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    printf ("> No suitable free blocks; extend DA by 0x%lX to 0x%lX\n", da_inc, cairo_da.size);
#endif
	    /* There is not a free block at the end. Allocate the block and create a free block for
	     * the rest.  */
	    ptr = cairo_da.base_addr + old_da_size;
	    if (da_inc - size > 0)
	    {
	    	if ((mn = memory_node_create (ptr + size, da_inc - size)) == NULL)
		    return NULL;
		memory_node_add_to_end (&free_list, mn);
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
		printf ("> Allocate block at end of (extended) DA, create free block for remainder (%p -> %p, %lX)\n", mn->addr, mn->addr + mn->size, mn->size);
#endif
	    }
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    else
	    {
		printf ("> Allocate block at end of (extended) DA, no remainder\n");
	    }
#endif
	}
    }

#ifdef CAIRO_RISCOS_DEBUG_MEMORY
    dump_memory_node_list ("> Free list after allocation");
    printf ("> Block (%p -> %p, 0x%lX) allocated\n", ptr, ptr + size, size);
#endif

    return ptr;
}

void *
cairo_riscos_mmap (size_t size, int flags)
{
    void *ptr = cairo_riscos_memory_alloc (size);

    if (ptr && flags & MAP_ANON)
	memset (ptr, 0, size);

    return ptr;
}

/* Free a block of memory.
 * 
 * The specified block need not have the same address or size as a previously allocated
 * block as long as an allocated block fully contains it.  */
int
cairo_riscos_memory_free (void *addr, size_t len)
{
    memory_node *search;

    if (addr == NULL)
    {
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	fprintf (stderr,"%s: Attempt to free NULL address.\n", __func__);
#endif
	return -1;
    }

    if (len == 0)
    {
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	fprintf (stderr,"%s: Attempt to free zero sized memory block.\n", __func__);
#endif
	return -1;
    }

    if ((byte *)addr < cairo_da.base_addr || (byte *)addr >= cairo_da.base_addr + cairo_da.size)
    {
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	fprintf (stderr,"%s: Attempt to free address outside dynamic area.\n", __func__);
#endif
	return -1;
    }

#ifdef CAIRO_RISCOS_DEBUG_MEMORY
    printf ("* %s (%p -> %p, %lX)\n", __func__, addr, (byte *)addr + len, len);
    dump_memory_node_list ("* Free list before merge");
#endif

#ifdef SANITY_CHECK
    for (search = free_list.first;
	 search;
	 search = search->next)
    {
	if (((byte *)addr >= search->addr &&
	     (byte *)addr + len < search->addr + search->size) ||
	    ((byte *)addr < search->addr &&
	     (byte *)addr + len > search->addr && 
	     (byte *)addr + len < search->addr + search->size) ||
	    ((byte *)addr >= search->addr &&
	     (byte *)addr < search->addr + search->size &&
	     (byte *)addr + len > search->addr + search->size))
	{
	    break;
	}
    }

    if (search != NULL)
    {
	printf (">>> %s: Attempt to free block (%p -> %p, %X) outside bounds of any previously allocated block.\n", __func__, addr, addr + len, len);
	return -1;
    }
#endif

    /* Find the correct position in the list.  */
    for (search = free_list.first;
	 search && (byte *)addr >= search->addr;
	 search = search->next)
	/* Empty loop.  */;

    if (search == NULL)
    {
	/* Either the list is empty in which case we create the first node, or the end of the
	 * list was reached which means that this block has a higher address than all others.
	 * Either way add it to the end of the list.  */
	memory_node *mn;

	if ((mn = memory_node_create (addr, len)) == NULL)
	    return -1;
	memory_node_add_to_end (&free_list, mn);
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	printf ("* Could not merge; Add free node to end (%p -> %p, %lX)\n",
		mn->addr, mn->addr + mn->size, mn->size);
#endif
    }
    else
    {
	memory_node *prev = search->prev;
	int merge_count = 0;

	if ((byte *)addr + len == search->addr)
	{
	    /* The block to be free'd is adjacent to a free list block after it, merge it into
	     * that.  */
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    printf ("* Block (%p -> %p, %lX) merged into next block (%p -> %p, %lX)\n",
		    addr, addr + len, len, search->addr, search->addr + search->size, search->size);
#endif
	    search->addr = addr;
	    search->size += len;
	    merge_count++;
	}
	if (prev && prev->addr + prev->size == addr)
	{
	/* The block to be free'd is adjacent to a free list block before it, merge it into
	 * that.  */
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    printf ("* Block (%p -> %p, %lX) merged into previous block (%p -> %p, %lX)\n",
		addr, addr + len, len, prev->addr, prev->addr + prev->size, prev->size);
#endif
	    prev->size += len;
	    merge_count++;
	}
	if (merge_count == 0)
	{
	    /* Block was not merged into either previous or next block, so insert a new
	     * free list node at the correct place.  */
	    memory_node *mn;

	    if ((mn = memory_node_create (addr, len)) == NULL)
		return -1;
	    memory_node_add_before (&free_list, search, mn);
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    printf ("* Could not merge; Insert free node (%p -> %p, %lX)\n",
		    mn->addr, mn->addr + mn->size, mn->size);
#endif
	}
	else if (merge_count == 2)
	{
	    /* Block was merged into previous and next block. Expand the previous block to
	     * cover the whole area. The next block is no longer required and can be removed.  */
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    printf ("* Block merged into previous and next block. Removing surplus node.\n");
#endif
	    prev->size += search->size - len;
	    memory_node_remove (&free_list, search);
	    free (search);
	}
    }

#ifdef CAIRO_RISCOS_DEBUG_MEMORY
    dump_memory_node_list ("* Free list after merge");
#endif

    return 0;
}

void *
cairo_riscos_map_file (size_t length, int prot, int flags, int fd, off_t offset)
{
    char *ptr = NULL;

    if (fd != -1)
    {
	off_t old_offset = lseek (fd, 0, SEEK_CUR);

	if ((ptr = cairo_riscos_memory_alloc (length)) == NULL)
	{
#ifdef CAIRO_RISCOS_DEBUG_MEMORY
	    fprintf (stderr, "%s: Failed to allocate memory to map file.\n", __func__);
#endif
	    return (void *)-1;
	}

	lseek (fd, offset, SEEK_SET);
	read (fd, ptr, length);
	lseek (fd, old_offset, SEEK_SET);
    }

    return ptr;
}

#if defined (CAIRO_RISCOS_DEBUG_MEMORY) && defined (STANDALONE_TEST)

int main (void)
{
    char *ptr = cairo_riscos_memory_alloc (0x8000);

    printf ("  Allocate; %X bytes, ptr = %p\n", 0x8000, ptr);
	
    printf ("  Free; free list empty create node\n");
    cairo_riscos_memory_free (ptr + 0x100, 0x100);

    printf ("  Free; no merging, create node at end\n");
    cairo_riscos_memory_free (ptr + 0x500, 0x100);

    printf ("  Free; no merging, create node midway\n");
    cairo_riscos_memory_free (ptr + 0x300, 0x100);

    printf ("  Free; merging with previous block\n");
    cairo_riscos_memory_free (ptr + 0x400, 0x80);

    printf ("  Free; merging with next block\n");
    cairo_riscos_memory_free (ptr + 0x280, 0x80);

    printf ("  Free; merging with first block\n");
    cairo_riscos_memory_free (ptr + 0x80, 0x80);

    printf ("  Free; merging with previous and next block\n");
    cairo_riscos_memory_free (ptr + 0x200, 0x80);

    printf ("  Free; outside allocated block\n");
    cairo_riscos_memory_free (ptr + 0x280, 0x80);

    return 0;
}

#endif
