#ifndef BERROR_HEADER_INCLUDED
#define BERROR_HEADER_INCLUDED

#include <iostream>
#include "BString.h"

class BError
{
public:
	enum ErrorCode
	{
		// Allgemein
		Unusable, Generic, System,
		NotImplemented, NewFailed,

		// ChunkFile
		ChunkNotFound, ChunkIndex,
		NoChunkFile,

		// Buffer
		OutOfRange, FileNotFound,

		// ArgParser
		WrongNumArgs,

		// Library
		NotALib,

		// File handling
		IsAFile, CantOpenFile
	};

	BError(ErrorCode a_err, BString a_file, int a_line);
	BError(BString a_strerr, BString a_file, int a_line);
	~BError();

	ErrorCode GetErrorCode() const { return m_err; }

private:

	friend ostream &operator<<(ostream &out, const BError &a_err);
	BString m_strErr;
	BString m_file;
	ErrorCode m_err;
	int m_line;
};

#define THROW_ERR throw BError (BError::Generic, __FILE__, __LINE__);
#define THROW_SPEC_ERR(errno) throw BError (errno, __FILE__, __LINE__);

#endif
