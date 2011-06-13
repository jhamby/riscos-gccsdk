/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Nick Burrett
 * Copyright (c) 2000-2011 GCCSDK Developers
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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "aoffile.h"
#include "area.h"
#include "chunkfile.h"
#include "depend.h"
#include "elf.h"
#include "error.h"
#include "output.h"
#include "os.h"
#include "main.h"
#include "symbol.h"

#ifdef __riscos__
#  include <swis.h>
#endif
#if !defined(__TARGET_SCL__)
#  include <ctype.h>
#endif

static FILE *objfile;

#define FIX(n) ((3+(int)(n))&~3)
#define EXTRA(n) (FIX(n)-(n))

#define GET_IDFN ((idfn_text) ? idfn_text : DEFAULT_IDFN)
const char *idfn_text = NULL; /**< Identifier, when NULL use DEFAULT_IDFN; this is a malloced string.  */

#define MAXNAME 1024
static char outname[MAXNAME + 1];

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

void
outputInit (const char *outfile)
{
  objfile = NULL;
  if (outfile && !(outfile[0] == '-' && outfile[1] == '\0'))
    {
      strncpy (outname, outfile, MAXNAME);

#ifdef __riscos__
      /* Smell if we have outfile in RISC OS filename syntax.  If so,
         convert it to Unix syntax.  */
      char *temp = strchr (outname, '.');
      if (temp != NULL)
	{
	  *temp = '\0';
	  _kernel_osfile_block blk;
          bool isROSyntax = _kernel_osfile (17, outname, &blk) == 2;
	  *temp = '.';
	  if (isROSyntax)
	    {
	      for (temp = outname; *temp; ++temp)
		{
		  if (*temp == '.')
		    *temp = '/';
		  else if (*temp == '/')
		    *temp = '.';
		}
	    }
	}
#endif /* ! __riscos__ */

      if ((objfile = fopen (outname, "wb")) == NULL)
	errorAbort (PACKAGE_NAME " can't write %s: %s", outname, strerror (errno));
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
    {
      fclose (objfile);
      objfile = NULL;
#ifdef __riscos__
      /* Set filetype to 0xE1F (ELF, ELF output) or 0xFFF (Text, AOF output).  */
      _kernel_swi_regs regs;
      regs.r[0] = 18;
      regs.r[1] = (int) outname;
      regs.r[2] = (option_aof) ? 0xFFF : 0xE1F;

      _kernel_swi(OS_File, &regs, &regs);
#endif
    }
  if (outname[0])
    dependWrite (outname);
}

void
outputRemove (void)
{
  outputFinish ();
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
  return fwrite (&chunk_entry, 1, sizeof (chunk_entry), objfile);
}

void
outputAof (void)
{
  /* We must call relocFix() before anything else.  */
  int obj_area_size = 0;
  int noareas = 0;
  /* avoid problems with no areas.  */
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      ap->used = noareas++;

      ap->area.info->norelocs = relocFix (ap);
      if (AREA_IMAGE (ap->area.info))
	obj_area_size += FIX (ap->value.Data.Int.i)
			  + ap->area.info->norelocs * sizeof (AofReloc);
    }

  size_t stringSizeNeeded;
  const AofHeader aof_head =
    {
      .Type = armword (AofHeaderID),
      .Version = armword (310),
      .noAreas = armword (noareas),
      .noSymbols = armword (symbolFix (&stringSizeNeeded)),
      .EntryArea = armword (areaEntrySymbol ? areaEntrySymbol->used + 1 : 0),
      .EntryOffset = armword (areaEntrySymbol ? areaEntryOffset : 0)
    };

  /* Write out the chunk file header.  */
  const ChunkFileHeader chunk_header =
    {
      .ChunkField = armword (ChunkFileID),
      .maxChunks = armword (5),
      .noChunks = armword (5)
    };
  size_t written = fwrite (&chunk_header, 1, sizeof (chunk_header), objfile);
  size_t offset = sizeof (chunk_header) + 5*sizeof (ChunkFileHeaderEntry);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_HEAD,
			 sizeof (AofHeader) + noareas*sizeof (AofEntry), &offset);

  size_t idfn_size = FIX (strlen (GET_IDFN) + 1);
  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_IDFN, idfn_size, &offset);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_STRT,
                         FIX (stringSizeNeeded) + 4, &offset);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_SYMT,
                         ourword (aof_head.noSymbols)*sizeof (AofSymbol), &offset);

  written += writeEntry (ChunkID_OBJ, ChunkID_OBJ_AREA,
                         obj_area_size, &offset);

  if (written != sizeof (chunk_header) + 5*sizeof (ChunkFileHeaderEntry))
    {
      errorAbortLine (NULL, 0, "Internal outputAof: error when writing chunk file header");
      return;
    }

  /******** Chunk 0 Header ********/
  fwrite (&aof_head, 1, sizeof (aof_head), objfile);
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      const AofEntry aof_entry =
	{
	  .Name = armword (ap->offset + 4), /* +4 because of extra length entry */
          .Type = armword (ap->area.info->type),
          .Size = armword (FIX (ap->value.Data.Int.i)),
          .noRelocations = armword (ap->area.info->norelocs),
          .BaseAddr = armword (ap->area.info->baseAddr)
	};
      if (aof_entry.noRelocations != 0 && !AREA_IMAGE (ap->area.info))
	errorAbortLine (NULL, 0, "Internal outputAof: relocations in uninitialised area");
      fwrite (&aof_entry, 1, sizeof (aof_entry), objfile);
    }

  /******** Chunk 1 Identification *********/
  if (idfn_size != fwrite (GET_IDFN, 1, idfn_size, objfile))
    {
      errorAbortLine (NULL, 0, "Internal outputAof: error when writing identification");
      return;
    }
  /******** Chunk 2 String Table ***********/
  unsigned int strt_size = armword (stringSizeNeeded + 4);
  if (fwrite (&strt_size, 1, 4, objfile) != sizeof (strt_size))
    {
      errorAbortLine (NULL, 0, "Internal outputAof: error when writing string table size");
      return;
    }
  symbolStringOutput (objfile, stringSizeNeeded);
  for (int pad = EXTRA (stringSizeNeeded); pad; pad--)
    fputc (0, objfile);

  /******** Chunk 3 Symbol Table ***********/
  symbolSymbolAOFOutput (objfile);

  /******** Chunk 4 Area *****************/
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      if (AREA_IMAGE (ap->area.info))
	{
	  if (fwrite (ap->area.info->image, 1, ap->value.Data.Int.i, objfile)
	      != (size_t)ap->value.Data.Int.i)
	    errorAbortLine (NULL, 0, "Internal outputAof: error when writing %s image", ap->str);
	  /* Word align the written area.  */
	  for (int pad = EXTRA (ap->value.Data.Int.i); pad; --pad)
	    fputc (0, objfile);
	  relocAOFOutput (objfile, ap);
	}
    }
}

