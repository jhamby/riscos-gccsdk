/*
 * List symbols of an AOF file.
 *
 * Copyright (c) 2002 Nick Burrett
 * Copyright (c) 2003, 2004, 2005 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "libiberty.h"
#include "demangle.h"

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1
#endif
#define NM_VERSION "1.01"

static void nm_help (void);

static int opt_print_file_name = 0;
static int opt_debug_syms = 0;
static int opt_demangle = 0;
static int opt_dynamic = 0;
static int opt_extern_only = 1;
static int opt_numeric_sort = 0;
static int opt_no_sort = 0;
static int opt_portability = 0;
static int opt_print_armap = 0;
static int opt_reverse_sort = 0;
static int opt_size_sort = 0;
static int opt_undefined_only = 0;

typedef enum
{
  FNOERR,
  FEND,
  FRWERR
} Ferror;

/*
 * Chunk file / AOF structures
 */

struct chunkhdr
  {
    unsigned long chunkfileid;
    unsigned long maxchunks;
    unsigned long numchunks;
  };

struct chunkent
  {
    char chunkid[8];
    unsigned long offset;
    unsigned long size;
  };

struct aofhdr
  {				/* fixed part of AOF header only */
    unsigned long filetype;
    unsigned long version;
    unsigned long numareas;
    unsigned long numsyms;
    unsigned long entryarea;
    unsigned long entryoffset;
  };

#if defined(LITTLE_ENDIAN)
struct areahdr_flags
  {
    unsigned char AL;
    unsigned char AT;
    unsigned short zeroes;
  };
#else /* big endian assumed */
struct areahdr_flags
  {
    unsigned short zeroes;
    unsigned char AT;
    unsigned char AL;
  };
#endif

struct areahdr
  {
    unsigned long name;
    unsigned long flags;
    unsigned long size;
    unsigned long numrelocs;
    unsigned long reserved;
  };

/*
 * area attributes (areahdr->flags.AT)
 */
#define AT_CODE		(1<<1)
#define AT_COMMDEF	(1<<2)
#define AT_NOINIT	(1<<4)
#define AT_READONLY	(1<<5)
#define AT_DEBUG	(1<<7)

struct reloc
  {
    unsigned long offset;
    unsigned long flags;			/* varies depending on type */
  };

struct symbol
  {
    unsigned long name;
    unsigned long flags;
    unsigned long value;
    unsigned long areaname;
  };


/*
 * check for EOF or write/read errors on stream.
 */
static Ferror
check_stream (FILE *fp)
{
  int ret = FNOERR;

  if (feof (fp))
    ret = FEND;
  else if (ferror (fp))
    ret = FRWERR;
  if (ret != FNOERR)
    clearerr (fp);
  return (ret);
}

#ifndef LITTLE_ENDIAN
/*
 * read a byte from the input stream.
 */
static unsigned char
read_byte (FILE *ifp)
{
  return (unsigned char) getc (ifp);
}
#endif

#if 0
/*
 * read a little-endian 2-byte halfword from the input stream.
 */
static unsigned short
read_halfword (FILE *ifp)
{
  union
    {
      unsigned short h;
      unsigned char b[sizeof (unsigned short)];
    }
  ret;

#if defined(LITTLE_ENDIAN)
  fread ((char *) &ret.h, 1, sizeof (unsigned short), ifp);
#else
  ret.b[HALFWORD0] = read_byte (ifp);
  ret.b[HALFWORD1] = read_byte (ifp);
#endif
  return (ret.h);
}
#endif

/*
 * read a little-endian 4-byte word from the input stream.
 */
static unsigned long
read_word (FILE *ifp)
{
  union
    {
      unsigned long w;
      unsigned char b[sizeof (unsigned long)];
    }
  ret;

#if defined(LITTLE_ENDIAN)
  fread ((char *) &ret.w, 1, sizeof (unsigned long), ifp);
#else
  ret.b[WORD0] = read_byte (ifp);
  ret.b[WORD1] = read_byte (ifp);
  ret.b[WORD2] = read_byte (ifp);
  ret.b[WORD3] = read_byte (ifp);
#endif
  return (ret.w);
}

