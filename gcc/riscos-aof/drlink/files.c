/*
** Drlink AOF Linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This file contains all the general file manipulation routines
** and those for reading AOF files
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>

#include "drlhdr.h"
#include "chunkhdr.h"
#include "filehdr.h"
#include "edithdr.h"
#include "procdefs.h"

#ifdef TARGET_RISCOS
#include <kernel.h>
#include <swis.h>
#include <unixlib/local.h>
#endif

#ifdef TARGET_RISCOS
#define WILDCARDS "#*"		/* RISC OS filename wildcard characters */
#else
#define WILDCARDS "?*["		/* Unixish filename wildcard characters */
#endif

/* Variables referenced from other files */

chunkindex *chunkhdrbase;		/* Address of file's chunk header when in memory */

unsigned int chunkcount;		/* Count of chunks in file */

symtentry *symtbase;			/* Address of OFL_SYMT chunk when in memory */

unsigned int
  symtsize,         			/* Size of OFL_SYMT chunk */
  strtsize,         			/* Size of OBJ_STRT chunk */
  dirysize,         			/* Size of LIB_DIRY chunk */
  vsrnsize;         			/* Size of LIB_VSRN chunk */

unsigned int
  *dirybase,				/* Address of LIB_DIRY chunk when in memory */
  *vsrnbase;				/* Address of LIB_VSRN chunk when in memory */

char *strtbase;				/* Address of OBJ_STRT chunk when in memory */

image imagetype;			/* What sort of file linker will produce */

filelist
  *aofilelist,				/* Pointer to list of loaded files */
  *aofilelast;				/* Pointer to last loaded file entry */

libheader
  *liblist,				/* Pointer to list of libraries */
  *liblast;				/* Pointer to end of list of libraries */

chunkheader header;			/* Chunk header from last file read */

FILE
  *objectfile,				/* AOF or library being read */
  *imagefile,				/* Image file being produced */
  *symbolfile,				/* Symbol file being written */
  *mapfile;				/* Area map file */

char
   *symbolname,				/* Pointer to name of symbol file */
   *mapfilename;			/* Pointer to name of map file */

const char
   *imagename;				/* Pointer to name of linker output file */

bool
  object_open,				/* TRUE if an object file is being read */
  image_open,				/* TRUE if the image file is being written */
  symbol_open,				/* TRUE if the symbol file is being written */
  map_open;				/* TRUE if the area map file is being written */

char objectname[FNAMELEN];		/* File currently being read */

unsigned int
  filecount,				/* Count of AOF files loaded */
  buffersize,				/* Size of image file buffer */
  headersize,				/* Size of AIF (or BIN) header */
  debugsize,				/* Size of debugging tables in image */
  imagesize;				/* Size of image file to be written to disk */

unsigned int *filebase;			/* Pointer to a file when loaded */

linkfiles
  *linklist,				/* Pointer to list of files to link */
  *linklast;				/* Pointer to end of file list */

debugfiles *debugflist;			/* List of files where debug info is to be kept */

char
  *cmdbuffer,				/* Pointer to buffer holding command line */
  *cmdbufend,				/* Pointer to end of command line buffer */
  *cvptr,				/* Pointer to whichever buffer is being used */
  *cvptrend,				/* Pointer to end of a buffer */
  *lastcvptr;				/* Saved pointer */

/* Private declarations */

#define MINSIZE 100			/* Rough minimum size for an AOF file or library in bytes */

#ifdef TARGET_RISCOS

/* Information on RISCOS files */

#define TYPE_FILE 1			/* Directory entry type for a file */
#define TYPE_DIR 2			/* Directory entry type for a directory */
#define FILECOUNT 100			/* Number of directory entries returned per OS_GBPB call */
#define DIRENTSIZE 20			/* Characters allowed per directory entry */
#define DIRBUFSIZE (FILECOUNT*DIRENTSIZE)

#define TEXT 0xFFF			/* Number for filetype 'text' */
#define DATA 0xFFD			/* Number for filetype 'data' */
#define MODULE 0xFFA			/* Number for filetype 'module' */
#define ABSOLUTE 0xFF8			/* Number for filetype 'absolute' */
#define DEBIMAGE 0xFD3			/* Number for filetype 'debimage' */
#endif

static char
  *filebuffer,		/* General purpose buffer used by file routines */
  *filebuftop,		/* Current top of file buffer */
  *filebufend;		/* End of file buffer */

static unsigned int
  linewidth,		/* Chars printed on line in symbol file */
  addroffset;		/* Value subtracted from addresses printed in symbol file */

#ifdef TARGET_RISCOS
/*
** 'rearrange' is called to check for a file name of the form
** 'xxxx.o' and if so to change it in to 'o.xxxx'. The text of
** the name, the address of which is given by 'filename', is changed.
** The function returns 'TRUE' if this was possible, otherwise it returns
** FALSE. This function is specific to RISC OS
*/
static bool rearrange(const char *filename) {
  int len;
  char *toaddr, *fromaddr;
  len = strlen(filename);
  if (len<3) return FALSE;
  toaddr = (char *)filename+len-1;	/* Point at last char in name */
  if (*(toaddr-1)=='.' && tolower(*toaddr)=='o') {
    fromaddr = toaddr-2;
    while (fromaddr>=filename && *fromaddr!='.' && *fromaddr!=':') {
      *toaddr = *fromaddr;
      toaddr--;
      fromaddr--;
    }
    fromaddr++;		/* Now insert 'o.' */
    *fromaddr = 'o';
    *(fromaddr+1) = '.';
    return TRUE;
  }
  else {
    return FALSE;
  }
}

