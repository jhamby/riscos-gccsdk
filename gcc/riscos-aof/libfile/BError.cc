#include "BError.h"

BError theError(BError::Unusable, __FILE__, __LINE__);;

BError::BError(int a_err, BString a_file, int a_line)
{
	m_err=a_err;
	m_file=a_file;
	m_line=a_line;
}

BError::~BError()
{
}

ostream &operator<<(ostream &out, BError *a_err)
{
#ifdef DEBUG
 	out << "Error " << a_err->m_err << endl;
 	out << "File: " << a_err->m_file << endl;
 	out << "Line: " << a_err->m_line << endl;
#else
	out << "Error: ";
#endif
 	switch(a_err->m_err)
 	{
	 	case BError::Unusable:
	 		out << "Error not usable";
	 		break;
	 	case BError::Generic:
	 		out << "General Error";
	 		break;
	 	case BError::System:
	 		out << "System Error";
	 		break;
	 	case BError::NotImplemented:
	 		out << "Sorry, this function has not been implemented";
	 		break;
	 	case BError::NewFailed:
	 		out << "Operator 'new' failed (out of memory)";
	 		break;
	 	case BError::ChunkNotFound:
	 		out << "The specified chunk could not be found";
	 		break;
	 	case BError::OutOfRange:
	 		out << "Memory access outside array bounds";
	 		break;
	 	case BError::FileNotFound:
	 		out << "The specified file could not be opened";
	 		break;
	 	case BError::WrongNumArgs:
	 		out << "Wrong number of arguments after option";
	 		break;
	 	case BError::ChunkIndex:
	 		out << "Chunk index too high";
	 		break;
	 	case BError::NoChunkFile:
	 		out << "File is not a chunk file (wrong header)";
	 		break;
	 	case BError::NotALib:
	 		out << "File is not a library";
	 		break;
	 	case BError::IsAFile:
	 		out << "Cannot create directory. File of same name exists";
	 		break;
		case BError::CantOpenFile:
			out << "Cannot open file for writing";
			break;
		default:
			out << "Panic! Unknown Error (should not happen)";
 	}
 	out << "." << endl;
}
