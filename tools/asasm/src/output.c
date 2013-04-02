/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Nick Burrett
 * Copyright (c) 2000-2013 GCCSDK Developers
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * output.c
 */

#include "config.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#ifdef __riscos__
#  include <swis.h>
#endif

#if !defined(__riscos__) || defined(__TARGET_UNIXLIB__)
#  include <sys/param.h>		/* for MAXPATHLEN */
#else
#  define MAXPATHLEN 1024
#endif

#include "aoffile.h"
#include "area.h"
#include "chunkfile.h"
#include "depend.h"
#include "elf.h"
#include "error.h"
#include "filename.h"
#include "main.h"
#include "output.h"
#include "os.h"
#include "reloc.h"
#include "symbol.h"

static FILE *oFHandle;

#define FIX(n) ((3+(int)(n))&~3)
#define EXTRA(n) (FIX(n)-(n))

#define GET_IDFN ((idfn_text) ? idfn_text : DEFAULT_IDFN)
const char *idfn_text = NULL; /**< Identifier, when NULL use DEFAULT_IDFN; this is a malloced string.  */

static char outname[MAXPATHLEN];

#if defined(WORDS_BIGENDIAN)
/* Convert to ARM byte-sex.  */
ARMWord
armword (ARMWord val)
{
  return (val >> 24) |
         ((val >> 8) & 0xff00)   |
         ((val << 8) & 0xff0000) |
          (val << 24);
}

/* Convert from ARM byte-sex.  */
ARMWord
ourword (ARMWord val)
{
  return  (val >> 24) |
         ((val >> 8) & 0xff00)   |
         ((val << 8) & 0xff0000) |
          (val << 24);
}
#endif


const char *
Output_OpenOutput (const char *outfile)
{
  oFHandle = NULL;
  for (unsigned pathidx = 0; /* */; ++pathidx)
    {
      const char *out[3];
      bool state[3] = { false, false, false };

      do
	{
	  out[0] = FN_AnyToNative (outfile, pathidx, outname, sizeof (outname),
				   &state[0], eA_Dot_B);
	  if (out[0] && (oFHandle = fopen (out[0], "wb")) != NULL)
	    return outname;

	  out[1] = FN_AnyToNative (outfile, pathidx, outname, sizeof (outname),
				   &state[1], eB_DirSep_A);
	  if (out[1] && (oFHandle = fopen (out[1], "wb")) != NULL)
	    return outname;

	  out[2] = FN_AnyToNative (outfile, pathidx, outname, sizeof (outname),
				   &state[2], eA_Slash_B);
	  if (out[2] && (oFHandle = fopen (out[2], "wb")) != NULL)
	    return outname;

	  assert (state[0] == state[1] && state[0] == state[2]);
	} while (out[0] && out[1] && out[2] && state[0]);

      if (out[0] == NULL && out[1] == NULL && out[2] == NULL)
	break;
    }

  Error (ErrorError, PACKAGE_NAME " can't write %s: %s", outname, strerror (errno));
  return NULL;
}


void
Output_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case eStartUp:
      case ePassOne:
      case ePassTwo:
	break;

      case eOutput:
	{
	  if (outname[0])
	    Depend_Write (outname);
	  break;
	}

      case eCleanUp:
	{
	  if (oFHandle != NULL)
	    {
	      fclose (oFHandle);
	      oFHandle = NULL;
#ifdef __riscos__
	      /* Set filetype to 0xE1F (ELF, ELF output) or 0xFFD (Data, AOF output).  */
	      _kernel_swi_regs regs;
	      regs.r[0] = 18;
	      regs.r[1] = (int) outname;
	      regs.r[2] = (option_aof) ? 0xFFD : 0xE1F;

	      _kernel_swi(OS_File, &regs, &regs);
#endif
	    }
	  break;
	}
    }
}


void
Output_Remove (void)
{
  if (oFHandle != NULL)
    {
      fclose (oFHandle);
      oFHandle = NULL;
    }
  if (outname[0])
    remove (outname);
}


static size_t
writeEntry (int ID, int type, size_t size, size_t *offset)
{
  const ChunkFileHeaderEntry chunk_entry =
    {
      .ChunkIDPrefix = armword (ID),
      .ChunkIDType = armword (type),
      .FileOffset = armword (*offset),
      .Size = armword (size)
    };
  *offset += size;
  return fwrite (&chunk_entry, 1, sizeof (chunk_entry), oFHandle);
}

