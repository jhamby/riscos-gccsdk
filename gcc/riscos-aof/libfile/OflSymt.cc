#include <stdio.h>
#include <iostream.h>
#include "OflSymt.h"
#include "TimeStamp.h"
#include "BError.h"
#include "ObjFile.h"
#include "LibDir.h"
#include "LibData.h"

OflSymt::OflSymt(Library *a_owner) : Chunk("OFL_SYMT", a_owner)
{
}

OflSymt::~OflSymt()
{
 List_of_piter<SymbolInfo> iter(m_symbols);
 SymbolInfo *symbol;

 while(symbol = iter.next())
 	delete symbol;
}

void OflSymt::set(const List_of_p<Chunk> &a_memberList, LibDir *a_libDir)
{
 Const_list_of_piter<Chunk> iter(a_memberList);
 Chunk *chunk;
 ObjFile ofile;
 BError *err;
 int chunkIndex;

 for(chunkIndex = 0; chunk = iter.next(); chunkIndex++)
 {
  	List<BString> externalList;

  	// process LibData chunks only
  	if(chunk->getName() == "LIB_DATA")
  	{
  	 	//load file data
  	 	ofile.load(chunk->getData());
  	 	TRY
  	 	{
  	 	 	// try to get symbol table
  	 	 	externalList = ofile.getExternals();
  	 	}
  	 	CATCH(err)
  	 	{
  	 	 	// there is none
  	 	 	// could print a warning

  	 	 	// BString fileName;
  	 	 	// cerr << "Warning: no symbol table in file '";
  	 	 	// cerr << a_libDir->getMemberName(chunkIndex);
  	 	 	// cerr << "' (ignored)" << endl;
  	 	 	continue;
  	 	}
  	}

	// build list of symbols
  	Listiter<BString> extIter(externalList);
  	BString *symbol;
  	SymbolInfo *newSymbol;

  	while(symbol = extIter.next())
  	{
  	 	newSymbol = new SymbolInfo(*symbol, chunkIndex);
  	 	if(!newSymbol)
  	 		THROW_SPEC_ERR(BError::NewFailed);
  	 	m_symbols.put(newSymbol);
  	}
 }
}

void OflSymt::set(Buffer *a_data)
{
 int start = 0, entryStart;
 int bufLen = a_data->length();
 int index, len1, len2;
 BString symbolName;
 SymbolInfo *entry;

 // loop until end of buffer
 while(start < bufLen)
 {
  	entryStart = start;
  	// get chunk index of file
  	index = a_data->getInt(start);
  	// get total entry length
  	len1 = a_data->getInt(start);
  	// check if entry is in use
  	if(index == 0)
  	{
  	 	start = entryStart + len1;
  	 	continue;
  	}

  	// get length of filename + padding
  	len2 = a_data->getInt(start);
  	// get filename
  	symbolName = a_data->getString(start);
  	// skip padding
  	start = entryStart + len1;

	// add to list
  	entry = new SymbolInfo(symbolName, index);
  	if(!entry)
  		THROW_SPEC_ERR(BError::NewFailed);
  	m_symbols.put(entry);
 }
}

void OflSymt::buildData()
{
 List_of_piter<SymbolInfo> iter(m_symbols);
 SymbolInfo *symbol;

 while(symbol = iter.next())
 {
  	// Length of symbol name including '\0'
  	int rawLen = symbol->m_symbolName.laenge() + 1;
  	int nameLen = rawLen;
  	// pad to a multiple of 4
  	while(nameLen & 3)
  		nameLen++;

	// write chunk index
	m_data.putInt(symbol->m_chunkIndex);
	// write total entry length
	m_data.putInt(nameLen + 12);
	// write data length
	m_data.putInt(nameLen);
	// write symbol name & terminator
	m_data.put((unsigned char *)symbol->m_symbolName(), 1);
	// write padding
	while(rawLen < nameLen)
	{
	 	m_data.put((unsigned char)'\0');
	 	rawLen++;
	}
 }
}

void OflSymt::print(LibDir *a_dir, int a_long)
{
 List_of_piter<SymbolInfo> iter(m_symbols);
 SymbolInfo *entry;

 int l, maxLen = 0;

 while(entry = iter.next())
 {
  	l = entry->m_symbolName.laenge();
  	if(l > maxLen)
  		maxLen = l;
 }

 maxLen++;
 iter.reset();

 while(entry = iter.next())
 {
  	cout << format(entry->m_symbolName, maxLen);
  	if(a_long)
		cout << " " << a_dir->getMemberName(entry->m_chunkIndex);
  	cout << endl;
 }
}

int OflSymt::calculateChunkSize()
{
 List_of_piter<SymbolInfo> iter(m_symbols);
 SymbolInfo *symbol;

 int totalLength = 0;

 while(symbol = iter.next())
 {
  	// Length of symbol name including '\0'
  	int nameLen = symbol->m_symbolName.laenge() + 1;
  	// pad to a multiple of 4
  	while(nameLen & 3)
  		nameLen++;

  	// add to total length
  	totalLength += 12 + nameLen;
 }

 return totalLength;
}
