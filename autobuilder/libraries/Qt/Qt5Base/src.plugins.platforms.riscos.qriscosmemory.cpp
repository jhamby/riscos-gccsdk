/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the RISC OS platform plugin of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
 *
 * Written by Lee Noar
 *
 * Memory allocator using a dynamic area for large blocks of memory.
 * Uses a doubly linked list to maintain a list of free blocks.
 * The free blocks are stored in order of ascending address.
 * The caller can free just part of a block of memory as long as it's
 * fully contained within a previously allocated block.
 */

#include <kernel.h>
#include <swis.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "oslib/os.h"
#include "qriscos.h"

#if defined (DEBUG_MEMORY)
#define SANITY_CHECK 1
#endif

#define MAX_DA_SIZE (200 * 1024 * 1024)
#define INIT_DA_SIZE (4 * 1024)

#ifdef DEBUG_MEMORY
void 
QDynamicArea::dump (const char *title)
{
    memory_node *mn;
    if (title)
	printf ("%s\n", title);
    else
	printf ("Free list\n");
    if (m_freeList.count())
    {
	for (mn = m_freeList.first(); mn; mn = mn->next())
	    printf (">   %p -> %p, %lX\n", mn->addr(), mn->endAddr(), mn->size());
	putchar ('\n');
    }
    else
	printf ("  <Empty>\n");
}
#endif


QDynamicArea::QDynamicArea (const char *da_name,
			    int init_da_size,
			    int max_da_size)
{
    memory_node *free_block;

    // FIXME: How do we handle failure here?
    xosdynamicarea_create (-1, init_da_size, (byte *)-1, os_AREA_NO_USER_DRAG, max_da_size, 0, 0,
			  da_name, &m_handle, &m_baseAddr, nullptr);

    m_size = init_da_size;

    /* Create a free block for the initial DA allocation.  */
    free_block = new memory_node (m_baseAddr, m_size);
    m_freeList.addToEnd (free_block);
}

QDynamicArea::~QDynamicArea()
{
    xosdynamicarea_delete(m_handle);
}

/* alloc
 *
 * Allocate a block of memory of the given size.
 * 
 * First the free list is searched for a block of exact size. If that fails,
 * then the first block that is big enough is used.
 */
void *
QDynamicArea::alloc (size_t size)
{
    void *ptr = nullptr;
    memory_node *mn;

#ifdef DEBUG_MEMORY
	printf ("%s (%lX)\n", __func__, size);
	dump ("> Free list before allocation");
#endif

/* Search for block of exact size.  */
    for (mn = m_freeList.first();
	 mn && size != mn->size();
	 mn = mn->next())
      /* Empty loop. */;

    if (mn)
    {
#ifdef DEBUG_MEMORY
	printf ("> Requested size same as free block (%p -> %p, %lX)\n", mn->addr(), mn->endAddr(), mn->size());
#endif
	ptr = mn->addr();
	m_freeList.remove(mn);
	delete mn;
    }
    else
    {
	/* Search for block that is big enough for the requested size.  */
	for (mn = m_freeList.first();
	     mn && size > mn->size();
	     mn = mn->next())
	  /* Empty loop.  */;

	if (mn)
	{
#ifdef DEBUG_MEMORY
	    printf ("> Requested size fits within free block (%p -> %p, %lX)\n", mn->addr(), mn->endAddr(), mn->size());
#endif
	    ptr = mn->addr();
	    mn->adjustAddress (size);
	    mn->adjustSize (-size);
	}
    }

    /* If allocation from free list failed, then extend DA.  */
    if (ptr == nullptr)
    {
	os_error *err;
	size_t da_inc;
	memory_node *last = m_freeList.last();

	if (last && (last->endAddr()) == m_baseAddr + m_size)
	{
	    /* There's a free block at the end of the DA, but it's not big enough.
	     * Extend the DA so the block becomes big enough.  */
	    da_inc = ((size - last->size()) + 0xfff) & ~0xfff;

	    if ((err = xos_change_dynamic_area (m_handle, da_inc, nullptr)) != nullptr)
		return nullptr;

	    m_size += da_inc;
#ifdef DEBUG_MEMORY
	    printf ("> No suitable free blocks; extend DA by 0x%lX to 0x%lX\n", da_inc, m_size);
	    printf ("> Use free block (%p -> %p, %lX) at end of (extended) DA\n", last->addr(), last->endAddr(), last->size());
#endif
	    /* There's a free block at the end, and now that the DA has been extended, it's
	     * big enough for this allocation.  */
	    ptr = last->addr();

	    if (last->size() + da_inc - size == 0)
	    {
		/* This allocation consumed the whole of the last free block; remove it
		 * from the list.  */
		m_freeList.remove (last);
		delete last;
	    }
	    else
	    {
		last->adjustAddress(size);
		last->adjustSize(da_inc - size);
	    }
	}
	else
	{
	    size_t old_da_size = m_size;

	    da_inc = (size + 0xfff) & ~0xfff;

	    if ((err = xos_change_dynamic_area (m_handle, da_inc, nullptr)) != nullptr)
		return nullptr;

	    m_size += da_inc;
#ifdef DEBUG_MEMORY
	    printf ("> No suitable free blocks; extend DA by 0x%lX to 0x%lX\n", da_inc, m_size);
#endif
	    /* There is not a free block at the end. Allocate the block and create a free block for
	     * the rest.  */
	    ptr = m_baseAddr + old_da_size;
	    if (da_inc - size > 0)
	    {
		mn = new memory_node (ptr + size, da_inc - size);
		m_freeList.addToEnd (mn);
#ifdef DEBUG_MEMORY
		printf ("> Allocate block at end of (extended) DA, create free block for remainder (%p -> %p, %lX)\n",
			mn->addr(), mn->endAddr(), mn->size());
#endif
	    }
#ifdef DEBUG_MEMORY
	    else
	    {
		printf ("> Allocate block at end of (extended) DA, no remainder\n");
	    }
#endif
	}
    }

#ifdef DEBUG_MEMORY
    dump ("> Free list after allocation");
    printf ("> Block (%p -> %p, 0x%lX) allocated\n", ptr, ptr + size, size);
#endif

    return ptr;
}

