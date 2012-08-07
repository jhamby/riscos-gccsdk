/*
 * decode an AOF file
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2012 GCCSDK Developers
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

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "decaof.h"
#include "decode.h"
#include "error.h"
#include "io.h"
#include "main.h"
#include "misc.h"

static struct chunkhdr *hdr;
static struct chunkent *ents, *ent;
static struct aofhdr *aofhdr;
static struct areahdr *areahdrs;
static struct symbol *symboltab;
static const aof_obj_strt *stringtab;
static long area_offset;
static Word symboltab_size;

static void print_area (FILE * ifp, struct areahdr *areahdr, Word offset,
			Word reloff);
static const char *string (Word offset);
static const char *symname (Word offset);
static const char *areaname (Word offset);

void
decode (void)
{
  int i;
  Word offset;

  while (nfiles--)
    {
      const char *filename = *files++;
      FILE *ifp = fopen (filename, "r");
      if (!ifp)
	{
	  error ("unable to open file \"%s\"", filename);
	  continue;
	}

      hdr = NULL;
      ents = NULL;
      aofhdr = NULL;
      areahdrs = NULL;
      stringtab = NULL;
      symboltab = NULL;

      /* read chunk header */
      hdr = read_chunkhdr (ifp);
      if (!hdr)
	{
	  error ("reading file \"%s\"", filename);
	  goto next_file;
	}
      if (hdr->chunkfileid != 0xc3cbc6c5)
	{
	  error ("file \"%s\" is not a chunk file", filename);
	  goto next_file;
	}

      /* read chunk entries in */
      ents = read_chunkents (ifp, hdr);
      if (!ents)
	{
	  error ("reading chunk entries for file \"%s\"", filename);
	  goto next_file;
	}

      printf ("** File:\n%s\n", filename);

      /* read string table */
      ent = find_ent (hdr, ents, "OBJ_STRT");
      if (ent)
	{
	  stringtab = read_stringtab (ifp, ent);
	  if (!stringtab)
	    {
	      error ("reading string table for file \"%s\"", filename);
	      goto next_file;
	    }
	}
      else
	puts ("\n** No String table");

      /* read and print identification string */
      ent = find_ent (hdr, ents, "OBJ_IDFN");
      if (ent)
	{
	  const char *idstr = read_ident (ifp, ent);
	  if (!idstr)
	    {
	      error ("reading identification for file \"%s\"", filename);
	      goto next_file;
	    }
	  printf ("\n** Identification:\n%s\n", idstr);
	  free_chunk_memory ((void *)idstr);	/* not needed */
	}
      else
	puts ("\n** No Identification chunk");

      /* find file offset of OBJ_AREA (for later use) */
      ent = find_ent (hdr, ents, "OBJ_AREA");
      area_offset = ent->offset;

      /* read-in AOF header */
      ent = find_ent (hdr, ents, "OBJ_HEAD");
      if (ent)
	{
	  aofhdr = read_aofhdr (ifp, ent);
	  if (!aofhdr)
	    {
	      error ("reading AOF header for file \"%s\"", filename);
	      goto next_file;
	    }
	  const char *cptr;
	  switch (aofhdr->filetype)
	    {
	      case 0xc5e2d080:
		cptr = "Relocatable object format";
		break;
	      case 0xc5e2d081:
		cptr = "AOF image type 1";
		break;
	      case 0xc5e2d083:
		cptr = "AOF image type 2";
		break;
	      case 0xc5e2d087:
		cptr = "AOF image type 3";
		break;
	      default:
		cptr = "unknown image type";
		break;
	    }
	  printf ("\n** AOF Header: %s\n\n", cptr);

	  /* Version 150 : AOF 1.xx
	     Version 200 : AOF 2.xx
	     Version 310/311 : AOF 3.xx */
	  printf ("AOF version: %d\n", aofhdr->version);
	  i = aofhdr->numareas;
	  printf ("%d area%s\n", i, i == 1 ? "" : "s");
	  i = aofhdr->numsyms;
	  printf ("%d symbol%s\n", i, i == 1 ? "" : "s");

	  /* read in the symbol table, if any */
	  if (aofhdr->numsyms)
	    {
	      ent = find_ent (hdr, ents, "OBJ_SYMT");
	      if (ent)
		{
		  symboltab = read_symboltab (ifp, ent, aofhdr->numsyms);
		  if (!symboltab)
		    {
		      error ("reading symbol table for file \"%s\"",
			     filename);
		      goto next_file;
		    }
		  symboltab_size = ent->size;
		}
	    }

	  /* decode each of the areas */
	  areahdrs = (struct areahdr *) &aofhdr[1];
	  offset = 0;
	  for (i = 0; i < aofhdr->numareas; i++)
	    {
	      print_area (ifp, &areahdrs[i], offset, offset + areahdrs[i].size);
	      if (!(areahdrs[i].flags & AREA_UDATA))
		offset += areahdrs[i].size + areahdrs[i].numrelocs*sizeof (struct reloc);
	    }
	}
      else
	puts ("\n** No AOF header");

      if (symtab)
	{
	  if (aofhdr->numsyms)
	    puts ("\n** Symbol table:\n");
	  else
	    puts ("\n** No Symbol table");
	  for (i = 0; i < aofhdr->numsyms; i++)
	    {
	      int flags = symboltab[i].flags;
	      printf ("%-16s (%02x) ", string (symboltab[i].name), flags);

	      switch (flags & 0x3)
		{
		  case 0x01:
		    fputs ("local", stdout);
		    break;
		  case 0x02:
		    fputs ("extern", stdout);
		    break;
		  case 0x03:
		    fputs ("global", stdout);
		    break;
		  default:
		    fputs ("unknown-type", stdout);
		    break;
		}
	      if ((flags & (1 << 2)) && flags & (1 << 0))
		fputs (", absolute", stdout);
	      if ((flags & (1 << 3)) && !(flags & (1 << 0)))
		fputs (", case-insensitive", stdout);
	      if ((flags & (1 << 4)) && ((flags & 0x03) == 0x02))
		fputs (", weak", stdout);
	      if ((flags & (1 << 5)) && ((flags & 0x03) == 0x03))
		fputs (", strong", stdout);
	      if ((flags & (1 << 6)) && ((flags & 0x03) == 0x02))
		fputs (", common", stdout);
	      if (flags & (1 << 8))
		fputs (", cadatum", stdout);
	      if (flags & (1 << 9))
		fputs (", fpargs", stdout);
	      if (flags & (1 << 11))
		fputs (", leaf", stdout);
	      if (flags & (1 << 12))
		fputs (", thumb", stdout);
	      if (flags & ((1 << 0) | (1 << 6)))
		{
		  if (flags & ((1 << 2) | (1 << 6)))
		    printf (" = 0x%08x", symboltab[i].value);
		  else
		    printf (" at \"%s\" + 0x%06x",
		      string (symboltab[i].areaname),
		      symboltab[i].value);
		}
	      putchar ('\n');
	    }
	}
      
      if (strtab && stringtab->size)
	{
	  puts ("\n** String table:\n");
	  offset = 4;
	  const char *cptr;
	  while ((cptr = string (offset)) != NULL)
	    {
	      size_t len = strlen (cptr);
	      if (len)
		printf ("%06x: %s\n", offset, cptr);
	      offset += len + 1;
	    }
	}

    next_file:
      free_chunk_memory (aofhdr);
      free_chunk_memory (symboltab);
      free_chunk_memory (stringtab);
      free_chunk_memory (ents);
      fclose (ifp);
    }
}

