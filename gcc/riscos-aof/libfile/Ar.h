#ifndef AR_HEADER_INCLUDED
#define AR_HEADER_INCLUDED

#include "BError.h"
#include "BList.h"
#include "BString.h"

class Ar
{
public:
	enum Action
	{
		ActionDelete,
		ActionList,
		ActionInsert,
		ActionExtract,
		ActionHelp,
		ActionNone
	};

	Ar(int a_argc, char **a_argv);
	~Ar();

	void run();

private:
	void usage() const;
	void version() const;

private:
	Action iAction;
	BString iErrorString;
	bool iModCreate;
	bool iModVerbose;
	bool iModShowVersion;
	
	BString iArchive;
	List<BString> iMembers;
};

#endif
