/*
** Drlink AOF linker - AOF File creation
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This module contains the routines concerned with the creation
** of a partially-linked AOF file
**
** The linker creates AOF version 2 AOF files in that it puts
** (or more accurately, leaves) type-1 relocations in the AOF
** file it creates. This could be fixed, but the code is
** horrible enough as it is.
*/

#include <stdio.h>
#include <string.h>
#include "drlhdr.h"
#include "filehdr.h"
#include "areahdr.h"
#include "chunkhdr.h"
#include "procdefs.h"

/* Private declarations */

typedef struct newhead {
  int headhash;				/* Hashed version of area name */
  char *headname;			/* Pointer to name of area */
  unsigned int headattr;		/* Area's attributes */
  struct newhead *headflink;		/* Next area in list */
} newhead;

typedef struct newsymt {
  int newhash;				/* Symbol's hash value */
  symtentry *newsymtptr;		/* Pointer to old OBJ_SYMT entry */
  unsigned int newindex;		/* New OBJ_SYMT index */
  struct newsymt *newnext;		/* Next entry in order of creation */
  struct newsymt *newflink;		/* Next entry in list */
 } newsymt;

typedef struct strtentry {
  char *strtname;			/* Pointer to name */
  int strthash;				/* Name's hash value */
  unsigned int strtoffset;		/* Name's offset within the OBJ_STRT chunk */
  struct strtentry *strtnext;		/* Next entry in order of creation */
  struct strtentry *strtflink;		/* Next name in list */
} strtentry;

#define STRTENTRIES 16			/* Number of STRT hash chains (Must be power of 2) */
#define STRTMASK (STRTENTRIES-1)

#define MAXCHUNKS 5			/* Space in chunk header */
#define NUMCHUNKS 5			/* Entries used in chunk header */

static unsigned int
  newsymcount,				/* Initial value of new SYMT index in each AOF file */
  newareacount,				/* Number of areas in new AOF file */

  file_offset,				/* Current offset in file */
  newentryarea,				/* Area number of entry point */
  head_start, head_size,		/* Start and size of OBJ_HEAD */
  area_start, area_size,		/* Start and size of OBJ_AREA */
  idfn_start, idfn_size,		/* Start and size of OBJ_IDFN */
  symt_start, symt_size,		/* Start and size of OBJ_SYMT */
  strt_start, strt_size;		/* Start and size of OBJ_STRT */

static strtentry *strtable[STRTENTRIES];
static strtentry *strtlist, *strtlast;

static newsymt *newsymtable[MAXGLOBALS];
static newsymt *symtlist, *symtlast;

static newhead *headlist, *headlast;

#define IDFNSTRING "Drlink AOF Linker Version "

/*
** 'init_aofile' initialises things concerning the AOF file
*/
void init_aofile(void) {
  int n;
  head_start = head_size = 0;
  area_start = area_size = 0;
  idfn_start = idfn_size = 0;
  symt_start = symt_size = 0;
  strt_start = 0;
  file_offset = 0;
  newsymcount = 0;
  newareacount = 0;
  newentryarea = 0;
  strtlist = strtlast = NIL;
  symtlist = symtlast = NIL;
  headlist = headlast = NIL;
  for (n = 0; n<STRTENTRIES; n++) strtable[n] = NIL;
  strt_size = sizeof(unsigned int);
  for (n = 0; n<MAXGLOBALS; n++) newsymtable[n] = NIL;
}

/*
** 'addto_strt' adds a name to the OBJ_STRT chunk if it is not already
** present, returning the offset of the entry.
** Note that the STRT strings are linked in two ways. Firstly, they
** are kept in the usual hash table to speed up searching and, secondly,
** they are linked together in the order they are created for when
** the actual OBJ_STRT is built.
*/
static unsigned int addto_strt(char *name) {
  strtentry *p;
  int hashval;
  hashval = hash(name);
  p = strtable[hashval & STRTMASK];
  while (p!=NIL && (hashval!=p->strthash || strcmp(name, p->strtname)!=0)) p = p->strtflink;
  if (p==NIL) {		/* New name. Create STRT entry for it */
    if ((p = allocmem(sizeof(strtentry)))==NIL) error("Fatal: Out of memory in 'addto_strt'");
    p->strtname = name;
    p->strthash = hashval;
    p->strtoffset = strt_size;
    p->strtnext = NIL;
    p->strtflink = strtable[hashval & STRTMASK];
    strtable[hashval & STRTMASK] = p;
    if (strtlast==NIL) {
      strtlist = p;
    }
    else {
      strtlast->strtnext = p;
    }
    strtlast = p;
    strt_size+=strlen(name)+sizeof(char);
  }
  return p->strtoffset;
}

