#ifndef __LibTime_h__
#define __LibTime_h__

#include "LibTimeBas.h"

class LibTime : public LibTimeBas
{
public:
	LibTime(Library *a_owner) : LibTimeBas("LIB_TIME", a_owner)
	{
	}
};

#endif
