/*
** Drlink AOF Linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** This file contains all the general file manipulation routines
** and those for reading AOF files
*/

#include "config.h"

#ifndef CROSS_COMPILE
# define __UNIXLIB_INTERNALS
# include <kernel.h>
# include <swis.h>
# include <unixlib/local.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "drlhdr.h"
#include "chunkhdr.h"
#include "edithdr.h"
#include "filehdr.h"
#include "libraries.h"
#include "procdefs.h"

#ifndef CROSS_COMPILE
# define WILDCARDS "#*"		/* RISC OS filename wildcard characters */
#else
# define WILDCARDS "?*["	/* Unixish filename wildcard characters */
#endif

/* Variables referenced from other files */

image imagetype;		/* What sort of file linker will produce */

filelist *aofilelist,		/* Pointer to list of loaded files */
 *aofilelast;			/* Pointer to last loaded file entry */

libheader *liblist,		/* Pointer to list of libraries */
 *liblast;			/* Pointer to end of list of libraries */

FILE *imagefile,		/* Image file being produced */
 *symbolfile,			/* Symbol file being written */
 *mapfile;			/* Area map file */

char *symbolname,		/* Pointer to name of symbol file */
 *mapfilename;			/* Pointer to name of map file */

const char *imagename;		/* Pointer to name of linker output file */

const char *objectname;		/* AOF/ALF file currently being read */

unsigned int buffersize,	/* Size of image file buffer */
  headersize,			/* Size of AIF (or BIN) header */
  debugsize,			/* Size of debugging tables in image */
  imagesize;			/* Size of image file to be written to disk */

linkfiles *linklist,		/* Pointer to list of files to link */
 *linklast;			/* Pointer to end of file list */

debugfiles *debugflist;		/* List of files where debug info is to be kept */

char *cmdbuffer,		/* Pointer to buffer holding command line */
 *cmdbufend,			/* Pointer to end of command line buffer */
 *cvptr,			/* Pointer to whichever buffer is being used */
 *cvptrend,			/* Pointer to end of a buffer */
 *lastcvptr;			/* Saved pointer */

/* Private declarations */

#define MINSIZE 100		/* Rough minimum size for an AOF file or library in bytes */

#ifndef CROSS_COMPILE
/* Information on RISC OS files */

# define TYPE_FILE 1		/* Directory entry type for a file */
# define TYPE_DIR 2		/* Directory entry type for a directory */
# define FILECOUNT 100		/* Number of directory entries returned per OS_GBPB call */
# define DIRENTSIZE 20		/* Characters allowed per directory entry */
# define DIRBUFSIZE (FILECOUNT*DIRENTSIZE)

# define TEXT 0xFFF		/* Number for filetype 'text' */
# define DATA 0xFFD		/* Number for filetype 'data' */
# define MODULE 0xFFA		/* Number for filetype 'module' */
# define ABSOLUTE 0xFF8		/* Number for filetype 'absolute' */
# define DEBIMAGE 0xFD3		/* Number for filetype 'debimage' */
#endif

static char *filebuffer,	/* General purpose buffer used by file routines */
 *filebuftop,			/* Current top of file buffer */
 *filebufend;			/* End of file buffer */

static unsigned int linewidth,  /* Chars printed on line in symbol file */
  addroffset;			/* Value subtracted from addresses printed in symbol file */

#ifndef CROSS_COMPILE
static bool rearrange (const char *filename);
static void set_filetype (const char *filename, int filetype);
static void set_loadexec (const char *filename);
#endif
static int find_size (FILE * file);
static bool need_debug (const char *name, int hashval);

static bool obj_check_and_adjust_head (const char *filename, const obj_overview * objoverview);
static bool obj_check_and_adjust_area (const char *filename, const obj_overview * objoverview);
static bool obj_check_and_adjust_symt (const char *filename, const obj_overview * objoverview);

static bool read_file_and_process_wrap (const char *filename);
static bool read_file_and_process (const char *filename);
static bool read_file_and_process_int (void *filebase, int filesize, const char *filename, FILE *objfile);

static bool unread (const char *filename);
static int match_files (const char dirname[], const char leafname[]);
static void split_names (const char *filename, char dirname[], char leafname[]);
static bool wildcarded (const char *p);
static bool load_textfile (const char *filename, char **where, int *size);
static void write_block (void *where, int size);
static void check_write (void);

#ifndef CROSS_COMPILE
/*
** 'rearrange' is called to check for a file name of the form
** 'xxxx.o' and if so to change it in to 'o.xxxx'. The text of
** the name, the address of which is given by 'filename', is changed.
** The function returns 'TRUE' if this was possible, otherwise it returns
** FALSE. This function is specific to RISC OS
*/
static bool
rearrange (const char *filename)
{
  int len;
  char *toaddr, *fromaddr;
  len = strlen (filename);
  if (len < 3)
    return FALSE;
  toaddr = (char *) filename + len - 1; /* Point at last char in name */
  if (*(toaddr - 1) == '.' && tolower (*toaddr) == 'o')
    {
      fromaddr = toaddr - 2;
      while (fromaddr >= filename && *fromaddr != '.' && *fromaddr != ':')
	{
	  *toaddr = *fromaddr;
	  toaddr--;
	  fromaddr--;
	}
      *++fromaddr = 'o';	/* Now insert 'o.' */
      *++fromaddr = '.';
      return TRUE;
    }

  return FALSE;
}

FILE *fopen_wrapper(const char *name, const char *mode)
{
  FILE *handle = fopen (name, mode);

  if (handle == NULL)
    {
      __set_riscosify_control(__get_riscosify_control () | __RISCOSIFY_NO_SUFFIX);
      handle = fopen (name, mode);
     __set_riscosify_control(__get_riscosify_control () & ~__RISCOSIFY_NO_SUFFIX);
    }
  return handle;  
}

#define fopen fopen_wrapper

#endif

/*
** 'find_size' determines the size of the file whose handle is passed to it
*/
static int
find_size (FILE * file)
{
  int oldplace, size;

  oldplace = ftell (file);
  if (fseek (file, 0, SEEK_END))
    return -1;
  size = ftell (file);
  if (fseek (file, oldplace, SEEK_SET))
    return -1;

  return size;
}

