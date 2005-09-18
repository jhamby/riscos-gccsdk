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
** This file contains the procedures that handle the creation of the
** debug tables, both the low-level and compiler generated types
*/

#include "config.h"

#include <string.h>

#include "drlhdr.h"
#include "symbolhdr.h"
#include "areahdr.h"
#include "procdefs.h"

/* Private declarations */

typedef struct
{
  unsigned int lldrecinfo;	/* Record type/size */
  unsigned int lldflags;	/* Flag to say what record contains */
  unsigned int lldrostart;	/* Address of start of program code */
  unsigned int lldrwstart;	/* Address of start of read/write data */
  unsigned int lldrosize;	/* Size of read-only part */
  unsigned int lldrwsize;	/* Size of read/write part */
  unsigned int lldanon;		/* ?? */
  unsigned int lldbgsize;	/* Size of this debug part */
  unsigned int lldcount;	/* Count of symbols in low level tables */
} lldheader;

typedef struct
{
  unsigned int lldoffset;	/* Entry type and offset of name in table */
  unsigned int lldvalue;	/* Symbol's value */
} lldinfo;

/* Entry types in low level debug tables */

#define LLD_SPECIAL  0x01000000	/* Linker symbols */
#define LLD_ROAREA   0x02000000	/* Read-only area name */
#define LLD_ROSYMBOL 0x03000000	/* Symbol in read-only part of image */
#define LLD_RWAREA   0x04000000	/* Read/write area name */
#define LLD_RWSYMBOL 0x05000000	/* Symbol in read/write area */
#define LLD_RWDATA   0x06000000	/* Symbol in read/write data area */
#define LLD_COMMON   0x07000000	/* Common block name */

#define LLD_FLAGS 0x02000000	/* Flag values at start of debug area */
int lldentco,			/* Count of entries in low level tables */
  lldnamesize;			/* Size of low level name table */

lldheader lldhdr;

static lldinfo *lldinfotable, *lldinfonext;	/* Entries to go in low level table */
static char *lldnametable, *lldnamenext;	/* Text of names to go in low level table */

/*
** 'nullfill' pads the character string at 'p' to a whole
** number of words long, returning the updated pointer
*/

#define ALIGNBITS (sizeof(int)-1)

static char *
nullfill (char *p)
{
  while ((COERCE (p, int) & ALIGNBITS) != 0)
    {
      *p = '\0';
      p++;
    }
  return p;
}

/*
** 'add_lldentry' adds one entry to the low level debugging tables
*/
static void
add_lldentry (const char *name, unsigned int class, int value)
{
  strcpy (lldnamenext, name);
  lldinfonext->lldoffset = class + (lldnamenext - lldnametable);
  lldinfonext->lldvalue = value;
  lldinfonext++;
  lldnamenext += strlen (name) + sizeof (char);
  lldentco += 1;
}

static void
add_lldsymbol (const symbol * sp)
{
  symtentry *stp;
  stp = sp->symtptr;
  add_lldentry (stp->symtname, LLD_SPECIAL, stp->symtvalue);
}

/*
** 'gen_llsymt' goes through the OBJ_SYMT area of a entry
** in the file list and generates low-level debug entries for
** each symbol defined there. Both local and global symbols
** are included, but any that start 'x$' are excluded as they
** just clutter up the table. This is, admittedly, a bit of
** a kludge. Symbols which have no areas associated with them,
** which are absolute or are defined in areas where the count
** of references to the area is zero are also omitted.
*/
static void
gen_llsymt (filelist * fp)
{
  symtentry *sp;
  int n, count;
  unsigned int lldclass;
  unsigned char areattr;
  sp = fp->obj.symtptr;
  count = fp->symtcount;
  for (n = 1; n <= count; n++)
    {
      if ((sp->symtattr & (SYM_DEFN | SYM_ABSVAL)) == SYM_DEFN &&
	  strncmp (sp->symtname, "x$", 2) != 0
	  && strcmp (sp->symtname, "__codeseg") != 0)
	{
	  if (sp->symtarea.areaptr != NULL
	      && sp->symtarea.areaptr->arefcount != 0)
	    {
	      areattr = sp->symtarea.areaptr->aratattr;
	      lldclass =
		((areattr & ATT_RDONLY) != 0 ? LLD_ROSYMBOL : LLD_RWSYMBOL);
	      add_lldentry (sp->symtname, lldclass, sp->symtvalue);
	    }
	}
      sp++;
    }
}

