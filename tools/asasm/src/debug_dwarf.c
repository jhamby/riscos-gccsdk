/*
 * AsAsm an assembler for ARM
 * Copyright (c) 2013 GCCSDK Developers
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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dwarf.h"

#include "area.h"
#include "debug_dwarf.h"
#include "error.h"
#include "main.h"
#include "output.h"
#include "os.h"

#define FIX(n) ((3 + (n)) & ~3)

typedef enum
{
  eMarkData_FileName,
  eMarkData_LineNumber,
  eMarkData_Offset
} MarkData_eTypes;

typedef union
{
  const char *strP;
  uint8_t raw[sizeof (const char *)];
} MarkData_FileName_t;

typedef union
{
  unsigned num;
  uint8_t raw[sizeof (unsigned)];
} MarkData_LineNumber_t;

typedef union
{
  uint32_t num;
  uint8_t raw[sizeof (uint32_t)];
} MarkData_Offset_t;

typedef struct DWARF_FileEntry
{
  struct DWARF_FileEntry *nextP;
  Dwarf_Unsigned idx; /** Index returned from dwarf_add_file_decl().  */
  char name[];
} DWARF_FileEntry;

typedef struct DWARF_DirEntry
{
  struct DWARF_DirEntry *nextP;
  struct DWARF_FileEntry *siblP;
  Dwarf_Unsigned idx; /* Index returned from dwarf_add_directory_decl() or 0 in case of CWD.  */
  char name[];
} DWARF_DirEntry;

typedef struct
{
  DWARF_DirEntry *rootP;
  Dwarf_P_Debug dwHandle;
} DWARF_FSObjTree;

static DWARF_UserState_t *oDWUserStateP; /* Temporary points to DWARF user state
  data, only valid during the dwarf_transform_to_disk_form() call.  */


void
DWARF_InitializeState (DWARF_State_t *stateP)
{
  stateP->lastFileNameP = NULL;
  stateP->lastLineNumber = 0;
  stateP->lastOffset = UINT32_MAX; /* Not 0 as we want a mark for first instruction/data entry in area.  */
  stateP->lineDataIdx = 0;
  stateP->lineDataSize = 0;
  stateP->lineDataBufP = NULL;

  stateP->elfRelocDataP = NULL;
}


void
DWARF_FinalizeState (DWARF_State_t *stateP)
{
  free (stateP->lineDataBufP);
  free (stateP->elfRelocDataP);
}


static void
DWARF_MarkData_Store (DWARF_State_t *dwarfStateP, MarkData_eTypes type,
                     const uint8_t *rawP, size_t size)
{
  size_t minSize = dwarfStateP->lineDataIdx + size + 1;
  if (minSize > dwarfStateP->lineDataSize)
    {
      size_t newSize = dwarfStateP->lineDataSize ? 2*dwarfStateP->lineDataSize : 256;
      if (newSize < minSize)
	newSize = minSize;
      uint8_t *newDataP = realloc (dwarfStateP->lineDataBufP, newSize);
      if (newDataP == NULL)
	Error_OutOfMem ();
      dwarfStateP->lineDataSize = newSize;
      dwarfStateP->lineDataBufP = newDataP;
    }
  size_t idx = dwarfStateP->lineDataIdx; 
  dwarfStateP->lineDataBufP[idx++] = (uint8_t) type;
  for (size_t i = 0; i != size; ++i)
    dwarfStateP->lineDataBufP[idx++] = rawP[i];
  dwarfStateP->lineDataIdx = idx;
}


static void
DWARF_MarkData_Load (uint8_t *rawP, const uint8_t *bufP, size_t size)
{
  for (size_t i = 0; i != size; ++i)
    rawP[i] = bufP[i];
}


/**
 * For DWARF .debug_line generation.
 * Does nothing for non-CODE areas.
 */
