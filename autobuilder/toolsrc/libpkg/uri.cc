// This file is part of LibPkg.
// Copyright © 2004 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#include "libpkg/uri.h"

using namespace std;

namespace pkg {

uri::uri(const string& s)
{
	// Initialise index and limit.
	string::size_type i=0;
	string::size_type e=s.size();

	// Parse scheme.
	string::size_type j=i;
	while ((i!=e)&&(s[i]!=':')&&(s[i]!='/')&&(s[i]!='?')&&(s[i]!='#'))
		++i;
	if ((i!=e)&&(i!=j)&&(s[i]==':'))
	{
		_scheme=string(s,j,i-j+1);
		++i;
	}
	else i=j;

	// Parse authority.
	j=i;
	if ((i+1<e)&&(s[i]=='/')&&(s[i+1]=='/'))
	{
		j=i;
		i+=2;
		while ((i!=e)&&(s[i]!='/')&&(s[i]!='?')&&(s[i]!='#')) ++i;
		_authority=string(s,j,i-j);
	}

	// Parse path.
	j=i;
	while ((i!=e)&&(s[i]!='?')&&(s[i]!='#')) ++i;
	_path=string(s,j,i-j);

	// Parse query.
	if ((i!=e)&&(s[i]=='?'))
	{
		j=i;
		++i;
		while ((i!=e)&&(s[i]!='#')) ++i;
		_query=string(s,j,i-j);
	}

	// Parse fragment.
	if ((i!=e)&&(s[i]=='#'))
	{
		j=i;
		++i;
		_fragment=string(s,j,e-j);
	}
}

uri::operator string()
{
	return _scheme+_authority+_path+_query+_fragment;
}

uri uri::operator+(const uri& rel_uri)
{
	uri& base_uri(*this);
	uri abs_uri(rel_uri);

	// If path empty and scheme, authority and query undefined then
	// return after inheriting everything except fragment.
	if (!rel_uri.path().length()&&!rel_uri.scheme().length()&&
		!rel_uri.authority().length()&&!rel_uri.query().length())
	{
		abs_uri=base_uri;
		abs_uri.fragment(rel_uri.fragment());
		return abs_uri;
	}

	// If scheme specified then finished, otherwise inherit scheme.
	if (rel_uri.scheme().length()) return abs_uri;
	abs_uri.scheme(base_uri.scheme());

	// If authority specified then finished, otherwise inherit authority.
	if (rel_uri.authority().length()) return abs_uri;
	abs_uri.authority(base_uri.authority());

	// If absolute path specified then finished, otherwise merge paths.
	if (rel_uri.path().length()&&(rel_uri.path()[0]=='/')) return abs_uri;
	string path=base_uri.path();
	path.resize(path.rfind('/')+1);
	path+=rel_uri.path();
	string::size_type i=1;
	while (i!=path.length())
	{
		string::size_type j=i;
		while ((i!=path.length())&&(path[i]!='/')) ++i;
		string subpath(path,j,i-j);
		if (subpath==".")
		{
			if (i!=path.length()) path.erase(j,2);
			else path.erase(j,1);
			i=j;
		}
		else if (subpath=="..")
		{
			string::size_type f=(j>=2)?path.rfind('/',j-2):string::npos;
			if ((f!=string::npos)&&(string(path,f+1,j-f-2)!=".."))
			{
				if (i!=path.length()) path.erase(f+1,i-f);
				else path.erase(f+1,i-f-1);
				i=f+1;
			}
			else
			{
				if (i!=path.length()) ++i;
			}
		}
		else
		{
			if (i!=path.length()) ++i;
		}
	}
	abs_uri.path(path);

	// Return result.
	return abs_uri;
}

uri& uri::operator+=(const uri& rel_uri)
{
	*this=operator+(rel_uri);
	return *this;
}

uri& uri::scheme(const string& scheme)
{
	_scheme=scheme;
	return *this;
}

uri& uri::authority(const string& authority)
{
	_authority=authority;
	return *this;
}

uri& uri::path(const string& path)
{
	_path=path;
	return *this;
}

uri& uri::query(const string& query)
{
	_query=query;
	return *this;
}

uri& uri::fragment(const string& fragment)
{
	_fragment=fragment;
	return *this;
}

}; /* namespace pkg */
