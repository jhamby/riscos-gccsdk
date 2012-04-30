/* Load an ELF sharable library into memory.

   Copyright (C) 1993-1996, Eric Youngdale.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */



/* This file contains the helper routines to load an ELF sharable
   library into memory and add the symbol table info to the chain. */

#ifdef __riscos

#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#else
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/errno.h>
#include <linux/mman.h>
#include <linux/stat.h>
#endif /* __riscos */
#include "string.h"
/*#include <stdlib.h>*/
#include "hash.h"
#include "linuxelf.h"
#include "sysdep.h"
#ifdef __riscos
#define MAP_ANONYMOUS 0x20
#include <unistd.h>
#else
#include <linux/unistd.h>
#endif /* __riscos */
#include "syscall.h"
#ifdef USE_CACHE
#include "../config.h"
#endif

extern char *_dl_progname;

#ifdef USE_CACHE

static caddr_t _dl_cache_addr = NULL;
static size_t _dl_cache_size = 0;

int _dl_map_cache(void)
{
  int fd;
  struct stat st;
  header_t *header;
  libentry_t *libent;
  int i, strtabsize;

  if (_dl_cache_addr == (caddr_t)-1)
    return -1;
  else if (_dl_cache_addr != NULL)
    return 0;

  if (_dl_stat(LDSO_CACHE, &st) || (fd = _dl_open(LDSO_CACHE, O_RDONLY)) < 0)
  {
    _dl_fdprintf(2, "%s: can't open cache '%s'\n", _dl_progname, LDSO_CACHE);
    _dl_cache_addr = (caddr_t)-1; /* so we won't try again */
    return -1;
  }

  _dl_cache_size = st.st_size;
  _dl_cache_addr = (caddr_t)_dl_mmap(0, _dl_cache_size, PROT_READ,
				     MAP_SHARED, fd, 0);
  _dl_close (fd);
  if (_dl_cache_addr == (caddr_t)-1)
  {
    _dl_fdprintf(2, "%s: can't map cache '%s'\n", _dl_progname, LDSO_CACHE);
    return -1;
  }

  header = (header_t *)_dl_cache_addr;

  if (_dl_cache_size < sizeof (header_t) ||
      _dl_memcmp(header->magic, LDSO_CACHE_MAGIC, LDSO_CACHE_MAGIC_LEN) ||
      _dl_memcmp(header->version, LDSO_CACHE_VER, LDSO_CACHE_VER_LEN) ||
      _dl_cache_size <
        (sizeof (header_t) + header->nlibs * sizeof (libentry_t)) ||
      _dl_cache_addr[_dl_cache_size-1] != '\0')
  {
    _dl_fdprintf(2, "%s: cache '%s' is corrupt\n", _dl_progname, LDSO_CACHE);
    goto fail;
  }

  strtabsize = _dl_cache_size - sizeof (header_t) -
    header->nlibs * sizeof (libentry_t);
  libent = (libentry_t *)&header[1];

  for (i = 0; i < header->nlibs; i++)
  {
    if (libent[i].sooffset >= strtabsize ||
	libent[i].liboffset >= strtabsize)
    {
      _dl_fdprintf(2, "%s: cache '%s' is corrupt\n", _dl_progname, LDSO_CACHE);
      goto fail;
    }
  }

  return 0;

fail:
  _dl_munmap(_dl_cache_addr, _dl_cache_size);
  _dl_cache_addr = (caddr_t)-1;
  return -1;
}

int _dl_unmap_cache(void)
{
  if (_dl_cache_addr == NULL || _dl_cache_addr == (caddr_t)-1)
    return -1;

#if 1
  _dl_munmap (_dl_cache_addr, _dl_cache_size);
  _dl_cache_addr = NULL;
#endif

  return 0;
}

#endif

/*
 * Used to return error codes back to dlopen et. al.
 */

unsigned int _dl_error_number;
unsigned int _dl_internal_error_number;

