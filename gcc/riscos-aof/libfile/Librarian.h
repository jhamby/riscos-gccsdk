#ifndef __Librarian_h__
#define __Librarian_h__

#include "BString.h"
#include "BList.h"

class ArgParser;
class Library;

class Librarian
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

	Librarian(int a_argc,char **a_argv);
	~Librarian();

	void run();
	void usage();

private:
	ArgParser *m_argParser;
};

#endif