void
DWARF_MarkAs (const Symbol *areaSymP, uint32_t offset,
	      const char *fileNameP, unsigned lineNumber)
{
  /* Record DWARF debug line data.
     Only for ELF output, when debug is specified, for code and this during
     phase 2.  */
  if ((areaSymP->area->type & AREA_CODE) == 0)
    return;
  if (option_debug && !option_aof && gPhase == ePassTwo)
    {
      DWARF_State_t *dwarfStateP = &areaSymP->area->dwarf;
      if (fileNameP != dwarfStateP->lastFileNameP)
	{
	  const MarkData_FileName_t markData = { .strP = fileNameP };
	  DWARF_MarkData_Store (dwarfStateP, eMarkData_FileName, markData.raw, sizeof (markData.raw));
	  dwarfStateP->lastFileNameP = fileNameP;
	}
      if (lineNumber != dwarfStateP->lastLineNumber)
	{
	  const MarkData_LineNumber_t markData = { .num = lineNumber };
	  DWARF_MarkData_Store (dwarfStateP, eMarkData_LineNumber, markData.raw, sizeof (markData.raw));
	  dwarfStateP->lastLineNumber = lineNumber;
	}
      assert (offset > dwarfStateP->lastOffset || dwarfStateP->lastOffset == UINT32_MAX);
      const MarkData_Offset_t markData = { .num = offset };
      DWARF_MarkData_Store (dwarfStateP, eMarkData_Offset, markData.raw, sizeof (markData.raw));
      dwarfStateP->lastOffset = offset;
    }
}


static DWARF_FileEntry *
FSObjTree_CreateFileEntry (Dwarf_P_Debug dwHandle, Dwarf_Unsigned dirIdx,
                           const char *fnameP)
{
  size_t nameLen = strlen (fnameP) + 1;
  DWARF_FileEntry *rtrn;
  if ((rtrn = malloc (sizeof (DWARF_FileEntry) + nameLen)) == NULL)
    Error_OutOfMem ();
  rtrn->nextP = NULL;
  memcpy (rtrn->name, fnameP, nameLen);
  Dwarf_Error dwErr;
  if ((rtrn->idx = dwarf_add_file_decl (dwHandle, rtrn->name, dirIdx, 0 /* FIXME: mtime ? */, 0 /* FIXME: size ? */, &dwErr)) == (Dwarf_Unsigned)DW_DLV_NOCOUNT)
    Error_Abort ("dwarf_add_file_decl() failed: %s", dwarf_errmsg (dwErr));
  return rtrn;
}


static DWARF_DirEntry *
FSObjTree_CreateDirEntry (Dwarf_P_Debug dwHandle,
                          const char *dnameP, size_t nameLen, bool isCWD)
{
  DWARF_DirEntry *rtrn;
  if ((rtrn = malloc (sizeof (DWARF_DirEntry) + nameLen + 1)) == NULL)
    Error_OutOfMem ();
  rtrn->nextP = NULL;
  rtrn->siblP = NULL;
  memcpy (rtrn->name, dnameP, nameLen);
  rtrn->name[nameLen] = '\0';
  if (!isCWD)
    {
      Dwarf_Error dwErr;
      if ((rtrn->idx = dwarf_add_directory_decl (dwHandle, rtrn->name, &dwErr)) == (Dwarf_Unsigned)DW_DLV_NOCOUNT)
	Error_Abort ("dwarf_add_directory_decl() failed: %s", dwarf_errmsg (dwErr));
    }
  else
    rtrn->idx = 0;
  return rtrn;
}


static void
DWARF_FSObjTree_Init (Dwarf_P_Debug dwHandle, DWARF_FSObjTree *treeP)
{
  const char *cwdP = OS_GetCWD ();
  treeP->rootP = FSObjTree_CreateDirEntry (dwHandle, cwdP, strlen (cwdP), true);
  treeP->dwHandle = dwHandle;
  free ((void *)cwdP);
}


static void
DWARF_FSObjTree_Finalize (DWARF_FSObjTree *treeP)
{
  for (DWARF_DirEntry *dirEntryP = treeP->rootP; dirEntryP != NULL; /* */)
    {
      for (DWARF_FileEntry *fileEntryP = dirEntryP->siblP; fileEntryP != NULL; /* */)
	{
	  DWARF_FileEntry *nextFileEntryP = fileEntryP->nextP;
	  free (fileEntryP);
	  fileEntryP = nextFileEntryP;
	}
      DWARF_DirEntry *nextDirEntryP = dirEntryP->nextP;
      free (dirEntryP);
      dirEntryP = nextDirEntryP;
    }
}