/* Free a block of memory.
 * 
 * The specified block need not have the same address or size as a previously allocated
 * block as long as an allocated block fully contains it.  */
void QDynamicArea::free (void *addr, size_t len)
{
    memory_node *search;

    if (addr == nullptr)
    {
#ifdef DEBUG_MEMORY
	fprintf (stderr, "%s: Attempt to free NULL address.\n", __func__);
#endif
	return;
    }

    if (len == 0)
    {
#ifdef DEBUG_MEMORY
	fprintf (stderr,"%s: Attempt to free zero sized memory block.\n", __func__);
#endif
	return ;
    }

    if (addr < m_baseAddr || addr >= m_baseAddr + m_size)
    {
#ifdef DEBUG_MEMORY
	fprintf (stderr,"%s: Attempt to free address outside dynamic area.\n", __func__);
#endif
	return;
    }

#ifdef DEBUG_MEMORY
    printf ("* %s (%p -> %p, %lX)\n", __func__, addr, (char *)addr + len, len);
    dump ("* Free list before merge");
#endif

#ifdef SANITY_CHECK
    for (search = m_freeList.first();
	 search;
	 search = search->next())
    {
	if (((char *)addr >= search->addr() &&
	     (char *)addr + len < search->endAddr()) ||
	    ((char *)addr < search->addr() &&
	     (char *)addr + len > search->addr() && 
	     (char *)addr + len < search->endAddr()) ||
	    ((char *)addr >= search->addr() &&
	     (char *)addr < search->endAddr() &&
	     (char *)addr + len > search->endAddr()))
	{
	    break;
	}
    }

    if (search != NULL)
    {
	printf (">>> %s: Attempt to free block (%p -> %p, %lX) outside bounds of any previously allocated block.\n", __func__, addr, addr + len, len);
	return;
    }
#endif

    /* Find the correct position in the list.  */
    for (search = m_freeList.first();
	 search && addr >= search->addr();
	 search = search->next())
	/* Empty loop.  */;

    if (search == nullptr)
    {
	/* Either the list is empty in which case we create the first node, or the end of the
	 * list was reached which means that this block has a higher address than all others.
	 * Either way add it to the end of the list.  */
	memory_node *mn;

	mn = new memory_node (addr, len);
	m_freeList.addToEnd (mn);
#ifdef DEBUG_MEMORY
	printf ("* Could not merge; Add free node to end (%p -> %p, %lX)\n",
		mn->addr(), mn->endAddr(), mn->size());
#endif
    }
    else
    {
	memory_node *prev = search->prev();
	int merge_count = 0;

	if ((char *)addr + len == search->addr())
	{
	    /* The block to be free'd is adjacent to a free list block after it, merge it into
	     * that.  */
#ifdef DEBUG_MEMORY
	    printf ("* Block (%p -> %p, %lX) merged into next block (%p -> %p, %lX)\n",
		    addr, addr + len, len, search->addr(), search->endAddr(), search->size());
#endif
	    search->setAddress(addr);
	    search->adjustSize(len);
	    merge_count++;
	}
	if (prev && prev->endAddr() == addr)
	{
	    /* The block to be free'd is adjacent to a free list block before it, merge it into
	     * that.  */
#ifdef DEBUG_MEMORY
	    printf ("* Block (%p -> %p, %lX) merged into previous block (%p -> %p, %lX)\n",
		    addr, addr + len, len, prev->addr(), prev->endAddr(), prev->size());
#endif
	    prev->adjustSize(len);
	    merge_count++;
	}
	if (merge_count == 0)
	{
	    /* Block was not merged into either previous or next block, so insert a new
	     * free list node at the correct place.  */
	    memory_node *mn;

	    mn = new memory_node (addr, len);
	    m_freeList.addBefore (search, mn);
#ifdef DEBUG_MEMORY
	    printf ("* Could not merge; Insert free node (%p -> %p, %lX)\n",
		    mn->addr(), mn->endAddr(), mn->size());
#endif
	}
	else if (merge_count == 2)
	{
	    /* Block was merged into previous and next block. Expand the previous block to
	     * cover the whole area. The next block is no longer required and can be removed.  */
#ifdef DEBUG_MEMORY
	    printf ("* Block merged into previous and next block. Removing surplus node.\n");
#endif
	    prev->adjustSize(search->size() - len);
	    m_freeList.remove (search);
	    delete search;
	}
    }

#ifdef DEBUG_MEMORY
    dump ("* Free list after merge");
#endif
}
