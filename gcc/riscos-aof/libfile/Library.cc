#include <cerrno>
#include "Library.h"
#include "LibTime.h"
#include "LibVersion.h"
#include "LibDir.h"
#include "LibData.h"
#include "OflTime.h"
#include "OflSymt.h"
#include "BError.h"
#include "Path.h"

Library::Library(const BString &a_fileName) : ChunkFile(a_fileName)
{
 // Create mandatory empty chunks
 m_time = new LibTime(this);
 m_version = new LibVersion(this);
 m_dir = new LibDir(this);
 // These are for object libraries only
 m_oflTime = 0;
 m_oflSymt = 0;
}

Library::~Library()
{
 // delete all chunks
// The following doesn't work safely:
// while(m_members)
//        delete m_members.get();
}

void Library::save()
{
 // calculate header and build data
 build();
 // And save it
 ChunkFile::save(m_fileName);
}

void Library::build()
{
 int members = m_members.length();

 // Size of Header (12 for ChunkFile Header, 16 per chunk)
 int offset = 12 + 16*members;
 m_data.empty();

 // Chunk FileID
 m_data.putInt(0xC3CBC6C5);

 // Number of chunks in header
 m_data.putInt(members);

 // Number of used chunks
 m_data.putInt(members);

 // Get current timestamp
 m_time->set();

 List_of_piter<Chunk> iter(m_members);
 Chunk *chunk;

 // Write library header (all chunk headers)
 while(chunk = iter.next())
 {
        chunk->buildHeader(offset);
        chunk->writeHeader(&m_data);
 }

 iter.reset();

 // Write chunk data
 while(chunk = iter.next())
 {
        chunk->buildData();
        chunk->writeData(&m_data);
 }
}

void Library::load()
{
 // Load the file into buffer
 ChunkFile::load(m_fileName);

 // How many chunks are there?
 int chunks = getNumChunks();

 int i;
 Chunk *chunk;
 LibData *data;

 // Get library chunks
 if(chunk = getChunk("LIB_TIME"))
        m_time->set(chunk->getData());

 if(chunk = getChunk("LIB_VRSN"))
        m_version->set(chunk->getData());

 if(chunk = getChunk("LIB_DIRY"))
        m_dir->set(chunk->getData());
 else
        THROW_SPEC_ERR(BError::NotALib);

 for(i=0; i<chunks; i++)
 {
        if(getChunkName(i) == "LIB_DATA")
        {
                chunk = getChunk(i);
                data = new LibData(this);
                if(!data)
                        THROW_SPEC_ERR(BError::NewFailed);
                data->set(chunk->getData());
        }
 }

 if(chunk = getChunk("OFL_TIME"))
 {
        m_oflTime = new OflTime(this);
        if(!m_oflTime)
                THROW_SPEC_ERR(BError::NewFailed);
        m_oflTime->set(chunk->getData());
 }

 if(chunk = getChunk("OFL_SYMT"))
 {
        m_oflSymt = new OflSymt(this);
        if(!m_oflSymt)
                THROW_SPEC_ERR(BError::NewFailed);
        m_oflSymt->set(chunk->getData());
 }
}

void Library::deleteMembers(const List<BString> &a_wildMembers)
{
 List<BString> a_members = m_dir->expandMemberList(a_wildMembers);
 Const_listiter<BString> iter(a_members);
 BString *file;

 while(file = iter.next())
 {
        // Check if member can be removed (eg. if it exists)
        // and do it, update the chunk indices in LibDir
        int chunkIndex = m_dir->deleteMember(*file);
        if(chunkIndex >= 0)
        {
                // Get corresponding LibData chunk
                Chunk *chunk = m_members[chunkIndex];
                // and delete it, it will remove itself from the
                // chunks list, so the chunk indices in LibDir
                // are still valid
                delete chunk;
        }
        else
                cerr << "Warning: Member '" << *file << "'not found in library" << endl;
 }
}

