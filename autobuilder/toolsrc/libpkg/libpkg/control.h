// This file is part of LibPkg.
// Copyright © 2003 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#ifndef _LIBPKG_CONTROL
#define _LIBPKG_CONTROL

#include <map>
#include <string>
#include <iosfwd>
#include <stdexcept>

namespace pkg {

/** A class to represent the content of a RiscPkg control file.
 * Behaviour is that of a map<string,string>, except that:
 * - key comparison is case-insensitive;
 * - standard key values can be recognised (by derivatives of this class),
 *   and given a priority which overrides the normal sort order.
 */
class control
{
public:
	/** The key type.
	 * This is a std::string, augmented to include a priority.
	 *
	 * Note that the standard comparison operators treat this class as
	 * a std::string, so are case sensitive and do not take account of
	 * the priority.  To obtain the modified sort order, use
	 * control::cmp_key.
	 */
	class key_type:
		public std::string
	{
		friend class control;
	private:
		/** The priority. */
		int _priority;
	public:
		/** Construct key from string.
		 * @param value the key value
		 */
		key_type(const std::string& value);

		/** Construct key from C-string.
		 * @param value the key value
		 */
		key_type(const char* value);

		/** Get priority.
		 * @return the priority
		 */
		int priority() const
			{ return _priority; }
	};

	/** The mapped type. */
	typedef std::string mapped_type;

	/** The value type. */
	typedef std::pair<const key_type,mapped_type> value_type;

	/** The iterator type. */
	typedef std::map<key_type,mapped_type>::iterator iterator;

	/** The constant iterator type. */
	typedef std::map<key_type,mapped_type>::const_iterator const_iterator;

	class parse_error;
private:
	/** A class for comparing keys.
	 * This comparison routine is case-insensitive and takes account
	 * of priority.
	 */
	class cmp_key
	{
	public:
		/** Perform comparison.
		 * @param lhs the left hand side
		 * @param rhs the right hand side
		 * @return true if lhs<rhs, otherwise false
		 */
		bool operator()(const key_type& lhs,const key_type& rhs) const;
	};

	/** A map from key to value. */
	std::map<key_type,mapped_type,cmp_key> _data;
public:
	/** Construct control file. */
	control();

	/** Destroy control file. */
	virtual ~control();

	/** Get constant iterator for beginning of control file.
	 * @return the beginning of the control file
	 */
	const_iterator begin() const
		{ return _data.begin(); }

	/** Get constant iterator for end of control file.
	 * @return the end of the control file
	 */
	const_iterator end() const
		{ return _data.end(); }

	/** Find constant iterator for key.
	 * @param key the key to be found
	 * @return the field matching the key if there is one, otherwise end().
	 */
	const_iterator find(key_type key) const;

	/** Get iterator for beginning of control file.
	 * @return the beginning of the control file
	 */
	iterator begin()
		{ return _data.begin(); }

	/** Get iterator for end of control file.
	 * @return the end of the control file
	 */
	iterator end()
		{ return _data.end(); }

	/** Find iterator for key.
	 * @param key the key to be found
	 * @return the field matching the key if there is one, otherwise end().
	 */
	iterator find(key_type key);

	/** Get value corresponding to given key.
	 * If the key does not exist within the control file then it is created.
	 * The value is returned as a reference and is writable.  The key may
	 * be supplied as a string (as opposed to a key_type) and will be
	 * automatically converted.
	 * @param key the key
	 * @return a reference to the value
	 */
	mapped_type& operator[](key_type key);

	/** Clear control file.
	 * All keys and values are deleted.
	 */
	void clear();

	/** Get number of fields.
	 * @return the number of fields
	 */
	unsigned int size();

	/** Get package name.
	 * @return the package name if there is one, otherwise the empty string.
	 */
	std::string pkgname() const;

	/** Get package version.
	 * @return the package version if there is one,
	 *  otherwise the empty string.
	 */
	std::string version() const;

	/** Get standard-version.
	 * @return the standards-version if there is one,
	 *  otherwise the empty string.
	 */
	std::string standards_version() const;

	/** Get description.
	 * @return the description if there is one, otherwise the empty string.
	 */
	std::string description() const;

	/** Get short description.
	 * @return the short description if there is one,
	 *  otherwise the empty string.
	 */
	std::string short_description() const;

	/** Get dependencies.
	 * @return a comma-separated list of dependencies if there are any,
	 *  otherwise the empty string
	 */
	std::string depends() const;

	/** Get recommendations.
	 * @return a comma-separated list of recommendations if there are any,
	 *  otherwise the empty string
	 */
	std::string recommends() const;

	/** Get suggestions.
	 * @return a comma-separated list of suggestions if there are any,
	 *  otherwise the empty string
	 */
	std::string suggests() const;

	/** Get conflicts.
	 * @return a comma-separated list of conflicts if there are any,
	 *  otherwise the empty string
	 */
	std::string conflicts() const;

	/** Get download URL
	 * @return a URL from which the package may be downloaded if there
	 *  is one, otherwise the empty string
	 */
	std::string url() const;
protected:
	/** Get priority.
	 * This function may be overridden to modify the sort order.
	 * By default, all keys have a priority of 0.  The key with the most
	 * negative priority is output first.
	 *
	 * The returned priority must be consistent for any given key
	 * (otherwise data is likely to be lost).
	 * @param value the string to be used as a key (converted to lower case)
	 * @return the priority
	 */
	virtual int priority(const std::string& value) const;
};

/** An exception class for reporting parse errors. */
class control::parse_error:
	public std::runtime_error
{
public:
	/** Construct parse error.
	 * @param message a message which describes the parse error.
	 */
	parse_error(const char* message);
};

/** Write control file to output stream.
 * @param out the output stream
 * @param ctrl the control file
 * @return the output stream
 */
std::ostream& operator<<(std::ostream& out,const control& ctrl);

/** Read control file from input stream.
 * @param in the input stream
 * @param ctrl the control file
 * @return the output stream
 */
std::istream& operator>>(std::istream& in,control& ctrl);

}; /* namespace pkg */

#endif
