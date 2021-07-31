/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Written by Lee Noar
 */

#include "config.h"
#include "OSAllocator.h"

#if OS(RISCOS)

#include "PageAllocation.h"
#include <errno.h>
#include <wtf/Assertions.h>

#include <kernel.h>
#include <swis.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "oslib/os.h"
#include <QDynamicArea>


#if defined (DEBUG_MEMORY)
#define SANITY_CHECK 1
#endif

namespace WTF {

namespace riscos {

class dynamicAreaTable
{
public:
    dynamicAreaTable();

    QDynamicArea *findDA(void *addr);
    QDynamicArea &operator[](OSAllocator::Usage usage);

private:
    QDynamicArea mFastMalloc;
    QDynamicArea mGCHeap;
    QDynamicArea mJITCode;
    QDynamicArea mVMStack;
};


dynamicAreaTable::dynamicAreaTable()
	: mFastMalloc("Webkit Fast Malloc", 200 * 1024),	// 200KB
	  mGCHeap("Webkit GC Heap", 100 * 1024 * 1024),		// 100MB
	  mJITCode("Webkit JIT Code", 10 * 1024 * 1024),	// 10MB
	  mVMStack("Webkit VM Stack", 8 * 1024 * 1024)		// 8MB
{
}

QDynamicArea *
dynamicAreaTable::findDA(void *addr)
{
    if (mFastMalloc.exists() && mFastMalloc.contains(addr))
	return &mFastMalloc;
    else if (mGCHeap.exists() && mGCHeap.contains(addr))
	return &mGCHeap;
    else if (mJITCode.exists() && mJITCode.contains(addr))
	return &mJITCode;
    else if (mVMStack.exists() && mVMStack.contains(addr))
	return &mVMStack;

    return nullptr;
}

QDynamicArea &
dynamicAreaTable::operator[](OSAllocator::Usage usage)
{
    switch (usage)
    {
      case OSAllocator::JSGCHeapPages:
	  return mGCHeap;
      case OSAllocator::JSVMStackPages:
	  return mVMStack;
      case OSAllocator::JSJITCodePages:
	  return mJITCode;
      case OSAllocator::UnknownUsage:
      case OSAllocator::FastMallocPages:
      default:
	  return mFastMalloc;
    }

    // Should never get here.
    CRASH();
}

static dynamicAreaTable kDynamicAreaTable;

} // namespace riscos

void* OSAllocator::reserveUncommitted(size_t bytes, Usage usage, bool writable, bool executable, bool includesGuardPages)
{
    return reserveAndCommit (bytes, usage, writable, executable, includesGuardPages);
}

void* OSAllocator::reserveAndCommit(size_t bytes, Usage usage, bool writable, bool executable, bool includesGuardPages)
{
    UNUSED_PARAM(writable);
    UNUSED_PARAM(executable);
    UNUSED_PARAM(includesGuardPages);

    QDynamicArea &da = riscos::kDynamicAreaTable[usage];

    // Create dynamic area on first use.
    if (!da.exists()) {
	if (!da.create())
	    CRASH();
    }

    void *result = da.alloc(bytes);

#if 1
    memset (result, 0, bytes);
#endif

    return result;
}

void OSAllocator::commit(void* address, size_t bytes, bool writable, bool executable)
{
    UNUSED_PARAM(address);
    UNUSED_PARAM(bytes);
    UNUSED_PARAM(writable);
    UNUSED_PARAM(executable);
}

void OSAllocator::decommit(void* address, size_t bytes)
{
    UNUSED_PARAM(address);
    UNUSED_PARAM(bytes);
}

void OSAllocator::releaseDecommitted(void* address, size_t bytes)
{
    QDynamicArea *da = riscos::kDynamicAreaTable.findDA(address);

    if (da) {
	da->free(address, bytes);
	return;
    }

    CRASH();
    // Or shall we just exit silently?
}

} // namespace WTF

#endif // OS(RISCOS)
