#ifndef __HiAsnError__
#define __HiAsnError__

#include <iostream.h>
#include "BException.h"
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

	BError(int a_err, BString a_file, int a_line);
	~BError();

	friend ostream &operator<<(ostream &out, BError *a_err);
protected:
	BString m_file;
	int m_line;
	int m_err;
};

extern BError theError;

#define THROW_ERR \
	{ theError=BError((int) BError::Generic, __FILE__,__LINE__); \
	THROW(&theError) }

#define THROW_SPEC_ERR(errno) \
	{ theError=BError((int) errno, __FILE__,__LINE__); \
	THROW(&theError) }

#endif
