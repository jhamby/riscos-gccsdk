// extract symbols from a list of aof and alf files
// (c) Ben Summers 1995

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOOL int
#define FALSE 0
#define TRUE 1

/**********************************************************/

typedef struct
{
  unsigned int ChunkFieldID;
  int MaxChunks;
  int NumChunks;
}
CFF_Header;

#define CFF_CHUNKFIELDID 0xc3cbc6c5

typedef struct
{
  char ChunkID[8];
  int Offset;
  int Size;
}
CFF_Entry;

typedef struct
{
  unsigned int FileType;
  int Version;
  int Areas;
  int NSymbols;
  int EntryAddressArea;
  int EntryAddressOffset;
  // area entries follow
}
AOF_Header;

#define AOFHEADER_FILETYPE 0xc5e2d080

typedef struct
{
  int NameOffset;
  struct
  {
    unsigned int AT_Defined:1;
    unsigned int AT_GlobalScope:1;
    unsigned int AT_Absolute:1;
    unsigned int AT_CaseInsense:1;
    unsigned int AT_Weak:1;
    unsigned int AT_Strong:1;
    unsigned int AT_Common:1;
    unsigned int Zero:25;
  }
  Info;
  int Value;
  int AreaNameOffset;
}
AOF_SymbolEntry;

#define SET     1
#define UNSET   0

typedef struct
{
  int ChunkIndex;
  int EntryLength;
  int DataLength;
  char FileName[4];		// but is usually longer
}
ALF_DirectoryEntry;

/**********************************************************/

FILE *OutFile;
FILE *InFile;
int InFileStart;

char InFilename[64];
char TheFilename[64];

int FilesProcessed = 0;
int SymbolsFound = 0;

/**********************************************************/

static void *
LoadEntry (const char *EntryName, char *Header, int *Size)
{
  CFF_Header *TheHeader = (CFF_Header *)(void *) Header;
  CFF_Entry *Entries = (CFF_Entry *) (TheHeader + 1);
  int l = 0;
  BOOL Found = FALSE;
  char *Block;

  // search to find the entry name
  while (l < TheHeader->NumChunks)
    {
      if (strncmp (Entries[l].ChunkID, EntryName, 8) == 0)
	{
	  Found = TRUE;
	  break;
	}
      l++;
    }

  // did we get it?
  if (!Found)
    {
      printf ("Could not find data within file '%s'\n\n", InFilename);
      return 0;
    }

  // get a block of the correct size
  if ((Block = malloc (Entries[l].Size)) == 0)
    {
      printf ("no room\n\n");
      return 0;
    }

  // seek to the data
  if (fseek (InFile, (long int) (InFileStart + Entries[l].Offset), SEEK_SET)
      != 0)
    {
      printf ("could not get data from file '%s'\n\n", InFilename);
      return 0;
    }

  // load the data
  if (fread (Block, Entries[l].Size, 1, InFile) != 1)
    {
      printf ("could not read data from file '%s'\n\n", InFilename);
      return 0;
    }

  if (Size != 0)
    (*Size) = Entries[l].Size;

  // done
  return Block;
}

static char *
LoadHeader (void)
{
  char *FileHeader;
  CFF_Header TheHeader;

  // get to the start of the file
  if (fseek (InFile, InFileStart, SEEK_SET) != 0)
    {
      printf ("could not get data from file '%s'\n\n", InFilename);
      return 0;
    }

  // read the header in
  if (fread (&TheHeader, sizeof (TheHeader), 1, InFile) != 1)
    {
      printf ("could not read data from file '%s'\n\n", InFilename);
      return 0;
    }

  // check the header
  if (TheHeader.ChunkFieldID != CFF_CHUNKFIELDID || TheHeader.NumChunks <= 0)
    {
      printf ("file '%s' is not an AOF file\n\n", InFilename);
      return 0;
    }

  // get some memory for the header
  if (
      (FileHeader =
       malloc (sizeof (CFF_Header) +
	       (sizeof (CFF_Entry) * TheHeader.NumChunks))) == 0)
    {
      printf ("no room\n\n");
      return 0;
    }

  // copy the header
  *((CFF_Header *)(void *) FileHeader) = TheHeader;

  // read in the entries
  if (fread
      (FileHeader + sizeof (CFF_Header),
       sizeof (CFF_Entry) * TheHeader.NumChunks, 1, InFile) != 1)
    {
      printf ("could not read data from file '%s'\n\n", InFilename);
      return 0;
    }

  // sorted!
  return FileHeader;
}