/*
** 'reloc_aofarlist' goes through the list of areas passed to it
** and calculates where each area will be in the 'super areas' in
** the final AOF file. 'arplace' is set to the offset within the
** area. This value is added later to the value of all non-absolute
** symbols in the OBJ_SYMT chunk that lie in these areas. It also
** builds a linked list of the new areas used to determine the
** areas' index in the OBJ_HEAD chunk. The routine also modifies
** the entry point area and offset if there is an entry point in
** the AOF files to be included
*/
static void reloc_aofarlist(arealist *ap) {
  arealist *firstarea;
  newhead *hp;

  firstarea = NIL;
  while (ap!=NIL) {
    unsigned int areabase;
    if (ap->arbase!=firstarea) {	/* Area name differs from last one */
      firstarea = ap->arbase;
      newareacount+=1;
      areabase = 0;
      if ((hp = allocmem(sizeof(newhead)))==NIL) error("Fatal: Out of memory in 'reloc_aofarlist'");
      hp->headhash = firstarea->arhash;
      hp->headname = firstarea->arname;
      hp->headattr = firstarea->aratattr;
      hp->headflink = NIL;
      if (headlast==NIL) {
        headlist = hp;
      }
      else {
        headlast->headflink = hp;
      }
      headlast = hp;

      ap->arplace = areabase;
      if (ap==entryarea) {	/* Note new entry area if necessary */
	newentryarea = newareacount;
	entryoffset+=areabase;
      }
      areabase+=ap->arobjsize;
    }
    ap = ap->arflink;
  }
}

/*
** 'reloc_aofareas' goes through the area lists and merging
** and recalculating the sizes of areas with the same name.
** It also calculates the size of the new OBJ_HEAD chunk as
** this is known at this point.
*/
static void reloc_aofareas() {
  reloc_aofarlist(rocodelist);
  reloc_aofarlist(rodatalist);
  reloc_aofarlist(rwcodelist);
  reloc_aofarlist(rwdatalist);
  reloc_aofarlist(zidatalist);
  reloc_aofarlist(debuglist);
  head_size = sizeof(aofheader)+newareacount*sizeof(areaentry);
}

/*
** 'find_areaindex' finds the area index of the area passed to
** it in the OBJ_HEAD chunk of the new AOF file
*/
unsigned int find_areaindex(arealist *ap) {
  newhead *hp;
  unsigned int index;
  int hash;
  char *name;
  unsigned int attr;
  name = ap->arname;
  attr = ap->aratattr;
  hash = ap->arhash;
  hp = headlist;
  index = 0;
  while (hp!=NIL && (attr!=hp->headattr || hash!=hp->headhash || strcmp(name,hp->headname)!=0)) {
    hp = hp->headflink;
    index+=1;
  }
  if (hp==NIL) error("Fatal: Could not find area '%s' in 'find_areaindex'", name);
  return index;
}

/*
** 'compact_reloclist' is called to modify the relocation info
** for each area, changing the offset of each relocation to
** its new offset in the 'super area', changing the SYMT index
** of each relocation to its new value and compressing the
** relocation data to get rid of any relocations that have
** been carried out, that is, PC-relative ones within the
** 'super area'.
*/
static void compact_reloclist(arealist *ap) {
  relocation *oldrp, *newrp;
  unsigned int n, oldcount, newcount, offset, typesym, reltype;
  indextable *newindex;
  while (ap!=NIL) {
    newindex = ap->arfileptr->symtries.symtlookup;
    newrp = oldrp = ap->areldata;
    oldcount = ap->arnumrelocs;
    newcount = 0;
    offset = ap->arplace;
    for (n = 1; n<=oldcount; n++) {
      if (oldrp->reloffset!=ALLFS) {	/* Relocation that still needs doing */
        newrp->reloffset = oldrp->reloffset+offset;
        typesym = oldrp->reltypesym;
        if ((typesym & REL_TYPE2)==0) {	/* Type 1 relocation (16 bit SYMT index) */
          reltype = get_type1_type(typesym);
          if ((reltype & (REL_PC|REL_SYM))!=0) {
            newrp->reltypesym = (reltype<<16)+*newindex[get_type1_index(typesym)];
          }
          else {
            newrp->reltypesym = oldrp->reltypesym;
          }
        }
        else {	/* Type 2 relocation (24 bit SYMT index) */
          reltype = get_type2_type(typesym);
          if ((reltype & REL_SYM)!=0) {
            newrp->reltypesym = (reltype<<24)+*newindex[get_type2_index(typesym)];
          }
          else {
            newrp->reltypesym = oldrp->reltypesym;
          }
        }
        newrp++;
        newcount+=1;
      }
      oldrp++;
    }
    ap->arnumrelocs = newcount;
    if ((ap->aratattr & ATT_NOINIT)==0) {	/* Area will appear in AOF file */
      area_size+=ap->arobjsize+newcount*sizeof(relocation);
    }
    ap = ap->arflink;
  }
}

