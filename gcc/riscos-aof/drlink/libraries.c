/*
** Drlink AOF linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This source file contains the routines used for handling libraries
*/

#include <string.h>
#include "drlhdr.h"
#include "filehdr.h"
#include "chunkhdr.h"
#include "procdefs.h"

/* Private declarations */

#define ALFV1 1			/* Library is ALF format version 1 */
#define ALFV2 2			/* Library is ALF format version 2 */

/*
** 'loadlist' records details of members loaded from libraries and
** from which library they came from.
*/
typedef struct loadlist {
  int ldhash;			/* Hashed member/library name */
  char *ldlibname;		/* Library name */
  char *ldmemname;		/* Member name */
  struct loadlist *ldflink;	/* Next entry in list */
} loadlist;

static unsigned int
  alfversion,           	/* Version of ALF used by library */
  vsrnstart,            	/* Offset of LIB_VSRN chunk in library */
  dirystart,            	/* Offset of LIB_DIRY chunk in library */
  symtstart;            	/* Offset of OFL_SYMT chunk in library */

static int current_libhash;	/* Hash value of current library's name */

static loadlist *memberlist;	/* List of library member loaded so far */
static libentry *freeliblist;	/* List of free 'libentry' slots */

/*
** 'read_libchunks' reads the 'LIB_VSRN', 'LIB_DIRY' and 'OFL_SYMT' chunks
** from a library. It returns 'TRUE' if this worked otherwise it returns
** 'FALSE'
*/
static bool read_libchunks(libheader *lp) {
  dirybase = allocmem(dirysize);
  vsrnbase = allocmem(vsrnsize);
  symtbase = allocmem(symtsize);
  if (dirybase==NIL || vsrnbase==NIL || symtbase==NIL) {
    error("Fatal: Out of memory in 'read_libchunks' reading '%s'", lp->libname);
    return FALSE;
  }
  return read_chunk("LIB_DIRY", dirystart, dirysize, dirybase) &&
   read_chunk("LIB_VSRN", vsrnstart, vsrnsize, vsrnbase) &&
   read_chunk("OFL_SYMT", symtstart, symtsize, symtbase);
}

static void find_libchunks(libheader *lp) {
  unsigned int *base;
  base = lp->libase;
  dirybase = COERCE(COERCE(base, char *)+dirystart, unsigned int *);
  vsrnbase = COERCE(COERCE(base, char *)+vsrnstart, unsigned int *);
  symtbase = COERCE(COERCE(base, char *)+symtstart, symtentry *);
}

/*
** 'find_chunkentry' returns a pointer to the chunk header entry
** for the given chunk index
*/
static chunkindex *find_chunkentry(unsigned int index) {
  return chunkhdrbase+index;
}

/*
** 'addto_liblist' is called to add the name of the library passed
** to it to the list of libraries to be scanned later. Libraries
** read whilst processing the AOF file list and those given on
** the '-lib' parameter are handled here. Libraries that have
** not been read into memory have 'libase' set to NIL.
*/
bool addto_liblist(const char *name, unsigned int *filebase, unsigned int filesize) {
  libheader *lp;
  char *cp;
  int n;
  cp = allocmem(strlen(name)+sizeof(char));
  lp = allocmem(sizeof(libheader));
  if (cp==NIL || lp==NIL) {
    error("Fatal: Out of memory in 'addto_liblist' adding '%s'", name);
  }
  strcpy(cp, name);
  lp->libname = cp;
  lp->libase = filebase;
  lp->libextent = filesize;
  lp->libgotsyms = FALSE;
  lp->libvalid = FALSE;
  lp->libflink = NIL;
  for (n = 0; n<MAXENTRIES; n++) lp->librarysyms[n] = NIL;
  if (liblist==NIL) {
    liblist = lp;
  }
  else {
    liblast->libflink = lp;
  }
  liblast = lp;
  return TRUE;
}

/*
** 'isoldlib' scans the chunk header of a library to see if there is a
** 'LIB_VSRN' chunk. If there isn't, the the library is an 'old style'
** library (and every member in it has to be loaded). It returns 'true'
** is the library is an 'old style' one otherwise 'false'.
*/
bool isoldlib(void) {
  int n, chunkcount;
  chunkindex *cp;
  chunkhdr *chp;
  chp = COERCE(filebase, chunkhdr *);
  cp = &chp->firstentry;
  chunkcount = chp->header.numchunks;
  n = 1;
  while (n<=chunkcount && (cp->chunkclass!=LIB_XXXX || cp->chunktype!=LIB_VSRN)) {
    cp++;
    n+=1;
  }
  return n>chunkcount;
}

