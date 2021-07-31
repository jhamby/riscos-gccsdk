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

static struct elf_resolve *
_dl_try_path (char * const buffer,
	      const char *abi_dir,
	      const char *vfp_dir,
	      const char *library_name)
{
  const char *root_path = "/SharedLibs:/lib/";
  char *path_end = buffer;
  while (*root_path) *path_end++ = *root_path++;

  if (abi_dir)
  {
    /* Copy abi version string of application.  */
    while (*abi_dir) *path_end++ = *abi_dir++;
    *path_end++ = '/';
    *path_end = '\0';
  }

  if (vfp_dir)
  {
    while (*vfp_dir) *path_end++ = *vfp_dir++;
    *path_end++ = '/';
    *path_end = '\0';
  }

  /* Copy the name of the library.  */
  while(*library_name) *path_end++ = *library_name++;
  *path_end = '\0';

  return _dl_load_elf_shared_library(buffer, 0);
}

struct elf_resolve *
_dl_load_shared_library(struct elf_resolve * app_tpnt,
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

  while (*pnt) {
    if(*pnt == '/') libname = pnt+1;
    pnt++;
  }

 /* If the filename has any '/', try it straight and leave it at that. */
  if (libname != full_libname) {
    tpnt1 = _dl_load_elf_shared_library(full_libname, 0);
    if (tpnt1)
      return tpnt1;
    goto goof;
  }

  /*
   * The ABI specifies that RPATH is searched before LD_*_PATH or
   * the default path of /usr/lib.
   * Check in rpath directories
   */
  struct elf_resolve *tpnt;
  for (tpnt = _dl_loaded_modules; tpnt; tpnt = tpnt->next) {
    if (tpnt->libtype == elf_executable) {
      pnt1 = (char *)tpnt->dynamic_info[DT_RPATH];
      if(pnt1) {
	pnt1 += (unsigned int) tpnt->loadaddr + tpnt->dynamic_info[DT_STRTAB];
	while(*pnt1){
	  pnt2 = mylibname;
	  while(*pnt1 && *pnt1 != ',') {
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
	  if(*pnt1 == ',') pnt1++;
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

  if (app_tpnt->abi_version)
  {
    if (app_tpnt->elf_flags & EF_ARM_VFP_FLOAT)
    {
      /* We have a VFP compiled executable; check in /SharedLibs:/lib/<ABI version>/vfp */
      if ((tpnt1 = _dl_try_path (mylibname, app_tpnt->abi_version, "vfp", libname)) != NULL)
	return tpnt1;
    }

    /* No VFP library or not a VFP executable; check in /SharedLibs:/lib/<ABI version> */
    if ((tpnt1 = _dl_try_path (mylibname, app_tpnt->abi_version, NULL, libname)) != NULL)
      return tpnt1;
  }

  /* Last resort; check in /SharedLibs:/lib/ */
  if ((tpnt1 = _dl_try_path (mylibname, NULL, NULL, libname)) != NULL)
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
  unsigned int dynamic_info[29];
  int * lpnt;
  unsigned int libaddr;
  char *abi_version = NULL;
  int i;

  unsigned int handle;
  struct object_info objinfo;
  unsigned registered = 0;
  os_error *err = NULL;

  _dl_memset(&objinfo, 0, sizeof(objinfo));

  extern char *_dl_riscosify_dl(const char *name, int create_dir);
  libname = _dl_riscosify_dl(libname, 0);

  libname = _dl_resolve_symlinks(libname);
  if (!libname)
    return NULL;

  /* If this file is already loaded by the dynamic linker for the current client, skip this step */
  tpnt = _dl_check_hashed_files(libname);
  if(tpnt) 
  {
    _dl_som_free(libname);
    return tpnt;
  }

  /* Library has not been seen by the dynamic linker for this client, how about the
   * Shared Object Manager?  */
  handle = _dl_check_system_files(libname);
  if (handle != 0)
  {
    /*
     * If the library is already loaded by the Shared Object Manager then retrieve its details
     */
    _dl_query_object_global(handle, &objinfo);
  }
  else
  {
    /* Shared Object Manager hasn't seen this library yet either.  */
    elffile_handle infile;

    err = _dl_som_elffile_open (libname, &infile);
    if (err)
    {
      _dl_internal_error_number = DL_ERROR_NOFILE;
      goto null_exit;
    }

    /* TODO: Check ABI of library against ABI of executable.  */

    if (!err)
      err = _dl_som_elffile_alloc (infile);

    if (!err)
      err = _dl_som_elffile_load (infile);

    if (!err)
      err = _dl_som_get_public_info (infile, &objinfo);

    _dl_som_elffile_close(infile);

    if (err)
    {
      _dl_fdprintf(2, "%s: %s: OS Error: %s\n", _dl_progname, libname, err->errmess);
      goto null_exit;
    }

    handle = (unsigned)objinfo.text_segment;
  }

  dynamic_addr = (unsigned int)(objinfo.library_data_segment + objinfo.dyn_offset);
  dynamic_size = objinfo.dyn_size;
  epnt = (Elf32_Ehdr *)objinfo.text_segment;
  libaddr = (unsigned int)objinfo.text_segment;

  /*
   * If the client has already seen and registered the library, then don't do it
   * again
   */
  registered = (_dl_query_object_client(handle, NULL) == 0);
  if (!registered)
  { 
    /* Allocate the memory for the client's own copy of the r/w segment. Note that
     * the relocation code uses the data in the public copy to correctly relocate
     * the same data in the private copy.
     * 
     * Add 4 to the size in case adjustment for alignment is required.
     */
    if ((objinfo.client_data_segment = _dl_malloc(objinfo.data_size + 4)) == NULL)
      goto null_exit;
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
      if( dpnt->d_tag > DT_FINI_ARRAYSZ ) {dpnt++; continue; }
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

    {
    Elf32_Ehdr *elf_image_hdr = (Elf32_Ehdr *)libaddr;

      /* Scan for exception tables.  */
      tpnt->endaddr = objinfo.elfimage_data_segment;
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

      /* Read the ELF header flags.  */
      tpnt->elf_flags = elf_image_hdr->e_flags;
    }

    /*
     * If the client has already seen and registered the library, then don't do it
     * again
     */
    if (!registered)
    {
      /* We have to be careful how we align the start of the block. It must match the
       * alignment as it is stored in the ELF file so that any 64bit variables that may
       * be accessed via a double word load/store instruction will have correct alignment.
       * This does not mean that it should always start on a double word bounadary. If in the file,
       * it starts on a non double word boundary, then we need to ensure that that is
       * also the case when allocating the client's block here to preserve the alignment of
       * any contained 64bit variables.
       */
      int align = objinfo.flags.data_seg_8byte_aligned ? 8 : 4;

      /* Save this so that dlclose() can free client R/W data segments when unloading
       * a library. A library loaded during initialisation has its R/W segment stored
       * outside the heap and so should never be freed.  */
      tpnt->rw_addr = objinfo.client_data_segment;
      if ((align == 8 && ((unsigned)tpnt->rw_addr & 7) != 0) || (align == 4 && ((unsigned)tpnt->rw_addr & 7) == 0))
	objinfo.client_data_segment += 4;

      /* Register the library with the support module.  */
      objinfo.name = libname;
      err = _dl_register_lib(handle, &objinfo);
      if (err)
      {
	_dl_fdprintf(2, "%s: %s: OS Error: %s\n", _dl_progname, libname, err->errmess);
	goto null_exit;
      }
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
    lpnt = (int *) (objinfo.client_data_segment + objinfo.got_offset);
    /* ABI v0.00 & v1.00 have 5 word GOTs with a zero at offset 3.
       ABI v2.00 & armeabihf has a 3 word GOT, offset 3 will not be zero.  */
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
