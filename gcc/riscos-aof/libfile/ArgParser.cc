#include <errno.h>
#include <iostream>
#include "ArgParser.h"
#include "BError.h"

ArgParser::ArgParser(int argc, char **argv)
{
 int i;
 for(i=1; i<argc; i++)
 	expand(argv[i]);
}

ArgParser::~ArgParser()
{
}

void ArgParser::expand(char *a_arg)
{
 int l = (int) strlen(a_arg);

 if((a_arg[0] == '-') && (l > 2))
 {
  	// This is a multiple option e.g. -clso
  	int i;
  	char bf[2];
  	bf[1] = 0;

  	for(i = 1; i < l ; i++)
  	{
  	 	bf[0] = a_arg[i];
  	 	m_argList.put(BString("-") + BString(bf));
  	}
 }
 else
 	// Simple option or argument
 	m_argList.put(BString(a_arg));
}

List<BString> ArgParser::getTrailingArgs()
{
 List<BString> argList;

 Listiter<BString> iter(m_argList);
 BString *arg;

 // Start with last argument
 iter.end_reset();

 // Search argument list for non-option entries
 while(arg = iter.prev())
 {
  	if((*arg)[0] == '-')
  		break;
	argList.put(*arg);
  	iter.remove_next();
 }

 return argList;
}

int ArgParser::getOption(BString a_option)
{
 List<BString> dummyList;
 return getOption(a_option, dummyList, 0, 0);
}

int ArgParser::getOption(BString a_option, BString &a_singleArg)
{
  List<BString> dummyList;
  int ret = 0;

  try
    {
      ret = getOption(a_option, dummyList, 1, 1);
      if(ret)
	a_singleArg = dummyList[0];
    }
  catch (BError)
    {
      ret = 0;
      a_singleArg = "";
      cout << "Warning: ignoring option '" << a_option << "' (parameter missing)" << endl;
    }
  
  return ret;
}

int ArgParser::getOption(BString a_option, List<BString> &a_argList, int a_min, int a_max)
{
 // Empty parameter argument list
 a_argList = List<BString>();

 Listiter<BString> iter(m_argList);
 BString *arg;

 // Search argument list for desired option
 while(arg = iter.next())
 	if((*arg) == a_option)
 	{
 	 	// Option found, remove it
 	 	iter.remove_prev();
 	 	// Now see if there are arguments (non-options)
 	 	while(arg = iter.next())
 	 	{
 	 		if((*arg)[0] != '-')
 	 		{
 	 			// No leading '-'
 	 		 	if(a_max>0)
 	 		 	{
 	 		 	 	// We still want more args
 	 				a_argList.put(*arg);
 	 				// extract the arg from the list
 	 				iter.remove_prev();
 	 			}
 	 			else
 	 				// finished
 	 				break;
 	 			a_min--;
 	 			a_max--;
 	 		}
 	 		else
 	 		{
 	 			// Leading '-' = next option
 	 			break;
 	 		}

 	 	}
 	 	if(a_min>0)
 	 		THROW_SPEC_ERR(BError::WrongNumArgs);
 	 	return 1;
 	}
 // Option not found
 return 0;
}

void ArgParser::addArgsFromFile(BString a_fileName)
{
 FILE *file;
 file = fopen(a_fileName(), "r");
 if(!file)
 	THROW_SPEC_ERR(BError::FileNotFound);
 int ch;
 char hs[2];
 hs[1]=0;
 BString line;

 do
 {
  	ch = fgetc(file);
  	if((ch <= ' ') || (ch == EOF))
  	{
  	 	if(line != "")
  	 		m_argList.put(line);
  		line = "";
  	}
  	else
  	{
	  	hs[0] = (char) ch;
  		line = line + BString(hs);
  	}
 }
 while(ch != EOF);
 fclose(file);
}

void ArgParser::dump()
{
 Listiter<BString> iter(m_argList);
 BString *arg;
 int i = 0;

 cout << "-----------------------" << endl;
 while(arg = iter.next())
	cout << "Arg[" << i++ << "] = " << (*arg) << endl;
}

void ArgParser::warn()
{
 Listiter<BString> iter(m_argList);
 BString *arg;
 int i = 0;

 while(arg = iter.next())
	cout << "Warning: option " << (*arg) << " unknown" << endl;
}