/*
** 'find_filesize' is called to find the size of a file that has not
** been opened yet. -1 is returned if the file cannot be found
** otherwise its size
*/
int
find_filesize (const char *name)
{
  int size;
  FILE *thefile;

  if ((thefile = fopen (name, "rb")) == NULL)
    return -1;

  size = find_size (thefile);
  fclose (thefile);

  return size;
}

/*
** Convert endian of a given number of words
*/
#ifdef WORDS_BIGENDIAN
void
convert_endian (void *words, size_t size)
{
  unsigned int *values = words;

  while (size-- > 0)
    {
      unsigned int value = *values;
      *values++ =
	(value >> 24) | ((value >> 8) & 0xff00) | ((value << 8) & 0xff0000) |
	(value << 24);
    }
}
#endif

/*
** 'examine_file' is called to check that the file passed to it is an
** AOF file or ALF library.  In the later case it determines it the
** ALF librariy is old-style (OLDLIB) or new-style (LIBRARY).
** It returns the type of the file if it was
** successfully validated, or NOWT if something is wrong with it.
** This function is used when it is necessary to read the file's chunk
** header without bringing the whole file into memory
*/
fileinfo
examine_file (const char *filename)
{
  FILE *objectfile;
  int filesize;
  unsigned int n;
  size_t count;
  fileinfo filetype;
  chunkheader ch;
  chunkindex ci;
  bool cont;

  if ((objectfile = fopen (filename, "rb")) == NULL)
    {
      error ("Error: Cannot read '%s'", filename);
      return NOWT;
    }
  if ((filesize = find_size (objectfile)) < 0)
    {
      error ("Error: Unable to determine length of '%s'", filename);
      return NOWT;
    }
  /* Read chunkheader + 1 chunk */
  count = fread (&ch, sizeof (ch), 1, objectfile);
  if (count != 1)
    {	/* Didn't read one chunkheader + 1 chunk */
      error ("Error: Cannot read '%s'", filename);
      fclose (objectfile);
      return NOWT;
    }

  if (!check_and_adjust_chunkheader (&ch, filename, (unsigned int)filesize))
    {
      fclose (objectfile);
      return NOWT;
    }

  filetype = NOWT;
  for (n = 0, cont = TRUE; n < ch.maxchunks && cont; ++n)
    {
      count = fread (&ci, sizeof (ci), 1, objectfile);
      if (count != 1)
	{
	  error ("Error: Cannot read '%s'", filename);
	  break;
	}

      convert_endian(&ci, sizeof(ci)/4);
      switch (ci.chunkclass)
        {
          case OBJ_XXXX:
            filetype = AOFILE;
            cont = FALSE;
            break;

          case OFL_XXXX:
          case LIB_XXXX:
	    if (ci.chunkoffset != 0
	        && ci.chunkclass == LIB_XXXX
	        && ci.chunktype == LIB_VSRN)
	      {
	        filetype = LIBRARY; /* New-style library */
	        cont = TRUE;
	      }
	    else
              filetype = OLDLIB;	/* Assume an old-style library */
	    break;
	}
    }
  if (filetype == NOWT)
    error ("Error: Cannot determine type of '%s'", filename);

  fclose (objectfile);
  return filetype;
}

/*
** 'need_debug' checks to see if debug information should be kept
** or generated for the file 'name'. It returns 'TRUE' if it should
*/
static bool
need_debug (const char *name, int hashval)
{
  debugfiles *p;
  p = debugflist;
#ifdef IGNORE_CASE
  while (p != NULL
	 && (hashval != p->dbghash || stricmp (name, p->dbgname) != 0))
    p = p->dbgnext;
#else
  while (p != NULL
	 && (hashval != p->dbghash || strcmp (name, p->dbgname) != 0))
    p = p->dbgnext;
#endif
  if (p != NULL)
    p->dbgread = TRUE;
  return p != NULL;
}

/*
** 'create_filelist' returns a pointer
** to a new file's entry in the file list or NULL if it failed
*/
filelist *
create_filelist (const char *fname, unsigned int fsize)
{
  filelist *p;
  char *cp;
  int i, hashval;

  cp = allocmem (strlen (fname) + sizeof (char));
  p = allocmem (sizeof (filelist));
  if (p == NULL || cp == NULL)
    error ("Fatal: Out of memory in 'create_filelist' reading '%s'", fname);

  strcpy (cp, fname);
  p->chfilename = cp;
  p->chfilehash = hashval = hash (fname);
  p->chfilesize = fsize;
  p->keepdebug = opt_debug || imagetype == AOF || need_debug (fname, hashval);
  p->aofv3 = FALSE;

  memset(&p->obj, 0, sizeof(obj_overview));

  p->areacount = p->symtcount = 0;
  p->nextfile = NULL;
  for (i = 0; i < MAXLOCALS; i++)
    p->localsyms[i] = NULL;
  p->symtries.wantedsyms = NULL;
  p->strongrefs = NULL;

  return p;
}

/*
** 'addto_filelist' adds a file to the front of file list.
*/
void
addto_filelist(filelist *fp)
{
  if (aofilelist == NULL)
    aofilelist = fp;
  else
    aofilelast->nextfile = fp;
  aofilelast = fp;
}


/*
** 'free_filelist' free the given filelist strucutre.
*/
void
free_filelist (filelist * fp)
{
  if (fp == NULL)
    return;
  freemem ((void *)fp->chfilename, strlen(fp->chfilename) + 1);
  freemem (fp, sizeof (filelist));
}


/*
** 'addto_debuglist' adds the file name passed to it to the list of
** files where debug information is to be kept and/or generated.
*/
void
addto_debuglist (const char *name)
{
  debugfiles *p;
  if ((p = allocmem (sizeof (debugfiles))) == NULL)
    error ("Fatal: Out of memory in 'addto_debuglist'");
  p->dbgname = name;
  p->dbghash = hash (name);
  p->dbgread = FALSE;
  p->dbgnext = debugflist;
  debugflist = p;
}

