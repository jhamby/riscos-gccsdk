/*
 * file input/output
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2013 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "decaof.h"
#include "error.h"
#include "io.h"

/*
 * check for EOF or write/read errors on stream.
 */
Ferror
check_stream (FILE * fp)
{
  int ret = FNOERR;

  if (feof (fp))
    ret = FEND;
  else if (ferror (fp))
    ret = FRWERR;
  if (ret != FNOERR)
    clearerr (fp);
  return ret;
}

/**
 * Read a byte from the input stream.
 */
Byte
read_byte (FILE * ifp)
{
  return (Byte) getc (ifp);
}

/**
 * Read a little-endian 2-byte halfword from the input stream.
 */
Halfword
read_halfword (FILE * ifp)
{
  Byte lowByte = read_byte (ifp);
  Byte highByte = read_byte (ifp);

  return lowByte + (highByte << 8);
}

/**
 * Read a little-endian 4-byte word from the input stream.
 */
Word
read_word (FILE * ifp)
{
  Halfword lowHalfword = read_halfword (ifp);
  Halfword highHalfword = read_halfword (ifp);

  return lowHalfword + (highHalfword << 16);
}

/**
 * Read in the chunk header
 */
const struct chunkhdr *
read_chunkhdr (FILE * ifp)
{
  static struct chunkhdr hdr;

  fseek (ifp, 0L, SEEK_SET);
  hdr.chunkfileid = read_word (ifp);
  hdr.maxchunks = read_word (ifp);
  hdr.numchunks = read_word (ifp);
  return check_stream (ifp) != FRWERR ? &hdr : NULL;
}

/*
 * memory pointers maintained by read_xxx functions
 */
static const struct chunkent *oChunkEntries;	/* chunk file entries */
static const aof_obj_strt *oStringTable;	/* string table */
static const struct symbol *oSymbolTable;	/* symbol table */
static char *oIdentString;		/* identification string */
static const struct aofhdr *oAOFHdr;	/* AOF header */

/**
 * Free the memory used by a chunk
 */
void
free_chunk_memory (const void *ptr)
{
  if (ptr == (const void *) oChunkEntries)
    {
      free ((void *) oChunkEntries);
      oChunkEntries = NULL;
    }
  else if (ptr == (const void *) oStringTable)
    {
      free ((void *) oStringTable);
      oStringTable = NULL;
    }
  else if (ptr == (const void *) oSymbolTable)
    {
      free ((void *) oSymbolTable);
      oSymbolTable = NULL;
    }
  else if (ptr == (const void *) oIdentString)
    {
      free ((void *) oIdentString);
      oIdentString = NULL;
    }
  else if (ptr == (const void *) oAOFHdr)
    {
      free ((void *) oAOFHdr);
      oAOFHdr = NULL;
    }
}

/**
 * Read in the chunk entries
 */
const struct chunkent *
read_chunkents (FILE * ifp, const struct chunkhdr *hdr)
{
  free ((void *)oChunkEntries);
  struct chunkent *chunkEntries = (struct chunkent *) malloc (sizeof (struct chunkent) * hdr->maxchunks);
  if ((oChunkEntries = chunkEntries) == NULL)
    {
      error ("memory exhausted");
      exit (EXIT_FAILURE);
    }

  fseek (ifp, sizeof (struct chunkhdr), SEEK_SET);
  for (uint32_t chunkIdx = 0; chunkIdx != hdr->numchunks; ++chunkIdx)
    {
      fread (chunkEntries[chunkIdx].chunkid, 1, 8, ifp);
      chunkEntries[chunkIdx].offset = read_word (ifp);
      chunkEntries[chunkIdx].size = read_word (ifp);
    }

  return check_stream (ifp) != FRWERR ? oChunkEntries : NULL;
}

/**
 * Read in the string table OBJ_STRT
 */
const aof_obj_strt *
read_stringtab (FILE * ifp, const struct chunkent *strent)
{
  if (strent->size < 4)
    {
      error ("String table size is too short");
      exit (EXIT_FAILURE);
    }

  free ((void *)oStringTable);
  aof_obj_strt *stringTable = (aof_obj_strt *) malloc (strent->size);
  if ((oStringTable = stringTable) == NULL)
    {
      error ("memory exhausted");
      exit (EXIT_FAILURE);
    }

  fseek (ifp, strent->offset, SEEK_SET);
  stringTable->size = read_word (ifp);	/* size in 1st word */
  fread (&stringTable->str, 1, strent->size - 4, ifp);

  return check_stream (ifp) != FRWERR ? stringTable : NULL;
}

