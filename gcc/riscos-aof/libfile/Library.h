#ifndef __Library_h__
#define __Library_h__

#include "BList.h"
#include "LibData.h"
#include "ChunkFile.h"

class LibDir;
class LibTime;
class LibVersion;
class OflTime;
class OflSymt;

class Library : public ChunkFile
{
 friend class Chunk;

 public:
 	Library(const BString &a_fileName);
	~Library();

 	void load();
 	void build();
 	void save();

 	void addMembers(const List<BString> &a_newFiles);
 	void extractMembers(const List<BString> &a_memberList, const BString &a_path);
 	void extractAllMembers(const BString &a_path);
	void deleteMembers(const List<BString> &a_members);

 	void listMembers(int a_long=0);
	void listSymbolTable(int a_long=0);

	void updateOflSymt();
	void updateOflTime();

 	void print();

 protected:
	void attach(Chunk *a_member);
	void detach(Chunk *a_member);

	List_of_p<Chunk> m_members;
	LibDir *m_dir;
	LibTime *m_time;
	LibVersion *m_version;
	OflTime *m_oflTime;
	OflSymt *m_oflSymt;
	int m_hasOflSymt, m_hasOflTime;
};

#endif