/*
** 'check_debuglist' is called to ensure that all the files listed
** in the debug list were read and list any that were not
*/
void
check_debuglist (void)
{
  debugfiles *p;
  bool msg = FALSE;
  for (p = debugflist; p != NULL; p = p->dbgnext)
    {
      if (!p->dbgread)
	{
	  if (!msg)
	    {
	      error ("Warning: The following file(s) listed on '-keepdebug' were not loaded:");
	      msg = TRUE;
	    }
	  error ("    %s", p->dbgname);
	}
    }
}


/*
** 'obj_check_and_adjust_head' checks the OBJ_HEAD chunk and (if needed)
** adjusts the OBJ_HEAD data for endian differences.  This routine may
** only be called once on the same chunk data.
** Returns TRUE if everything is ok, FALSE otherwise.
*/
/* FIXME: add additional OBJ_HEAD checks ! */
static bool
obj_check_and_adjust_head (const char *filename, const obj_overview * objoverview)
{
  objheadhdr *objheadptr = objoverview->headptr;
  unsigned int objheadsize = objoverview->headsize;

  convert_endian (objheadptr, sizeof (aofheader) / 4);
  if (objheadsize !=
      sizeof (aofheader) +
      objheadptr->areaheader.numareas * sizeof (areaentry))
    {
      error ("AOF file '%s' OBJ_HEAD size doesn't match with its declaration (0x%x vs 0x%x)",
	     filename, objheadsize,
	     sizeof (aofheader) +  objheadptr->areaheader.numareas * sizeof (areaentry));
      return FALSE;
    }
  convert_endian (&objheadptr->firstarea,
		  objheadptr->areaheader.numareas * sizeof (areaentry) / 4);

  return TRUE;
}


/*
** 'obj_check_and_adjust_area' checks the OBJ_AREA chunk and (if needed)
** adjusts the OBJ_AREA data for endian differences.  This routine may
** only be called once on the same chunk data.
** Returns TRUE if everything is ok, FALSE otherwise.
*/
static bool
obj_check_and_adjust_area (const char *filename, const obj_overview * objoverview)
{
  filename = filename;
  objoverview = objoverview;
  convert_endian (objoverview->areaptr, objoverview->areasize / 4);

  return TRUE;
}


/*
** 'obj_check_and_adjust_area' checks the OBJ_SYMT chunk and (if needed)
** adjusts the OBJ_SYMT data for endian differences.  This routine may
** only be called once on the same chunk data.
** Returns TRUE if everything is ok, FALSE otherwise.
*/
/* FIXME: more checks are possible ! */
static bool
obj_check_and_adjust_symt (const char *filename, const obj_overview * objoverview)
{
  unsigned int objsymtsize = objoverview->symtsize;

  if (objsymtsize !=
      objoverview->headptr->areaheader.numsymbols * sizeof (objsymt))
    {
      error ("AOF file '%s' OBJ_SYMT size doesn't match with its declaration (0x%x vs 0x%x)",
	     filename, objsymtsize,
	     objoverview->headptr->areaheader.numsymbols * sizeof (objsymt));
      return FALSE;
    }
  convert_endian (objoverview->symtptr2, objsymtsize / 4);

  return TRUE;
}


