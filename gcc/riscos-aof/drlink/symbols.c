/*
** Drlink AOF Linker
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
** This module contains the functions concerned with manipulating the
** various symbol tables
*/

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "chunkhdr.h"
#include "drlhdr.h"
#include "edithdr.h"
#include "libraries.h"
#include "procdefs.h"
#include "symbolhdr.h"

/* Variables referenced from other files */

symbol *globalsyms[MAXGLOBALS]; /* Global symbol table */

const symbol * image_robase,	/* Symbol table entries of pre-defined symbols */
  *image_rwbase, *image_zibase, *image_rolimit, *image_rwlimit, *image_zilimit, *image_codebase,	/* Old symbols used by Fortran 77 */
  *image_codelimit, *image_database, *image_datalimit, *reloc_code;

libheader *current_lib;		/* Pointer to libheader entry for library being searched */

symtentry * current_symtbase,	/* Start of OBJ_SYMT chunk in current file containing references */
  *current_symtend;		/* End of OBJ_SYMT chunk in current file */

unsigned int totalsymbols,	/* Total number of symbols defined in program */
  numwanted,			/* Number of symbols currently wanted */
  numfound,			/* Number of symbols found this library pass */
  lldsize;			/* Space required for names in low-level debug table */

/* Private declarations */

typedef struct missing
{
  const char *symname, *filename;	/* Symbol not found and the file that referenced it */
  struct missing *left;		/* Left in binary tree				    */
  struct missing *right;	/* Right in binary tree				    */
} missing;

static symbol *commonsyms[MAXCOMMON];	/* Common Block symbol table */

static symbol *symblock,	/* Memory to hold symbol entries for file when reading file */
 *wantedlist;			/* 'Wanted symbols' list in current file */

static filelist *current_file;  /* File list entry for file being processed */
static symtable *current_table; /* Pointer to local symbol table of current file */
static editcmd * current_symedit,	/* Pointer to symbol edits for current file */
 *current_refedit;		/* Pointer to reference edits for current file */

#ifndef HAVE_STRICMP
/*
** 'stricmp' performs a comparison of two null-terminated character
** strings ignoring the case of the letters
*/
int
stricmp (const char *str1, const char *str2)
{
  while (*str1 && tolower (*str1) == tolower (*str2))
    {
      str1++;
      str2++;
    }
  return *str1 - *str2;
}
#endif

/*
** 'hash' returns a hash value for the character string passed to it.
** The character is OR'ed with 0x20 to do a quick and dirty
** conversion to lower case because we need to ignore the case of
** any letters. I suppose it would be better to use 'tolower' but
** it's only a hashing function.
*/
int
hash (const char *name)
{
  int total;
  for (total = 0; *name != 0; ++name)
    total = total * 5 ^ (*name | 0x20);
  return total;
}

/*
** 'isrelocatable' is called to determine if a relocation refers to
** something that itself is relocatable. References to areas are
** relocatable but references to symbols can be absolute.
*/
bool
isrelocatable (relocation * rp)
{
  unsigned int symtindex, reltype;
  symtentry *sp;
  bool istype2;
  reltype = rp->reltypesym;
  istype2 = (reltype & REL_TYPE2) != 0;

  reltype = (istype2) ? get_type2_type (reltype) : get_type1_type (reltype);
  if ((reltype & REL_SYM) == 0)
    return TRUE;			/* Simple area reference */

  symtindex = (istype2) ? get_type2_index (rp->reltypesym) : get_type1_index (rp->reltypesym);

  sp = symtbase + symtindex;
  if ((sp->symtattr & SYM_WEAKREF) != 0)
    return FALSE;			/* Reference to a weak external */

  if ((sp->symtattr & SYM_DEFN) == 0)
    sp = sp->symtarea.symdefptr;	/* Reference to external symbol */

  return (sp->symtattr & SYM_ABSVAL) == 0;
}

/*
** 'check_edits' is called to see if there are any edits for the
** current file and to set up pointers to the edits if there are
*/
static void
check_edits (const char *filename, int hashval)
{
  editcmd *ep;
  current_symedit = NULL;
  current_refedit = NULL;

  if (symedit_count > 0)
    {	/* Symbol edits exist */
      ep = symedit_list;
      while (ep != NULL && hashval > ep->edtfnhash)
	ep = ep->edtnext;

      if (ep != NULL && hashval == ep->edtfnhash
	  && stricmp (filename, ep->edtfile) == 0)
	current_symedit = ep;
    }
  if (refedit_count > 0)
    {	/* Reference edits exist */
      ep = refedit_list;
      while (ep != NULL && hashval > ep->edtfnhash)
	ep = ep->edtnext;
      if (ep != NULL && hashval == ep->edtfnhash
	  && stricmp (filename, ep->edtfile) == 0)
	current_refedit = ep;
    }
}

/*
** 'check_symedit' is called to check if the symbol passed
** to it has to be edited and if so to carry out that edit.
** Note that multiple edits on one symbol are allowed, so
** that, for example, a symbol can be renamed and made local
** or global at the same time.
*/
static void
check_symedit (symtentry * symtp)
{
  const char *symname = symtp->symtname;
  int fnhashval = current_file->chfilehash;
  int syhashval = hash (symname);
  editcmd *ep;

  for (ep = current_symedit; ep != NULL && ep->edtfnhash == fnhashval; ep = ep->edtnext)
    {
      if (ep->edtsyhash == syhashval &&
	  ((!opt_case && strcmp (ep->edtold, symname) == 0)
	   || (opt_case && stricmp (ep->edtold, symname) == 0)))
	{			/* Found symbol */
	  symedit_count -= 1;
	  ep->edtdone = TRUE;

	  switch (ep->edtoper)
	    {
	    case EDT_RENAME:
	      symtp->symtname = ep->edtnew;
	      if (opt_verbose)
		error ("Warning: Symbol '%s' in '%s' renamed as '%s'",
		       ep->edtold, ep->edtfile, ep->edtnew);
	      break;

	    case EDT_HIDE:
	      if ((symtp->symtattr & SYM_SCOPE) == SYM_GLOBAL)
		{		/* Global symbol */
		  symtp->symtattr =
		    symtp->symtattr - (SYM_GLOBAL - SYM_LOCAL);
		  if (opt_verbose)
		    error ("Warning: Symbol '%s' in '%s' now marked as 'local'",
			   ep->edtold, ep->edtfile);
		}
	      else
		error ("Warning: Symbol '%s' in '%s' is already a 'local' symbol",
		       symtp->symtname, ep->edtfile);
	      break;

	    case EDT_REVEAL:
	      if ((symtp->symtattr & SYM_SCOPE) == SYM_LOCAL)
		{		/* Local symbol */
		  symtp->symtattr =
		    symtp->symtattr + (SYM_GLOBAL - SYM_LOCAL);
		  if (opt_verbose)
		    error ("Warning: Symbol '%s' in '%s' now marked as 'global'",
			   ep->edtold, ep->edtfile);
		}
	      else
		error ("Warning: Symbol '%s' in '%s' is already a 'global' symbol",
		       symtp->symtname, ep->edtfile);

	    default:
	      break;
	    }
	}
    }
}


/*
** 'check_libedit' checks if a library entry might be renamed.  This is used to
** index a library before its symbols are loaded.
*/
const char *
check_libedit(const char *memname, const char *symname, int syhashval)
{
  static int fnhashval = 0;
  editcmd *ep;
  static const char *current_member = NULL;

  if (!symedit_count)
    return symname;

  if (current_member != memname)
    {
      check_edits (memname, fnhashval = hash (memname));
      current_member = memname;
    }

  for (ep = current_symedit; ep != NULL && ep->edtfnhash == fnhashval; ep = ep->edtnext)
    {
      if (ep->edtsyhash == syhashval &&
	  ((!opt_case && strcmp (ep->edtold, symname) == 0)
	   || (opt_case && stricmp (ep->edtold, symname) == 0)))
	{			/* Found symbol */
	  switch (ep->edtoper)
	    {
	    case EDT_RENAME:
	      if (opt_verbose)
		error ("Warning: Library reference '%s' in '%s' renamed as '%s'",
		       ep->edtold, ep->edtfile, ep->edtnew);
	      return ep->edtnew;

	    default:
	      break;
	    }
	}
    }

  return symname;
}


