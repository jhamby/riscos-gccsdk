#ifndef __BException_H__
#define __BException_H__

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

class BException
{
 public:
 	BException()
 	{
 	 memcpy(m_prevBuffer,m_lastBuffer,sizeof(jmp_buf));
 	 m_code=0;
 	 m_nestLevel++;
 	}

 	~BException()
 	{
 	 memcpy(m_lastBuffer,m_prevBuffer,sizeof(jmp_buf));
 	 m_nestLevel--;
 	}

 	static void jump(int x, const char *file, int line)
 	{
	 if(BException::m_nestLevel>0)
	  longjmp(BException::m_lastBuffer, x);
	 else
	 {
	  printf("Uncaught Exception: %d\n", ((BException*) x)->m_code);
	  printf("File: %s\n",file);
	  printf("Line: %d\n",line);
	  exit(0);
	 }
 	}

 	jmp_buf m_prevBuffer;
 	static jmp_buf m_lastBuffer;
 	static int m_code;
 	static int m_nestLevel;
};


#define TRY \
	{ \
	BException _excpt_; \
	if((BException::m_code=setjmp(BException::m_lastBuffer))==0)

#define CATCH(x) \
	} \
	(int) (x)=BException::m_code; \
	BException::m_code=0; \
	if((int) (x))

#define THROW(x) \
	BException::jump((int) x,__FILE__, __LINE__);

#define RETHROW \
	} \
	longjmp(BException::m_lastBuffer,BException::m_code);

#endif