#endif

/*
** 'find_size' determines the size of the file whose handle is passed to it
*/
static int find_size(FILE *file) {
  int oldplace, size, result;
  oldplace = ftell(file);
  result = fseek(file, 0, SEEK_END);
  size = ftell(file);
  result = fseek(file, oldplace, SEEK_SET);
  return size;
}

/*
** 'find_filesize' is called to find the size of a file that has not
** been opened yet. -1 is returned if the file cannot be found
** otherwise its size
*/
int find_filesize(const char *name) {
  int size;
  FILE *thefile;
  thefile = fopen(name, "rb");

  if (thefile==NIL)
    size = -1;
  else {
    size = find_size(thefile);
    fclose(thefile);
  }
  return size;
}

/*
** 'find_filetype' verifies the file is a legal chunk file and
** determines its type. As a side effect, it also sets a couple
** of important variables needed when scanning the chunk file
** index (chunkhdrbase and chunkcount).
*/
static fileinfo find_filetype(void) {
  fileinfo filetype;
  chunkhdr *hp;
  hp = COERCE(filebase, chunkhdr*);
  if (hp->header.chunkfileid==BIGENDIAN) {
    error("Error: File '%s' is a 'big endian' file. Drlink does not support these", objectname);
    return NOWT;
  }
  else if (hp->header.chunkfileid!=CHUNKFILE || hp->header.maxchunks<hp->header.numchunks) {
    error("Error: File '%s' is not of the right format or is corrupt", objectname);
    return NOWT;
  }
  chunkcount = hp->header.numchunks;
  chunkhdrbase = &hp->firstentry;
  switch (chunkhdrbase->chunkclass) {
  case OBJ_XXXX:
    filetype = AOFILE;
    break;
  case LIB_XXXX: case OFL_XXXX:
    filetype = LIBRARY;
    break;
  default:
    error("Error: File '%s' does not contain object code nor is it a library", objectname);
    filetype = NOWT;
  }
  return filetype;
}

/*
** Convert endian of a given number of words
*/
#if !defined(__riscos__) && defined(WORDS_BIGENDIAN)
static void convert_endian(void *words, int size) {
  int *values = words;

  while (size-- > 0) {
    int value = *values;

    *values =  (value >> 24) |
             ((value >> 8) & 0xff00)   |
             ((value << 8) & 0xff0000) |
              (value << 24);
    values++;
  }
}
#else
#define convert_endian(words, size)
#endif

/*
** 'examine_file' is called to check that the file passed to it is an
** AOF file or ALF library. It returns the type of the file if it was
** successfully validated, or NOWT if something is wrong with it.
** This function is used when it is necessary to read the file's chunk
** header without bringing the whole file into memory
*/
fileinfo examine_file(const char *filename) {
  FILE *libfile;
  unsigned int filesize, n;
  size_t count;
  fileinfo filetype;
  chunkheader libheader;
  chunkindex index;
  libfile = fopen(filename, "rb");

  if (libfile==NIL) {
    error("Error: Cannot read '%s'", filename);
    return NOWT;
  }
  filesize = find_size(libfile);
  count = fread(&libheader, sizeof(chunkheader), 1, libfile);
  convert_endian(&libheader, sizeof(chunkheader) / 4);
  if (count!=1) {	    /* Didn't read one chunkheader */
    error("Error: Cannot read '%s'", filename);
    fclose(libfile);
    return NOWT;
  }
  if (libheader.chunkfileid==BIGENDIAN) {
    error("Error: '%s' is a 'big endian' file. Drlink does not support these", filename);
    fclose(libfile);
    return NOWT;
  }
  else if (libheader.chunkfileid!=CHUNKFILE
   || libheader.numchunks>libheader.maxchunks || filesize<MINSIZE) {
    error("Error: File '%s' is corrupt or not of a suitable type to be linked", filename);
    fclose(libfile);
    return NOWT;
  }
  count = fread(&index, sizeof(chunkindex), 1, libfile);
  convert_endian(&index, sizeof(chunkindex) / 4);
  if (count!=1) {
    error("Error: Cannot read '%s'", filename);
    fclose(libfile);
    return NOWT;
  }
  switch (index.chunkclass) {
  case OBJ_XXXX:
    filetype = AOFILE;
    break;
  case OFL_XXXX: case LIB_XXXX:
    filetype = OLDLIB;	/* Assume an old-style library */
    for (n = 1; n<=libheader.numchunks && filetype==OLDLIB; n++) {	/* Check for new style */
      if (index.chunkoffset!=0 && index.chunkclass==LIB_XXXX && index.chunktype==LIB_VSRN) {
        filetype = LIBRARY;
      }
      else {
        count = fread(&index, sizeof(chunkindex), 1, libfile);
        convert_endian(&index, sizeof(chunkheader) / 4);
        if (count!=1) {
          error("Error: Cannot read '%s'", filename);
          filetype = NOWT;
        }
      }
    }
    break;
  default: filetype = NOWT;
  }
  fclose(libfile);
  return filetype;
}

