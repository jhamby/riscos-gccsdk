// This file is part of LibPkg.
// Copyright © 2003 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

// This version was modified by Alan Buckley to open the zipfile in
// read-only mode to prevent packages being skipped due to file
// permissions on unix. 

#include <string.h>
#include "zlib.h"

#include "libpkg/zipfile.h"

using namespace std;

namespace pkg {

namespace {

typedef zipfile::uint16 uint16;
typedef zipfile::uint32 uint32;

/** Read 16-bit unsigned integer from stream.
 * Storage is little-endian.
 * @param in the input stream
 * @return the integer
 */
uint16 read_16(istream& in)
{
	uint16 value=static_cast<uint16>(in.get());
	value|=static_cast<uint16>(in.get())<<8;
	return value;
}

/** Read 32-bit unsigned integer from stream.
 * Storage is little-endian.
 * @param in the input stream
 * @return the integer
 */
uint32 read_32(istream& in)
{
	uint32 value=static_cast<uint32>(in.get());
	value|=static_cast<uint32>(in.get())<<8;
	value|=static_cast<uint32>(in.get())<<16;
	value|=static_cast<uint32>(in.get())<<24;
	return value;
}

/** Read string from stream.
 * This function will always read the specified number of bytes from the
 * stream, however the string itself can be zero-terminated so may be shorter.
 * @param in the input stream
 * @param size the number of bytes to read
 * @return the string
 */
string read_string(istream& in,int size)
{
	string value(size,' ');
	for (int i=0;i!=size;++i)
		value[i]=in.get();
	string::size_type f=value.find(char(0));
	if (f!=string::npos) value.resize(f);
	return value;
}

class buffer
{
private:
	Bytef* _data;
public:
	buffer(unsigned int size);
	~buffer();
	operator Bytef*()
		{ return _data; }

