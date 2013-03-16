
#include "summary.h"

SummaryInfo::SummaryInfo(pkg::control &control, const std::string &location)
{
	_name = control.pkgname();
	_version = control.version();
	_summary = control.short_description();
	_location = location;
}

SummaryInfo &SummaryInfo::operator=(const SummaryInfo &other)
{
	_name = other._name;
	_version = other._version;
	_summary = other._summary;
	_location = other._location;

	return *this;
}
