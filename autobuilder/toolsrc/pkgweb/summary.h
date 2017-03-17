
#ifndef _SUMMARY_H_
#define _SUMMARY_H_

#include "libpkg/control.h"
#include <string>
#include <vector>

class SummaryInfo
{
public:
	SummaryInfo(pkg::control &control, const std::string &location);
	SummaryInfo &operator=(const SummaryInfo &other);

	std::string get_name() const {return _name;}
	std::string get_version() const {return _version;}
	std::string get_summary() const {return _summary;}
	std::string get_location() const {return _location;}
	std::string get_architecture() const {return _architecture;}

protected:
	std::string _name;
	std::string _version;
	std::string _summary;
	std::string _location;
	std::string _architecture;
};

class SectionInfo
{
public:
	SectionInfo(std::string sname) {_name = sname;}

	std::string get_name() const {return _name;}

	void add_package(const SummaryInfo &summary) {_packages.push_back(summary);}

	std::vector<SummaryInfo>::const_iterator begin() const {return _packages.begin();}
	std::vector<SummaryInfo>::const_iterator end() const  {return _packages.end();}

protected:
	std::string _name;
	std::vector<SummaryInfo> _packages;
};

#endif
