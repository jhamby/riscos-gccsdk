#include <iostream.h>
#include "LibData.h"
#include "TimeStamp.h"
#include "BError.h"

#ifndef UNIX
extern "C" OS_File(int *);
#endif

LibData::LibData(Library *a_owner) : Chunk("LIB_DATA", a_owner)
{
}

LibData::~LibData()
{
}

void LibData::save(const BString &a_fileName, TimeStamp a_time)
{
 m_data.save(a_fileName, 0);

#ifndef UNIX
 int regs[6] = { 2, 0, 0, 0, 0, 0 };
 regs[1] = (int) a_fileName();
 regs[2] = (a_time.m_load | 0xFFFFFD00);
 OS_File(regs);
 regs[0] = 3;
 regs[3] = a_time.m_exe;
 OS_File(regs);
#endif
}

void LibData::set(Buffer *a_data)
{
 m_data.set(a_data);
}

void LibData::set(const BString &a_fileName)
{
 m_data.load(a_fileName);
}

void LibData::buildData()
{
}

int LibData::calculateChunkSize()
{
 return m_data.length();
}

void LibData::print()
{
 cout << "Chunk LibData:" << endl;
 cout << "Data Length " << m_data.length() << endl;
}
