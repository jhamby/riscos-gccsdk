#ifndef __Buffer_h__
#define __Buffer_h__

#include "BString.h"

class Buffer
{
 public:
 	Buffer();
 	Buffer(Buffer &a_buffer);
 	Buffer(Buffer *a_buffer);
 	~Buffer();

 	void set(Buffer *a_buffer);
 	void set(Buffer *a_buffer, int a_start, int a_len);
 	void empty();

 	Buffer &operator=(const Buffer &a_buffer);

 	void put(unsigned char a_c);
 	void put(const unsigned char *a_string, int a_terminate=0);
 	void put(Buffer *a_buffer);
 	void putInt(int a_data);

	int getInt(int &a_index);
	BString getStringByLen(int &a_index, int a_len);
	BString getString(int &a_index, int a_terminate=0);

 	int length();

 	unsigned char &operator[](int a_index);

 	unsigned char *getData();
	void fill(int a_fillValue=0);

	void save(const BString &a_file, int append=0);
	void load(const BString &a_file);

	void dump();

 protected:
	void increase(int a_bytes);
 	unsigned char *m_data;
 	int m_length,m_size;
};

#endif
