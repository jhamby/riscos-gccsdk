#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <set>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include "libpkg/zipfile.h"
#include "libpkg/binary_control.h"
#include "libpkg/version.h"
#include "libpkg/md5.h"

using namespace std;

/**********************************************************

   Return lower case version of a string

**********************************************************/

inline string lowercase(const std::string &text)
{
	string::const_iterator char_iter;
	string lower;

	for (char_iter = text.begin(); char_iter != text.end(); ++char_iter)
		lower += tolower(*char_iter);

	return lower;
}

/*************************************************************

	Class to create the index.

*************************************************************/

class PackageIndexer
{
public:
	PackageIndexer() {check_file = false;}

    void process_directory(const string &dirname, bool rename_pkgs);
	void write_index(const string &filename, int path_ignore, const string &base_url);

	void add_include_contains(const string &text) {include_contains.push_back(lowercase(text));check_file = true;}
	void add_exclude_contains(const string &text) {exclude_contains.push_back(lowercase(text));check_file = true;}

protected:
	bool include_file(const string &name);
	void extract_control(const string &filename, bool rename_pkgs);
        std::string get_env_suffix(const std::string &env_text);

	class IndexEntry
	{
	public:
		IndexEntry(const string &fname, const pkg::binary_control &ctrl);
		IndexEntry &operator=(const IndexEntry &other);
		bool operator<(const IndexEntry &other);

		string filename;
		pkg::binary_control control;
	};

	map<string, IndexEntry> packages;
        set<string> packages_with_env;
	bool check_file;
	vector<string> include_contains;
	vector<string> exclude_contains;
};

/**************************************************************

	Main entry point

**************************************************************/

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cout << "Usage: pkgindex <dir_to_index> <index_file>";
		cout << " [-u <base_url>] [-i <include_contains>]";
		cout << " [-x <exclude_contains>]";
		cout << " [-r (rename according to version and target environment)]";
		cout << endl;
		return -1;
	}

	PackageIndexer indexer;
	string dirname, index_file, base_url;

	int i = 1;
	char opt;
	bool rename_pkgs=false;

	while (i < argc)
	{
		if (argv[i][0] == '-')
		{
			opt = tolower(argv[i][1]);
			if (opt != 'r')
				if (++i >= argc)
				{
					cout << "error missing argument to " << argv[i-1];
					cout << " option." << endl;
					return -1;
				}
		} else
			opt = 0;

		switch(opt)
		{
		case 'u': base_url = argv[i]; break;
		case 'i': indexer.add_include_contains(argv[i]); break;
		case 'x': indexer.add_exclude_contains(argv[i]); break;
		case 'r': rename_pkgs=true; break;
		case 0:
			if (dirname.empty()) dirname = argv[i];
			else index_file = argv[i];
			break;
		default:
			cout << "Invalid option " << argv[i-1] << endl;
			return -1;
			break;
		}
		i++;
	}

	// first pass for renames, second pass to compose index
	if (rename_pkgs)
		indexer.process_directory(dirname,true);
	indexer.process_directory(dirname,false);
	

	indexer.write_index(index_file, dirname.size()+1, base_url);

	return 0;
}

/****************************************************

	Process one directory.

	Recursively calls itself to process the
	subdirectories.

****************************************************/

void PackageIndexer::process_directory(const string &dirname, bool rename_pkgs)
{
	vector<string> sub_dirs;

	DIR *dir = opendir(dirname.c_str());
	if (dir == NULL) return;

	struct dirent *entry;
	struct stat fstat;

	while((entry = readdir(dir)) != NULL)
	{
		std::string name(entry->d_name);
		if (name != "." && name != "..")
		{
			name.insert(0,"/");
			name.insert(0,dirname);

			if (stat(name.c_str(), &fstat) == 0)
			{
				if (S_ISDIR(fstat.st_mode))
				{
					sub_dirs.push_back(name);
				} else
				{
					if (!check_file || include_file(entry->d_name))
						extract_control(name,rename_pkgs);
				}
			}
		}
	}
	closedir(dir);

	if (sub_dirs.size())
	{
		vector<string>::iterator i;
		for (i = sub_dirs.begin(); i != sub_dirs.end(); ++i)
		{
			process_directory(*i,rename_pkgs);
		}
	}
}

/*************************************************************

	Extract the control record from the package and
	store it if it is a new package or the version
	is newer than any package we have already found.

*************************************************************/

void PackageIndexer::extract_control(const string &filename, bool rename_pkgs)
{
	try
	{
		pkg::zipfile zfile(filename);
		stringstream ss;

		zfile.extract("RiscPkg/Control", ss);

		ss.seekp(0);

		pkg::binary_control control;

		ss >> control;

		string pkgname(control.pkgname());

                // Need to take into account environment and modules now
                string env_suffix = get_env_suffix(control.environment());
                string os_depends = get_env_suffix(control.osdepends());
                if (!os_depends.empty()) env_suffix += "-" + os_depends;
                if (!env_suffix.empty()) env_suffix = "_" + env_suffix;

		if (rename_pkgs)
		{
			char *rename_name = strdup(filename.c_str());
			if (!rename_name)
				throw("Out of memory");	
			string newname = dirname(rename_name) + string("/") + control.pkgname() + string("_") + control.version() + env_suffix + string(".zip");
			if (newname != filename)
			{
				cout << "Renaming package " << filename << " to " << newname << endl;
				// and nothing to do if it fails
				int r,e;
				printf("%s -> %s\n",filename.c_str(),newname.c_str());
				r = rename(filename.c_str(),newname.c_str());
				e = errno;
				//cout << r << endl << e << endl;
			}
			free(rename_name);
		}
		else
		{
		        // Key for map is case insensitive
 		        string base_key(lowercase(pkgname));
	                string key = base_key + env_suffix;
			map<string, IndexEntry>::iterator found = packages.find(key);
			if (found == packages.end())
			{
				// new entry
				packages.insert(pair<string, IndexEntry>(key, IndexEntry(filename,control)));
				if (!env_suffix.empty()) packages_with_env.insert(base_key);				
			} else
			{
				pkg::version stored_version((*found).second.control.version());
				pkg::version this_version(control.version());
				if (stored_version < this_version)
				{
					(*found).second = IndexEntry(filename, control);
				}
			}
		
		cout << "found package " << control.pkgname();
		cout << " version " << control.version() << endl;
		}
	} catch(runtime_error &e)
	{
		cout << "File " << filename << " skipped due to error " << e.what() << endl;
	}
}

