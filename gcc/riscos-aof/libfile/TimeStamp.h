#ifndef __TimeStamp_h__
#define __TimeStamp_h__

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
#include "BString.h"
#include "Buffer.h"

#ifndef CROSS_COMPILE
extern "C" void *OS_File(int *);
extern "C" void *OS_Word(int, int);
extern "C" void *OS_ConvertDataAndTime(int *);
#endif

class TimeStamp
{
public:
	TimeStamp()
	{
	 	setNull();
	}

 	TimeStamp(int a_load,int a_exe)
 	{
 	 	if(m_useNull) return;
 	 	m_load = a_load;
 		m_exe = a_exe;
 	}

 	void setNull()
 	{
	 	m_load = 0;
	 	m_exe = 0;
 	}

 	void getCurrent()
 	{
 	 	if(m_useNull)
 	 	{
 	 	 	setNull();
 	 	 	return;
 	 	}

#ifdef CROSS_COMPILE
		time ((time_t *) &m_exe);
		m_load = 0;
#else
 	 	unsigned char bf[5];
 	 	bf[0]=3;
 	 	OS_Word(14, (int) bf);
 	 	m_exe = (bf[0] & 0xFF);
 	 	m_exe |= (bf[1] & 0xFF) << 8;
 	 	m_exe |= (bf[2] & 0xFF) << 16;
 	 	m_exe |= (bf[3] & 0xFF) << 24;
 	 	m_load = bf[4] & 0xFF;
#endif
 	}

 	BString asString()
 	{
#ifdef CROSS_COMPILE
		return BString (ctime ((time_t *) &m_exe));
#else
 	 	char *fmt = "%24:%MI:%SE %DY-%M3-%CE%YR";
 	 	char res[200];
 	 	int reg[4];
 	 	unsigned char bf[5];
 	 	bf[0] = m_exe & 0xFF;
 	 	bf[1] = (m_exe >> 8) & 0xFF;
 	 	bf[2] = (m_exe >> 16) & 0xFF;
 	 	bf[3] = (m_exe >> 24) & 0xFF;
 	 	bf[4] = m_load & 0xFF;
 	 	reg[0] = (int) bf;
 	 	reg[1] = (int) res;
 	 	reg[2] = 199;
 	 	reg[3] = (int) fmt;
 	 	OS_ConvertDataAndTime(reg);
 	 	return BString(res);
#endif
 	}

 	void put(Buffer &a_dest)
 	{
 	 	int p=((m_load&255)<<16)|((m_exe>>16)&0xFFFF);
 	 	a_dest.putInt(p);
 	 	p=((m_exe&0xFF)<<24)|((m_exe&0xFF00)<<8);
 	 	a_dest.putInt(p);
 	}

 	void get(Buffer *a_dest, int &a_index)
 	{
 	 	if(m_useNull)
 	 	{
 	 	 	setNull();
 	 	 	return;
 	 	}

 	 	int i1, i2, load, exe;
 	 	i1 = a_dest->getInt(a_index);
 	 	i2 = a_dest->getInt(a_index);
 	 	m_load = (i1 >> 16) & 0xFF;
 	 	m_exe = (i1 & 0xFFFF) << 16;
 	 	m_exe |= (i2 >> 24) & 0xFF;
 	 	m_exe |= (i2 >> 8) & 0xFF00;
 	}

 	void load(BString a_file)
 	{
 	 	if(m_useNull)
 	 	{
 	 	 	setNull();
 	 	 	return;
 	 	}

#ifdef CROSS_COMPILE
		struct stat f;

		stat (a_file(), &f);
		m_exe = f.st_ctime;
	        m_load = 0;
#else
 		int reg[6]={17,0,0,0,0,0};

 		reg[1]=(int) a_file();
 		OS_File(reg);
 		m_load = reg[2];
 		m_exe = reg[3];
#endif
 	}

 	void print()
 	{
// 	 	cout << "TimeStamp: " << m_load << ", " << m_exe << endl;
		printf("Load: 0x%X, Exe: 0x%X\n", m_load, m_exe);
 	}

 	static void useNull()
 	{
 	 	m_useNull = 1;
 	}

 	int m_load, m_exe;

private:
 	static int m_useNull;

};

#endif
