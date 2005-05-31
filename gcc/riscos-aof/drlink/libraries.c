/*
** Drlink AOF linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
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
** This source file contains the routines used for handling libraries
*/

#include "config.h"

#include <string.h>

#include "drlhdr.h"
#include "chunkhdr.h"
#include "filehdr.h"
#include "libraries.h"
#include "procdefs.h"

/* Private declarations */

/*
** 'loadlist' records details of members loaded from libraries and
** from which library they came from.
*/
typedef struct loadlist
{
  int ldhash;			/* Hashed member/library name */
  const char *ldlibname;	/* Library name */
  const char *ldmemname;	/* Member name */
  struct loadlist *ldflink;	/* Next entry in list */
} loadlist;

static int current_libhash;	/* Hash value of current library's name */

static loadlist *memberlist;	/* List of library member loaded so far */
static libentry *freeliblist;	/* List of free 'libentry' slots */

static bool lib_check_and_adjust_diry (const char *libname, const lib_overview * liboverviewp);
static bool lib_check_and_adjust_vsrn (const char *libname, const lib_overview * liboverviewp);
static bool lib_check_and_adjust_symt (const char *libname, const lib_overview * liboverviewp);
static bool lib_get_overview (chunkhdr * ch, const char *filename, unsigned int filesize, lib_overview *liboverviewp);
static const char *find_membername (const lib_overview *liboverviewp, unsigned int index);
static bool add_libsymbol (libheader * fp, unsigned int index, const char *name);
static bool scan_libsymt (libheader * lp);
static bool read_libchunkhdr (libheader * lp);
static bool read_chunk (libheader *lp, const char *chunkid, unsigned int chunkoffset, unsigned int chunksize, void *loadaddr);
static void add_loadlist (libentry * mp);
static filelist *read_member (libentry * lp, filelist * inwhat, symbol * forwhat);
static bool extract_member (const char *libname, chunkhdr * ch, unsigned int chsize);

/*
** 'lib_check_and_adjust_diry' checks the LIB_DIRY chunk and (if needed)
** adjusts the LIB_DIRY data for endian differences.  This routine may
** only be called once on the same chunk data.
** Returns TRUE if everything is ok, FALSE otherwise.
*/
/* FIXME: we can do more checks ! */
static bool
lib_check_and_adjust_diry (const char *libname, const lib_overview *liboverviewp)
{
  libdiryentry * diryptr = liboverviewp->diryptr;
  unsigned int size = liboverviewp->dirysize;
  libdiryentry *diryendptr = COERCE (COERCE (diryptr, char *) + size, libdiryentry *);
  while (diryptr < diryendptr)
    {
      convert_endian (diryptr, sizeof (libdiryentry) / 4);
      diryptr =
	COERCE (COERCE (diryptr, char *) + diryptr->dirylen, libdiryentry *);
    }
  if (diryptr != diryendptr)
    {
      error ("Error: ALF file '%s' LIB_DIRY size is unexpected", libname);
      return FALSE;
    }

  return TRUE;
}


/*
** 'lib_check_and_adjust_vsrn' checks the LIB_VSRN chunk and (if needed)
** adjusts the LIB_VSRN data for endian differences.  This routine may
** only be called once on the same chunk data.
** Returns TRUE if everything is ok, FALSE otherwise.
*/
static bool
lib_check_and_adjust_vsrn (const char *libname, const lib_overview *liboverviewp)
{
  unsigned int *vsrnptr = liboverviewp->vsrnptr;
  unsigned int size = liboverviewp->vsrnsize;

  if (size != 4)
    {
      error ("Error: ALF file '%s' has unexpected LIB_VSRN size (0x%x)\n",
	     libname, size);
      return FALSE;
    }

  convert_endian (vsrnptr, 4 / 4);
  /* FIXME: I don't know what ALFV2 ALF files are or in what way they are
  ** different from ALFV1.  drlink code doesn't do anything special for
  ** ALFV2 ALF files.
  */
  if (*vsrnptr != ALFV1 && *vsrnptr != ALFV2)
    {
      error
	("Error: '%s' is a library of a type that Drlink does not support",
	 libname);
      return FALSE;
    }

  return TRUE;
}


