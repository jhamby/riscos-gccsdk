#ifndef __LibDir_h__
#define __LibDir_h__

#include "Chunk.h"
#include "BList.h"
#include "BString.h"
#include "TimeStamp.h"

class DirEntry;

class LibDir : public Chunk
{
 public:
 	LibDir(Library *a_owner);
 	~LibDir();

	void set(Buffer *a_data);
	void set(const List<BString> &a_fileList);

 	virtual void buildData();

	int addFile(const BString &a_file);
	int deleteMember(const BString &a_file);

	DirEntry *getDirEntry(const BString &a_fileName);
	BString getMemberName(int a_chunkIndex);

	List<BString> getMemberList();
	List<BString> expandMemberList(const List<BString> &a_wildCards);
	List<BString> expandMember(const BString &a_wildCard);

	void print(const List_of_p<Chunk> &a_chunks, int a_long);


 protected:
 	virtual int calculateChunkSize();
 	void readData(Buffer *a_data);

 	static int sortFunc(const DirEntry*&, const DirEntry*&);

 	List_of_p<DirEntry> m_fileList;
};

class DirEntry
{
 public:
 	DirEntry(BString a_fileName)
 	{
 	 	m_fileName = a_fileName;
 	 	m_chunkIndex = -1;
 	 	m_time.setNull();
 	}

 	DirEntry(BString a_fileName, int a_chunkIndex, TimeStamp a_time)
 	{
 	 	m_fileName = a_fileName;
 	 	m_chunkIndex = a_chunkIndex;
 	 	m_time = a_time;
 	}

 	BString m_fileName;
 	TimeStamp m_time;
 	int m_chunkIndex;
};

#endif