/*
** 'scan_libchunkhdr' checks the chunk file index of the library to
** ensure that it is okay, returning 'TRUE' if it is or 'FALSE' if
** something is wrong
*/
static bool scan_libchunkhdr(libheader *lp) {
  unsigned int n, start, size;
  chunkindex *cp;
  bool datafound;
  dirystart = vsrnstart = symtstart = 0;
  datafound = FALSE;
  cp = chunkhdrbase;
  for (n = 1; n<=chunkcount; n++) {
    start = cp->chunkoffset;
    size = cp->chunksize;
    if (start>lp->libextent || start+size>lp->libextent) {
      error("Error: Chunk file header in library '%s' is corrupt", lp->libname);
      return FALSE;
    }
    if (start!=0 && (cp->chunkclass==LIB_XXXX || cp->chunkclass==OFL_XXXX)) {
      switch (cp->chunktype) {
      case LIB_VSRN:
        vsrnstart = start;
        vsrnsize = size;
        break;
      case LIB_DIRY:
        dirystart = start;
        dirysize = size;
        break;
      case LIB_DATA:
        datafound = TRUE;
        break;
      case OFL_SYMT:
        symtstart = start;
        symtsize = size;
        break;
      case LIB_TIME:
        break;
      default:
        error("Error: Entry %d in chunk index of library '%s' is corrupt ", n, lp->libname);
        return FALSE;
      }
    }
    cp++;
  }
  if (dirystart==0 || !datafound) {
    error("Error: Library '%s' does not contain the correct 'chunk' types. Is it corrupt?", lp->libname);
    return FALSE;
  }
  else if (symtstart==0) {
    error("Error: Library '%s' does not appear to be an object code library", lp->libname);
    return FALSE;
  }
  return TRUE;
}

/*
** 'find_membername' is called to scan the LIB_DIRY chunk for the
** entry for the LIB_DATA chunk 'index', returning a pointer to
** the member name in the LIB_DIRY chunk or NIL.
** From the Acorn docs, it appears that the LIB_DIRY entries
** can be in any order, so the entire LIB_DIRY chunk has to be
** searched for the chunk index 'index'.
*/
static char *find_membername(unsigned int index) {
  typedef struct {
    unsigned int dataindex;	/* Index of LIB_DATA chunk */
    unsigned int dirylen;	/* Size of LIB_DIRY entry */
    unsigned int dirydatalen;	/* Size of data portion of LIB_DIRY entry */
    char diryname;          	/* Name of LIB_DATA entry */
  } diryentry;
  diryentry *dp, *diryend;
  dp = COERCE(dirybase, diryentry *);
  diryend = COERCE(COERCE(dp, char *)+dirysize, diryentry *);
  while (dp<diryend && index!=dp->dataindex) dp = COERCE(COERCE(dp, char *)+dp->dirylen, diryentry *);
  return (dp<diryend ? &dp->diryname : NIL);
}

/*
** 'add_libsymbol' adds an entry from the OFL_SYMT chunk to the
** library's symbol table. It returns 'TRUE' if this went okay
** otherwise it returns 'FALSE'
*/
static bool add_libsymbol(libheader *fp, unsigned int index, char *name) {
  char *memname;
  libentry *lp;
  chunkindex *hp;
  int hashval;
  memname = find_membername(index);
  if (memname==NIL) {
    error("Error: Cannot find LIB_DIRY entry for '%s' in '%s'. Is library corrupt?",
     name, fp->libname);
    memname = name;
  }
  if (freeliblist!=NIL) {	/* Allocate entry from free list */
    lp = freeliblist;
    freeliblist = freeliblist->libflink;
  }
  else if ((lp = allocmem(sizeof(libentry)))==NIL ) {
    error("Fatal: Out of memory in 'add_libsymbol' adding '%s'", fp->libname);
  }
  hashval = hash(name);
  hp = find_chunkentry(index);
  lp->libhash = hashval;
  lp->libname = name;
  lp->libmember = memname;
  lp->liboffset = hp->chunkoffset;
  lp->libsize = hp->chunksize;
  lp->libflink = fp->librarysyms[hashval&LIBEMASK];
  fp->librarysyms[hashval&LIBEMASK] = lp;
  return TRUE;
}

