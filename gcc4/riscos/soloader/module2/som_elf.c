/* som_elf.c
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#include <kernel.h>
#include <stdio.h>
#include <string.h>
#include "som.h"
#include "somanager.h"
#include "som_alloc.h"
#include "som_elf.h"
#include "som_symlinks.h"

void
elffile_init (elf_file *file)
{
  memset (file, 0, sizeof (elf_file));
}

#define ELF_ID (unsigned int)(('F' << 24) | ('L' << 16) | ('E' << 8) | 0x7F)

_kernel_oserror *
elffile_open (const char *filename, elf_file *file)
{
  _kernel_oserror *err;

#ifdef USE_SYMLINKS
  char *resolved_filename = NULL;

  if ((err = som_resolve_links (filename, &resolved_filename)) != NULL)
    return err;
  filename = resolved_filename;
#endif

  if ((file->handle = fopen (filename, "r")) == NULL)
    goto error;

#ifdef USE_SYMLINKS
  som_free (resolved_filename);
  resolved_filename = NULL;
#endif

  /* Read the main ELF header at the beginning of the file.  */
  if (fread (&file->elf_header, sizeof (Elf32_Ehdr), 1, file->handle) != 1)
    goto error;

  /* Make sure the file is suitable for RISC OS.  */
  if (*((unsigned int *) file->elf_header.e_ident) != ELF_ID
      || file->elf_header.e_ident[EI_CLASS] != ELFCLASS32
      || file->elf_header.e_ident[EI_DATA] != ELFDATA2LSB
      || file->elf_header.e_ident[EI_VERSION] != EV_CURRENT
      || file->elf_header.e_machine != EM_ARM)
    {
      err = somerr_invalid_elf;
      goto error;
    }

  /* Allocate space for program headers and read them from file.  */
  if ((err = som_alloc (file->elf_header.e_phentsize * file->elf_header.e_phnum,
			(void **) (void *) &file->prog_headers)) != NULL)
    goto error;

  fseek (file->handle, file->elf_header.e_phoff, SEEK_SET);
  if (fread (file->prog_headers, file->elf_header.e_phentsize,
	     file->elf_header.e_phnum, file->handle) != file->elf_header.e_phnum)
    goto error;

  Elf32_Phdr *phdr = file->prog_headers;
  int phnum = file->elf_header.e_phnum;
  unsigned int memory_size = 0;

  while (phnum--)
    {
      switch (phdr->p_type)
	{
	case PT_INTERP:
	  /* Return the address of the name of the interpreter (dynamic
	     loader) if any.  */
	  file->interp_name = (char *) phdr->p_vaddr;
	  break;

	case PT_DYNAMIC:
	  /* Return the address of the dynamic segment program header if
	     any.  */
	  file->dynamic_seg = phdr;
	  break;

	case PT_LOAD:
	  if (phdr->p_offset + phdr->p_memsz > memory_size)
	    memory_size = phdr->p_offset + phdr->p_memsz;

	  if (file->base_addr == NULL
	      || (som_PTR) phdr->p_vaddr < file->base_addr)
	    file->base_addr = (som_PTR) phdr->p_vaddr;
	  break;
	}

      phdr++;
    }

  file->memory_size = memory_size;

  return NULL;

error:
  if (err == NULL
      && (err = _kernel_last_oserror ()) == NULL)
    err = somerr_file_error;	/* Default to file error.  */

#ifdef USE_SYMLINKS
  if (resolved_filename)
    som_free (resolved_filename);
#endif

  elffile_close (file);

  return err;
}

void
elffile_close (elf_file *file)
{
  if (file->handle)
    {
      fclose (file->handle);
      file->handle = NULL;
    }

  if (file->prog_headers)
    {
      som_free (file->prog_headers);
      file->prog_headers = NULL;
    }
}

/* For an ELF object with a NULL p_vaddr for its first loadable segment,
   load_offset defines the load address. For an executable, load_offset
   should be NULL.  */
_kernel_oserror *
elffile_load (elf_file *file, som_PTR load_offset, bool init_bss)
{
  Elf32_Phdr *phdr = file->prog_headers;
  int phnum = file->elf_header.e_phnum;
  _kernel_oserror *err = NULL;

  while (phnum--)
    {
      if (phdr->p_type == PT_LOAD)
	{
	  fseek (file->handle, phdr->p_offset, SEEK_SET);

	  /* It's possible to have a data segment that has zero size in
	     file, but none zero size in memory, ie, the entire r/w segment
	     comprises of .bss section.  */
	  if (phdr->p_filesz != 0)
	    if (fread
		((void *) (load_offset + phdr->p_vaddr), phdr->p_filesz, 1,
		 file->handle) != 1)
	      goto error;

	  if (init_bss)
	    {
	      int bss_size = phdr->p_memsz - phdr->p_filesz;
	      /* If there's a difference between memory size and file size,
	         then this indicates the bss area which needs zeroing.  */
	      if (bss_size != 0)
		memset ((void *) (load_offset + phdr->p_vaddr +
				  phdr->p_filesz), 0, bss_size);
	    }
	}

      phdr++;
    }

  return NULL;

error:
  if (!err)
    if ((err = _kernel_last_oserror ()) == NULL)
      err = somerr_file_error;

  return err;
}

/* Fill in an elf_file structure from an image already in memory.  */
_kernel_oserror *
elffile_from_memory (elf_file *loader, som_PTR base_addr)
{
  _kernel_oserror *err;

  loader->base_addr = base_addr;
  memcpy (&loader->elf_header, base_addr, sizeof (Elf32_Ehdr));

  /* Allocate space for program headers and copy them from the image.  */
  if ((err = som_alloc (loader->elf_header.e_phentsize * loader->elf_header.e_phnum,
			(void **) (void *) &loader->prog_headers)) != NULL)
    return err;

  memcpy (loader->prog_headers, base_addr + loader->elf_header.e_phoff,
	  loader->elf_header.e_phentsize * loader->elf_header.e_phnum);

  Elf32_Phdr *phdr = loader->prog_headers;
  int phnum = loader->elf_header.e_phnum;

  while (phnum--)
    {
      switch (phdr->p_type)
	{
	case PT_DYNAMIC:
	  /* Return the address of the dynamic segment program header if
	     any.  */
	  loader->dynamic_seg = phdr;
	  break;

	case PT_LOAD:
	  loader->memory_size += phdr->p_memsz;
	  break;
	}

      phdr++;
    }

  return NULL;
}