/*
** 'obj_check_and_adjust()' checks the OBJ_HEAD, OBJ_AREA and OBJ_SYMT
** chunks of an AOF file.
** ch->header should already be verified for correctness and adjusted
** for endian differences.  This routine may only be called once on
** the same AOF data.
** Returns TRUE if everything is ok, FALSE otherwise.  The obj_overview
** structure will be filled in with chunk information found.
*/
bool
obj_check_and_adjust (chunkhdr * ch, const char *filename, unsigned int filesize, obj_overview *objoverviewp)
{
  chunkindex *curchunk;
  unsigned int maxchunks, numchunks;
  unsigned int chindex;
  unsigned int symindex;

  maxchunks = ch->header.maxchunks;
  numchunks = ch->header.numchunks;
  /* Quick scan to pickup position of the chunks.  */
  memset (objoverviewp, 0, sizeof (obj_overview));
  for (chindex = 1, curchunk = &ch->firstentry; chindex <= maxchunks;
       ++chindex, ++curchunk)
    {
      unsigned int chunkoffset, chunksize;
      convert_endian (curchunk, sizeof (chunkindex) / 4);
      if ((chunkoffset = curchunk->chunkoffset) == 0)
	continue;
      chunksize = curchunk->chunksize;
      if (chunkoffset % 4)
	{
	  error ("Error: AOF file '%s' chunk index %d has non 4 divisable chunk offset (0x%x)",
	         filename, chindex, chunkoffset);
	  return FALSE;
	}
      /* Adding chunkoffset and chunksize can overflow so first check
         their value individually.  */
      if (chunkoffset > filesize || chunksize > filesize
	  || chunkoffset + chunksize > filesize)
	{
	  error ("Error: AOF file '%s' chunk index %d has bogus offset and/or size values (0x%x, 0x%x, 0x%x)",
	         filename, chindex, chunkoffset, chunksize, filesize);
	  return FALSE;
	}
      if (chunkoffset < maxchunks * sizeof (chunkindex))
	{
	  error ("Error: AOF file '%s' chunk index %d interfers with chunk index table (offset is 0x%x)",
	         filename, chindex, chunkoffset);
	  return FALSE;
	}

      if (curchunk->chunkclass == OBJ_XXXX)
	{
	  switch (curchunk->chunktype)
	    {
	      case OBJ_HEAD:
	        if (objoverviewp->headptr != NULL)
		  {
		    error ("Error: AOF file '%s' got more than one OBJ_HEAD chunk",
		           filename);
		    return FALSE;
		  }
	        if (chunksize < sizeof (aofheader) || (chunksize % 4))
		  {
		    error ("Error: AOF file '%s' has unexpected OBJ_HEAD size (0x%x)",
		           filename, chunksize);
		    return FALSE;
		  }
	        objoverviewp->headptr = COERCE (COERCE (ch, char *) + chunkoffset, objheadhdr *);
	        objoverviewp->headsize = chunksize;
	        break;

	      case OBJ_AREA:
	        if (objoverviewp->areaptr != NULL)
		  {
		    error ("Error: AOF file '%s' got more than one OBJ_AREA chunk",
		           filename);
		    return FALSE;
		  }
	        /* FIXME: is this a correct test on chunksize ? */
	        if (chunksize % 4)
		  {
		    error ("Error: AOF file '%s' chunk index %d has non 4 divisable chunk size for OBJ_AREA",
		           filename, chindex);
		    return FALSE;
		  }
	        objoverviewp->areaptr = COERCE (COERCE (ch, char *) + chunkoffset, unsigned int *);
	        objoverviewp->areasize = chunksize;
	        break;

	      case OBJ_SYMT:
	        if (objoverviewp->symtptr2 != NULL)
		  {
		    error ("Error: AOF file '%s' got more than one OBJ_SYMT chunk",
		           filename);
		    return FALSE;
		  }
	        objoverviewp->symtptr2 = COERCE (COERCE (ch, char *) + chunkoffset, objsymt *);
	        objoverviewp->symtsize = chunksize;
	        break;
	      case OBJ_STRT:
	        if (objoverviewp->strtptr != NULL)
		  {
		    error ("Error: AOF file '%s' got more than one OBJ_STRT chunk",
		           filename);
		    return FALSE;
		  }
	        objoverviewp->strtptr = COERCE (ch, char *) + chunkoffset;
	        objoverviewp->strtsize = chunksize;
	        break;
	    }
	}
    }

  /* Check if the minimal set of OBJ_XXXX chunks are present.  */
  if (objoverviewp->headptr == NULL)
    {
      error ("Error: AOF file '%s' does not have an OBJ_HEAD chunk",
	     filename);
      return FALSE;
    }
  if (objoverviewp->areaptr == NULL)
    {
      error ("Error: AOF file '%s' does not have an OBJ_AREA chunk",
	     filename);
      return FALSE;
    }

  /* Check & adjust OBJ_HEAD and OBJ_AREA chunks.  */
  if (!obj_check_and_adjust_head (filename, objoverviewp)
      || !obj_check_and_adjust_area (filename, objoverviewp))
    return FALSE;

  /* Check OBJ_SYMT thoroughly.  */
  if (objoverviewp->symtptr2 != NULL)
    {
      if (objoverviewp->symtsize !=
	  objoverviewp->headptr->areaheader.numsymbols * sizeof (objsymt))
	{
	  error ("Error: AOF file '%s' OBJ_SYMT chunk has unexpected size (0x%x vs 0x%x)",
	         filename,
	         objoverviewp->headptr->areaheader.numsymbols * sizeof (objsymt),
	         curchunk->chunksize);
	  return FALSE;
	}
      if (!obj_check_and_adjust_symt (filename, objoverviewp))
	return FALSE;
    }

  /* Alloc room for arealist ptrs, one per area */
  if ((objoverviewp->arealistptrs = allocmem (objoverviewp->headptr->areaheader.numareas * sizeof (arealist *))) == NULL)
    return FALSE;
  /* Alloc room for symbols */
  if ((objoverviewp->symtptr = allocmem (objoverviewp->headptr->areaheader.numsymbols * sizeof (symtentry))) == NULL)
    return FALSE;
  for (symindex = 0; symindex < objoverviewp->headptr->areaheader.numsymbols; ++symindex)
    {
      /* Sanity check symbol name. */
      unsigned int symnameoffset = objoverviewp->symtptr2[symindex].name;
      if (symnameoffset >= objoverviewp->strtsize)
        {
	  error ("Error: AOF file '%s' contains bad symbol name in OBJ_SYMT chunk (0x%x > 0x%x)",
	         filename, symnameoffset, objoverviewp->strtsize);
	  return FALSE;
	}
      objoverviewp->symtptr[symindex].symtname = &objoverviewp->strtptr[symnameoffset];
      objoverviewp->symtptr[symindex].symtattr = objoverviewp->symtptr2[symindex].attr;
      objoverviewp->symtptr[symindex].symtvalue = objoverviewp->symtptr2[symindex].value;
      objoverviewp->symtptr[symindex].symtarea.areaname = objoverviewp->symtptr2[symindex].areaname;
    }

  return TRUE;
}

/*
** 'check_and_adjust_chunkheader' checks and adjust for endian differences
** chunkheader at 'ch'.
** Needs at least sizeof(chunkheader) bytes and may only be called once
** for the same chunkheader.
** Returns TRUE for ok, FALSE for error
*/
bool
check_and_adjust_chunkheader (chunkheader *ch, const char *filename, unsigned int filesize)
{
  if (filesize < sizeof (chunkheader))
    {
      error ("Error: File '%s' is too short", filename);
      return FALSE;
    }
  convert_endian (ch, sizeof (chunkheader) / 4);
  switch (ch->chunkfileid)
    {
      case CHUNKFILE:
        if (ch->maxchunks < ch->numchunks)
	  {
	    error ("Error: File '%s' is corrupt", filename);
	    return FALSE;
	  }
        /* We expect at least one chunk.  */
        if (ch->numchunks == 0)
	  {
	    error ("Error: File '%s' doesn't contain any chunk", filename);
	    return FALSE;
	  }
        /* Make sure the chunk header + all chunks are in the file.  */
        if (filesize < sizeof (chunkheader) + ch->maxchunks * sizeof (chunkindex))
	  {
	    error ("Error: File '%s' is too short to contain all chunk information", filename);
	    return FALSE;
	  }
        break;

      case BIGENDIAN:
        error ("Error: File '%s' is a 'big endian' file. Drlink does not support this",
	       filename);
        return FALSE;

      default:
        error ("Error: File '%s' has an unrecognised file format", filename);
        return FALSE;
    }

  return TRUE;
}