struct elf_resolve *
_dl_load_shared_library(struct elf_resolve * app_tpnt,
			struct elf_resolve * tpnt,
			char * full_libname)
{
  char * pnt, *pnt1, *pnt2;
  struct elf_resolve *tpnt1 = NULL;
  char mylibname[2050];
  char * libname;

  _dl_internal_error_number = 0;

  /* quick hack to ensure mylibname buffer doesn't overflow.  don't
     allow full_libname or any directory to be longer than 1024. */
  if (_dl_strlen(full_libname) > 1024)
    goto goof;

  pnt = libname = full_libname;
#ifndef __riscos
  while (*pnt) {
    if(*pnt == '.') libname = pnt+1;
    pnt++;
  }

 /* If the filename has any '.', try it straight and leave it at that. */
  if (libname != full_libname) {
    tpnt1 = _dl_load_elf_shared_library(full_libname, 0);
    if (tpnt1)
      return tpnt1;
    goto goof;
  }
#endif

  /*
   * The ABI specifies that RPATH is searched before LD_*_PATH or
   * the default path of /usr/lib.
   * Check in rpath directories
   */
  for (tpnt = _dl_loaded_modules; tpnt; tpnt = tpnt->next) {
    if (tpnt->libtype == elf_executable) {
      pnt1 = (char *)tpnt->dynamic_info[DT_RPATH];
      if(pnt1) {
	pnt1 += (unsigned int) tpnt->loadaddr + tpnt->dynamic_info[DT_STRTAB];
	while(*pnt1){
	  pnt2 = mylibname;
	  while(*pnt1 && *pnt1 != ':') {
	    if (pnt2 - mylibname < 1024)
	      *pnt2++ = *pnt1++;
	    else
	      pnt1++;
	  }
	  if (pnt2 - mylibname >= 1024)
	    break;
	  if(pnt2[-1] != '/') *pnt2++ = '/';
	  pnt = libname;
	  while(*pnt) *pnt2++  = *pnt++;
	  *pnt2++ = 0;
	  tpnt1 = _dl_load_elf_shared_library(mylibname, 0);
	  if(tpnt1) return tpnt1;
	  if(*pnt1 == ':') pnt1++;
	}
      }
    }
  }

  /* Check in LD_{ELF_}LIBRARY_PATH, if specified and allowed */
  pnt1 = _dl_library_path;
  if (pnt1 && *pnt1) {
    while (*pnt1) {
      pnt2 = mylibname;
      while(*pnt1 && *pnt1 != ',' && *pnt1 != ';') {
	if (pnt2 - mylibname < 1024)
	  *pnt2++ = *pnt1++;
	else
	  pnt1++;
      }
      if (pnt2 - mylibname >= 1024)
	break;
      if(pnt2[-1] != '.' && pnt2[-1] != ':') *pnt2++ = '.';
      pnt = libname;
      while(*pnt) *pnt2++  = *pnt++;
      *pnt2++ = 0;
      tpnt1 = _dl_load_elf_shared_library(mylibname, 0);
      if(tpnt1) return tpnt1;
      if(*pnt1 == ',' || *pnt1 == ';') pnt1++;
    }
  }


  /*
   * Where should the cache be searched?  There is no such concept in the
   * ABI, so we have some flexibility here.  For now, search it before
   * the default path of /usr/lib.
   */
#ifdef USE_CACHE
  if (_dl_cache_addr != NULL && _dl_cache_addr != (caddr_t)-1)
  {
    int i;
    header_t *header = (header_t *)_dl_cache_addr;
    libentry_t *libent = (libentry_t *)&header[1];
    char *strs = (char *)&libent[header->nlibs];

    for (i = 0; i < header->nlibs; i++)
    {
      if ((libent[i].flags == LIB_ELF ||
	   libent[i].flags == LIB_ELF_LIBC5) &&
	  _dl_strcmp(libname, strs+libent[i].sooffset) == 0 &&
	  (tpnt1 = _dl_load_elf_shared_library(strs+libent[i].liboffset, 0)))
	return tpnt1;
    }
  }
#endif

  /* Check in /SharedLibs:/lib/<ABI version>/ */
  pnt1 = "/SharedLibs:/lib/";

  /* Copy the root name.  */
  pnt = mylibname;
  while(*pnt1) *pnt++ = *pnt1++;

  /* Remember where the end of the root name is.  */
  pnt2 = pnt; 

  /* Copy abi version string of application.  */
  if (app_tpnt->abi_version)
  {
    pnt1 = app_tpnt->abi_version;
    while(*pnt1) *pnt++ = *pnt1++;

    *pnt++ = '/';

    /* Copy the name of the library.  */
    pnt1 = libname;
    while(*pnt1) *pnt++ = *pnt1++;
    *pnt = '\0';

    if ((tpnt1 = _dl_load_elf_shared_library(mylibname, 0)) != NULL)
      return tpnt1;
  }

  /* Check in /SharedLibs:/lib/ */
  pnt1 = libname;
  pnt = pnt2;
  while(*pnt1) *pnt++ = *pnt1++;
  *pnt = '\0';
  if ((tpnt1 = _dl_load_elf_shared_library(mylibname, 0)) != NULL)
    return tpnt1;

goof:
  /* Well, we shot our wad on that one.  All we can do now is punt */
  if (_dl_internal_error_number) _dl_error_number = _dl_internal_error_number;
	else _dl_error_number = DL_ERROR_NOFILE;
  return NULL;
}

