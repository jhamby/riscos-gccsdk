/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Nick Burrett
 * Copyright (c) 2000-2014 GCCSDK Developers
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
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __riscos__
#  include <swis.h>
#endif
#include <fcntl.h>
#include <unistd.h>

#include "libelf.h"

#include "aoffile.h"
#include "area.h"
#include "chunkfile.h"
#include "debug_dwarf.h"
#include "depend.h"
#include "error.h"
#include "filename.h"
#include "main.h"
#include "output.h"
#include "os.h"
#include "reloc.h"
#include "symbol.h"

static FILE *oFHandle;
static int oFHandleELF = -1;

#define FIX(n) ((3 + (n)) & ~3)
#define EXTRA(n) (FIX(n)-(n))

#define GET_IDFN (idfn_text ? idfn_text : DEFAULT_IDFN)
const char *idfn_text = NULL; /**< Identifier, when NULL use DEFAULT_IDFN; this is a malloced string.  */

static char outname[PATH_MAX];

/**
 * Opens the AOF/ELF file for output.
 * \return true in case of error.
 */
static bool
Output_Open (const char *fname)
{
  if (option_aof)
    return (oFHandle = fopen (fname, "wb")) == NULL;
  return (oFHandleELF = open (fname, O_WRONLY | O_CREAT, 0777)) < 0;
}