/*
** 'open_object' opens an AOF file. It returns 'TRUE' if this was
** okay otherwise it returns 'FALSE'.
*/
bool open_object(const char *filename) {
  objectfile = fopen(filename, "rb");
  object_open = objectfile!=NIL;
  if (object_open) strcpy(objectname, filename);
  return object_open;
}

/*
** 'close_object' close the object file that was just read
*/
void close_object(void) {
  fclose(objectfile);
  object_open = FALSE;
}

/*
** 'need_debug' checks to see if debug information should be kept
** or generated for the file 'name'. It returns 'TRUE' if it should
*/
static bool need_debug(const char *name, int hashval) {
  debugfiles *p;
  p = debugflist;
#ifdef IGNORE_CASE
  while (p!=NIL && (hashval!=p->dbghash || stricmp(name, p->dbgname)!=0)) p = p->dbgnext;
#else
  while (p!=NIL && (hashval!=p->dbghash || strcmp(name, p->dbgname)!=0)) p = p->dbgnext;
#endif
  if (p!=NIL) p->dbgread = TRUE;
  return p!=NIL;
}

/*
** 'addto_filelist' adds a file to the file list, returning a pointer
** to the file's entry in the file list or nil if it failed
*/
static filelist *addto_filelist(const char *fname, unsigned int fsize) {
  filelist *p;
  char *cp;
  int i, hashval;
  cp = allocmem(strlen(fname)+sizeof(char));
  p = allocmem(sizeof(filelist));
  if (p==NIL || cp==NIL) {
    error("Fatal: Out of memory in 'addto_filelist' reading '%s'", fname);
  }
  filecount+=1;
  strcpy(cp, fname);
  p->chfilename = cp;
  p->chfilehash = hashval = hash(fname);
  p->chfilesize = fsize;
  p->keepdebug = opt_debug || need_debug(fname, hashval) || imagetype==AOF;
  p->cached = TRUE;
  p->edited = FALSE;
  p->aofv3 = FALSE;
  p->objheadptr = NIL;
  p->objareaptr = NIL;
  p->objsymtptr = NIL;
  p->objstrtptr = NIL;
  p->areacount = p->symtcount = 0;
  p->nextfile = NIL;
  for (i = 0; i<MAXLOCALS; i++) p->localsyms[i] = NIL;
  p->symtries.wantedsyms = NIL;
  p->strongrefs = NIL;
  if (aofilelist==NIL) {
    aofilelist = p;
  }
  else {
    aofilelast->nextfile = p;
  }
  aofilelast = p;
  return p;
}

/*
** 'addto_debuglist' adds the file name passed to it to the list of
** files where debug information is to be kept and/or generated.
*/
void addto_debuglist(const char *name) {
  debugfiles *p;
  if ((p = allocmem(sizeof(debugfiles)))==NIL) {
    error("Fatal: Out of memory in 'addto_debuglist'");
  }
  p->dbgname = name;
  p->dbghash = hash(name);
  p->dbgread = FALSE;
  p->dbgnext = debugflist;
  debugflist = p;
}

/*
** 'check_debuglist' is called to ensure that all the files listed
** in the debug list were read and list any that were not
*/
void check_debuglist(void) {
  debugfiles *p;
  bool msg;
  msg = FALSE;
  p = debugflist;
  while (p!=NIL) {
    if (!p->dbgread) {
      if (!msg) {
        error("Warning: The following file(s) listed on '-keepdebug' were not loaded:");
        msg = TRUE;
      }
      error("    %s", p->dbgname);
    }
    p = p->dbgnext;
  }
}

/*
** 'scan_chunkhdr' scans and validates the chunk file header. It
** returns 'TRUE' if it is okay, otherwise it returns 'FALSE'.
*/
static bool scan_chunkhdr(filelist *fp) {
  unsigned int i;
  unsigned int start, size;
  chunkindex *cp;
  cp = chunkhdrbase;
  convert_endian(cp, sizeof(chunkheader) / 4);
  for (i = 1; i<=chunkcount; i++) {
    start = cp->chunkoffset;
    size = cp->chunksize;
    if (start+size>fp->chfilesize) {
      error("Error: Chunk file header entry %d in '%s' (or file) is corrupt", i, fp->chfilename);
      return FALSE;
    }
    if (start!=0 && cp->chunkclass==OBJ_XXXX) {
      switch (cp->chunktype) {
      case OBJ_HEAD:
        fp->objheadptr = COERCE(COERCE(filebase, char *)+start, objheadhdr*);
        fp->objheadsize = size;
        break;
      case OBJ_AREA:
        if (fp->objareaptr!=NIL) {
          error("Error: More than one OBJ_AREA chunk found in '%s'", fp->chfilename);
          return FALSE;
        }
        else {
          fp->objareaptr = COERCE(COERCE(filebase, char *)+start, unsigned int*);
          fp->objareasize = size;
        }
        break;
      case OBJ_SYMT:
        fp->objsymtptr = COERCE(COERCE(filebase, char *)+start, symtentry*);
        fp->objsymtsize = size;
        break;
      case OBJ_STRT:
        fp->objstrtptr = strtbase = COERCE(filebase, char *)+start;
        fp->objstrtsize = size;
        break;
      }
    }
    cp++;
  }
  if (fp->objheadptr==NIL || fp->objareaptr==NIL || fp->objsymtptr==NIL || fp->objstrtptr==NIL) {
    error("Error: AOF file '%s' appears to be either incomplete or corrupt", objectname);
    return FALSE;
  }
  return TRUE;
}

