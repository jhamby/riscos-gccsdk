/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas R�emo
 * Copyright (c) 1997 Nick Burrett
 * Copyright (c) 2000-2006 GCCSDK Developers
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
#include "version.h"

#ifdef __TARGET_SCL__
#  include <swis.h>
#else
#  include <ctype.h>
#  include <errno.h>
#endif

#ifndef CROSS_COMPILE
#  include "depend.h"
#endif

static FILE *objfile;

#define FIX(n) ((3+(int)(n))&~3)
#define EXTRA(n) (FIX(n)-(n))

const char *idfn_text = "GCCSDK AS AOF"
#ifndef NO_ELF_SUPPORT
	"/ELF"
#endif
	" Assembler " AS_VERSION " (" __DATE__ ") [GCCSDK " __VERSION__ "]\n";

#define MAXNAME 1024
static char outname[MAXNAME + 1];

#if defined(WORDS_BIGENDIAN)
/* Convert to ARM byte-sex.  */
WORD armword (WORD val)
{
  return (val >> 24) |
         ((val >> 8) & 0xff00)   |
         ((val << 8) & 0xff0000) |
          (val << 24);
}

/* Convert from ARM byte-sex.  */
WORD ourword (WORD val)
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
      char *temp;
#ifdef CROSS_COMPILE
      strncpy (outname, outfile, MAXNAME);
      temp = strstr (outname, ".s");
      if (temp && temp[2] == '\0')
	temp[1] = 'o';
#else
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
    {
      fclose (objfile);
      objfile = NULL;
#ifdef __TARGET_SCL__
      /* Set filetype to text */
      {
        _kernel_swi_regs regs;

        regs.r[0] = 18;
        regs.r[1] = (int)__riscosify_scl(outname, 0);
        regs.r[2] = 0xfff;

        _kernel_swi(OS_File, &regs, &regs);
      }
#endif
    }
}

void
outputRemove (void)
{
  outputFinish ();
  if (outname[0])
    remove (outname);
}

static int
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

static int
writeEntry (int ID, int type, int size, int *offset)
{
  ChunkFileHeaderEntry chunk_entry;

  chunk_entry.ChunkIDPrefix = armword (ID);
  chunk_entry.ChunkIDType = armword (type);
  chunk_entry.FileOffset = armword (*offset);
  chunk_entry.Size = armword (size);
  *offset += size;
  return fwrite (&chunk_entry, 1, sizeof (chunk_entry), objfile);
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
  /* avoid problems with no areas.  */
  for (ap = areaHead; ap != NULL; ap = ap->area.info->next)
    {
      ap->area.info->norelocs = relocFix (ap);
      if (AREA_IMAGE (ap->area.info))
	obj_area_size += FIX (ap->value.ValueInt.i)
	  + ap->area.info->norelocs * sizeof (AofReloc);
    }

  aof_head.Type = armword (AofHeaderID);
  aof_head.Version = armword (310);
  aof_head.noAreas = armword (noareas);
  aof_head.noSymbols = armword (symbolFix ());
  aof_head.EntryArea = armword (areaEntry ? areaEntry->used + 1 : 0);
  aof_head.EntryOffset = armword (areaEntry ? areaEntryOffset : 0);

  /* Write out the chunk file header.  */
  chunk_header.ChunkField = armword (ChunkFileID);
  chunk_header.maxChunks = armword (5);
  chunk_header.noChunks = armword (5);
  written = fwrite (&chunk_header, 1, sizeof (chunk_header), objfile);
  offset = sizeof (chunk_header) + 5 * sizeof (chunk_entry);

  written += writeEntry(ChunkID_OBJ, ChunkID_OBJ_HEAD,
               sizeof (aof_head) + noareas * sizeof (aof_entry), &offset);

  written += writeEntry(ChunkID_OBJ, ChunkID_OBJ_IDFN,
                        idfn_size = FIX (strlen (idfn_text) + 1), &offset);

  written += writeEntry(ChunkID_OBJ, ChunkID_OBJ_STRT,
                        strt_size = FIX (symbolStringSize ()), &offset);

  written += writeEntry(ChunkID_OBJ, ChunkID_OBJ_SYMT,
                        ourword (aof_head.noSymbols) * sizeof (AofSymbol), &offset);

  written += writeEntry(ChunkID_OBJ, ChunkID_OBJ_AREA,
                        obj_area_size, &offset);

  if (written != (sizeof (chunk_header) + 5 * sizeof (chunk_entry)))
    {
      errorLine (0, NULL, ErrorSerious, FALSE,
		 "Internal outputAof: error when writing chunk file header");
      return;
    }

/******** Chunk 0 Header ********/
  fwrite (&aof_head, 1, sizeof (aof_head), objfile);

  for (ap = areaHead; ap != NULL; ap = ap->area.info->next)
    {
      aof_entry.Name = armword (ap->offset);
      aof_entry.Type = armword (ap->area.info->type);
      aof_entry.Size = armword (FIX (ap->value.ValueInt.i));
      aof_entry.noRelocations = armword(ap->area.info->norelocs);
      if (aof_entry.noRelocations != 0 && !AREA_IMAGE (ap->area.info))
	errorLine (0, NULL, ErrorSerious, FALSE,
		   "Internal outputAof: relocations in uninitialised area");
      aof_entry.Unused = 0;
      fwrite (&aof_entry, 1, sizeof (aof_entry), objfile);
    }

/******** Chunk 1 Identification *********/
  if (idfn_size != fwrite (idfn_text, 1, idfn_size, objfile))
    {
      errorLine (0, NULL, ErrorSerious, FALSE,
		 "Internal outputAof: error when writing identification");
      return;
    }
/******** Chunk 2 String Table ***********/
  strt_size = armword(symbolStringSize ());
  if (fwrite (&strt_size, 1, 4, objfile) != sizeof (strt_size))
    {
      errorLine (0, NULL, ErrorSerious, FALSE,
		 "Internal outputAof: error when writing string table size");
      return;
    }
  symbolStringOutput (objfile);
  for (pad = EXTRA (symbolStringSize ()); pad; pad--)
    fputc (0, objfile);

/******** Chunk 3 Symbol Table ***********/
  symbolSymbolOutput (objfile);

/******** Chunk 4 Area *****************/
  for (ap = areaHead; ap != NULL; ap = ap->area.info->next)
    {
      if (AREA_IMAGE (ap->area.info))
	{
	  if ((size_t)ap->value.ValueInt.i !=
	      fwrite (ap->area.info->image, 1, ap->value.ValueInt.i, objfile))
	    {
	      errorLine (0, NULL, ErrorSerious, FALSE,
		"Internal outputAof: error when writing %s image", ap->str);
	      return;
	    }
	  /* Word align the written area.  */
	  for (pad = EXTRA (ap->value.ValueInt.i); pad; --pad)
	    fputc (0, objfile);
	  relocOutput (objfile, ap);
	}
    }
}

