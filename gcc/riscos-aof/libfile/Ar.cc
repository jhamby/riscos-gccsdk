#include "Ar.h"
#include "BError.h"
#include "Library.h"

// Class to emulate the archiver ar.
// Written by Nick Burrett, 29 Nov 1998.

#define LIBRARIAN_VERSION "1.01"

Ar::Ar(int argc, char **argv)
{
  iAction = ActionNone;
  // iErrorString
  iModShowVersion = iModVerbose = iModCreate = false;

  if (argc <= 1)
    {
      iAction = ActionHelp;
      return;
    }

  char *operationP;
  if ((operationP = argv[1])[0] == '-')
    ++operationP;

  char option;
  while ((option = *operationP++))
    {
      switch (option)
        {
          // Operations:
          case 'd':
            iAction = ActionDelete;
            break;
          case 'm':
            // There is no move action implemented
            cerr << "Ar: warning: move operation is not implemented" << endl;
            break;
          case 'p':
            // There is no print action implemented
            cerr << "Ar: warning: print operation is not implemented" << endl;
            break;
          case 't':
            iAction = ActionList;
            break;
          case 'q':
          case 'r':
            iAction = ActionInsert;
            break;
          case 'x':
            iAction = ActionExtract;
            break;
          case 'h':
          case '?':
            iAction = ActionHelp;
            break;

          // Modifiers:
          case 'c':
            iModCreate = true;
            break;
          case 'v':
            iModVerbose = true;
            break;
          case 'V':
            iModShowVersion = true;
            break;
          case 'a':
          case 'b':
          case 'f':
          case 'i':
          case 'l':
          case 'N':
          case 'o':
          case 'P':
          case 's':
          case 'S':
          case 'u':
            cerr << "Ar: warning: modifier " << option << " is not implemented" << endl;
            break;

          default:
            iErrorString = BString("Unknown operation/modifier");
            break;
        }
    }

  // Read archive name:
  if (argc <= 2)
    return;
  iArchive = BString(argv[2]);

  // Read members:
  for(int i = 3; i < argc; ++i)
    iMembers.put(BString(argv[i]));
}

Ar::~Ar()
{
}

void Ar::run()
{
  if (iErrorString.laenge())
    THROW_SPEC_ERR(iErrorString);

  if (iAction == ActionHelp)
    {
      usage();
      return;
    }
  if (iAction == ActionNone)
    return;

  if (iModShowVersion)
    version();

  if (!iArchive.laenge())
    THROW_SPEC_ERR("No archive specified");

  Library library(iArchive);

  switch (iAction)
    {
      case ActionDelete:
        library.load();
        if (!iMembers.length())
          cerr << "Ar: warning: no members specified to delete" << endl;
        else
          library.deleteMembers(iMembers);
        library.updateOflTime();
        library.updateOflSymt();
        library.save();
        break;

      case ActionList:
        library.load();
        library.listMembers(iModVerbose ? 1 : 0);
        break;

      case ActionInsert:
        if (iModCreate)
          {
            // Don't error if the library does not exist.  Silently create
            // a new one instead.
            try
              {
                library.load();
              }
            catch(BError err)
              {
                // Re-throw the error if it is one we aren't interested in.
                if (err.GetErrorCode() != BError::FileNotFound)
                  throw err;
              }
          }
        else
          library.load();
        library.addMembers(iMembers);
        library.updateOflTime();
        library.updateOflSymt();
        library.save();
        break;

      case ActionExtract:
        library.load();
#ifdef CROSS_COMPILE
        if (!iMembers.length())
          library.extractAllMembers("./", iModVerbose);
        else
          library.extractMembers(iMembers, "./", iModVerbose);
#else
        if (!iMembers.length())
          library.extractAllMembers("@.", iModVerbose);
        else
          library.extractMembers(iMembers, "@.", iModVerbose);
#endif
        break;

      default:
        THROW_SPEC_ERR(BError::Generic);
        break;
    }
}

void Ar::version() const
{
  cout << "ar (AOF Librarian) " LIBRARIAN_VERSION " (" __DATE__ ") [GCCSDK " << __VERSION__ << "]" << endl
       << "Copyright (C) Bernhard Walter, 18 September 1998" << endl
       << "Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 GCCSDK Developers" << endl
       << "http://www.riscos.info/" << endl
       << "Distribute freely" << endl
       << "Use at your own risk" << endl;
}

void Ar::usage() const
{
  cout << "ar (AOF Librarian) " LIBRARIAN_VERSION " (" __DATE__ ") [GCCSDK " << __VERSION__ << "]" << endl
       << "Create, modify, delete and extract from ALF archives" << endl
       << endl
       << "Syntax: ar [-]{dmpqrtxh?}[abcfilNoPsSuvV] <Archive> [ <MemberList> ]" << endl

       << "Wildcards allowed in <MemberList>:" << endl
       << endl
       << "'#'       Single character" << endl
       << "'*'       Character sequence(only allowed at the end of a word)" << endl
       << endl
       << "Operations:" << endl << endl
       << "d         Delete the members in <MemberList>" << endl
       << "m         Move members in library - NOT IMPLEMENTED" << endl
       << "p         Print file(s) found in the archive - NOT IMPLEMENTED" << endl
       << "t[v]      List library member names" << endl
       << "r, q      Insert files in <MemberList>, replace existing members of same name" << endl
       << "x         Extract members in <MemberList> (or all members in case <MemberList> is not specified), place them in files of same name" << endl
       << "h, ?      Display help" << endl
       << endl
       << "Modifiers:" << endl << endl
       << "a, b      NOT IMPLEMENTED" << endl
       << "c         Create new library" << endl
       << "f, i, l, N, o, P, s, S, u   NOT IMPLEMENTED" << endl
       << "v         Be verbose" << endl
       << "V         Show version number" << endl
       << endl;
}