/**
 * Print each AOF area...
 */
static void
print_area (FILE * ifp, struct areahdr *areahdr, Word offset, Word reloff)
{
  Word ubits;			/* the unknown bits */
  int isCode, isData;
  Word flags = areahdr->flags;

  printf ("\n** Area (0x%06x) \"%s\", aligned at %d byte%s, ",
	  areahdr->flags >> 8,
	  string (areahdr->name),
	  1 << (areahdr->flags & 0xFF), (areahdr->flags & 0xFF) ? "s" : "");
  if (flags & AREA_DEBUG)
    {
      fputs ("[debug] ", stdout);
      isCode = isData = 0;
      ubits = flags & AREA_CODE;
    }
  else
    {
      if (flags & AREA_CODE)
	{
	  fputs ("[code] ", stdout);
	  isCode = 1;
	  isData = 0;
	}
      else
	{
	  fputs ("[data] ", stdout);
	  isData = 1;
	  isCode = 0;
	}
      ubits = 0;
    }
  if (flags & AREA_ABS)
    fputs ("[abs] ", stdout);
  if (flags & AREA_COMMONDEF)
    fputs ("[commdef] ", stdout);
  if (flags & AREA_COMMONREF)
    fputs ("[commref] ", stdout);
  if (flags & AREA_UDATA)
    fputs ("[noinit] ", stdout);
  if (flags & AREA_READONLY)
    fputs ("[readonly] ", stdout);
  if (flags & AREA_PIC)
    fputs ("[pic] ", stdout);
  if (flags & AREA_32BITAPCS)
    {
      if (isCode)
	fputs ("[32bit] ", stdout);
      else
	ubits |= AREA_32BITAPCS;
    }
  if (flags & AREA_REENTRANT)
    {
      if (isCode)
	fputs ("[reentrant] ", stdout);
      else
	ubits |= AREA_REENTRANT;
    }
  if (flags & AREA_EXTFPSET)
    {
      if (isCode)
	fputs ("[extfpset] ", stdout);
      else
	ubits |= AREA_EXTFPSET;
    }
  if (flags & AREA_NOSTACKCHECK)
    {
      if (isCode)
	fputs ("[nostackcheck] ", stdout);
      else
	ubits |= AREA_NOSTACKCHECK;
    }
  if (flags & AREA_BASED)
    {				/* is same test for AREA_THUMB */
      if (isCode)
	{
	  fputs ("[thumb] ", stdout);
	  ubits |= flags & AREA_MASKBASEREGS;
	}
      else if (isData)
	printf ("[based r%d] ", (flags & AREA_MASKBASEREGS) >> 24);
      else
	ubits |= flags & (AREA_BASED | AREA_MASKBASEREGS);
    }
  if (flags & AREA_STUBDATA)
    {				/* is same test for AREA_HALFWORD */
      if (isCode)
	fputs ("[halfword] ", stdout);
      else if (isData)
	fputs ("[stubdata] ", stdout);
      else
	ubits |= AREA_STUBDATA;
    }
  if (flags & AREA_INTERWORK)
    {
      if (isCode)
	fputs ("[interwork] ", stdout);
      else
	ubits |= AREA_INTERWORK;
    }
  if (flags & AREA_SOFTFLOAT)
    fputs ("[soft-float] ", stdout);
  if (flags & AREA_LINKONCE)
    fputs ("[linkonce] ", stdout);
  ubits |= flags & AREA_UNKNOWNBITS;
  if (ubits)
    printf ("[unknown bits %06x00] ", (flags & AREA_UNKNOWNBITS) >> 8);
  printf ("\n");

  if (areahdr->baseaddr || (flags & AREA_ABS))
    printf ("   Base address 0x%x%s\n",
	    areahdr->baseaddr, (flags & AREA_ABS) ? "" : " (but area is NOT marked as absolute)");

  printf ("   Size %d byte%s, %d relocation%s\n",
	  areahdr->size,
	  (areahdr->size == 1) ? "" : "s",
	  areahdr->numrelocs, (areahdr->numrelocs == 1) ? "" : "s");

  if (area_contents && !(flags & AREA_UDATA))
    {
      fseek (ifp, area_offset + offset, SEEK_SET);
      int cols = 0;
      for (Word area_off = 0; area_off < areahdr->size; area_off += 4)
	{
	  if ((cols++ & 7) == 0)
	    printf ("\n%06x: ", areahdr->baseaddr + area_off);
	  else
	    putchar (' ');
	  printf ("%08x", read_word (ifp));
	}
      printf ("\n\n");
    }

  if (reloc_dir && areahdr->numrelocs)
    {
      fseek (ifp, area_offset + reloff, SEEK_SET);
      puts ("** Relocations:\n");
      for (Word numrelocs = areahdr->numrelocs; numrelocs; numrelocs--)
	{
	  enum { type1, type2, type3 } rtype;

	  struct reloc *reloc = read_reloc (ifp);
	  if (!reloc)
	    {
	      error ("reading relocation directive");
	      return;
	    }

	  if ((reloc->flags & 0xfff00000) == 0x00000000)
	    rtype = type1;
	  else if ((reloc->flags & 0xf0000000) == 0x80000000)
	    rtype = type2;
	  else if ((reloc->flags & 0x80000000) == 0x80000000)
	    rtype = type3;
	  else
	    {
	      puts ("** unknown relocation type");
	      continue;
	    }

	  printf ("At 0x%06x (flags 0x%08x): ", reloc->offset, reloc->flags);
	  bool isWordSize = false;
	  switch ((reloc->flags >> (rtype == type1 ? 16 : 24)) & 0x3)
	    {
	      case 0x0:
		printf ("Byte ");
		break;
	      case 0x1:
		printf ("Halfword ");
		break;
	      case 0x2:
		printf ("Word ");
		isWordSize = true;
		break;
	      case 0x3:
		switch (reloc->flags & (0x3 << 29))
		  {
		    case 0x0 << 29:
		      printf ("Unlimited number of instructions ");
		      break;
		    case 0x1 << 29:
		      printf ("At most 1 instruction ");
		      break;
		    case 0x2 << 29:
		      printf ("At most 2 instructions ");
		      break;
		    case 0x3 << 29:
		      printf ("At most 3 instructions ");
		      break;
		  }
		break;
	    }
	  
	  switch (rtype)
	    {
	      case type1:
		switch ((reloc->flags >> 18) & 3)
		  {
		    case 0:
		      /* Additive internal : the base address of the area (with
		         which this relocation directive is associated) is added
			 into the field to be relocated.  */
		      fputs ("displaced by base of this area", stdout);
		      if (!isWordSize)
		        fputs ("!!! Relocation data size is not 4 bytes !!!", stdout);
		      break;

		    case 1:
		      /* PC-relative (always symbol) */
		      printf ("PC-relative to symbol \"%s\"",
		        symname (reloc->flags & 0xffff));
		      break;

		    case 2:
		      /* Additive symbol : the value of the given symbol is added
		         to the field being relocated.  */
		      printf ("displaced by symbol \"%s\"",
		        symname (reloc->flags & 0xffff));
		      if (!isWordSize)
		        fputs ("!!! Relocation data size is not 4 bytes !!!", stdout);
		      break;

		    case 3:
		      fputs ("UNDEFINED", stdout);
		      break;
		  }
		fputs (" (type-1 reloc)\n", stdout);
		break;
	      case type2:
		switch ((reloc->flags >> 26) & 3)
		  {
		    case 0:
		      /* Additive internal : the base address of the area (with
		         which this relocation directive is associated) is added
			 into the field to be relocated.  */
		      printf ("displaced by base of area \"%s\"",
		        areaname (reloc->flags & 0xffffff));
		      if (!isWordSize)
		        fputs ("!!! Relocation data size is not 4 bytes !!!", stdout);
		      break;

		    case 1:
		      /* PC-relative internal */
		      printf ("PC-relative to base of area \"%s\"",
		        areaname (reloc->flags & 0xffffff));
		      break;

		    case 2:
		      /* additive symbol */
		      printf ("displaced by symbol \"%s\"",
		        symname (reloc->flags & 0xffffff));
		      if (!isWordSize)
		        fputs ("!!! Relocation data size is not 4 bytes !!!", stdout);
		      break;

		    case 3:
		      /* PC-relative symbol */
		      printf ("PC-relative to symbol \"%s\"",
		        symname (reloc->flags & 0xffffff));
		      break;
		  }
		fputs (" (type-2 reloc)\n", stdout);
		break;
	      case type3:
		switch (reloc->flags & (5<<26))
		  {
		    case 0<<26: /* R = 0, B = 0 */
		      printf ("displaced by ");
		      if (reloc->flags & (1<<27))
			printf ("symbol \"%s\"", symname (reloc->flags & 0xffffff));
		      else
			printf ("base of area \"%s\"", areaname (reloc->flags & 0xffffff));
		      break;
		    case 1<<26: /* R = 1, B = 0 : intra-link-unit value branch is used */
		    case 5<<26: /* R = 1, B = 1 : inter-link-unit value branch is used */
		      if (reloc->flags & (1<<27))
			printf ("PC-relative to base of area \"%s\"", areaname (reloc->flags & 0xffffff));
		      else
			printf ("absolute PC"); /* FIXME: correct ??? */
		      if (reloc->flags & (4<<26))
		        printf ("  using inter-link-unit value");
		      break;
		    case 4<<26: /* R = 0, B = 1 */
		      /* FIXME: meaning of A here ? */
		      printf ("displaced by data base of area \"%s\"", areaname (reloc->flags & 0xffffff));
		      break;
		  }
		fputs (" (type-3 reloc)\n", stdout);
		break;
	    }
	}
      putc ('\n', stdout);
    }
}

/**
 * \return pointer to a string in the string table
 */
static const char *
string (Word offset)
{
  offset -= 4;
  if (!stringtab || offset >= stringtab->size - 4)
    return NULL;
  return &stringtab->str[offset];
}

/**
 * \return pointer to a symbol name in the symbol table
 */
static const char *
symname (Word offset)
{
  if (!symboltab || offset > symboltab_size)
    return NULL;
  const struct symbol *sym = (const struct symbol *) (symboltab + offset);
  return string (sym->name);
}

/**
 * \return area name
 */
static const char *
areaname (Word areaIndex)
{
  if (!aofhdr || !areahdrs || areaIndex >= aofhdr->numareas)
    return NULL;
  return string (areahdrs[areaIndex].name);
}
