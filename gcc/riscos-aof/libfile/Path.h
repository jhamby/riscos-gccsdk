#ifndef __Path_h__
#define __Path_h__

#include "BString.h"
#include "BList.h"

class Path
{
 public:
 	static int isRelativePath(const BString &a_path);
 	static int match(const BString &a_wildCard, const BString &a_fileName);
 	static int getNextFile(const BString &a_wildName, BString &a_fileName, int &a_start);
 	static BString getPath(const BString &a_fileName);
 	static BString getFileName(const BString &a_path);
	static List<BString> getMatchingFiles(const BString &a_wildName);

 private:
 	// No instances will ever be needed
 	Path();
};

#endif