/*
** 'compact_relocs' is called to modify the relocation info in all of
** the areas in the AOF file
*/
static void compact_relocs(void) {
  compact_reloclist(rocodelist);
  compact_reloclist(rodatalist);
  compact_reloclist(rwcodelist);
  compact_reloclist(rwdatalist);
  compact_reloclist(debuglist);
}

/*
** 'build_symt' is called to compact the OBJ_SYMT of all the
** OBJ_SYMT chunks in the AOF file list by weeding out all the
** 'dead' entries, that is, external references that have been
** resolved and local variables. It creates tables for each
** file giving the mapping of old SYMT indexes to new ones. At
** the end, 'newsymcount' contains the number of SYMT entries the
** new AOF file will contain. The function also calculates the size
** of the new OBJ_SYMT chunk
*/
static void build_symt(void) {
  unsigned int oldcount, newcount, n, attr;
  indextable *indexlookup;
  symtentry *newsp, *oldsp;
  filelist *fp;
  newsymcount = 0;
  fp = aofilelist;
  do {
    if (fp->chfilesize!=0) {	/* Not a dummy entry in the list */
      oldcount = fp->symtcount;
      if ((indexlookup = allocmem(oldcount*sizeof(unsigned int)))==NIL) {
        error("Fatal: Out of memory in 'build_symt'");
      }
      oldsp = newsp = fp->objsymtptr;
      newcount = 0;
      for (n = 1; n<=oldcount; n++) {
        attr = oldsp->symtattr;
        if ((attr & SYM_COMMON)!=0) {	/* Ref to common block: convert back to unresolved ref */
/* Fetch length from CB definition's area's entry */
          oldsp->symtvalue = oldsp->symtarea.symdefptr->symtarea.areaptr->arobjsize;
          oldsp->symtarea.symdefptr = NIL;
        }
        if ((attr & SYM_SCOPE)==SYM_LOCAL || ((attr & SYM_DEFN)==0 && oldsp->symtarea.symdefptr!=NIL)) {
/* Entry not needed */
          *indexlookup[n-1] = ALLFS;
        }
        else {
          *newsp = *oldsp;
          newsp->symtname = COERCE(addto_strt(newsp->symtname), char *);
          if ((attr & (SYM_DEFN|SYM_ABSVAL))==SYM_DEFN) {	/* Def'n of symbol */
            newsp->symtarea.areaname = addto_strt(newsp->symtarea.areaptr->arname);
          }
          else {
            newsp->symtarea.areaname = 0;
          }
          *indexlookup[n-1] = newsymcount+newcount;
          newcount+=1;
          newsp++;
        }
        oldsp++;
      }
      fp->symtcount = newcount;
      fp->symtries.symtlookup = indexlookup;
      newsymcount+=newcount;
    }
    fp = fp->nextfile;
  } while (fp!=NIL);
  symt_size = newsymcount*sizeof(symtentry);
}

/*
** 'write_index' is called to create an entry in the AOF file's
** chunk header
*/
static void write_index(unsigned int ctype, unsigned int offset, unsigned int size) {
  chunkindex indexentry;
  indexentry.chunkclass = OBJ_XXXX;
  indexentry.chunktype = ctype;
  indexentry.chunkoffset = offset;
  indexentry.chunksize = size;
  write_image(&indexentry, sizeof(chunkindex));
}

/*
** 'start_aofile' is called to create the AOF file itself and
** to set up the initial version of the chunk file header
*/
static void start_aofile(void) {
  chunkheader header;
  idfn_size = align(strlen(IDFNSTRING)+strlen(VERSION)+sizeof(char));
  imagesize = sizeof(chunkheader)+MAXCHUNKS*sizeof(chunkindex)+
   head_size+idfn_size+area_size+symt_size+strt_size;
  open_image();
  header.chunkfileid = CHUNKFILE;
  header.maxchunks = header.numchunks = MAXCHUNKS;
  write_image(&header, sizeof(header));
  write_index(OBJ_HEAD, 0, 0);
  write_index(OBJ_IDFN, 0, 0);
  write_index(OBJ_AREA, 0, 0);
  write_index(OBJ_SYMT, 0, 0);
  write_index(OBJ_STRT, 0, 0);
  file_offset = sizeof(chunkheader)+MAXCHUNKS*sizeof(chunkindex);
}