/*
 * read in the chunk header
 */
static struct chunkhdr *
read_chunkhdr (FILE *ifp)
{
  static struct chunkhdr hdr;

  fseek (ifp, 0L, 0);
  hdr.chunkfileid = read_word (ifp);
  hdr.maxchunks = read_word (ifp);
  hdr.numchunks = read_word (ifp);
  return (check_stream (ifp) != FRWERR ? &hdr : NULL);
}

/*
 * memory pointers maintained by read_xxx functions
 */

static struct chunkent *ents;	/* chunk file entries */
static char *strptr;		/* string table */
static struct symbol *symptr;	/* symbol table */
static struct aofhdr *aofhdr;	/* AOF header */

/*
 * read in the chunk entries
 */
static struct chunkent *
read_chunkents (FILE *ifp, struct chunkhdr *hdr)
{
  unsigned int i;

  if (ents)
    free (ents);
  ents = xmalloc (sizeof (struct chunkent) * hdr->maxchunks);

  fseek (ifp, sizeof (struct chunkhdr), 0);
  for (i = 0; i < hdr->numchunks; i++)
    {
      fread (ents[i].chunkid, 1, 8, ifp);
      ents[i].offset = read_word (ifp);
      ents[i].size = read_word (ifp);
    }

  return (check_stream (ifp) != FRWERR ? ents : NULL);
}

/*
 * read in the string table
 */
static char *
read_stringtab (FILE *ifp, struct chunkent *strent)
{
  if (strptr)
    free (strptr);
  strptr = xmalloc (strent->size);
  fseek (ifp, strent->offset, 0);
  *(unsigned long *)(void *)strptr = read_word (ifp);	/* size in 1st word */
  fread (strptr + 4, 1, (int) strent->size - 4, ifp);

  return (check_stream (ifp) != FRWERR ? strptr : NULL);
}

/*
 * read in the symbol table
 */
static struct symbol *
read_symboltab (FILE *ifp, struct chunkent *syment, int numsyms)
{
  int i;

  if (symptr)
    free (symptr);
  symptr = xmalloc (numsyms * sizeof (struct symbol));

  fseek (ifp, syment->offset, 0);
  for (i = 0; i < numsyms; i++)
    {
      symptr[i].name = read_word (ifp);
      symptr[i].flags = read_word (ifp);
      symptr[i].value = read_word (ifp);
      symptr[i].areaname = read_word (ifp);
    }

  return (check_stream (ifp) != FRWERR ? symptr : NULL);
}

/*
 * read in the AOF header
 */
static struct aofhdr *
read_aofhdr (FILE *ifp, struct chunkent *hdrent)
{
  unsigned int i;
  struct areahdr *areahdr;

  if (aofhdr)
    free (aofhdr);
  aofhdr = xmalloc (hdrent->size);

  /* read-in whole of AOF header */
  fseek (ifp, hdrent->offset, 0);
  aofhdr->filetype = read_word (ifp);
  aofhdr->version = read_word (ifp);
  aofhdr->numareas = read_word (ifp);
  aofhdr->numsyms = read_word (ifp);
  aofhdr->entryarea = read_word (ifp);
  aofhdr->entryoffset = read_word (ifp);
  areahdr = (struct areahdr *) &aofhdr[1];
  for (i = 0; i < aofhdr->numareas; i++)
    {
      areahdr[i].name = read_word (ifp);
      areahdr[i].flags = read_word (ifp);
      areahdr[i].size = read_word (ifp);
      areahdr[i].numrelocs = read_word (ifp);
      areahdr[i].reserved = read_word (ifp);
    }
  return (check_stream (ifp) != FRWERR ? aofhdr : NULL);
}

/*
 * return a pointer to a string in the string table
 */
