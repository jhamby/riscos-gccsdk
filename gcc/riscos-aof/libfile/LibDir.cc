#include <errno.h>
#include <iostream.h>
#include <ctype.h>
#include "LibDir.h"
#include "TimeStamp.h"
#include "BError.h"
#include "Path.h"

#ifndef CROSS_COMPILE
extern "C" void OS_File(int *);
#endif

LibDir::LibDir(Library *a_owner) : Chunk("LIB_DIRY", a_owner)
{
}

LibDir::~LibDir()
{
}

void LibDir::set(Buffer *a_data)
{
 readData(a_data);
}

void LibDir::set(const List<BString> &a_fileList)
{
 Const_listiter<BString> iter(a_fileList);
 BString *file;

 while(file = iter.next())
 	addFile(*file);
}

int LibDir::deleteMember(const BString &a_file)
{
 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;
 int sub = 0, theChunk = -1;

 // Go through list of directory entries
 while(entry = iter.next())
 {
  	// reduce index by one if entry was deleted
  	entry->m_chunkIndex -= sub;
  	// This is the desired entry
  	if(entry->m_fileName == a_file)
  	{
  	 	// Save the chunkIndex
  	 	theChunk = entry->m_chunkIndex;
  	 	// Remove it from the list
  	 	iter.remove_prev();
  	 	// And delete it
  	 	delete entry;
  	 	// shift all following entries
  	 	sub = 1;
  	}
 }

 if(sub)
 	return theChunk;

 return -1;
}

int LibDir::addFile(const BString &a_file)
{
 DirEntry *entry;
 // first chunkindex is 3
 int index = 3 + m_fileList.length();
 TimeStamp time;

 if(exists(a_file))
 {
  	// get real filename (upper/lower case)
  	// todo

  	// remove file from library if it exists
  	deleteMember(a_file);

  	// get timestamp
  	time.load(a_file);

	// add to list
	entry = new DirEntry(a_file, index, time);
	if(!entry)
		THROW_SPEC_ERR(BError::NewFailed);
	m_fileList.put(entry);
	return 1;
 }
 else
 	cout << "Warning: File '" << a_file << "' cannot be loaded" << endl;

 return 0;
}

int LibDir::sortFunc(const DirEntry *&e1, const DirEntry *&e2)
{
 return (e1->m_chunkIndex) < (e2->m_chunkIndex);
}

void LibDir::readData(Buffer *a_data)
{
 int start = 0, entryStart;
 int bufLen = a_data->length();
 int index, len1, len2;
 TimeStamp time;
 BString fileName;
 DirEntry *entry;

 // loop until end of buffer
 while(start < bufLen)
 {
  	entryStart = start;
  	// get chunk index of file
  	index = a_data->getInt(start);
  	// get total entry length
  	len1 = a_data->getInt(start);
  	// check if entry is in use
  	if(index == 0)
  	{
  	 	start = entryStart + len1;
  	 	continue;
  	}

  	// get length of filename + padding + timestamp (if any)
  	len2 = a_data->getInt(start);
  	// get filename
  	fileName = a_data->getString(start, 1);
  	// skip padding
  	int padding = len2 - 8 - fileName.laenge() - 1;
  	start += padding;
  	// read timestamp if present
  	if(start < (entryStart+len1))
  		time.get(a_data, start);
  	else
  		time.setNull();

	// add to list
  	entry = new DirEntry(fileName, index, time);
  	if(!entry)
  		THROW_SPEC_ERR(BError::NewFailed);
  	m_fileList.put(entry);
 }
 m_fileList.sort(sortFunc);
}

DirEntry *LibDir::getDirEntry(const BString &a_fileName)
{
 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;

 while(entry = iter.next())
 	if(entry->m_fileName == a_fileName)
 		return entry;

 return 0;
}

void LibDir::buildData()
{
 int len=0, l, ol;
 int k;

 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;

 while(entry = iter.next())
 {
	// get length of filename incl. termination char
  	ol = entry->m_fileName.laenge() + 1;
  	l = ol;
  	// round up to multiple of 4
  	while(l & 3)
  		l++;
  	// total entry length
  	len = 4+4+4+l+8;

	// write chunk index
  	m_data.putInt(entry->m_chunkIndex);
  	// write total length of entry
  	m_data.putInt(len);
  	// write length of filename + padding + timestamp
  	m_data.putInt(l+8);
  	// write filename
  	m_data.put((unsigned char *)(entry->m_fileName)(),1);

	// write padding
  	for(k=ol; k<l; k++)
  		m_data.put((unsigned char)'\0');

	// write timestamp
  	(entry->m_time).put(m_data);
 }
}

int LibDir::calculateChunkSize()
{
 int len=0, l;
 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;

 while(entry = iter.next())
 {
  	// get length of filename
  	l = entry->m_fileName.laenge() + 1;
  	// round up to multiple of 4
  	while(l & 3)
  		l++;
  	// calculate total entry length
  	len += 4+4+4+l+8;
 }

 return len;
}

void LibDir::print(const List_of_p<Chunk> &a_chunks, int a_long)
{
 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;

 while(entry = iter.next())
 {
  	if(a_long)
  	{
  		cout << format(entry->m_fileName, 20) << " ";
  		cout << entry->m_time.asString() << " ";
  		BString len(a_chunks[entry->m_chunkIndex]->getChunkSize());
  		cout << format(len, 7, 0) << " Bytes ";
  		cout << "(Chunk " << entry->m_chunkIndex << ")";
  	}
  	else
		cout << entry->m_fileName;

	cout << endl;
 }
}

List<BString> LibDir::getMemberList()
{
 List<BString> res;

 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;

 while(entry = iter.next())
 	res.put(entry->m_fileName);

 return res;
}

BString LibDir::getMemberName(int a_chunkIndex)
{
 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;

 while(entry = iter.next())
 {
  	if(entry->m_chunkIndex == a_chunkIndex)
  		return entry->m_fileName;
 }

 // Should not happen
 THROW_SPEC_ERR(BError::ChunkIndex);
 // To make compiler happy
 return "";
}

List<BString> LibDir::expandMemberList(const List<BString> &a_wildCards)
{
 List<BString> result;
 Listiter<BString> rIter(result);

 Const_listiter<BString> wIter(a_wildCards);
 BString *fn, *ft, *fs;
 int found;

 while(fn = wIter.next())
 {
  	List<BString> tRes = expandMember(*fn);
  	Listiter<BString> sIter(tRes);
  	while(fs = sIter.next())
  	{
	  	found = 0;
	  	rIter.reset();
  	 	while(ft = rIter.next())
	  		if((*ft) == (*fs))
	  		{
	  		 	found = 1;
	  		 	break;
	  		}
	  	if(!found)
	  		result.put(*fs);
  	}
 }
 return result;
}

List<BString> LibDir::expandMember(const BString &a_wildCard)
{
 List<BString> result;
 List_of_piter<DirEntry> iter(m_fileList);
 DirEntry *entry;

 while(entry = iter.next())
 {
  	if(Path::match(a_wildCard, entry->m_fileName))
  		result.put(entry->m_fileName);
 }
 return result;
}