/*
** 'lib_check_and_adjust_symt' checks the OFL_SYMT chunk and (if needed)
** adjusts the OFL_SYMT data for endian differences.  This routine may
** only be called once on the same chunk data.
** Returns TRUE if everything is ok, FALSE otherwise.
*/
static bool
lib_check_and_adjust_symt (const char *libname, const lib_overview *liboverviewp)
{
  libsymtentry * symtptr = liboverviewp->symtptr;
  unsigned int size = liboverviewp->symtsize;
  libsymtentry *symtendptr = COERCE (COERCE (symtptr, char *) + size, libsymtentry *);
  unsigned int maxchunks = liboverviewp->base->header.maxchunks;

  while (symtptr < symtendptr)
    {
      convert_endian (symtptr, sizeof (libsymtentry) / 4);
      if (symtptr->chunkindex > maxchunks)
	{
	  error
	    ("Error: Chunk index in OFL_SYMT chunk in '%s' is too high (value=%d, max=%d)",
	     libname, symtptr->chunkindex, maxchunks);
	  return FALSE;
	}
      symtptr =
	COERCE (COERCE (symtptr, char *) + symtptr->entrysize,
		libsymtentry *);
    }
  if (symtptr != symtendptr)
    {
      error ("Error: ALF file '%s' OFL_SYMT chunk size is unexpected",
	     libname);
      return FALSE;
    }

  return TRUE;
}

/*
** 'lib_check_and_adjust()' checks the OFL_SYMT, LIB_DIRY and LIB_VSRN
** chunks of a new-style ALF file.
** ch->header should already be verified for correctness and adjusted
** for endian differences.  This routine may only be called once on
** the same ALF data.
** Note that the LIB_DATA chunks (or any other chunks) are not verified
** nor adjusted for endian differences.
** Returns TRUE if everything is ok, FALSE otherwise.  The lib_overview
** structure will be filled in with chunk information found.
*/
bool
lib_check_and_adjust (chunkhdr * ch, const char *filename, unsigned int filesize, lib_overview *liboverviewp)
{
  if (!lib_get_overview(ch, filename, filesize, liboverviewp))
    return FALSE;

  /* Check and adjust the OFL_SYMT, LIB_DIRY and LIB_VSRN chunks. */
  if (!lib_check_and_adjust_vsrn (filename, liboverviewp)
      || !lib_check_and_adjust_diry (filename, liboverviewp)
      || !lib_check_and_adjust_symt (filename, liboverviewp))
    return FALSE;

  /* We don't check nor adjust LIB_DATA chunks at this point.
  ** This will be done at read_member() time for the two cases
  ** where the library is fully loaded in memory and when
  ** its members are loaded on demand.
  */

  return TRUE;
}


