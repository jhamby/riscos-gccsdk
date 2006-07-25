#ifndef __Chunk_h__
#define __Chunk_h__

#include <cstdio>
#include "BString.h"
#include "Buffer.h"

class Library;

class Chunk
{
 public:
 	Chunk();
 	Chunk(const BString &a_chunkName, Library *a_owner=0);
 	virtual ~Chunk();

 	void load(const BString &a_fileName);
 	void load(Buffer *a_buffer, int a_start, int a_len);

 	virtual void save(const BString &a_fileName, int a_append=0);

 	void writeHeader(Buffer *a_buffer);
 	void writeData(Buffer *a_buffer);

	int getChunkSize();

 	virtual void buildData() {}
 	void buildHeader(int &a_offset);

 	Buffer *getData() { return &m_data; }
 	Buffer *getHead() { return &m_head; }

 	BString getName() { return m_chunkName; }

 	virtual void print();

	int createPath(const BString &a_path);

 protected:
 	virtual int calculateChunkSize() { return 0; }

	static int exists(const BString &a_file);
 	BString format(const BString &a_string, int a_len, int a_left=1) const;

 	Buffer m_data;
 	Buffer m_head;
 	int m_chunkSize;
 	BString m_chunkName;
 	Library *m_owner;
};

#endif