void
Output_AOF (void)
{
  size_t totalAreaSize = 0;
  uint32_t numAreas = 0;
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      ap->area->number = numAreas++;
      Reloc_PrepareRelocOutPart1 (ap);
      if (!Area_IsNoInit (ap->area))
	totalAreaSize += FIX (ap->area->maxIdx)
			   + ap->area->relocOutP->num * sizeof (AofReloc);
    }

  SymbolOut_t symOut = Symbol_CreateSymbolOut ();
  
  const AofHeader aof_head =
    {
      .Type = armword (AofHeaderID),
      .Version = armword (310),
      .noAreas = armword (numAreas),
      .noSymbols = armword (symOut.numAllSymbols),
      .EntryArea = armword (areaEntrySymbol ? areaEntrySymbol->area->number + 1 : 0),
      .EntryOffset = armword (areaEntrySymbol ? areaEntryOffset : 0)
    };

  /* Write out the chunk file header.  */
  const ChunkFileHeader chunk_header =
    {
      .ChunkField = armword (ChunkFileID),
      .maxChunks = armword (5),
      .noChunks = armword (5)
    };
  size_t written = fwrite (&chunk_header, 1, sizeof (chunk_header), oFHandle);
  size_t offset = sizeof (chunk_header) + 5*sizeof (ChunkFileHeaderEntry);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_HEAD,
			 sizeof (AofHeader) + numAreas*sizeof (AofEntry), &offset);

  size_t idfn_size = FIX (strlen (GET_IDFN) + 1);
  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_IDFN, idfn_size, &offset);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_STRT,
                         FIX (symOut.strDataSize), &offset);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_SYMT,
                         symOut.symDataSize, &offset);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_AREA,
                         totalAreaSize, &offset);

  if (written != sizeof (chunk_header) + 5*sizeof (ChunkFileHeaderEntry))
    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing chunk file header");

  /* Chunk 0 Header (OBJ_HEAD).  */
  fwrite (&aof_head, 1, sizeof (aof_head), oFHandle);
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      const AofEntry aof_entry =
	{
	  .Name = armword (ap->offset),
          .Type = armword (ap->area->type & AREA_INT_AOFMASK),
          .Size = armword (FIX (ap->area->maxIdx)),
          .noRelocations = armword (ap->area->relocOutP->num),
          .BaseAddr = armword ((ap->area->type & AREA_ABS) ? Area_GetBaseAddress (ap) : 0)
	};
      assert (ap->area->relocOutP->num == 0 || !Area_IsNoInit (ap->area));
      fwrite (&aof_entry, 1, sizeof (aof_entry), oFHandle);
    }

  /* Chunk 1 Identification (OBJ_IDFN).  */
  if (fwrite (GET_IDFN, 1, idfn_size, oFHandle) != idfn_size)
    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing identification");

  /* Chunk 2 String Table (OBJ_STRT).  */
  if (fwrite (symOut.strDataP, 1, symOut.strDataSize, oFHandle) != symOut.strDataSize)
    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing OBJ_STRT");    
  for (unsigned pad = EXTRA (symOut.strDataSize); pad; pad--)
    fputc (0, oFHandle);

  /* Chunk 3 Symbol Table (OBJ_SYMT).  */
  if (fwrite (symOut.symDataP, 1, symOut.symDataSize, oFHandle) != symOut.symDataSize)
    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing OBJ_SYMT");    

  /* Chunk 4 Area (OBJ_AREA).  */
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      if (!Area_IsNoInit (ap->area))
	{
	  if (fwrite (ap->area->image, 1, ap->area->maxIdx, oFHandle) != ap->area->maxIdx)
	    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing data for area %s", ap->str);
	  /* Word align the written area.  */
	  for (unsigned pad = EXTRA (ap->area->maxIdx); pad; --pad)
	    fputc (0, oFHandle);
	  Reloc_PrepareRelocOutPart2 (ap);
	  if (fwrite (ap->area->relocOutP->relocs.rawP,
		      1, ap->area->relocOutP->size, oFHandle) != ap->area->relocOutP->size)
	    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing reloc for area %s", ap->str);
	}
    }

  Symbol_FreeSymbolOut (&symOut);
}