/*
 * Read in the symbol table
 */
const struct symbol *
read_symboltab (FILE * ifp, const struct chunkent *syment, uint32_t numSyms)
{
  free ((void *)oSymbolTable);
  struct symbol *symbolTable = (struct symbol *) malloc (numSyms * sizeof (struct symbol)); 
  if ((oSymbolTable = symbolTable) == NULL)
    {
      error ("memory exhausted");
      exit (EXIT_FAILURE);
    }

  fseek (ifp, syment->offset, SEEK_SET);
  for (uint32_t symIdx = 0; symIdx != numSyms; ++symIdx)
    {
      symbolTable[symIdx].name = read_word (ifp);
      symbolTable[symIdx].flags = read_word (ifp);
      symbolTable[symIdx].value = read_word (ifp);
      symbolTable[symIdx].areaname = read_word (ifp);
    }

  return check_stream (ifp) != FRWERR ? symbolTable : NULL;
}

/**
 * Read in the identification chunk
 */
const char *
read_ident (FILE * ifp, const struct chunkent *ident)
{
  free ((void *)oIdentString);
  char *identString = (char *) malloc (ident->size);
  if ((oIdentString = identString) == NULL)
    {
      error ("memory exhausted");
      exit (EXIT_FAILURE);
    }

  fseek (ifp, (long) ident->offset, SEEK_SET);
  fread (identString, 1, (int) ident->size, ifp);

  return check_stream (ifp) != FRWERR ? identString : NULL;
}

/**
 * Read in the AOF header
 */
const struct aofhdr *
read_aofhdr (FILE * ifp, const struct chunkent *hdrent)
{
  if (hdrent->size < sizeof (struct aofhdr))
    {
      error ("AOF header size is %d which is smaller than the expected minimum size %zd",
	     hdrent->size, sizeof (struct aofhdr));
      return NULL;
    }
  free ((void *)oAOFHdr);
  struct aofhdr *aofHdr = (struct aofhdr *) malloc (hdrent->size);
  if ((oAOFHdr = aofHdr) == NULL)
    {
      error ("memory exhausted");
      exit (EXIT_FAILURE);
    }

  /* read-in whole of AOF header */
  fseek (ifp, hdrent->offset, SEEK_SET);
  aofHdr->filetype = read_word (ifp);
  aofHdr->version = read_word (ifp);
  aofHdr->numareas = read_word (ifp);
  aofHdr->numsyms = read_word (ifp);
  aofHdr->entryarea = read_word (ifp);
  aofHdr->entryoffset = read_word (ifp);
  if (hdrent->size != sizeof (struct aofhdr) + aofHdr->numareas * sizeof (struct areahdr))
    {
      error ("Malformed header size : is %d bytes but based on number of declared areas we expect it to be %zd bytes",
	     hdrent->size,
	     sizeof (struct aofhdr) + aofHdr->numareas * sizeof (struct areahdr));
      return NULL;
    }
  struct areahdr *areaHdr = (struct areahdr *) &aofHdr[1];
  for (uint32_t areaIdx = 0; areaIdx != aofHdr->numareas; ++areaIdx)
    {
      areaHdr[areaIdx].name = read_word (ifp);
      areaHdr[areaIdx].flags = read_word (ifp);
      areaHdr[areaIdx].size = read_word (ifp);
      areaHdr[areaIdx].numrelocs = read_word (ifp);
      areaHdr[areaIdx].baseaddr = read_word (ifp);
    }

  return check_stream (ifp) != FRWERR ? aofHdr : NULL;
}

/**
 * Read in a relocation directive
 */
const struct reloc *
read_reloc (FILE * ifp)
{
  static struct reloc reloc;

  reloc.offset = read_word (ifp);
  reloc.flags = read_word (ifp);
  return check_stream (ifp) != FRWERR ? &reloc : NULL;
}
