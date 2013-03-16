// This file is part of LibPkg.
// Copyright © 2003 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#ifndef _LIBPKG_ZIPFILE
#define _LIBPKG_ZIPFILE

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <stdexcept>

namespace pkg {

/** An interface class to represent a zip file. */
class zipfile
{
public:
	class file_info;
	class extra_info;
	class riscos_info;

	/** A type to represent an unsigned 16-bit integer. */
	typedef unsigned short uint16;

	/** A type to represent an unsigned 32-bit integer. */
	typedef unsigned long uint32;

	class not_found;
	class unsupported_compression_method;
	class zlib_error;
private:
	/** The pathname to the underlying zip file. */
	std::string _pathname;

	/** The stream used to access the underlying zip file. */
	mutable std::fstream _zfs;

	/** A vector of file information records, in the order they appear
	 * in the underlying zip file.
	 */
	std::vector<file_info*> _directory;
public:
	/** Construct zip file object.
	 * @param pathname the pathname of the zip file.
	 */
	zipfile(const std::string& pathname);
private:
	/** Copy zip file object.
	 * This method is private (and not implemented), therefore
	 * zip file objects cannot be copied.
	 */
	zipfile(const zipfile&);
public:
	/** Destroy zip file object. */
	~zipfile();
private:
	/** Assign zip file object.
	 * This method is private (and not implemented), therefore
	 * zip file objects cannot be assigned.
	 */
	zipfile& operator=(const zipfile&);
public:
	/** Get number of file information records.
	 * @return the number of file information records
	 */
	unsigned int size() const;

	/** Get const file information record at index.
	 * @param index the index
	 * @return a const reference to the file information record
	 */
	const file_info& operator[](unsigned int index) const;

	/** Find const file information record for pathname.
	 * @param pathname the pathname
	 * @return a const pointer to the file information record,
	 *  or 0 if not found
	 */
	const file_info* find(const std::string& pathname) const;

	/** Extract file from zip file.
	 * @param src_pathname the source pathname, with respect to the
	 *  root of the zip file
	 * @param dst_pathname the destination pathname
	 */
	void extract(const std::string& src_pathname,const std::string& dst_pathname) const;

	/** Extract file from zip file to a stream.
	 * 
	 * @param src_pathname the source pathname, with respect to the
	 *  root of the zip file
	 * @param out stream to copy it to.
	 */
	void extract(const std::string& src_pathname,std::ostream& out) const;
};

/** A class to represent a file information record from a zip file. */
class zipfile::file_info
{
private:
	/** The offset to the local file header. */
	uint32 _offset;

	/** The version needed to extract. */
	uint16 _xversion;

	/** The general purpose flag bits. */
	uint16 _gpbits;

	/** The compression method. */
	uint16 _method;

	/** The last modification time (MSDOS format). */
	uint16 _modtime;

	/** The last modification date (MSDOS format). */
	uint16 _moddate;

	/** The CRC. */
	uint32 _crc32;

	/** The compressed file size. */
	uint32 _csize;

	/** The uncompressed file size. */
	uint32 _usize;

	/** The pathname. */
	std::string _pathname;

	/** A map of extra information records, indexed by tag. */
	std::map<uint16,extra_info*> _extra;
public:
	/** Construct file information record. */
	file_info();

	/** Construct file information record from stream.
	 * @param in the input stream
	 */
	file_info(std::istream& in);

	/** Copy file information record.
	 * This method is not currently implemented.
	 */
	file_info(const file_info&);

	/** Destroy file information record. */
	~file_info();

	/** Assign file information record.
	 * This method is not currently implemented.
	 */
	file_info& operator=(const file_info&);

	/** Get offset to file data.
	 * @return the offset from the start of the zip file
	 */
	uint32 offset() const
		{ return _offset; }

	/** Get compression method.
	 * @return the compression method
	 */
	uint16 method() const
		{ return _method; }

	/** Get compressed file size.
	 * @return the compressed file size
	 */
	uint32 csize() const
		{ return _csize; }

	/** Get uncompressed file size.
	 * @return the uncompressed file size
	 */
	uint32 usize() const
		{ return _usize; }