static void
writeElfSH (Elf32_Word nmoffset, unsigned int type, unsigned int flags,
	    unsigned int addr,
	    unsigned int size, unsigned int link, unsigned int info,
	    unsigned int addralign, unsigned int entsize, size_t *offset)
{
  const Elf32_Shdr sect_hdr =
    {
      .sh_name = nmoffset,
      .sh_type = type,
      .sh_flags = flags,
      .sh_addr = addr,
      .sh_offset = type == SHT_NULL ? 0 : *offset,
      .sh_size = size,
      .sh_link = link,
      .sh_info = info,
      .sh_addralign = addralign,
      .sh_entsize = entsize
    };
  if (type != SHT_NOBITS)
    *offset += FIX (size);
  if (fwrite (&sect_hdr, sizeof (sect_hdr), 1, oFHandle) != 1)
    Error_AbortLine (NULL, 0, "Internal writeElfSH: error when writing chunk file header");
}

void
Output_ELF (void)
{
  /* We must call Reloc_GetNumberRelocs() before anything else.  */
  uint32_t numAreas = 0;
  uint32_t numRelocs = 0;
  uint32_t areaSectionID = 3;
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      ++numAreas;

      ap->area->number = areaSectionID++;
      bool anyRelocData = Reloc_PrepareRelocOutPart1 (ap); 
      if (anyRelocData)
	{
	  ++areaSectionID;
	  ++numRelocs;
	}
    }

  Elf32_Ehdr elf_head;
  elf_head.e_ident[EI_MAG0] = ELFMAG0;
  elf_head.e_ident[EI_MAG1] = ELFMAG1;
  elf_head.e_ident[EI_MAG2] = ELFMAG2;
  elf_head.e_ident[EI_MAG3] = ELFMAG3;
  elf_head.e_ident[EI_CLASS] = ELFCLASS32;
  elf_head.e_ident[EI_DATA] = ELFDATA2LSB;
  elf_head.e_ident[EI_VERSION] = EV_CURRENT;
  elf_head.e_ident[EI_OSABI] = ELFOSABI_ARM;
  elf_head.e_ident[EI_ABIVERSION] = 0;
  for (int i = EI_PAD; i < EI_NIDENT; i++)
    elf_head.e_ident[i] = 0;
  elf_head.e_type = ET_REL;
  elf_head.e_machine = EM_ARM;
  elf_head.e_version = EV_CURRENT;
  elf_head.e_entry = areaEntrySymbol ? areaEntryOffset : 0;
  elf_head.e_phoff = 0;
  elf_head.e_shoff = sizeof (elf_head);
#ifdef ELF_EABI
  elf_head.e_flags = EF_ARM_EABI_VER5;
#else
  elf_head.e_flags = 0;