/*********************************************************************

	Check if this file should be included in the package.

	Test is case insensitive.

	Params:
	   name - leaf name of file

*********************************************************************/

bool PackageIndexer::include_file(const string &name)
{
	bool use_file = true;
	string lowername(lowercase(name));
	std::vector<string>::iterator i;

	if (include_contains.size())
	{
		use_file = false;
		for (i = include_contains.begin(); i != include_contains.end() && !use_file; ++i)
			if (lowername.find((*i)) != string::npos) use_file = true;
	}

	if (use_file && exclude_contains.size())
	{
		for (i = exclude_contains.begin(); i != exclude_contains.end() && use_file; ++i)
		{
			if (lowername.find((*i)) != string::npos) use_file = false;
		}
	}

	return use_file;
}

/**************************************************************************

   Build a suffix from an environment field for use in renaming a file
   or using as part of the unique package keys

   Params:
      env_text - text of the field to process

   Returns:
      sorted list of "-" seperated entries or "" if field is empty
     
**************************************************************************/
std::string PackageIndexer::get_env_suffix(const std::string &env_text)
{
    std::string result;
    if (!env_text.empty())
    {
       // put into a set to remove duplicates and sort
       std::set<std::string> envs;
       std::string env;
	   for (std::string::const_iterator ci = env_text.begin(); ci != env_text.end(); ++ci)
       {
		   const char &c = *ci;
           if (c == ',')
           {
              if (!env.empty()) envs.insert(env);
              env.clear();
           } else if (!isspace(c))
           {
              env += tolower(c);
           }
       }
       if (!env.empty()) envs.insert(env);
       for(std::set<std::string>::const_iterator si = envs.begin(); si != envs.end(); ++si)
       {
		  const std::string &env = *si;
          if (!result.empty()) result += '-';
          result+=env;
       }
     }

     return result;
}

/*******************************************************************

	Write the index file.

	Params:
	  filename - index file name to write
	  path_ignore - number of characters to strip from beginning
	     of package filename to create the url.
	  base_url - start of url to add pathname to.

*******************************************************************/

void PackageIndexer::write_index(const string &filename, int path_ignore, const string &base_url)
{
	ofstream index_stream(filename.c_str());

	cout << "Writing index " << endl;
       
	map<string, IndexEntry>::iterator i;

	// May decide we want to sort them
	list<IndexEntry> sorted_packages;

	for (i = packages.begin(); i != packages.end(); ++i)
	{
	     // Exclude packages that haven't got the environment set if there
             // is an alternative with the environment set
	     bool include = !i->second.control.environment().empty() || !i->second.control.osdepends().empty();
             if (!include)
             {
	        if (packages_with_env.count(i->first) == 0) include = true;
             }
             if (include)
             {
		sorted_packages.push_back((*i).second);
             }
	}
	sorted_packages.sort();

	list<IndexEntry>::iterator j;
	for (j = sorted_packages.begin(); j != sorted_packages.end(); ++j)
	{
		IndexEntry entry = (*j);
		string pkgfilename(entry.filename);
		string url(base_url + pkgfilename.substr(path_ignore));
		struct stat fstat;
		stat(pkgfilename.c_str(), &fstat);

		cout << "package " << entry.control.pkgname();
		cout << ", version " << entry.control.version();
                if (!entry.control.environment().empty())
                {
                   cout << ", environment " << entry.control.environment();
                }
                if (!entry.control.osdepends().empty())
                {
                   cout << ", osdepends " << entry.control.osdepends() << std::endl;
                }
		cout << ", url " << url << endl;

		pkg::md5 md;
		ifstream md5_stream(pkgfilename.c_str());
		md(md5_stream);
		md();

		index_stream << entry.control;
		index_stream << "Size: " << fstat.st_size << endl;
		index_stream << "MD5Sum: " << string(md) << endl;
		index_stream << "URL: " << url << endl;
		index_stream << endl;
		index_stream << endl;
	}
}

PackageIndexer::IndexEntry::IndexEntry(const string &fname, const pkg::binary_control &ctrl)
{
	filename = fname;
	control = ctrl;
}

PackageIndexer::IndexEntry &PackageIndexer::IndexEntry::operator=(const PackageIndexer::IndexEntry &other)
{
	filename = other.filename;
	control = other.control;

	return *this;
}

/**
 * For this program we are only interested in the package name for sorting
 */
bool PackageIndexer::IndexEntry::operator<(const IndexEntry &other)
{
	return (control.pkgname() < other.control.pkgname());
}