static BOOL
ProcessAOF (void)
{
  char *FileHeader;
  AOF_Header *AOFHeader;
  AOF_SymbolEntry *Symbols;
  char *StringTable;
  int l;

  // load the header
  if ((FileHeader = LoadHeader ()) == 0)
    return FALSE;

  // load the data from the file
  if (
      ((AOFHeader = (AOF_Header *) LoadEntry ("OBJ_HEAD", FileHeader, 0)) ==
       0)
      ||
      ((Symbols = (AOF_SymbolEntry *) LoadEntry ("OBJ_SYMT", FileHeader, 0))
       == 0) || ((StringTable = LoadEntry ("OBJ_STRT", FileHeader, 0)) == 0))
    {
      return FALSE;
    }

  // check the header
  if (AOFHeader->FileType != AOFHEADER_FILETYPE)
    {
      printf ("File '%s' isn't AOF\n\n", InFilename);
      return FALSE;
    }

  // write a helpful comment
  fprintf (OutFile, "# from %s\n", InFilename);

  // process the symbols
  for (l = 0; l < AOFHeader->NSymbols; l++)
    {
      if ((Symbols[l].Info.AT_Defined == SET)
	  && (Symbols[l].Info.AT_GlobalScope == SET))
	{
	  // found a symbol we want to output
	  fprintf (OutFile, "%s\n", StringTable + Symbols[l].NameOffset);
	  SymbolsFound++;
	}
    }

  // put a blank line in for neatness
  fprintf (OutFile, "\n");

  // increment number of files processed
  FilesProcessed++;

  // free the data
  free (FileHeader);
  free (AOFHeader);
  free (Symbols);
  free (StringTable);

  return TRUE;
}

static BOOL
ProcessALF (void)
{
  char *FileHeader;
  char *Entries;
  int DirectorySize;
  int CharsToCompare = -1;
  char InternalName[64];
  int l, i;
  BOOL DoThisOne;
  int Offset;
  ALF_DirectoryEntry *Entry;
  CFF_Header *CFFHeader;
  CFF_Entry *CFFEntries;

  // OK, load the ALF file header
  InFileStart = 0;
  if ((FileHeader = LoadHeader ()) == 0)
    return FALSE;

  CFFHeader = (CFF_Header *)(void *) FileHeader;
  CFFEntries = (CFF_Entry *) (CFFHeader + 1);

  // load the directory
  if ((Entries = LoadEntry ("LIB_DIRY", FileHeader, &DirectorySize)) == 0)
    {
      return FALSE;
    }

  // get the internal name
  l = i = 0;
  while (InFilename[l] > ' ' && InFilename[l] != ',')
    {
      l++;
    }
  if (InFilename[l] != ',' || InFilename[l + 1] <= ' ')
    {
      printf ("no internal filename specified for '%s'\n\n", InFilename);
      return FALSE;
    }
  l++;				// skip the ,
  while (InFilename[l] > ' ' && l < 62)
    {
      InternalName[i] = InFilename[l];
      l++;
      i++;
    }
  InternalName[i] = '\0';

  // see if it's got a wildcard
  l = 0;
  while (InternalName[l] > ' ')
    {
      if (InternalName[l] == '*')
	{
	  CharsToCompare = l;

	  // check that the wildcard is the last thing in the internal filename
	  if (InternalName[l + 1] > ' ')
	    {
	      printf
		("wildcard is not the last character of the internal filename for '%s'\n\n",
		 InFilename);
	      return FALSE;
	    }
	}
      l++;
    }
// printf("%s %s %s %d\n", InFilename, TheFilename, InternalName, CharsToCompare);

  // go through the entries...
  Offset = 0;
  while (Offset < DirectorySize)
    {
      DoThisOne = FALSE;
      Entry = (ALF_DirectoryEntry *)(void *) (Entries + Offset);
// printf("%d : %s\n", Entry->ChunkIndex, Entry->FileName);

      if (Entry->ChunkIndex != 0)
	{
	  if (CharsToCompare == -1)
	    {
	      if (strcmp (Entry->FileName, InternalName) == 0)
		DoThisOne = TRUE;
	    }
	  else
	    {
	      if (CharsToCompare == 0
		  || strncmp (Entry->FileName, InternalName,
			      CharsToCompare) == 0)
		DoThisOne = TRUE;
	    }
	}

      if (DoThisOne)
	{
	  // make a new in filename for the benifit of the comments and errors
	  sprintf (InFilename, "%s,%s", TheFilename, Entry->FileName);

	  // check that the entry is a valid one
	  if ((Entry->ChunkIndex >= CFFHeader->NumChunks) ||
	      (strncmp (CFFEntries[Entry->ChunkIndex].ChunkID, "LIB_DATA", 8)
	       != 0))
	    {
	      printf ("invalid ALF entry for '%s'\n\n", InFilename);
	      return FALSE;
	    }

	  // process the AOF file we've just found
	  InFileStart = CFFEntries[Entry->ChunkIndex].Offset;
	  if (!ProcessAOF ())
	    return FALSE;
	}

      Offset += Entry->EntryLength;
    }

  // free the stuff
  free (FileHeader);
  free (Entries);

  return TRUE;
}