	// Return as a char pointer for access from iostream read/write routines
	operator char *()
	    { return (char *)_data;}
};

buffer::buffer(unsigned int size):
	_data(new Bytef[size])
{}

buffer::~buffer()
{
	delete[] _data;
}

}; /* anonymous namespace */

//ARB: Changed to open file read-only to stop permission problems
zipfile::zipfile(const string& pathname):
	_pathname(pathname),
	_zfs(pathname.c_str(),ios::in/*|ios::out*/|ios::binary)
{
	_zfs.peek();
	while (_zfs&&!_zfs.eof())
	{
		uint32 sig=read_32(_zfs);
		switch (sig)
		{
		case 0x04034b50:
			_directory.push_back(new file_info(_zfs));
			break;
		default:
			_zfs.clear(fstream::eofbit);
			break;
		}
		_zfs.peek();
	}
}

zipfile::~zipfile()
{
	for (vector<file_info*>::iterator i=_directory.begin();
		i!=_directory.end();++i)
	{
		delete *i;
		*i=0;
	}
}

const zipfile::file_info& zipfile::operator[](unsigned int index) const
{
	return *_directory[index];
}

const zipfile::file_info* zipfile::find(const string& pathname) const
{
	for (unsigned int i=0;i!=_directory.size();++i)
		if (_directory[i]->pathname()==pathname) return _directory[i];
	return 0;
}

unsigned int zipfile::size() const
{
	return _directory.size();
}

void zipfile::extract(const string& src_pathname,
	const string& dst_pathname) const
{
	// Find file information record, report error if it does not exist.
	// This is repeated here so output file is not created if
	// source doesn't exist.
	const file_info* finfo=find(src_pathname);
	if (!finfo) throw not_found(src_pathname);

	// Create output stream.
	ofstream out(dst_pathname.c_str());

	extract(src_pathname, out);
}

void zipfile::extract(const string& src_pathname,
	ostream& out) const
{
	// Find file information record, report error if it does not exist.
	const file_info* finfo=find(src_pathname);
	if (!finfo) throw not_found(src_pathname);

	// Reset input stream state and seek to start of compressed data.
	_zfs.clear();
	_zfs.seekg(finfo->offset());

	// Allocate buffers.
	const unsigned int csize=1024;
	const unsigned int usize=1024;
	buffer cbuffer(csize);
	buffer ubuffer(usize);

	// Create and initialise zlib stream.
	z_stream zs;
	zs.zalloc=Z_NULL;
	zs.zfree=Z_NULL;
	zs.opaque=Z_NULL;
	zs.next_in=cbuffer;
	zs.avail_in=0;
	zs.total_in=0;
	zs.next_out=ubuffer;
	zs.avail_out=usize;
	zs.total_out=0;
	int err=inflateInit2(&zs,-MAX_WBITS);
	if (err!=Z_OK) throw zlib_error(err);

	while (err==Z_OK)
	{
		// If input buffer is empty then try to fill it.
		if (!zs.avail_in)
		{
			unsigned int count=min<int>(csize,finfo->csize()-zs.total_in);
			_zfs.read(cbuffer,count);
			zs.next_in=cbuffer;
			zs.avail_in=_zfs.gcount();
		}

		// If output buffer is full then empty it.
		if (!zs.avail_out)
		{
			out.write(ubuffer,zs.next_out-ubuffer);
			zs.next_out=ubuffer;
			zs.avail_out=usize;
		}

		// Decompress next chunk.
		switch (finfo->method())
		{
		case 0:
			{
				unsigned int count=min<int>(zs.avail_in,zs.avail_out);
				memcpy(zs.next_out,zs.next_in,count);
				zs.next_out+=count;
				zs.avail_out-=count;
				zs.total_out+=count;
				zs.next_in+=count;
				zs.avail_in-=count;
				zs.total_in+=count;
			}
			break;
		case 8:
			err=inflate(&zs,Z_NO_FLUSH);
			if (err<Z_OK) throw zlib_error(err);
			break;
		default:
			throw unsupported_compression_method(finfo->method());
		}

		// If there is space in the output buffer, and nothing left to
		// place in the input buffer, then change the return code to
		// Z_STREAM_END because the end has been reached.
		if (zs.avail_out&&(finfo->csize()==zs.total_in)) err=Z_STREAM_END;
	}

	// Finalise zlib stream.
	inflateEnd(&zs);

	// Flush output buffer.
	if (zs.avail_out!=usize)
	{
		out.write(ubuffer,zs.next_out-ubuffer);
	}
}

zipfile::file_info::file_info():
	_offset(0),
	_xversion(0),
	_gpbits(0),
	_method(0),
	_modtime(0),
	_moddate(0),
	_crc32(0),
	_csize(0),
	_usize(0)
{}

zipfile::file_info::file_info(istream& in)
{
	read(in);
}

zipfile::file_info::~file_info()
{
	for (map<uint16,extra_info*>::iterator i=_extra.begin();
		i!=_extra.end();++i)
	{
		delete i->second;
		i->second=0;
	}
}

void zipfile::file_info::read(istream& in)
{
	_xversion=read_16(in);
	_gpbits=read_16(in);
	_method=read_16(in);
	_modtime=read_16(in);
	_moddate=read_16(in);
	_crc32=read_32(in);
	_csize=read_32(in);
	_usize=read_32(in);
	uint16 fn_length=read_16(in);
	uint16 ex_length=read_16(in);
	_pathname=read_string(in,fn_length);
	read_extra(in,ex_length);
	_offset=in.tellg();
	in.seekg(_offset+_csize);
}

void zipfile::file_info::read_extra(istream& in,int length)
{
	uint32 base=in.tellg();
	uint32 offset=base;
	while (offset+4<=base+length)
	{
		uint16 tag=read_16(in);
		uint16 length2=4+read_16(in);
		if (offset+length2<=base+length)
		{
			switch (tag)
			{
			case 0x4341:
				*create_extra<riscos_info>()=riscos_info(in);
				break;
			}
		}
		offset+=length2;
		in.seekg(offset);
	}
	in.seekg(base+length);
}

zipfile::extra_info::extra_info()
{}

zipfile::extra_info::~extra_info()
{}

zipfile::riscos_info::riscos_info():
	_sig(0),
	_loadaddr(0),
	_execaddr(0),
	_attr(0)
{}

zipfile::riscos_info::riscos_info(istream& in)
{
	read(in);
}

zipfile::riscos_info::~riscos_info()
{}

void zipfile::riscos_info::read(istream& in)
{
	_sig=read_32(in);
	_loadaddr=read_32(in);
	_execaddr=read_32(in);
	_attr=read_32(in);
	read_32(in);
}

zipfile::not_found::not_found(const string& pathname)
: runtime_error("\"" + pathname + "\" not found in zip file")
{
}

zipfile::unsupported_compression_method::unsupported_compression_method(
	unsigned int method)
	: runtime_error("unsupported compression method")
{}

zipfile::zlib_error::zlib_error(int code):
    runtime_error(error_message(code)),
	_code(code)
{}

std::string zipfile::zlib_error::error_message(int code)
{
	std::string msg;
	switch (code)
	{
	case Z_VERSION_ERROR:
		msg="version error";
		break;
	case Z_BUF_ERROR:
		msg="buffer error";
		break;
	case Z_MEM_ERROR:
		msg="memory error";
		break;
	case Z_DATA_ERROR:
		msg="data error";
		break;
	case Z_STREAM_ERROR:
		msg="stream error";
		break;
	case Z_ERRNO:
		msg="filesystem error";
		break;
	case Z_OK:
		msg="ok";
		break;
	case Z_STREAM_END:
		msg="end of stream";
		break;
	case Z_NEED_DICT:
		msg="dictionary needed";
		break;
	default:
		if (code<0) msg="unrecognised zlib error";
		else msg="unrecognised zlib return code";
		break;
	}
	return msg;
}

}; /* namespace pkg */
