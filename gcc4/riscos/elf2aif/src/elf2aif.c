/* Static linked ELF to AIF convertor for ARM binaries
   Copyright (c) 2006-2010 GCCSDK Developers

   Written by John Tytgat.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __riscos__
#  include <kernel.h>
#  include <swis.h>
#else
#  include <sys/stat.h>
#endif

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "elf/common.h"
#include "elf/external.h"

#define COPYRIGHT "Copyright (c) 2006, 2007, 2009 GCCSDK Developers"
#define DISCLAIMER "This is free software; see the source for copying conditions.  There is NO\n" \
                   "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"

typedef struct phdr_list_s
{
  struct phdr_list_s *nextP;	/* Needs to be first */
  Elf32_External_Phdr phdr;
} phdr_list_t;

/* Entries marked with (*) need to be updated after copying data at
   'aifcode'.  */
typedef struct
{
  uint8_t decompress[4];	/* BL to decompression code for compressed images */
  uint8_t selfreloc[4];		/* BL to self-relocation code */
  uint8_t zeroinit[4];		/* BL to zero init code */
  uint8_t entrypoint[4];	/* BL to image entry point (*) */
  uint8_t exitinst[4];		/* Exit instruction */
  uint8_t rosize[4];		/* R/O area size (*) */
  uint8_t rwsize[4];		/* R/W area size (*) */
  uint8_t debugsize[4];		/* Debug area size */
  uint8_t zisize[4];		/* Zero init area size (*) */
  uint8_t debugtype[4];		/* Debug info type flag */
  uint8_t imagebase[4];		/* Address of image base (*) */
  uint8_t workspace[4];		/* Relocatable image workspace size */
  uint8_t amode[4];		/* 26/32 bit address mode and flags */
  uint8_t data[4];		/* Address of image data base */
  uint8_t reserv[4 * 2];	/* Reserved */
  uint8_t zicode[4 * 16];	/* zero init code */
} aifheader_t;

static int opt_verbose = 0;

static Elf32_External_Ehdr elf_ehdr;
static phdr_list_t *elf_phdrlistP;
static const char *elf_filename;

static const unsigned int aifcode[] = {
  0xE1A00000,			/* NOP (BL decompress)      */
  0xE1A00000,			/* NOP (BL self-relocate)   */
  0xEB00000C,			/* BL zero-init             */
  0xEB000000,			/* BL entrypoint (*)        */
  0xEF000011,			/* SWI OS_Exit              */
  0x00000000,			/* Image R/O size (*)       */
  0x00000000,			/* Image R/W size (*)       */
  0x00000000,			/* Image debug size         */
  0x00000000,			/* Image zero-init size (*) */
  0x00000000,			/* Image debug type         */
  0x00000000,			/* Image base (*)           */
  0x00000000,			/* Workspace size           */
  0x00000020,			/* 32-bit addressing        */
  0x00000000,			/* Data base                */
  0x00000000,			/* Reserved                 */
  0x00000000,			/* Reserved                 */
  0xE1A00000,			/* MOV   R0, R0             */
  0xE04EC00F,			/* SUB   R12, R14, PC       */
  0xE08FC00C,			/* ADD   R12, PC, R12       */
  0xE99C0017,			/* LDMIB R12, {R0-R2, R4}   */
  0xE24CC010,			/* SUB   R12, R12, #0x10    */
  0xE08CC000,			/* ADD   R12, R12, R0       */
  0xE08CC001,			/* ADD   R12, R12, R1       */
  0xE3A00000,			/* MOV   R0, #0             */
  0xE3A01000,			/* MOV   R1, #0             */
  0xE3A02000,			/* MOV   R2, #0             */
  0xE3A03000,			/* MOV   R3, #0             */
  0xE3540000,			/* CMP   R4, #0             */
/* zeroloop: */
  0xD1A0F00E,			/* MOVLE PC, R14            */
  0xE8AC000F,			/* STMIA R12!, {R0-R3}      */
  0xE2544010,			/* SUBS  R4, R4, #16        */
  0xEAFFFFFB			/* B     zeroloop           */
};

