// This file is part of LibPkg.
// Copyright © 2004 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#ifndef _LIBPKG_URI
#define _LIBPKG_URI

#include <string>

namespace pkg {

/** A class to represent a uniform resource identifier.
 * This is a suitable representation for accessing the individual
 * components of a URI (the scheme, authority, path, query and fragment).
 * Where an opaque representation is sufficient, a string is likely to
 * be a more efficient method of storage.
 *
 * For simplicity, the scheme, authority, query and fragment are taken to
 * include the prefix/suffix with which they are associated.  This does
 * not match the syntax given in RFC 2396.
 */
class uri
{
private:
	/** The scheme component.
	 * This should either be blank (meaning that there is no scheme
	 * component) or end with ":".
	 */
	std::string _scheme;

	/** The authority component.
	 * This should either be blank (meaning that there is no authority
	 * component) or begin with "//".
	 */
	std::string _authority;

	/** The path component. */
	std::string _path;

	/** The query component.
	 * This should either be blank (meaning that there is no query
	 * component) or begin with "?".
	 */
	std::string _query;

	/** The fragment component.
	 * This should either be blank (meaning that there is no fragment
	 * component) or begin with "#".
	 */
	std::string _fragment;
public:
	/** Construct empty URI. */
	uri();

	/** Construct URI from string. */
	explicit uri(const std::string& s);

	/** Convert URI to string.
	 * @return the URI as a string.
	 */
	operator std::string();

	/** Resolve relative URI to produce absolute URI.
	 * This URI acts as the base.
	 * @param rel_uri the relative URI
	 * @return the absolute URI
	 */
	uri operator+(const uri& rel_uri);

	/** Resolve relative URI to produce absolute URI.
	 * This URI initially acts as the base.
	 * @param rel_uri the relative URI
	 * @return the absolute URI
	 */
	uri& operator+=(const uri& rel_uri);

	/** Get scheme component.
	 * This should either be blank (meaning that there is no scheme
	 * component) or end with ":".
	 * @return the scheme component
	 */
	const std::string& scheme() const
		{ return _scheme; }

	/** Get authority component.
	 * This should either be blank (meaning that there is no authority
	 * component) or begin with "//".
	 * @return the authority component, or 0 if undefined
	 */
	const std::string& authority() const
		{ return _authority; }

	/** Get path component.
	 * @return the path component
	 */
	const std::string& path() const
		{ return _path; }

	/** Get query component.
	 * This should either be blank (meaning that there is no query
	 * component) or begin with "?".
	 * @return the query component
	 */
	const std::string& query() const
		{ return _query; }

	/** Get fragment component.
	 * This should either be blank (meaning that there is no fragment
	 * component) or begin with "#".
	 * @return the fragment component
	 */
	const std::string& fragment() const
		{ return _fragment; }

	/** Set scheme component.
	 * This should either be blank (meaning that there is no scheme
	 * component) or end with ":".
	 * @param scheme the required scheme component
	 * @return a reference to this
	 */
	uri& scheme(const std::string& scheme);

	/** Set authority component.
	 * This should either be blank (meaning that there is no authority
	 * component) or begin with "//".
	 * @param authority the required authority component
	 * @return a reference to this
	 */
	uri& authority(const std::string& authority);

	/** Set path component.
	 * @param path the required path component
	 * @return a reference to this
	 */
	uri& path(const std::string& path);

	/** Set query component.
	 * This should either be blank (meaning that there is no query
	 * component) or begin with "?".
	 * @param query the required query component
	 * @return a reference to this
	 */
	uri& query(const std::string& query);

	/** Set fragment component.
	 * This should either be blank (meaning that there is no fragment
	 * component) or begin with "#".
	 * @param fragment the required fragment component
	 * @return a reference to this
	 */
	uri& fragment(const std::string& fragment);
};

}; /* namespace pkg */

#endif