#ifndef NO_ELF_SUPPORT
static int
countRels (Symbol * ap)
{
  int i = 0;
  while (ap)
    {
      if (ap->area.info->norelocs) i++;
      ap = ap->area.info->next;
    }
  return i;
}

static int
writeElfSH (int nmoffset, int type, int flags, int size,
            int link, int info, int addralign, int entsize, int *offset)
{
  Elf32_Shdr sect_hdr;

  sect_hdr.sh_name = nmoffset;
  sect_hdr.sh_type = type;
  sect_hdr.sh_flags = flags;
  sect_hdr.sh_addr = 0;
  sect_hdr.sh_offset = type==SHT_NULL?0:*offset;
  sect_hdr.sh_size = size;
  sect_hdr.sh_link = link;
  sect_hdr.sh_info = info;
  sect_hdr.sh_addralign = addralign;
  sect_hdr.sh_entsize = entsize;
  if (type!=SHT_NOBITS)
    *offset += size;
  return fwrite (&sect_hdr, 1, sizeof (sect_hdr), objfile);
}

void
outputElf (void)
{
  int noareas = countAreas (areaHead);
  int norels;
  int written, offset, obj_area_size, pad, strsize;
  int elfIndex, nsyms, shstrsize=0;
  int i, sectionSize, sectionType;
  Elf32_Ehdr elf_head;
  Symbol *ap;

  /* We must call relocFix() before anything else.  */
  obj_area_size = 0;
  for (ap = areaHead; ap != NULL; ap = ap->area.info->next)
    ap->area.info->norelocs = relocFix (ap);
  norels = countRels(areaHead);

  elf_head.e_ident[EI_MAG0] = ELFMAG0;
  elf_head.e_ident[EI_MAG1] = ELFMAG1;
  elf_head.e_ident[EI_MAG2] = ELFMAG2;
  elf_head.e_ident[EI_MAG3] = ELFMAG3;
  elf_head.e_ident[EI_CLASS] = ELFCLASS32;
  elf_head.e_ident[EI_DATA] = ELFDATA2LSB;
  elf_head.e_ident[EI_VERSION] = EV_CURRENT;
  elf_head.e_ident[EI_OSABI] = ELFOSABI_ARM;
  elf_head.e_ident[EI_ABIVERSION] = 0;
  for (i = EI_PAD; i< EI_NIDENT; i++)
    elf_head.e_ident[i]=0;
  elf_head.e_type = ET_REL;
  elf_head.e_machine = EM_ARM;
  elf_head.e_version = EV_CURRENT;
  elf_head.e_entry = areaEntry?areaEntryOffset:0;
  elf_head.e_phoff = 0;
  elf_head.e_shoff = sizeof(elf_head);
  /* We like to take all the aspects of EF_ARM_CURRENT but not its ARM EABI version
     as we aren't complying with any of the versions so we set the version to 0 which
     means EF_ARM_ABI_UNKNOWN.  */
  elf_head.e_flags = EF_ARM_CURRENT & ~EF_ARM_EABIMASK;
  if (areaEntry)
    elf_head.e_flags |= EF_ARM_HASENTRY;
  elf_head.e_ehsize = sizeof(elf_head);
  elf_head.e_phentsize = 0;
  elf_head.e_phnum = 0;
  elf_head.e_shentsize = sizeof(Elf32_Shdr);
  elf_head.e_shnum = (noareas+norels)+4;
  elf_head.e_shstrndx = (noareas+norels)+3;

  written = fwrite (&elf_head, 1, sizeof (elf_head), objfile);

  offset = sizeof (elf_head) + elf_head.e_shnum * sizeof (Elf32_Shdr);

  /* Section headers - index 0 */
  written += writeElfSH(shstrsize, SHT_NULL, 0, 0, SHN_UNDEF, 0, 0, 0, &offset);  shstrsize += 1; /* Null */

  /* Symbol table - index 1 */
  nsyms = symbolFix();
  written += writeElfSH(shstrsize, SHT_SYMTAB, 0,
                        (nsyms + 1) * sizeof (Elf32_Sym),
                        2, 0, 4, sizeof (Elf32_Sym), &offset);
  shstrsize += 8; /* .symtab */

  strsize = symbolStringSize() - 3; /* not outputting size, but null */

  /* String table - index 2 */
  written += writeElfSH(shstrsize, SHT_STRTAB, 0, FIX (strsize),
                        0, 0, 1, 0, &offset);
  shstrsize += 8; /* .strtab */

  /* Area headers - index 3 */
  
  elfIndex = 3;
  for (ap = areaHead; ap != NULL; ap = ap->area.info->next)
    {
      int areaFlags = 0;
      if ((ap->area.info->type & AREA_CODE) > 0)
        areaFlags |= SHF_EXECINSTR;
      if ((ap->area.info->type & AREA_READONLY) == 0)
        areaFlags |= SHF_WRITE;
      if ((ap->area.info->type & AREA_COMMONDEF) > 0)
        areaFlags |= SHF_COMDEF;
      if (ap == areaEntry)
        areaFlags |= SHF_ENTRYSECT;
      areaFlags |= SHF_ALLOC;
      sectionSize = FIX (ap->value.ValueInt.i);
      sectionType = AREA_IMAGE (ap->area.info) ? SHT_PROGBITS : SHT_NOBITS;
      writeElfSH(shstrsize, sectionType, areaFlags, sectionSize,
                 0, 0, 4, 0, &offset);
      shstrsize += ap->len + 1;
      if (ap->area.info->norelocs)
        {
          /* relocations */
          writeElfSH(shstrsize, SHT_REL, 0,
            (ap->area.info->norelocs)*sizeof(Elf32_Rel),
            1, elfIndex, 4, sizeof(Elf32_Rel), &offset);
          shstrsize += ap->len + 5;
          elfIndex++;
        }
      elfIndex++;
    }

  /* Section head string table */
  shstrsize += 10; /* .shstrtab */
  written += writeElfSH(shstrsize-10, SHT_STRTAB, 0, shstrsize, 0,
                        0, 1, 0, &offset);

  /* Write out the sections */
  /* Symbol table */
  symbolSymbolElfOutput(objfile);

  /* String table */
  fputc (0, objfile);
  symbolStringOutput (objfile);
  for (pad = EXTRA (strsize); pad; pad--)
    fputc (0, objfile);

  /* Areas */
  for (ap = areaHead; ap != NULL; ap = ap->area.info->next)
    {
      if (AREA_IMAGE (ap->area.info))
        {
          if ((size_t)ap->value.ValueInt.i !=
              fwrite (ap->area.info->image, 1, ap->value.ValueInt.i, objfile))
            {
              errorLine (0, NULL, ErrorSerious, FALSE,
                "Internal outputElf: error when writing %s image", ap->str);
              return;
            }
	  /* Word align the written area.  */
	  for (pad = EXTRA (ap->value.ValueInt.i); pad; --pad)
	    fputc (0, objfile);
          if (ap->area.info->norelocs)
            relocElfOutput (objfile, ap);
        }
    }

  /* Section head string table */
  fputc (0, objfile);
  fwrite (".symtab", 1, sizeof(".symtab"), objfile);
  fwrite (".strtab", 1, sizeof(".strtab"), objfile);
  for (ap = areaHead; ap != NULL; ap = ap->area.info->next)
    {
      fwrite (ap->str, 1, ap->len + 1, objfile);
      if (ap->area.info->norelocs)
        {
          fwrite (".rel", 1, sizeof(".rel")-1, objfile);
          fwrite (ap->str, 1, ap->len + 1, objfile);
        }
    }

  fwrite (".shstrtab", sizeof(char), sizeof(".shstrtab"), objfile);
  for (pad = EXTRA (shstrsize); pad; pad--)
    fputc (0, objfile);
}
#endif