/* Read a little-endian 'short' value.  */
static uint16_t
RdShort (const uint8_t sh[2])
{
  return sh[0] | (sh[1] << 8);
}

/* Read a little-endian 'long' value.  */
static uint32_t
RdLong (const uint8_t lg[4])
{
  return lg[0] | (lg[1] << 8) | (lg[2] << 16) | (lg[3] << 24);
}

/* Write little-endian 'long' value.  */
static void
WrLong (uint8_t lg[4], uint32_t val)
{
  lg[0] = (val >> 0) & 0xFF;
  lg[1] = (val >> 8) & 0xFF;
  lg[2] = (val >> 16) & 0xFF;
  lg[3] = (val >> 24) & 0xFF;
}

static void
e2a_givehelp (void)
{
  fprintf (stderr, "Usage: elf2aif [options] <ELF file> [<AIF file>]\n"
	   "Convert static ARM ELF binary to AIF (Acorn Image Format) binary.\n"
	   "Options:\n"
	   "  -v, --verbose	prints informational messages during processing\n"
	   "      --help	display this help and exit\n"
	   "      --version	output version information and exit\n");
}

static void
e2a_giveversion (void)
{
  fprintf (stderr, PACKAGE_STRING "\n" COPYRIGHT "\n" DISCLAIMER "\n");
}

