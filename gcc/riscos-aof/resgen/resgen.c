/*
 * A tool to create an AOF file encapsulating one or more files ready for
 * (de)registering with ResourceFS.
 *
 * Copyright (c) 2006 John Tytgat / BASS  <John.Tytgat@aaug.net>
 * Copyright (c) 2006 GCCSDK Developers
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CROSS_COMPILE
#  include <kernel.h>
#  include <unixlib/local.h>
#  include <sys/param.h>
#endif

#include "../../sdk-config.h"

typedef struct
{
  unsigned int id;
  unsigned int max_chunks;
  unsigned int num_chunks;
} aof_chheader;

typedef struct
{
  char id[8];
  unsigned int offset;
  unsigned int size;
} aof_chunk;

typedef struct
{
  aof_chheader chheader;
  aof_chunk ident;
  aof_chunk header;
  aof_chunk areas;
  aof_chunk symbol;
  aof_chunk string;
  aof_chunk spare1;
  aof_chunk spare2;
  aof_chunk spare3;
} aof_header;

typedef struct
{
  unsigned int symname;
  unsigned int attr;
  unsigned int value;
  unsigned int areaname;
} aof_symbol;

typedef struct
{
  unsigned int filetype;
  unsigned int version;
  unsigned int numareas;
  unsigned int numsymbols;
  unsigned int entryareaindex;
  unsigned int entryoffset;
} aof_headerchunk;

typedef struct
{
  unsigned int name;
  unsigned int attralign;
  unsigned int size;
  unsigned int numrelocs;
  unsigned int baseaddr;
} aof_areaheader;

typedef struct
{
  unsigned int offset;
  unsigned int loadaddr;
  unsigned int execaddr;
  unsigned int size;
  unsigned int attr;
} res_header;

#define RESGENVERSION "v1.00 (" __DATE__ ") [GCC " __VERSION__ "]"

#if defined(WORDS_BIGENDIAN) && defined(CROSS_COMPILE)
#  define HOST2LITTLEENDIAN(x) \
  ((x) >> 24) | (((x) >> 8) & 0xFF00) | (((x) << 8) & 0xFF0000) | ((x) << 24)
#else
#  define HOST2LITTLEENDIAN(x) \
  (x)
#endif

#define CHUNKFILE_ID (0xC3CBC6C5)

static const char *aofareaname;
static FILE *outfh;
static aof_header aofheader;
static unsigned int aofoffset;
static char buf[16*1024];

static void
resgen_help (void)
{
  printf ("ResourceFS AOF Generator " RESGENVERSION "\n"
          "\n"
          "The command syntax is:\n"
          "  resgen <area name> <output file> [ <local file> <ResourceFS name> ]*\n");
}


/* Checks if the user gave a valid areaname.  Returns 0 if that's the case,
 * non-0 if not.
 */
static int
check_areaname (const char *areaname)
{
  int topbit;
  const unsigned char *str;
  
  for (str = (const unsigned char *)areaname, topbit = 0; *str; ++str)
    {
      if ((*str < 32 && *str != 10 && *str != 13) || *str == 127)
        {
          fprintf (stderr, "Area name contains unprintable characters\n");
          return 1;
        }
      if (*str >= 128)
        topbit = 1;
    }
  if (topbit)
    printf ("Warning: area name contains top-bit characters\n");
  
  aofareaname = areaname;
  
  return 0;
}


/* This writes a more or less dummy AOF header in order to reserve its
   place at the beginning of the AOF file. It will be rewritten with
   its final & correct contents at output_aofheader_pass2() time.  */