static char *
string (const char *stringtab, unsigned long offset)
{
  if (!stringtab || offset < 4 || offset >= *(unsigned long *)(void *)stringtab)
    return (NULL);
  return ((char *) (stringtab + offset));
}

/*
 * locate a chunk entry by chunk ID
 */
static struct chunkent *
find_ent (struct chunkhdr *hdr, struct chunkent *ents, const char *name)
{
  unsigned int i;

  for (i = 0; i < hdr->numchunks; i++)
    if (memcmp (ents[i].chunkid, name, 8) == 0)
      return (&ents[i]);
  return (NULL);
}

/* Return -1 on error. 0 on success.  */
static int
decode (const char *aof_file)
{
  FILE *handle;
  struct chunkhdr *hdr;
  struct chunkent *ents, *ent;
  struct aofhdr *aofhdr = NULL;
  struct symbol *symboltab = NULL;
  char *stringtab = NULL;
  long area_offset;
  long symboltab_size;

  handle = fopen (aof_file, "rb");
  if (handle == NULL)
    {
      fprintf (stderr, "File '%s' not found\n", aof_file);
      return -1;
    }

  /* Read chunk header.  */
  hdr = read_chunkhdr (handle);
  if (hdr == NULL)
    {
      fprintf (stderr, "Failed reading chunk header\n");
      return -1;
    }
  if (hdr->chunkfileid != 0xc3cbc6c5)
    {
      fprintf (stderr, "File '%s' is not a chunk file\n", aof_file);
      return -1;
    }

  /* Read chunk entries.  */
  ents = read_chunkents (handle, hdr);
  if (ents == NULL)
    {
      fprintf (stderr, "Failed reading chunk entries\n");
      return -1;
    }

  /* Read string table.  */
  ent = find_ent (hdr, ents, "OBJ_STRT");
  if (ent)
    {
      stringtab = read_stringtab (handle, ent);
      if (stringtab == NULL)
	{
	  fprintf (stderr, "Failed reading string table\n");
	  return -1;
	}
    } else {
      fprintf (stderr, "File '%s' is not an AOF file\n", aof_file);
      return -1;
    }

  /* Find file offset for OBJ_AREA.  */
  ent = find_ent (hdr, ents, "OBJ_AREA");
  if (ent != NULL)
    area_offset = ent->offset;
  else
    area_offset = 0;

  /* Read AOF header.  */
  ent = find_ent (hdr, ents, "OBJ_HEAD");
  if (ent)
    {
      aofhdr = read_aofhdr (handle, ent);
      if (aofhdr == NULL)
	{
	  fprintf (stderr, "Failed reading AOF header\n");
	  return -1;
	}

      /* Read in the symbol table, if any.  */
      if (aofhdr->numsyms)
	{
	  ent = find_ent (hdr, ents, "OBJ_SYMT");
	  if (ent)
	    {
	      symboltab = read_symboltab (handle, ent, aofhdr->numsyms);
	      if (symboltab == NULL)
		{
		  fprintf (stderr, "Failed reading AOF symbol table\n");
		  return -1;
		}
	      symboltab_size = ent->size;
	    }
	}

#if 0
      /* decode each of the areas */
      areahdrs = (struct areahdr *)&aofhdr[1];
      offset = reloff = 0;
      for (i = 0; i < aofhdr->numareas; i++)
	{
	  Byte flags = (areahdrs[i].flags >> 8) & 0xff;

	  if (!(flags & AT_NOINIT))
	    reloff = offset + areahdrs[i].size;
	  print_area (ifp, &areahdrs[i], offset, reloff);
	  if (!(flags & AT_NOINIT))
	    offset = reloff + (areahdrs[i].numrelocs * sizeof (struct reloc));
	}
#endif
    }

  /* Print the symbol table.  */
  if (aofhdr->numsyms)
    {
      unsigned int i, flags;

      for (i = 0; i < aofhdr->numsyms; i++)
	{
	  flags = symboltab[i].flags & 0xff;
	  if (opt_extern_only && (flags & 0x02))
	    {
               const char *name = string (stringtab, symboltab[i].name);
               const char *demangle;

               if (opt_demangle &&
                   (demangle = (cplus_demangle (name, DMGL_PARAMS | DMGL_ANSI))))
                 puts (demangle);
               else
	         puts (name);
	    }
	}
    }

  fclose (handle);
  return 0;
}