/*
** 'read_file' reads an AOF file into memory. It determines the size of
** the file and then reads the entire file into memory returning either
** the size of the file if it is read successfully or -1 if it fails.
*/
static int read_file(const char *filename) {
  int filesize;
  FILE *objfile;
  size_t count;
  if (opt_verbose) error("Drlink: Reading file '%s'", filename);
  objfile = fopen(filename, "rb");

  if (objfile==NIL) {	/* Could not open file */
    error("Error: Cannot find file '%s'", filename);
    return -1;
  }
  filesize = find_size(objfile);
  filebase = allocmem(filesize);
  if (filebase==NIL) {	/* Not enough memory */
    fclose(objfile);
    switch (examine_file(filename)) {
    case AOFILE: case OLDLIB:	/* No chance */
      error("Fatal: Not enough memory is available to load file '%s'", filename);
    case LIBRARY:
      addto_liblist(filename, NIL, filesize);
      return 0;
    default:
      return -1;
    }
  }
  count = fread(filebase, filesize, 1, objfile);
  if (count==1) {	/* File read successfully */
    strcpy(objectname, filename);
  }
  else {	/* Read failed */
    error("Error: Cannot read file '%s'", filename);
    filesize = -1;
  }
  fclose(objfile);
  return filesize;
}

/*
** 'process_file' deals with a file once it has been loaded into
** memory. If the file is an AOF file, it builds the various
** linker tables for the file. If a library, if a new-style
** library, the file is added to the library list, otherwise
** all the library members are loaded and added to the file
** list. If the 'low memory' flag is set, libraries are
** immediately unloaded from memory after adding them to the
** library list. There is probably not enough memory left to
** complete the link if this flag is set, but it only adds a
** couple of lines of code to the function to deal with it.
** The proc returns 'TRUE' if all this worked otherwise it
** returns 'FALSE'
*/
static bool process_file(const char *filename, unsigned int filesize) {
  filelist *fp;
  bool ok;

  ok = FALSE;
  switch (find_filetype()) {
  case AOFILE:
    fp = addto_filelist(filename, filesize);
    if (scan_chunkhdr(fp)) ok = read_tables(fp);
    break;
  case LIBRARY:
    if (isoldlib()) {	/* Old-style library - Load everything in it */
      fp = addto_filelist(filename, 0);	/* Create dummy entry for library in file list */
      ok = load_wholelib(filename, filesize);
    }
    else {	/* New-style library */
      addto_liblist(filename, filebase, filesize);
      ok = TRUE;
    }
    break;
  default:
    break;  
  }
  return ok;
}

/*
** 'unread' checks the the file passed to it has not already
** been loaded. It returns 'TRUE' if it has not.
*/
static bool unread(const char *filename) {
  unsigned int hashval;
  filelist *fp;
  fp = aofilelist;
  hashval = hash(filename);
#ifdef IGNORE_CASE
  while (fp!=NIL && (hashval!=fp->chfilehash || stricmp(filename, fp->chfilename)!=0)) fp = fp->nextfile;
#else
  while (fp!=NIL && (hashval!=fp->chfilehash || strcmp(filename, fp->chfilename)!=0)) fp = fp->nextfile;
#endif
  if (fp!=NIL) error("Warning: File '%s' has already been read. Ignored.", filename);
  return fp==NIL;
}

#ifdef TARGET_RISCOS
/*
** 'match_files' obtains the directory entries for the possibly
** wildcarded filename passed to it. It uses OS_GBPB call 9 to
** provide a list of matching names, returning the information
** in 'filebuffer'. It returns 0 if an error occured or if no
** files match what is given in 'filename', otherwise the
** number of entries found.
**
** Multiple calls have to be made to OS_GBPB as per the PRM
** as it says that it is possible that not every matching
** entry will be returned in one call.
*/
static int match_files(char dirname[], char leafname[]) {
  int filecount, offset, i;
  char *fp;
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  fp = filebuffer;
  offset = 0;
  filecount = 0;
  do {
    regs.r[0] = 9;	/* OS_GBPB call used */
    regs.r[1] = COERCE(&dirname[0], int);
    regs.r[2] = COERCE(fp, int);	/* Address within filebuffer at which to start */
    regs.r[3] = FILECOUNT;
    regs.r[4] = offset;
    regs.r[5] = DIRBUFSIZE;
    regs.r[6] = COERCE(&leafname[0], int);
    swierror = _kernel_swi(OS_GBPB, &regs, &regs);
    if (swierror!=NIL) {
      error("Error: Cannot read directory entry for '%s'", leafname, &swierror->errmess);
      return 0;
    }
    else {
      filecount+=regs.r[3];	/* Number of file entries returned is passed in R3 */
      offset = regs.r[4];
      if (offset>=0) {	/* More entries to come. Point fp at place to put them */
        for (i = 1; i<=regs.r[3]; i++) {
          do {
            fp++;
          } while (*fp!=NULLCHAR);
          fp++;
        }
        if (fp>filebuffer+buffersize) {	/* Oops... Run out of buffer */
          error("Fatal: Directory '%s' contains too many files for linker to handle", dirname);
        }
      }
    }
  } while (offset>=0);
  return filecount;
}

#else

