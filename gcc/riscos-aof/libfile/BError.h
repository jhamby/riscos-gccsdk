#ifndef __HiAsnError__
#define __HiAsnError__

#include <iostream.h>
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

#define THROW_ERR throw BError ((int) BError::Generic, __FILE__, __LINE__);
#define THROW_SPEC_ERR(errno) throw BError ((int) errno, __FILE__, __LINE__);

#endif
