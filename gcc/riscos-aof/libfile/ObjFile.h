#ifndef __ObjFile_h__
#define __ObjFile_h__

#include "Buffer.h"
#include "BString.h"
#include "ChunkFile.h"
#include "BList.h"

class ObjFile : public ChunkFile
{
 public:
 	ObjFile();
 	ObjFile(const BString &a_fileName);
 	ObjFile(Buffer *a_data);
 	~ObjFile();

	List<BString> getExternals();

 protected:
	static int sortIntFunc(const void *e1,const void *e2);
};

#endif
