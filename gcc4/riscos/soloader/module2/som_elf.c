/* som_elf.c
 *
 * Copyright 2007-2019 GCCSDK Developers
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

#define DEBUG_PRINT 0
#define DEBUG_REPORT 0
#define DEBUG_ELFFILE (DEBUG_PRINT || DEBUG_REPORT)

#if DEBUG_ELFFILE
#if DEBUG_PRINT
#define DEBUG_PRINT_IT puts(buffer)
#else
#define DEBUG_PRINT_IT
#endif
#if DEBUG_REPORT
#define DEBUG_REPORT_IT report_text(buffer)
#else
#define DEBUG_REPORT_IT
#endif
#define DEBUG_OUTPUT(format, args...) \
{\
char buffer[256];\
sprintf(buffer,format,args);\
DEBUG_REPORT_IT;\
DEBUG_PRINT_IT;\
}
#else
#define DEBUG_OUTPUT(format, args...)
#endif

static const char armeabihf[] = "armeabihf";

void
elffile_init (elf_file *file)
{
  if (file)
    {
      memset (file, 0, sizeof (elf_file));

      file->magic = ELF_ID;
    }
}

_kernel_oserror *
elffile_open (const char *filename, elf_file *file)
{
  _kernel_oserror *err;

  if (!file || file->magic != ELF_ID)
    return somerr_bad_param;

  if ((err = som_resolve_links (filename, &file->name)) != NULL)
    return err;

  DEBUG_OUTPUT("%s: filename: %s",__func__,filename);

  if ((file->handle = fopen (file->name, "r")) == NULL)
    goto error;

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

  file->data_segment_phdr = NULL;

  while (phnum--)
    {
      switch (phdr->p_type)
	{
	case PT_INTERP:
	  /* Return the address of the name of the interpreter (dynamic
	     linker) if any.  */
	  file->interp_name = (char *) phdr->p_vaddr;
	  break;

	case PT_DYNAMIC:
	  /* Return the address of the dynamic segment program header if
	     any.  */
	  file->dynamic_segment_phdr = phdr;
	  break;

	case PT_LOAD:
	  /* Is the segment read only (don't care about executable) ?  */
	  if ((phdr->p_flags & (PF_R | PF_W)) == PF_R)
	    {
	      /* Dont check for the eXecutable bit when looking for the text segment.
	         The text segment may not contain code, in which case the static linker will
	         not set the X bit. libm is an example of such a library.  */
	      file->text_segment_phdr = phdr;
	      file->text_size = phdr->p_memsz;
	    }
	  /* Is the segment readable and writable, but not executable?  */
	  else if ((phdr->p_flags & (PF_R | PF_W | PF_X)) == (PF_R | PF_W))
	    {
	      file->data_segment_phdr = phdr;
	      file->data_size = phdr->p_memsz;
	      file->bss_offset = phdr->p_filesz;
	      file->bss_size = phdr->p_memsz - phdr->p_filesz;
	      file->data_seg_8byte_aligned = ((phdr->p_offset & 7) == 0);
	    }

	  if (phdr->p_offset + phdr->p_memsz > memory_size)
	    memory_size = phdr->p_offset + phdr->p_memsz;

	  break;
	}

      phdr++;
    }

  if (file->dynamic_segment_phdr && file->data_segment_phdr)
    {
      file->dynamic_offset = file->dynamic_segment_phdr->p_offset - file->data_segment_phdr->p_offset;
      file->dynamic_size = file->dynamic_segment_phdr->p_memsz;

      Elf32_Dyn *dyn;
      int i, dyn_size = file->dynamic_segment_phdr->p_filesz / sizeof(Elf32_Dyn);

      if ((err = som_alloc (file->dynamic_segment_phdr->p_filesz, (void **) &dyn)) != NULL)
	goto error;

      fseek (file->handle, file->dynamic_segment_phdr->p_offset, SEEK_SET);
      if (fread ((void *) dyn, file->dynamic_segment_phdr->p_filesz, 1, file->handle) != 1)
	{
	  goto error;
	}

      for (i = 0; i < dyn_size; i++)
	{
	  switch (dyn[i].d_tag)
	    {
	      case DT_RISCOS_ABI_VERSION:
		{
		  unsigned dyn_offset = dyn[i].d_un.d_ptr;
		  if (file->interp_name)
		    {
		      /* We have an executable, adjust the offset to account for the application
			base address.  */
		      dyn_offset -= 0x8000;
		    }

		  fseek (file->handle, dyn_offset, SEEK_SET);
		  if (fread ((void *)file->abi_version, ABI_MAX_LEN - 1, 1, file->handle) != 1)
		    {
		      som_free (dyn);
		      err = somerr_abi_fail;
		      goto error;
		    }
		}
		break;
	      case DT_PLTGOT:
		file->got_offset = dyn[i].d_un.d_ptr - file->data_segment_phdr->p_vaddr;
		break;
	    }
	}

      som_free (dyn);

      if (strcmp (file->abi_version, armeabihf) == 0)
	file->is_armeabihf = true;
    }

  file->memory_size = memory_size;

  DEBUG_OUTPUT("%s:  abi (%s), text_size: 0x%X, data_size: 0x%X",__func__,file->abi_version,file->text_size,file->data_size);
  DEBUG_OUTPUT("%s:  bss_offset: 0x%X, bss_size: 0x%X",__func__,file->bss_offset,file->bss_size);
  DEBUG_OUTPUT("%s:  dynamic_offset: 0x%X, dynamic_size: 0x%X",__func__,file->dynamic_offset,file->dynamic_size);
  DEBUG_OUTPUT("%s:  got_offset: 0x%X, is_armeabihf: %d",__func__,file->got_offset,file->is_armeabihf);

  return NULL;

