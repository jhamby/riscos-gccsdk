#ifndef __LibVersion_h__
#define __LibVersion_h__

#include "Chunk.h"

class LibVersion : public Chunk
{
 public:
 	LibVersion(Library *a_owner);
 	~LibVersion();

 	void set(int a_version);
 	void set(Buffer *a_data);

 	virtual void buildData();
 	int getVersion() { return m_version; }

 	virtual void print();

 protected:
 	virtual int calculateChunkSize();

 	int m_version;
};

#endif
