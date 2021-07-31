/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
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
#include <errno.h>
#include <assert.h>
#include "oslib/os.h"
#include "qdynamicarea.h"

#if defined (DEBUG_MEMORY)
#define SANITY_CHECK 1
#endif

#define VERBOSE_MEMORY 1

QT_BEGIN_NAMESPACE

static QDynamicArea qt_mmap_da("Qt mmap", 20 * 1024 * 1024); // 20MB

#ifdef DEBUG_MEMORY
void 
QDynamicArea::dump (const char *title)
{
    QLinkedList<memory_node>::iterator iterator;

    if (title)
	printf ("%s: (DA '%s': %p -> %p)\n", title, m_name, m_baseAddr, m_baseAddr + m_size);
    else
	printf ("Free list: (DA '%s': %p -> %p)\n", m_name, m_baseAddr, m_baseAddr + m_size);
    if (m_freeList.count())
    {
	for (iterator = m_freeList.begin();
	     iterator != m_freeList.end();
	     iterator++)
	    printf ("    [node:this:%p]   %p -> %p, %lX\n",
		    &*iterator,iterator->addr(), iterator->endAddr(), iterator->size());
	putchar ('\n');
    }
    else
	printf ("  <Empty>\n");
}
#endif


QDynamicArea::QDynamicArea (const char *da_name,
			    int max_da_size,
			    int init_da_size) :
	m_handle(os_DYNAMIC_AREA_INVALID),
	m_baseAddr(nullptr),
	m_size(init_da_size),
	m_maxSize(max_da_size),
	m_pageSize(getpagesize())
{
    strncpy (m_name, da_name, sizeof (m_name));
    m_name[sizeof (m_name) - 1] = '\0';

    // Keep the initial size to a minimum of 4KB and ensure all sizes are
    // rounded up to a multiple of the page size.
    m_size = (m_size < m_pageSize) ?
	      m_pageSize :
	      (m_size + (m_pageSize - 1)) & ~(m_pageSize - 1);
    m_maxSize = (m_maxSize + (m_pageSize - 1)) & ~(m_pageSize - 1);
    m_pageShift = __builtin_ctz(m_pageSize);
}

QDynamicArea::~QDynamicArea()
{
    destroy();
}

void QDynamicArea::destroy()
{
    if (m_handle != os_DYNAMIC_AREA_INVALID)
    {
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	_kernel_oserror *err;
        err = _swix(OS_DynamicArea, _INR(0,1), 1/*Remove*/, m_handle);
#else
        os_error *err;
        err = xosdynamicarea_delete(m_handle);
#endif
#ifdef VERBOSE_MEMORY
	if (err) {
	    printf ("[%s:%d:%s] - RISC OS Error: %s\n",
		    __func__, __LINE__, __FILE__, err->errmess);
	}
#endif
	m_handle = os_DYNAMIC_AREA_INVALID;
    }
}

bool
QDynamicArea::create ()
{    
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    _kernel_oserror *err;
    err = _swix(OS_DynamicArea, _INR(0,8)|_OUT(1)|_OUT(3),
		0/*Create*/, -1, m_size, -1,
		os_AREA_NO_USER_DRAG, m_maxSize, 0, 0,
		m_name,
		&m_handle, &m_baseAddr);
#else
    os_error *err;
    err = xosdynamicarea_create (-1, m_size, (byte *)-1, os_AREA_NO_USER_DRAG, m_maxSize, 0, 0,
				 m_name, &m_handle, &m_baseAddr, nullptr);
#endif
    if (err)
    {
#ifdef VERBOSE_MEMORY
	printf ("[%s:%d:%s] - RISC OS Error: %s\n",
		__func__, __LINE__, __FILE__, err->errmess);
#endif
	return false;
    }

    // Create a free block for the initial DA allocation.
    memory_node *free_block = new memory_node (m_baseAddr, m_size);
    m_freeList.append (*free_block);

#ifdef DEBUG_MEMORY
    printf ("[%s] - Created dynamic area '%s' with handle %d and base address %p\n",
	    __PRETTY_FUNCTION__, m_name, m_handle, m_baseAddr);
#endif

    return true;
}