error:
  if (err == NULL
      && (err = _kernel_last_oserror ()) == NULL)
    err = somerr_file_error;	/* Default to file error.  */

  elffile_close (file);

  return err;
}

void
elffile_close (elf_file *file)
{
  if (!file || file->magic != ELF_ID)
    return;

  file->magic = 0;

  if (file->handle)
    {
      fclose (file->handle);
      file->handle = NULL;
    }

  if (file->name)
    {
      som_free (file->name);
      file->name = NULL;
    }

  if (file->prog_headers)
    {
      som_free (file->prog_headers);
      file->prog_headers = NULL;
    }
}

_kernel_oserror *
elffile_alloc_segments(elf_file *file)
{
  if (!file || file->magic != ELF_ID)
    return somerr_bad_param;

  _kernel_oserror *err;

  err = som_alloc_lib (file->memory_size, (void **)&file->text_segment);
  if (err)
    return err;

  file->data_segment = file->data_init_segment =
	file->text_segment + file->data_segment_phdr->p_offset;

  return NULL;
}

_kernel_oserror *
elffile_load (elf_file *file, bool init_bss)
{
  if (!file || file->magic != ELF_ID)
    return somerr_bad_param;

  Elf32_Phdr *phdr = file->prog_headers;
  int phnum = file->elf_header.e_phnum;
  _kernel_oserror *err = NULL;

  while (phnum--)
    {
      if (phdr->p_type == PT_LOAD)
	{
	  som_PTR segment = NULL;

	  fseek (file->handle, phdr->p_offset, SEEK_SET);

	  if ((phdr->p_flags & (PF_R | PF_W)) == PF_R)
	    {
	      segment = file->text_segment;
	      DEBUG_OUTPUT("%s: loading text segment to %p",__func__,segment);
	    }
	  else if ((phdr->p_flags & (PF_R | PF_W | PF_X)) == (PF_R | PF_W))
	    {
	      segment = file->data_init_segment;
	      DEBUG_OUTPUT("%s: loading data segment to %p",__func__,segment);
	    }

	  if (!segment)
	    {
	      err = somerr_no_seg_loader;
	      goto error;
	    }

	  /* It's possible to have a data segment that has zero size in
	     file, but none zero size in memory, ie, the entire r/w segment
	     comprises of .bss section.  */
	  if (phdr->p_filesz != 0){
	    if (fread
		((void *) segment/*(load_offset + phdr->p_vaddr)*/, phdr->p_filesz, 1,
		 file->handle) != 1)
	      goto error;
	  }
	  if (init_bss)
	    {
	      int bss_size = phdr->p_memsz - phdr->p_filesz;
	      /* If there's a difference between memory size and file size,
	         then this indicates the bss area which needs zeroing.  */
	      if (bss_size != 0)
		memset ((void *) (segment/*load_offset + phdr->p_vaddr*/ +
				  phdr->p_filesz), 0, bss_size);
	    }
	}

      phdr++;
    }

  if (!err)
    return NULL;

error:
  if (!err)
    if ((err = _kernel_last_oserror ()) == NULL)
      err = somerr_file_error;

  return err;
}