/*
** 'check_refedit' is called to check if the symbol reference
** passed to it has to be changed and if so to change it
*/
static void
check_refedit (symtentry * symtp)
{
  int syhashval, fnhashval;
  editcmd *ep;
  const char *symname;
  symname = symtp->symtname;
  fnhashval = current_file->chfilehash;
  syhashval = hash (symname);
  for (ep = current_refedit; ep != NULL && ep->edtfnhash == fnhashval; ep = ep->edtnext)
    {
      if (ep->edtsyhash == syhashval &&
	  ((!opt_case && strcmp (ep->edtold, symname) == 0)
	   || (opt_case && stricmp (ep->edtold, symname) == 0)))
	{			/* Found symbol */
	  refedit_count -= 1;
	  ep->edtdone = TRUE;
	  symtp->symtname = ep->edtnew;
	  if (opt_verbose)
	    error ("Warning: Symbol reference '%s' in '%s' changed to '%s'",
		   ep->edtold, ep->edtfile, ep->edtnew);
	}
    }
}


static symbol **
insert_symbol (symbol ** tree, const char *name)
{
  if (opt_case)
    while (*tree != NULL)
      {
	int compval = stricmp (name, (*tree)->symtptr->symtname);
	if (compval == 0)
	  break;
	tree = (compval > 0) ? &(*tree)->right : &(*tree)->left;
      }
  else
    while (*tree != NULL)
      {
	int compval = strcmp (name, (*tree)->symtptr->symtname);
	if (compval == 0)
	  break;
	tree = (compval > 0) ? &(*tree)->right : &(*tree)->left;
      }

  return tree;
}


/*
** 'add_symbol' adds a symbol to one of the symbol tables. It returns
** 'TRUE' if the new entry was created successfully or FALSE if the
** call failed. It checks for duplicate entries and flags any it finds.
** It is possible for there to be two occurences of a symbol, one
** ordinary one and one with the 'strong' attribute set. 'symtptr'
** points at the 'strong' definition if one is found (as this will
** be the one most widely used). 'symnormal' points at the non-strong
** definition when there are two versions, else it is nil. At present,
** however, the case where a strong symbol that duplicates a non-strong
** one is detected whilst scanning libraries cannot be handled. The
** strong version will be used in preference to the non-strong one when
** future external references are dealt with, almost certainly leading
** to errors in the link. (To fix this properly would require the linker
** to scan the OBJ_SYMT chunk of every entry in every library for
** every symbol *just in case* there is a strong version. This would slow
** things down horribly.) Another case to check for is duplicated symbols
** in code and data common areas. In this case, the second and subsequent
** symbol definitions have to be ignored. This can be dealt with as the
** area containing each symbol will be the same (the OBJ_HEAD
** processing takes care of this) so it is just a matter of checking
** the 'area' pointers and if they are the same quietly ignoring the
** symbol.
**
** If there are duplicate entries then, for a local' table entry, this
** means there is something wrong with the OBJ_SYMT chunk but for a
** 'global' symbol then it is just a simple case (of the same symbol
** appearing in more than one file.
**
** There is a kludge in here to handle something the C compiler does
** in its AOF files. It generates local symbols that are not associated
** with any area. If one of these is found, the symbol is converted
** to an absolute value. This would be okay, except that, in certain
** cases, the C compiler also generates relocations for the symbol as
** well, which means the relocation code has to be kludged to handle
** them too.
*/
static bool
add_symbol (filelist *fp, symtentry * symtp)
{
  symbol *p;
  symbol **table;
  int hashval;
  unsigned int attr;
  const char *name;
  arealist *ap;

  symtp->symtname += COERCE (fp->obj.strtptr, unsigned int);

  if (current_symedit != NULL)
    check_symedit (symtp);
  name = symtp->symtname;
  hashval = hash (name);
  attr = symtp->symtattr & SYM_ATMASK;

  if ((attr & SYM_ABSVAL) == 0)
    {	/* Symbol is not an absolute value. Find its area */
      ap = find_area (fp->obj.strtptr + symtp->symtarea.areaname);
      if (ap == NULL)
	{	/* Dodgy C symbol reference to unknown area */
	  symtp->symtattr |= SYM_ABSVAL;	/* Convert symbol to absolute */
	}
    }
  else
    ap = NULL;

  symtp->symtarea.areaptr = ap;

  if ((attr & SYM_SCOPE) == SYM_LOCAL)
    table = &(*current_table)[hashval & LOCALMASK];
  else
    table = &globalsyms[hashval & GLOBALMASK];

  if (current_file->keepdebug)
    lldsize += strlen (name) + sizeof (char);

  table = insert_symbol (table, name);
  p = *table;

  if (p == NULL)
    {				/* Entry not found */
      p = symblock++;
      p->symhash = hashval;
      p->symtptr = symtp;
      p->symnormal = NULL;
      p->left = p->right = NULL;
      *table = p;
    }
  else if ((p->symtptr->symtattr & SYM_STRONG) == (attr & SYM_STRONG))
    {	/* Duplicate symbol */
      arealist *cp = p->symtptr->symtarea.areaptr;

      if ((ap->aratattr & (ATT_COMDEF | ATT_COMMON)) != 0	/* Symbol's area is common block */
	  && (cp->aratattr & (ATT_COMDEF | ATT_COMMON)) != 0	/* Dup symbol's area is common block too */
	  && strcmp (ap->arname, cp->arname) == 0	/* Common block names are the same */
	  && symtp->symtvalue == p->symtptr->symtvalue)
	{			/* Symbol values are the same */
	  return TRUE;		/* No problem */
	}

      if (link_state == LIB_SEARCH)
	error
	  ("Error: '%s' in '%s(%s)' duplicates a symbol already read in '%s'",
	   name, current_lib->libname, objectname, cp->arfileptr->chfilename);
      else
	error
	    ("Error: '%s' in '%s' duplicates a symbol already read in '%s'",
	     name, objectname, cp->arfileptr->chfilename);

      return FALSE;
    }
  else
    {	/* New symbol of different 'strength' to existing definition */
      if ((attr & SYM_STRONG) != 0)
	{	/* New symbol is strong; old one is non-strong */
	  if (link_state == LIB_SEARCH)
	    {	/* Avoid possible use of wrong symbol... */
	      error
		("Error: 'Strong' definition of '%s' found in '%s' after non-strong definition has been used",
		 name, objectname);
	      return FALSE;
	    }
	  p->symnormal = p->symtptr;
	  p->symtptr = symtp;
	}
      else
	{	/* Old symbol is strong; new one is non-strong */
	  p->symnormal = symtp;
	}
    }

  return TRUE;
}

/*
** 'add_externref' adds an externally defined symbol to a file's
** 'wanted symbols' list.
** Memory for the symbol is taken from the block allocated for
** symbols in the file being processed.
*/
static void
add_externref (filelist *fp, symtentry * symtp)
{
  symbol *sp, **insert;
  const char *name;

  symtp->symtname = symtp->symtname + COERCE (fp->obj.strtptr, unsigned int);

  if (current_refedit != NULL)
    check_refedit (symtp);
  name = symtp->symtname;
  sp = symblock++;
  sp->symhash = hash (name);
  sp->symtptr = symtp;
  sp->symnormal = NULL;
  sp->left = sp->right = NULL;

  insert = insert_symbol (&wantedlist, name);
  if (!*insert)
    *insert = sp;

  numwanted++;			/* Bump up count of wanted symbols */
}