#endif
  /* Softfloat:  elf_head.e_flags |= 0x200; */
  if (areaEntrySymbol)
    elf_head.e_flags |= EF_ARM_HASENTRY;
  elf_head.e_ehsize = sizeof (elf_head);
  elf_head.e_phentsize = 0;
  elf_head.e_phnum = 0;
  elf_head.e_shentsize = sizeof (Elf32_Shdr);
  elf_head.e_shnum = numAreas + numRelocs + 4; /* 4 = "" (SHT_NULL) + ".symtab" (SHT_SYMTAB) + ".strtab" (SHT_STRTAB) + ".shstrtab" (SHT_STRTAB) */
  elf_head.e_shstrndx = numAreas + numRelocs + 3;
  fwrite (&elf_head, 1, sizeof (elf_head), oFHandle);

  /* Section order:
       "" (SHT_NULL)
       ".symtab" (SHT_SYMTAB)
       ".strtab" (SHT_STRTAB) - string table containing the symbol names
       [ "<AreaName>" (SHT_PROGBITS / SHT_NOBITS)
         [ ".rel.<AreaName without leading dot character>" (SHT_REL) ]
       ]*
       ".shstrtab" (SHT_STRTAB) - string table containing section names

     When an area has no relocations, its SHT_REL section is not emitted.  */
  
  size_t offset = sizeof (elf_head) + elf_head.e_shnum * sizeof (Elf32_Shdr);
  Elf32_Word shstrsize = 0;
  
  /* Section headers - index 0 */
  writeElfSH (shstrsize, SHT_NULL, 0, 0, 0, SHN_UNDEF, 0, 0, 0, &offset);
  shstrsize += sizeof ("");

  /* Symbol table - index 1 */
  SymbolOut_t symOut = Symbol_CreateSymbolOut ();
  writeElfSH (shstrsize, SHT_SYMTAB, 0, 0,
	      symOut.symDataSize,
	      2 /* The section header index of the associated string table.  */,
	      symOut.numLocalSymbols,
	      4 /* Align. */,
	      sizeof (Elf32_Sym) /* Entry size.  */,
	      &offset);
  shstrsize += sizeof (".symtab");

  /* String table - index 2 */
  writeElfSH (shstrsize, SHT_STRTAB, 0, 0, symOut.strDataSize, 0, 0, 1, 0, &offset);
  shstrsize += sizeof (".strtab");

  /* Area headers - index 3 */
  int elfIndex = 3;
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      int areaFlags = 0;
      if (ap->area->type & AREA_CODE)
        areaFlags |= SHF_EXECINSTR;
      if (!(ap->area->type & AREA_READONLY))
        areaFlags |= SHF_WRITE;
      if (ap->area->type & AREA_COMMONDEF)
        areaFlags |= SHF_COMDEF;
      if (ap == areaEntrySymbol)
        areaFlags |= SHF_ENTRYSECT;
      areaFlags |= SHF_ALLOC;
      unsigned int sectionSize = FIX (ap->area->maxIdx);
      unsigned int sectionType = !Area_IsNoInit (ap->area) ? SHT_PROGBITS : SHT_NOBITS;
      writeElfSH (shstrsize, sectionType, areaFlags,
		  ap->value.Data.Int.i,
		  sectionSize,
                  0,
		  0,
		  1 << (ap->area->type & AREA_ALIGN_MASK),
		  0,
		  &offset);
      shstrsize += ap->len + 1;

      if (ap->area->relocOutP->num)
        {
          /* Relocations.  */
          writeElfSH (shstrsize, SHT_REL, 0, 0,
	              ap->area->relocOutP->num * sizeof(Elf32_Rel),
	              1, elfIndex, 4, sizeof(Elf32_Rel), &offset);
          shstrsize += sizeof (".rel.")-1 + (ap->str[0] == '.' ? -1 : 0) + ap->len + 1;
          elfIndex++;
        }
      elfIndex++;
    }

  /* Section head string table.  */
  shstrsize += sizeof (".shstrtab");
  writeElfSH (shstrsize - sizeof (".shstrtab"), SHT_STRTAB, 0, 0, shstrsize, 0, 0, 1, 0, &offset);

  /* Symbol table (.symtab).  */
  if (fwrite (symOut.symDataP, 1, symOut.symDataSize, oFHandle) != symOut.symDataSize) 
    Error_AbortLine (NULL, 0, "Internal Output_ELF: error when writing .symtab");    

  /* String table (.strtab).  */
  if (fwrite (symOut.strDataP, 1, symOut.strDataSize, oFHandle) != symOut.strDataSize) 
    Error_AbortLine (NULL, 0, "Internal Output_ELF: error when writing .strtab");    
  for (unsigned pad = EXTRA (symOut.strDataSize); pad; pad--)
    fputc (0, oFHandle);

  /* Areas */
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      if (!Area_IsNoInit (ap->area))
        {
          if (fwrite (ap->area->image, 1, ap->area->maxIdx, oFHandle) != ap->area->maxIdx)
            Error_AbortLine (NULL, 0, "Internal Output_ELF: error when writing data for area %s", ap->str);
	  /* Word align the written area.  */
	  for (unsigned pad = EXTRA (ap->area->curIdx); pad; --pad)
	    fputc (0, oFHandle);
	  Reloc_PrepareRelocOutPart2 (ap);
	  if (fwrite (ap->area->relocOutP->relocs.rawP,
		      1, ap->area->relocOutP->size, oFHandle) != ap->area->relocOutP->size)
	    Error_AbortLine (NULL, 0, "Internal Output_ELF: error when writing reloc for area %s", ap->str);
	}
    }

  /* Section head string table (.shstrtab).  */
  fputc (0, oFHandle);
  fwrite (".symtab", 1, sizeof(".symtab"), oFHandle);
  fwrite (".strtab", 1, sizeof(".strtab"), oFHandle);
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      fwrite (ap->str, 1, ap->len + 1, oFHandle);
      if (ap->area->relocOutP->num)
        {
          fwrite (".rel.", 1, sizeof(".rel.")-1 + (ap->str[0] == '.' ? -1 : 0), oFHandle);
          fwrite (ap->str, 1, ap->len + 1, oFHandle);
        }
    }
  fwrite (".shstrtab", 1, sizeof(".shstrtab"), oFHandle);
  for (unsigned pad = EXTRA (shstrsize); pad; pad--)
    fputc (0, oFHandle);

  Symbol_FreeSymbolOut (&symOut);
#endif
}