const char *
Output_OpenOutput (const char *outfile)
{
  oFHandle = NULL;
  oFHandleELF = -1;
  for (unsigned pathidx = 0; /* */; ++pathidx)
    {
      const char *out[3];
      bool state[3] = { false, false, false };

      do
	{
	  out[0] = FN_AnyToNative (outfile, pathidx, outname, sizeof (outname),
				   &state[0], eA_Dot_B);
	  if (out[0] && !Output_Open (out[0]))
	    return outname;

	  out[1] = FN_AnyToNative (outfile, pathidx, outname, sizeof (outname),
				   &state[1], eB_DirSep_A);
	  if (out[1] && !Output_Open (out[1]))
	    return outname;

	  out[2] = FN_AnyToNative (outfile, pathidx, outname, sizeof (outname),
				   &state[2], eA_Slash_B);
	  if (out[2] && !Output_Open (out[2]))
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
#ifdef __riscos__
	  bool doFileType = false;
#endif
	  if (oFHandle != NULL)
	    {
	      fclose (oFHandle);
	      oFHandle = NULL;
#ifdef __riscos__
	      doFileType = true;
#endif
	    }
	  if (oFHandleELF >= 0)
	    {
	      close (oFHandleELF);
	      oFHandleELF = -1;
#ifdef __riscos__
	      doFileType = true;
#endif
	    }
#ifdef __riscos__
	  if (doFileType)
	    {
	      /* Set filetype to 0xE1F (ELF, ELF output) or 0xFFD (Data, AOF output).  */
	      _kernel_swi_regs regs;
	      regs.r[0] = 18;
	      regs.r[1] = (int) outname;
	      regs.r[2] = (option_aof) ? 0xFFD : 0xE1F;

	      _kernel_swi (OS_File, &regs, &regs);
	    }
#endif
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
  if (oFHandleELF >= 0)
    {
      close (oFHandleELF);
      oFHandleELF = -1;
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
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->attr.area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      ap->attr.area->number = numAreas++;
      if (!Area_IsNoInit (ap->attr.area))
	totalAreaSize += FIX (ap->attr.area->maxIdx);
      totalAreaSize += Reloc_GetRawRelocSize (&ap->attr.area->reloc);
    }

  SymbolOut_t symOut = Symbol_CreateSymbolOut ();
  
  const AofHeader aof_head =
    {
      .Type = armword (AofHeaderID),
      .Version = armword (310),
      .noAreas = armword (numAreas),
      .noSymbols = armword (symOut.numAllSymbols),
      .EntryArea = armword (areaEntrySymbol ? areaEntrySymbol->attr.area->number + 1 : 0),
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
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->attr.area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      const AofEntry aof_entry =
	{
	  .Name = armword (ap->attr.offset),
          .Type = armword (ap->attr.area->type & AREA_INT_AOFMASK),
          .Size = armword (FIX (ap->attr.area->maxIdx)),
          .noRelocations = armword (Reloc_GetNumRelocs (&ap->attr.area->reloc)),
          .BaseAddr = armword ((ap->attr.area->type & AREA_ABS) ? Area_GetBaseAddress (ap) : 0)
	};
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
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->attr.area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      if (!Area_IsNoInit (ap->attr.area))
	{
	  if (fwrite (ap->attr.area->image, 1, ap->attr.area->maxIdx, oFHandle) != ap->attr.area->maxIdx)
	    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing data for area %s", ap->str);
	  /* Word align the written area.  */
	  for (unsigned pad = EXTRA (ap->attr.area->maxIdx); pad; --pad)
	    fputc (0, oFHandle);
	}
      void *rawRelocP = Reloc_GetRawRelocData (&ap->attr.area->reloc);
      if (rawRelocP != NULL)
	{
	  size_t len = Reloc_GetRawRelocSize (&ap->attr.area->reloc);
	  if (fwrite (rawRelocP, 1, len, oFHandle) != len)
	    Error_AbortLine (NULL, 0, "Internal Output_AOF: error when writing reloc for area %s", ap->str);
	}
    }

  Symbol_FreeSymbolOut (&symOut);
}


void
Output_ELF (void)
{
  if (elf_version (EV_CURRENT) == EV_NONE)
    Error_Abort ("elf_version() failed");

  Elf *elfHandle;
  if ((elfHandle = elf_begin (oFHandleELF, ELF_C_WRITE, NULL)) == NULL)
    Error_Abort ("Failed to get ELF handle");

  Elf32_Ehdr *ehdr;
  if ((ehdr = elf32_newehdr (elfHandle)) == NULL)
    Error_Abort ("elf32_newehdr() failed");

  ehdr->e_ident[EI_CLASS] = ELFCLASS32;
  ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
  ehdr->e_ident[EI_VERSION] = EV_CURRENT;
  ehdr->e_ident[EI_OSABI] = ELFOSABI_ARM;
  ehdr->e_ident[EI_ABIVERSION] = 0;
  ehdr->e_type = ET_REL;
  ehdr->e_machine = EM_ARM;
  ehdr->e_version = EV_CURRENT;
  ehdr->e_entry = areaEntrySymbol ? areaEntryOffset : 0;
#ifdef ELF_EABI
  ehdr->e_flags = EF_ARM_EABI_VER5;
#endif

  /* Section order:

       "" (SHT_NULL)
       ".symtab" (SHT_SYMTAB)
       ".strtab" (SHT_STRTAB) - string table containing the symbol names
       [ "<AreaName>" (SHT_PROGBITS / SHT_NOBITS)
         [ ".rel.<AreaName without leading dot character>" (SHT_REL) ]
       ]*
       [ ".debug_*" (SHT_PROGBITS) - DWARF section
         [ ".rel.debug_*" (SHT_REL) - DWARF relocation section ]
       ]*
       ".shstrtab" (SHT_STRTAB) - string table containing section names

     When an area has no relocations, its SHT_REL section is not emitted.  */

  /* Section 0 : "".  */
  shstrtab_t shStrTabData = SHSTRTAB_INIT_VALUE;
  shstrtab_add (&shStrTabData, "", sizeof (""));

  /* Section 1 & 2 : ".symtab", ".strtab".  */
  Elf_Scn *symTabScnP, *strTabScnP;
  if ((symTabScnP = elf_newscn (elfHandle)) == NULL
      || (strTabScnP = elf_newscn (elfHandle)) == NULL)
    Error_Abort ("elf_newscn() failed");
  shstrtab_add (&shStrTabData, ".symtab\0.strtab", sizeof (".symtab\0.strtab"));

  /* Sections per area and its relocs.  */
  bool atLeastOneArea = false;
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->attr.area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      atLeastOneArea = true;

      Elf_Scn *areaScnP;
      if ((areaScnP = elf_newscn (elfHandle)) == NULL)
	Error_Abort ("elf_newscn() failed");
      ap->attr.area->number = elf_ndxscn (areaScnP);
      shstrtab_add (&shStrTabData, ap->str, ap->len + 1); 
      if (Reloc_GetNumRelocs (&ap->attr.area->reloc) != 0)
	{
	  Elf_Scn *areaRelScnP;
	  if ((areaRelScnP = elf_newscn (elfHandle)) == NULL)
	    Error_Abort ("elf_newscn() failed");
	  shstrtab_add (&shStrTabData, ".rel.", sizeof (".rel.")-1 + (ap->str[0] == '.' ? -1 : 0));
	  shstrtab_add (&shStrTabData, ap->str, ap->len + 1);
	}
    }

  /* Sections for DWARF.  */
  DWARF_UserState_t dwUserState =
    {
      .elfHandle = elfHandle,
      .symTabScnIdx = elf_ndxscn (symTabScnP),
      .shStrTabDataP = &shStrTabData,
      .shStrTabSizeForDWARF = 0
    };
  if (option_debug && atLeastOneArea)
    DWARF_CreateAreasAndSections (&dwUserState);

  /* Section ".shstrtab".  */
  Elf_Scn *shStrTabScnP;
  if ((shStrTabScnP = elf_newscn (elfHandle)) == NULL)
    Error_Abort ("elf_newscn() failed");
  shstrtab_add (&shStrTabData, ".shstrtab", sizeof (".shstrtab"));

  if (elf_setshstrndx (elfHandle , elf_ndxscn (shStrTabScnP)) == 0)
    Error_Abort ("elf_setshstrndx() failed");

  SymbolOut_t symOut = Symbol_CreateSymbolOut ();

  /* Write section 0 data (i.e. do nothing).  */
  Elf32_Word scnNameIdx = sizeof ("");
  
  /* Write .symtab section data.  */
  Elf_Data *symTabDataP;
  if ((symTabDataP = elf_newdata (symTabScnP)) == NULL)
    Error_Abort ("elf_newdata() failed");
  symTabDataP->d_align = 4;
  symTabDataP->d_buf = symOut.symDataP;
  /* symTabDataP->d_off */
  symTabDataP->d_size = symOut.symDataSize;
  symTabDataP->d_type = ELF_T_SYM;
  /* symTabDataP->d_version */

  Elf32_Shdr *symTabSHdrP;
  if ((symTabSHdrP = elf32_getshdr (symTabScnP)) == NULL)
    Error_Abort ("elf32_getshdr() failed");
  symTabSHdrP->sh_name = scnNameIdx;
  symTabSHdrP->sh_type = SHT_SYMTAB;
  symTabSHdrP->sh_link = elf_ndxscn (strTabScnP); /* Section header index of
    the assocated string table.  */
  symTabSHdrP->sh_info = symOut.numLocalSymbols; /* Index of the first global
    symbol.  */
  symTabSHdrP->sh_entsize = sizeof (Elf32_Sym);

  scnNameIdx += sizeof (".symtab");

  /* Write .strtab section data.  */
  Elf_Data *strTabDataP;
  if ((strTabDataP = elf_newdata (strTabScnP)) == NULL)
    Error_Abort ("elf_newdata() failed");
  strTabDataP->d_align = 1;
  strTabDataP->d_buf = symOut.strDataP;
  /* strTabDataP->d_off */
  strTabDataP->d_size = symOut.strDataSize;
  strTabDataP->d_type = ELF_T_BYTE;
  /* strTabDataP->d_version */

  Elf32_Shdr *strTabSHdrP;
  if ((strTabSHdrP = elf32_getshdr (strTabScnP)) == NULL)
    Error_Abort ("elf32_getshdr() failed");
  strTabSHdrP->sh_name = scnNameIdx;
  strTabSHdrP->sh_type = SHT_STRTAB;

  scnNameIdx += sizeof (".strtab");

  /* Write area data and their relocs as separate sections.  */
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->attr.area->next)
    {
      /* Skip the implicit area and the conveniance DWARF areas we've
         created.  */
      if (Area_IsImplicit (ap) || Area_IsDWARF (ap->attr.area))
	continue;

      Elf32_Word scnFlags = 0;
      if (ap->attr.area->type & AREA_CODE)
        scnFlags |= SHF_EXECINSTR;
      if (!(ap->attr.area->type & AREA_READONLY))
        scnFlags |= SHF_WRITE;
      if (ap->attr.area->type & AREA_COMMONDEF)
        scnFlags |= SHF_COMDEF;
      if (ap == areaEntrySymbol)
        scnFlags |= SHF_ENTRYSECT;
      scnFlags |= SHF_ALLOC;

      Elf_Scn *areaScnP;
      if ((areaScnP = elf_getscn (elfHandle, ap->attr.area->number)) == NULL)
	Error_Abort ("elf_getscn() failed");
      Elf_Data *areaDataP;
      if ((areaDataP = elf_newdata (areaScnP)) == NULL)
	Error_Abort ("elf_newdata() failed");
      areaDataP->d_align = 1 << (ap->attr.area->type & AREA_ALIGN_MASK);
      areaDataP->d_buf = ap->attr.area->image; /* For AREA_UDATA, this is NULL.  */
      /* areaDataP->d_off */
      areaDataP->d_size = FIX (ap->attr.area->maxIdx); /* FIXME: do we always need to round up area section size ? */
      areaDataP->d_type = ELF_T_BYTE;
      /* areaDataP->d_version */

      Elf32_Shdr *areaSHdrP;
      if ((areaSHdrP = elf32_getshdr (areaScnP)) == NULL)
	Error_Abort ("elf32_getshdr() failed");
      areaSHdrP->sh_name = scnNameIdx;
      areaSHdrP->sh_type = !Area_IsNoInit (ap->attr.area) ? SHT_PROGBITS : SHT_NOBITS;
      areaSHdrP->sh_flags = scnFlags;
      areaSHdrP->sh_addr = 0; /* FIXME: ORG should be implemented differently then ap->value.Data.Int.i; */

      scnNameIdx += ap->len + 1;

      void *rawRelocP = Reloc_GetRawRelocData (&ap->attr.area->reloc);
      if (rawRelocP != NULL)
        {
	  Elf_Scn *areaRelScnP;
	  if ((areaRelScnP = elf_getscn (elfHandle, ap->attr.area->number + 1)) == NULL)
	    Error_Abort ("elf_getscn() failed");
	  Elf_Data *areaRelDataP;
	  if ((areaRelDataP = elf_newdata (areaRelScnP)) == NULL)
	    Error_Abort ("elf_newdata() failed");
	  areaRelDataP->d_align = 4;
	  areaRelDataP->d_buf = rawRelocP;
	  /* areaRelDataP->d_off */
	  areaRelDataP->d_size = Reloc_GetRawRelocSize (&ap->attr.area->reloc);
	  areaRelDataP->d_type = ELF_T_REL;
	  /* areaRelDataP->d_version */

	  Elf32_Shdr *areaRelSHdrP;
	  if ((areaRelSHdrP = elf32_getshdr (areaRelScnP)) == NULL)
	    Error_Abort ("elf32_getshdr() failed");
	  areaRelSHdrP->sh_name = scnNameIdx;
	  areaRelSHdrP->sh_type = SHT_REL;
	  areaRelSHdrP->sh_link = elf_ndxscn (symTabScnP); /* Section header
	    index of the associated symbol table.  */
	  areaRelSHdrP->sh_info = ap->attr.area->number; /* The section header index
	    of the section to which the relocation applies.  */
	  areaRelSHdrP->sh_entsize = sizeof (Elf32_Rel);

	  scnNameIdx += sizeof(".rel.")-1 + (ap->str[0] == '.' ? -1 : 0) + ap->len + 1; 
	}
    }

  /* Write the DWARF sections.  */
  if (option_debug && atLeastOneArea)
    {
      DWARF_OutputSectionData (&dwUserState);
      scnNameIdx += dwUserState.shStrTabSizeForDWARF;
    }

  /* Write .shstrtab section data.  */
  Elf_Data *shStrTabDataP;
  if ((shStrTabDataP = elf_newdata (shStrTabScnP)) == NULL)
    Error_Abort ("elf_newdata() failed");
  shStrTabDataP->d_align = 1;
  shStrTabDataP->d_buf = shStrTabData.bufP;
  /* shStrTabDataP->d_off */
  shStrTabDataP->d_size = shStrTabData.idx;
  shStrTabDataP->d_type = ELF_T_BYTE;
  /* shStrTabDataP->d_version */

  Elf32_Shdr *shStrTabSHdrP;
  if ((shStrTabSHdrP = elf32_getshdr (shStrTabScnP)) == NULL)
    Error_Abort ("elf32_getshdr() failed");
  shStrTabSHdrP->sh_name = scnNameIdx;
  shStrTabSHdrP->sh_type = SHT_STRTAB;

  scnNameIdx += sizeof (".shstrtab");
  assert (scnNameIdx == shStrTabData.idx);

  /* Flush all data to file.  */
  if (elf_update (elfHandle, ELF_C_WRITE) < 0)
    Error_Abort ("elf_update() failed");

  /* We're done.  */
  if (option_debug && atLeastOneArea)
    DWARF_Finish (&dwUserState);
  shstrtab_finish (&shStrTabData);
  elf_end (elfHandle);

  Symbol_FreeSymbolOut (&symOut);
}

/**
 * Add given string to the .shstrtab section data.
 * \param strLen Length of the string *including* terminating \0 if
 * necessary.
 */
void
shstrtab_add (shstrtab_t *shStrTabP, const char *str, size_t strLen)
{
  if (shStrTabP->idx + strLen > shStrTabP->size)
    {
      size_t newSize = 2*shStrTabP->size + 32;
      char *newBufP = realloc (shStrTabP->bufP, newSize);
      if (newBufP == NULL)
	Error_OutOfMem ();
      shStrTabP->size = newSize;
      shStrTabP->bufP = newBufP;
    }
  memcpy (&shStrTabP->bufP[shStrTabP->idx], str, strLen);
  shStrTabP->idx += strLen;
}

void
shstrtab_finish (shstrtab_t *shStrTabP)
{
  free (shStrTabP->bufP);
  shStrTabP->idx = 0;
  shStrTabP->size = 0;
  shStrTabP->bufP = NULL;
}