/*
** 'scan_libsymt' scan the 'OFL_SYMT' chunk to build the library's
** symbol table
*/
static bool scan_libsymt(libheader *lp) {
  typedef struct {
    unsigned int chunkindex;	/* Index of chunk containing entry */
    unsigned int entrysize;	/* Size of OFL_SYMT entry */
    unsigned int datasize;	/* Size of data portion of OFL_SYMT entry */
    char entryname;		/* Start of entry's name */
  } symtentry;
  symtentry *sp, *symtend;
  sp = COERCE(symtbase, symtentry *);
  symtend = COERCE(COERCE(sp, char *)+symtsize, symtentry *);
  while (sp<symtend) {
    if (sp->chunkindex>chunkcount) {
      error("Error: Chunk index in OFL_SYMT chunk in '%s' is too high (value=%d, max=%d)",
       lp->libname, sp->chunkindex, chunkcount);
      return FALSE;
    }
    if (COERCE(COERCE(sp, char *)+sp->entrysize, symtentry *)>symtend) {
      error("Error: 'OFL_SYMT' entry size in '%s' is too large", lp->libname);
      return FALSE;
    }
    if (!add_libsymbol(lp, sp->chunkindex, &sp->entryname)) return FALSE;
    sp = COERCE(COERCE(sp, char *)+sp->entrysize, symtentry *);
  }
  lp->libgotsyms = TRUE;
  return TRUE;
}

/*
** 'open_library' is called to open a library and read the chunk header
** and assorted library chunks into memory. It also scans the chunks
** and builds up the library's symbol table. It returns 'TRUE' if all
** this worked otherwise it returns 'FALSE'.
*/
bool open_library(libheader *lp) {
  chunkhdr *libstart;
  bool ok;
  libstart = COERCE(lp->libase, chunkhdr *);
  if (lp->libgotsyms) {		/* Aready read library symbol table */
    current_lib = lp;
    current_libhash = hash(lp->libname);
    current_libsyms = &lp->librarysyms;
    ok = TRUE;
    if (libstart==NIL) {	/* Reading library members from disk */
      ok = open_object(lp->libname);
    }
    return ok;
  }
  if (libstart==NIL) {		/* Library is not in memory */
    if (!open_object(lp->libname)) return FALSE;
    if (!read_libchunkhdr(lp)) return FALSE;
    if (!scan_libchunkhdr(lp)) return FALSE;
    if (!read_libchunks(lp)) return FALSE;
  }
  else {	/* Library is in memory */
    chunkhdrbase = &libstart->firstentry;
    chunkcount = libstart->header.numchunks;
    if (!scan_libchunkhdr(lp)) return FALSE;
    find_libchunks(lp);
  }
  alfversion = *vsrnbase;
  if (alfversion!=ALFV1 && alfversion!=ALFV2) {
    error("Error: '%s' is a library of a type that Drlink does not support", lp->libname);
    return FALSE;
  }
  current_lib = lp;
  current_libsyms = &lp->librarysyms;
  return scan_libsymt(lp);
}

/*
** 'close_library' is called after scanning a library to tidy up
** if necessary. If the library is not going to be scanned again,
** any memory used is freed. Also, if the library is not memory
** resident, the file is closed.
*/
void close_library(libheader *lp) {
  int n;
  libentry *lep, *temp;
  if (lp->libase==NIL) close_object();		/* Were reading library members from disk */
  if (!opt_rescan) {	/* Not scanning library again - Free memory used */
    lp->libgotsyms = FALSE;
    for (n = 0; n<MAXENTRIES; n++) {
      if (lp->librarysyms[n]!=NIL) {
        lep = lp->librarysyms[n];
        while (lep!=NIL) {	/* Free library entries */
          temp = lep->libflink;
          lep->libflink = freeliblist;
          freeliblist = lep;
          lep = temp;
        }
      }
    }
  }
}

/*
** 'free_libmem' is called to return any memory used for handling
** libraries to the heap
*/
void free_libmem(void) {
  libentry *lp, *temp;
  lp = freeliblist;
  while (lp!=NIL) {	/* Free library entries */
    temp = lp->libflink;
    freemem(lp, sizeof(libentry));
    lp = temp;
  }
}

/*
** 'add_loadlist' adds details of a library member that has been brought
** into memory to the load list
*/
static void add_loadlist(libentry *mp) {
  loadlist *lp;
  if ((lp = allocmem(sizeof(loadlist)))==NIL) error("Fatal: Out of memory in 'add_loadlist'");
  lp->ldhash = current_libhash+hash(mp->libmember);
  lp->ldmemname = mp->libmember;
  lp->ldlibname = current_lib->libname;
  lp->ldflink = memberlist;
  memberlist = lp;
}