	/** Get pathname.
	 * This is relative to the root of the zipfile, and uses '/' as
	 * the path separator.
	 * @return the pathname
	 */
	const std::string& pathname() const
		{ return _pathname; }

	/** Create extra information record.
	 * @return a pointer to the extra information record
	 */
	template<class extra_type>
	extra_type* create_extra();

	/** Find extra information record.
	 * @return a pointer to the extra information record, or 0 if not found
	 */
	template<class extra_type>
	const extra_type* find_extra() const;
private:
	/** Read local file header from stream.
	 * @param in the input stream
	 */
	void read(std::istream& in);

	/** Read extra information record from stream.
	 * @param in the input stream
	 * @param length the record length
	 */
	void read_extra(std::istream& in,int length);
};

/** A base class to represent an extra information record from a zip file. */
class zipfile::extra_info
{
public:
	/** Construct extra information record. */
	extra_info();

	/** Destroy extra information record. */
	virtual ~extra_info();
};

/** A class to represent a RISC OS extra information record from a zip file. */
class zipfile::riscos_info:
	public zipfile::extra_info
{
private:
	/** The RISC OS extra information record signature. */
	uint32 _sig;

	/** The RISC OS load address. */
	uint32 _loadaddr;

	/** The RISC OS execution address. */
	uint32 _execaddr;

	/** The RISC OS attributes. */
	uint32 _attr;
public:
	/** Construct RISC OS extra information record. */
	riscos_info();

	/** Construct RISC OS extra information record from stream.
	 * @param in the input stream
	 */
	riscos_info(std::istream& in);

	/** Destroy RISC OS extra information record. */
	virtual ~riscos_info();

	/** Determine whether extra information record is valid.
	 * To be valid the signature must be equal to "ARC0".
	 * @return true if valid, otherwise false
	 */
	bool valid() const
		{ return _sig==0x30435241; }

	/** Get RISC OS load address.
	 * @return the RISC OS load address
	 */
	uint32 loadaddr() const
		{ return _loadaddr; }

	/** Get RISC OS execution address.
	 * @return the RISC OS execution address
	 */
	uint32 execaddr() const
		{ return _execaddr; }

	/** Get RISC OS attributes.
	 * @return the RISC OS attributes
	 */
	uint32 attr() const
		{ return _attr; }
private:
	/** Read RISC OS extra information record from stream.
	 * @param in the input stream
	 */
	void read(std::istream& in);
public:
	/** Get tag.
	 * The tag is used to identify the type of an extra information record.
	 * @return the tag.
	 */
	static uint16 tag()
		{ return 0x4341; }
};

template<class extra_type>
extra_type* zipfile::file_info::create_extra()
{
	extra_info*& einfo=_extra[extra_type::tag()];
	if (!einfo) einfo=new extra_type();
	return dynamic_cast<extra_type*>(einfo);
}

template<class extra_type>
const extra_type* zipfile::file_info::find_extra() const
{
	std::map<uint16,extra_info*>::const_iterator f=
		_extra.find(extra_type::tag());
	return (f!=_extra.end())?dynamic_cast<const extra_type*>(f->second):0;
}

/** An exception class for reporting not found errors. */
class zipfile::not_found:
	public std::runtime_error
{
public:
	/** Construct not found error.
	 * @param pathname the pathname that was not found
	 */
	not_found(const std::string& pathname);
};

/** An exception class for reporting unsupported compression method errors. */
class zipfile::unsupported_compression_method:
	public std::runtime_error
{
public:
	/** Construct unsupported compression method error.
	 * @param method the compression method that is unsupported
	 */
	unsupported_compression_method(unsigned int method);

};

/** An exception class for reporting errors in zlib. */
class zipfile::zlib_error:
	public std::runtime_error
{
private:
	/** The zlib error code. */
	int _code;
public:
	/** Construct zlib error.
	 * @param code the zlib error code
	 */
	zlib_error(int code);

	/**
	 * Get error message for a zlib error code.
	 * @param code the zlib error code
	 * @return string with error message.
	 */
	static std::string error_message(int code);

};

}; /* namespace pkg */

#endif
