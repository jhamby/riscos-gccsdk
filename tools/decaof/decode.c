/*
 * decode an AOF file
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

static const struct chunkhdr *hdr;
static const struct chunkent *ents, *ent;
static const struct aofhdr *aofhdr;
static const struct areahdr *areahdrs;
static const struct symbol *symboltab;
static const aof_obj_strt *stringtab;
static uint32_t area_offset;
static Word symboltab_size;

static void print_area (FILE * ifp, const struct areahdr *areahdr, Word offset,
			Word reloff);
static const char *string (Word offset);
static const char *symname (Word offset);
static const char *areaname (Word offset);

void
decode (void)
{
  const bool only_one_file = nfiles == 1;
  while (nfiles--)
    {
      const char *filename = *files++;
      FILE *ifp = fopen (filename, "rb");
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
      area_offset = 0;
      symboltab_size = 0;

      /* read chunk header */
      hdr = read_chunkhdr (ifp);
      if (!hdr)
	{
	  error ("reading file \"%s\" (", filename);
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
      if (opt_print_ident)
	{
	  ent = find_ent (hdr, ents, "OBJ_IDFN");
	  if (ent)
	    {
	      const char *idstr = read_ident (ifp, ent);
	      if (!idstr)
		{
		  error ("reading identification for file \"%s\"", filename);
		  goto next_file;
		}
	      printf ("\n** Identification (%s):\n\n%s\n", filename, idstr);
	      free_chunk_memory ((void *)idstr);	/* not needed */
	    }
	  else
	    puts ("\n** No Identification chunk");
	}

      /* Read area header (must be present).  */
      ent = find_ent (hdr, ents, "OBJ_AREA");
      if (!ent)
	{
	  error ("reading file \"%s\" (no OBJ_AREA found)", filename);
	  goto next_file;
	}

      /* File offset of OBJ_AREA (for later use).  */
      area_offset = ent->offset;

      /* Read AOF header (must be present).  */
      ent = find_ent (hdr, ents, "OBJ_HEAD");
      if (!ent)
	{
	  error ("reading file \"%s\" (no OBJ_HEAD found)", filename);
	  goto next_file;
	}
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
      printf ("\n** AOF Header: %s", cptr);
      if (!only_one_file)
	printf (" (%s)", filename);
      printf (":\n\n");

      /* Version 150 : AOF 1.xx
	 Version 200 : AOF 2.xx
	 Version 310/311 : AOF 3.xx */
      printf ("AOF version: %d\n", aofhdr->version);
      uint32_t numAreas = aofhdr->numareas;
      printf ("%d area%s\n", numAreas, numAreas == 1 ? "" : "s");
      uint32_t numSyms = aofhdr->numsyms;
      printf ("%d symbol%s\n", numSyms, numSyms == 1 ? "" : "s");

      /* Read in the symbol table, if any.  */
      if (aofhdr->numsyms)
	{
	  ent = find_ent (hdr, ents, "OBJ_SYMT");
	  if (ent)
	    {
	      symboltab = read_symboltab (ifp, ent, aofhdr->numsyms);
	      if (!symboltab)
		{
		  error ("reading symbol table for file \"%s\"", filename);
		  goto next_file;
		}
	      symboltab_size = ent->size;
	    }
	}

      areahdrs = (const struct areahdr *) &aofhdr[1];

      /* Decode each of the areas.  */
      if (opt_print_area_dec)
	{
	  for (uint32_t areaIdx = 0, offset = 0; areaIdx != aofhdr->numareas; ++areaIdx)
	    {
	      Word relocOffset = (areahdrs[areaIdx].flags & AREA_UDATA) ? offset : offset + areahdrs[areaIdx].size;
	      print_area (ifp, &areahdrs[areaIdx], offset, relocOffset);
	      offset = relocOffset + areahdrs[areaIdx].numrelocs*sizeof (struct reloc);
	    }
	}
      
      if (opt_print_symtab)
	{
	  if (!aofhdr->numsyms)
	    {
	      printf ("\n** No Symbol table");
	      if (!only_one_file)
		printf (" (%s)", filename);
	    }
	  else
	    {
	      printf ("\n** Symbol table");
	      if (!only_one_file)
		printf (" (%s)", filename);
	      printf (":\n\n");
	      for (uint32_t symIdx = 0; symIdx != aofhdr->numsyms; ++symIdx)
		{
		  uint32_t flags = symboltab[symIdx].flags;
		  printf ("%-16s (%02x) ", string (symboltab[symIdx].name), flags);

		  switch (SYMBOL_KIND (flags))
		    {
		      case SYMBOL_LOCAL:
			fputs ("local", stdout);
			break;
		      case SYMBOL_REFERENCE:
			fputs ("extern", stdout);
			break;
		      case SYMBOL_GLOBAL:
			fputs ("global", stdout);
			break;
		      default:
			fputs ("unknown-type 0", stdout);
			break;
		    }
		  if ((flags & SYMBOL_ABSOLUTE) != 0)
		    fputs ((flags & SYMBOL_DEFINED) ? ", absolute" : ", bit 2 ???", stdout);
		  if ((flags & SYMBOL_NOCASE) != 0)
		    fputs (SYMBOL_KIND (flags) == SYMBOL_REFERENCE ? ", case-insensitive" : ", bit 3 ???", stdout);
		  if ((flags & SYMBOL_WEAK) != 0)
		    fputs (SYMBOL_KIND (flags) == SYMBOL_REFERENCE ? ", weak" : ", bit 4 ???", stdout);
		  if ((flags & SYMBOL_STRONG) != 0)
		    fputs (SYMBOL_KIND (flags) == SYMBOL_GLOBAL ? ", strong" : ", bit 5 ???", stdout);
		  if ((flags & SYMBOL_COMMON) != 0)
		    fputs (SYMBOL_KIND (flags) == SYMBOL_REFERENCE ? ", common" : ", bit 6 ???", stdout);
		  if ((flags & SYMBOL_DATUM) != 0)
		    fputs (", cadatum", stdout);
		  if ((flags & SYMBOL_FPREGARGS) != 0)
		    fputs (", fpregargs", stdout);
		  if ((flags & SYMBOL_USESSB) != 0)
		    fputs (", usessb", stdout);
		  if ((flags & SYMBOL_LEAF) != 0)
		    fputs (", leaf", stdout);
		  if ((flags & SYMBOL_THUMB) != 0)
		    fputs (", thumb", stdout);
		  if ((flags & SYMBOL_SOFTFP) != 0)
		    fputs (", softfp", stdout);
		  if ((flags & ~SYMBOL_SUPPORTED_AOF_BITS) != 0)
		    printf (", set bits 0x%x ???", flags & ~SYMBOL_SUPPORTED_AOF_BITS);
		  if (flags & (SYMBOL_DEFINED | SYMBOL_COMMON))
		    {
		      if (flags & (SYMBOL_ABSOLUTE | SYMBOL_COMMON))
			printf (" = 0x%08x", symboltab[symIdx].value);
		      else
			printf (" at \"%s\" + 0x%06x",
				string (symboltab[symIdx].areaname),
				symboltab[symIdx].value);
		    }
		  putchar ('\n');
		}
	    }
	}

      if (opt_print_strtab)
	{
	  if (!stringtab->size)
	    {
	      printf ("\n** No String table");
	      if (!only_one_file)
		printf (" (%s)", filename);
	    }
	  else
	    {
	      printf ("\n** String table");
	      if (!only_one_file)
		printf (" (%s)", filename);
	      printf (":\n\n");
	      uint32_t offset = 4;
	      const char *str;
	      while ((str = string (offset)) != NULL)
		{
		  size_t len = strlen (str);
		  if (len)
		    printf ("%06x: %s\n", offset, str);
		  offset += len + 1;
		}
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
print_area (FILE * ifp, const struct areahdr *areahdr, Word offset, Word reloff)
{
  const Word areaFlags = areahdr->flags;

  printf ("\n** Area (0x%06x) \"%s\", aligned at %d byte%s, ",
	  areaFlags >> 8,
	  string (areahdr->name),
	  1 << (areaFlags & 0xFF), (areaFlags & 0xFF) ? "s" : "");
  bool isCode, isData;
  Word ubits; /* unknown bits */
  if (areaFlags & AREA_DEBUG)
    {
      fputs ("[debug] ", stdout);
      isCode = isData = false;
      ubits = areaFlags & AREA_CODE;
    }
  else
    {
      if (areaFlags & AREA_CODE)
	{
	  fputs ("[code] ", stdout);
	  isCode = true;
	  isData = false;
	}
      else
	{
	  fputs ("[data] ", stdout);
	  isData = true;
	  isCode = false;
	}
      ubits = 0;
    }
  if (areaFlags & AREA_ABS)
    fputs ("[abs] ", stdout);
  if (areaFlags & AREA_COMMONDEF)
    fputs ("[commdef] ", stdout);
  if (areaFlags & AREA_COMMONREF)
    fputs ("[commref] ", stdout);
  if (areaFlags & AREA_UDATA)
    fputs ("[noinit] ", stdout);
  if (areaFlags & AREA_READONLY)
    fputs ("[readonly] ", stdout);
  if (areaFlags & AREA_PIC)
    fputs ("[pic] ", stdout);
  if (areaFlags & AREA_32BITAPCS)
    {
      if (isCode)
	fputs ("[32bit] ", stdout);
      else
	ubits |= AREA_32BITAPCS;
    }
  if (areaFlags & AREA_REENTRANT)
    {
      if (isCode)
	fputs ("[reentrant] ", stdout);
      else
	ubits |= AREA_REENTRANT;
    }
  if (areaFlags & AREA_EXTFPSET)
    {
      if (isCode)
	fputs ("[extfpset] ", stdout);
      else
	ubits |= AREA_EXTFPSET;
    }
  if (areaFlags & AREA_NOSWSTACKCHECK)
    {
      if (isCode)
	fputs ("[noswstackcheck] ", stdout);
      else
	ubits |= AREA_NOSWSTACKCHECK;
    }
  if (areaFlags & AREA_BASED)
    {				/* is same test for AREA_THUMB */
      if (isCode)
	{
	  fputs ("[thumb] ", stdout);
	  ubits |= areaFlags & AREA_MASKBASEREG;
	}
      else if (isData)
	printf ("[based r%d] ", (areaFlags & AREA_MASKBASEREG) >> 24);
      else
	ubits |= areaFlags & (AREA_BASED | AREA_MASKBASEREG);
    }
  if (areaFlags & AREA_STUBDATA)
    {				/* is same test for AREA_HALFWORD */
      if (isCode)
	fputs ("[halfword] ", stdout);
      else if (isData)
	fputs ("[stubdata] ", stdout);
      else
	ubits |= AREA_STUBDATA;
    }
  if (areaFlags & AREA_INTERWORK)
    {
      if (isCode)
	fputs ("[interwork] ", stdout);
      else
	ubits |= AREA_INTERWORK;
    }
  if (areaFlags & AREA_VFP)
    fputs ("[vfp] ", stdout);
  ubits |= areaFlags & ~(isCode ? AREA_INT_CODEMASK : AREA_INT_DATAMASK);
  if (ubits)
    printf ("[unknown bits %06x00] ", ubits & ~AREA_ALIGN_MASK);
  printf ("\n");

  if (areahdr->baseaddr || (areaFlags & AREA_ABS) != 0)
    printf ("   Base address 0x%x%s\n",
	    areahdr->baseaddr, (areaFlags & AREA_ABS) ? "" : " (but area is NOT marked as absolute)");

  printf ("   Size %d byte%s, %d relocation%s\n",
	  areahdr->size,
	  (areahdr->size == 1) ? "" : "s",
	  areahdr->numrelocs, (areahdr->numrelocs == 1) ? "" : "s");

  if (opt_print_area_contents && (areaFlags & AREA_UDATA) == 0)
    {
      fseek (ifp, area_offset + offset, SEEK_SET);
      unsigned cols = 0;
      Word repeatedValue;
      unsigned numRepeatedValue = 0;
      for (uint32_t areaIdx = 0; areaIdx != areahdr->size; /* */)
	{
	  /* Print full rows.  */
	  while (areaIdx != areahdr->size)
	    {
	      if ((cols % 8) == 0)
		printf ("\n%06x:", areahdr->baseaddr + areaIdx);
	      Word value = read_word (ifp);
	      if (numRepeatedValue == 0)
		repeatedValue = value;
	      if (repeatedValue == value)
		++numRepeatedValue;
	      else
		numRepeatedValue = 0;
	      printf (" %08x", value);

	      areaIdx += 4;
	      ++cols;
	      if ((cols % 8) == 0 && numRepeatedValue > 4)
		break;
	    }

	  /* When enough words have repeated themselves, try to collaps them
	     as one or more rows marked with "...".  */
	  Word value;
	  unsigned colsR = 0; /* Number of repeated values.  */
	  while (areaIdx != areahdr->size)
	    {
	      value = read_word (ifp);
	      if (value != repeatedValue)
		break;
	      areaIdx += 4;
	      ++colsR;
	    }
	  if (colsR / 8 != 0)
	    printf ("\n..."); /* At least one row of repeated values.  */
	  cols += colsR;
	  if (areaIdx != areahdr->size)
	    {
	      colsR &= 7;
	      printf ("\n%06x:", areahdr->baseaddr + areaIdx - 4*colsR);
	      while (colsR--)
		printf (" %08x", repeatedValue);
	      printf (" %08x", value);
	      areaIdx += 4;
	      cols += 1;
	    }
	}
      printf ("\n");
    }

  if (opt_print_reloc_dir && areahdr->numrelocs)
    {
      fseek (ifp, area_offset + reloff, SEEK_SET);
      puts ("\n** Relocations:\n");
      for (Word numrelocs = areahdr->numrelocs; numrelocs; numrelocs--)
	{
	  const struct reloc *reloc = read_reloc (ifp);
	  if (!reloc)
	    {
	      error ("reading relocation directive");
	      return;
	    }

	  enum { type1, type2 } rtype;
	  if ((reloc->flags & 0xfff00000) == 0x00000000)
	    rtype = type1;
	  else if ((reloc->flags & 0x80000000) == 0x80000000)
	    rtype = type2;
	  else
	    {
	      puts ("** Unknown relocation type");
	      continue;
	    }

	  printf ("At 0x%06x (flags 0x%08x): ", reloc->offset, reloc->flags);

	  bool isWordSize; /**< Will be used to detect : bytes and halfwords
	    may only be relocated by constant values of suitably small size.
	    They may not be relocated by an area's base address.  */
	  switch ((reloc->flags >> (rtype == type1 ? 16 : 24)) & 0x3)
	    {
	      case 0x0:
		isWordSize = false;
		printf ("Byte ");
		break;

	      case 0x1:
		isWordSize = false;
		printf ("Short ");
		break;

	      case 0x2:
		isWordSize = true;
		printf ("Word ");
		break;

	      case 0x3:
		isWordSize = true;
		/* Only from AOF 3 onwards.  */
		if (aofhdr->version < 310)
		  {
		    puts ("** Undefined field type");
		    continue;
		  }
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
		      /* A = 0, R = 0
		         Additive internal : the base address of the area (with
		         which this relocation directive is associated) is added
			 into the field to be relocated.
			 SID field is ignored.  */
		      fputs ("displaced by base of this area", stdout);
		      /* As relocation is based on area.  */
		      if (!isWordSize)
		        fputs ("!!! Relocation data size is not 4 bytes !!!", stdout);
		      break;

		    case 1:
		      /* A = 0, R = 1
		         PC-relative (always symbol).  */
		      printf ("PC-relative to symbol \"%s\"",
			      symname (reloc->flags & 0xffff));
		      /* As PC-relative works on instructions... */
		      if (!isWordSize)
		        fputs ("!!! Relocation data size is not 4 bytes !!!", stdout);
		      break;

		    case 2:
		      /* A = 1, R = 0
		         Additive symbol : the value of the given symbol is added
		         to the field being relocated.  */
		      printf ("displaced by symbol \"%s\"",
			      symname (reloc->flags & 0xffff));
		      break;

		    case 3:
		      /* A = 1, R = 1 */
		      fputs ("UNDEFINED", stdout);
		      break;
		  }
		puts (" (type-1 reloc)\n");
		break;

	      case type2:
		{
		  /* bits B & R */
		  const char *relocHow;
		  switch (reloc->flags & 0x14<<24)
		    {
		      case 0x00<<24: /* B = 0, R = 0 */
			/* Additive relocation: the relocation value is added
			   to the subject field.

			     subject_field = subject_field + relocation_value
			 */
			relocHow = "displaced by";
		        break;

		      case 0x04<<24: /* B = 0, R = 1 */
			/* PC-relative relocation: to the subject field is
			   added the difference between the relocation value
			   and the base of the area containing the subject
			   field.

			     subject_field = subject_field + (relocation_value -
					 		      base_of_area_containing(subject_field))

			   As a special case, if <A> is 0, and the relocation
			   value is specified as the base of the area
			   containing the subject field, then it is not added and:

			     subject_field = subject_field - base_of_area_containing(subject_field)

			  This caters for relocatable PC-relative branches to
			  fixed target addresses.  */
			relocHow = "PC-relative to";
		        break;

		      case 0x10<<24: /* B = 1, R = 0 */
			/* Based area relocation. The relocation value must be
			   an address within a based data area. The subject
			   field is incremented by the difference between this
			   value and the base address of the consolidated based
			   area group (the linker consolidates all areas based
			   on the same base register into a single, contiguous
			   region of the output image).

			     subject_field = subject_field + (relocation_value -
							      base_of_area_group_containing(relocation_value))
			 */
			relocHow = "base-relative to";
		        break;

		      case 0x14<<24: /* B = 1, R = 1 */
			/* Used to denote that the inter-link-unit value of a
			   branch destination is to be used, rather than the 
			   more usual intra-link-unit value.  */ 
			relocHow = "tailcall to";
			break;
		    }
		  const char *relocValType = (reloc->flags & (1<<27)) ? "symbol" : "base of area"; /* bit A */
		  const char *relocVal = (reloc->flags & (1<<27)) ? symname (reloc->flags & 0xFFFFFF) : areaname (reloc->flags & 0xFFFFFF); /* bit A */
		  printf ("%s %s \"%s\" (type-2 reloc)\n", relocHow, relocValType, relocVal);
		  if ((reloc->flags & (1<<27)) == 0 && !isWordSize)
		    puts ("!!! Relocation data size is not 4 bytes !!!");
		  break;
	        }
	    }
	}
    }
}

/**
 * \return pointer to a string in the string table
 */
static const char *
string (Word offset)
{
  if (!stringtab || offset < 4 || offset >= stringtab->size)
    return NULL;
  return &stringtab->str[offset - 4];
}

/**
 * \return pointer to a symbol name in the symbol table
 */
static const char *
symname (Word offset)
{
  if (!symboltab || offset >= symboltab_size)
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
