// This file is part of LibPkg.
// Copyright © 2003 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#ifndef _LIBPKG_BINARY_CONTROL
#define _LIBPKG_BINARY_CONTROL

#include "libpkg/control.h"

namespace pkg {

/** A class to represent the content of a RiscPkg binary control file.
 * Behaviour is that of a map<string,string>, except that:
 * - key comparison is case-insensitive;
 * - standard key values for a binary control file are recognised,
 *   and given a priority which overrides the normal sort order.
 *
 * The sort order is subject to change without notice.  This should not
 * break anything, because nothing should depend on the sort order.
 */
class binary_control:
	public control
{
public:
	/** Construct binary control file. */
	binary_control();

	/** Destroy binary control file. */
	virtual ~binary_control();
protected:
	virtual int priority(const std::string& value) const;
};

}; /* namespace pkg */

#endif
