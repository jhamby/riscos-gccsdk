#ifndef __LibData_h__
#define __LibData_h__

#include "Chunk.h"
#include "BString.h"
#include "Buffer.h"
#include "TimeStamp.h"

class LibData : public Chunk
{
 public:
 	LibData(Library *a_owner);
 	~LibData();

	void set(Buffer *a_data);
	void set(const BString &a_fileName);

	void save(const BString &a_fileName, TimeStamp a_time);

 	virtual void buildData();

 	virtual void print();

 protected:
 	virtual int calculateChunkSize();
};

#endif