static Dwarf_Unsigned
DWARF_FSObjTree_GetIdx (DWARF_FSObjTree *treeP, const char *fnameP)
{
  const char * const canonFNameP = realpath (fnameP, NULL);

  /* FIXME: on RISC OS, convert RISC OS fspec into Unix syntax ? */
#ifndef __riscos__
  const char *leafFNameP = strrchr (canonFNameP, '/');
#else
  const char *leafFNameP = strrchr (canonFNameP, '.');
  if (leafFNameP == NULL)
    leafFNameP = strrchr (canonFNameP, ':');
#endif
  DWARF_DirEntry *toAddP;
  if (leafFNameP == NULL) /* FIXME: Can this in fact happen ? */
    {
      leafFNameP = canonFNameP;
      toAddP = treeP->rootP;
    }
  else
    {
      size_t dirLenToMatch = leafFNameP - canonFNameP;
      ++leafFNameP;
      toAddP = NULL;
      DWARF_DirEntry *lastDirEntryP = NULL;
      for (DWARF_DirEntry *dirEntryP = treeP->rootP; dirEntryP != NULL; dirEntryP = dirEntryP->nextP)
	{
	  if (!memcmp (dirEntryP->name, canonFNameP, dirLenToMatch)
	      && dirEntryP->name[dirLenToMatch] == '\0')
	    {
	      toAddP = dirEntryP;
	      break;
	    }
	  lastDirEntryP = dirEntryP;
	}
      assert (toAddP != NULL || lastDirEntryP != NULL);
      if (toAddP == NULL)
	toAddP = lastDirEntryP->nextP = FSObjTree_CreateDirEntry (treeP->dwHandle,
								  canonFNameP, dirLenToMatch,
								  false);
    }
  assert (toAddP != NULL);
  DWARF_FileEntry **lastFileEntryPP = &toAddP->siblP;
  DWARF_FileEntry *fileEntryP;
  for (fileEntryP = *lastFileEntryPP; fileEntryP != NULL; fileEntryP = fileEntryP->nextP)
    {
      if (!strcmp (fileEntryP->name, leafFNameP))
	break;
      lastFileEntryPP = &fileEntryP->nextP;
    }
  if (fileEntryP == NULL)
    {
      fileEntryP = FSObjTree_CreateFileEntry (treeP->dwHandle, toAddP->idx,
					      leafFNameP);
      assert (*lastFileEntryPP == NULL);
      *lastFileEntryPP = fileEntryP;
    }

  free ((void *)canonFNameP);

  return fileEntryP->idx;
}


static void
DWARF_DumpDebugLine (Dwarf_P_Debug dwHandle)
{
  DWARF_FSObjTree fsObjTree;
  DWARF_FSObjTree_Init (dwHandle, &fsObjTree);

  for (const Symbol *areaSymP = areaHeadSymbol; areaSymP != NULL; areaSymP = areaSymP->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (areaSymP))
	continue;
      assert (!Area_IsDWARF (areaSymP->area));

      const DWARF_State_t *dwarfStateP = &areaSymP->area->dwarf;

      Dwarf_Error dwErr;
      bool did_set_address = false;
      const char *curFileNameP = NULL;
      Dwarf_Unsigned curFileIdx = 0;
      unsigned curLineNumer = 0;
      uint32_t curOffset = 0;
      for (size_t i = 0; i != dwarfStateP->lineDataIdx; /* */)
	{
	  switch (dwarfStateP->lineDataBufP[i++])
	    {
	      case eMarkData_FileName:
		{
		  MarkData_FileName_t markData;
		  DWARF_MarkData_Load (markData.raw, dwarfStateP->lineDataBufP + i, sizeof (markData.raw));
		  i += sizeof (markData.raw);
		  curFileNameP = markData.strP;
		  curFileIdx = DWARF_FSObjTree_GetIdx (&fsObjTree, curFileNameP);
		  break;
		}

	      case eMarkData_LineNumber:
		{
		  MarkData_LineNumber_t markData;
		  DWARF_MarkData_Load (markData.raw, dwarfStateP->lineDataBufP + i, sizeof (markData.raw));
		  i += sizeof (markData.raw);
		  curLineNumer = markData.num;
		  break;
		}

	      case eMarkData_Offset:
		{
		  MarkData_Offset_t markData;
		  DWARF_MarkData_Load (markData.raw, dwarfStateP->lineDataBufP + i, sizeof (markData.raw));
		  i += sizeof (markData.raw);
		  curOffset = markData.num;

		  if (!did_set_address)
		    {
		      if (dwarf_lne_set_address (dwHandle, curOffset, (Dwarf_Unsigned) areaSymP, &dwErr) != 0)
			Error_Abort ("dwarf_lne_set_address() failed: %s", dwarf_errmsg (dwErr));
		      did_set_address = true;
		    }
		  if (dwarf_add_line_entry (dwHandle, curFileIdx,
		                            (Dwarf_Addr) curOffset,
		                            (Dwarf_Unsigned) curLineNumer,
		                            (Dwarf_Signed) 0 /* FIXME: column_number */,
		                            1, 0,
		                            &dwErr))
		    Error_Abort ("dwarf_add_line_entry() failed: %s", dwarf_errmsg (dwErr));
		  break;
		}

	      default:
		assert (0);
		break;
	    }
	}

      if (did_set_address
          && dwarf_lne_end_sequence (dwHandle, FIX (areaSymP->area->maxIdx) /* FIXME: FIX() needed ? */, &dwErr) != 0)
	Error_Abort ("dwarf_lne_end_sequence() failed: %s", dwarf_errmsg (dwErr));
    }
  DWARF_FSObjTree_Finalize (&fsObjTree);
}