/*
** 'create_externref' creates dummy OBJ_SYMT and symbol entries for
** the symbol passed to it. It is used when dealing with strong
** symbols.
**
** External references created by this routine are dealt with in the
** same way as normal references. They are added to the current AOF
** AOF file's 'wanted' list. 'resolve' will come across these in the
** normal way, but will find that the symbol definition lies within
** the same module as the reference (although the OBJ_SYMT external
** reference is not: it only checks the whereabouts of the
** definition. If there is a non-strong definition, then there is no
** problem. If none, the library search code has to deal with the
** reference.
*/
symbol *
create_externref (symtentry * stp)
{
  symbol *sp, **insert;
  symtentry *newstp;
  const char *name;

  sp = allocmem (sizeof (symbol));
  newstp = allocmem (sizeof (symtentry));
  if (sp == NULL || newstp == NULL)
    error ("Fatal: Out of memory in 'create_externref'");
  name = stp->symtname;
  newstp->symtname = name;
  newstp->symtattr = SYM_EXTERN;
  newstp->symtvalue = 0;
  newstp->symtarea.symdefptr = NULL;
  sp->symhash = hash (name);
  sp->symtptr = newstp;
  sp->symnormal = NULL;
  sp->left = sp->right = NULL;

  insert = insert_symbol (&wantedlist, name);
  if (!*insert)
    *insert = sp;

  numwanted++;

  return sp;
}

/*
** 'add_commonref' is called when a symbol has the 'common reference' bit
** set to check if a common block whose name matches that of the symbol
** exists and if not to create it. The reference to the common block
** is resolved at this time as well, so, although the 'external ref'
** bit is set, the symbol is not added to the 'wanted symbols' list.
** One slightly iffy things the code does is to set the size of the
** common block to zero after creating the common block to stop the
** resolution code using the size of the common block as an offset (the
** words 'bug fix' and 'hack' come to mind here).
*/
static void
add_commonref (filelist *fp, symtentry * stp)
{
  const char *name;
  symbol *sp;

  stp->symtname = stp->symtname + COERCE (fp->obj.strtptr, unsigned int);
  if (current_refedit != NULL)
    check_refedit (stp);

  name = stp->symtname;
  sp = symblock++;
  sp->symhash = hash (name);
  sp->symtptr = stp;
  sp->symnormal = NULL;
  sp->left = sp->right = NULL;

  stp->symtarea.symdefptr = make_commonarea (sp);
  stp->symtvalue = 0;
}

/*
** 'scan_symt' is called to add the symbols in the OBJ_SYMT chunk to
** the different symbol tables. It adds symbol definitions to the
** symbol tables and external references to a linked list of references
** for the file. Note that memory to hold all the SYMT entries is
** allocated at the start for reasons of efficiency and in a vain
** attempt to make the linker go as fast as ARM's. The proc returns
** 'TRUE' if this all worked okay otherwise it returns FALSE.
**
** There is a kludge in here to handle a problem with partially-linked
** AOF files produced by version 5 of the ARM linker. It creates
** external references with the 'absolute' attribute bit set. This is
** meaningless for a reference, so the code clears it. The code to
** handle relocations goes wrong otherwise.
*/
bool
scan_symt (filelist * fp)
{
  int i, symcount, attr;
  symtentry *symtp;
  bool ok, addok, gotstrong;
  unsigned int strtsize;

  symcount = fp->symtcount;
  if ((symblock = allocmem (symcount * sizeof (symbol))) == NULL)
    error ("Fatal: Out of memory in 'scan_symt' reading file '%s'",
	   fp->chfilename);

  current_file = fp;
  current_table = &fp->localsyms;

  /* Are there any link edits for this file? */
  check_edits (current_file->chfilename, current_file->chfilehash);

  strtsize = fp->obj.strtsize;
  wantedlist = NULL;
  gotstrong = FALSE;
  addok = ok = TRUE;

  for (i = 1, symtp = fp->obj.symtptr; i <= symcount && ok; ++i, ++symtp)
    {
      if (COERCE (symtp->symtname, unsigned int) >= strtsize)
	{
	  error
	    ("Error: Offset of symbol name in OBJ_SYMT chunk is bad in '%s' (0x%x > 0x%x)",
	     fp->chfilename, COERCE (symtp->symtname, unsigned int), strtsize);
	  symtp->symtname = NULL;
	  ok = FALSE;
	  break;
	}

      if (!strcmp (symtp->symtname + COERCE (fp->obj.strtptr, unsigned int), "$d")
	  || !strcmp (symtp->symtname + COERCE (fp->obj.strtptr, unsigned int), "$a"))
	{
	  /* Convert symbol to absolute to prevent an attempt at relocation */
	  symtp->symtattr = symtp->symtattr | SYM_ABSVAL;
	  /* Ignore these symbols due to possible objasm bug? */
	  continue;
	}
      attr = symtp->symtattr;
      if (aofv3flag && (attr & SYM_UNSUPATTR) != 0)
	{ /* Reject unsupported symbol attributes */
	  error ("Warning: Symbol '%s' in '%s' has unsupported AOF symbol attributes (%08x). It might be that you are using one or more AOF files produced by an older 'as' version which contained a bug causing this problem.",
	     symtp->symtname + COERCE (fp->obj.strtptr, unsigned int),
	     fp->chfilename, attr & SYM_UNSUPATTR);
	  attr &= ~SYM_UNSUPATTR;
	}
      attr = attr & SYM_ATMASK;

      if ((attr & SYM_DEFN) != 0)
	{			/* Add symbol defintion to symbol table */
	  addok = addok && add_symbol (fp, symtp);
	  gotstrong = gotstrong || (attr & SYM_STRONG) != 0;
	  totalsymbols++;
	}
      else if (attr == (SYM_COMMON | SYM_EXTERN))
	add_commonref (fp, symtp);	/* Reference to a common block */
      else if (attr == SYM_EXTERN)
	add_externref (fp, symtp);	/* Reference to an external symbol */
      else if (attr == (SYM_EXTERN | SYM_ABSVAL))
	{			/* Dodgy ARM linker trick... */
	  symtp->symtattr -= SYM_ABSVAL;	/* Lose 'absolute' attribute bit */
	  add_externref (fp, symtp);
	}
      else
	{
	  error
	    ("Error: OBJ_SYMT attribute value (0x%x) of symbol '%s' in '%s' is bad. Is file corrupt?",
	     attr, COERCE (fp->obj.strtptr, unsigned int) + symtp->symtname,
	     fp->chfilename);
	  ok = FALSE;
	}
    }
  if (gotstrong)
    check_strongrefs (fp);	/* Special processing for strong symbols */
  fp->symtries.wantedsyms = wantedlist;
  return ok && addok;
}


static symbol *
find_symbol (symbol * tree, const char *name, bool ignorecase)
{
  if (ignorecase)
    while (tree != NULL)
      {
	int compval = stricmp (name, tree->symtptr->symtname);

	if (compval == 0)
	  return tree;

	tree = (compval > 0) ? tree->right : tree->left;
      }
  else
    while (tree != NULL)
      {
	int compval = strcmp (name, tree->symtptr->symtname);

	if (compval == 0)
	  return tree;

	tree = (compval > 0) ? tree->right : tree->left;
      }

  return NULL;
}



/*
** 'search_global' is a custom routine for searching the global symbol table as
** quickly as possible
*/
static symbol *
search_global (symbol * wantedsym)
{
  symbol *p;
  const char *name;

  p = globalsyms[wantedsym->symhash & GLOBALMASK];
  name = wantedsym->symtptr->symtname;

  return find_symbol (p, name, opt_case
		      || (wantedsym->symtptr->symtattr & SYM_IGNCASE) != 0);
}