// alloc
//
// Allocate a block of memory of the given size.
//
// First the free list is searched for a block of exact size. If that fails,
// then the first block that is big enough is used.
//
void *
QDynamicArea::alloc (size_t size)
{
    QLinkedList<memory_node>::iterator iterator;
    void *ptr = nullptr;

#ifdef DEBUG_MEMORY
    printf ("* %s [%s] (%lX)\n", __PRETTY_FUNCTION__, m_name, size);
    dump ("  Free list before allocation");
#endif

    // Search for block of exact size.
    for (iterator = m_freeList.begin();
	 iterator != m_freeList.end() && size != iterator->size();
	 iterator++)
      /* Empty loop. */;

    if (iterator != m_freeList.end())
    {
#ifdef DEBUG_MEMORY
	printf ("  Requested size same as free block (%p -> %p, %lX)\n",
		iterator->addr(), iterator->endAddr(), iterator->size());
#endif
	ptr = iterator->addr();
	m_freeList.erase(iterator);
	// NOTE: QLinkedList::erase calls delete for this node.
    }
    else
    {
	// Search for block that is big enough for the requested size.
	for (iterator = m_freeList.begin();
	     iterator != m_freeList.end() && size > iterator->size();
	     iterator++)
	  /* Empty loop. */;

	if (iterator != m_freeList.end())
	{
#ifdef DEBUG_MEMORY
	    printf ("  Requested size fits within free block (%p -> %p, %lX)\n",
		    iterator->addr(), iterator->endAddr(), iterator->size());
#endif
	    ptr = iterator->addr();
	    iterator->adjustAddress (size);
	    iterator->adjustSize (-size);
	}
    }

    // If allocation from free list failed, then extend DA.
    if (ptr == nullptr)
    {
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	_kernel_oserror *err;
#else
	os_error *err;
#endif
	size_t da_inc;

	if (!m_freeList.isEmpty() && m_freeList.last().endAddr() == m_baseAddr + m_size)
	{
	    memory_node &last = m_freeList.last();

	    // There's a free block at the end of the DA, but it's not big enough.
	    // Extend the DA so the block becomes big enough.
	    da_inc = ((size - last.size()) + (m_pageSize - 1)) & ~(m_pageSize - 1);

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	    if ((err = _swix(OS_ChangeDynamicArea, _INR(0,1), m_handle, da_inc)) != nullptr)
#else
	    if ((err = xos_change_dynamic_area (m_handle, da_inc, nullptr)) != nullptr)
#endif
	    {
#ifdef VERBOSE_MEMORY
	        printf("[%s:%d:%s] - RISC OS Error: %s\n",
		       __func__, __LINE__, __FILE__, err->errmess);
		printf("  handle (%d), size (%ld), da_inc (%ld)\n", m_handle, size, da_inc);
#endif
		return nullptr;
	    }

	    m_size += da_inc;
#ifdef DEBUG_MEMORY
	    printf ("  No suitable free blocks; extend DA by 0x%lX to 0x%lX\n", da_inc, m_size);
	    printf ("  Use free block (%p -> %p, %lX) at end of (extended) DA\n", last.addr(), last.endAddr(), last.size());
#endif
	    // There's a free block at the end, and now that the DA has been extended, it's
	    // big enough for this allocation.
	    ptr = last.addr();

	    if (last.size() + da_inc - size == 0)
	    {
		// This allocation consumed the whole of the last free block; remove it
		// from the list.
		m_freeList.removeLast();
		// NOTE: QLinkedList::removeLast also calls delete for this node.
	    }
	    else
	    {
		last.adjustAddress(size);
		last.adjustSize(da_inc - size);
	    }
	}
	else
	{
	    size_t old_da_size = m_size;

	    da_inc = (size + (m_pageSize - 1)) & ~(m_pageSize - 1);

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	    if ((err = _swix(OS_ChangeDynamicArea, _INR(0,1), m_handle, da_inc)) != nullptr)
#else
	    if ((err = xos_change_dynamic_area (m_handle, da_inc, nullptr)) != nullptr)
#endif
	    {
#ifdef VERBOSE_MEMORY
	        printf("[%s:%d:%s] - RISC OS Error: %s\n",
		       __func__, __LINE__, __FILE__, err->errmess);
		printf("  handle (%d), size (%ld), da_inc (%ld), m_size (%ld)\n",
		       m_handle, size, da_inc, m_size);
#endif
		return nullptr;
	    }

	    m_size += da_inc;
#ifdef DEBUG_MEMORY
	    printf ("  No suitable free blocks; extend DA by 0x%lX to 0x%lX\n", da_inc, m_size);
#endif
	    // There is not a free block at the end. Allocate the block and create a free block for
	    // the rest.
	    ptr = m_baseAddr + old_da_size;
	    if (da_inc - size > 0)
	    {
		memory_node *mn = new memory_node (static_cast<char *>(ptr) + size, da_inc - size);
		m_freeList.append (*mn);
#ifdef DEBUG_MEMORY
		printf ("  Allocate block at end of (extended) DA, create free block for remainder (%p -> %p, %lX)\n",
			mn->addr(), mn->endAddr(), mn->size());
#endif
	    }
#ifdef DEBUG_MEMORY
	    else
	    {
		printf ("  Allocate block at end of (extended) DA, no remainder\n");
	    }
#endif
	}
    }

#ifdef DEBUG_MEMORY
    dump ("  Free list after allocation");
    printf ("  Block (%p -> %p, 0x%lX) allocated\n",
	    ptr, static_cast<char *>(ptr) + size, size);
#endif

    return ptr;
}