/**
 * DWARF library callback.
 */
static int
DWARF_Callback_CreateSection (char *name, int size, Dwarf_Unsigned type,
			      Dwarf_Unsigned flags, Dwarf_Unsigned link,
			      Dwarf_Unsigned info, Dwarf_Unsigned *index,
			      int *error __attribute__ ((unused)))
{
  bool isRelocSection = !strncmp (name, ".rel.debug_", sizeof (".rel.debug_")-1);

  /* Create ELF section.  */
  Elf_Scn *scn = elf_newscn (oDWUserStateP->elfHandle);
  if (scn == NULL)
    Error_Abort ("elf_newscn() failed");
  Elf32_Shdr *shdr = elf32_getshdr (scn);
  if (shdr == NULL)
    Error_Abort ("elf32_getshdr() failed");
  shdr->sh_name   = oDWUserStateP->shStrTabDataP->idx;
  shdr->sh_type   = type;
  shdr->sh_flags  = flags;
  //shdr->sh_addr   = 0;
  //shdr->sh_offset = 0;
  shdr->sh_size   = size;
  shdr->sh_link   = isRelocSection ? oDWUserStateP->symTabScnIdx : link;
  shdr->sh_info   = info;
  shdr->sh_addralign = 1;
  shdr->sh_entsize = isRelocSection ? sizeof (Elf32_Rel) : 0;
  
  shstrtab_add (oDWUserStateP->shStrTabDataP, name, strlen (name) + 1);
  oDWUserStateP->shStrTabSizeForDWARF += strlen (name) + 1;
  
  /* Create AREA holding the DWARF data, only for ".debug_*"  */
  if (!isRelocSection)
    {
      Symbol *areaSymP = Area_CreateDWARF (name, elf_ndxscn (scn));
    
      /* Return as symbol index the our area symbol ptr.  At reloc enumeration
         time, we will convert this to the real symbol index which only gets
         determined at Symbol_CreateSymbolOut() time.  */
      *index = (Dwarf_Unsigned) areaSymP;
    }
  else
    *index = 0;

  /* Return ELF section idx.  */
  return elf_ndxscn (scn);
}

