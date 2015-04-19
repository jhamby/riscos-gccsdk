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

/****************************************************************************
**
** Written by Lee Noar
**
** This file defines a class that wraps a dynamic area.
**
****************************************************************************/

#ifndef QDYNAMICAREA_H
#define QDYNAMICAREA_H

#include <QLinkedList>
#include "oslib/os.h"

//#define DEBUG_MEMORY

QT_BEGIN_NAMESPACE

#define os_DYNAMIC_AREA_INVALID os_DYNAMIC_AREA_APPLICATION_SPACE

class Q_CORE_EXPORT QDynamicArea
{
public:
    QDynamicArea(const char *da_name,
		  int max_da_size,
		  int init_da_size = 4 * 1024);		// 4KB
    ~QDynamicArea();

    bool create();
    void *alloc(size_t size);
    void free(void *ptr, size_t size);

    bool exists() const {
        return m_handle != os_DYNAMIC_AREA_INVALID;
    }

    // Return true if the given address is in the dynamic area's current
    // address range (not whether it is allocated).
    bool contains(void *addr) const {
	return addr >= m_baseAddr && addr < m_baseAddr + m_size;
    }

#ifdef DEBUG_MEMORY
    void dump(const char *title);
#endif

private:
    void Remove();

    struct memory_node
    {
	memory_node(void *addr, size_t size)
	{
	  m_addr = addr;
	  m_size = size;
	}

	~memory_node() { }

	void adjustAddress(int by) { m_addr = static_cast<char *>(m_addr) + by; }
	void adjustSize(int by) { m_size += by; }
	void setAddress(void *addr) { m_addr = addr; }

	void *addr() const { return m_addr; }
	void *endAddr() const { return static_cast<char *>(m_addr) + m_size; }
	size_t size() const { return m_size; }

    private:
	void *m_addr;
	size_t m_size;
    };

    os_dynamic_area_no m_handle;
    byte *m_baseAddr;
    size_t m_size;
    size_t m_maxSize;
    QLinkedList<memory_node> m_freeList;

    char m_name[32];
};

QT_END_NAMESPACE

#endif