/*
** 'check_and_get_chunkclass_before_adjust' determine chunkclass and gives
** an error when non can be determined.  The chunkheader at ch already
** needs to be verified and adjusted for endian differences and all
** chunkindexes should be in memory.
** This routine can be called more than once and does not change any
** chunk data at all and can only work on the original chunk data
** (i.e. no endian changes done).  If endian adjusting already has
** been done use check_and_get_chunkclass() instead.
** isoldlib: can be NULL but when it isn't, the bool value will be updated
** with the status that it is an 'old style' library (i.e. no LIB_VSRN chunk)
** or not.
*/
unsigned int
check_and_get_chunkclass_before_adjust (const chunkhdr *ch, const char *filename, bool *isoldlibP)
{
  chunkindex ci;
  const chunkindex *cip;
  unsigned int i;
  unsigned int chunkclass;
  bool isoldlib;

  chunkclass = 0;
  isoldlib = TRUE;
  for (i = 0, cip = &ch->firstentry; i < ch->header.maxchunks; ++i, ++cip)
    {
      ci = *cip;
      convert_endian (&ci, sizeof (chunkindex) / 4);
      if (ci.chunkoffset != 0)
	{
	  /* Valid & used chunk */
	  if (chunkclass == 0
	      && (ci.chunkclass == OBJ_XXXX
		  || ci.chunkclass == LIB_XXXX
		  || ci.chunkclass == OFL_XXXX))
	    chunkclass = ci.chunkclass;
	  if (ci.chunkclass == LIB_XXXX && ci.chunktype == LIB_VSRN)
	    isoldlib = FALSE;
	}
    }
  if (chunkclass == 0)
    error ("Error: File '%s' does not contain object code nor is it a library", filename);

  if (isoldlibP != NULL)
    *isoldlibP = (chunkclass != LIB_XXXX) ? FALSE : isoldlib;

  return chunkclass;
}


/*
** 'check_and_get_chunkclass' determine chunkclass and gives
** an error when non can be determined.  The chunkheader at ch already
** needs to be verified.  The chunkheader and all chunkindexes
** should have been adjusted for endian differences and all
** chunkindexes should be in memory (if not, you might need to use
** check_and_get_chunkclass_before_adjust() instead).
** This routine can be called more than once and does not change any
** chunk data at all.
*/
unsigned int
check_and_get_chunkclass (const chunkhdr *ch, const char *filename)
{
  const chunkindex *cip;
  unsigned int i;

  for (i = 0, cip = &ch->firstentry; i < ch->header.maxchunks; ++i, ++cip)
    {
      if (cip->chunkoffset != 0
	  && (cip->chunkclass == OBJ_XXXX
	      || cip->chunkclass == LIB_XXXX
	      || cip->chunkclass == OFL_XXXX))
	break;
    }
  if (i == ch->header.maxchunks)
    {
      error ("Error: File '%s' does not contain object code nor is it a library", filename);
      return 0;
    }

  return cip->chunkclass;
}


/*
** 'read_file_and_process_wrap' sets/unsets the global 'objectname'
** variable before/after a 'read_file_and_process' call.
*/
static bool
read_file_and_process_wrap (const char *filename)
{
  bool result;

  if (objectname != NULL)
    {
      error ("Error: Recursive read_file_and_process() calls are not supported");
      return FALSE;
    }
  objectname = filename;
  result = read_file_and_process (filename);
  objectname = NULL;
  return result;
}


/*
** 'read_file_and_process' reads an AOF/ALF file into memory. It determines the size of
** the file and then reads the entire file into memory returning either
** the size of the file if it is read successfully or -1 if it fails.
*/
static bool
read_file_and_process (const char *filename)
{
  bool ok;
  FILE *objfile;
  int filesize;
  void *filebase;

  if (!unread (filename))
    return TRUE;

  if (opt_verbose)
    error ("Drlink: Reading file '%s'", filename);

  if ((objfile = fopen (filename, "rb")) == NULL)
    {	/* Could not open file */
      error ("Error: Cannot find file '%s'", filename);
      return FALSE;
    }

  if ((filesize = find_size (objfile)) < 0)
    {
      error ("Error: Cannot determine length of file '%s'", filename);
      return FALSE;
    }
  filebase = allocmem (filesize);

  if (low_memory || filebase == NULL)
    {	/* Running short on memory */
      switch (examine_file (filename))
	{
	  case AOFILE:
	  case OLDLIB:
	    ok = read_file_and_process_int (filebase, (unsigned int)filesize, filename, objfile);
	    break;
	  case LIBRARY:
	    ok = addto_liblist (filename, NULL, (unsigned int)filesize);
	    break;
	  default:
	    ok = FALSE;
	    break;
	}
    }
  else
    {
      if ((ok = read_file_and_process_int (filebase, (unsigned int)filesize, filename, objfile)) == FALSE)
        freemem(filebase, filesize);
    }

  fclose (objfile);
  return ok;
}


/*
** 'read_file_and_process_int' reads and processes AOF/ALF files
** when they can be stored fully in memory.
** filebase can be NULL (no more free memory condition and this will
** be checked for) or non NULL.
** filesize is the filesize length of filename with handle objfile.
*/
static bool
read_file_and_process_int (void *filebase, int filesize, const char *filename, FILE *objfile)
{
  size_t count;
  bool ok, isoldlib;

  if (filebase == NULL)
    {				/* Not enough memory */
      error ("Fatal: Not enough memory is available to load file '%s'",
	     filename);
      return FALSE;
    }

  count = fread (filebase, filesize, 1, objfile);
  if (count != 1)
    {				/* Read failed */
      error ("Error: Cannot read file '%s'", filename);
      return FALSE;
    }

  if (!check_and_adjust_chunkheader (filebase, filename, filesize))
    return FALSE;

  /*
  ** If the file is an AOF file, it builds the various
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

  switch (check_and_get_chunkclass_before_adjust (filebase, filename, &isoldlib))
    {
      case OBJ_XXXX:
	{
	  filelist *fp;

	  ok = (fp = create_filelist (filename, filesize)) != NULL
	       && obj_check_and_adjust (filebase, filename, filesize, &fp->obj)
	       && read_tables (fp);
	  if (ok)
	    addto_filelist(fp);
	  else
	    free_filelist(fp);
	  break;
	}

      case LIB_XXXX:
      case OFL_XXXX:
	if (isoldlib)
	  {	/* Old-style library - Load everything in it */
	    filelist *fp;
	    /* Create dummy entry for library in file list with
	     * filesize = 0 (!)
	     */
	    if ((fp = create_filelist(filename, 0)) == NULL)
	      return FALSE;
	    addto_filelist (fp);
	    if ((ok = load_wholelib (filename, filebase, filesize)) == FALSE)
	      free_filelist(fp);
	  }
	else
	  {	/* New-style library */
	    lib_overview liboverview;
	    ok = lib_check_and_adjust (filebase, filename, filesize, &liboverview)
		 && addto_liblist (filename, &liboverview, filesize);
	  }
	break;

      default:
	ok = FALSE;
	break;
    }

  return ok;
}

