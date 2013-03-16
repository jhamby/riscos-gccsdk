// This file is part of LibPkg.
// Copyright © 2003 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.
// This version modified for use by the GCCSDK
#include "libpkg/control.h"
#include <ostream>
#include <istream>

using namespace std;

namespace pkg {

static string to_lower(string value)
{
	for (string::iterator i=value.begin();i!=value.end();++i)
		*i=tolower(*i);
	return value;
}

control::control()
{}

control::~control()
{}

control::const_iterator control::find(key_type key) const
{
	key._priority=priority(to_lower(key));
	return _data.find(key);
}

control::iterator control::find(key_type key)
{
	key._priority=priority(to_lower(key));
	return _data.find(key);
}

control::mapped_type& control::operator[](key_type key)
{
	key._priority=priority(to_lower(key));
	return _data[key];
}

void control::clear()
{
	_data.clear();
}

string control::pkgname() const
{
	static const key_type key("Package");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::version() const
{
	static const key_type key("Version");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::standards_version() const
{
	static const key_type key("Standards-Version");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::depends() const
{
	static const key_type key("Depends");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::recommends() const
{
	static const key_type key("Recommends");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::suggests() const
{
	static const key_type key("Suggests");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::conflicts() const
{
	static const key_type key("Conflicts");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::description() const
{
	static const key_type key("Description");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

string control::short_description() const
{
	string desc=description();
	return desc.substr(0,desc.find('\n'));
}

string control::url() const
{
	static const key_type key("URL");
	const_iterator f=find(key);
	return (f==end())?string():(*f).second;
}

int control::priority(const string& value) const
{
	return 0;
}

control::key_type::key_type(const string& value):
	string(value),
	_priority(0)
{}

control::key_type::key_type(const char* value):
	string(value),
	_priority(0)
{}

bool control::cmp_key::operator()(const key_type& lhs,
	const key_type& rhs) const
{
	if (lhs.priority()!=rhs.priority())
		return lhs.priority()<rhs.priority();
	string::const_iterator i=lhs.begin();
	string::const_iterator j=rhs.begin();
	string::const_iterator e=i+min(lhs.length(),rhs.length());
	while (i!=e) if (*i++!=*j++) return *--i<*--j;
	return lhs.length()<rhs.length();
}

control::parse_error::parse_error(const char* message):
	runtime_error(message)
{}

std::ostream& operator<<(std::ostream& out,const control& ctrl)
{
	for (control::const_iterator i=ctrl.begin();i!=ctrl.end();++i)
	{
		out << (*i).first << ':';
		string value((*i).second);
		string::size_type i=0;
		string::size_type f=value.find('\n',i);
		while (f!=string::npos)
		{
			if ((f==i)&&(i!=0))
				out << " ." << endl;
			else
				out << ' ' << value.substr(i,f-i) << endl;
			i=f+1;
			f=value.find('\n',i);
		}
		if ((value.length()==i)&&(i!=0))
			out << " ." << endl;
		else
			out << ' ' << value.substr(i) << endl;
	}
	return out;
}

istream& operator>>(istream& in,control& ctrl)
{
	string* field=0;
	bool done=false;
	while (in&&!done)
	{
		// Get line from input stream.
		string line;
		getline(in,line);

		// Convert to sequence.
		string::const_iterator first=line.begin();
		string::const_iterator last=line.end();

		// Strip trailing spaces.
		while ((last!=first)&&isspace(*(last-1))) --last;

		if ((first==last)||isspace(*first))
		{
			// Line is blank or begins with a space:
			// Skip leading spaces.
			string::const_iterator p=first;
			while ((p!=last)&&isspace(*p)) ++p;
			if (p==last)
			{
				// Line is blank (or contains only spaces):
				done=true;
			}
			else
			{
				// Line is a continuation line:
				// Check whether there is a field to continue.
				if (!field)
					throw control::parse_error(
						"continuation line not allowed here");

				// If line contains nothing but a period
				// then skip that character.
				if ((p+1==last)&&(*p=='.')) ++p;

				// Append continuation line to field.
				*field+=string("\n");
				*field+=string(p,last);
			}
		}
		else
		{
			// Line does not begin with a space:
			// Parse fieldname.
			string::const_iterator p=first;
			while ((p!=last)&&(*p!=':'))
			{
				if (isspace(*p))
					throw control::parse_error("syntax error");
				++p;
			}
			string fieldname=string(first,p);
			if (ctrl.find(fieldname)!=ctrl.end())
				throw control::parse_error("duplicate field name");

			// Parse colon at end of fieldname.
			if ((p!=last)&&(*p==':')) ++p;
			else throw control::parse_error("':' expected");

			// Skip spaces.
			while ((p!=last)&&isspace(*p)) ++p;

			// Store field name and value
			field=&ctrl[fieldname];
			*field=string(p,last);
		}
	}
	return in;
}

}; /* namespace pkg */
