#ifndef __LibTimeBas_h__
#define __LibTimeBas_h__

#include "Chunk.h"
#include "TimeStamp.h"

class LibTimeBas : public Chunk
{
 public:
 	LibTimeBas(const BString &a_chunkName, Library *a_owner);
 	~LibTimeBas();

 	void set(Buffer *a_data);
	void set();

 	virtual void buildData();

 	virtual void print();

 protected:
 	virtual int calculateChunkSize();

 	TimeStamp m_now;
};

#endif