static void
parse_args (int argc, char **argv)
{
  int index;

  /* Starting the short option string with '-' is for programs that
     expect options and other ARGV-elements in any order and that care about
     the ordering of the two.  We describe each non-option ARGV-element
     as if it were the argument of an option with character code 1.  */

  const char *shortopts = "oaCDgvpPsruV";

  /* 150 isn't special; it's just an arbitrary non-ASCII char value.  */

#define OPT_SIZE_SORT	150
#define OPT_HELP	151
#define OPT_NO_DEMANGLE 152

  static struct option longopts[] =
  {
    {"print-file-name", no_argument, NULL, 'o'},
    {"debug-syms", no_argument, NULL, 'a'},
    {"demangle", no_argument, NULL, 'C'},
    {"no-demangle", no_argument, NULL, OPT_NO_DEMANGLE},
    {"dynamic", no_argument, NULL, 'D'},
    {"extern-only", no_argument, NULL, 'g'},
    {"numeric-sort", no_argument, NULL, 'v'},
    {"no-sort", no_argument, NULL, 'p'},
    {"portability", no_argument, NULL, 'P'},
    {"print-armap", no_argument, NULL, 's'},
    {"reverse-sort", no_argument, NULL, 'r'},
    {"size-sort", no_argument, NULL, OPT_SIZE_SORT},
    {"undefined-only", no_argument, NULL, 'u'},
    {"version", no_argument, NULL, 'V'},
    {"help", no_argument, NULL, OPT_HELP},
    {NULL, no_argument, NULL, 0}
  };

  if (argc <= 1)
    nm_help ();

  while (1)
    {
      /* getopt_long_only is like getopt_long, but '-' as well as '--' can
         indicate a long option.  */
      int longind;
      int optc;

      optc = getopt_long (argc, argv, shortopts, longopts, &longind);

      if (optc == -1)
	break;
      switch (optc)
	{
	default:
	  exit (EXIT_FAILURE);
	case 'A':
	case 'o':
	  opt_print_file_name = 1;
	  break;
	case 'a':
	  opt_debug_syms = 1;
	  break;
	case 'C':
	  opt_demangle = 1;
	  break;
	case OPT_NO_DEMANGLE:
	  opt_demangle = 0;
	  break;
	case 'D':
	  opt_dynamic = 1;
	  break;
	case 'g':
	  opt_extern_only = 1;
	  break;
	case 'v':
	  opt_numeric_sort = 1;
	  break;
	case 'p':
	  opt_no_sort = 1;
	  break;
	case 'P':
	  opt_portability = 1;
	  break;
	case 's':
	  opt_print_armap = 1;
	  break;
	case 'r':
	  opt_reverse_sort = 1;
	  break;
	case OPT_SIZE_SORT:
	  opt_size_sort = 1;
	  break;
	case 'u':
	  opt_undefined_only = 1;
	  break;
	case 'V':
	  printf ("nm version %s\n", NM_VERSION);
	  break;
	case OPT_HELP:
	  nm_help ();
	  exit (EXIT_FAILURE);
	  break;
	}
    }

  for (index = optind; index < argc; index++)
    decode (argv[index]);
}

int main (int argc, char **argv)
{
  parse_args (argc, argv);
  return EXIT_SUCCESS;
}


static void nm_help (void)
{
  printf ("Usage: nm [option(s)] [file(s)]\n"
	  " List symbols in [file(s)]\n"
	  " The options are:\n"
	  "  -C, --demangle         Demangle C++ symbols\n"
	  "  -g, --extern-only      Display only external symbols\n");

  printf ("\nnm: supported targets: aof\n");
}