static BOOL
ProcessFile (void)
{
  int l;
  struct
  {
    CFF_Header Header;
    CFF_Entry FirstEntry;
  }
  Begin;
  BOOL Recognised;
  BOOL IsALF = FALSE;

  // copy the filename into a buffer up until the comma seperator
  l = 0;
  while ((InFilename[l] != ',') && (InFilename[l] > 32) && (l < 62))
    {
      TheFilename[l] = InFilename[l];
      l++;
    }
  TheFilename[l] = '\0';

  // attempt to open the file
  if ((InFile = fopen (TheFilename, "rb")) == 0)
    {
      printf ("couldn't open input file '%s'\n\n", TheFilename);
      return FALSE;
    }

  // work out what type of file it is - load the header in
  if (fread (&Begin, sizeof (Begin), 1, InFile) != 1)
    {
      printf ("couldn't read from file '%s'\n\n", TheFilename);
      return FALSE;
    }

  // check that it's a chunk file
  Recognised = TRUE;
  if (Begin.Header.ChunkFieldID != CFF_CHUNKFIELDID
      || Begin.Header.MaxChunks <= 0)
    {
      Recognised = FALSE;
    }
  else
    {
      if (strncmp (Begin.FirstEntry.ChunkID, "OBJ_", 4) == 0)
	{
	  IsALF = FALSE;
	}
      else if (strncmp (Begin.FirstEntry.ChunkID, "LIB_", 4) == 0
	       || strncmp (Begin.FirstEntry.ChunkID, "OFL_", 4) == 0)
	{
	  IsALF = TRUE;
	}
      else
	{
	  Recognised = FALSE;
	}
    }

  if (Recognised == FALSE)
    {
      printf ("File '%s' is not an AOF or ALF file - skipping\n",
	      TheFilename);
      return TRUE;
    }

  if (IsALF == FALSE)
    {
      // process AOF file
      InFileStart = 0;
      if (!ProcessAOF ())
	return FALSE;
    }
  else
    {
      if (!ProcessALF ())
	return FALSE;
    }

  return TRUE;
}

int
main (int argc, char *argv[])
{
  int l;

  printf ("extractsym v0.10  [" __DATE__ "]\n(c) Ben Summers 1995\n\n");

  // check we've got enough command line arguments and the o|e is valid
  if (argc < 4
      || ((tolower (argv[1][0]) != 'o' && tolower (argv[1][0]) != 'e')
	  || argv[1][1] != '\0'))
    {
      printf
	("bad command line syntax\nsyntax: extractsym o|e <output file> <input files> ...\no = overwrite output file, e = extend output file with new symbols\nInput files can be AOF or ALF. For ALF, specify filename within\nlibrary with <library name>,<internal filename>. The internal\nfilename can end with a * wildcard.\n\n");
      return 1;
    }

  // open the output file
  if ((OutFile = fopen (argv[2], (tolower (argv[1][0]) == 'o') ? "w" : "a"))
      == 0)
    {
      printf ("could not open output file\n\n");
      return 1;
    }

  // process the file names
  for (l = 3; l < argc; l++)
    {
      strncpy (InFilename, argv[l], 63);

      if (!ProcessFile ())
	return 1;
    }

  // close the output file
  fclose (OutFile);

  // report to the user
  printf ("%d AOF files processed, %d symbols extracted\n\n", FilesProcessed,
	  SymbolsFound);

  // and that's it for today
  return 0;
}
