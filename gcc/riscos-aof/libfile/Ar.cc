#include <cctype>
#include <cstdlib>
#include "Chunk.h"
#include "Ar.h"
#include "ArgParser.h"
#include "ObjFile.h"
#include "BError.h"
#include "LibTime.h"
#include "LibVersion.h"
#include "LibDir.h"
#include "LibData.h"
#include "Library.h"
#include "TimeStamp.h"

// Class to emulate the archiver ar.
// Written by Nick Burrett, 29 Nov 1998.

Ar::Ar (int argc, char **argv)
{
  char *argv1 = NULL;

  if (argc >= 2)
    {
      argv1 = new char[strlen (argv[1]) + 2];
      argv1[0] = '-';
      strcpy (argv1 + 1, argv[1]);
      argv[1] = argv1;
    }
  m_argParser = new ArgParser (argc, argv);

  if (! m_argParser)
    THROW_SPEC_ERR (BError::NewFailed);
  
  if (argc>=2)
    delete argv1;
}

Ar::~Ar ()
{
}

void Ar::run ()
{
  BString libFile, destDir;
  List<BString> argList, memberName, tmpList;
  int listSymbols = 0;
  int listLib = 0;
  int nullStamps = 0;
  Action action = ActionNone;
  
  if (m_argParser->getOption ("-h")
      || m_argParser->getOption ("-?"))
    {
      usage ();
      return;
    }

  argList = m_argParser->getTrailingArgs ();
  if (argList)
    libFile = argList.unput ();

  if (m_argParser->getOption ("-V"))
    version ();

#ifdef CROSS_COMPILE
  destDir = "./";
#else
  destDir = "@.";
#endif

  // Delete modules from the archive
  if (m_argParser->getOption ("-d"))
    action = ActionDelete;

  // Insert files into archive (with replacement)
  if (m_argParser->getOption ("-r")
      || m_argParser->getOption ("-q")
      || m_argParser->getOption ("-c"))
    action = ActionInsert;

  // Extract members from the archive
  if (m_argParser->getOption ("-x"))
    action = (argList.length () == 0) ? ActionExtractAll : ActionExtract;

  // Display a table listing the archive contents
  if (m_argParser->getOption ("-t"))
    {
      listLib = 1;
      // Provide more info if verbose is set
      if (m_argParser->getOption ("-v"))
	listLib = 2;
    }

  if (libFile == "")
    {
      usage ();
      return;
    }

  Library *library = new Library (libFile);
  switch (action)
    {
    case ActionInsert:
      // Don't error if the library does not exist.  Sliently create
      // a new one instead.
      try
	{
	  library->load ();
	}

      catch (BError err)
	{
	  // Re-throw the error if it is one we aren't interested in.
	  if (err.m_err != BError::FileNotFound)
	    THROW_SPEC_ERR (BError::FileNotFound);
	};
      library->addMembers (argList);
      library->updateOflTime ();
      library->updateOflSymt ();
      library->save ();
      break;

    case ActionExtract:
      library->load ();
      library->extractMembers (argList, destDir);
      break;

    case ActionExtractAll:
      library->load();
      library->extractAllMembers (destDir);
      break;

    case ActionExtractDelete:
    case ActionDelete:
      library->load ();
      if (action == ActionExtractDelete)
	library->extractMembers (argList, destDir);

      library->deleteMembers (argList);
      library->updateOflTime ();
      library->updateOflSymt ();
      library->save ();
      break;

    default:
      library->load ();
      break;
    }

  if (listLib)
    library->listMembers (listLib - 1);

  if (listSymbols)
    library->listSymbolTable (1);
}

void Ar::version ()
{
  cout << "Version 1.00" << endl;
}

void Ar::usage ()
{
  cout << "ar (AOF Librarian)" << endl;
  cout << "Create, modify and extract from ALF archives" << endl;
  cout << endl;
  cout << "Syntax: ar [-]{dmpqrtx}[abcilosSuvV] archive files..." << endl;

 cout << "Wildcards allowed in <file list> and <MemberList>:" << endl << endl;
 cout << "'#'       Single character" << endl;
 cout << "'*'       Character sequence (only allowed at the end of a word)" << endl;
 cout << endl;
 cout << "Options:" << endl << endl;
 cout << "-c        Create new library containing files in <FileList>" << endl;
 cout << "-r, -q    Insert files in <FileList>, replace existing members of same name" << endl;
 cout << "-d        Delete the members in <MemberList>" << endl;
 cout << "-x        Extract members in <MemberList>, place them in files of same name" << endl;
 cout << "-t        List library member names (combine with any other option)" << endl;
 cout << "-tv       List library member names & info (combine with any other option)" << endl;
 cout << "-h        Display help" << endl;
 cout << "-t        Use NULL timestamps" << endl;
 cout << endl;
}



