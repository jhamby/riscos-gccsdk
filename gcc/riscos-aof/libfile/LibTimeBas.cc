#include <stdio.h>
#include "LibTimeBas.h"
#include "BError.h"

#ifndef CROSS_COMPILE
extern "C" void OS_File(int *);
#endif

LibTimeBas::LibTimeBas(const BString &a_chunkName, Library *a_owner)
 : Chunk(a_chunkName, a_owner)
{
}

LibTimeBas::~LibTimeBas()
{
}

void LibTimeBas::set(Buffer *a_data)
{
 int start = 0;
 m_now.get(a_data, start);
}

void LibTimeBas::set()
{
 m_now.getCurrent();
}

void LibTimeBas::buildData()
{
 m_now.put(m_data);
}

int LibTimeBas::calculateChunkSize()
{
 return 8;
}

void LibTimeBas::print()
{
 cout << "Chunk LibTime:" << endl;
 m_now.print();
}
