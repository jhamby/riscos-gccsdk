#ifndef __OflTime_h__
#define __OflTime_h__

#include "LibTimeBas.h"

class OflTime : public LibTimeBas
{
public:
	OflTime(Library *a_owner) : LibTimeBas("OFL_TIME", a_owner)
	{
	}
};

#endif
