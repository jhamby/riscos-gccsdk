#ifndef __Ar_h__
#define __Ar_h__

#include "BString.h"
#include "BList.h"

class ArgParser;
class Library;

class Ar
{
public:
	enum Action
	{
		ActionCreate, ActionInsert,
		ActionDelete, ActionExtract,
		ActionExtractAll,
		ActionExtractDelete,
		ActionNone
	};

	Ar(int a_argc,char **a_argv);
	~Ar();

	void run();
	void usage();
	void version();
private:
	ArgParser *m_argParser;
};

#endif