/*
 * Read one ELF library into memory, mmap it into the correct locations and
 * add the symbol info to the symbol chain.  Perform any relocations that
 * are required.
 */

//extern _elf_rtbndr(void);

struct elf_resolve *
_dl_load_elf_shared_library(char * libname, int flag)
{
  Elf32_Ehdr * epnt;
  unsigned int dynamic_addr = 0;
  unsigned int dynamic_size = 0;
  Elf32_Dyn * dpnt;
  struct elf_resolve * tpnt;
  Elf32_Phdr * ppnt;
  unsigned int dynamic_info[24];
  int * lpnt;
  unsigned int libaddr;
  char *abi_version = NULL;
  int i;

  unsigned int handle;
  struct object_info objinfo;
  int infile = -1;

  extern char *_dl_riscosify_dl(const char *name, int create_dir);
  libname = _dl_riscosify_dl(libname, 0);

  libname = _dl_resolve_symlinks(libname);
  if (!libname)
    return NULL;

  /* If this file is already loaded for the current client , skip this step */
  tpnt = _dl_check_hashed_files(libname);

  if(tpnt) 
  {
    _dl_som_free(libname);
    return tpnt;
  }

  handle = _dl_check_system_files(libname);

  if (handle != 0)
  {
    /*
     * If the library is already loaded by the system, then retrieve its details
     */
    _dl_query_object_global(handle,&objinfo);

     dynamic_addr = (unsigned int)(objinfo.public_rw_ptr + objinfo.dyn_offset);
     dynamic_size = objinfo.dyn_size;
     epnt = (Elf32_Ehdr *)objinfo.base_addr;
     libaddr = (unsigned int)objinfo.base_addr;
  }
  else
  {
  char header[4096];
  unsigned int memsize = 0;

    libaddr = 0;
    infile = _dl_open(libname, O_RDONLY);
    if(infile < 0)
    {
#if 0
      /*
       * NO!  When we open shared libraries we may search several paths.
       * it is inappropriate to generate an error here.
       */
      _dl_fdprintf(2, "%s: can't open '%s'\n", _dl_progname, libname);
#endif
      _dl_internal_error_number = DL_ERROR_NOFILE;
      goto null_exit;
    }

    _dl_read(infile, header, sizeof(header));
    epnt = (Elf32_Ehdr *) header;
    if (epnt->e_ident[0] != 0x7f ||
        epnt->e_ident[1] != 'E' ||
        epnt->e_ident[2] != 'L' ||
        epnt->e_ident[3] != 'F')
    {
      _dl_fdprintf(2, "%s: '%s' is not an ELF file\n", _dl_progname, libname);
      _dl_internal_error_number = DL_ERROR_NOTELF;
      goto null_exit;
    }

    if((epnt->e_type != ET_DYN) ||
       (epnt->e_machine != MAGIC1
#ifdef MAGIC2
        && epnt->e_machine != MAGIC2
#endif
        ))
    {
      _dl_internal_error_number = (epnt->e_type != ET_DYN ? DL_ERROR_NOTDYN : DL_ERROR_NOTMAGIC);
      _dl_fdprintf(2, "%s: '%s' is not an ELF executable for " ELF_TARGET "\n",
		 _dl_progname, libname);
      goto null_exit;
    }

    ppnt = (Elf32_Phdr *) &header[epnt->e_phoff];

    /*
     * Work out how much memory is required by adding up the sizes of
     * the loadable segments
     */
    for (i = 0;i < epnt->e_phnum; i++)
    {

      if (ppnt->p_type == PT_DYNAMIC)
      {
        if (dynamic_addr)
	  _dl_fdprintf(2, "%s: '%s' has more than one dynamic section\n",
		       _dl_progname, libname);
        dynamic_addr = ppnt->p_vaddr;
        dynamic_size = ppnt->p_filesz;
      }
      else if (ppnt->p_type == PT_LOAD)
      {
        /* We use the file size because the bss section will not be used in the library's copy */
        if (ppnt->p_vaddr + ppnt->p_filesz > memsize)
          memsize = ppnt->p_vaddr + ppnt->p_filesz;
      }

      ppnt++;
    }
    /* Get the memory to store the library */
    libaddr = _dl_alloc_lib(memsize);
    if (!libaddr)
    {
      _dl_fdprintf(2,"Unable to allocate memory for shared library\n");
      goto null_exit;
    }

    objinfo.base_addr = (char *)libaddr;

    /* We assume that all loadable segments are consequtive in memory */
    ppnt = (Elf32_Phdr *) &header[epnt->e_phoff];

    for(i=0;i < epnt->e_phnum; i++)
    {
      if(ppnt->p_type == PT_LOAD)
      {
        if(ppnt->p_flags & PF_W)
        { /* A read/write loadable segment contains data */
 	  objinfo.public_rw_ptr = (char *)(libaddr + ppnt->p_vaddr);
	  objinfo.rw_size = ppnt->p_memsz;
	  objinfo.bss_offset = ppnt->p_filesz;
	  objinfo.bss_size = ppnt->p_memsz - ppnt->p_filesz;

	  /* This is the public version which contains default values */
	  if (_dl_set_file_pos(infile, ppnt->p_offset) ||
	      _dl_read(infile, objinfo.public_rw_ptr, ppnt->p_filesz))
	  {
	    _dl_fdprintf(2,"Failed to read library data segment\n\r");
	    goto null_exit;
	  }
        }
        else
        { /* A read only loadable segment contains code */
          if (_dl_set_file_pos(infile, ppnt->p_offset) ||
              _dl_read(infile, (char *)(libaddr + ppnt->p_vaddr), ppnt->p_filesz))
          {
            _dl_fdprintf(2,"Failed to read library code segment\n\r");
	    goto null_exit;
          }
        }
      }
      ppnt++;
    }

    _dl_close(infile);
    infile = -1;

    dynamic_addr += (unsigned int) libaddr;

    handle = libaddr;
    objinfo.dyn_offset = (unsigned int)dynamic_addr - (unsigned int)objinfo.public_rw_ptr;
    objinfo.dyn_size = dynamic_size;
  }

 /*
  * OK, the ELF library is now loaded into VM in the correct locations
  * The next step is to go through and do the dynamic linking (if needed).
  */

    /* Start by scanning the dynamic section to get all of the pointers */

    if(!dynamic_addr)
    {
      _dl_internal_error_number = DL_ERROR_NODYNAMIC;
      _dl_fdprintf(2, "%s: '%s' is missing a dynamic section\n", _dl_progname, libname);
      goto null_exit;
    }

    dpnt = (Elf32_Dyn *) dynamic_addr;

    dynamic_size = dynamic_size / sizeof(Elf32_Dyn);

    _dl_memset(dynamic_info, 0, sizeof(dynamic_info));
    for(i=0; i< dynamic_size; i++)
    {
      if (dpnt->d_tag == DT_RISCOS_ABI_VERSION)
	abi_version = (char *)(dpnt->d_un.d_ptr + libaddr);
      if( dpnt->d_tag > DT_JMPREL ) {dpnt++; continue; }
      dynamic_info[dpnt->d_tag] = dpnt->d_un.d_val;
      if(dpnt->d_tag == DT_TEXTREL ||
         SVR4_BUGCOMPAT) dynamic_info[DT_TEXTREL] = 1;
      dpnt++;
    }

    tpnt = _dl_add_elf_hash_table(libname, (char *) libaddr, dynamic_info, dynamic_addr,
			      dynamic_size);

    tpnt->ppnt = (Elf32_Phdr *) (tpnt->loadaddr + epnt->e_phoff);
    tpnt->n_phent = epnt->e_phnum;
    tpnt->abi_version = abi_version;

    /* Scan for exception tables.  */
    {
    Elf32_Ehdr *elf_image_hdr = (Elf32_Ehdr *)libaddr;

      tpnt->endaddr = objinfo.public_rw_ptr;
      for(ppnt = (Elf32_Phdr *)(libaddr + elf_image_hdr->e_phoff),
	  i = 0;
	  i < elf_image_hdr->e_phnum && ppnt->p_type != PT_ARM_EXIDX;
	  ppnt++,
	  i++)
	/* Empty loop.  */;

      if (i < elf_image_hdr->e_phnum)
      {
	tpnt->exidx = (_Unwind_Ptr)(libaddr + ppnt->p_vaddr);
	tpnt->exidx_size = ppnt->p_memsz;
      }
      else
      {
	tpnt->exidx = 0;
	tpnt->exidx_size = 0;
      }
    }

    /*
     * If the client has already seen and registered the library, then don't do it
     * again
     */
    if (_dl_query_object_client(handle, NULL) < 0)
    {
    char *got;

      got = (char *)(dynamic_info[DT_PLTGOT] + libaddr);

      objinfo.got_offset = (got - objinfo.public_rw_ptr);

      /* Allocate the memory for the client's own copy of the r/w segment. Note that
       * the relocation code uses the data in the public copy to correctly relocate
       * the same data in the private copy.
       */
      if ((objinfo.private_rw_ptr = _dl_malloc(objinfo.rw_size)) == NULL)
	goto null_exit;

      /* Register the library with the support module. During registration an object index
       * is written into the GOT. This index needs to be copied into the private version
       * along with the rest of the GOT contents. It's therefore important that registration
       * occurs before the copying takes place.
       */
      objinfo.name = libname;
      _dl_register_lib(handle,&objinfo);

      /*  Copy the r/w segment. */
      _dl_memcpy(objinfo.private_rw_ptr, objinfo.public_rw_ptr, objinfo.rw_size - objinfo.bss_size);

      /* zero the bss area */
      for (i = 0; i < objinfo.bss_size; i++)
        objinfo.private_rw_ptr[objinfo.bss_offset + i] = 0;
    }

  /*
   * OK, the next thing we need to do is to insert the dynamic linker into
   * the proper entry in the GOT so that the PLT symbols can be properly
   * resolved.
   */

  /*
    The DT_PLTGOT entry does not exist in the dynamic segment if there is no
    .plt section, even if there is a .got section.
   */
  lpnt = (int *) dynamic_info[DT_PLTGOT];

  if (lpnt)
  {
    lpnt = (int *) (objinfo.private_rw_ptr + objinfo.got_offset);
     /* ABI v0.00 & v1.00 have 5 word GOTs with a zero at offset 3.
        ABI v2.00 has a 3 word GOT, offset 3 will not be zero.  */
    if (lpnt[3] == 0 &&
        (tpnt->abi_version == NULL ||
        _dl_strncmp (tpnt->abi_version, "abi-1.0", 7) == 0))
    {
      INIT_41_GOT (lpnt, tpnt)
    }
    else
      INIT_GOT (lpnt, tpnt)
  }

  _dl_som_free(libname);

  return tpnt;

null_exit:
  if (infile >= 0)
    _dl_close(infile);

  _dl_som_free(libname);

  return NULL;
}