/*
** 'lib_get_overview' fills in the lib_overview structure based on the
** ALF data starting with the chunkhdr ch.
** ch->header should already be verified for correctness and adjusted
** for endian differences.  This routine may only be called once on
** the same ALF data.
** Returns TRUE if everything is ok, FALSE otherwise.
*/
static bool
lib_get_overview (chunkhdr * ch, const char *filename, unsigned int filesize, lib_overview *liboverviewp)
{
  chunkindex *curchunk;
  unsigned int maxchunks, numchunks;
  unsigned int chindex;

  memset(liboverviewp, 0, sizeof(lib_overview));
  liboverviewp->base = ch;

  maxchunks = ch->header.maxchunks;
  numchunks = ch->header.numchunks;
  /* Quick scan to pickup position of the chunks.  */
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
	  error
	    ("Error: ALF file '%s' chunk index %d has non 4 divisable chunk offset (0x%x)",
	     filename, chindex, chunkoffset);
	  return FALSE;
	}
      /* Adding chunkoffset and chunksize can overflow so first check
         their value individually.  */
      if (chunkoffset > filesize || chunksize > filesize
	  || chunkoffset + chunksize > filesize)
	{
	  error
	    ("Error: ALF file '%s' chunk index %d has bogus offset and/or size values (0x%x, 0x%x, 0x%x)",
	     filename, chindex, chunkoffset, chunksize, filesize);
	  return FALSE;
	}
      if (chunkoffset < maxchunks * sizeof (chunkindex))
	{
	  error
	    ("Error: ALF file '%s' chunk index %d interfers with chunk index table (offset is 0x%x)",
	     filename, chindex, chunkoffset);
	  return FALSE;
	}

      switch (curchunk->chunkclass)
	{
	case LIB_XXXX:
	  switch (curchunk->chunktype)
	    {
	    case LIB_DIRY:
	      if (liboverviewp->diryptr != NULL)
	        {
	          error("Error: ALF file '%s' contains more than one LIB_DIRY chunk ?", filename);
	          return FALSE;
	        }
	      liboverviewp->diryptr = COERCE (COERCE (ch, char *) + chunkoffset, libdiryentry *);
	      liboverviewp->dirysize = chunksize;
	      break;

	    case LIB_VSRN:
	      if (liboverviewp->vsrnptr != NULL)
	        {
	          error("Error: ALF file '%s' contains more than one LIB_VSRN chunk ?", filename);
	          return FALSE;
	        }
	      liboverviewp->vsrnptr = COERCE (COERCE (ch, char *) + chunkoffset, unsigned int *);
	      liboverviewp->vsrnsize = chunksize;
	      break;
	    }
	  break;

	case OFL_XXXX:
	  switch (curchunk->chunktype)
	    {
	    case OFL_SYMT:
	      if (liboverviewp->symtptr != NULL)
	        {
	          error("Error: ALF file '%s' contains more than one OFL_SYMT chunk ?", filename);
	          return FALSE;
	        }
	      liboverviewp->symtptr = COERCE (COERCE (ch, char *) + chunkoffset, libsymtentry *);
	      liboverviewp->symtsize = chunksize;
	      break;
	    }
	  break;
	}
    }
  /* Check if minimal set of LIB_XXXX chunks are present.  */
  if (liboverviewp->symtptr == NULL)
    {
      error ("Error: ALF file '%s' does not have OFL_SYMT chunk", filename);
      return FALSE;
    }
  if (liboverviewp->diryptr == NULL)
    {
      error ("Error: ALF file '%s' does not have LIB_DIRY chunk", filename);
      return FALSE;
    }
  /* We do not check LIB_VSRN presence. */

  return TRUE;
}

/*
** 'addto_liblist' is called to add the name of the library passed
** to it to the list of libraries to be scanned later. Libraries
** read whilst processing the AOF file list and those given on
** the '-lib' parameter are handled here. Libraries that have
** not been read into memory have 'liboverviewp->base' set to NULL.
*/
bool
addto_liblist (const char *libname, const lib_overview *liboverviewp,
	       unsigned int filesize)
{
  libheader *lp;
  char *cp;
  int n;

  cp = allocmem (strlen (libname) + sizeof (char));
  lp = allocmem (sizeof (libheader));
  if (cp == NULL || lp == NULL)
    error ("Fatal: Out of memory in 'addto_liblist' adding '%s'", libname);

  strcpy (cp, libname);
  lp->libname = cp;
  lp->fhandle = NULL;
  lp->libextent = filesize;

  if (liboverviewp == NULL)
    {
      memset(&lp->lib, 0, sizeof(lib_overview));
      lp->fullyloaded = FALSE;
    }
  else
    {
      lp->lib = *liboverviewp;
      lp->fullyloaded = TRUE;
    }

  lp->libgotsyms = FALSE;
  for (n = 0; n < MAXENTRIES; n++)
    lp->librarysyms[n] = NULL;

  lp->libflink = NULL;

  if (liblist == NULL)
    liblist = lp;
  else
    liblast->libflink = lp;
  liblast = lp;
  return TRUE;
}

/*
** 'find_membername' is called to scan the LIB_DIRY chunk for the
** entry for the LIB_DATA chunk 'index', returning a pointer to
** the member name in the LIB_DIRY chunk or NULL.
** From the Acorn docs, it appears that the LIB_DIRY entries
** can be in any order, so the entire LIB_DIRY chunk has to be
** searched for the chunk index 'index'.
*/
static const char *
find_membername (const lib_overview *liboverviewp, unsigned int index)
{
  const libdiryentry *dp, *diryend;

  dp = COERCE (liboverviewp->diryptr, const libdiryentry *);
  diryend = COERCE (COERCE (dp, char *) + liboverviewp->dirysize, const libdiryentry *);
  while (dp < diryend && index != dp->dataindex)
    dp = COERCE (COERCE (dp, const char *) + dp->dirylen, const libdiryentry *);
  return (dp < diryend) ? (const char *) &dp[1] : NULL;
}