/* Fill in an elf_file structure from an image already in memory.  */
_kernel_oserror *
elffile_from_memory (elf_file *file,
		     som_PTR text_segment,
		     som_PTR elfimage_data_segment,
		     som_PTR library_init_segment)
{
  _kernel_oserror *err;

  file->text_segment = text_segment;
  file->data_segment = elfimage_data_segment;
  file->data_init_segment = library_init_segment;

  memcpy (&file->elf_header, text_segment, sizeof (Elf32_Ehdr));

  /* Allocate space for program headers and copy them from the image.  */
  if ((err = som_alloc (file->elf_header.e_phentsize * file->elf_header.e_phnum,
			(void **) (void *) &file->prog_headers)) != NULL)
    return err;

  memcpy (file->prog_headers, text_segment + file->elf_header.e_phoff,
	  file->elf_header.e_phentsize * file->elf_header.e_phnum);

  Elf32_Phdr *phdr = file->prog_headers;
  int phnum = file->elf_header.e_phnum;
  unsigned int memory_size = 0;

  while (phnum--)
    {
      switch (phdr->p_type)
	{
	case PT_INTERP:
	  /* Record the address of the name of the interpreter (dynamic
	     linker) if any.  */
	  file->interp_name = (char *) phdr->p_vaddr;
	  break;

	case PT_DYNAMIC:
	  /* Record the address of the dynamic segment program header if
	     any.  */
	  file->dynamic_segment_phdr = phdr;
	  break;

	case PT_LOAD:
	  if ((phdr->p_flags & (PF_R | PF_W)) == PF_R)
	    {
	      file->text_segment_phdr = phdr;
	      file->text_size = phdr->p_memsz;
	    }
	  else if ((phdr->p_flags & (PF_R | PF_W | PF_X)) == (PF_R | PF_W))
	    {
	      file->data_segment_phdr = phdr;
	      file->data_size = phdr->p_memsz;
	      file->bss_offset = phdr->p_filesz;
	      file->bss_size = phdr->p_memsz - phdr->p_filesz;
	      file->data_seg_8byte_aligned = ((phdr->p_offset & 7) == 0);
	    }

	  if (phdr->p_offset + phdr->p_memsz > memory_size)
	    memory_size = phdr->p_offset + phdr->p_memsz;

	  break;
	}

      phdr++;
    }

  file->dynamic_offset = file->dynamic_segment_phdr->p_offset - file->data_segment_phdr->p_offset;
  file->dynamic_size = file->dynamic_segment_phdr->p_memsz;

  Elf32_Dyn *dyn;
  int i, dyn_size = file->dynamic_segment_phdr->p_filesz / sizeof(Elf32_Dyn);

  if ((err = som_alloc (file->dynamic_segment_phdr->p_filesz, (void **) &dyn)) != NULL)
    goto error;

  memcpy (dyn,
	  file->data_init_segment + file->dynamic_offset,
	  file->dynamic_segment_phdr->p_filesz);

  for (i = 0; i < dyn_size; i++)
    {
      switch (dyn[i].d_tag)
	{
	  case DT_RISCOS_ABI_VERSION:
	    {
	      unsigned dyn_offset = dyn[i].d_un.d_ptr;
	      if (file->interp_name)
		{
		  /* We have an executable, adjust the offset to account for the application
		     base address.  */
		  dyn_offset -= 0x8000;
		}

	      memcpy (file->abi_version, text_segment + dyn_offset, ABI_MAX_LEN - 1);
	    }
	    break;
	  case DT_PLTGOT:
	    file->got_offset = dyn[i].d_un.d_ptr - file->data_segment_phdr->p_vaddr;
	    break;
	}
    }

  som_free (dyn);

  if (strcmp (file->abi_version, armeabihf) == 0)
    file->is_armeabihf = true;

  file->memory_size = memory_size;

  DEBUG_OUTPUT("%s:  abi (%s), text_size: 0x%X, data_size: 0x%X",__func__,file->abi_version,file->text_size,file->data_size);
  DEBUG_OUTPUT("%s:  text_segment: 0x%X, data_segment: 0x%X",__func__,file->text_segment,file->data_segment);
  DEBUG_OUTPUT("%s:  bss_offset: 0x%X, bss_size: 0x%X",__func__,file->bss_offset,file->bss_size);
  DEBUG_OUTPUT("%s:  dynamic_offset: 0x%X, dynamic_size: 0x%X",__func__,file->dynamic_offset,file->dynamic_size);
  DEBUG_OUTPUT("%s:  got_offset: 0x%X, is_armeabihf: %d",__func__,file->got_offset,file->is_armeabihf);

  return NULL;

error:
  elffile_close (file);
  return err;
}

_kernel_oserror *
elffile_get_public_info (elf_file *file,
			 som_objinfo *info)
{
  if (!file || file->magic != ELF_ID || !info)
    return somerr_bad_param;

  memset(info, 0, sizeof (som_objinfo));

  info->text_segment = file->text_segment;
  info->text_size = file->text_size;
  info->library_data_segment = file->data_segment;
  info->library_init_segment = file->data_init_segment;
  info->client_data_segment = NULL;
  info->data_size = file->data_size;
  info->got_offset = file->got_offset;
  info->bss_offset = file->bss_offset;
  info->bss_size = file->bss_size;
  info->dyn_offset = file->dynamic_offset;
  info->dyn_size = file->dynamic_size;
  if (file->is_armeabihf)
    info->flags.is_armeabihf = true;
  if (file->data_seg_8byte_aligned)
    info->flags.data_seg_8byte_aligned = true;
  info->name = file->name;

  return NULL;
}
