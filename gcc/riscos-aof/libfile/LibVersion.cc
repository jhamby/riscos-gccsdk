#include <iostream>
#include "LibVersion.h"

LibVersion::LibVersion(Library *a_owner) : Chunk("LIB_VRSN", a_owner)
{
 m_version = 1;
}

LibVersion::~LibVersion()
{
}

void LibVersion::set(int a_version)
{
 m_version = a_version;
}

void LibVersion::set(Buffer *a_data)
{
 int start = 0;
 m_version = a_data->getInt(start);
}

void LibVersion::buildData()
{
 m_data.putInt(m_version);
}

int LibVersion::calculateChunkSize()
{
 return 4;
}

void LibVersion::print()
{
 cout << m_version;
}