/*
** 'add_libsymbol' adds an entry from the OFL_SYMT chunk to the
** library's symbol table. It returns 'TRUE' if this went okay
** otherwise it returns 'FALSE'
*/
static bool
add_libsymbol (libheader * fp, unsigned int index, const char *name)
{
  const char *memname;
  libentry *lp;
  const chunkindex *hp;
  int hashval;

  /* Validate chunkindex index */
  if (index >= fp->lib.base->header.maxchunks)
    {
      error ("Error: '%s' requires a chunk index %d which does not exist in '%s'", name, index, fp->libname);
      return FALSE;
    }
  hp = &fp->lib.base->firstentry + index;

  if ((memname = find_membername (&fp->lib, index)) == NULL)
    {
      error
	("Error: Cannot find LIB_DIRY entry for '%s' in '%s'. Is library corrupt?",
	 name, fp->libname);
      return FALSE;
    }

  if (freeliblist != NULL)
    {				/* Allocate entry from free list */
      lp = freeliblist;
      freeliblist = freeliblist->left;
    }
  else if ((lp = allocmem (sizeof (libentry))) == NULL)
    error ("Fatal: Out of memory in 'add_libsymbol' adding '%s'", fp->libname);
  hashval = hash (name);

  {
    const char *newname = check_libedit (memname, name, hashval);

    if (newname != name)
      {
	name = newname;
	hashval = hash (name);
      }
  }

  lp->left = NULL;
  lp->right = NULL;
  lp->libhash = hashval;
  lp->libname = name;
  lp->libmember = memname;
  lp->liboffset = hp->chunkoffset;
  lp->libsize = hp->chunksize;
  lp->alfname = fp->libname;

  {
    libentry **tree = &fp->librarysyms[hashval & LIBEMASK];

    if (opt_case)
      {
        while (*tree != NULL)
	  tree = (stricmp (name, (*tree)->libname) > 0) ? &(*tree)->right : &(*tree)->left;
      }
    else
      {
        while (*tree != NULL)
	  tree = (strcmp (name, (*tree)->libname) > 0) ? &(*tree)->right : &(*tree)->left;
      }

    *tree = lp;
  }

  return TRUE;
}

/*
** 'scan_libsymt' scan the 'OFL_SYMT' chunk to build the library's
** symbol table
*/
static bool
scan_libsymt (libheader * lp)
{
  libsymtentry *sp, *symtend;

  sp = lp->lib.symtptr;
  symtend = COERCE (COERCE (sp, char *) + lp->lib.symtsize, libsymtentry *);

  while (sp < symtend)
    {
      if (!add_libsymbol (lp, sp->chunkindex, (const char *) &sp[1]))
	return FALSE;
      sp = COERCE (COERCE (sp, char *) + sp->entrysize, libsymtentry *);
    }
  if (sp != symtend)
    {
      error ("ALF file '%s' has unexpected LIB_SYMT length", lp->libname);
      return FALSE;
    }
  lp->libgotsyms = TRUE;

  return TRUE;
}

/*
** 'open_library' is called to open a library and read the chunk header
** and assorted library chunks into memory. It also scans the chunks
** and builds up the library's symbol table. It returns 'TRUE' if all
** this worked otherwise it returns 'FALSE'.
** Sets the global variables current_lib, current_libhash.
*/
bool
open_library (libheader * lp)
{
  if (lp->libgotsyms)
    {				/* Already read library symbol table */
      current_lib = lp;
      current_libhash = hash (lp->libname);
      return TRUE;
    }

  if (lp->lib.base == NULL)
    {	/* Library is not in memory, pull in the minimum needed chunks */
      if (lp->fhandle == NULL
          && (lp->fhandle = fopen(lp->libname, "rb")) == NULL)
        {
          error ("Error: Failed to open ALF file '%s' for reading", lp->libname);
          return FALSE;
        }
      if (!read_libchunkhdr (lp))
	return FALSE;
    }

  current_lib = lp;
  current_libhash = hash (lp->libname);
  return scan_libsymt (lp);
}

