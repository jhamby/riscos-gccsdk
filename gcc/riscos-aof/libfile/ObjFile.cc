#include <errno.h>
#include <iostream>
#include "ObjFile.h"
#include "BError.h"

ObjFile::ObjFile()
{
}

ObjFile::ObjFile(const BString &a_fileName)
{
 load(a_fileName);
}

ObjFile::ObjFile(Buffer *a_data)
{
 load(*a_data);
}

ObjFile::~ObjFile()
{
}

int ObjFile::sortIntFunc(const void *e1,const void *e2)
{
 int i1=*((int *) e1);
 int i2=*((int *) e2);
 return(i1-i2);
}

List<BString> ObjFile::getExternals()
{
 // Open symboltable
 Chunk *symbolTable = getChunk("OBJ_SYMT");
 if(!symbolTable)
 	THROW_SPEC_ERR(BError::ChunkNotFound);

// Buffer symbolData = symbolTable->getData();
 Buffer symbolData(symbolTable->getData());

 // Open symbol name table
 Chunk *symbolNameTable = getChunk("OBJ_STRT");
 if(!symbolNameTable)
 	THROW_SPEC_ERR(BError::ChunkNotFound);

// Buffer symbolNameData = symbolNameTable->getData();
 Buffer symbolNameData(symbolNameTable->getData());

 int numSymbols = symbolData.length() / 16;
 int i, position, info, index=0;
 BString symbolName;
 List<BString> nameList;

 // Check each symbol
 for(i=0; i<numSymbols; i++)
 {
  	// Index in symbol name chunk
  	position = symbolData.getInt(index);
  	// Info about symbol
  	info = symbolData.getInt(index);
  	// Symbol is external
	//if((info & 1) || (info & (2 | 6)))
	if ((info & 0x03) == 0x03)
	{
  	 	// Get symbol name
  	 	symbolName = symbolNameData.getString(position);
  	 	// Append to list
  	 	nameList.put(symbolName);
		//cout << "External: " << symbolName << "\n";
	}
#if 0
	else {
		symbolName = symbolNameData.getString (position);
		cout << "Other: " << symbolName << ": " << info << "\n";
	}
#endif
  	// Skip values we don't need
  	symbolData.getInt(index);
  	symbolData.getInt(index);
 }

 return nameList;
}