#ifndef NO_ELF_SUPPORT
static void
writeElfSH (Elf32_Word nmoffset, unsigned int type, unsigned int flags,
	    unsigned int size, unsigned int link, unsigned int info,
	    unsigned int addralign, unsigned int entsize, size_t *offset)
{
  const Elf32_Shdr sect_hdr =
    {
      .sh_name = nmoffset,
      .sh_type = type,
      .sh_flags = flags,
      .sh_addr = 0, /* This is not part of the final image.  */
      .sh_offset = type == SHT_NULL ? 0 : *offset,
      .sh_size = size,
      .sh_link = link,
      .sh_info = info,
      .sh_addralign = addralign,
      .sh_entsize = entsize
    };
  if (type != SHT_NOBITS)
    *offset += FIX (size);
  if (fwrite (&sect_hdr, sizeof (sect_hdr), 1, objfile) != 1)
    errorAbortLine (NULL, 0, "Internal writeElfSH: error when writing chunk file header");
}

void
outputElf (void)
{
  /* We must call relocFix() before anything else.  */
  int noareas = 0, norels = 0;
  int areaSectionID = 3;
  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;

      ++noareas;

      ap->used = areaSectionID++;
      ap->area.info->norelocs = relocFix (ap);
      if (ap->area.info->norelocs)
	{
	  ++areaSectionID;
	  ++norels;
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
  elf_head.e_entry = areaEntrySymbol?areaEntryOffset:0;
  elf_head.e_phoff = 0;
  elf_head.e_shoff = sizeof (elf_head);
  /* We like to take all the aspects of EF_ARM_CURRENT but not its
     ARM EABI version as we aren't complying with any of the versions
     so we set the version to 0 which means EF_ARM_ABI_UNKNOWN.  */
  elf_head.e_flags = EF_ARM_CURRENT & ~EF_ARM_EABIMASK;
  if (option_apcs_softfloat)
    elf_head.e_flags |= 0x200;
  if (areaEntrySymbol)
    elf_head.e_flags |= EF_ARM_HASENTRY;
  elf_head.e_ehsize = sizeof (elf_head);
  elf_head.e_phentsize = 0;
  elf_head.e_phnum = 0;
  elf_head.e_shentsize = sizeof (Elf32_Shdr);
  elf_head.e_shnum = noareas + norels + 4; /* 4 = "" (SHT_NULL) + ".symtab" (SHT_SYMTAB) + ".strtab" (SHT_STRTAB) + ".shstrtab" (SHT_STRTAB) */
  elf_head.e_shstrndx = noareas + norels + 3;
  fwrite (&elf_head, 1, sizeof (elf_head), objfile);
  
  size_t offset = sizeof (elf_head) + elf_head.e_shnum * sizeof (Elf32_Shdr);
  Elf32_Word shstrsize = 0;
  
  /* Section headers - index 0 */
  writeElfSH (shstrsize, SHT_NULL, 0, 0, SHN_UNDEF, 0, 0, 0, &offset);
  shstrsize += sizeof ("");

  /* Symbol table - index 1 */
  size_t stringSizeNeeded;
  unsigned int nsyms = symbolFix (&stringSizeNeeded);
  writeElfSH (shstrsize, SHT_SYMTAB, 0, (nsyms + 1) * sizeof (Elf32_Sym),
	      2 /* The section header index of the associated string table.  */,
	      nsyms + 1 /* One greater than the symbol table index of the last local symbol (binding STB_LOCAL). */,
	      4 /* Align. */,
	      sizeof (Elf32_Sym) /* Entry size.  */,
	      &offset);
  shstrsize += sizeof (".symtab");

  size_t strsize = stringSizeNeeded + 1; /* Add extra NUL terminator at start. */

  /* String table - index 2 */
  writeElfSH (shstrsize, SHT_STRTAB, 0, strsize, 0, 0, 1, 0, &offset);
  shstrsize += sizeof (".strtab");

  /* Area headers - index 3 */
  int elfIndex = 3;
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      int areaFlags = 0;
      if (ap->area.info->type & AREA_CODE)
        areaFlags |= SHF_EXECINSTR;
      if (!(ap->area.info->type & AREA_READONLY))
        areaFlags |= SHF_WRITE;
      if (ap->area.info->type & AREA_COMMONDEF)
        areaFlags |= SHF_COMDEF;
      if (ap == areaEntrySymbol)
        areaFlags |= SHF_ENTRYSECT;
      areaFlags |= SHF_ALLOC;
      unsigned int sectionSize = FIX (ap->value.Data.Int.i);
      unsigned int sectionType = AREA_IMAGE (ap->area.info) ? SHT_PROGBITS : SHT_NOBITS;
      writeElfSH (shstrsize, sectionType, areaFlags, sectionSize,
                  0,
		  0,
		  1 << (ap->area.info->type & AREA_ALIGN_MASK),
		  0,
		  &offset);
      shstrsize += ap->len + 1;

      if (ap->area.info->norelocs)
        {
          /* relocations */
          writeElfSH (shstrsize, SHT_REL, 0,
	              ap->area.info->norelocs * sizeof(Elf32_Rel),
	              1, elfIndex, 4, sizeof(Elf32_Rel), &offset);
          shstrsize += sizeof (".rel.")-1 + (ap->str[0] == '.' ? -1 : 0) + ap->len + 1;
          elfIndex++;
        }
      elfIndex++;
    }

  /* Section head string table */
  shstrsize += sizeof (".shstrtab");
  writeElfSH (shstrsize - sizeof (".shstrtab"), SHT_STRTAB, 0, shstrsize, 0, 0, 1, 0, &offset);

  /* Symbol table (.symtab).  */
  symbolSymbolELFOutput (objfile);

  /* String table (.strtab).  */
  fputc (0, objfile);
  symbolStringOutput (objfile, stringSizeNeeded);
  for (int pad = EXTRA (strsize); pad; pad--)
    fputc (0, objfile);

  /* Areas */
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      if (AREA_IMAGE (ap->area.info))
        {
          if (fwrite (ap->area.info->image, 1, ap->value.Data.Int.i, objfile)
	      != (unsigned)ap->value.Data.Int.i)
            {
              errorAbortLine (NULL, 0, "Internal outputElf: error when writing %s image", ap->str);
              return;
            }
	  /* Word align the written area.  */
	  for (int pad = EXTRA (ap->value.Data.Int.i); pad; --pad)
	    fputc (0, objfile);
          if (ap->area.info->norelocs)
            relocELFOutput (objfile, ap);
        }
    }

  /* Section head string table.  */
  fputc (0, objfile);
  fwrite (".symtab", 1, sizeof(".symtab"), objfile);
  fwrite (".strtab", 1, sizeof(".strtab"), objfile);
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      /* Skip the implicit area.  */
      if (Area_IsImplicit (ap))
	continue;
      
      fwrite (ap->str, 1, ap->len + 1, objfile);
      if (ap->area.info->norelocs)
        {
          fwrite (".rel.", 1, sizeof(".rel.")-1 + (ap->str[0] == '.' ? -1 : 0), objfile);
          fwrite (ap->str, 1, ap->len + 1, objfile);
        }
    }
  fwrite (".shstrtab", 1, sizeof(".shstrtab"), objfile);
  for (int pad = EXTRA (shstrsize); pad; pad--)
    fputc (0, objfile);
}
#endif