void
DWARF_CreateAreasAndSections (DWARF_UserState_t *dwUserStateP)
{
  Dwarf_Unsigned flags = DW_DLC_SIZE_32 | DW_DLC_SYMBOLIC_RELOCATIONS | DW_DLC_TARGET_LITTLEENDIAN | DW_DLC_WRITE;
  Dwarf_Error dwErr;
  Dwarf_P_Debug dwHandle = dwarf_producer_init_b (flags, DWARF_Callback_CreateSection, NULL, NULL, &dwErr);
  if (dwHandle == DW_DLV_BADADDR)
    Error_Abort ("dwarf_producer_init_b() failed: %s", dwarf_errmsg (dwErr));

  if (dwarf_producer_set_isa (dwHandle, DW_ISA_ARM, &dwErr) != DW_DLV_OK)
    Error_Abort ("dwarf_producer_set_isa() failed: %s", dwarf_errmsg (dwErr));

  /* Create top level DIE.   */
  Dwarf_P_Die compileUnitDieP = dwarf_new_die (dwHandle, DW_TAG_compile_unit, NULL, NULL, NULL, NULL, &dwErr);
  if (compileUnitDieP == DW_DLV_BADADDR)
    Error_Abort ("dwarf_new_die() failed: %s", dwarf_errmsg (dwErr));

  /* Write DW_AT_comp_dir.  */
  const char *cwd = OS_GetCWD ();
  if (dwarf_add_AT_comp_dir (compileUnitDieP, (char * /* yuck! */)cwd, &dwErr) == DW_DLV_BADADDR)
    Error_Abort ("dwarf_add_AT_comp_dir() failed: %s", dwarf_errmsg (dwErr));
  free ((void *)cwd);

  /* Write DW_AT_name.  */
  /* FIXME: the input file can be found via an user supplied include path.  In
     that case we better give the fully resolved input filename.  */
  if (dwarf_add_AT_name (compileUnitDieP, (char * /* yuck! */)gSourceFileName_Canon, &dwErr) == DW_DLV_BADADDR)
    Error_Abort ("dwarf_add_AT_name() failed: %s", dwarf_errmsg (dwErr));

  /* Write DW_AT_producer.  */
  if (dwarf_add_AT_producer (compileUnitDieP, (char * /* yuck! */)DEFAULT_IDFN, &dwErr) == DW_DLV_BADADDR)
    Error_Abort ("dwarf_add_AT_producer() failed: %s", dwarf_errmsg (dwErr));

  /* Write DW_AT_language.  */
#ifndef DW_LANG_Mips_Assembler
# define DW_LANG_Mips_Assembler 0x8001
#endif
  if (dwarf_add_AT_unsigned_const (dwHandle, compileUnitDieP, DW_AT_language, DW_LANG_Mips_Assembler, &dwErr) == DW_DLV_BADADDR)
    Error_Abort ("dwarf_add_AT_const_value_unsignedint() failed: %s", dwarf_errmsg (dwErr));
  
  if (dwarf_add_die_to_debug (dwHandle, compileUnitDieP, &dwErr) != 0)
    Error_Abort ("dwarf_add_die_to_debug() failed: %s", dwarf_errmsg (dwErr));

  /* Write DW_AT_low_pc/DW_AT_high_pc (when compilation unit consists of one
     area), or DW_AT_ranges (FIXME: not done yet).  */
  unsigned numAreas = 0;
  const Symbol *areaSymP = NULL;
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      assert (!Area_IsDWARF (ap->area));

      areaSymP = ap;
      numAreas += 1;
    }
  assert (numAreas > 0); /* Otherwise we don't dump DWARF data.  */
  if (numAreas == 1)
    {
      if (dwarf_add_AT_targ_address_b (dwHandle, compileUnitDieP, DW_AT_low_pc, 0, (Dwarf_Unsigned) areaSymP, &dwErr) == NULL)
	Error_Abort ("dwarf_add_AT_targ_address_b() failed: %s", dwarf_errmsg (dwErr));
      if (dwarf_add_AT_targ_address_b (dwHandle, compileUnitDieP, DW_AT_high_pc, FIX (areaSymP->area->maxIdx) /* FIXME: Is FIX() needed ? */, (Dwarf_Unsigned) areaSymP, &dwErr) == NULL)
	Error_Abort ("dwarf_add_AT_targ_address_b() failed: %s", dwarf_errmsg (dwErr));
    }
  else
    {
      /* FIXME: ... */
    }

  /* Add .debug_line section.  */
  DWARF_DumpDebugLine (dwHandle);
  
  /* Add .debug_aranges section.  One entry for each area.  */
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      assert (!Area_IsDWARF (ap->area));

      if (dwarf_add_arange (dwHandle, 0, FIX (ap->area->maxIdx) /* FIXME: FIX() needed ? */, (Dwarf_Signed) ap, &dwErr) == 0)
	Error_Abort ("dwarf_add_arange() failed: %s", dwarf_errmsg (dwErr));
    }

  /* This will trigger our DWARF callback.  */
  dwUserStateP->dwHandle = dwHandle;
  oDWUserStateP = dwUserStateP;
  dwUserStateP->numDWSections = dwarf_transform_to_disk_form (dwHandle, &dwErr);
  oDWUserStateP = NULL;
  if (dwUserStateP->numDWSections == DW_DLV_NOCOUNT)
    Error_Abort ("dwarf_transform_to_disk_form() failed: %s", dwarf_errmsg (dwErr));
}


