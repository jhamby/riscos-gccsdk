#include <cctype>
#include <glob.h>
#include <iostream>
#include "Path.h"

#ifndef CROSS_COMPILE
extern "C" void *OS_GBPB(int *);
#endif

#ifdef CROSS_COMPILE
#define DIRECTORY_SEPARATOR "/"
#else
#define DIRECTORY_SEPARATOR "."
#endif

int Path::isRelativePath(const BString &a_path)
{
#ifdef CROSS_COMPILE
  // user root directory
  if (a_path.suchen("~") != -1)
    return 0;
  
  // library directory
  if (a_path.suchen("/usr/lib") != -1)
    return 0;
#else
  // is a filing system given?
  if(a_path.suchen(":") != -1)
    return 0;
  
  // root directory
  if(a_path.suchen("$") != -1)
    return 0;
  
  // user root directory
  if(a_path.suchen("&") != -1)
    return 0;
  
  // library directory
  if(a_path.suchen("%") != -1)
    return 0;
  
  // current directory
  if(a_path.suchen("@") != -1)
    return 0;
#endif

 return 1;
}

int Path::match(const BString &a_wildCard, const BString &a_fileName)
{
 int i, l1 = a_wildCard.laenge();
 int l2 = a_fileName.laenge();
 char ch1, ch2;

 // check each character in first string
 for(i = 0; i < l1; i++)
 {
  	ch1 = a_wildCard[i];

	// match: don't care about the rest
  	if(ch1 == '*')
  		return 1;

	// second string is shorter than first
  	if(i >= l2)
  		return 0;

  	ch2 = a_fileName[i];

	// don't care about this character
  	if(ch1 == '#')
  		continue;

	// if a-z compare case insensitive
	if(isalpha(ch1) && (toupper(ch1) == toupper(ch2)))
		continue;

	// no match: characters differ
	if(ch1 != ch2)
  		return 0;
 }

 // both strings must have the same length
 if(i != l2)
 	return 0;

 return 1;
}

List<BString> Path::getMatchingFiles(const BString &a_wildName)
{
	List<BString> result;
 	BString file;
 	int start = 0;
 	int type;

	glob_t zzz;
	if (glob ((char *) (a_wildName ()), 0, NULL, &zzz) == 0)
	  {
	    for (start = 0; start < zzz.gl_pathc; start++) {
	      result.put (zzz.gl_pathv[start]);
	    }
	  }
        return result;

#ifndef CROSS_COMPILE
        if (result.length() == 0) {
        	while(type = getNextFile(a_wildName, file, start))
        		if((type == 1) && (match(a_wildName, file)))
        			result.put(file);
        }
#endif
	if (result.length() == 0)
		cout << "Warning: File '" << a_wildName() << "' not found" << endl;

	return result;
}

#ifndef CROSS_COMPILE
int Path::getNextFile(const BString &a_wildName, BString &a_fileName, int &a_start)
{
 BString path = getPath(a_wildName);
 BString file = "*";

 int regs[7];
 char buffer[1000];

 do
   {
     regs[0] = 10;
     regs[1] = (int) path();
     regs[2] = (int) buffer;
     regs[3] = 1;
     regs[4] = a_start;
     regs[5] = 1000;
     regs[6] = (int) file();

     // See PRMs
     OS_GBPB(regs);

     a_start = regs[4];
   } while (a_start != -1 && regs[3] == 0);

  if (a_start == -1)
    return 0;

 int type = *((int *) (buffer + 16));
 BString full = BString(buffer + 20);

 if(path == "")
   a_fileName = full;
 else
   a_fileName = path + DIRECTORY_SEPARATOR + full;
 
 return type;
}
#endif

BString Path::getPath(const BString &a_fileName)
{
// cout << a_fileName << endl;
 int start = 0;
 int last = 0;
 while((start = a_fileName.suchen(DIRECTORY_SEPARATOR, start)) != -1)
 {
 	start++;
 	last = start;
 }

 if(last == 0)
 	return "";

 return a_fileName.auslesen(0, last-1);
}

BString Path::getFileName(const BString &a_path)
{
 int start = 0;
 int last = 0;
 while((start = a_path.suchen(DIRECTORY_SEPARATOR, start)) != -1)
 {
 	start++;
 	last = start;
 }

 return a_path.auslesen(last, -1);
}
