#ifndef __TimeStamp_h__
#define __TimeStamp_h__

#include <cstdio>
#include <ctime>
#include <stdint.h>
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

 	TimeStamp(uint32_t a_load, uint32_t a_exe)
 	{
 		if(m_useNull)
 			return;
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
		time_t utime;
		time (&utime);
		const uint64_t rotime = 100ULL*utime + 0x336e996a00ULL;
		m_exe = (uint32_t)(rotime & 0xFFFFFFFF);
		m_load = (uint32_t)((rotime >> 32) & 0xFF);
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

	BString asString() const
	{
#ifdef CROSS_COMPILE
		const uint64_t rotime = ((uint64_t)m_load << 32) + m_exe;
		const time_t utime = (rotime - 0x336e996a00ULL) / 100ULL;
		BString rslt(ctime (&utime));
		rslt.minus(1); // Strip ending new-line
		return rslt;
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

	void put(Buffer &a_dest) const
	{
		a_dest.putInt(((m_load & 0xFF) << 16) | ((m_exe >> 16) & 0xFFFF));
		a_dest.putInt((m_exe & 0xFFFF) << 16);
	}

	void get(Buffer *a_dest, int &a_index)
	{
		if(m_useNull)
		{
			setNull();
			return;
		}

		uint32_t i1 = a_dest->getInt(a_index);
		uint32_t i2 = a_dest->getInt(a_index);
		m_load = (i1 >> 16) & 0xFF;
		m_exe = ((i1 & 0xFFFF) << 16) | ((i2 >> 16) & 0xFFFF);
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
		const uint64_t rotime = 100ULL*f.st_ctime + 0x336e996a00ULL;
		m_exe = (uint32_t)(rotime & 0xFFFFFFFF);
		m_load = (uint32_t)((rotime >> 32) & 0xFF);
#else
 		int reg[6]={17,0,0,0,0,0};

 		reg[1]=(int) a_file();
 		OS_File(reg);
 		m_load = reg[2];
 		m_exe = reg[3];
#endif
	}

	void print() const
	{
		cout << asString();
	}

	static void useNull()
	{
		m_useNull = 1;
	}

	uint32_t m_load, m_exe;

private:
	static int m_useNull;
};

#endif