/* Fills in & validates 'elf_ehdr'.  */
static int
e2a_readehdr (FILE * elfhandle)
{
  if (fread (&elf_ehdr, sizeof (Elf32_External_Ehdr), 1, elfhandle) != 1)
    {
      fprintf (stderr, "Failed to read ELF header\n");
      return EXIT_FAILURE;
    }

  /* Header sanity checking.  */

  if (RdShort (elf_ehdr.e_ehsize) < sizeof (Elf32_External_Ehdr))
    {
      fprintf (stderr, "ELF header is too small\n");
      return EXIT_FAILURE;
    }

  if (elf_ehdr.e_ident[EI_MAG0] != ELFMAG0
      || elf_ehdr.e_ident[EI_MAG1] != ELFMAG1
      || elf_ehdr.e_ident[EI_MAG2] != ELFMAG2
      || elf_ehdr.e_ident[EI_MAG3] != ELFMAG3)
    {
      fprintf (stderr, "'%s' is not an ELF file\n", elf_filename);
      return EXIT_FAILURE;
    }

  if (elf_ehdr.e_ident[EI_CLASS] != ELFCLASS32)
    {
      fprintf (stderr, "ELF file '%s' is not a 32-bit object file\n",
	       elf_filename);
      return EXIT_FAILURE;
    }

  if (elf_ehdr.e_ident[EI_DATA] != ELFDATA2LSB)
    {
      fprintf (stderr, "ELF file '%s' is not little-endian\n", elf_filename);
      return EXIT_FAILURE;
    }

  if (elf_ehdr.e_ident[EI_VERSION] != 1)
    {
      fprintf (stderr, "ELF file '%s' has unsupported version %d\n",
	       elf_filename, elf_ehdr.e_ident[EI_VERSION]);
      return EXIT_FAILURE;
    }

  if (elf_ehdr.e_ident[EI_OSABI] != ELFOSABI_ARM)
    {
      fprintf (stderr, "ELF file '%s' is not for ARM\n", elf_filename);
      return EXIT_FAILURE;
    }

  /* No check on ABI version (EI_ABIVERSION) as this shouldn't hold us creating
     a valid AIF file.  */
  if (RdShort (elf_ehdr.e_type) != ET_EXEC)
    {
      fprintf (stderr, "ELF file '%s' is not a executable file\n",
	       elf_filename);
      return EXIT_FAILURE;
    }

  if (RdShort (elf_ehdr.e_machine) != EM_ARM)
    {
      fprintf (stderr,
	       "ELF file '%s' is not suited for ARM machine architecture\n",
	       elf_filename);
      return EXIT_FAILURE;
    }

  if (RdShort (elf_ehdr.e_version) != EV_CURRENT)
    {
      fprintf (stderr, "ELF file '%s' has unsupported version %d\n",
	       elf_filename, elf_ehdr.e_ident[EI_VERSION]);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

static int
e2a_loadphentry (FILE * elfhandle, const Elf32_External_Phdr * phentryP)
{
  phdr_list_t *phdr_prevP = (phdr_list_t *) & elf_phdrlistP;
  phdr_list_t *phdr_curP, *phdr_nextP;
  uint32_t paddr, pmemsize;

  /* Don't support virtual and physical addresses to be different.  */
  if (RdLong (phentryP->p_vaddr) != (paddr = RdLong (phentryP->p_paddr)))
    {
      fprintf (stderr,
	       "ELF file '%s' has program header entry with different physical and virtual addresses\n",
	       elf_filename);
      return EXIT_FAILURE;
    }
  if ((pmemsize = RdLong (phentryP->p_memsz)) < RdLong (phentryP->p_filesz))
    {
      fprintf (stderr,
	       "More data in file than need to be loaded in memory ?\n");
      return EXIT_FAILURE;
    }

  /* Search insert point.  */
  for (phdr_nextP = phdr_prevP->nextP; phdr_nextP != NULL;
       phdr_prevP = phdr_nextP, phdr_nextP = phdr_nextP->nextP)
    {
      if (paddr <= RdLong (phdr_nextP->phdr.p_paddr))
	break;
    }
  if (phdr_nextP != NULL)
    {
      /* Do some sanity check concerning overlap.  */
      /* FIXME: We don't check here for uint32_t overflows.  */
      if (paddr + pmemsize >= RdLong (phdr_nextP->phdr.p_paddr))
	{
	  fprintf (stderr, "Overlapping program header entries\n");
	  return EXIT_FAILURE;
	}
    }
  /* Insert.  */
  if ((phdr_curP = malloc (sizeof (phdr_list_t))) == NULL)
    {
      fprintf (stderr, "Out of memory\n");
      return EXIT_FAILURE;
    }
  phdr_prevP->nextP = phdr_curP;
  phdr_curP->nextP = phdr_nextP;
  memcpy (&phdr_curP->phdr, phentryP, sizeof (Elf32_External_Phdr));

  return EXIT_SUCCESS;
}

static int
e2a_readphdr (FILE * elfhandle)
{
  uint32_t phoffset, phentrysize, phentrycount;

  /* Check if there are program headers available.  */
  if ((phoffset = RdLong (elf_ehdr.e_phoff)) == 0
      || (phentrycount = RdShort (elf_ehdr.e_phnum)) == 0)
    {
      fprintf (stderr, "ELF file '%s' does not have program headers\n",
	       elf_filename);
      return EXIT_FAILURE;
    }

  if ((phentrysize =
       RdShort (elf_ehdr.e_phentsize)) < sizeof (Elf32_External_Phdr))
    {
      fprintf (stderr, "Size program header entry is too small\n");
      return EXIT_FAILURE;
    }

  /* Load the program header entries.  */
  while (phentrycount)
    {
      Elf32_External_Phdr phentry;
      uint32_t phtype;

      if (fseek (elfhandle, phoffset, SEEK_SET) != 0
	  || fread (&phentry, sizeof (Elf32_External_Phdr), 1,
		    elfhandle) != 1)
	{
	  fprintf (stderr, "Failed to read program header entry\n");
	  return EXIT_FAILURE;
	}
      phtype = RdLong (phentry.p_type);
      if (opt_verbose)
	printf
	  ("Processing program header entry at offset 0x%x with type %d\n",
	   phoffset, phtype);
      switch (phtype)
	{
	case PT_LOAD:
	  if (e2a_loadphentry (elfhandle, &phentry) != EXIT_SUCCESS)
	    return EXIT_FAILURE;
	  break;

	case PT_DYNAMIC:
	  fprintf (stderr,
		   "ELF file '%s' contains non-static program data which makes it unconvertable to AIF\n",
		   elf_filename);
	  return EXIT_FAILURE;
	}

      phoffset += phentrysize;
      --phentrycount;
    }

  return EXIT_SUCCESS;
}

static int
e2a_copy (FILE * elfhandle, FILE * aifhandle)
{
  const phdr_list_t *phdrP;
  uint32_t load_addr, prev_addr, exec_addr, flags;
  uint32_t robase, rwbase, zibase, zilimit;
  long aifend;
  aifheader_t aifhdr;
  uint32_t rosize, rwsize, zisize;

  /* Do some sanity checking.  */
  if (elf_phdrlistP == NULL)
    {
      fprintf (stderr, "Nothing to turn into an AIF file\n");
      return EXIT_FAILURE;
    }

  prev_addr = load_addr = RdLong (elf_phdrlistP->phdr.p_paddr);
  exec_addr = RdLong (elf_ehdr.e_entry);
  flags = RdLong (elf_phdrlistP->phdr.p_flags);
  if (prev_addr != load_addr
      && (exec_addr < prev_addr
	  || exec_addr >= prev_addr + RdLong (phdrP->phdr.p_filesz))
      && ((flags & (PF_R | PF_W | PF_X)) == (PF_R | PF_X)))
    {
      fprintf (stderr, "Unsupported case of entry address\n");
      return EXIT_FAILURE;
    }
  /* Check the dodgy assumptions we're making.  */
  if (RdLong (elf_phdrlistP->phdr.p_offset) != 0
      || exec_addr - load_addr < sizeof (aifcode))
    {
      fprintf (stderr, "First program segment is not what we hoped to be\n");
      return EXIT_FAILURE;
    }
  robase = prev_addr;
  rwbase = zibase = zilimit = 0;
  for (phdrP = elf_phdrlistP; phdrP != NULL; phdrP = phdrP->nextP)
    {
      uint32_t cur_addr, cur_flags;
      uint32_t foffset, fsize, msize;
      void *ptr;

      cur_addr = RdLong (phdrP->phdr.p_paddr);
      if (prev_addr != cur_addr)
	{
	  assert (prev_addr < cur_addr);

	  if (zibase == 0)
	    {
	      /* We're not yet at zi area, so we have to write it explicitly.  */
	      char zeros[32 * 1024];

	      if (opt_verbose)
		printf ("Adding %d bytes of zero.\n", cur_addr - prev_addr);
	      memset (zeros, 0, sizeof (zeros));
	      while (prev_addr < cur_addr)
		{
		  const uint32_t chunk =
		    (cur_addr - prev_addr >
		     sizeof (zeros)) ? sizeof (zeros) : cur_addr - prev_addr;
		  if (fwrite (zeros, chunk, 1, aifhandle) != 1)
		    {
		      fprintf (stderr, "Failed to write program segment\n");
		      return EXIT_FAILURE;
		    }
		  prev_addr += chunk;
		}
	    }
	}

      fsize = RdLong (phdrP->phdr.p_filesz);
      msize = RdLong (phdrP->phdr.p_memsz);

      cur_flags = RdLong (phdrP->phdr.p_flags);
      if (rwbase == 0 && (cur_flags & (PF_R | PF_W)) == (PF_R | PF_W))
	rwbase = cur_addr;
      if (rwbase != 0)
	{
	  if ((cur_flags & (PF_R | PF_W)) != (PF_R | PF_W))
	    {
	      fprintf (stderr,
		       "Wrong order of read-only / read-write segments : not supported\n");
	      return EXIT_FAILURE;
	    }
	  if (zibase != 0 && fsize != 0)
	    {
	      fprintf (stderr,
		       "Need to load non-zero data from file in zi area : not supported\n");
	      return EXIT_FAILURE;
	    }
	  if (fsize != msize)
	    {
	      if (zibase == 0)
		zibase = zilimit = cur_addr + fsize;
	      zilimit += msize - fsize;
	    }
	}

      ptr = malloc (fsize);
      if (ptr == NULL)
	{
	  fprintf (stderr, "Out of memory\n");
	  return EXIT_FAILURE;
	}
      foffset = RdLong (phdrP->phdr.p_offset);
      if (opt_verbose)
	printf
	  ("Reading ELF program segment from file offset 0x%x, size 0x%x\n",
	   foffset, fsize);
      if (fseek (elfhandle, foffset, SEEK_SET) != 0
	  || fread (ptr, fsize, 1, elfhandle) != 1)
	{
	  fprintf (stderr, "Failed to read program segment\n");
	  return EXIT_FAILURE;
	}
      if (fwrite (ptr, fsize, 1, aifhandle) != 1)
	{
	  fprintf (stderr, "Failed to write program segment\n");
	  return EXIT_FAILURE;
	}
      free (ptr);
      prev_addr = cur_addr + msize;
    }

  rosize = rwbase - robase;
  rwsize = zibase - rwbase;
  zisize = zilimit - zibase;
  if (opt_verbose)
    printf ("Load address 0x%x\n"
	    "Entry address 0x%x\n"
	    "RO size: %d bytes\n"
	    "RW size: %d bytes\n"
	    "ZI size: %d bytes\n", load_addr, exec_addr, rosize, rwsize,
	    zisize);

  assert (sizeof (aifheader_t) == sizeof (aifcode));
  /* Write AIF header */
  aifend = ftell (aifhandle);
  if (fseek (aifhandle, 0, SEEK_SET) != 0)
    {
      fprintf (stderr, "fseek() failed\n");
      return EXIT_FAILURE;
    }

  memcpy (&aifhdr, aifcode, sizeof (aifcode));
  WrLong (aifhdr.entrypoint,
	  RdLong (aifhdr.entrypoint) +
	  ((exec_addr - load_addr -
	    (offsetof (aifheader_t, entrypoint) + 8)) >> 2));
  WrLong (aifhdr.rosize, rosize);
  WrLong (aifhdr.rwsize, rwsize);
  WrLong (aifhdr.zisize, zisize);
  WrLong (aifhdr.imagebase, load_addr);
  if (fwrite (&aifhdr, sizeof (aifhdr), 1, aifhandle) != 1
      || fseek (aifhandle, aifend, SEEK_SET) != 0)
    {
      fprintf (stderr, "Failed to write aif header\n");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

/* When aiffilename is NULL, write the AIF file with filename elffilename.  */
static int
e2a_convert (const char *elffilename, const char *aiffilename)
{
  FILE *elfhandle, *aifhandle;
  int status;
  const char *temp_aiffilename;
  bool aiffile_exists = false;

  aifhandle = elfhandle = NULL;
  status = EXIT_FAILURE;

  if (aiffilename != NULL)
    {
      temp_aiffilename = NULL;
      if (opt_verbose)
	printf ("Converting '%s' to '%s'.\n", elffilename, aiffilename);
    }
  else
    {
      char *temp_filename;
      if ((temp_filename =
	   malloc (strlen (elffilename) + sizeof ("_tmp") - 1 + 1)) == NULL)
	{
	  fprintf (stderr, "Out of memory\n");
	  return EXIT_FAILURE;
	}
      strcpy (temp_filename, elffilename);
      strcat (temp_filename, "_tmp");
      aiffilename = temp_filename;
      temp_aiffilename = temp_filename;
      if (opt_verbose)
	printf ("Converting '%s'.\n", elffilename);
    }

  if ((elfhandle = fopen (elffilename, "r")) == NULL)
    {
      fprintf (stderr, "Failed to open file '%s' for reading\n", elffilename);
      goto convert_end;
    }

  if ((aifhandle = fopen (aiffilename, "w")) == NULL)
    {
      fprintf (stderr, "Failed to open file '%s' for writing\n", aiffilename);
      goto convert_end;
    }
  aiffile_exists = true;

  elf_filename = elffilename;

  if (e2a_readehdr (elfhandle) == EXIT_SUCCESS
      && e2a_readphdr (elfhandle) == EXIT_SUCCESS
      && e2a_copy (elfhandle, aifhandle) == EXIT_SUCCESS)
    status = EXIT_SUCCESS;

#ifdef __riscos__
  if (status == EXIT_SUCCESS)
    {
      _kernel_swi_regs regs;
      _kernel_oserror *swierror;

      /* Set filetype to 0xFF8. */
      regs.r[0] = 0x12;		/* Set file type.  */
      regs.r[1] = (int) aiffilename;
      regs.r[2] = 0xFF8;	/* Absolute.  */
      if ((swierror = _kernel_swi (OS_File, &regs, &regs)) != NULL)
	{
	  fprintf (stderr, "Cannot set filetype of '%s' (%s)", aiffilename,
		   swierror->errmess);
	  status = EXIT_FAILURE;
	}
    }
#endif

convert_end:
  if (elfhandle != NULL)
    {
      if (fclose (elfhandle))
	{
	  fprintf (stderr, "Failed to close file '%s'\n", elffilename);
	  status = EXIT_FAILURE;
	}
      elfhandle = NULL;
    }
  if (aifhandle != NULL)
    {
      if (fclose (aifhandle))
	{
	  fprintf (stderr, "Failed to close file '%s'\n", aiffilename);
	  status = EXIT_FAILURE;
	}
      aifhandle = NULL;
    }
#ifndef __riscos__
  if (status == EXIT_SUCCESS)
    {
      /* Make AIF output file executable so we can run it with e.g. QEMU.  */
      struct stat buf;
      if (stat (aiffilename, &buf) < 0)
	{
	  fprintf (stderr, "Failed to get file status '%s'\n", aiffilename);
	  status = EXIT_FAILURE;
	}
      else
	{
	  buf.st_mode &= S_IRWXU | S_IRWXG | S_IRWXO;
	  if (buf.st_mode & S_IRUSR)
	    buf.st_mode |= S_IXUSR;
	  if (buf.st_mode & S_IRGRP)
	    buf.st_mode |= S_IXGRP;
	  if (buf.st_mode & S_IROTH)
	    buf.st_mode |= S_IXOTH;
	  if (chmod (aiffilename, buf.st_mode) < 0)
	    {
	      fprintf (stderr, "Failed to make '%s' executable\n", aiffilename);
	      status = EXIT_FAILURE;
	    }
	}
    }
#endif
  /* When there was an error, remove the output file if there was one
     written.  */
  if (status != EXIT_SUCCESS)
    {
      if (aiffile_exists)
	{
	  if (remove (aiffilename))
	    {
	      fprintf (stderr, "Failed to delete file '%s'\n", aiffilename);
	      status = EXIT_FAILURE;
	    }
	}
    }
  else if (temp_aiffilename != NULL)
    {
      /* rename () from libscl with existing destination (which is the case
         here), fails.  Therefore delete the destination file first.  */
#ifdef __TARGET_SCL__
      if (remove (elffilename) || rename (temp_aiffilename, elffilename))
#else
      if (rename (temp_aiffilename, elffilename))
#endif
	{
	  fprintf (stderr, "Failed to rename file '%s' to '%s'\n",
		   temp_aiffilename, elffilename);
	  status = EXIT_FAILURE;
	  if (remove (temp_aiffilename))
	    fprintf (stderr, "Failed to delete file '%s'\n",
		     temp_aiffilename);
	}
    }

  free ((void *) temp_aiffilename);

  if (opt_verbose)
    printf ((status == EXIT_SUCCESS) ? "Done.\n" : "Failed.\n");

  return status;
}

int
main (int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; ++i)
    {
      if (argv[i][0] != '-')
	break;			/* no options anymore */

      if (!strcmp (&argv[i][1], "-help"))
	{
	  e2a_givehelp ();
	  if (i + 1 != argc)
	    fprintf (stderr, "Warning: extra options/arguments ignored\n");
	  return EXIT_SUCCESS;
	}
      else if (!strcmp (&argv[i][1], "-verbose")
	       || !strcmp (&argv[i][1], "v"))
	++opt_verbose;
      else if (!strcmp (&argv[i][1], "-version"))
	{
	  e2a_giveversion ();
	  if (i + 1 != argc)
	    fprintf (stderr, "Warning: extra options/arguments ignored\n");
	  return EXIT_SUCCESS;
	}
      else
	{
	  fprintf (stderr, "Error: unknown option '%s'\n", argv[i]);
	  return EXIT_FAILURE;
	}
    }

  if (i == argc)
    {
      fprintf (stderr, "Error: nothing to do\n");
      return EXIT_FAILURE;
    }
  else if (i != argc - 1 && i != argc - 2)
    {
      fprintf (stderr, "Error: wrong number of filenames specified\n");
      return EXIT_FAILURE;
    }

  return e2a_convert (argv[i], argv[i + 1]);
}
