#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include "Chunk.h"
#include "Librarian.h"
#include "ArgParser.h"
#include "ObjFile.h"
#include "BError.h"
#include "LibTime.h"
#include "LibVersion.h"
#include "LibDir.h"
#include "LibData.h"
#include "Library.h"
#include "TimeStamp.h"

Librarian::Librarian(int a_argc,char **a_argv)
{
 m_argParser = new  ArgParser(a_argc, a_argv);
 if(!m_argParser)
 	THROW_SPEC_ERR(BError::NewFailed);
}

Librarian::~Librarian()
{
}

void Librarian::run()
{
 BString libFile, viaFile, destDir;
 List<BString> argList, tmpList;

 int listSymbols = 0;
 int listLib = 0;
 int nullStamps = 0;
 Action action = ActionNone;

 if(m_argParser->getOption("-v", viaFile))
  	m_argParser->addArgsFromFile(viaFile);

 m_argParser->getOption("-q", destDir);

 if(m_argParser->getOption("-h"))
 {
  	usage();
  	return;
 }

 if(m_argParser->getOption("-?"))
 {
  	usage();
  	return;
 }

 argList = m_argParser->getTrailingArgs();
 if(argList)
 	libFile = argList.unput();

 if(m_argParser->getOption("-c"))
 	action = ActionCreate;

 if(m_argParser->getOption("-i"))
 	action = ActionInsert;

 if(m_argParser->getOption("-d"))
 	action = ActionDelete;

 if(m_argParser->getOption("-e"))
 {
  	if(action == ActionDelete)
  		action = ActionExtractDelete;
  	else
 		action = ActionExtract;
 }

 if(m_argParser->getOption("-x"))
 {
     action = ActionExtractAll;
 }

 if(m_argParser->getOption("-t"))
 	TimeStamp::useNull();

 if(m_argParser->getOption("-l"))
 	listLib = 1;

 if(m_argParser->getOption("-L"))
 	listLib = 2;

 if(m_argParser->getOption("-s"))
 	listSymbols = 1;

 if(libFile == "")
 	return;

 m_argParser->warn();

 Library *library = new Library(libFile);

 switch(action)
 {
  	case ActionInsert:
  	case ActionCreate:
  		if((action == ActionInsert) || (argList.length() == 0))
		  {
		    FILE *fp = fopen (libFile (), "r");
		    if (fp != NULL)
		      {
			fclose (fp);
  			library->load();
		      }
		  }

  		library->addMembers(argList);
 		library->updateOflSymt();
 		library->updateOflTime();
  		library->save();
  		break;

  	case ActionExtract:
  		library->load();
		library->extractMembers(argList, destDir);
  		break;

	case ActionExtractAll:
		library->load();
#ifdef CROSS_COMPILE
		if(!m_argParser->getOption("-p"))
		  destDir = "./";
#else
		if(!m_argParser->getOption("-p"))
		  destDir = "@.";
#endif
		library->extractAllMembers(destDir);
		break;

  	case ActionExtractDelete:
  	case ActionDelete:
  		library->load();
  		if(action == ActionExtractDelete)
  			library->extractMembers(argList, destDir);

  		library->deleteMembers(argList);
 		library->updateOflSymt();
 		library->updateOflTime();
  		library->save();
  		break;

  	default:
  		library->load();
  		break;
 }

 if(listLib)
 	library->listMembers(listLib-1);

 if(listSymbols)
 	library->listSymbolTable(1);
}

void Librarian::usage()
{
 cout << "AOF Librarian" << endl;
 cout << "Version 1.00 compiled by gcc " << __VERSION__ << endl;
 cout << "ALF creation and maintenance tool" << endl;
 cout << "(18-Sep-1998) by Bernhard Walter" << endl;
 cout << endl;
 cout << "Syntax: LibFile <Options> <Library> [ <FileList> | <MemberList> ]" << endl;
 cout << endl;
 cout << "Wildcards allowed in <FileList> and <MemberList>:" << endl << endl;
 cout << "'#'       Single character" << endl;
 cout << "'*'       Character sequence (only allowed at the end of a word)" << endl;
 cout << endl;
 cout << "Options:" << endl << endl;
 cout << "-c        Create new library containing files in <FileList>" << endl;
 cout << "-i        Insert files in <FileList>, replace existing members of same name" << endl;
 cout << "-d        Delete the members in <MemberList>" << endl;
 cout << "-e        Extract members in <MemberList>, place them in files of same name" << endl;
 cout << "-x        Extract all members, place them in files of the same name" << endl;
 cout << "-l        List library member names (combine with any other option)" << endl;
 cout << "-L        List library member names & info (combine with any other option)" << endl;
 cout << "-s        List symbol table (combine with any other option)" << endl;
 cout << "-v <File> Take further arguments from file <File>" << endl;
 cout << "-h        Display help" << endl;
 cout << "-t        Use NULL timestamps" << endl;
 cout << "-q <Dir>  Place relative filenames under directory <Dir>" << endl;
 cout << endl;
}