/*
** This is the non OS-specific version of match_files.
*/
static int match_files(char dirname[], char leafname[]) {
  strcpy(filebuffer, leafname);
  return 1;
}
#endif

/*
** 'split_names' is passed a RISCOS path name in 'filename' and
** returns it broken down into a directory name and a leaf name
** in the arrays 'dirname' and 'leafname' respectively
*/
static void split_names(const char *filename, char dirname[], char leafname[]) {
  char *p, *leafstart, *dirstart;
  int len;
  len = strlen(filename)-1;	/* Get offset of last char */
  strcpy(dirname, filename);
  dirstart = &dirname[0];
  p = dirstart+len;
  while (p>=dirstart && *p!='.' && *p!=':') p--;
  leafstart = p+1;
  strcpy(leafname, leafstart);
  if (p<dirstart) {	/* No directory name found */
    *dirstart = NULLCHAR;
  }
  else {
    if (*p==':') p++;	/* Need ':' as part of directory name in names like wibble:eek */
    *p = NULLCHAR;
  }
}

/*
** 'wildcarded' returns TRUE if the filename pointed at by 'p'
** contains wildcards
*/
static bool wildcarded(const char *p) {
  while (*p>' ' && strchr(WILDCARDS, *p)==NIL) p++;
  return *p>' ';
}

/*
** 'get_files' is passed the name of a file to load from the command
** line. This could contains wildcards and so it has to expand this
** into the actual list of files to be read.
** If memory is running short, the routine checks the start of the
** file to see what it is. If it is a library, it does not read it
** but adds it to the library list. AOF files and old-style libraries
** are still read in.
*/
bool get_files(const char *filename) {
  int count, i, filesize;
  char *dp, *p;
  bool ok;
  char dirname[FNAMELEN];
  char leafname[LEAFLEN];

  if (!wildcarded(filename)) {	/* No wildcards. Simple call */
    ok = TRUE;
    if (unread(filename)) {
      if (low_memory) {		/* Running short on memory */
        switch(examine_file(filename)) {
        case AOFILE: case OLDLIB:
          filesize = read_file(filename);
          ok = process_file(filename, filesize);
          break;
        case LIBRARY:
          addto_liblist(filename, NIL, find_filesize(filename));
          break;
        default:
          ok = FALSE;
        }
      }
      else {	/* Plenty of memory yet... */
        filesize = read_file(filename);
        ok = filesize>=0;
        if (filesize>0) ok = process_file(filename, filesize);
      }
    }
    return ok;
  }
  else {
/*
** Wildcards found, so we have to find all the file names that
** match the wildcard specification. Separate directory name
** and leaf name and then see what there is, allowing for
** back-to-front 'xxx*.o' filenames. 'wildcarded' is used
** here as a quick check for 'xxx*' as a directory name (when
** it could be the leafname in 'xxx*.o')
*/
    split_names(filename, dirname, leafname);
    count = 0;
    if (!wildcarded(dirname)) count = match_files(dirname, leafname);
#ifdef TARGET_RISCOS
    if (count==0 && rearrange(filename)) {	/* 'xxx*.o' type filename */
      split_names(filename, dirname, leafname);
      count = match_files(dirname, leafname);
    }
#endif
    if (count==0) {
      error("Error: Cannot find '%s'", filename);
      return FALSE;
    }
    ok = TRUE;
    dp = filebuffer;
    p = &dirname[0]+strlen(dirname);
    if (p!=&dirname[0] && *(p-1)!=':') {	/* Need '.' after dir name */
      *p = '.';
      p++;
      *p = NULLCHAR;
    }
    for (i = 1; i<=count && ok; i++) {
      strcpy(p, dp);
      if (unread(dirname)) {
        if (low_memory) {		/* Running short on memory */
          switch (examine_file(dirname)) {
          case AOFILE: case OLDLIB:
            filesize = read_file(dirname);
            ok = process_file(dirname, filesize);
            break;
          case LIBRARY:
            addto_liblist(dirname, NIL, find_filesize(dirname));
            break;
          default:
            ok = FALSE;
          }
        }
        else {
          filesize = read_file(dirname);
          ok = filesize>=0;
          if (filesize>0) ok = process_file(dirname, filesize);
        }
      }
      dp+=strlen(dp)+1;
    }
    return ok;
  }
}

/*
** 'tidy_files' is called when a fatal linker error occurs to make
** sure all files are closed properly. It also deletes any half-
** completed files that were being written at the time
*/
void tidy_files(void) {
  if (object_open) fclose(objectfile);
  if (image_open) {
    fclose(imagefile);
    remove(imagename);
  }
  if (symbol_open) {
    fclose(symbolfile);
    remove(symbolname);
  }
  if (map_open) {
    fclose(mapfile);
    remove(mapfilename);
   }
}

/*
** 'load_textfile' is called to read in a text file in its entirety.
** A buffer is obtained to hold the file. Note that this space must
** not be released once the file has been processed as there will be
** pointers to file names and so forth in these buffers.
** The function returns 'TRUE' if the file was read otherwise it
** returns FALSE.
*/
static bool load_textfile(const char* filename, char **where, int *size) {
  unsigned int fsize;
  char *p;
  FILE *textfile;
  size_t count;
  textfile = fopen(filename, "r");
  if (textfile==NIL) {
    error("Error: Unable to read file '%s'", filename);
    return FALSE;
  }
  fsize = find_size(textfile);
  p = allocmem(fsize+sizeof(char));
  if (p==NIL) {
    fclose(textfile);
    error("Fatal: Out of memory reading '%s' in 'load_textfile'", filename);
  }
  count = fread(p, sizeof(char), fsize, textfile);
  fclose(textfile);
  if (count!=fsize) {
    error("Error: Unable to read file '%s'", filename);
    return FALSE;
  }
  *(p+fsize) = NULLCHAR;
  *where = p;
  *size = fsize;
  return TRUE;
}

