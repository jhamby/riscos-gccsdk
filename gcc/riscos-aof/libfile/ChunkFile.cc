#include <cstdlib>
#include <cassert>
#include <iostream>
#include "ChunkFile.h"
#include "BError.h"

ChunkFile::ChunkFile()
{
}

ChunkFile::ChunkFile(const BString &a_fileName)
{
 m_fileName = a_fileName;
}

ChunkFile::~ChunkFile()
{
}

void ChunkFile::load(const BString &a_fileName)
{
 m_fileName = a_fileName;
 m_data.load(m_fileName);
 if(!isChunkFile())
 	THROW_SPEC_ERR(BError::NoChunkFile);
}

void ChunkFile::load(const Buffer &a_data)
{
 m_data = a_data;
}

void ChunkFile::load(Buffer *a_data)
{
 m_data.set(a_data);
}

void ChunkFile::save(const BString &a_fileName)
{
 m_data.save(a_fileName);
}

int ChunkFile::getNumChunks()
{
 int start = 8;
 int chunks = m_data.getInt(start);
 return chunks;
}

BString ChunkFile::getChunkName(int a_index)
{
 if(a_index >= getNumChunks())
 	THROW_SPEC_ERR(BError::ChunkIndex);

 int start = 12 + a_index*16;
 BString chunkName;

 chunkName = m_data.getStringByLen(start, 8);

 return chunkName;
}

Chunk *ChunkFile::getChunk(int a_index)
{
 if(a_index >= getNumChunks())
 	return 0;

 int start = 12 + a_index*16;
 int cStart, cLen;
 BString chunkName;

 chunkName = m_data.getStringByLen(start, 8);
 cStart = m_data.getInt(start);
 cLen = m_data.getInt(start);

 m_scrapChunk.load(&m_data, cStart, cLen);
 return &m_scrapChunk;
}

Chunk *ChunkFile::getChunk(const BString &a_chunkName)
{
 int chunks = getNumChunks();
 int i;
 BString chunkName;
 Chunk *chunk;

 for(i=0; i<chunks; i++)
 {
 	if(getChunkName(i) == a_chunkName)
 	{
 	 	chunk = getChunk(i);
 	 	return chunk;
 	}
 }
 return NULL;
}

int ChunkFile::isChunkFile()
{
 int start=0;
 int id=m_data.getInt(start);
 if(id==0xC3CBC6C5)
 	return 1;
 return 0;
}