/*
** 'search_local' searches the current local symbol table for the required
** symbol, returning a pointer to its entry if found or nil if not.
*/
static symbol *
search_local (symbol * wantedsym)
{
  symbol *p;
  const char *name;

  p = (*current_table)[wantedsym->symhash & LOCALMASK];
  name = wantedsym->symtptr->symtname;

  return find_symbol (p, name, opt_case
		      || (wantedsym->symtptr->symtattr & SYM_IGNCASE) != 0);
}

/*
** 'search_common' searches the common block symbol table for the required
** symbol, returning a pointer to its entry if found or nil if not.
*/
symbol *
search_common (symbol * wantedsym)
{
  symbol *p;
  const char *name;

  p = commonsyms[wantedsym->symhash & COMMONMASK];
  name = wantedsym->symtptr->symtname;

  return find_symbol (p, name, opt_case
		      || (wantedsym->symtptr->symtattr & SYM_IGNCASE) != 0);
}

/*
** 'find_common' is called to look for the common block 'name'. It
** has to create a dummy 'symbol' and 'symt' entry for the common
** block and returns a pointer to the symbol table for the common
** block if found or 'nil'
*/
symbol *
find_common (const char *name)
{
  symbol dummysym;
  symtentry dummysymt;
  dummysym.symhash = hash (name);
  dummysym.symtptr = &dummysymt;
  dummysymt.symtname = name;
  dummysymt.symtattr = 0;
  return search_common (&dummysym);
}

/*
** 'add_unresolved' adds a symbol to one of the missing symbol
** lists if there is no reference to it already there
*/
static void
add_unresolved (missing ** tree, symbol * sp, filelist * fp)
{
  const char *sname = sp->symtptr->symtname;

  if (opt_case || (sp->symtptr->symtattr & SYM_IGNCASE) != 0)
    while (*tree != NULL)
      {
	int compval = stricmp (sname, (*tree)->symname);

	if (compval == 0)
	  return;

	tree = (compval > 0) ? &(*tree)->right : &(*tree)->left;
      }
  else
    while (*tree != NULL)
      {
	int compval = strcmp (sname, (*tree)->symname);

	if (compval == 0)
	  return;

	tree = (compval > 0) ? &(*tree)->right : &(*tree)->left;
      }

  if (*tree == NULL)
    {				/* New unresolved or weak symbol */
      if ((*tree = allocmem (sizeof (missing))) == NULL)
	error ("Fatal: Out of memory in 'add_unresolved'");
      (*tree)->symname = sname;
      (*tree)->filename = fp->chfilename;
      (*tree)->left = (*tree)->right = NULL;
    }
}

/*
** 'list_unresolved' is called when one or more symbol references are
** left unresolved to build up lists of missing symbols and unresolved
** weak external references. It is permissible to leave weak symbols
** unresolved, although the linker prints out a list of them at the
** end.
*/
static void
list_unresolved_print (missing * tree)
{
  if (tree == NULL)
    return;

  list_unresolved_print (tree->left);
  error ("    '%s' referenced in '%s'", tree->symname, tree->filename);
  list_unresolved_print (tree->right);
}


static void
list_unresolved_add (symbol * sp, filelist * fp, missing ** misslist,
		     missing ** weaklist)
{
  if (sp != NULL)
    {
      list_unresolved_add (sp->left, fp, misslist, weaklist);

      if ((sp->symtptr->symtattr & SYM_WEAKREF) != 0)
	{			/* No problem if weak external ref */
	  add_unresolved (weaklist, sp, fp);
	  numwanted--;
	}
      else
	{
	  add_unresolved (misslist, sp, fp);
	}

      list_unresolved_add (sp->right, fp, misslist, weaklist);
    }
}


static void
list_unresolved (void)
{
  filelist *fp;
  missing *misslist = NULL, *weaklist = NULL;

  for (fp = aofilelist; fp != NULL; fp = fp->nextfile)
    list_unresolved_add (fp->symtries.wantedsyms, fp, &misslist, &weaklist);

  if (weaklist != NULL && (opt_verbose || opt_info))
    {
      error
	("Warning: The following symbols are unresolved 'weak' references:");
      list_unresolved_print (weaklist);
    }

  if (misslist != NULL)
    {
      error ("Error: The following symbols could not be found:");
      list_unresolved_print (misslist);
    }
}

/*
** 'search_lib' seaches the current library's symbol table for
** the symbol 'wantedsym'. If it is found, the function returns
** a pointer to the library symbol table entry, otherwise it
** returns 'nil'.
*/
static libentry *
search_lib_tree (libentry * lp, const char *name, bool ignorecase)
{
  if (lp != NULL)
    {
      int compval =
	ignorecase ? stricmp (name, lp->libname) : strcmp (name, lp->libname);

      if (compval == 0)
	return lp;

      return search_lib_tree ((compval > 0) ? lp->right : lp->left, name, ignorecase);
    }

  return NULL;
}

static libentry *
search_lib (symbol * wantedsym)
{
  return search_lib_tree (current_lib->librarysyms[wantedsym->symhash & LIBEMASK],
			  wantedsym->symtptr->symtname,
			  opt_case || (wantedsym->symtptr->symtattr & SYM_IGNCASE));
}

/*
** 'get_nextlib' returns a pointer to the next entry in the hash chain that
** matches the one passed to it
*/
static libentry *
get_nextlib (libentry * lp)
{
  int hashval;
  const char *name;
  hashval = lp->libhash;
  name = lp->libname;
  while (lp != NULL
	 && (lp->libhash != hashval || strcmp (name, lp->libname) != 0))
    lp = lp->left;
  return lp;
}

/*
** 'remove_entry' deletes an entry from a file's wanted list
*/
static void
remove_entry (symbol ** wp)
{
  symbol *sym = *wp, *subnode;

  /* Binary tree removal.

     If there's no children, nothing needs to be done.
     If there's only one child on either side, then move it up to the current position.

     If there's two children, then when a subtree exists on the right side, replace it
     with the inorder successor.

   */

  if (sym->left == NULL && sym->right != NULL)
    {
      subnode = sym->right;
      freemem (sym, sizeof (symbol));
      *wp = subnode;
    }
  else if (sym->left != NULL && sym->right == NULL)
    {
      subnode = sym->left;
      freemem (sym, sizeof (symbol));
      *wp = subnode;
    }
  else if (sym->left != NULL && sym->right != NULL)
    {
      /* Find successor */
      symbol **success = &sym->right;

      if (!(*success)->left)
	{
	  /* When there's no left successor, just move the right node up */

	  (*success)->left = sym->left;
	  *wp = *success;

	}
      else
	{
	  while ((*success)->left != NULL)
	    success = &(*success)->left;
	  subnode = *success;

	  *success = subnode->right;
	  subnode->left = sym->left;
	  subnode->right = sym->right;
	  *wp = subnode;
	}

      freemem (sym, sizeof (symbol));
    }
  else
    {
      freemem (sym, sizeof (symbol));
      *wp = NULL;
    }

  numwanted--;
}

