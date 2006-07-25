#include <cerrno>
#include <cctype>
#include "LibDir.h"
#include "TimeStamp.h"
#include "BError.h"
#include "Path.h"

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
  int sub = 0, theChunk = -1;

  DirEntryList::iterator iter, iterToRemove;
  for (iter = m_fileList.begin(); iter != m_fileList.end(); ++iter)
    {
      // reduce index by one if entry was deleted
      DirEntry &dirEntry = (DirEntry &)*iter;
      dirEntry.m_chunkIndex -= sub;
      // This is the desired entry
      if (iter->m_fileName == a_file)
        {
          // Save the chunkIndex
          theChunk = iter->m_chunkIndex;
          iterToRemove = iter;
          // shift all following entries
          sub = 1;
        }
    }

  if (sub)
    {
      // Remove it from the list
      m_fileList.erase(iterToRemove);
      return theChunk;
    }

  return -1;
}

// return chunk index of new member added
// -1 if nothing added
int LibDir::addFile(const BString &a_file)
{
  if (!exists(a_file))
    {
      cout << "Warning: File '" << a_file << "' cannot be loaded" << endl;
      return -1;
    }

  // get real filename (upper/lower case)
  // todo

  // first chunkindex is 3
  int index = 3 + m_fileList.size();

  // get timestamp
  TimeStamp time;
  time.load(a_file);

  // add to list
  DirEntry entry(a_file, index, time);
  m_fileList.insert(entry);

  return index;
}

void LibDir::readData(Buffer *a_data)
{
  int start = 0;
  const int bufLen = a_data->length();

  // loop until end of buffer
  while (start < bufLen)
    {
      int entryStart = start;
      // get chunk index of file
      int index = a_data->getInt(start);
      // get total entry length
      int len1 = a_data->getInt(start);
      // check if entry is in use
      if(index == 0)
        {
          start = entryStart + len1;
          continue;
        }

      // get length of filename + padding + timestamp (if any)
      int len2 = a_data->getInt(start);
      // get filename
      BString fileName(a_data->getString(start, 1));
      // skip padding
      int padding = len2 - 8 - fileName.laenge() - 1;
      start += padding;

      // read timestamp if present
      TimeStamp time;
      if (start < entryStart+len1)
        time.get(a_data, start);
      else
        time.setNull();

      // add to list
      DirEntry entry(fileName, index, time);
      m_fileList.insert(entry);
    }
}


const DirEntry *LibDir::getDirEntry(const BString &a_fileName) const
{
  DirEntryList::const_iterator iter;
  for (iter = m_fileList.begin(); iter != m_fileList.end(); ++iter)
    {
      if (iter->m_fileName == a_fileName)
        return &(*iter);
    }

  return NULL;
}


void LibDir::buildData()
{
  DirEntryList::const_iterator iter;
  for (iter = m_fileList.begin(); iter != m_fileList.end(); ++iter)
    {
      // get length of filename incl. termination char
      int ol = iter->m_fileName.laenge() + 1;
      int l = ol;
      // round up to multiple of 4
      while (l & 3)
        l++;
      // total entry length
      int len = 4+4+4+l+8;

      // write chunk index
      m_data.putInt(iter->m_chunkIndex);
      // write total length of entry
      m_data.putInt(len);
      // write length of filename + padding + timestamp
      m_data.putInt(l+8);
      // write filename
      m_data.put((unsigned char *)(iter->m_fileName)(),1);

      // write padding
      for (int k = ol; k < l; k++)
        m_data.put((unsigned char)'\0');

      // write timestamp
      iter->m_time.put(m_data);
    }
}


int LibDir::calculateChunkSize()
{
  int len = 0;
  DirEntryList::const_iterator iter;
  for (iter = m_fileList.begin(); iter != m_fileList.end(); ++iter)
    {
      // get length of filename
      int l = iter->m_fileName.laenge() + 1;
      // round up to multiple of 4
      while (l & 3)
        l++;
      // calculate total entry length
      len += 4+4+4+l+8;
    }

 return len;
}


void LibDir::print(const List_of_p<Chunk> &a_chunks, int a_long) const
{
  DirEntryList::const_iterator iter;
  for (iter = m_fileList.begin(); iter != m_fileList.end(); ++iter)
    {
      if (a_long)
        {
          cout << format(iter->m_fileName, 20) << " ";
          cout << iter->m_time.asString() << " ";
          BString len(a_chunks[iter->m_chunkIndex]->getChunkSize());
          cout << format(len, 7, 0) << " Bytes ";
          cout << "(Chunk " << iter->m_chunkIndex << ")";
        }
      else
        cout << iter->m_fileName;

      cout << endl;
    }
}


List<BString> LibDir::getMemberList() const
{
  List<BString> res;

  DirEntryList::const_iterator iter;
  for (iter = m_fileList.begin(); iter != m_fileList.end(); ++iter)
    res.put(iter->m_fileName);

  return res;
}


BString LibDir::getMemberName(int a_chunkIndex) const
{
  DirEntryList::const_iterator iter;
  for (iter = m_fileList.begin(); iter != m_fileList.end() && iter->m_chunkIndex != a_chunkIndex; ++iter)
    /* */;

  if (iter == m_fileList.end())
    {
      // Should not happen
      THROW_SPEC_ERR(BError::ChunkIndex);
      // To make compiler happy
      return "";
    }

  return iter->m_fileName;
}

List<BString> LibDir::expandMemberList(const List<BString> &a_wildCards) const
{
  List<BString> result;
  Listiter<BString> rIter(result);

  Const_listiter<BString> wIter(a_wildCards);
  BString *fn;
  while (fn = wIter.next())
    {
      List<BString> tRes = expandMember(*fn);
      Listiter<BString> sIter(tRes);
      BString *fs;
      while (fs = sIter.next())
        {
          bool found = false;
          rIter.reset();
          BString *ft;
          while (ft = rIter.next())
            {
              if (*ft == *fs)
                {
                  found = true;
                  break;
                }
            }
          if (!found)
            result.put(*fs);
        }
    }
  return result;
}

List<BString> LibDir::expandMember(const BString &a_wildCard) const
{
  List<BString> result;

  DirEntryList::const_iterator iter;
  for (iter = m_fileList.begin(); iter != m_fileList.end(); ++iter)
    {
      if (Path::match(a_wildCard, iter->m_fileName))
        result.put(iter->m_fileName);
    }

  return result;
}