// Free a block of memory.
// 
// The specified block need not have the same address or size as a previously allocated
// block as long as an allocated block fully contains it.
void QDynamicArea::free (void *addr, size_t len)
{
    QLinkedList<memory_node>::iterator iterator;

    if (addr == nullptr)
    {
#ifdef VERBOSE_MEMORY
	fprintf (stderr, "%s: Attempt to free NULL address.\n", __func__);
#endif
	return;
    }

    if (len == 0)
    {
#ifdef VERBOSE_MEMORY
	fprintf (stderr,"%s: Attempt to free zero sized memory block.\n", __func__);
#endif
	return;
    }

    if (addr < m_baseAddr || addr >= m_baseAddr + m_size)
    {
#ifdef VERBOSE_MEMORY
	fprintf (stderr,"%s: Attempt to free address outside dynamic area.\n", __func__);
#endif
	return;
    }

#ifdef DEBUG_MEMORY
    printf ("* %s [%s] (block %p -> %p, %lX)\n", __PRETTY_FUNCTION__, m_name, addr, (char *)addr + len, len);
    dump ("  Free list before merge");
#endif

#ifdef SANITY_CHECK
    for (iterator = m_freeList.begin();
	 iterator != m_freeList.end();
	 ++iterator)
    {
	// Check to see if the block being free'd is within or overlaps an already
	// free block.
	if (((char *)addr >= iterator->addr() &&
	     (char *)addr + len < iterator->endAddr()) ||
	    ((char *)addr < iterator->addr() &&
	     (char *)addr + len > iterator->addr() && 
	     (char *)addr + len < iterator->endAddr()) ||
	    ((char *)addr >= iterator->addr() &&
	     (char *)addr < iterator->endAddr() &&
	     (char *)addr + len > iterator->endAddr()))
	{
	    break;
	}
    }

    if (iterator != m_freeList.end())
    {
	printf (">>> %s: Attempt to free block (%p -> %p, %lX) outside bounds of any previously allocated block.\n",
		__PRETTY_FUNCTION__, addr, static_cast<char *>(addr) + len, len);
	return;
    }
#endif

    // Find the correct position in the list.
    for (iterator = m_freeList.begin();
	 iterator != m_freeList.end() && addr >= iterator->addr();
	 ++iterator)
	/* Empty loop. */;

    if (iterator == m_freeList.end())
    {
	// Either the list is empty in which case we create the first node, or the end of the
	// list was reached which means that this block has a higher address than all others.
	// Either way add it to the end of the list.
	memory_node *mn;

	mn = new memory_node (addr, len);
	m_freeList.append (*mn);
#ifdef DEBUG_MEMORY
	printf ("  Could not merge; Add free node to end (%p -> %p, %lX)\n",
		mn->addr(), mn->endAddr(), mn->size());
#endif
    }
    else
    {
	QLinkedList<memory_node>::iterator prev_iter(iterator-1);
	int merge_count = 0;

	if (static_cast<char *>(addr) + len == iterator->addr())
	{
	    // The block to be free'd is adjacent to a free list block after it, merge it into
	    // that.
#ifdef DEBUG_MEMORY
	    printf ("  Block (%p -> %p, %lX) merged into next block (%p -> %p, %lX)\n",
		    addr, static_cast<char *>(addr) + len, len, iterator->addr(), iterator->endAddr(), iterator->size());
#endif
	    iterator->setAddress(addr);
	    iterator->adjustSize(len);
	    merge_count++;
#ifdef DEBUG_MEMORY
	    printf ("      = (%p -> %p, %lX)\n", iterator->addr(), iterator->endAddr(), iterator->size());
#endif
	}
	if (iterator != m_freeList.begin() && prev_iter->endAddr() == addr)
	{
	    // The block to be free'd is adjacent to a free list block before it, merge it into
	    // that.
#ifdef DEBUG_MEMORY
	    printf ("  Block (%p -> %p, %lX) merged into previous block (%p -> %p, %lX)\n",
		    addr, static_cast<char *>(addr) + len, len, prev_iter->addr(), prev_iter->endAddr(), prev_iter->size());
#endif
	    prev_iter->adjustSize(len);
	    merge_count++;
#ifdef DEBUG_MEMORY
	    printf ("      = (%p -> %p, %lX)\n", prev_iter->addr(), prev_iter->endAddr(), prev_iter->size());
#endif
	}
	if (merge_count == 0)
	{
	    // Block was not merged into either previous or next block, so insert a new
	    // free list node at the correct place.
	    memory_node *mn;

	    mn = new memory_node (addr, len);
	    m_freeList.insert (iterator, *mn);
#ifdef DEBUG_MEMORY
	    printf ("  Could not merge; Insert free node (%p -> %p, %lX)\n",
		    mn->addr(), mn->endAddr(), mn->size());
#endif
	}
	else if (merge_count == 2)
	{
	    // Block was merged into previous and next block. Expand the previous block to
	    // cover the whole area. The next block is no longer required and can be removed.
#ifdef DEBUG_MEMORY
	    printf ("  Block merged into previous and next block. Removing surplus node.\n");
#endif
	    prev_iter->adjustSize(iterator->size() - len);
	    m_freeList.erase (iterator);
	    // NOTE: QLinkedList::erase calls delete for this node.
	}
    }

    // Try to reclaim any unused pages of memory at the end of the dynamic area.
    if (!m_freeList.isEmpty() && m_freeList.last().endAddr() == m_baseAddr + m_size)
    {
	// There's a free block at the end of the dynamic area.
        unsigned char *start_addr = static_cast<unsigned char *>(m_freeList.last().addr());
	unsigned char *end_addr = static_cast<unsigned char *>(m_freeList.last().endAddr());

	if (((end_addr - start_addr) >> m_pageShift) != 0)
	{
	    // There's at least one whole page to reclaim.
	    size_t reduce_by;

	    if (start_addr == m_baseAddr)
	    {
		// If this is also at the start of the dynamic area, then freeing it
		// would leave a 0KB dynamic area. Adjust things so that we leave
		// 4KB.

		// Round up to whole pages.
		reduce_by = ((end_addr - start_addr) + (m_pageSize - 1)) & ~(m_pageSize - 1);

		// Keep one page.
		reduce_by -= m_pageSize;
	    }
	    else
	    {
		reduce_by = (end_addr - start_addr) & ~(m_pageSize - 1);
	    }

	    if (reduce_by)
	    {
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
		_kernel_oserror *err;
		err = _swix(OS_ChangeDynamicArea, _INR(0,1), m_handle, -reduce_by);
#else
		os_error *err;
		err = xos_change_dynamic_area (m_handle, -reduce_by, nullptr);
#endif
		if (!err)
		{
		    m_size -= reduce_by;
		    if (((end_addr - start_addr) & (m_pageSize - 1)) == 0 &&
			  start_addr != m_freeList.first().addr())
		    {
			// We have a whole number of pages with no remainder; just
			// remove the last free block.
			// NOTE: QLinkedList::removeLast also calls delete for this node.
			m_freeList.removeLast();
		    }
		    else
		    {
			// There's part of a page left over; adjust the last free block
			// to describe it.
			m_freeList.last().adjustSize(-reduce_by);
		    }
#ifdef DEBUG_MEMORY
		    printf ("* [%s] Shrunk DA by %ld pages\n", m_name, reduce_by / m_pageSize);
#endif
		}
#ifdef VERBOSE_MEMORY
		else
		{
		    printf("[%s:%d:%s] - RISC OS Error: %s\n",
			  __func__, __LINE__, __FILE__, err->errmess);
		}
#endif
	    }
	}
    }

#ifdef DEBUG_MEMORY
    dump ("  Free list after merge");
#endif
}

void *qt_mmap(size_t size, int fd, off_t offset)
{
    assert(offset == 0);

    if (!qt_mmap_da.exists())
	qt_mmap_da.create();

    void *address = qt_mmap_da.alloc(size);
    if (address == NULL)
    {
	errno = ENOMEM;
	return nullptr;
    }

    if (fd >= 0)
    {
	if (read(fd, address, size) < 0)
	{
	    errno = EBADF;
	    qt_mmap_da.free(address, size);
	    return nullptr;
	}
    }

    return address;
}

void qt_munmap(void *address, size_t len)
{
  qt_mmap_da.free(address, len);
}

QT_END_NAMESPACE