/*
** 'unread' checks the the file passed to it has not already
** been loaded. It returns 'TRUE' if it has not.
*/
static bool
unread (const char *filename)
{
  unsigned int hashval;
  filelist *fp;
  fp = aofilelist;
  hashval = hash (filename);
#ifdef IGNORE_CASE
  while (fp != NULL
	 && (hashval != fp->chfilehash
	     || stricmp (filename, fp->chfilename) != 0))
    fp = fp->nextfile;
#else
  while (fp != NULL
	 && (hashval != fp->chfilehash
	     || strcmp (filename, fp->chfilename) != 0))
    fp = fp->nextfile;
#endif
  if (fp != NULL)
    error ("Warning: File '%s' has already been read. Ignored.", filename);
  return fp == NULL;
}

#ifndef CROSS_COMPILE
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
static int
match_files (const char dirname[], const char leafname[])
{
  int filecount, offset, i;
  char *fp;
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;

  fp = filebuffer;
  offset = 0;
  filecount = 0;
  do
    {
      regs.r[0] = 9;		/* OS_GBPB call used */
      regs.r[1] = COERCE (&dirname[0], int);
      regs.r[2] = COERCE (fp, int);	/* Address within filebuffer at which to start */
      regs.r[3] = FILECOUNT;
      regs.r[4] = offset;
      regs.r[5] = DIRBUFSIZE;
      regs.r[6] = COERCE (&leafname[0], int);
      swierror = _kernel_swi (OS_GBPB, &regs, &regs);
      if (swierror != NULL)
	{
	  error ("Error: Cannot read directory entry for '%s'", leafname,
		 &swierror->errmess);
	  return 0;
	}
      else
	{
	  filecount += regs.r[3];	/* Number of file entries returned is passed in R3 */
	  offset = regs.r[4];
	  if (offset >= 0)
	    {			/* More entries to come. Point fp at place to put them */
	      for (i = 1; i <= regs.r[3]; i++)
		{
		  while (*++fp != '\0');
		  fp++;
		}
	      if (fp > filebuffer + buffersize)
		{		/* Oops... Run out of buffer */
		  error ("Fatal: Directory '%s' contains too many files for linker to handle",
		         dirname);
		}
	    }
	}
    }
  while (offset >= 0);
  return filecount;
}
#else

/*
** This is the non OS-specific version of match_files.
*/
static int
match_files (const char dirname[], const char leafname[])
{
  dirname = dirname;
  strcpy (filebuffer, leafname);
  return 1;
}
#endif

/*
** 'split_names' is passed a RISC OS path name in 'filename' and
** returns it broken down into a directory name and a leaf name
** in the arrays 'dirname' and 'leafname' respectively
*/
static void
split_names (const char *filename, char dirname[], char leafname[])
{
  char *p, *leafstart, *dirstart;
  int len;
  len = strlen (filename) - 1;  /* Get offset of last char */
  strcpy (dirname, filename);
  dirstart = &dirname[0];
  p = dirstart + len;
  while (p >= dirstart && *p != '.' && *p != ':')
    p--;
  leafstart = p + 1;
  strcpy (leafname, leafstart);
  if (p < dirstart)
    {				/* No directory name found */
      *dirstart = '\0';
    }
  else
    {
      if (*p == ':')
	p++;			/* Need ':' as part of directory name in names like wibble:eek */
      *p = '\0';
    }
}