/*
** 'check_library' is called to go through a file's 'wanted' list and loads
** any library members that satisfy any of the external references in the
** 'wanted' list. It returns 'TRUE' if this was achieved successfully
** otherwise it returns 'FALSE'. Note that it is necessary to check the
** global symbol table before searching the library to see if the reference
** can be satisfied by a library member now in memory.
**
** The search process is complicated is the presence of strong symbols.
** Cases to consider:
** a) A non-strong definition exists and the library module contains
**    a strong one
** b) A strong definition exists and the library module contains a
**    non-strong one.
**
** a) cannot be dealt with unless the linker is rewritten. All references
**    will have been resolved to the non-strong definition before the
**    strong definition was found and the information to enable the
**    linker to go back and correct the references will have been
**    discarded. (Put out a warning message about this? Will it ever
**    happen?)
** b) At this point, the module containing the reference and the definition
**    have to be different. If the module that contains the strong definition
**    is the one that contains the reference, the non-strong definition should
**    be used otherwise the strong one is the one to employ.
**
** If opt_leaveweak is enabled, weak symbol references that would lead to
** a library module being loaded are ignored. If a library member that
** satisfies the reference is already in memory, it is still used. This
** could lead to some confusion as it is possible that a later reference
** could load a module that would resolve the original weak ref. Unless
** '-rescan' is used, that original ref will remain unresolved. OTOH, if
** the module is already in memory, it will be resolved. To put it another
** way, whether or not a weak reference is resolved depends on the order
** in which the non-weak and weak references are encountered...
*/
static bool
check_library_tree (symbol ** wp, filelist * fp)
{
  bool ok = TRUE;

  if (*wp != NULL)
    {
      symbol *sp;
      libentry *lp;

      ok &= check_library_tree (&(*wp)->left, fp);
      ok &= check_library_tree (&(*wp)->right, fp);

/*
** First, check if the symbol is now available. It is possible that a
** library member now loaded contains the symbol that is required here.
** If so, we should use this symbol rather than search the library
** again. A complication, as ever, is the possibility of a strong
** symbol...
*/
      sp = search_global (*wp); /* Is symbol now available? */
      if (sp != NULL)
	{			/* Yes */
/*
** At this point, a module that contains a symbol definition is in
** memory. There are two possibilities to deal with here:
** a)  The symbol definition is strong and is located in the module
**     whose external references are being processed.
** b)  The symbol definition is non-strong
**
** In case (a), the symbol is not the one to use. We have to search the
** library for the symbol and, as the module being processed is the one
** with the strong definition, we have to find a non-strong version.
** of course, if the strong symbol came from this library, the search
** mechanism will find it first, so we have to ignore that entry and
** find another...
** In case (b), there is no problem. This is the definition to use
*/
	  if ((sp->symtptr->symtattr & SYM_STRONG) != 0
	      && sp->symtptr >= current_symtbase
	      && sp->symtptr < current_symtend
	      && sp->symnormal == NULL)
	    {
	      lp = search_lib (*wp);

	      if (lp != NULL && isloaded (lp))
		{		/* Found symbol in library. Has member been loaded? */
		  lp = get_nextlib (lp);	/* If yes, look for another version */
		  if (isloaded (lp))
		    lp = NULL;
		}

	      if (lp != NULL)
		{
		  numfound++;
		  ok &= load_member (lp, fp, *wp);	/* Not in memory, so load it */
		}
	    }
	  else
	    {			/* Non-strong or strong and non-strong def'n known */
	      numfound++;
	      lp = COERCE (sp, libentry *);	/* This is just so that lp!=nil */
	    }
	}
      else if (((*wp)->symtptr->symtattr & SYM_WEAKREF) != 0 && opt_leaveweak)
	{			/* Ignore if weak external */
	  lp = NULL;
	}
      else
	{			/* Symbol is not available yet. Search library */
	  lp = search_lib (*wp);
	  if (lp != NULL)
	    {			/* Entry found */
	      numfound++;
/*
** Load member. The member containing the ref has to be different to the
** one containing the definition, so there are no problems with strong
** definitions and references in the same modules meaning we've got the
** wrong module
*/
	      ok &= load_member (lp, fp, *wp);
	      sp = search_global (*wp);
	    }
	}

      if (ok && lp != NULL)
	{ /* Now link symbol and reference */
	  if (sp == NULL)
	    {
	      error
		("Error: Cannot find symbol '%s' in library member '%s(%s)'. Is library corrupt?",
		 (*wp)->symtptr->symtname, current_lib->libname,
		 lp->libmember);
	      ok = FALSE;
	    }
	  else
	    {
	      symtentry *wstp = (*wp)->symtptr; /* OBJ_SYMT entry of external reference */

	      if (sp->symnormal != NULL && sp->symtptr >= current_symtbase
		  && sp->symtptr < current_symtend)
		{
		  wstp->symtarea.symdefptr = sp->symnormal;
		}
	      else
		{ /* Only one def or strong def in another module */
		  wstp->symtarea.symdefptr = sp->symtptr;
		}
	      if ((wstp->symtattr & SYM_WEAKREF) != 0)
		{ /* Resolving a weak ref? */
		  wstp->symtattr -= SYM_WEAKREF;
		}
	    }
	  remove_entry (wp);
	}
    }
  return ok;
}



static bool
check_library (filelist * fp)
{
  current_symtbase = fp->obj.symtptr;	/* For detecting 'strong symbol' refs */
  current_symtend = fp->obj.symtptr + fp->obj.symtsize; /* These point at module containing ref */

  return check_library_tree (&fp->symtries.wantedsyms, fp);
}

/*
** 'resolve_refs' is called to try to resolve a file's external symbols
** references using the local, global and common symbol tables. It also
** takes care of strong and non-strong references. This is a bit messy
** so an explanation might be useful for future reference.
**
** In the file being processed, if both a strong and a non-strong
** definition of a symbol exists, the non-strong one is used. In all
** other cases the strong definition is the one employed. The presence of
** two versions of the symbol is indicated by the value of 'symnormal'.
** If it is 'nil' then only one definition exists (strong or non-strong;
** it does not matter which). If not 'nil', then two definitions exists
** and 'symnormal' points at the SYMT entry of the non-strong definition.
** The routine detects whether or not the strong definition of the symbol
** is in the current file by comparing the address of that symbol's SYMT
** entry against the base and end address of the current file's OBJ_SYMT
** chunk.
**
** A complication is that when resolving a symbol reference in the same
** module as its definition, if the definition is a strong definition, it
** cannot be assumed it will be the correct one as a non-strong definition
** might pop up later during the library scans. To allow for this, the
** symbol resolution has to be defered until the 'last pass' when invented
** symbols are dealt with.
*/
void
resolve_refs_tree (symbol ** wp)
{

  if (*wp != NULL)
    {
      symbol *sp;

      resolve_refs_tree (&(*wp)->left);
      resolve_refs_tree (&(*wp)->right);

      sp = search_local (*wp);
      if (sp == NULL)
	{
	  sp = search_global (*wp);
	  if (sp == NULL)
	    sp = search_common (*wp);
	}

      if (sp != NULL)
	{			/* Symbol found */
	  symtentry *wstp = (*wp)->symtptr;	/* Point at SYMT entry for the external ref */
	  symtentry *stp = sp->symtptr; /* Point at first SYMT entry for definition of symbol */

	  if (stp >= current_symtbase && stp <= current_symtend)
	    {			/* Is ref in same module as def'n? */
	      if (sp->symnormal != NULL)
		{		/* Non-strong def'n exists - use that */
		  wstp->symtarea.symdefptr = sp->symnormal;	/* Point at non-strong definition's SYMT entry */
		}
	      else if ((stp->symtattr & SYM_STRONG) == 0)
		{		/* Non-strong symbol */
		  wstp->symtarea.symdefptr = stp;
		}
	      else
		{		/* Strong def'n: do not resolve at this time */
		  sp = NULL;	/* Pretend symbol was not found */
		}
	    }
	  else
	    {			/* Symbol def'n is elsewhere */
	      wstp->symtarea.symdefptr = stp;	/* Point at strong or only definition's SYMT entry */
	    }

	  if (sp != NULL)
	    {
	      remove_entry (wp);
	      if ((wstp->symtattr & SYM_WEAKREF) != 0)
		{		/* Resolving a weak ref? */
		  wstp->symtattr -= SYM_WEAKREF;
		}
	    }
	}
    }
}


void
resolve_refs (filelist * fp)
{

  current_table = &fp->localsyms;
  current_symtbase = fp->obj.symtptr;	/* For detecting 'strong symbol' refs */
  current_symtend = fp->obj.symtptr + fp->obj.symtsize;

  resolve_refs_tree (&fp->symtries.wantedsyms);
}

