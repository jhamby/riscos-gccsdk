#include <stdio.h>
#include <iostream.h>
#include <assert.h>
#include <sys/stat.h>
#include "Chunk.h"
#include "Library.h"
#include "BError.h"

#ifndef CROSS_COMPILE
extern "C" void OS_File(int *);
#endif

Chunk::Chunk()
{
  m_chunkSize = -1;
  m_chunkName = "";
  m_owner = 0;
}

Chunk::Chunk(const BString &a_chunkName, Library *a_owner)
{
  m_chunkSize=-1;
  m_chunkName=a_chunkName;
  m_owner = a_owner;
  if(m_owner)
    m_owner->attach(this);
}

Chunk::~Chunk()
{
  if(m_owner)
    m_owner->detach(this);
}

void Chunk::load(Buffer *a_buffer, int a_start, int a_len)
{
  m_data.set(a_buffer, a_start, a_len);
}

void Chunk::load(const BString &a_fileName)
{
  m_data.load(a_fileName);
}

void Chunk::save(const BString &a_fileName, int a_append)
{
  m_data.save(a_fileName, a_append);
}

void Chunk::writeHeader(Buffer *a_buffer)
{
  a_buffer->put(&m_head);
}

void Chunk::writeData(Buffer *a_buffer)
{
  a_buffer->put(&m_data);
}

int Chunk::getChunkSize()
{
  if(m_chunkSize==-1)
    m_chunkSize=calculateChunkSize();
  return m_chunkSize;
}

void Chunk::buildHeader(int &a_offset)
{
  m_head.put((unsigned char *) m_chunkName());
  m_head.putInt(a_offset);
  m_head.putInt(getChunkSize());
  
  a_offset+=getChunkSize();
}

int Chunk::exists(const BString &a_file)
{
  FILE *fp=fopen(a_file(),"r");
  if(!fp)
    return 0;
  fclose(fp);
  return(1);
}

void Chunk::print()
{
  cout << "Chunk::print()" << endl;
}

int Chunk::createPath(const BString &a_path)
{
  int save, start = 0;
  BString dir, fullDir;
  int regs[6] = { 17, 0, 0, 0, 0, 0 };
#ifdef CROSS_COMPILE
  while(a_path.leseWort(dir, start, "/"))
    {
      fullDir += dir;
      save = start;
      if(a_path.leseWort(dir, save, "/"))
  	{
	  struct stat r;
	  
	  //cerr << "create " << fullDir << endl;
	  if (stat ((char *) fullDir (), &r) == 0)
	    if (! S_ISDIR (r.st_mode))
	      THROW_SPEC_ERR(BError::IsAFile);
	  
	  mkdir ((char *) fullDir(), 0755);
	  fullDir += "/";
	}
    }
#else
  while(a_path.leseWort(dir, start, "."))
    {
      fullDir += dir;
      save = start;
      if(a_path.leseWort(dir, save, "."))
  	{
	  regs[0] = 17;
	  regs[1] = (int) fullDir();
	  OS_File(regs);
	  //cerr << "create " << fullDir << endl;
	  
	  // object is a file, but should be a directory
	  if(regs[0] & 1)
	    THROW_SPEC_ERR(BError::IsAFile);
	  
	  regs[0] = 8;
	  regs[1] = (int) fullDir ();
	  regs[4] = 0;
	  OS_File(regs);
	  
	  fullDir += ".";
  	}
    }
#endif
  return 1;
}

BString Chunk::format(const BString &a_string, int a_len, int a_left)
{
 int l = a_string.laenge();
 if(l>a_len)
 	return a_string.auslesen(0, a_len);
 else
 {
  	BString pad;
  	pad.anhaengen(' ', a_len-l);
  	if(a_left)
	 	return a_string + pad;
	else
		return pad + a_string;
 }
}


