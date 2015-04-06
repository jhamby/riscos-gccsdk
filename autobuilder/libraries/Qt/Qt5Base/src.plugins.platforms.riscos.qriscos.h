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

/****************************************************************************
**
** Written by Lee Noar
**
** This file defines a class that wraps a dynamic area.
**
****************************************************************************/

#ifndef QRISCOS_H
#define QRISCOS_H

#include "oslib/os.h"

//#define DEBUG_MEMORY 1
//#define SANITY_CHECK 1

class QDynamicArea
{
public:
    QDynamicArea (const char *da_name,
		  int init_da_size = 4 * 1024,		// 4KB
		  int max_da_size = 200 * 1024 * 1024); // 200MB
    ~QDynamicArea ();

    void *alloc (size_t size);
    void free (void *ptr, size_t size);
    
#ifdef DEBUG_MEMORY
    void dump (const char *title);
#endif

private:
    void Remove ();

    struct memory_node
    {
	memory_node (void *addr, size_t size)
	{
	  m_prev = m_next = nullptr;
	  m_addr = addr;
	  m_size = size;
	}

	void adjustAddress(int by) { m_addr = static_cast<char *>(m_addr) + by; }
	void adjustSize(int by) { m_size += by; }
	void setAddress(void *addr) { m_addr = addr; }

	memory_node *prev() const { return m_prev; }
	memory_node *next() const { return m_next; }
	void *addr() const { return m_addr; }
	void *endAddr() const { return static_cast<char *>(m_addr) + m_size; }
	size_t size() const { return m_size; }

//    private:
	struct memory_node *m_prev;
	struct memory_node *m_next;
	void *m_addr;
	size_t m_size;
    };

    struct memory_node_list
    {
	memory_node_list ()
	{
	  m_first = m_last = nullptr;
	  m_count = 0;
	}

	void addToEnd (memory_node *link)
	{
	    memory_node *oldlast;

	    oldlast = m_last;
	    link->m_next = nullptr;
	    link->m_prev = oldlast;

	    if (oldlast == nullptr)
		m_first = link;
	    else
		oldlast->m_next = link;

	    m_last = link;
	    m_count++;
	}

	void addToFront (memory_node *link)
	{
	    memory_node *oldfirst;

	    oldfirst = m_first;
	    link->m_next = oldfirst;
	    link->m_prev = nullptr;

	    if (oldfirst == NULL)
		m_last = link;
	    else
		oldfirst->m_prev = link;

	    m_first = link;
	    m_count++;
	}

	void addBefore (memory_node *at, memory_node *link)
	{
	    memory_node *oldprev;

	    if (at == nullptr)
	    {
		addToEnd (link);
		return;
	    }

	    oldprev = at->m_prev;
	    if (oldprev == nullptr)
		addToFront (link);
	    else
	    {
		oldprev->m_next = link;
		link->m_prev = oldprev;
		at->m_prev = link;
		link->m_next = at;
		m_count++;
	    }
	}

	void remove (memory_node *link)
	{
	    memory_node *prev, *next;

	    next = link->m_next;
	    prev = link->m_prev;

	    if (next == nullptr)
		m_last = prev;
	    else
		next->m_prev = prev;

	    if (prev == nullptr)
		m_first = nullptr;
	    else
		prev->m_next = next;

	    link->m_next = link->m_prev = NULL;
	    m_count--;
	}

	memory_node *first() const { return m_first; }
	memory_node *last() const { return m_last; }
	int count() const { return m_count; }

    private:
	memory_node *m_first;
	memory_node *m_last;
	int m_count;
    };

    os_dynamic_area_no m_handle;
    byte *m_baseAddr;
    size_t m_size;
    memory_node_list m_freeList;
};

#endif