/*
** 'resolve' is the main function called when trying to resolve
** symbol references. This process is carried out in three stages.
** The first step is to go through all the files in the file
** list and try to resolve symbols between them. If there are
** any unresolved symbols after this, the routine then goes
** through the libraries to find the symbols. Library members that
** satisfy references are loaded immediately and any references they
** contain that can be resolved from the loaded AOF files are
** handled there and then. The files are added to the file list
** anyway, so searching the current library 'comes out in the
** wash'. If the '-rescan' option is used, it continues to search
** the libraries until all references have been satisfied or it
** cannot find them in any library. If a partially-linked AOF
** file is to be created, the symbol resolution step ends here.
** If any other type of image is being created, The final step is
** to search the global symbol table again. This is necessary to
** deal with strong symbols and because global symbols such as
** 'wibble$$Base' can be invented as a side effect of loading
** library members. If there is a reference somewhere to a
** yet-to-be-invented symbol, the library search mechanism will not
** find it.
**
** One thing to note is how multiple scans of libraries are handled.
** What happens is that the library list is turned into a circular
** list and the code just keeps going around the ring until either
** all references have been satisfied or all the libraries have
** been searched for a particular symbol.
*/
bool
resolve (void)
{
  filelist *fp;
  libheader *lp, *lastlp;
  bool ok;

  link_state = AOF_SEARCH;
  if (aofilelist == NULL)
    error ("Fatal: There is nothing to link!");
  if (opt_verbose)
    error ("Drlink: Resolving symbol references (%d to find)...", numwanted);

  for (fp = aofilelist; fp != NULL; fp = fp->nextfile)
    {	/* Resolve symbol references between AOF files */
      if (fp->symtries.wantedsyms != NULL)
	resolve_refs (fp);
    }

  /* Now search the libraries */
  link_state = LIB_SEARCH;
  opt_rescan = opt_rescan && liblist != liblast;
  if (opt_rescan && liblist != NULL)
    liblast->libflink = liblist;	/* Make library list circular */

  ok = TRUE;
  lastlp = NULL;
  numfound = 0;

  for (lp = liblist;
       lp != NULL && numwanted != 0 && lp != lastlp && ok;
       lp = lp->libflink)
    {
      numfound = 0;

      if (opt_verbose)
	{
	  error
	    ("Drlink: Searching library '%s' to resolve symbol references (%d to find)...",
	     lp->libname, numwanted);
	}

      ok = open_library (lp);
      for (fp = aofilelist; fp != NULL && numwanted != 0 && ok; fp = fp->nextfile)
	{
	  if (fp->symtries.wantedsyms != NULL)
	    ok = check_library (fp);
	}

      if (numfound != 0 || lastlp == NULL)
	lastlp = lp;
      close_library (lp);
    }
  free_libmem ();

  if (imagetype == AOF)
    {	/* Quick way out for partially-linked AOF files */
      if (opt_verbose)
	{
	  error
	    ("Drlink: %d reference(s) left unresolved in partially-linked AOF file",
	     numwanted);
	}
      numwanted = 0;
    }
  else
    {
      /* Now check for invented entries and unresolved strong symbols */
      for (fp = aofilelist; fp != NULL && numwanted != 0; fp = fp->nextfile)
	{
	  if (fp->symtries.wantedsyms != NULL)
	    resolve_refs (fp);
	}
      if (numwanted > 0 && !got_errors ())
	list_unresolved ();
    }

  return numwanted == 0;
}


/*
** 'relocate_symbols' goes through the OBJ_SYMT chunk of each
** AOF file and relocates each entry in it.
*/
void
relocate_symbols (void)
{
  filelist *fp;

  for (fp = aofilelist; fp != NULL; fp = fp->nextfile)
    {
      symtentry *sp;
      int n, last;
      last = fp->symtcount;
      for (n = 1, sp = fp->obj.symtptr; n <= last; ++n, ++sp)
	{
	  if (sp->symtarea.areaptr != (void *) 4)
	    {
	      if ((sp->symtattr & (SYM_DEFN | SYM_ABSVAL)) == SYM_DEFN)
		{		/* Relocate if not absolute */
		  sp->symtvalue += sp->symtarea.areaptr->arplace;
		}
	    }
	}
    }
}

/*
** 'define_symbol' is used to set the given symbol to the supplied value
*/
void
define_symbol (const symbol * sp, unsigned int value)
{
  sp->symtptr->symtvalue = value;
}

/*
** 'make_symbol' creates entries in the either the global symbol table
** or the common block table for pre-defined linker symbols and other
** special entries. The table used is controlled by whether the 'common'
** attribute is set or not. Note that this is a bit kludgy in that the
** 'common' symbol attribute denotes a reference to a common block and
** not a common block definition! Another point to note that entries
** are marked as being relocatable although they are not relocated by
** the linker at all.
*/
const symbol *
make_symbol (const char *name, unsigned int attributes)
{
  unsigned int hashval;
  symbol **insert;

  hashval = hash (name);
  if ((attributes & SYM_COMMON) == 0)
    {	/* Put in global table */
      insert = insert_symbol (&globalsyms[hashval & GLOBALMASK], name);
    }
  else
    {	/* Put in common block table */
      insert = insert_symbol (&commonsyms[hashval & COMMONMASK], name);
    }

  if (*insert)
    {
      /* Symbol is already added, we expect the same attributes. */
      if ((attributes | SYM_LINKDEF) != (*insert)->symtptr->symtattr)
        error ("Error: Inconsistent set of attributes for symbol '%s' (0x%x vs 0x%x)", name, attributes, (*insert)->symtptr->symtattr & ~SYM_LINKDEF);
    }
  else
    {
      symbol *sp;
      symtentry *stp;

      sp = allocmem (sizeof (symbol));
      stp = allocmem (sizeof (symtentry));
      if (sp == NULL || stp == NULL)
        error ("Fatal: Out of memory in 'make_symbol'");
      stp->symtname = name;
      stp->symtattr = attributes | SYM_LINKDEF;
      stp->symtvalue = 0;
      stp->symtarea.areaptr = NULL;

      sp->symhash = hashval;
      sp->symtptr = stp;
      sp->symnormal = NULL;
      sp->left = sp->right = NULL;

      *insert = sp;

      totalsymbols++;
    }

  return *insert;
}

/*
** 'new_symbol' is called to create a symbol that will be added to the
** low-level debugging tables
*/
static const symbol *
new_symbol (const char *name)
{
  if (opt_keepdebug)
    lldsize += strlen (name) + sizeof (char);
  return make_symbol (name, SYM_GLOBAL);
}

/*
** 'find_areasymbol' is called to try to find a globally defined symbol
** from the area 'ap'. It returns a pointer to a name or NULL if it
** cannot find anything
*/
const char *
find_areasymbol (arealist * ap)
{
  symtentry *sp;
  filelist *fp;
  const char *anp;
  unsigned int symcount, n;
  anp = ap->arname;
  fp = ap->arfileptr;
  sp = fp->obj.symtptr;
  symcount = fp->symtcount;
  for (n = 1;
       n <= symcount && (sp->symtarea.areaptr != ap
			 || (sp->symtattr & SYM_SCOPE) != SYM_GLOBAL); n++)
    sp++;
  if (n > symcount)
    return NULL;

  return sp->symtname;
}

/*
** 'find_nonstrong' is called to locate a non-strong version of the
** symbol passed to it, returning a pointer to the OBJ_SYMT entry
** of the non-strong symbol. This is needed to deal with strong
** symbols, when a relocation refers to the *definition* of a strong
** symbol in the file in which the strong symbol was defined. The
** relocation has to refer to the non-strong version, which would
** have been dragged in by the symbol resolution code.
*/
symtentry *
find_nonstrong (symtentry * stp)
{
  symbol dummysym;
  symtentry dummysymt;
  const char *name;
  symbol *sp;
  name = stp->symtname;
  dummysym.symhash = hash (name);
  dummysym.symtptr = &dummysymt;
  dummysymt.symtname = name;
  dummysymt.symtattr = 0;
  sp = search_global (&dummysym);
  if (sp == NULL)
    error
      ("Fatal: Could not find non-strong version of symbol '%s' in 'find_nonstrong'",
       name);
  return sp->symnormal;
}