/* ---------- 'via' file processing ---------- */

/*
** 'load_viafile' reads a 'via' file into memory and points the
** 'command line' pointers at it so that 'command line' parameters
** can be taken from the 'via' file.
*/
bool load_viafile(const char *vianame) {
  char *vp;
  int size;
  inviafile = load_textfile(vianame, &vp, &size);
  if (inviafile) {
    lastcvptr = cvptr;
    cvptr = vp;
    cvptrend = vp+size;
  }
  return inviafile;
}

/* ---------- 'edit' file processing ---------- */

/*
** 'load_editfile' loads a file of link edit commands into memory.
** It return 'TRUE' is this went okay, otherwise it returns FALSE
*/
bool load_editfile(const char *name) {
  char *ep;
  int size;
  bool ok;
  ok = load_textfile(name, &ep, &size);
  if (ok) {
    editptr = ep;
    editendptr = ep+size;
  }
  editname = name;
  return ok;
}

/* ---------- Routines to read libraries ---------- */

/*
** 'read_chunk' reads a chunk from the current file, saving it at
** 'loadaddr'. It returns 'true' if this went okay otherwise it
** returns 'false'
*/
bool read_chunk(const char *chunkid, int chunkaddr, int chunksize, void *loadaddr) {
  int result;
  result = fseek(objectfile, chunkaddr, SEEK_SET);
  if (result!=0) {
    error("Error: Could not find '%s' chunk in '%s'", chunkid, objectname);
    return FALSE;
  }
  result = fread(loadaddr, chunksize, 1, objectfile);
  if (result!=1) {
    error("Error: Could not read '%s' chunk in '%s'", chunkid, objectname);
    return FALSE;
  }
return TRUE;
}

/*
** 'read_member' is called to read a member of a library into
** memory. It returns a pointer to the member's file list entry
** if this was okay or NIL if the member could not be read or was
** not an AOF file
*/
filelist *read_member(libentry *lp, filelist *inwhat, symbol *forwhat) {
  filelist *fp;
  char *membername;
  size_t count;
  int result;
  fp = NIL;
  membername = lp->libmember;
  strcpy(objectname, membername);
  if (current_lib->libase==NIL) {	/* Library is not in memory */
    result = fseek(objectfile, lp->liboffset, SEEK_SET);
    if (result!=NIL) {
      error("Error: Cannot find library member '%s'", membername);
      return NIL;
    }
    if ((filebase = allocmem(lp->libsize))==NIL) {
      error("Fatal: Not enough memory available to load library member '%s'", membername);
    }
    count = fread(filebase, lp->libsize, 1, objectfile);
    if (count!=1) {
      error("Error: Cannot read library member '%s'", membername);
      return NIL;
    }
  }
  else {
    filebase = COERCE(COERCE(current_lib->libase, char *)+lp->liboffset, unsigned int*);
  }
  switch (find_filetype()) {
  case AOFILE:
    if (opt_verbose) {
      error("Drlink: Reading library member '%s' to resolve '%s' in '%s'",
       membername, forwhat->symtptr->symtname, inwhat->chfilename);
    }
    fp = addto_filelist(membername, lp->libsize);
    if (!scan_chunkhdr(fp)) fp = NIL;
    break;
  case LIBRARY:
    error("Error: '%s' is a library. Nested libraries are not supported", membername);
  default:
    break;  
  }
  return fp;
}

/*
** 'extract_member' is called to add a member from a library to
** the file list when the entire library is being read. It returns
** 'TRUE' if this worked, otherwise it returns 'FALSE'
*/
bool extract_member(chunkindex *cp) {
  unsigned int *oldbase;
  filelist *fp;
  bool ok;

  oldbase = filebase;
  filebase = COERCE(COERCE(filebase, char *)+cp->chunkoffset, unsigned int*);
  ok = FALSE;
  switch (find_filetype()) {
  case AOFILE:
    fp = addto_filelist(objectname, cp->chunksize);
    ok = scan_chunkhdr(fp) && read_tables(fp);
    break;
  case LIBRARY:
    error("Error: '%s' is a library. Nested libraries are not supported", objectname);
  default:
    break;
  }
  filebase = oldbase;
  return ok;
}

/*
** 'read_libchunkhdr' reads the chunk header index of the library 'lp'. It
** copies it into the general purpose file buffer 'filebuffer' to save
** allocating memory needlessly.
*/
bool read_libchunkhdr(libheader *lp) {
  unsigned int size;
  size_t count;
  count = fread(&header, sizeof(header), 1, objectfile);
  if (count!=1) {
    error("Error: Cannot read start of library '%s'", objectname);
    return FALSE;
  }
  chunkcount = header.numchunks;
  size = chunkcount*sizeof(chunkindex);
  if (size>buffersize) {
    error("Fatal: Library '%s' is too large to read", lp->libname);
  }
  chunkhdrbase = COERCE(filebuffer, chunkindex*);
  count = fread(chunkhdrbase, size, 1, objectfile);
  if (count!=1) {
    error("Error: Cannot read header from library '%s'", lp->libname);
    return FALSE;
  }
  return TRUE;
}

