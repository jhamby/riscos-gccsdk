/* som_elf.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_ELF_H
#define SOM_ELF_H

#include <kernel.h>
#include <stdio.h>
#include <sys/elf.h>
#include "som_types.h"

#define ABI_MAX_LEN 16

#define ELF_ID (unsigned int)(('F' << 24) | ('L' << 16) | ('E' << 8) | 0x7F)

typedef struct elf_file
{
  /* validation word - ELF_ID */
  unsigned int magic;

  char *name;

  FILE *handle;

  Elf32_Ehdr elf_header;

  Elf32_Phdr *prog_headers;

  Elf32_Phdr *text_segment_phdr;
  Elf32_Phdr *data_segment_phdr;
  Elf32_Phdr *dynamic_segment_phdr;

  char *interp_name;

  char abi_version[ABI_MAX_LEN];

  /* Total size of all loadable segments which are assumed to be
     consecutive in memory.  */
  int memory_size;

  som_PTR text_segment;

  /* Size in bytes of the read only text segment.  */
  int text_size;

  /* This points to the unmapped data segment that will be used to generate aborts
     allowing the abort handler to map in the relevant segment for the client.  */
  som_PTR data_segment;

  /* This points to the actual data segment loaded from the library file and is
     used to initialise the private data segment of new clients.  */
  som_PTR data_init_segment;

  /* Size in bytes of the read/write data segment.  */
  int data_size;

  /* Byte offset from start of data segment.  */
  unsigned bss_offset;
  unsigned bss_size;

  /* Byte offset from start of data segment.  */
  unsigned dynamic_offset;
  unsigned dynamic_size;

  /* Byte offset from start of data segment.  */
  unsigned got_offset;

  bool is_armeabihf;

  /* True if the data segment in the file is aligned to 8 bytes, false if aligned
     to 4. The dynamic linker needs to use the same alignment so that double
     word load instructions work correctly on double word global variables.  */
  bool data_seg_8byte_aligned;

} elf_file;

extern void elffile_init (elf_file *file);

extern _kernel_oserror *elffile_open (const char *filename, elf_file *file);

extern void elffile_close (elf_file *file);

extern _kernel_oserror *
elffile_alloc_text_pages(elf_file *file);

extern _kernel_oserror *
elffile_alloc_data_pages(elf_file *file);

extern _kernel_oserror *
elffile_alloc_segments(elf_file *file);

extern _kernel_oserror *
elffile_load (elf_file *file,
	      bool init_bss);

extern _kernel_oserror *
elffile_from_memory (elf_file *loader,
		     som_PTR text_segment,
		     som_PTR elfimage_data_segment,
		     som_PTR data_init_segment);

extern _kernel_oserror *
elffile_get_public_info (elf_file *file,
			 som_objinfo *info);

static inline som_PTR
elffile_entry_point (elf_file *file)
{
  som_PTR base =
    (file->text_segment == (som_PTR) 0x8000) ? NULL : file->text_segment;

  return base + file->elf_header.e_entry;
}

static inline _kernel_oserror *
elffile_get_abi (elf_file *file, const char **abi_ret)
{
  if (!file || file->magic != ELF_ID || abi_ret == NULL)
    return somerr_bad_param;

  *abi_ret = file->abi_version;

  return NULL;
}

static inline _kernel_oserror *
elffile_is_armeabihf (elf_file *file, bool *is_armeabihf_ret)
{
  if (!file || file->magic != ELF_ID || is_armeabihf_ret == NULL)
    return somerr_bad_param;

  *is_armeabihf_ret = file->is_armeabihf;

  return NULL;
}

#endif