/*
** 'load_member' is called to read a member from a library. The
** normal processing for an AOF file is carried out, and a search
** made to resolve any symbols it contains in the global symbol
** table. It returns 'TRUE' if this was carried out successfully
** otherwise it returns 'FALSE'.
*/
bool load_member(libentry *lp, filelist *inwhat, symbol *forwhat) {
  filelist *fp;
  symtentry *last_symtbase, *last_symtend;
  bool ok;
  fp = read_member(lp, inwhat, forwhat);
  ok = fp!=NIL && read_tables(fp);
  if (ok) {
    add_loadlist(lp);
    if (fp->symtries.wantedsyms!=NIL) {
      last_symtbase = current_symtbase;
      last_symtend = current_symtend;
      resolve_refs(fp);
      current_symtbase = last_symtbase;
      current_symtend = last_symtend;
    }
    lp->liboffset = 0;	/* Mark member as loaded */
  }
  return ok;
}

/*
** 'load_wholelib' is called when an 'old style' library is
** found. It validates the chunk header and, if that is okay,
** adds every member in it to the file list. The proc returns
** 'TRUE' if this worked otherwise it returns 'FALSE'
*/
bool load_wholelib(const char *libname, unsigned int filesize) {
  int n, chunkcount;
  chunkindex *cp;
  chunkhdr *chp;
  unsigned int start;
  bool got_data, got_time;
  if (opt_verbose) error("Drlink: Reading all members of old-style library '%s'", libname);
  got_data = got_time = FALSE;
  dirystart = 0;
  chp = COERCE(filebase, chunkhdr *);
  cp = &chp->firstentry;
  chunkcount = chp->header.numchunks;
  for (n = 1; n<=chunkcount; n++) {
    start = cp->chunkoffset;
    if (start>filesize || start+cp->chunksize>filesize) {
      error("Error: Chunk file header in library '%s' is corrupt", libname);
      return FALSE;
    }
    if (start!=0) {
      if (cp->chunkclass==LIB_XXXX) {
        switch (cp->chunktype) {
        case LIB_DIRY:
          dirystart = start;
          dirysize = cp->chunksize;
          break;
        case LIB_TIME:
          got_time = TRUE;
          break;
        case LIB_DATA:
          got_data = TRUE;
          break;
        default:
          error("Error: Chunk file header in Library '%s' is corrupt", libname);
          return FALSE;
        }
      }
      else if(cp->chunkclass!=OFL_XXXX) {
        error("Error: Chunk file header in Library '%s' is corrupt", libname);
        return FALSE;
      }
    }
    cp++;
  }
  if (!got_data || !got_time || dirystart==0) {
    error("Error: Chunk file header in Library '%s' is corrupt", libname);
    return FALSE;
  }
  dirybase = COERCE(COERCE(filebase, char *)+dirystart, unsigned int *);
  cp = &chp->firstentry;
  chunkcount = chp->header.numchunks;
  for (n = 1; n<=chunkcount; n++) {
    if (cp->chunktype==LIB_DATA && cp->chunkoffset!=0) {
      strcpy(objectname, find_membername(n-1));
      if (opt_verbose) error("Drlink:   Extracting '%s'...", objectname);
      if (!extract_member(cp)) return FALSE;
    }
    cp++;
  }
  got_oldlibs = TRUE;
  return TRUE;
}

/*
** 'isloaded' returns TRUE if a library member has already been
** loaded into memory by scanning the 'loaded members' list
** (Thunks... Should this code allow for case-insensitive member
** and filename matching?)
*/
bool isloaded(libentry *mp) {
  loadlist *lp;
  int hashval;
  char *cp, *np;
  lp = memberlist;
  cp = current_lib->libname;
  np = mp->libmember;
  hashval = current_libhash+hash(np);
  while (lp!=NIL && (hashval!=lp->ldhash || strcmp(np, lp->ldmemname)!=0 || strcmp(cp, lp->ldlibname)!=0)) {
    lp = lp->ldflink;
  }
  return lp!=NIL;
}

/*
** 'discard_libraries' is a 'last gasp' routine that attempts to free
** some memory by discarding any libraries that have been loaded. They
** are added to the list of libraries referenced on disk. If any memory
** was released in this way, the proc returns 'TRUE' otherwise it
** returns FALSE
*/
bool discard_libraries(void) {
  libheader *lp;
  unsigned int *place;
  bool freed;
  lp = liblist;
  freed = FALSE;
  while (lp!=NIL) {
    if ((place = lp->libase)!=NIL) {
      lp->libase = NIL;
      freed = TRUE;
      freemem(place, lp->libextent);
      if (opt_verbose) error("Drlink: Discarding library '%s' to free up memory", lp->libname);
    }
    lp = lp->libflink;
  }
  return freed;
}

/*
** 'init_library' is called to initialise the library code
*/
void init_library(void) {
  liblist = NIL;
  freeliblist = NIL;
  memberlist = NIL;
}
