/*
 * output.c
 * Copyright © 1992 Niklas Röjemo
 * Copyright 1997, Nick Burrett.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "error.h"
#include "chunkfile.h"
#include "aoffile.h"
#include "symbol.h"
#include "output.h"
#include "area.h"
#include "os.h"
#include "version.h"
#include "endiandef.h"
#include "uname.h"
#if defined(UNIXLIB) || defined(CROSS_COMPILE)
#include <ctype.h>
#include <errno.h>
#endif
#ifdef __riscos
#include "depend.h"
#endif

static FILE *objfile;
extern char *ObjFileName;
extern int dde;

#define FIX(n) ((3+(int)n)&~3)
#define EXTRA(n) (FIX(n)-n)

char *idfn_text = MESSAGE;

#define MAXNAME 256
static char outname[MAXNAME + 1];

void
outputInit (char *outfile)
{
  objfile = NULL;
  if (outfile && ! (outfile[0] == '-' && outfile[1] == '\0'))
    {
      char *temp;
#ifdef CROSS_COMPILE
      strncpy (outname, outfile, MAXNAME);
      temp = strstr (outname, ".s");
      if (temp && temp[2] == '\0')
	temp[1] = 'o';
#else
#ifdef UNIXLIB
      strncpy (outname, outfile, MAXNAME);
      /* Look for either a '.s' sequence or a 's.' sequence which would
         be used for either Unix or RISC OS respectively.  Then change it
         to a '.o' or 'o.' sequence.  */
      temp = strstr (outname, ".s.");
      if (temp)
	temp[1] = 'o';
      if (outname[0] == 's' && outname[1] == '.')
	outname[0] = 'o';

      /* We don't want to convert something like foo.smart to foo.omart
         so look out for it. Hopefully if .s is at the end then the
         next character is not going to be alphanumeric.  */
      temp = strstr (outname, ".s");
      if (temp && temp[2] == '\0')
        temp[1] = 'o';
#else
      strncpy (outname, uname (outfile, dde), MAXNAME);
      temp = strrchr (outname, '.');
      if (temp > (outname + 2) && temp[-2] == '.' && temp[-1] == 's')
	temp[-1] = 'o';
#endif /* UNIXLIB */
#endif /* CROSS_COMPILE */
      if ((objfile = fopen (outname, "wb")) == NULL)
	{
#if defined(UNIXLIB) || defined(CROSS_COMPILE)
	  error (ErrorAbort, FALSE, "As can't write %s: %s",
		 outname, strerror (errno));
#else
	  error (ErrorAbort, FALSE, "As can't write %s", outname);
#endif
	}
      /* setvbuf (objfile, NULL, _IOFBF, 16 * 1024); */
#ifndef CROSS_COMPILE
      if (DependFileName)
	dependOpen (outname);
#endif
    }
  else
    {
      outname[0] = 0;
      objfile = stdout;
    }
}

void
outputFinish (void)
{
  if (objfile != NULL && objfile != stdout)
    fclose (objfile);
}

void
outputRemove (void)
{
  outputFinish ();
  if (outname[0])
    remove (outname);
}

int
countAreas (Symbol * ap)
{
  int i = 0;
  while (ap)
    {
      ap->used = i++;
      ap = ap->area.info->next;
    }
  return i;
}

