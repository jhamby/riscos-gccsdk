
#include "summary.h"
#include "envhelper.h"

SummaryInfo::SummaryInfo(pkg::control &control, const std::string &location)
{
	_name = control.pkgname();
	_version = control.version();
	_summary = control.short_description();
	_location = location;
    _environment = get_environment_string(control);
}

SummaryInfo &SummaryInfo::operator=(const SummaryInfo &other)
{
	_name = other._name;
	_version = other._version;
	_summary = other._summary;
	_location = other._location;
	_environment = other._environment;

	return *this;
}
