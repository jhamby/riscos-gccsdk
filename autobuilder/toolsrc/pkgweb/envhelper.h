// Environment helper functions

#ifndef _ENV_HELPERS_H
#define _ENV_HELPERS_H

#include <libpkg/control.h>
#include <string>

inline std::string get_environment_string(const pkg::control ctrl)
{
    std::string estr(ctrl.environment());
	if (!ctrl.osdepends().empty())
	{
		estr += ":" + ctrl.osdepends();
	}
    return estr;
}

std::string get_environment_text(const pkg::control ctrl);

#endif
