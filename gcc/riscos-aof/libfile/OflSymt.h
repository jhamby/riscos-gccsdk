#ifndef __OflSymt_h__
#define __OflSymt_h__

#include "Chunk.h"
#include "BList.h"
#include "BString.h"

class SymbolInfo;
class Buffer;
class LibDir;

class OflSymt : public Chunk
{
 public:
 	OflSymt(Library *a_owner);
 	~OflSymt();

 	void set(Buffer *a_data);
 	void set(const List_of_p<Chunk> &a_memberList, LibDir *a_libDir);

 	virtual void buildData();

 	void print(LibDir *a_dir, int a_long=0);

 protected:
 	virtual int calculateChunkSize();

 	List_of_p<SymbolInfo> m_symbols;
};

class SymbolInfo
{
 public:
 	SymbolInfo(const BString &a_symbolName, int a_chunkIndex)
 	{
 	 	m_symbolName = a_symbolName;
 	 	m_chunkIndex = a_chunkIndex;
 	}

	int m_chunkIndex;
	BString m_symbolName;
};

#endif