static int
output_aofheader_pass1 (void)
{
  aofheader.chheader.id = HOST2LITTLEENDIAN(CHUNKFILE_ID);
  aofheader.chheader.max_chunks = HOST2LITTLEENDIAN(8);
  aofheader.chheader.num_chunks = HOST2LITTLEENDIAN(5);

  if (fwrite (&aofheader, sizeof(aofheader), 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write AOF header\n");
      return 1;
    }

  aofoffset = sizeof(aofheader);

  return 0;
}


/* This rewrites the AOF header now that all its parameters are known
   and filled in.  */
static int
output_aofheader_pass2 (void)
{
  long cur_pos;
  
  if ((cur_pos = ftell (outfh)) == -1
      || fseek (outfh, 0L, SEEK_SET)
      || fwrite (&aofheader, sizeof(aofheader), 1, outfh) != 1
      || fseek (outfh, cur_pos, SEEK_SET))
    {
      fprintf (stderr, "Failed to write AOF header\n");
      return 1;
    }

  return 0;
}


static int
output_identchunk (void)
{
#define IDENTSTRING "resgen " RESGENVERSION "\0\0"
  memcpy (&aofheader.ident.id, "OBJ_IDFN", 8);
  aofheader.ident.offset = HOST2LITTLEENDIAN(aofoffset);
  aofheader.ident.size = HOST2LITTLEENDIAN(sizeof(IDENTSTRING) & -4);

  if (fwrite (IDENTSTRING, sizeof(IDENTSTRING) & -4, 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write OBJ_IDFN chunk\n");
      return 1;
    }
  
  aofoffset += sizeof(IDENTSTRING) & -4;
  
  return 0;
}


static int
output_stringchunk (void)
{
  unsigned int slen = strlen (aofareaname);
  unsigned int size = (4 + slen + 3) & -4;

  memcpy (&aofheader.string.id, "OBJ_STRT", 8);
  aofheader.string.offset = HOST2LITTLEENDIAN(aofoffset);
  aofheader.string.size = HOST2LITTLEENDIAN(size);

  if (fwrite (&aofheader.string.size, sizeof (aofheader.string.size), 1, outfh) != 1
      || fwrite (aofareaname, slen, 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write OBJ_STRT chunk\n");
      return 1;
    }
  if ((slen &= 3) != 0)
    for (; slen < 4; ++slen)
      fputc (0, outfh);

  aofoffset += size;

  return 0;
}


/* output_areachunk() needs to be called first.  */
static int
output_areaheader (void)
{
  aof_headerchunk headerchunk;
  aof_areaheader areaheader;
  unsigned int size = sizeof(headerchunk) + sizeof(areaheader);

  headerchunk.filetype = HOST2LITTLEENDIAN(0xC5E2D080);
  headerchunk.version = HOST2LITTLEENDIAN(310);
  headerchunk.numareas = HOST2LITTLEENDIAN(1);
  headerchunk.numsymbols = HOST2LITTLEENDIAN(1);
  headerchunk.entryareaindex = 0;
  headerchunk.entryoffset = 0;

  areaheader.name = HOST2LITTLEENDIAN(4); /* First string */
  areaheader.attralign = HOST2LITTLEENDIAN(0x12202);
  areaheader.size = aofheader.areas.size;
  areaheader.numrelocs = 0;
  areaheader.baseaddr = 0;

  memcpy (&aofheader.header.id, "OBJ_HEAD", 8);
  aofheader.header.offset = HOST2LITTLEENDIAN(aofoffset);
  aofheader.header.size = HOST2LITTLEENDIAN(size);

  if (fwrite (&headerchunk, sizeof(headerchunk), 1, outfh) != 1
      || fwrite (&areaheader, sizeof(areaheader), 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write OBJ_HEAD");
      return 1;
    }

  aofoffset += size;

  return 0;
}


static int
output_symbols (void)
{
  aof_symbol symbol;
  unsigned int size = sizeof(symbol);

  symbol.symname = HOST2LITTLEENDIAN(4); /* First string */
  symbol.attr = HOST2LITTLEENDIAN(3); /* Global symbol */
  symbol.value = 0;
  symbol.areaname = HOST2LITTLEENDIAN(4); /* First string */

  memcpy (&aofheader.symbol.id, "OBJ_SYMT", 8);
  aofheader.symbol.offset = HOST2LITTLEENDIAN(aofoffset);
  aofheader.symbol.size = HOST2LITTLEENDIAN(size);

  if (fwrite (&symbol, sizeof(symbol), 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write OBJ_SYMT");
      return 1;
    }
    
  aofoffset += size;

  return 0;
}

static int
get_fileinfo (FILE *lfh, const char *lfname, unsigned int *loadaddr,
              unsigned int *execaddr, unsigned int *fsize,
              unsigned int *attr)
{
#ifdef CROSS_COMPILE
  unsigned int lfnamesize;
  int x, y, z;
  long fsizer;
  
  if (fseek (lfh, 0L, SEEK_END)
      || (fsizer = ftell (lfh)) == -1
      || fseek (lfh, 0L, SEEK_SET))
    {
      fprintf (stderr, "Failed to determine length of %s\n", lfname);
      return 1;
    }
  *fsize = fsizer;
    
  lfnamesize = strlen(lfname);
  if (lfnamesize > 4
      && lfname[lfnamesize - 4] == ','
      && isxdigit(x = lfname[lfnamesize - 3])
      && isxdigit(y = lfname[lfnamesize - 2])
      && isxdigit(z = lfname[lfnamesize - 1]))
    {
      unsigned int filetype;
      filetype = (x <= '9') ? x - '0' : toupper (x) - 'A' + 10;
      filetype = (filetype << 4) + ((y <= '9') ? y - '0' : toupper (y) - 'A' + 10);
      filetype = (filetype << 4) + ((z <= '9') ? z - '0' : toupper (z) - 'A' + 10);
      
      *loadaddr = 0xFFF00000 + (filetype << 8);
    }
  else
    *loadaddr = 0xFFFFFF00;
  *execaddr = 0;
  *attr = 0x11;
#else
  _kernel_swi_regs regs;
  _kernel_oserror *err;
  char upath[MAXPATHLEN];
  int filetype;

  if (__riscosify_std (lfname, 0, upath, sizeof(upath), &filetype) == NULL)
    {
      fprintf (stderr, "Failed to convert filename %s to a RISC OS path\n", lfname);
      return 1;
    }
  
  regs.r[0] = 17;
  regs.r[1] = (int)upath;
  if ((err = _kernel_swi (8 /* OS_File */, &regs, &regs)) != NULL)
    {
      fputs (err->errmess, stderr);
      return 1;
    }

  if (regs.r[0] == 0)
    {
      fprintf (stderr, "File %s not found\n", lfname);
      return 1;
    }

  *loadaddr = (unsigned int)regs.r[2];
  *execaddr = (unsigned int)regs.r[3];
  *fsize = (unsigned int)regs.r[4];
  *attr = (unsigned int)regs.r[5];
#endif
  return 0;
}


static int
output_resfile (unsigned int *totalsize, const char *lfname,
                const char *rfname, int last)
{
  res_header resheader;
  FILE *lfh;
  unsigned int loadaddr, execaddr, fsize, attr;
  unsigned int numcp;
  unsigned int datsize, ressize;
  unsigned int rfnamesize;
  int failed;

  /* ResourceFS filenames should not begin with "$.".  */
  if (rfname[0] == '$' && rfname[1] == '.')
    rfname += 2;
  if (rfname[0] == '\0')
    {
      fprintf (stderr, "Empty ResourceFS name for local file %s\n", lfname);
      return 1;
    }
  rfnamesize = strlen (rfname);

  if ((lfh = fopen (lfname, "r")) == NULL)
    {
      fprintf (stderr, "Failed to open %s for reading\n", lfname);
      return 1;
    }
  /* Will be reset to 0 when everything has succesfully been finished.  */
  failed = 1;

  if (get_fileinfo (lfh, lfname, &loadaddr, &execaddr, &fsize, &attr))
    goto resfile_error;

  ressize = sizeof (res_header) + ((rfnamesize + 3) & -4)
            + 4 + ((fsize + 3) & -4);
  resheader.offset = HOST2LITTLEENDIAN(ressize);
  resheader.loadaddr = HOST2LITTLEENDIAN(loadaddr);
  resheader.execaddr = HOST2LITTLEENDIAN(execaddr);
  resheader.size = HOST2LITTLEENDIAN(fsize);
  resheader.attr = HOST2LITTLEENDIAN(attr);
  if (fwrite (&resheader, sizeof (resheader), 1, outfh) != 1
      || fwrite (rfname, rfnamesize, 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write ResourceFS file data\n");
      goto resfile_error;
    }
  if ((rfnamesize &= 3) != 0)
    for (; rfnamesize < 4; ++rfnamesize)
      fputc (0, outfh);

  /* Copy ResourceFS file data (first its aligned length, then the
     data itself.  */
  datsize = HOST2LITTLEENDIAN(fsize + 4);
  if (fwrite (&datsize, sizeof(datsize), 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write ResourceFS file data\n");
      goto resfile_error;
    }
  for (numcp = fsize; numcp != 0; /* */)
    {
      unsigned int nextsize = (numcp < sizeof(buf)) ? numcp : sizeof(buf);
      if (fread (buf, nextsize, 1, lfh) != 1
          || fwrite (buf, nextsize, 1, outfh) != 1)
        {
          fprintf (stderr, "Failed to copy ResourceFS file data\n");
          goto resfile_error;
        }
      numcp -= nextsize;
    }
  if ((fsize &= 3) != 0)
    for (; fsize < 4; ++fsize)
      fputc (0, outfh);
  /* Write offset 0 after last ResourceFS data block.  */
  if (last)
    {
      const unsigned int zero = 0;
      if (fwrite (&zero, sizeof(zero), 1, outfh) != 1)
        {
          fprintf (stderr, "Failed to write end of ResourceFS file data\n");
          goto resfile_error;
        }
      ressize += 4;
    }

  *totalsize += ressize;
  failed = 0;

resfile_error:
  if (fclose (lfh))
    {
      fprintf (stderr, "Failed to close %s\n", lfname);
      return 1;
    }

  return failed;
}


static int
output_asm (unsigned int *totalsize)
{
  const unsigned int asminstr[] = {
    HOST2LITTLEENDIAN(0xE28F0000),  /* ADD R0, PC, #0 */
    HOST2LITTLEENDIAN(0xE1A0F00E)   /* MOV PC, R14 */
    };

  if (fwrite (asminstr, sizeof(asminstr), 1, outfh) != 1)
    {
      fprintf (stderr, "Failed to write assembler\n");
      return 1;
    }

  *totalsize += sizeof(asminstr);

  return 0;
}


static int
output_areachunk (int numargs, const char * const *args)
{
  unsigned int size = 0;

  if (numargs == 0)
    printf ("Warning: no ResourceFS files specified\n");
  else
    {
      if (output_asm (&size))
        return 1;
      for (/* */; numargs > 1; numargs -= 2, args += 2)
        {
          if (output_resfile (&size, args[0], args[1], numargs == 2))
            return 1;
        }
      if (numargs == 1)
        {
          fprintf (stderr, "No ResourceFS name for local file %s\n", args[0]);
          return 1;
        }
    }

  memcpy (&aofheader.areas.id, "OBJ_AREA", 8);
  aofheader.areas.offset = HOST2LITTLEENDIAN(aofoffset);
  aofheader.areas.size = HOST2LITTLEENDIAN(size);
  
  aofoffset += size;
  
  return 0;
}


/* Syntax:
 *   resgen <area name> <output file> [ <local file> <ResourceFS name> ]
 */
int
main (int argc, char *argv[])
{
  int failed;
  
  if (argc < 3)
    {
      resgen_help ();
      return (argc == 1) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

  if (check_areaname (argv[1]))
    return EXIT_FAILURE;

  if ((outfh = fopen (argv[2], "w")) == NULL)
    {
      fprintf (stderr, "Failed to open %s for output\n", argv[2]);
      return EXIT_FAILURE;
    }

  failed = output_aofheader_pass1 ()
           || output_identchunk ()
           || output_stringchunk ()
           || output_areachunk (argc - 3, (const char * const *)&argv[3])
           || output_areaheader ()
           || output_symbols ()
           || output_aofheader_pass2 ();

  if (fclose (outfh))
    {
      fprintf (stderr, "Failed to close %s\n", argv[2]);
      failed = 1;
    }
  outfh = NULL;

  if (failed && remove (argv[2]))
    fprintf (stderr, "Failed to remove bogus output file %s\n", argv[2]);

  return (failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
