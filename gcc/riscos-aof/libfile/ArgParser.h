#ifndef __ArgParser_h__
#define __ArgParser_h__

#include "BString.h"
#include "BList.h"

class ArgParser
{
public:
	ArgParser(int argc, char **argv);
	~ArgParser();

	int getOption(BString a_option, List<BString> &a_argList, int a_min, int a_max);
	int getOption(BString a_option);
	int getOption(BString a_option, BString &a_singleArg);

	List<BString> getTrailingArgs();

	void addArgsFromFile(BString a_fileName);

	void dump();
	void warn();

private:
	void expand(char *a_arg);

	List<BString> m_argList;
};

#endif