/*
** 'write_objhead' creates the entries for the new OBJ_HEAD chunk
** from a particular area list, writing them directly to the AOF
** file
*/
static void write_objhead(arealist *ap) {
  arealist *firstarea;
  unsigned int relco, areasize;
  areaentry entry;
  if (ap==NIL) return;
  firstarea = ap->arbase;
  do {
    areasize = relco = 0;
    while (ap!=NIL && ap->arbase==firstarea) {
      areasize+=ap->arobjsize;
      relco+=ap->arnumrelocs;
      ap = ap->arflink;
    }
    entry.areaname = addto_strt(firstarea->arname);
    entry.attributes = (firstarea->aratattr<<8)+firstarea->aralign;
    entry.arsize = areasize;
    entry.arelocs = relco;
    entry.arlast.arzero = 0;
    write_image(&entry, sizeof(entry));
    if (ap!=NIL) firstarea = ap->arbase;
  } while (ap!=NIL);
}

/*
** 'create_objhead' creates the new OBJ_HEAD chunk to describe all
** the new areas
*/
static void create_objhead(void) {
  aofheader header;
  header.oftype = OBJFILETYPE;
  header.aofversion = AOFVER2;
  header.numareas = newareacount;
  header.numsymbols = newsymcount;
  header.eparea = newentryarea;
  header.epoffset = entryoffset;
  write_image(&header, sizeof(aofheader));
  write_objhead(rocodelist);
  write_objhead(rodatalist);
  write_objhead(rwcodelist);
  write_objhead(rwdatalist);
  write_objhead(zidatalist);
  write_objhead(debuglist);
  head_start = file_offset;
  file_offset+=head_size;
}

/*
** 'create_objidfn' is called to create the OBJ_IDFN chunk.
** Note that hack where the string is copied to a buffer
** to ensure that it is word aligned for 'write_image' to
** play with under RISCOS
*/
static void create_objidfn(void) {
  int n;
  unsigned int buffer[25];
  for (n = 0; n<25; n++) buffer[n] = 0;
  strcpy(COERCE(&buffer, char *), IDFNSTRING);
  strcat(COERCE(&buffer, char *), VERSION);
  write_image(&buffer, idfn_size);
  idfn_start = file_offset;
  file_offset+=idfn_size;
}

/*
** 'write_objarea' writes out the code and data areas in the list
** passed to it. It goes through the list writing out all the
** areas with the same name and then going back to write out
** all the relocation data associated with those areas.
*/
static void write_objarea(arealist *ap) {
  arealist *first, *newfirst;
  if (ap==NIL) return;
  first = ap->arbase;
  do {
    while (ap!=NIL && ap->arbase==first) {
      write_image(ap->arobjdata, ap->arobjsize);
      ap = ap->arflink;
    }
    newfirst = (ap!=NIL ? ap->arbase : NIL);
    while (first!=NIL && first!=newfirst) {
      write_image(first->areldata, first->arnumrelocs*sizeof(relocation));
      first = first->arflink;
    }
    first = newfirst;
  } while (ap!=NIL);
}

/*
** 'create_objarea' is the biggie. It creates the OBJ_AREA chunk
** in the AOF file
*/
static void create_objarea(void) {
  write_objarea(rocodelist);
  write_objarea(rodatalist);
  write_objarea(rwcodelist);
  write_objarea(rwdatalist);
  write_objarea(debuglist);
  area_start = file_offset;
  file_offset+=area_size;
}

/*
** 'create_objsymt' is called to create the AOF file's OBJ_SYMT
** chunk
*/
static void create_objsymt(void) {
  filelist *fp;
  fp = aofilelist;
  do {
    if (fp->chfilesize!=0) write_image(fp->objsymtptr, fp->symtcount*sizeof(symtentry));
    fp = fp->nextfile;
  } while (fp!=NIL);
  symt_start = file_offset;
  file_offset+=symt_size;
}

/*
** 'create_objstrt' is called to create the AOF file's OBJ_STRT
** chunk
*/
static void create_objstrt(void) {
  strtentry *p;
  p = strtlist;
  write_image(&strt_size, sizeof(unsigned int));
  while (p!=NIL) {
    write_string(p->strtname);
    p = p->strtnext;
  }
  strt_start = file_offset;
  file_offset+=strt_size;
}

/*
** 'finish_aof' is called to complete the AOF file. It resets
** the file pointer to the start of the chunk index and then
** fills in the index
*/
static void finish_aofile(void) {
  reset_image(sizeof(chunkheader));
  write_index(OBJ_HEAD, head_start, head_size);
  write_index(OBJ_IDFN, idfn_start, idfn_size);
  write_index(OBJ_AREA, area_start, area_size);
  write_index(OBJ_SYMT, symt_start, symt_size);
  write_index(OBJ_STRT, strt_start, strt_size);
  close_image();
}

void create_aofile(void) {
  init_aofile();
  reloc_aofareas();
  relocate_symbols();
  if (!fixup_relocs()) return;
  build_symt();
  compact_relocs();
  if (opt_verbose) error("Drlink: Creating partially-linked AOF file '%s'...", imagename);
  start_aofile();
  create_objidfn();
  create_objhead();
  create_objarea();
  create_objsymt();
  create_objstrt();
  finish_aofile();
}

