#ifndef __ChunkFile_h__
#define __ChunkFile_h__

#include "BString.h"
#include "Chunk.h"
#include "Buffer.h"

class ChunkFile
{
 public:
 	ChunkFile();
 	ChunkFile(const BString &a_fileName);
 	~ChunkFile();

 	void load(const BString &a_fileName);
 	void load(const Buffer &a_data);
 	void load(Buffer *a_data);
 	void save(const BString &a_fileName);

 	Chunk *getChunk(const BString &a_chunkName);
	Chunk *getChunk(int a_index);
	BString getChunkName(int a_index);
	int getNumChunks();

 protected:
 	int isChunkFile();

 	Buffer m_data;
 	BString m_fileName;

 	Chunk m_scrapChunk;
};

#endif