/*
** 'read_libchunkhdr' reads the chunk header index of the library 'lp'.
*/
static bool
read_libchunkhdr (libheader * lp)
{
  unsigned int size;
  size_t count;
  chunkheader header;		/* Chunk header from last file read */
  chunkhdr *ch;
  unsigned int chunkclass;
  bool isoldlib;

  libdiryentry *diryptr;
  unsigned int *vsrnptr;
  libsymtentry *symtptr;

  count = fread (&header, sizeof (chunkheader), 1, lp->fhandle);
  if (count != 1)
    {
      error ("Error: Cannot read start of library '%s'", lp->libname);
      return FALSE;
    }
  size = sizeof (chunkheader) + header.maxchunks * sizeof (chunkindex);
  if (fseek (lp->fhandle, 0, SEEK_SET))
    {
      error ("Error: Failed to seek to begin of file '%s'", lp->libname);
      return FALSE;
    }
  if ((ch = allocmem (size)) == NULL)
    error ("Fatal: Not enough memory to store library chunk array '%s'", lp->libname);

  count = fread (ch, size, 1, lp->fhandle);
  if (count != 1)
    {
      error ("Error: Cannot read chunk array from library '%s'", lp->libname);
      return FALSE;
    }

  if (!check_and_adjust_chunkheader (&ch->header, lp->libname, lp->libextent))
    {
      freemem(ch, size);
      return FALSE;
    }

  chunkclass = check_and_get_chunkclass_before_adjust (ch, lp->libname, &isoldlib);
  if (chunkclass != LIB_XXXX && chunkclass != OFL_XXXX || isoldlib)
    error ("Fatal: Program error, '%s' is not a new style ALF file", lp->libname);

  if (!lib_get_overview (ch, lp->libname, lp->libextent, &lp->lib))
    return FALSE;

  /* Load LIB_DIRY, LIB_VSRN and OFL_SYMT into memory. */
  if ((diryptr = allocmem(lp->lib.dirysize)) == NULL)
    error ("Fatal: Not enough memory for storing LIB_DIRY of library '%s'", lp->libname);
  if ((vsrnptr = allocmem(lp->lib.vsrnsize)) == NULL)
    error ("Fatal: Not enough memory for storing LIB_VSRN of library '%s'", lp->libname);
  if ((symtptr = allocmem(lp->lib.symtsize)) == NULL)
    error ("Fatal: Not enough memory for storing OFL_SYMT library '%s'", lp->libname);

  if (!read_chunk (lp, "LIB_DIRY", COERCE (lp->lib.diryptr, char *) - COERCE (ch, char *), lp->lib.dirysize, diryptr)
      || !read_chunk (lp, "LIB_VSRN", COERCE (lp->lib.vsrnptr, char *) - COERCE (ch, char *), lp->lib.vsrnsize, vsrnptr)
      || !read_chunk (lp, "OFL_SYMT", COERCE (lp->lib.symtptr, char *) - COERCE (ch, char *), lp->lib.symtsize, symtptr))
    {
      freemem (diryptr, lp->lib.dirysize);
      freemem (vsrnptr, lp->lib.vsrnsize);
      freemem (symtptr, lp->lib.symtsize);
      return FALSE;
    }
  lp->lib.diryptr = diryptr;
  lp->lib.vsrnptr = vsrnptr;
  lp->lib.symtptr = symtptr;

  /* Check and adjust the OFL_SYMT, LIB_DIRY and LIB_VSRN chunks. */
  if (!lib_check_and_adjust_vsrn (lp->libname, &lp->lib)
      || !lib_check_and_adjust_diry (lp->libname, &lp->lib)
      || !lib_check_and_adjust_symt (lp->libname, &lp->lib))
    return FALSE;

  return TRUE;
}

/*
** 'read_chunk' reads a chunk from the current file, saving it at
** 'loadaddr'. It returns 'true' if this went okay otherwise it
** returns 'false'
*/
static bool
read_chunk (libheader *lp, const char *chunkid, unsigned int chunkoffset, unsigned int chunksize, void *loadaddr)
{
  if (fseek (lp->fhandle, chunkoffset, SEEK_SET))
    {
      error ("Error: Could not find '%s' chunk in '%s'", chunkid, lp->libname);
      return FALSE;
    }
  if (fread (loadaddr, chunksize, 1, lp->fhandle) != 1)
    {
      error ("Error: Could not read '%s' chunk in '%s'", chunkid, lp->libname);
      return FALSE;
    }

  return TRUE;
}

