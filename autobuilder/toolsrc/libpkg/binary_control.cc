// This file is part of LibPkg.
// Copyright © 2003 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#include "libpkg/binary_control.h"

using namespace std;

namespace pkg {

static const map<string,int>& init_priorities()
{
	static map<string,int> priorities;
	int pr=0;
	priorities["md5sum"]=--pr;
	priorities["size"]=--pr;
	priorities["url"]=--pr;
	priorities["conflicts"]=--pr;
	priorities["suggests"]=--pr;
	priorities["recommends"]=--pr;
	priorities["depends"]=--pr;
	priorities["version"]=--pr;
	priorities["source"]=--pr;
	priorities["architecture"]=--pr;
	priorities["standards-version"]=--pr;
	priorities["maintainer"]=--pr;
	priorities["installed-size"]=--pr;
	priorities["section"]=--pr;
	priorities["priority"]=--pr;
	priorities["package"]=--pr;
	pr=0;
	priorities["description"]=++pr;
	return priorities;
}

binary_control::binary_control()
{}

binary_control::~binary_control()
{}

int binary_control::priority(const string& value) const
{
	static const map<string,int>& priorities=init_priorities();
	map<string,int>::const_iterator f=priorities.find(value);
	return (f!=priorities.end())?(*f).second:0;
}

}; /* namespace pkg */