/* --------- Create executable image file ---------- */

/*
** 'open_image' creates the image file and then opens it so that
** its contents can be written. The file is created first so that
** an area of disk of the correct size will be allocated rather
** than the OS having to keep extending the file. Hopefully this
** speeds up writing the image file.
*/
void open_image(void) {
#ifdef TARGET_RISCOS
  char ro_name[MAXPATHLEN];
  unsigned int filetype;
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  switch(imagetype) {
  case RMOD:
    filetype = MODULE;
    break;
  case AOF: case ALF:
    filetype = DATA;
    break;
  default:
    filetype = (opt_debimage && debugsize>0 ? DEBIMAGE : ABSOLUTE);
  }

  __riscosify_std(imagename, 1, ro_name, sizeof(ro_name), NULL);

  regs.r[0] = 0x0B;	/* OS_File call to create a file */
  regs.r[1] = COERCE(ro_name, int);
  regs.r[2] = filetype;
  regs.r[4] = 0;
  regs.r[5] = imagesize;
  swierror = _kernel_swi(OS_File, &regs, &regs);
  if (swierror!=NIL) {
    error("Fatal: Cannot create image file '%s'", imagename);
  }

  imagefile = fopen(imagename, "rb+");
#else
  imagefile = fopen(imagename, "wb");
#endif
  if (imagefile==NIL) {
    error("Fatal: Unable to open image file '%s'", imagename);
  }
  image_open = TRUE;
  filebuftop = filebuffer;
  filebufend = filebuffer+buffersize;
}

/*
** 'write_block' is called to actually write stuff to the image
** file and to check the outcome of the I/O request
*/
static void write_block(void *where, int size) {
  size_t count;
  if (size!=0) {
    count = fwrite(where, size, 1, imagefile);
    if (count!=1) error("Fatal: Error occured writing '%s'", imagename);
  }
}

/*
** 'write_image' is called to write the contents of an area either
** to the staging area or the image file. The staging area is used
** to improve the efficiency of writing the image file so that
** only large chunks are written to disk instead of lots of little
** ones, as what happens with the C compiler (or Darc, for that
** matter). The routine does not return anything to say if it
** worked as the only errors are fatal ones where the linker gives
** up on the spot.
*/
void write_image(void *areaddr, unsigned int areasize) {
  if (filebuftop+areasize>=filebufend) {	/* No room in staging buffer */
    write_block(filebuffer, filebuftop-filebuffer);
    filebuftop = filebuffer;
  }
  if (areasize>=buffersize) {	/* Area too big for buffer */
    write_block(areaddr, areasize);
  }
  else {
    memcpy(filebuftop, areaddr, areasize);
    filebuftop+=areasize;
  }
}

/*
** 'write_string' is called to write character strings to the
** image file. This is only used when creating partially-linked
** AOF files
*/
void write_string(const char *p) {
  int len;
  len = strlen(p)+sizeof(char);
  if (filebuftop+len>=filebufend) {	/* No room in staging buffer */
    write_block(filebuffer, filebuftop-filebuffer);
    filebuftop = filebuffer;
  }
  strcpy(filebuftop, p);
  filebuftop+=len;
}

/*
** 'write_zeroes' writes the specified number of zeroes to the
** image file
*/
void write_zeroes(unsigned int count) {
  unsigned int zeroblock[] = {0,0,0,0,0,0,0,0};
  while (count>=sizeof(zeroblock)) {
    write_image(&zeroblock, sizeof(zeroblock));
    count-=sizeof(zeroblock);
  }
  if (count>0) write_image(&zeroblock, count);
}

#ifdef TARGET_RISCOS
/*
** 'set_filetype' sets the type of the file passed to it
** to the filetype specified
*/
static void set_filetype(char *filename, int filetype) {
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  regs.r[0] = 0x12;	/* Set file type */
  regs.r[1] = COERCE(filename, int);
  regs.r[2] = TEXT;
  swierror = _kernel_swi(OS_File, &regs, &regs);
  if (swierror!=NIL) {
    error("Fatal: Cannot set filetype of '%s'", filename);
  }
}

static void set_loadexec(void) {
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  regs.r[0] = 2;	/* Call to reset load address */
  regs.r[1] = COERCE(imagename, int);
  regs.r[2] = codebase;
  swierror = _kernel_swi(OS_File, &regs, &regs);
  if (swierror!=NIL) {
    error("Fatal: Cannot alter load address of '%s'", imagename);
  }
  regs.r[0] = 3;	/* Call to reset execution address */
  regs.r[1] = COERCE(imagename, int);
  regs.r[3] = entryarea->arplace+entryoffset;
  swierror = _kernel_swi(OS_File, &regs, &regs);
  if (swierror!=NIL) {
    error("Fatal: Cannot alter execution address of '%s'", imagename);
  }
}
#endif

/*
** 'close_image' writes the final block of data to the image file
** and then closes it. If the image file has a load address different
** to the standard address of 0x8000, the file is changed so that
** it does not have a type but a load and exec address instead.
*/
void close_image(void) {
  write_block(filebuffer, filebuftop-filebuffer);
  fclose(imagefile);
  image_open = FALSE;
#ifdef TARGET_RISCOS
  if (opt_codebase) set_loadexec();
#endif
}