/*
** 'close_library' is called after scanning a library to tidy up
** if necessary. If the library is not going to be scanned again,
** any memory used is freed. Also, if the library is not memory
** resident, the file is closed.
*/
void
close_library_free (libentry * lep)
{
  if (lep == NULL)
    return;

  close_library_free (lep->left);
  close_library_free (lep->right);

  lep->left = freeliblist;
  freeliblist = lep;
  lep->right = NULL;
}


void
close_library (libheader * lp)
{
  if (lp->fhandle != NULL)
    {
      // FIXME: when the rescan option is given, this will lead to
      // fopen/fclose sequences on the same file.  Need to do better ?
      fclose(lp->fhandle);
      lp->fhandle = NULL;
    }

  if (!opt_rescan)
    {				/* Not scanning library again - Free memory used */
      int n;
      lp->libgotsyms = FALSE;

      for (n = 0; n < MAXENTRIES; n++)
	close_library_free (lp->librarysyms[n]);
    }

  current_lib = NULL;
  current_libhash = 0;
}

/*
** 'free_libmem' is called to return any memory used for handling
** libraries to the heap
*/
void
free_libmem (void)
{
  libentry *lp, *temp;
  for (lp = freeliblist; lp != NULL; lp = temp)
    {				/* Free library entries */
      temp = lp->left;
      freemem (lp, sizeof (libentry));
    }
}