/* The next few functions deal with the creation of the symbol listing */

static symtentry **symbinfotable;
static unsigned int symbcount;

/*
** 'build_symblist' builds a list of the symbols in the executable
** image. This is later sorted into address order and printed
*/
static void
build_symblist (void)
{
  filelist *fp;
  symtentry *sp;
  symtentry **sip;
  unsigned int n, count;
  symbcount = 0;

  sip = symbinfotable;
  for (fp = aofilelist; fp != NULL; fp = fp->nextfile)
    {
      count = fp->symtcount;
      for (n = 1, sp = fp->obj.symtptr; n <= count; ++n, ++sp)
	{
	  if ((sp->symtattr & (SYM_ABSVAL | SYM_DEFN)) == SYM_DEFN
	      && strcmp (sp->symtname, "__codeseg") != 0)
	    {
	      if ((sp->symtattr & SYM_ABSVAL) != 0
		  || (sp->symtarea.areaptr != NULL
		      && sp->symtarea.areaptr->arefcount != 0))
		{
		  *sip = sp;
		  sip++;
		  symbcount++;
		}
	    }
	}
    }

  if (imagetype != AOF)
    {	/* These symbols will not exist in a partially-linked file */
      *sip++ = image_robase->symtptr;
      *sip++ = image_rolimit->symtptr;
      *sip++ = image_rwbase->symtptr;
      *sip++ = image_rwlimit->symtptr;
      *sip++ = image_zibase->symtptr;
      *sip++ = image_zilimit->symtptr;
      symbcount += 6;
      if (got_oldlibs)
	{	/* Include symbols for old-style libraries */
	  *sip++ = image_codebase->symtptr;
	  *sip++ = image_codelimit->symtptr;
	  *sip++ = image_database->symtptr;
	  *sip++ = image_datalimit->symtptr;
	  symbcount += 4;
	}
    }
}

static void
print_symblist (void)
{
  symtentry **sip;
  unsigned int n;
  for (n = 1, sip = symbinfotable; n <= symbcount; ++n, ++sip)
    write_symbol (*sip);
}

/*
** 'symtcmp' compares the values of the two symbols passed to it
*/
static int
symtcmp (const void *first, const void *second)
{
  if (((*COERCE (first, symtentry **)))->symtvalue <
      ((*COERCE (second, symtentry **)))->symtvalue)
    return -1;
  if (((*COERCE (first, symtentry **)))->symtvalue >
      ((*COERCE (second, symtentry **)))->symtvalue)
    return 1;
  return 0;
}

/*
** 'print_symbols' is called to print a listing of the symbols found
** in the image file. These are listed by file.
*/
void
print_symbols (void)
{
  if ((symbinfotable = allocmem (totalsymbols * sizeof (symtentry *))) == NULL)
    {
      error ("Error: Cannot create symbol listing");
      return;
    }
  open_symbol ();
  build_symblist ();
  qsort (symbinfotable, symbcount, sizeof (symtentry *), symtcmp);
  print_symblist ();
  close_symbol ();
}

/*
** The next few functions deal with the processing needed for
** constructor and destructor lists generated by the Acorn C++
** compiler
*/

#define LINKNAME "__link"
#define HEADNAME "__head"

/*
** We must keep a list of all the locations that we have relocated so
** that we can write the relocations out in any table of relocations
** that we require for the output format. Typically this will be the
** Relocatable Module relocation table, however it could also be used
** for a Relocatable AIF. Partially linked AOF files are NOT supported
** by this mechanism at present. This will need to be addressed by
** someone clever, or at least who can put in the time to merge the
** symbol list into the relocations provided in the linked areas.
** For simplicity - not efficiency - this is implemented as a simple
** linked list. Someone with more time could change it to an extending
** array which might be more efficient.
*/
typedef struct linker_set_reloc_s linker_set_reloc_t;
struct linker_set_reloc_s {
  linker_set_reloc_t *next;
  symbol *sym;
};

static linker_set_reloc_t *linker_set_relocs = NULL;

/*
** 'build_lslist_named' will link all the symbols labelled with 'link_name'
** into a linked list. The head of the linked list will be stored at the
** 'head_name' symbol. This is a generalised form of the C++-constructor
** destructor list routine
*/
static void
build_lslist_named (const char *link_name, const char *head_name)
{
  symbol tempsym;
  symtentry tempsymt;
  unsigned int lastlink, offset;
  symbol *sp;
  symtentry *stp;
  arealist *ap;
  filelist *fp;

  tempsym.symhash = hash (link_name);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = link_name;
  tempsymt.symtattr = 0;
  lastlink = 0;
  for (fp = aofilelist; fp != NULL; fp = fp->nextfile)
    {
      /* file size = 0 means filelist entry is a dummy one */
      if (fp->chfilesize == 0)
	continue;

      current_table = &fp->localsyms;
      if ((sp = search_local (&tempsym)) != NULL)
	{
	  linker_set_reloc_t *lsreloc;

	  stp = sp->symtptr;
	  ap = stp->symtarea.areaptr;
	  offset = stp->symtvalue - ap->arplace;    /* Offset within area of __link */
	  *(ap->arobjdata + offset/sizeof(long)) = lastlink;	    /* ???? */

	  /*
	  ** We must remember the locations of the symbols so that they
	  ** can be written to a relocation list in the output.
	  */
	  if (lastlink != 0)
	    {
	      lsreloc = allocmem(sizeof(linker_set_reloc_t));
	      if (lsreloc == NULL)
		error ("Fatal: Out of memory in 'build_lslist_named' linking '%s'", link_name);
	      lsreloc->sym = sp;
	      lsreloc->next = linker_set_relocs;
	      linker_set_relocs = lsreloc;
	    }

	  lastlink = stp->symtvalue;
	}
    }
  if (lastlink == 0)
    {
      error ("Warning: No linkerset links found for head '%s'", head_name);
      return;
    }

  tempsym.symhash = hash (head_name);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = head_name;
  tempsymt.symtattr = 0;
  sp = search_global (&tempsym);
  if (sp != NULL)
    {	/* Just to prevent embarrassing problems if this is not really C++ */
      linker_set_reloc_t *lsreloc;

      stp = sp->symtptr;
      ap = stp->symtarea.areaptr;

      /* Verify this isn't in a NOINIT area.  */
      if ((ap->aratattr & ATT_NOINIT) != 0)
        error ("Error: Head of linkerset '%s' is located in zero initialised area '%s' in '%s'", head_name, ap->arname, ap->arfileptr->chfilename);

      offset = stp->symtvalue - ap->arplace;	/* Offset within area of __head */
      *(ap->arobjdata + offset/sizeof(long)) = lastlink;	/* ???? */

      /*
      ** We must remember the locations of the symbols so that they can be
      ** written to a relocation list in the output.
      */
      lsreloc = allocmem(sizeof(linker_set_reloc_t));
      if (lsreloc == NULL)
        error ("Fatal: Out of memory in 'build_lslist_named' linking '%s'", head_name);
      lsreloc->sym = sp;
      lsreloc->next = linker_set_relocs;
      linker_set_relocs = lsreloc;
    }
  else
    error ("Fatal: Program error, linkerset head '%s' can't be found back", head_name);
}