/* Ugly, ugly.  Some versions of the SVr4 linker fail to generate COPY
   relocations for global variables that are present both in the image and
   the shared library.  Go through and do it manually.  If the images
   are guaranteed to be generated by a trustworthy linker, then this
   step can be skipped. */

int _dl_copy_fixups(struct dyn_elf * rpnt)
{
  int goof = 0;
  struct elf_resolve * tpnt;
#ifdef BROKEN_LINKER
  int hn, hn1, si, si1;
  unsigned int elf_hash_number;
  struct elf_resolve * tpnt1;
  struct dyn_elf * rpnt1;
  char * strtab, *strtab1;
  Elf32_Sym * symtab, *symtab1;
  char * pnt, *pnt1;
#endif

  if(rpnt->next) goof += _dl_copy_fixups(rpnt->next);
  else return 0;

  tpnt = rpnt->dyn;

  if (tpnt->init_flag & COPY_RELOCS_DONE) return goof;
  tpnt->init_flag |= COPY_RELOCS_DONE;

/* Use BROKEN_LINKER if the SVr4 linker is being used. */
#ifndef BROKEN_LINKER
#ifdef ELF_USES_RELOCA
  goof += _dl_parse_copy_information(rpnt, tpnt->dynamic_info[DT_RELA],
					   tpnt->dynamic_info[DT_RELASZ], 0);

#else
  goof += _dl_parse_copy_information(rpnt, tpnt->dynamic_info[DT_REL],
					   tpnt->dynamic_info[DT_RELSZ], 0);

#endif
#else
  /* OK, now scan the symbol table for this module */

  symtab = (Elf32_Sym *) (tpnt->dynamic_info[DT_SYMTAB] +
				 tpnt->loadaddr);
  strtab = (char *) (tpnt->dynamic_info[DT_STRTAB] + tpnt->loadaddr);

  for(hn = 0; hn < tpnt->nbucket; hn++) {
    if(!tpnt->elf_buckets[hn]) continue;
    for(si = tpnt->elf_buckets[hn]; si; si = tpnt->chains[si]) {
      if(ELF32_ST_TYPE(symtab[si].st_info) != STT_OBJECT) continue;
      if(ELF32_ST_BIND(symtab[si].st_info) != STB_GLOBAL) continue;
      if(symtab[si].st_value == 0 || symtab[si].st_shndx == 0) continue;
      if(!symtab[si].st_size) continue;
      pnt = strtab + symtab[si].st_name;

      elf_hash_number = _dl_elf_hash(pnt);

      /* OK, we have a candidate.  Now search for the same symbol in other
	 libraries.  The hash number will be the same. */
      for(rpnt1 = rpnt->next; rpnt1; rpnt1 = rpnt1->next)
	{
	  tpnt1 = rpnt1->dyn;
	  hn1 = elf_hash_number % tpnt1->nbucket;
	  if(!tpnt1->elf_buckets[hn1]) continue;

	  symtab1 = (Elf32_Sym *) (tpnt1->dynamic_info[DT_SYMTAB] +
					 tpnt1->loadaddr);
	  strtab1 = (char *) (tpnt1->dynamic_info[DT_STRTAB] + tpnt1->loadaddr);
	  for(si1 = tpnt1->elf_buckets[hn1]; si1; si1 = tpnt1->chains[si1]) {
	    pnt1 = strtab1 + symtab1[si1].st_name;
	    if(ELF32_ST_TYPE(symtab1[si1].st_info) != STT_OBJECT) continue;
	    if(ELF32_ST_BIND(symtab1[si1].st_info) != STB_GLOBAL) continue;
	    if(symtab1[si1].st_value == 0 || symtab1[si1].st_shndx == 0) continue;
	    if(!symtab1[si1].st_size) continue;
	    if(symtab1[si1].st_size != symtab[si].st_size) continue;
	    pnt1 = strtab1 + symtab1[si1].st_name;
	    if(_dl_strcmp(pnt, pnt1) == 0) {
	      char * to, *from;
	      int count;
	      to = tpnt->loadaddr + symtab[si].st_value;
	      from = tpnt1->loadaddr + symtab1[si1].st_value;
	      count = symtab[si].st_size;
	      while(count--) *to++ = *from++;
	      /* Cannot use memcpy - SVr4 assembler complains about dup label*/
#if 0
	      _dl_fdprintf(2, "Global symbol in %s %s\n", tpnt1->libname, pnt);
#endif
	    };
	  };
	};
    };
  };
#endif
  return goof;
}