void
outputAof (void)
{
  int noareas = countAreas (areaHead);
  unsigned int idfn_size, strt_size;
  int offset, pad, written, obj_area_size;
  Symbol *ap;
  ChunkFileHeader chunk_header;
  ChunkFileHeaderEntry chunk_entry;
  AofHeader aof_head;
  AofEntry aof_entry;

  /* We must call relocFix() before anything else.  */
  obj_area_size = 0;
  ap = areaHead;		/* avoid problems with no areas.  */
  while (ap)
    {
      ap->area.info->norelocs = relocFix (ap);
      if (!(ap->area.info->type & AREA_UDATA))
	obj_area_size += ourword (FIX (ap->value.ValueInt.i))
	  + ourword (ap->area.info->norelocs) * sizeof (AofReloc);
      ap = ap->area.info->next;
    }

  aof_head.Type = AofHeaderID;
  aof_head.Version = 310;
  aof_head.noAreas = noareas;
  aof_head.noSymbols = symbolFix ();
  aof_head.EntryArea = areaEntry ? areaEntry->used + 1 : 0;
  aof_head.EntryOffset = areaEntry ? areaEntryOffset : 0;

  /* Write out the chunk file header.  */
  chunk_header.ChunkField = ChunkFileID;
  chunk_header.maxChunks = 5;
  chunk_header.noChunks = 5;
  written = fwrite ((void *) &chunk_header, sizeof (char),
		    sizeof (chunk_header), objfile);
  offset = sizeof (chunk_header) + 5 * sizeof (chunk_entry);

  chunk_entry.ChunkIDPrefix = ChunkID_OBJ;
  chunk_entry.ChunkIDType = ChunkID_OBJ_HEAD;
  chunk_entry.FileOffset = offset;
  chunk_entry.Size = sizeof (aof_head) + noareas * sizeof (aof_entry);
  offset += chunk_entry.Size;
  written += fwrite ((void *) &chunk_entry, sizeof (char),
		     sizeof (chunk_entry), objfile);

  chunk_entry.ChunkIDPrefix = ChunkID_OBJ;
  chunk_entry.ChunkIDType = ChunkID_OBJ_IDFN;
  chunk_entry.FileOffset = offset;
  chunk_entry.Size = FIX (strlen (idfn_text) + 1);
  offset += chunk_entry.Size;
  idfn_size = chunk_entry.Size;
  written += fwrite ((void *) &chunk_entry, sizeof (char),
		     sizeof (chunk_entry), objfile);

  chunk_entry.ChunkIDPrefix = ChunkID_OBJ;
  chunk_entry.ChunkIDType = ChunkID_OBJ_STRT;
  chunk_entry.FileOffset = offset;
  chunk_entry.Size = FIX (symbolStringSize ());
  offset += chunk_entry.Size;
  strt_size = chunk_entry.Size;
  written += fwrite ((void *) &chunk_entry, sizeof (char),
		     sizeof (chunk_entry), objfile);

  chunk_entry.ChunkIDPrefix = ChunkID_OBJ;
  chunk_entry.ChunkIDType = ChunkID_OBJ_SYMT;
  chunk_entry.FileOffset = offset;
  chunk_entry.Size = aof_head.noSymbols * sizeof (AofSymbol);
  offset += chunk_entry.Size;
  written += fwrite ((void *) &chunk_entry, sizeof (char),
		     sizeof (chunk_entry), objfile);

  chunk_entry.ChunkIDPrefix = ChunkID_OBJ;
  chunk_entry.ChunkIDType = ChunkID_OBJ_AREA;
  chunk_entry.FileOffset = offset;
  chunk_entry.Size = obj_area_size;
  offset += chunk_entry.Size;
  written += fwrite ((void *) &chunk_entry, sizeof (char),
		     sizeof (chunk_entry), objfile);

  if (written != (sizeof (chunk_header) + 5 * sizeof (chunk_entry)))
    {
      errorLine (0, 0, ErrorSerious, FALSE,
		 "Internal outputAof: error when writing chunk file header");
      exit (-1);
    }

/******** Chunk 0 Header ********/
  fwrite ((void *) &aof_head, sizeof (char), sizeof (aof_head), objfile);

  ap = areaHead;
  while (ap)
    {
      aof_entry.Name = ap->offset;
      aof_entry.Type = ap->area.info->type;
      aof_entry.Size = FIX (ap->value.ValueInt.i);
      aof_entry.noRelocations = ap->area.info->norelocs;
      if (aof_entry.noRelocations != 0 && aof_entry.Type & AREA_UDATA)
	errorLine (0, 0, ErrorSerious, FALSE,
		   "Internal outputAof: relocations in uninitialised area");
      aof_entry.Unused = 0;
      fwrite ((void *) &aof_entry, sizeof (char), sizeof (aof_entry), objfile);
      ap = ap->area.info->next;
    }

/******** Chunk 1 Identification *********/
  if (idfn_size !=
      fwrite ((void *) idfn_text, 1, idfn_size, objfile))
    {
      errorLine (0, 0, ErrorSerious, FALSE,
		 "Internal outputAof: error when writing identification");
      exit (-1);
    }
/******** Chunk 2 String Table ***********/
  if (fwrite ((void *) &strt_size, 1, 4, objfile) != sizeof (strt_size))
    {
      errorLine (0, 0, ErrorSerious, FALSE,
		 "Internal outputAof: error when writing string table size");
      exit (-1);
    }
  symbolStringOutput (objfile);
  for (pad = EXTRA (symbolStringSize ()); pad; pad--)
    fputc (0, objfile);

/******** Chunk 3 Symbol Table ***********/
  symbolSymbolOutput (objfile);

/******** Chunk 4 Area *****************/
  ap = areaHead;
  while (ap)
    {
      if (!(ap->area.info->type & AREA_UDATA))
	{
	  if (ourword (FIX (ap->value.ValueInt.i)) !=
	      fwrite ((void *) ap->area.info->image, sizeof (char),
		      ourword (FIX (ap->value.ValueInt.i)), objfile))
	    {
	      errorLine (0, 0, ErrorSerious, FALSE,
		"Internal outputAof: error when writing %s image", ap->str);
	      exit (-1);
	    }
	  relocOutput (objfile, ap);
	}
      ap = ap->area.info->next;
    }
}
