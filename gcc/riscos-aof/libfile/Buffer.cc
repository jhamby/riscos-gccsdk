#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream.h>
#include <ctype.h>
#include "Buffer.h"
#include "BString.h"
#include "BError.h"

#ifndef UNIX
extern "C" OS_File(int *);
extern "C" OS_GBPB(int *);
#endif

Buffer::Buffer()
{
 m_size=1000;
 m_data=(unsigned char *) malloc(m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
 m_length=0;
}

Buffer::Buffer(Buffer &a_buffer)
{
 m_size=a_buffer.m_size;
 m_length=a_buffer.m_length;
 m_data=(unsigned char *) malloc(m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
 memcpy(m_data,a_buffer.m_data,m_length);
}

Buffer::Buffer(Buffer *a_buffer)
{
 m_size=a_buffer->m_size;
 m_length=a_buffer->m_length;
 m_data=(unsigned char *) malloc(m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
 memcpy(m_data,a_buffer->m_data,m_length);
}

Buffer::~Buffer()
{
 if(m_data)
 	free(m_data);
}

void Buffer::empty()
{
 if(m_data)
 	free(m_data);
 m_size=1000;
 m_data=(unsigned char *) malloc(m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
 m_length=0;
}

void Buffer::set(Buffer *a_buffer)
{
 m_size=a_buffer->m_size;
 m_length=a_buffer->m_length;
 m_data=(unsigned char *) realloc(m_data, m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
 memcpy(m_data,a_buffer->m_data,m_length);
}

void Buffer::set(Buffer *a_buffer, int a_start, int a_len)
{
 m_size = a_len;
 m_length = a_len;
 m_data = (unsigned char *) realloc(m_data, m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
 memcpy(m_data, &((a_buffer->m_data)[a_start]), m_length);
}

Buffer &Buffer::operator=(const Buffer &a_buffer)
{
 cout << "Buffer::operator=(const Buffer &a_buffer)" << endl;
 if(m_data)
 	free(m_data);
 m_size=a_buffer.m_size;
 m_length=a_buffer.m_length;
 m_data=(unsigned char *) malloc(m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
 memcpy(m_data,a_buffer.m_data,m_length);
 return *this;
}

void Buffer::increase(int a_bytes)
{
 m_size+=a_bytes;
 m_data=(unsigned char *) realloc(m_data,m_size*sizeof(unsigned char));
 if(!m_data)
 	THROW_SPEC_ERR(BError::NewFailed);
}

int Buffer::getInt(int &a_index)
{
 int i,r=0;
 int b;
 for(i=0;i<4;i++)
 {
	if(a_index>=m_length)
 		THROW_SPEC_ERR(BError::OutOfRange);
  	b=m_data[a_index++] & 255;
 	r+=b << (i*8);
 }
 return r;
}

BString Buffer::getStringByLen(int &a_index,int a_len)
{
 BString res;
 int i;
 char bf[2];
 bf[1]=0;
 for(i=0;i<a_len;i++)
 {
	if(a_index>=m_length)
 		THROW_SPEC_ERR(BError::OutOfRange);
  	bf[0]=m_data[a_index++];
 	res+=BString(bf);
 }
 return res;
}

BString Buffer::getString(int &a_index, int a_terminate)
{
 BString res;
 int i;
 char bf[2];
 bf[1]=0;
 while(m_data[a_index] != 0)
 {
	if(a_index>=m_length)
 		THROW_SPEC_ERR(BError::OutOfRange);
  	bf[0]=m_data[a_index++];
 	res+=BString(bf);
 }
 if(a_terminate)
 	a_index++;
 return res;
}

void Buffer::putInt(int a_data)
{
 put(a_data&255);
 put((a_data>>8)&255);
 put((a_data>>16)&255);
 put((a_data>>24)&255);
}

void Buffer::put(unsigned char a_c)
{
 if(m_length>=m_size)
 	increase(200);
 m_data[m_length++]=a_c;
}

void Buffer::put(const unsigned char *a_string, int a_terminate)
{
 int i,l=(int) strlen((const char *)a_string);
 for(i=0;i<l;i++)
 	put(a_string[i]);
 if(a_terminate)
 	put((unsigned char)'\0');
}

void Buffer::put(Buffer *a_buffer)
{
 int i,l=a_buffer->length();
 for(i=0;i<l;i++)
 	put((a_buffer->m_data)[i]);
}

int Buffer::length()
{
 return m_length;
}

unsigned char &Buffer::operator[](int a_index)
{
 if(a_index>=m_length)
 {
 	m_data=(unsigned char *) realloc(m_data,(a_index+1)*sizeof(unsigned char));
	if(!m_data)
 		THROW_SPEC_ERR(BError::NewFailed)
 	int i;
 	m_size=m_length=a_index+1;
 	for(i=0;i<=a_index;i++)
 		m_data[i]=0;
 }
 return m_data[a_index];
}

unsigned char *Buffer::getData()
{
 return m_data;
}

void Buffer::fill(int a_fillValue)
{
 int i;
 for(i=0;i<m_length;i++)
 	m_data[i]=a_fillValue;
}

void Buffer::save(const BString &a_file, int a_append)
{
 FILE *fp;

 if(a_append)
 {
  	THROW_ERR;
 }
 else
 {
#ifdef UNIX
	fp = fopen (a_file(), "wb");
	if (fp == NULL)
	  THROW_SPEC_ERR(BError::CantOpenFile);
	fwrite (m_data, 1, m_length, fp);
	fclose (fp);
#else
 	int reg[6] = { 10, 0, 0, 0, 0, 0 };

 	reg[1] = (int) (a_file());
 	reg[2] = 0xFFD;
 	reg[4] = (int) m_data;
 	reg[5] = reg[4] + m_length;

 	OS_File(reg);
#endif
 }
}

void Buffer::load(const BString &a_file)
{
 FILE *fp;

 fp=fopen(a_file(),"rb");
 if(!fp)
 	THROW_SPEC_ERR(BError::FileNotFound);

#ifdef UNIX
 struct stat f;

 stat (a_file(), &f);
 increase (f.st_size - m_size);
 m_length = f.st_size;
 fread (m_data, 1, m_length, fp);
 fclose (fp);
#else
 fclose(fp);

 int reg[6] = { 17, 0, 0, 0, 0, 0 };

 reg[1] = (int) (a_file());
 OS_File(reg);
 increase(reg[4]-m_size);

 m_length = reg[4];

 reg[0] = 16;
 reg[2] = (int) m_data;
 reg[3] = 0;

 OS_File(reg);
#endif
}

void Buffer::dump()
{
	int l=length();
	int i,j,pos;
	unsigned int byte;
	BString hc,rc,res,line;
	int end=(l-1)/16+1;

	for(j=0;j<end;j+=1)
	{
		rc="";
		hc="";
		line=BString(j*16,"%05X");
		for(i=0;i<16;i++)
		{
			pos=i+j*16;
			if(pos<l)
			{
				byte = (m_data[pos] & 0xFF);
				hc+=BString((int) byte,"%02X")+" ";
				if(isprint((int) byte))
					rc.anhaengen((char) byte,1);
				else
					rc+=BString(".");
			}
			else
			{
				hc+=BString("   ");
				rc+=BString(" ");
			}
		}
		res =  res + line + " : " + hc + "   " + rc + "\n";
	}
	cout << BString("Laenge: ") + l + "\n" + res << endl;
}