/*
** 'fillin_lldhdr' completes the header record of the low-level
** debug table
*/
static void
fillin_lldhdr (void)
{
  unsigned int robase, rwbase;
  int tablesize;
  tablesize = lldnamenext - lldnametable;
  *(COERCE (lldnametable, int *)) = tablesize;
  robase = image_robase->symtptr->symtvalue;
  rwbase = image_rwbase->symtptr->symtvalue;
  debugsize += sizeof (lldheader) + lldentco * sizeof (lldinfo) + tablesize;
  lldhdr.lldrecinfo = (sizeof (lldheader) << 16) + 1;
  lldhdr.lldflags = LLD_FLAGS;
  lldhdr.lldrostart = robase;
  lldhdr.lldrwstart = rwbase;
  lldhdr.lldrosize = image_rolimit->symtptr->symtvalue - robase;
  lldhdr.lldrwsize =
    image_rwlimit->symtptr->symtvalue - rwbase -
    (image_zilimit->symtptr->symtvalue - image_zibase->symtptr->symtvalue);
  lldhdr.lldanon = 0;
  lldhdr.lldbgsize = debugsize;
  lldhdr.lldcount = lldentco;
}

/*
** 'write_lldtable' writes the completed debug table to the image
** file. Any errors that occur at this stage will be fatal, so
** it does not return anything to signify that it worked.
*/
void
write_lldtable (void)
{
  convert_endian (&lldhdr, sizeof (lldhdr) / 4);
  write_image (&lldhdr, sizeof (lldhdr));
  convert_endian (lldinfotable, lldentco * sizeof (lldinfo) / 4);
  write_image (lldinfotable, lldentco * sizeof (lldinfo));
  convert_endian (lldnametable, (lldnamenext - lldnametable) / 4);
  write_image (lldnametable, lldnamenext - lldnametable);
  freemem (lldnametable, lldsize);
  freemem (lldinfotable, totalsymbols * sizeof (lldinfo));
}

/*
** 'gen_lowlevel' handles the creation of the linker low-level
** debug tables. It returns 'TRUE' if this worked okay otherwise
** it returns 'FALSE'
*/
static void
add_common (arealist * ap)
{
  if (ap != NULL)
    {
      add_common (ap->left);

      if ((ap->aratattr & (ATT_COMMON | ATT_COMDEF)) != 0)
	add_lldentry (ap->arname, LLD_COMMON, ap->arplace);

      add_common (ap->right);
    }
}


static bool
gen_lowlevel (void)
{
  filelist *fp;
  lldinfotable = lldinfonext = allocmem (totalsymbols * sizeof (lldinfo));
  lldsize = align (lldsize);
  lldnametable = lldnamenext = allocmem (lldsize);
  if (lldinfotable == NULL || lldnametable == NULL)
    {
      error
	("Error: Not enough memory to construct low level debugging tables");
      return FALSE;
    }
  lldnamenext += sizeof (int);	/* First four bytes contains the table length */
  lldentco = 0;
  for (fp = aofilelist; fp != NULL; fp = fp->nextfile)
    {
      if (fp->chfilesize != 0 && fp->keepdebug)
	gen_llsymt (fp);
    }

  add_common (zidatalist);	/* Now add common blocks to table */

  add_lldsymbol (image_robase);
  add_lldsymbol (image_rolimit);
  add_lldsymbol (image_rwbase);
  add_lldsymbol (image_rwlimit);
  add_lldsymbol (image_zibase);
  add_lldsymbol (image_zilimit);
  if (got_oldlibs)
    {				/* Include old symbols */
      add_lldsymbol (image_codebase);
      add_lldsymbol (image_codelimit);
      add_lldsymbol (image_database);
      add_lldsymbol (image_datalimit);
    }
  lldnamenext = nullfill (lldnamenext);
  fillin_lldhdr ();
  return TRUE;
}

/*
** 'build_debugtables' is called to create the low-level debugging
** tables added to the image file
*/
bool
build_debugtables (void)
{
  bool ok;
  ok = gen_lowlevel ();
  imagesize += debugsize;
  if (imagetype == RELOC)
    relocaddr += debugsize;
  return ok;
}