/*
** 'add_loadlist' adds details of a library member that has been brought
** into memory to the load list
*/
static void
add_loadlist (libentry * mp)
{
  loadlist *lp;
  if ((lp = allocmem (sizeof (loadlist))) == NULL)
    error ("Fatal: Out of memory in 'add_loadlist'");

  lp->ldhash = current_libhash + hash (mp->libmember);
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
bool
load_member (libentry * lp, filelist * inwhat, symbol * forwhat)
{
  filelist *fp;

  if ((fp = read_member (lp, inwhat, forwhat)) == NULL)
    return FALSE;

  add_loadlist (lp);
  if (fp->symtries.wantedsyms != NULL)
    {
      symtentry *last_symtbase, *last_symtend;
      last_symtbase = current_symtbase;
      last_symtend = current_symtend;
      resolve_refs (fp);
      current_symtbase = last_symtbase;
      current_symtend = last_symtend;
    }

  return TRUE;
}

/*
** 'read_member' is called to read a member of a library into
** memory. It returns a pointer to the member's file list entry
** if this was okay or NULL if the member could not be read or was
** not an AOF file
*/
static filelist *
read_member (libentry * lp, filelist * inwhat, symbol * forwhat)
{
  filelist *fp;
  const char *membername = lp->libmember;
  chunkhdr *ch;

  if (!current_lib->fullyloaded)
    {				/* Library is not yet in memory. */
      size_t count;
      if (fseek (current_lib->fhandle, lp->liboffset, SEEK_SET))
	{
	  error ("Error: Cannot find library member '%s'", membername);
	  return NULL;
	}
      if ((ch = allocmem (lp->libsize)) == NULL)
	error
	  ("Fatal: Not enough memory available to load library member '%s' of ALF library '%s'",
	   membername, lp->alfname);
      count = fread (ch, lp->libsize, 1, current_lib->fhandle);
      if (count != 1)
	{
	  error ("Error: Cannot read library member '%s' of ALF library '%s'", membername, lp->alfname);
	  return NULL;
	}
    }
  else				/* Library is already in memory. */
    ch =
	COERCE (COERCE (current_lib->lib.base, char *) + lp->liboffset,
		chunkhdr *);

  if (!check_and_adjust_chunkheader (&ch->header, membername, lp->libsize))
    return FALSE;

  switch (check_and_get_chunkclass_before_adjust (ch, membername, NULL))
    {
      case OBJ_XXXX:
        if (opt_verbose)
	  error ("Drlink: Reading member '%s' of ALF library '%s' to resolve '%s' in '%s'", membername, lp->alfname, forwhat->symtptr->symtname, inwhat->chfilename);

	if ((fp = create_filelist (membername, lp->libsize)) != NULL
	    && obj_check_and_adjust (ch, membername, lp->libsize, &fp->obj)
	    && read_tables (fp))
	  addto_filelist(fp);
	else
	  free_filelist(fp);
      break;

      case LIB_XXXX:
      case OFL_XXXX:
        error ("Error: '%s' is a library inside another library '%s'. Nested libraries are not supported", membername, lp->alfname);
        /* fall through */

      default:
        fp = NULL;
        break;
    }

  return fp;
}

/*
** 'load_wholelib' is called when an 'old style' library is
** found. The chunk header is already validated and adjusted so
** after validating and checking all chunk indexes, it
** adds every LIB_DATA member in it to the file list. The proc returns
** 'TRUE' if this worked otherwise it returns 'FALSE'
*/
bool
load_wholelib (const char *libname, chunkhdr *ch, unsigned int filesize)
{
  lib_overview liboverview;
  unsigned int chindex, maxchunks;
  chunkindex *curchunk;

  if (opt_verbose)
    error ("Drlink: Reading all members of old-style library '%s'", libname);

  if (!lib_check_and_adjust(ch, libname, filesize, &liboverview))
    return TRUE;

  maxchunks = ch->header.maxchunks;
  for (chindex = 0, curchunk = &ch->firstentry; chindex < maxchunks; ++chindex, ++curchunk)
    {
      if (curchunk->chunkoffset == 0)
        continue;

      if (curchunk->chunkclass == LIB_XXXX && curchunk->chunktype == LIB_DATA)
        {
          chunkhdr *nch = COERCE(COERCE (ch, char *) + curchunk->chunkoffset, chunkhdr *);

	  if (!extract_member (libname, nch, curchunk->chunksize))
	    return FALSE;
	}
    }

  return TRUE;
}

/*
** 'extract_member' is called to add a member from a library to
** the file list when the entire library is being read. It returns
** 'TRUE' if this worked, otherwise it returns 'FALSE'
*/
static bool
extract_member (const char *libname, chunkhdr * ch, unsigned int chsize)
{
  bool ok;

  if (check_and_adjust_chunkheader (&ch->header, libname, chsize))
    return FALSE;

  switch (check_and_get_chunkclass_before_adjust (ch, libname, NULL))
    {
      case OBJ_XXXX:
        {
          filelist *fp;

	  ok = (fp = create_filelist (libname, chsize)) != NULL
	       && obj_check_and_adjust (ch, libname, chsize, &fp->obj)
	       && read_tables (fp);
	  if (ok)
	    addto_filelist(fp);
	  else
	    free_filelist(fp);
          break;
        }

      case LIB_XXXX:
      case OFL_XXXX:
        error ("Error: There is a library inside another library '%s'. Nested libraries are not supported", libname);
        /* fall through */

      default:
        ok = FALSE;
        break;
    }

  return ok;
}

/*
** 'isloaded' returns TRUE if a library member has already been
** loaded into memory by scanning the 'loaded members' list
** (Thunks... Should this code allow for case-insensitive member
** and filename matching?)
*/
bool
isloaded (libentry * mp)
{
  loadlist *lp;
  int hashval;
  const char *cp, *np;
  lp = memberlist;
  cp = current_lib->libname;
  np = mp->libmember;
  hashval = current_libhash + hash (np);
  while (lp != NULL
	 && (hashval != lp->ldhash || strcmp (np, lp->ldmemname) != 0
	     || strcmp (cp, lp->ldlibname) != 0))
    lp = lp->ldflink;

  return lp != NULL;
}

/*
** 'discard_libraries' is a 'last gasp' routine that attempts to free
** some memory by discarding any libraries that have been loaded. They
** are added to the list of libraries referenced on disk. If any memory
** was released in this way, the proc returns 'TRUE' otherwise it
** returns FALSE
*/
bool
discard_libraries (void)
{
  libheader *lp;
  chunkhdr *place;
  bool freed;

  freed = FALSE;
  for (lp = liblist; lp != NULL; lp = lp->libflink)
    {
      if ((place = lp->lib.base) != NULL)
	{
	  lp->lib.base = NULL;
	  freed = TRUE;
	  freemem (place, lp->libextent);
	  if (opt_verbose)
	    error ("Drlink: Discarding library '%s' to free up memory",
		   lp->libname);
	}
    }
  return freed;
}

/*
** 'init_library' is called to initialise the library code
*/
void
init_library (void)
{
  liblist = NULL;
  freeliblist = NULL;
  memberlist = NULL;
}
