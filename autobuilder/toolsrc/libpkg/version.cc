// This file is part of LibPkg.
// Copyright © 2003 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#include <ctype.h>

#include "libpkg/version.h"

using namespace std;

namespace pkg {

/** Compare two characters.
 * The sort order is by ASCII character code, modified so that letters
 * sort earlier than non-letters.
 * @param lhs the left hand side
 * @param rhs the right hand side
 * @return 0 if lhs==rhs, +1 if lhs>rhs or -1 if lhs<rhs
 */
static inline int cmp_char(char lhs,char rhs)
{
	bool lalpha=isalpha(lhs);
	bool ralpha=isalpha(rhs);
	if (lalpha!=ralpha)
	{
		if (lalpha) return -1;
		else return +1;
	}
	else if (lhs!=rhs)
	{
		if (lhs<rhs) return -1;
		else return +1;
	}
	else return 0;
}

/** Lexicographically compare two character sequences.
 * The sort order is by ASCII character code, modified so that letters
 * sort earlier than non-letters.
 * @param lfirst the start of the left hand side
 * @param llast the end of the left hand side
 * @param rfirst the start of the right hand side
 * @param rlast the end of the right hand side
 * @return 0 if lhs==rhs, +1 if lhs>rhs or -1 if lhs<rhs
 */
static int cmp_lex(string::const_iterator lfirst,string::const_iterator llast,
	string::const_iterator rfirst,string::const_iterator rlast)
{
	// Initialise iterators.
	string::const_iterator lp=lfirst;
	string::const_iterator rp=rfirst;

	// Skip matching characters.
	while ((lp!=llast)&&(rp!=rlast)&&(*lp==*rp))
	{
		++lp;
		++rp;
	}

	// Compare by first non-matching character if there is one,
	// otherwise compare by length.
	if (lp!=llast)
	{
		if (rp!=rlast) return cmp_char(*lp,*rp);
		else return +1;
	}
	else
	{
		if (rp!=rlast) return -1;
		else return 0;
	}
}

/** Numerically compare two character sequences.
 * Both sequences must be composed entirely of digits for the result
 * to be meaningful.
 * @param lfirst the start of the left hand side
 * @param llast the end of the left hand side
 * @param rfirst the start of the right hand side
 * @param rlast the end of the right hand side
 * @return 0 if lhs==rhs, +1 if lhs>rhs or -1 if lhs<rhs
 */
static int cmp_num(string::const_iterator lfirst,string::const_iterator llast,
	string::const_iterator rfirst,string::const_iterator rlast)
{
	// Initialise iterators.
	string::const_iterator lp=lfirst;
	string::const_iterator rp=rfirst;

	// Skip leading zeros.
	while ((lp!=llast)&&(*lp=='0')) ++lp;
	while ((rp!=rlast)&&(*rp=='0')) ++rp;

	// Compare by length if there is a difference,
	// otherwise compare by first non-matching digit.
	int llen=llast-lp;
	int rlen=rlast-rp;
	if (llen!=rlen)
	{
		if (llen<rlen) return -1;
		else return +1;
	}
	else
	{
		while ((lp!=llast)&&(*lp==*rp))
		{
			++lp;
			++rp;
		}
		if (lp!=llast)
		{
			if (*lp<*rp) return -1;
			else return +1;
		}
		else return 0;
	}
}

/** Compare two epochs.
 * @param lhs the left hand side
 * @param rhs the right hand side
 * @return 0 if lhs==rhs, +1 if lhs>rhs or -1 if lhs<rhs
 */
static int cmp_epoch(const string& lhs,const string& rhs)
{
	return cmp_num(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

/** Compare two upstream versions or package versions.
 * @param lhs the left hand side
 * @param rhs the right hand side
 * @return 0 if lhs==rhs, +1 if lhs>rhs or -1 if lhs<rhs
 */
static int cmp_verstr(const string& lhs,const string& rhs)
{
	// Initialise iterators.
	string::const_iterator lrm=lhs.end();
	string::const_iterator rrm=rhs.end();
	string::const_iterator lp=lhs.begin();
	string::const_iterator rp=rhs.begin();

	// Compare non-numeric then numeric segments alternately.
	while ((lp!=lrm)||(rp!=rrm))
	{
		// Compare non-numeric segment.
		string::const_iterator lq=lp;
		string::const_iterator rq=rp;
		while ((lp!=lrm)&&!isdigit(*lp)) ++lp;
		while ((rp!=rrm)&&!isdigit(*rp)) ++rp;
		if (int cmp=cmp_lex(lq,lp,rq,rp)) return cmp;

		// Compare numeric segment
		string::const_iterator lr=lp;
		string::const_iterator rr=rp;
		while ((lp!=lrm)&&isdigit(*lp)) ++lp;
		while ((rp!=rrm)&&isdigit(*rp)) ++rp;
		if (int cmp=cmp_num(lr,lp,rr,rp)) return cmp;
	}
	return 0;
}

/** Compare two versions.
 * @param lhs the left hand side
 * @param rhs the right hand side
 * @return 0 if lhs==rhs, +1 if lhs>rhs or -1 if lhs<rhs
 */
static int cmp_version(const version& lhs,const version& rhs)
{
	if (int cmp=cmp_epoch(lhs.epoch(),rhs.epoch()))
		return cmp;
	if (int cmp=cmp_verstr(lhs.upstream_version(),rhs.upstream_version()))
		return cmp;
	if (int cmp=cmp_verstr(lhs.package_version(),rhs.package_version()))
		return cmp;
	return 0;
}

version::version()
{}

version::version(const string& epoch,const string& upstream_version,
	const string& package_version):
	_epoch(epoch),
	_upstream_version(upstream_version),
	_package_version(package_version)
{
	validate();
}

version::version(string::const_iterator first,
	string::const_iterator last)
{
	parse(first,last);
	validate();
}

version::version(const string& verstr)
{
	parse(verstr.begin(),verstr.end());
	validate();
}

version::operator string() const
{
	// Determine whether the upstream version needed to be introduced
	// by a colon and/or terminated by a minus sign.
	bool include_colon=(_epoch.length()!=0)||
		(_upstream_version.find(':')!=string::npos);
	bool include_minus=(_package_version.length()!=0)||
		(_upstream_version.find('-')!=string::npos);

	// Calculate length of result.
	unsigned int length=_epoch.length()+include_colon+
		_upstream_version.length()+include_minus+_package_version.length();

	// Construct result.
	string verstr;
	verstr.reserve(length);
	if (include_colon)
	{
		verstr.append(_epoch);
		verstr.push_back(':');
	}
	verstr.append(_upstream_version);
	if (include_minus)
	{
		verstr.push_back('-');
		verstr.append(_upstream_version);
	}
	return verstr;
}

void version::parse(string::const_iterator first,string::const_iterator last)
{
	// Extract epoch.
	string::const_iterator cs=first;
	while ((cs!=last)&&(*cs!=':')) ++cs;
	if (cs!=last)
	{
		_epoch=string(first,cs);
		first=cs+1;
	}

	// Extract package version.
	string::const_iterator ds=last;
	while ((ds!=first)&&(*(ds-1)!='-')) --ds;
	if (ds!=first)
	{
		_package_version=string(ds,last);
		last=ds-1;
	}

	// Extract upstream version.
	_upstream_version=string(first,last);
}

void version::validate() const
{
	for (string::const_iterator i=_epoch.begin();i!=_epoch.end();++i)
	{
		char ch=*i;
		if (!isdigit(ch))
			throw parse_error("illegal character in epoch");
	}

	for (string::const_iterator i=_upstream_version.begin();
		i!=_upstream_version.end();++i)
	{
		char ch=*i;
		if (!isalnum(ch)&&(ch!='+')&&(ch!='-')&&(ch!='.')&&(ch!=':'))
			throw parse_error("illegal character in upstream version");
	}

	for (string::const_iterator i=_package_version.begin();
		i!=_package_version.end();++i)
	{
		char ch=*i;
		if (!isalnum(ch)&&(ch!='+')&&(ch!='.'))
			throw parse_error("illegal character in package version");
	}
}

bool operator==(const version& lhs,const version& rhs)
{
	return cmp_version(lhs,rhs)==0;
}

bool operator!=(const version& lhs,const version& rhs)
{
	return cmp_version(lhs,rhs)!=0;
}

bool operator<(const version& lhs,const version& rhs)
{
	return cmp_version(lhs,rhs)<0;
}

bool operator>=(const version& lhs,const version& rhs)
{
	return cmp_version(lhs,rhs)>=0;
}

bool operator<=(const version& lhs,const version& rhs)
{
	return cmp_version(lhs,rhs)<=0;
}

bool operator>(const version& lhs,const version& rhs)
{
	return cmp_version(lhs,rhs)>0;
}

version::parse_error::parse_error(const char* message):
	runtime_error(message)
{}

version::parse_error::~parse_error() throw()
{}


}; /* namespace pkg */