void
DWARF_OutputSectionData (DWARF_UserState_t *dwUserStateP)
{
  Dwarf_P_Debug dwHandle = dwUserStateP->dwHandle;
  Dwarf_Error dwErr;
  for (Dwarf_Signed dwSectIdx = 0; dwSectIdx != dwUserStateP->numDWSections; ++dwSectIdx)
    {
      Dwarf_Signed elfSectIdx;
      Dwarf_Unsigned elfSectLen;
      Dwarf_Ptr ptr = dwarf_get_section_bytes (dwHandle, dwSectIdx, &elfSectIdx,
					       &elfSectLen, &dwErr);
      assert (ptr != NULL);

      /* Link DWARF section data into ELF data structure.  */
      Elf_Scn *dwScnP;
      if ((dwScnP = elf_getscn (dwUserStateP->elfHandle, elfSectIdx)) == NULL)
	Error_Abort ("elf_getscn() failed");
      Elf_Data *dwDataP;
      if ((dwDataP = elf_newdata (dwScnP)) == NULL)
	Error_Abort ("elf_newdata() failed");
      dwDataP->d_align = 1;
      dwDataP->d_buf = ptr;
      /* dwDataP->d_off */
      dwDataP->d_size = elfSectLen;
      dwDataP->d_type = ELF_T_BYTE;
      /* dwDataP->d_version */
    }

  /* Relocation DWARF sections.  */
  Dwarf_Unsigned numRelocSections;
  int drdVersion;
  if (dwarf_get_relocation_info_count (dwHandle, &numRelocSections, &drdVersion, &dwErr) != DW_DLV_OK
      || drdVersion != 2) 
    Error_Abort ("dwarf_get_relocation_info_count() failed: %s", dwarf_errmsg (dwErr));
  for (Dwarf_Unsigned i = 0; i != numRelocSections; ++i)
    {
      /* DWARF section 'elfSectLinkIdx' gets one or more relocations based on
         its relocation section 'elfRelocSectIdx'.  */
      Dwarf_Signed elfRelocSectIdx;
      Dwarf_Signed elfSectLinkIdx; /* not used.  */
      Dwarf_Unsigned relocCount;
      Dwarf_Relocation_Data relocData;
      if (dwarf_get_relocation_info (dwHandle, &elfRelocSectIdx, &elfSectLinkIdx,
				     &relocCount, &relocData, &dwErr) != DW_DLV_OK)
	Error_Abort ("dwarf_get_relocation_info() failed: %s", dwarf_errmsg (dwErr));

      const size_t elfRelocDataSize = relocCount * sizeof (Elf32_Rel);
      Elf32_Rel *elfRelocDataP = malloc (elfRelocDataSize);
      if (elfRelocDataP == NULL)
	Error_OutOfMem ();
      /* Store copy of elfRelocDataP so it can get deallocated at the end.  */
      Symbol *dwarfAreaP = Area_FindDWARF (elfSectLinkIdx);
      dwarfAreaP->area->dwarf.elfRelocDataP = elfRelocDataP;
      for (Dwarf_Unsigned relocIdx = 0; relocIdx != relocCount; ++relocIdx)
	{
	  struct Dwarf_Relocation_Data_s *relocRecordP = &relocData[relocIdx];
	  /* drd_type can be dwarf_drt_none, dwarf_drt_data_reloc, dwarf_drt_segment_rel,
	     dwarf_drt_first_of_length_pair, dwarf_drt_second_of_length_pair.  */
	  assert (relocRecordP->drd_type == dwarf_drt_data_reloc && relocRecordP->drd_length == 4);
	  const Symbol *relocSymP = (const Symbol *)relocRecordP->drd_symbol_index;
	  assert (relocSymP->used >= 0);
	  elfRelocDataP[relocIdx].r_offset = relocRecordP->drd_offset;
	  elfRelocDataP[relocIdx].r_info = ELF32_R_INFO (relocSymP->used, R_ARM_ABS32);
	}

      /* Link DWARF relocation section data into ELF data structure.  */
      Elf_Scn *dwRelScnP;
      if ((dwRelScnP = elf_getscn (dwUserStateP->elfHandle, elfRelocSectIdx)) == NULL)
	Error_Abort ("elf_getscn() failed");
      Elf_Data *dwRelScnDataP;
      if ((dwRelScnDataP = elf_newdata (dwRelScnP)) == NULL)
	Error_Abort ("elf_newdata() failed");
      dwRelScnDataP->d_align = 4;
      dwRelScnDataP->d_buf = elfRelocDataP;
      /* dwRelScnDataP->d_off */
      dwRelScnDataP->d_size = elfRelocDataSize;
      dwRelScnDataP->d_type = ELF_T_REL;
      /* dwRelScnDataP->d_version */
    }
}

void
DWARF_Finish (DWARF_UserState_t *dwUserStateP)
{
  Dwarf_Error dwErr;
  if (dwarf_producer_finish (dwUserStateP->dwHandle, &dwErr) == (Dwarf_Unsigned)DW_DLV_NOCOUNT)
    Error_Abort ("dwarf_producer_finish() failed: %s", dwarf_errmsg (dwErr));
  dwUserStateP->dwHandle = 0;
}