/*
** 'reset_image' is called to reset the file pointer to the
** offset in the file given after flushing the file buffer.
*/
void reset_image(unsigned int offset) {
  int result;
  write_block(filebuffer, filebuftop-filebuffer);
  filebuftop = filebuffer;
  result = fseek(imagefile, offset, SEEK_SET);
  if (result!=0) {
    error("Fatal: Error occured moving to new position in '%s'", imagename);
  }
}

/*
** The next few routines handle the creation of the symbol
** listing file. The first one closes the file and sets its
** type to 'text'
*/
void close_symbol(void) {
  if (linewidth<3) fprintf(symbolfile, "\n");
  symbol_open = FALSE;
  fclose(symbolfile);
#ifdef TARGET_RISCOS
  set_filetype(symbolname, TEXT);
#endif
}

/*
** 'check_write' ensures that the last write to the symbol file worked
*/
static void check_write(void) {
  if (ferror(symbolfile)) {
    error("Fatal: Error occured writing to symbol file '%s'", symbolname);
  }
}

void open_symbol(void) {
  symbolfile = fopen(symbolname, "w");
  if (symbolfile==NIL) {
    error("Fatal: Cannot create symbol listing file '%s'", symbolname);
  }
  symbol_open = TRUE;
  linewidth = 0;
  addroffset = (imagetype==RMOD ? progbase : 0);
  if (!opt_acornmap) {
    fprintf(symbolfile, "                          Symbol Map of '%s'\n\n", imagename);
    check_write();
  }
}

#define FIELDWIDTH 26
/*
** 'write_symbol' adds one entry to the symbol listing
*/
void write_symbol(symtentry *sp) {
  int len;
  arealist *ap;
  len = strlen(sp->symtname)+8;
  if (opt_acornmap) {
    if (opt_revmap) {
      fprintf(symbolfile, "%06x  %s", sp->symtvalue-addroffset, sp->symtname);
    }
    else {
      fprintf(symbolfile, "%-25s %06x", sp->symtname, sp->symtvalue-addroffset);
    }
    check_write();
    if ((sp->symtattr & SYM_ABSVAL)==0 && sp->symtarea.areaptr!=NIL) {	/* A relocatable symbol */
      ap = sp->symtarea.areaptr;
      fprintf(symbolfile," in AREA %s from %s\n", ap->arname, ap->arfileptr->chfilename);
    } else {	/* Absolute symbol */
      fprintf(symbolfile,"\n");
    }
    check_write();
  }
  else {
    fprintf(symbolfile, "%06x %s ", sp->symtvalue-addroffset, sp->symtname);
    check_write();
    linewidth+=(len<=FIELDWIDTH ? 1 : 2);
    if (linewidth>=3) {
      fprintf(symbolfile, "\n");
      linewidth = 0;
    }
    else {
      len = FIELDWIDTH-len%FIELDWIDTH;
      if (len!=FIELDWIDTH) {		/* Pad with blanks */
        fprintf(symbolfile, "%*.*s", len, len, "                         ");
      }
    }
  }
  check_write();
}

/*
** 'open_mapfile' is called to create the area map file
*/
void open_mapfile(void) {
  mapfile = fopen(mapfilename, "w");
  if (mapfile==NIL) {
    error("Fatal: Cannot create area map file '%s'", mapfilename);
  }
  map_open = TRUE;
}

/*
** 'write_mapfile' is called to write a line to the area map file
** and to ensure the I/O was clean
*/
void write_mapfile(const char *text) {
  size_t count;
  count = fwrite(text, sizeof(char), strlen(text), mapfile);
  if (count!=strlen(text)) {
    error("Fatal: Error occured writing to area map file '%s'", mapfilename);
  }
}

void close_mapfile(void) {
  fclose(mapfile);
  map_open = FALSE;
#ifdef TARGET_RISCOS
  set_filetype(mapfilename, TEXT);
#endif
}

/*
** 'alloc_filebuffer' is called to allocate the memory used to cache the
** image file, hold directory entries and so on
*/
void alloc_filebuffer(void) {
  if (buffersize<MINBUFFER) buffersize = MINBUFFER;
  if ((filebuffer = allocmem(buffersize))==NIL) {
    error("Fatal: Out of memory in 'alloc_filebuffer'");
  }
}

/*
** 'resize_filebuffer' is called when memory is running short to reduce
** the size of the buffer to its minimum size, 8K. It is normally 64K
** bytes in size.
*/
void resize_filebuffer(void) {
  if (filebuffer==NIL) {	/* Not allocated buffer yet */
    buffersize = MINBUFFER;
  }
  else if (buffersize>MINBUFFER) {
    freemem(filebuffer, buffersize);
    filebuffer = allocmem(MINBUFFER);
    buffersize = MINBUFFER;
  }
}

/*
** 'init_files' initialises everything to do with files
*/
void init_files(void) {
  imagetype = NOTYPE;
  aofilelist = NIL;
  liblist = NIL;
  debugflist = NIL;
  object_open = image_open = symbol_open = map_open = FALSE;
  headersize = debugsize = imagesize = 0;
  imagename = NIL;
  filecount = 0;
  buffersize = STDBUFFER;
}