/*
** 'find_lsareas_named' is called to mark all areas that containing named
** linker set symbols as 'not deletable'.
*/
void
find_lsareas_named (const char *link_name, const char *head_name)
{
  symbol tempsym;
  symtentry tempsymt;
  symbol *sp;
  filelist *fp;
  tempsym.symhash = hash (link_name);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = link_name;
  for (fp = aofilelist; fp != NULL; fp = fp->nextfile)
    {
      if (fp->chfilesize != 0)
	{	/* file size = 0 means filelist entry is a dummy one */
	  current_table = &fp->localsyms;
	  if ((sp = search_local (&tempsym)) != NULL)
	    {	/* File contains a constructor/destructor */
	      mark_area (sp->symtptr->symtarea.areaptr);
	    }
	}
    }
  tempsym.symhash = hash (head_name);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = head_name;
  sp = search_global (&tempsym);
  if (sp != NULL)
    {	/* Just to prevent embarrassing problems if this is not really C++ */
      mark_area (sp->symtptr->symtarea.areaptr);
    }
}

/*
** 'write_lsreloc' will write the addresses of the symbols which require relocation
** to the image file. It is called by 'write_relocinfo' in c.areas. These are required
** for any relocatable image.
*/
void
write_lsreloc (void)
{
  linker_set_reloc_t *lsreloc;
  for (lsreloc = linker_set_relocs; lsreloc; lsreloc=lsreloc->next)
  {
    symbol *sym = lsreloc->sym;
    unsigned int value = sym->symtptr->symtvalue - progbase;

    convert_endian (&value, sizeof (value));
    write_image(&value, sizeof (value));
  }
}


/*
** 'build_cdlist' builds the constructor-destructor linked list
** that C++ programs contain. These are denoted by a local symbol in
** each AOF file with the name of '__link'. '__head' points at the
** first entry in the list.
*/
void
build_cdlist (void)
{
  if (opt_verbose)
    error ("Drlink: Performing C++ specific operations...");
  /*
  ** This routine has been simplified to use the generalised linker-set
  ** routine above.
  */
  build_lslist_named(LINKNAME, HEADNAME);
}

/*
** 'find_cdareas' is called to mark all areas that contain constructors
** or destructors as 'not deletable'
*/
void
find_cdareas (void)
{
  /*
  ** This routine has been simplified to use the generalised linker-set
  ** routine above.
  */
  find_lsareas_named(LINKNAME, HEADNAME);
}


/*
** 'search_lsprefix' is a helper function for the linker-set support. It
** searches a tree for all symbols in the given tree whose name is prefixed
** by a particular string and calls a function with their symbol.
*/
static void
search_lsprefix (symbol * tree, const char *prefix, void (*func)(symbol *p))
{
  int prefixlen = strlen(prefix);
  /* Note: We're only using a case-sensitive comparison here; this is
	   possibly wrong; maybe we should use the case flag for the
	   symbol? */
  int compval = strncmp (prefix, tree->symtptr->symtname, prefixlen);
  if (compval == 0)
    func(tree);

  if (tree->left)
    search_lsprefix(tree->left, prefix, func);
  if (tree->right)
    search_lsprefix(tree->right, prefix, func);
}


/*
** A '__head' symbol has been found in the global symbol table. We must
** link all the same-named '__link' symbols to it.
*/
static void
build_lslist_symbol(symbol *p)
{
  const char *headsuffix = p->symtptr->symtname + sizeof(HEADNAME)-1;
  int namelen = strlen(headsuffix) + sizeof(LINKNAME);
  char *linkname;

  if (*headsuffix == '\0')
    return; /* The C++ linker sets will have already been dealt with */

  /* Don't trigger on __head*$$Base or __head*$$Limit, even more, when
     suffix starts with "$$", don't consider using this.  */
  if (headsuffix[0] == '$' && headsuffix[1] == '$')
    return;

  linkname = allocmem (namelen);
  if (linkname == NULL)
    error ("Fatal: Out of memory in 'build_lslist_symbol'");

  /* Construct the link name that we associate with this head */
  strcpy(linkname, LINKNAME);
  strcpy(linkname + sizeof(LINKNAME)-1, headsuffix);

  build_lslist_named(linkname, p->symtptr->symtname);

  freemem (linkname, namelen);
}


/*
** 'build_lslist' builds a linker-set in a similar way to the
** constructor-destructor linked list that C++ programs contain.
** These are denoted by a local symbol in each AOF file with the prefix
** of '__link'. A symbol prefixed by '__head' points at the first entry in
** the list.
*/
void
build_lslist (void)
{
  int hashval;
  if (opt_verbose)
    error ("Drlink: Performing linker-set specific operations...");
  for (hashval = 0; hashval<MAXGLOBALS; hashval++)
    {
      symbol *p = globalsyms[hashval];
      if (p)
        search_lsprefix(p, HEADNAME, build_lslist_symbol);
    }
}


/*
** A '__head' symbol has been found in the global symbol table. We must
** make all the symbols associated with it as being in areas which are to
** not be deleted.
*/
static void
find_lsareas_symbol(symbol *p)
{
  const char *headsuffix = p->symtptr->symtname + sizeof(HEADNAME)-1;
  int namelen = strlen(headsuffix) + sizeof(LINKNAME);
  char *linkname;

  if (*headsuffix == '\0')
    return; /* The C++ linker sets will have already been dealt with */

  /* Don't trigger on __head*$$Base or __head*$$Limit, even more, when
     suffix starts with "$$", don't consider using this.  */
  if (headsuffix[0] == '$' && headsuffix[1] == '$')
    return;

  linkname = allocmem (namelen);
  if (linkname == NULL)
    error ("Fatal: Out of memory in 'find_lsareas_symbol'");

  /* Construct the link name that we associate with this head */
  strcpy(linkname, LINKNAME);
  strcpy(linkname + sizeof(LINKNAME)-1, headsuffix);

  find_lsareas_named(linkname, p->symtptr->symtname);

  freemem (linkname, namelen);
}


/*
** 'find_lsareas' is called to mark all areas that contain linker-sets
** as 'not deletable'.
*/
void
find_lsareas (void)
{
  int hashval;
  for (hashval = 0; hashval<MAXGLOBALS; hashval++)
  {
    symbol *p;
    p = globalsyms[hashval];
    if (p)
      search_lsprefix(p, HEADNAME, find_lsareas_symbol);
  }
}


/*
** 'init_symbols' is called at the start of the link to initialise
** the global symbol table and other stuff to do with symbols
*/
void
init_symbols (void)
{
  int i;
  for (i = 0; i < MAXGLOBALS; i++)
    globalsyms[i] = NULL;
  for (i = 0; i < MAXCOMMON; i++)
    commonsyms[i] = NULL;
  wantedlist = NULL;
  totalsymbols = numwanted = lldsize = 0;
}

/*
** 'create_linksyms' is called to add the standard linker
** symbols to the symbol table, unless the image file being
** created is a partially-linked AOF file in which case
** the symbols have to be left out as it is important that
** references to them are not resolved at this stage
*/
void
create_linksyms (void)
{
  if (imagetype != AOF)
    {
      image_robase = new_symbol ("Image$$RO$$Base");
      image_rolimit = new_symbol ("Image$$RO$$Limit");
      image_rwbase = new_symbol ("Image$$RW$$Base");
      image_rwlimit = new_symbol ("Image$$RW$$Limit");
      image_zibase = new_symbol ("Image$$ZI$$Base");
      image_zilimit = new_symbol ("Image$$ZI$$Limit");
      image_codebase = new_symbol ("Image$$CodeBase");
      image_codelimit = new_symbol ("Image$$CodeLimit");
      image_database = new_symbol ("Image$$DataBase");
      image_datalimit = new_symbol ("Image$$DataLimit");
      reloc_code = new_symbol ("__RelocCode");
    }
  else
    {
      image_robase = image_rolimit = image_rwbase = image_rwlimit = NULL;
      image_zibase = image_zilimit = reloc_code = NULL;
      image_codebase = image_codelimit = image_database = image_datalimit =
	NULL;
    }
}
