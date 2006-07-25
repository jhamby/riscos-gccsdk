#ifndef __Librarian_h__
#define __Librarian_h__

#include "ArgParser.h"
#include "BString.h"
#include "BList.h"

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

	Librarian(int a_argc, char **a_argv);
	~Librarian();

	void run();
	void usage();

private:
	ArgParser m_argParser;
};

#endif