/*
** 'wildcarded' returns TRUE if the filename pointed at by 'p'
** contains wildcards
*/
static bool
wildcarded (const char *p)
{
  while (*p > ' ' && strchr (WILDCARDS, *p) == NULL)
    p++;
  return *p > ' ';
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
bool
get_files (const char *filename)
{
    int count, i;
    char *dp, *p;
    bool ok;
    char dirname[FNAMELEN];
    char leafname[LEAFLEN];

  if (!wildcarded (filename))
    return read_file_and_process_wrap (filename);	/* No wildcards. Simple call */

  /*
  ** Wildcards found, so we have to find all the file names that
  ** match the wildcard specification. Separate directory name
  ** and leaf name and then see what there is, allowing for
  ** back-to-front 'xxx*.o' filenames. 'wildcarded' is used
  ** here as a quick check for 'xxx*' as a directory name (when
  ** it could be the leafname in 'xxx*.o')
  */
  split_names (filename, dirname, leafname);
  count = 0;
  if (!wildcarded (dirname))
    count = match_files (dirname, leafname);
#ifndef CROSS_COMPILE
  if (count == 0 && rearrange (filename))
    {			    /* 'xxx*.o' type filename */
      split_names (filename, dirname, leafname);
      count = match_files (dirname, leafname);
    }
#endif
  if (count == 0)
    {
      error ("Error: Cannot find '%s'", filename);
      return FALSE;
    }
  ok = TRUE;
  dp = filebuffer;
  p = &dirname[0] + strlen (dirname);
  if (p != &dirname[0] && p[-1] != ':')
    {			    /* Need '.' after dir name */
      *p++ = '.';
      *p = '\0';
    }
  for (i = 1; i <= count && ok; i++)
    {
      strcpy (p, dp);
      ok &= read_file_and_process_wrap (dirname);
      dp += strlen (dp) + 1;
    }
  return ok;
}

/*
** 'tidy_files' is called when a fatal linker error occurs to make
** sure all files are closed properly. It also deletes any half-
** completed files that were being written at the time
*/
void
tidy_files (void)
{
  if (imagefile != NULL)
    {
      fclose (imagefile);
      imagefile = NULL;
      remove (imagename);
    }
  if (symbolfile != NULL)
    {
      fclose (symbolfile);
      symbolfile = NULL;
      remove (symbolname);
    }
  if (mapfile != NULL)
    {
      fclose (mapfile);
      mapfile = NULL;
      remove (mapfilename);
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
static bool
load_textfile (const char *filename, char **where, int *size)
{
  int fsize;
  char *p;
  FILE *textfile;
  size_t count;

  if ((textfile = fopen (filename, "r")) == NULL)
    {
      error ("Error: Unable to read file '%s'", filename);
      return FALSE;
    }
  if ((fsize = find_size (textfile)) < 0)
    {
      error ("Error: Unable to determine length of '%s'", filename);
      return FALSE;
    }
  p = allocmem (fsize + sizeof (char));
  if (p == NULL)
    {
      fclose (textfile);
      error ("Fatal: Out of memory reading '%s' in 'load_textfile'",
	     filename);
    }
  count = fread (p, sizeof (char), (size_t)fsize, textfile);
  fclose (textfile);
  if (count != (size_t)fsize)
    {
      error ("Error: Unable to read file '%s'", filename);
      return FALSE;
    }
  p[fsize] = '\0';
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
bool
load_viafile (const char *vianame)
{
  char *vp;
  int size;
  inviafile = load_textfile (vianame, &vp, &size);
  if (inviafile)
    {
      lastcvptr = cvptr;
      cvptr = vp;
      cvptrend = vp + size;
    }
  return inviafile;
}

/* ---------- 'edit' file processing ---------- */

/*
** 'load_editfile' loads a file of link edit commands into memory.
** It return 'TRUE' is this went okay, otherwise it returns FALSE
*/
bool
load_editfile (const char *name)
{
  char *ep;
  int size;
  bool ok;
  ok = load_textfile (name, &ep, &size);
  if (ok)
    {
      editptr = ep;
      editendptr = ep + size;
    }
  editname = name;
  return ok;
}

/* --------- Create executable image file ---------- */

/*
** 'open_image' creates the image file and then opens it so that
** its contents can be written.
*/
void
open_image (void)
{
  imagefile = fopen (imagename, "wb");
#ifdef CROSS_COMPILE
  if (imagefile != NULL)
    {
      struct stat access;

      if (stat(imagename, &access) == 0)
        chmod(imagename, access.st_mode | S_IXUSR | S_IXGRP | S_IXOTH);
    }
  else
#else
  if (imagefile == NULL)
#endif
    error ("Fatal: Unable to open image file '%s'", imagename);
  filebuftop = filebuffer;
  filebufend = filebuffer + buffersize;
}

/*
** 'write_block' is called to actually write stuff to the image
** file and to check the outcome of the I/O request
*/
static void
write_block (void *where, int size)
{
  if (size != 0)
    {
      size_t count = fwrite (where, size, 1, imagefile);
      if (count != 1)
	error ("Fatal: Error occured writing '%s'", imagename);
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
void
write_image (void *areaddr, unsigned int areasize)
{
  if (filebuftop + areasize >= filebufend)
    {				/* No room in staging buffer */
      write_block (filebuffer, filebuftop - filebuffer);
      filebuftop = filebuffer;
    }

  if (areasize >= buffersize)	/* Area too big for buffer */
    write_block (areaddr, areasize);
  else
    {
      memcpy (filebuftop, areaddr, areasize);
      filebuftop += areasize;
    }
}

/*
** 'write_string' is called to write character strings to the
** image file. This is only used when creating partially-linked
** AOF files
*/
void
write_string (const char *p)
{
  size_t len = strlen (p) + sizeof (char);
  if (filebuftop + len >= filebufend)
    {				/* No room in staging buffer */
      write_block (filebuffer, filebuftop - filebuffer);
      filebuftop = filebuffer;
    }
  memcpy (filebuftop, p, len);
  filebuftop += len;
}

/*
** 'write_zeroes' writes the specified number of zeroes to the
** image file
*/
void
write_zeroes (unsigned int count)
{
  const unsigned int zeroblock[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  while (count >= sizeof (zeroblock))
    {
      write_image (&zeroblock, sizeof (zeroblock));
      count -= sizeof (zeroblock);
    }
  if (count > 0)
    write_image (&zeroblock, count);
}

#ifndef CROSS_COMPILE
/*
** 'set_filetype' sets the type of the file passed to it
** to the filetype specified
*/
static void
set_filetype (const char *filename, int filetype)
{
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  char ro_name[MAXPATHLEN];

  __riscosify_std (filename, 1, ro_name, sizeof (ro_name), NULL);

  regs.r[0] = 0x12;		/* Set file type */
  regs.r[1] = COERCE (ro_name, int);
  regs.r[2] = filetype;
  swierror = _kernel_swi (OS_File, &regs, &regs);
  if (swierror != NULL)
    error ("Fatal: Cannot set filetype of '%s' (%s)", filename, swierror->errmess);
}

/*
** 'set_loadexec' sets the load- and execution address of the
** file given by filename.
*/
static void
set_loadexec (const char *filename)
{
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  char ro_name[MAXPATHLEN];

  __riscosify_std (filename, 1, ro_name, sizeof (ro_name), NULL);

  regs.r[0] = 2;		/* Call to reset load address */
  regs.r[1] = COERCE (ro_name, int);
  regs.r[2] = codebase;
  swierror = _kernel_swi (OS_File, &regs, &regs);
  if (swierror != NULL)
    error ("Fatal: Cannot alter load address of '%s' (%s)", filename, swierror->errmess);

  regs.r[0] = 3;		/* Call to reset execution address */
  regs.r[1] = COERCE (ro_name, int);
  regs.r[3] = entryarea->arplace + entryoffset;
  swierror = _kernel_swi (OS_File, &regs, &regs);
  if (swierror != NULL)
    error ("Fatal: Cannot alter execution address of '%s' (%s)", filename, swierror->errmess);
}
#endif

/*
** 'close_image' writes the final block of data to the image file
** and then closes it. If the image file has a load address different
** to the standard address of 0x8000, the file is changed so that
** it does not have a type but a load and exec address instead.
*/
void
close_image (void)
{
  write_block (filebuffer, filebuftop - filebuffer);
  fclose (imagefile);
  imagefile = NULL;

#ifndef CROSS_COMPILE
  if (opt_codebase)
    set_loadexec (imagename);
  else
    {
      /* Set filetype: */
      unsigned int filetype;
      switch (imagetype)
        {
          case RMOD:
            filetype = MODULE;
            break;

          case AOF:
          case ALF:
            filetype = DATA;
            break;

          default:
            filetype = (opt_debimage && debugsize > 0) ? DEBIMAGE : ABSOLUTE;
            break;
        }
      set_filetype (imagename, filetype);
    }
#endif
}

/*
** 'reset_image' is called to reset the file pointer to the
** offset in the file given after flushing the file buffer.
*/
void
reset_image (unsigned int offset)
{
  int result;
  write_block (filebuffer, filebuftop - filebuffer);
  filebuftop = filebuffer;
  result = fseek (imagefile, offset, SEEK_SET);
  if (result != 0)
    error ("Fatal: Error occured moving to new position in '%s'", imagename);
}

/*
** The next few routines handle the creation of the symbol
** listing file. The first one closes the file and sets its
** type to 'text'
*/
void
close_symbol (void)
{
  if (linewidth < 3)
    fprintf (symbolfile, "\n");
  fclose (symbolfile);
  symbolfile = NULL;
#ifndef CROSS_COMPILE
  set_filetype (symbolname, TEXT);
#endif
}

/*
** 'check_write' ensures that the last write to the symbol file worked
*/
static void
check_write (void)
{
  if (ferror (symbolfile))
    error ("Fatal: Error occured writing to symbol file '%s'", symbolname);
}

void
open_symbol (void)
{
  symbolfile = fopen (symbolname, "w");
  if (symbolfile == NULL)
    error ("Fatal: Cannot create symbol listing file '%s'", symbolname);
  linewidth = 0;
  addroffset = (imagetype == RMOD ? progbase : 0);
  if (!opt_acornmap)
    {
      fprintf (symbolfile,
	       "                          Symbol Map of '%s'\n\n", imagename);
      check_write ();
    }
}

#define FIELDWIDTH 26
/*
** 'write_symbol' adds one entry to the symbol listing
*/
void
write_symbol (symtentry * sp)
{
  int len;
  arealist *ap;
  len = strlen (sp->symtname) + 8;
  if (opt_acornmap)
    {
      if (opt_revmap)
	fprintf (symbolfile, "%06x  %s", sp->symtvalue - addroffset,
		 sp->symtname);
      else
	fprintf (symbolfile, "%-25s %06x", sp->symtname,
		 sp->symtvalue - addroffset);
      check_write ();
      if ((sp->symtattr & SYM_ABSVAL) == 0 && sp->symtarea.areaptr != NULL)
	{			/* A relocatable symbol */
	  ap = sp->symtarea.areaptr;
	  fprintf (symbolfile, " in AREA %s from %s\n", ap->arname,
		   ap->arfileptr->chfilename);
	}
      else			/* Absolute symbol */
	fputc ('\n', symbolfile);
      check_write ();
    }
  else
    {
      fprintf (symbolfile, "%06x %s ", sp->symtvalue - addroffset,
	       sp->symtname);
      check_write ();
      linewidth += (len <= FIELDWIDTH ? 1 : 2);
      if (linewidth >= 3)
	{
	  fputc ('\n', symbolfile);
	  linewidth = 0;
	}
      else
	{
	  len = FIELDWIDTH - len % FIELDWIDTH;
	  if (len != FIELDWIDTH)	/* Pad with blanks */
	    fprintf (symbolfile, "%*.*s", len, len,
		     "                         ");
	}
    }
  check_write ();
}

/*
** 'open_mapfile' is called to create the area map file
*/
void
open_mapfile (void)
{
  mapfile = fopen (mapfilename, "w");
  if (mapfile == NULL)
    error ("Fatal: Cannot create area map file '%s'", mapfilename);
}

/*
** 'write_mapfile' is called to write a line to the area map file
** and to ensure the I/O was clean
*/
void
write_mapfile (const char *text)
{
  size_t count;

  count = fwrite (text, strlen (text), sizeof (char), mapfile);
  if (count != 1)
    error ("Fatal: Error occured writing to area map file '%s'", mapfilename);
}

void
close_mapfile (void)
{
  fclose (mapfile);
  mapfile = NULL;
#ifndef CROSS_COMPILE
  set_filetype (mapfilename, TEXT);
#endif
}

/*
** 'alloc_filebuffer' is called to allocate the memory used to cache the
** image file, hold directory entries and so on
*/
void
alloc_filebuffer (void)
{
  if (buffersize < MINBUFFER)
    buffersize = MINBUFFER;
  if ((filebuffer = allocmem (buffersize)) == NULL)
    error ("Fatal: Out of memory in 'alloc_filebuffer'");
}

/*
** 'resize_filebuffer' is called when memory is running short to reduce
** the size of the buffer to its minimum size, 8K. It is normally 64K
** bytes in size.
*/
void
resize_filebuffer (void)
{
  if (filebuffer == NULL)
    buffersize = MINBUFFER;	/* Not allocated buffer yet */
  else if (buffersize > MINBUFFER)
    {
      freemem (filebuffer, buffersize);
      filebuffer = allocmem (MINBUFFER);
      buffersize = MINBUFFER;
    }
}

/*
** 'init_files' initialises everything to do with files
*/
void
init_files (void)
{
  imagetype = NOTYPE;
  aofilelist = NULL;
  liblist = NULL;
  debugflist = NULL;
  headersize = debugsize = imagesize = 0;
  imagename = NULL;
  objectname = NULL;
  buffersize = STDBUFFER;
}