void Library::addMembers(const List<BString> &a_newFiles)
{
 List<BString> files;
 Const_listiter<BString> iter(a_newFiles);
 BString *file;

 // Expand wildcards
 while(file = iter.next())
        files.put(Path::getMatchingFiles(*file));

 if (pathNotFound)
   exit(EXIT_FAILURE);

 LibData *data;
 Const_listiter<BString> fiter(files);

 // Process all files in list
 while(file = fiter.next())
 {
        // Check if member can be removed (eg. if it exists)
        // and do it, update the chunk indices in LibDir
        int chunkIndex = m_dir->deleteMember(*file);
        if(chunkIndex >= 0)
        {
                // Get corresponding LibData chunk
                Chunk *chunk = m_members[chunkIndex];
                // and delete it, it will remove itself from the
                // chunks list, so the chunk indices in LibDir
                // are still valid
                delete chunk;
        }

        // Does LibDir chunk allow to add the file (access/ file exists)
        if (m_dir->addFile(*file) >= 0)
        {
                // create a new LibData chunk
                data = new LibData(this);
                if(!data)
                        THROW_SPEC_ERR(BError::NewFailed);
                // Load file
                data->set(*file);
        }
 }
}

void Library::print()
{
 /*
 cout << "Library Info:" << endl;
 m_time->print();
 m_version->print();

 listMembers();

 m_oflTime->print();
 if(m_oflSymt)
        cout << "Library has symbol table" << endl;

 List_of_piter<LibData> iter(m_members);
 LibData *entry;

 while(entry = iter.next())
        entry->print();
 */
}

void Library::listMembers(int a_long)
{
  if (a_long > 0)
    {
      cout << "Format version: ";
      m_version->print();
      cout << endl << endl;
      cout << "Last Modification: ";
      m_time->print();
      cout << endl << endl;
    }

  if (a_long > 0)
    cout << "Contents" << endl << endl;
  m_dir->print(m_members, a_long);
  if (a_long > 0)
    cout << "End of Library" << endl;
}

void Library::listSymbolTable(int a_long)
{
  if (m_oflSymt)
    m_oflSymt->print(m_dir, a_long);
  else
    cout << "Warning: Library does not have a symbol table" << endl;
}

void Library::extractAllMembers(const BString &a_path, bool beVerbose)
{
 extractMembers(m_dir->getMemberList(), a_path, beVerbose);
}

void Library::extractMembers(const List<BString> &a_wildMembers, const BString &a_path, bool beVerbose)
{
  List<BString> a_memberList = m_dir->expandMemberList(a_wildMembers);
  BString path = a_path;

  // Add a '.' to end of path if necessary
  int l = path.laenge();
#ifdef CROSS_COMPILE
  if (l && path[l-1] != '/')
    path += "/";
#else
  if (l && path[l-1] != '.')
    path += ".";
#endif

  // Get numer of chunks
  int maxChunks = m_members.length();

  // Process each name in the list
  Const_listiter<BString> iter(a_memberList);
  BString *member;
  while (member = iter.next())
    {
      // Get library directory information if it exists
      const DirEntry *dirEntry = m_dir->getDirEntry(*member);
      if (dirEntry)
        {
          // Get chunk index of member
          int chunkIndex = dirEntry->m_chunkIndex;
          // Valid?
          if (chunkIndex < maxChunks && chunkIndex >= 0)
            {
              LibData *data = (LibData *) m_members[chunkIndex];
              // Build path
              BString fullPath;
              if (Path::isRelativePath(*member))
                fullPath = path + (*member);
              else
                fullPath = *member;

              // Save file
              if (beVerbose)
                cout << "Writing '" << fullPath << "'" << endl;
              data->createPath(fullPath);
              data->save(fullPath, dirEntry->m_time);
            }
          else
            cout << "Warning: Member '" << (*member) << "' has an impossible chunkindex" << endl;
        }
      else
        cout << "Warning: Member '" << (*member) << "' not found in library" << endl;
    }
}

void Library::attach(Chunk *a_member)
{
 // Add chunk to list of this library
 m_members.put(a_member);
}

void Library::detach(Chunk *a_member)
{
 // Remove chunk from this library
 List_of_piter<Chunk> iter(m_members);
 Chunk *member;

 while(member = iter.next())
        if(member == a_member)
        {
                iter.remove_prev();
                return;
        }

 // Should not happen
 THROW_ERR;
}

void Library::updateOflSymt()
{
 if(m_oflSymt)
        delete m_oflSymt;

 m_oflSymt = new OflSymt(this);
 m_oflSymt->set(m_members, m_dir);
}

void Library::updateOflTime()
{
 if(m_oflTime)
        delete m_oflTime;

 m_oflTime = new OflTime(this);
 m_oflTime->set();
}
