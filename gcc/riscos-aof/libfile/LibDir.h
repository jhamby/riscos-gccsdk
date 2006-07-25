#ifndef __LibDir_h__
#define __LibDir_h__

#include <set>

#include "Chunk.h"
#include "BList.h"
#include "BString.h"
#include "TimeStamp.h"

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

struct DirEntrySort
{
  bool operator()(const DirEntry &de1, const DirEntry &de2) const
  {
    return de1.m_chunkIndex < de2.m_chunkIndex;
  }
};

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

	const DirEntry *getDirEntry(const BString &a_fileName) const;
	BString getMemberName(int a_chunkIndex) const;

	List<BString> getMemberList() const;
	List<BString> expandMemberList(const List<BString> &a_wildCards) const;
	List<BString> expandMember(const BString &a_wildCard) const;

	void print(const List_of_p<Chunk> &a_chunks, int a_long) const;


 protected:
 	virtual int calculateChunkSize();
 	void readData(Buffer *a_data);

	typedef std::set<DirEntry, DirEntrySort> DirEntryList;
 	DirEntryList m_fileList;
};

#endif
