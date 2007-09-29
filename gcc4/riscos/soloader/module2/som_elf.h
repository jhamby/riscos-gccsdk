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

typedef struct elf_file
{
  FILE *handle;

  Elf32_Ehdr elf_header;

  Elf32_Phdr *prog_headers;

  Elf32_Phdr *dynamic_seg;

  char *interp_name;

  /* Total size of all loadable segments which are assumed to be
     consecutive in memory.  */
  int memory_size;

  /* This will be NULL for libraries, and 0x8000 for exectuables.  */
  som_PTR base_addr;
} elf_file;

extern void elffile_init (elf_file *file);

extern _kernel_oserror *elffile_open (const char *filename, elf_file *file);

extern void elffile_close (elf_file *file);

/* For an ELF object with a NULL p_vaddr for its first loadable segment,
   load_offset defines the load address. For an executable, load_offset
   should be NULL.  */
extern _kernel_oserror *elffile_load (elf_file *file, som_PTR load_offset,
				      bool init_bss);

extern _kernel_oserror *elffile_from_memory (elf_file *loader,
					     som_PTR base_addr);

static inline som_PTR
elffile_entry_point (elf_file *file)
{
  som_PTR base =
    (file->base_addr == (som_PTR) 0x8000) ? NULL : file->base_addr;

  return base + file->elf_header.e_entry;
}

#endif
