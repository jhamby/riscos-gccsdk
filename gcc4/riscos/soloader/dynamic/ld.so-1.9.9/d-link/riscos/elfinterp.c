/* Run an ELF binary on a linux system.

   Copyright (C) 1993, Eric Youngdale.

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

/*#define DEBUG*/

#ifndef VERBOSE_DLINKER
#define VERBOSE_DLINKER
#endif
#ifdef VERBOSE_DLINKER
static char * _dl_reltypes[] = {"R_ARM_NONE", "R_ARM_PC_24", "R_ARM_ABS32", "R_ARM_REL32",
				"R_ARM_PC13", "R_ARM_ABS16", "R_ARM_ABS12", "R_ARM_THM_ABS5",
				"R_ARM_ABS8", "R_ARM_SBREL32", "R_ARM_THM_PC22", "R_ARM_THM_PC8",
				"R_ARM_AMP_VCALL9", "R_ARM_SWI24", "R_ARM_THM_SWI8", "R_ARM_XPC25",
				"R_ARM_THM_XPC22", "R_ARM_COPY", "R_ARM_GLOB_DATA", "R_ARM_JMP_SLOT",
				"R_ARM_RELATIVE", "R_ARM_GOTOFF", "R_ARM_GOTPC", "R_ARM_GOT32",
				"R_ARM_PLT32"};
#endif

/* Program to load an ELF binary on a linux system, and run it.
References to symbols in sharable libraries can be resolved by either
an ELF sharable library or a linux style of shared library. */

/* Disclaimer:  I have never seen any AT&T source code for SVr4, nor have
   I ever taken any courses on internals.  This program was developed using
   information available through the book "UNIX SYSTEM V RELEASE 4,
   Programmers guide: Ansi C and Programming Support Tools", which did
   a more than adequate job of explaining everything required to get this
   working. */

#ifdef __riscos
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/elf.h>
#else
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/unistd.h>
/*#include <stdlib.h>*/
#include "string.h"
#ifdef IBCS_COMPATIBLE
#include <ibcs/unistd.h>
#else
#include <linux/unistd.h>
#endif
#include <linux/fcntl.h>
#include <linux/elf.h>
#endif /* __riscos */

#include "hash.h"
#include "linuxelf.h"
#include "syscall.h"
#include "string.h"

#define SVR4_COMPATIBILITY

extern char *_dl_progname;

unsigned int __attribute__ ((visibility ("hidden")))
_dl_riscos_resolver(struct elf_resolve *tpnt, int reloc_entry)

{
  int reloc_type;
  Elf32_Rel * this_reloc;
  char * strtab;
  Elf32_Sym * symtab;
  Elf32_Rel * rel_addr;
  int symtab_index;
  char * new_addr;
  char ** got_addr;
  unsigned int instr_addr;
  unsigned int *got;
  struct som_rt_elem *objinfo;

  rel_addr = (Elf32_Rel *) (tpnt->dynamic_info[DT_JMPREL] +
				   tpnt->loadaddr);

  this_reloc = rel_addr + (reloc_entry >> 3);
  reloc_type = ELF32_R_TYPE(this_reloc->r_info);
  symtab_index = ELF32_R_SYM(this_reloc->r_info);

  symtab =  (Elf32_Sym *) (tpnt->dynamic_info[DT_SYMTAB] + tpnt->loadaddr);
  strtab = (char *) (tpnt->dynamic_info[DT_STRTAB] + tpnt->loadaddr);

  if (reloc_type != R_ARM_JUMP_SLOT)
  {
    _dl_fdprintf(2, "%s: Incorrect relocation type in jump relocations\n",
		 _dl_progname);
    _dl_exit(1);
  }

  got = (unsigned int *)(tpnt->dynamic_info[DT_PLTGOT] + tpnt->loadaddr);

  objinfo = *((struct som_rt_elem **)got[2]) + got[1];

  instr_addr = (((int)this_reloc->r_offset + (int)tpnt->loadaddr) -
		(int)objinfo->public_rw_ptr) + (int)objinfo->private_rw_ptr;

//  instr_addr  = ((int)this_reloc->r_offset  + (int)tpnt->loadaddr);
  got_addr = (char **) instr_addr;

#ifdef DEBUG
  _dl_fdprintf(2, "Resolving symbol %s\n",
	strtab + symtab[symtab_index].st_name);
#endif
  /* Get the address of the GOT entry */
  new_addr = _dl_find_hash(strtab + symtab[symtab_index].st_name,
  			tpnt->symbol_scope, 0, tpnt, 0);

  if(!new_addr) {
    _dl_fdprintf(2, "%s: can't resolve symbol '%s'\n",
	       _dl_progname, strtab + symtab[symtab_index].st_name);
    _dl_exit(1);
  }
  *got_addr = new_addr;

  return (unsigned int)new_addr; // keep compiler happy
}

void _dl_parse_lazy_relocation_information(struct elf_resolve * tpnt, int rel_addr,
       int rel_size, int type){
  int i;
  char * strtab;
  int reloc_type;
  int symtab_index;
  Elf32_Sym * symtab;
  Elf32_Rel * rpnt;
  unsigned int * reloc_addr;
  struct som_rt_elem *objinfo;
  unsigned int *got;

  got = (unsigned int *)(tpnt->dynamic_info[DT_PLTGOT] + tpnt->loadaddr);

  objinfo = *((struct som_rt_elem **)got[2]) + got[1];

  /* Now parse the relocation information */
  rpnt = (Elf32_Rel *) (rel_addr + tpnt->loadaddr);
  rel_size = rel_size / sizeof(Elf32_Rel);

  symtab =  (Elf32_Sym *) (tpnt->dynamic_info[DT_SYMTAB] + tpnt->loadaddr);
  strtab = ( char *) (tpnt->dynamic_info[DT_STRTAB] + tpnt->loadaddr);

  for(i=0; i< rel_size; i++, rpnt++){
    reloc_addr = (unsigned int *) (tpnt->loadaddr + (int)rpnt->r_offset);
    reloc_type = ELF32_R_TYPE(rpnt->r_info);
    symtab_index = ELF32_R_SYM(rpnt->r_info);

    /* When the dynamic linker bootstrapped itself, it resolved some symbols.
       Make sure we do not do them again */
    if(!symtab_index && tpnt->libtype == program_interpreter) continue;
    if(symtab_index && tpnt->libtype == program_interpreter &&
       _dl_symbol(strtab + symtab[symtab_index].st_name))
      continue;
    switch(reloc_type){
    case R_ARM_NONE: break;
    case R_ARM_JUMP_SLOT:
      {
      unsigned int *client_reloc_addr;

	/* Convert relocation addr from public GOT to private GOT. */
	client_reloc_addr = (unsigned int *)(((unsigned int)reloc_addr -
		(unsigned int)objinfo->public_rw_ptr) + (unsigned int)objinfo->private_rw_ptr);
	*client_reloc_addr = *reloc_addr + (unsigned int)tpnt->loadaddr;
//        *reloc_addr += (unsigned int) tpnt->loadaddr;
        break;
      }
    default:
      _dl_fdprintf(2, "%s: (LAZY) can't handle reloc type ", _dl_progname);
#ifdef VERBOSE_DLINKER
      _dl_fdprintf(2, "%s", _dl_reltypes[reloc_type]);
#endif
      if(symtab_index) _dl_fdprintf(2, "'%s'\n",
				  strtab + symtab[symtab_index].st_name);
      _dl_exit(1);
    }
  }
}

int _dl_parse_relocation_information(struct elf_resolve * tpnt, int rel_addr,
       int rel_size, int type){
  int i;
  char * strtab;
  int reloc_type;
  int goof = 0;
  Elf32_Sym * symtab;
  Elf32_Rel * rpnt;
  unsigned int * reloc_addr;
  unsigned int symbol_addr;
  int symtab_index;
  unsigned int *client_reloc_addr,symbol_offset;
  struct som_rt_elem *objinfo;
  unsigned int *got;

  got = (unsigned int *)(tpnt->dynamic_info[DT_PLTGOT] + tpnt->loadaddr);

  objinfo = *((struct som_rt_elem **)got[2]) + got[1];

  /* Now parse the relocation information */

  rpnt = (Elf32_Rel *) (rel_addr + tpnt->loadaddr);
  rel_size = rel_size / sizeof(Elf32_Rel);

  symtab =  (Elf32_Sym *) (tpnt->dynamic_info[DT_SYMTAB] + tpnt->loadaddr);
  strtab = ( char *) (tpnt->dynamic_info[DT_STRTAB] + tpnt->loadaddr);

  for(i=0; i< rel_size; i++, rpnt++)
  {
    reloc_addr = (unsigned int *) (tpnt->loadaddr + (int)rpnt->r_offset);
    reloc_type = ELF32_R_TYPE(rpnt->r_info);
    symtab_index = ELF32_R_SYM(rpnt->r_info);
    symbol_addr = 0;


    if(!symtab_index && tpnt->libtype == program_interpreter) continue;

    if(symtab_index) {

      if(tpnt->libtype == program_interpreter &&
	 _dl_symbol(strtab + symtab[symtab_index].st_name))
	continue;

      symbol_addr = (unsigned int)
	_dl_find_hash(strtab + symtab[symtab_index].st_name,
			      tpnt->symbol_scope, 0/*(int) reloc_addr*/,
		      (reloc_type == R_ARM_JUMP_SLOT ? tpnt : NULL), 0);

      /*
       * We want to allow undefined references to weak symbols - this might
       * have been intentional.  We should not be linking local symbols
       * here, so all bases should be covered.
       */
      if(!symbol_addr &&
	 ELF32_ST_BIND(symtab[symtab_index].st_info) == STB_GLOBAL) {
	_dl_fdprintf(2, "%s: can't resolve symbol '%s'\n",
		   _dl_progname, strtab + symtab[symtab_index].st_name);
	goof++;
      }
    }

    switch(reloc_type){
    case R_ARM_NONE:
      break;
/*    case R_386_32:
      *reloc_addr += symbol_addr;
      break;
    case R_386_PC32:
      *reloc_addr += symbol_addr - (unsigned int) reloc_addr;
      break;*/
    case R_ARM_GLOB_DAT:
    case R_ARM_ABS32:
	if ((unsigned int)reloc_addr < (unsigned int)objinfo->public_rw_ptr)
	{ /* Address of relocation is in text segment */
	  _dl_fdprintf(2,"%s: Text relocation of data symbol '%s' found:\r\n %s (offset 0x%X)\r\n",
		_dl_progname, strtab + symtab[symtab_index].st_name, tpnt->libname,
		(unsigned int)reloc_addr - (unsigned int)tpnt->loadaddr);

	  /* _dl_exit() would deregister the library from the client, but it would
	   * remain in memory until expiry and block attempts to replace it without
	   * reinitialising SOManager.
	   */
	  _dl_deregister_lib((unsigned int)tpnt->loadaddr); /* Deregister from client. */
	  _dl_deregister_lib((unsigned int)tpnt->loadaddr); /* Deregister from global list - clean up. */

	  _dl_exit(1);
	}

	client_reloc_addr = (unsigned int *)(((unsigned int)reloc_addr -
		(unsigned int)objinfo->public_rw_ptr) + (unsigned int)objinfo->private_rw_ptr);

	symbol_offset = symbol_addr - (unsigned int)objinfo->public_rw_ptr;

	/*
	 * If the symbol exists in this library's R/W segment, then relocate to client's
	 * copy.
	 */
	if (symbol_offset >= 0 && symbol_offset < objinfo->rw_size)
	  symbol_addr = (unsigned int)objinfo->private_rw_ptr + symbol_offset;
	else if ((char *)symbol_addr < tpnt->loadaddr || (char *)symbol_addr >= objinfo->public_rw_ptr + objinfo->rw_size)
	{
	  /*
	   * Check if the symbol exists in another library's R/W segment and relocate if so
	   */
	unsigned int *got = _dl_got_from_addr((void *)symbol_addr);
	struct som_rt_elem *dst_obj = *((struct som_rt_elem **)got[2]) + got[1];

	  symbol_offset = symbol_addr - (unsigned int)dst_obj->public_rw_ptr;
	  if (symbol_offset >= 0 && symbol_offset < dst_obj->rw_size)
	    symbol_addr = (unsigned int)dst_obj->private_rw_ptr + symbol_offset;
	}

	*client_reloc_addr += symbol_addr;

      break;
    case R_ARM_JUMP_SLOT:
      /* These relocations are used when LD_BIND_NOW is given in the environment */
      /* For the executable, reloc_addr already points to the right place, otherwise
         relocate it to the library's private GOT */
      if (tpnt->libtype == elf_executable)
        client_reloc_addr = reloc_addr;
      else
      {
        client_reloc_addr = (unsigned int *)(((unsigned int)reloc_addr -
		(unsigned int)objinfo->public_rw_ptr) + (unsigned int)objinfo->private_rw_ptr);
      }
      /* symbol_addr does not need any manipulation - it already points to the correct
         function */

      *client_reloc_addr = symbol_addr;
      break;
    case R_ARM_RELATIVE:
	if ((unsigned int)reloc_addr < (unsigned int)objinfo->public_rw_ptr)
	{
	  /* Address of relocation is in text segment.  */
	  _dl_fdprintf(2,"%s: Text relocation of data symbol '%s' found:\r\n %s (offset 0x%X)\n",
		_dl_progname,strtab + symtab[symtab_index].st_name,
		tpnt->libname,(unsigned int)reloc_addr - (unsigned int)tpnt->loadaddr);

	  /* _dl_exit() would deregister the library from the client, but it would
	   * remain in memory until expiry and block attempts to replace it without
	   * reinitialising SOManager.
	   */
	  _dl_deregister_lib((unsigned int)tpnt->loadaddr); /* Deregister from client. */
	  _dl_deregister_lib((unsigned int)tpnt->loadaddr); /* Deregister from global list - clean up. */

	  _dl_exit(1);
	}

	/*
	 * Alter relocation to be in the client's private GOT instead
	 * of the library's public GOT
	 *
	 * Convert reloc_addr to an offset from start of library public R/W segment and then
	 * as an absolute address in client's private R/W segment.
	 */
	client_reloc_addr = (unsigned int *)(((unsigned int)reloc_addr -
		(unsigned int)objinfo->public_rw_ptr) + (unsigned int)objinfo->private_rw_ptr);

	if (*reloc_addr < (objinfo->public_rw_ptr - tpnt->loadaddr))
	{
	  /*
	   * If the relocated address is in the R/O segment, then keep it there - don't
	   * relocate in to the client's private R/W segment
	   */
	  *client_reloc_addr = (unsigned int)tpnt->loadaddr + *reloc_addr;
	}
	else
	{
	unsigned int client_reloc;
	  /*
	   * (*reloc_addr) is an offset relative to the library's start address
	   * Convert (*reloc_addr) to absolute address in library and then to offset relative to
	   * library's public R/W segment. Finally, use this offset to find absolute address in
	   * client's private R/W segment.
	   */
	  client_reloc = (((*reloc_addr) + (unsigned int)tpnt->loadaddr) -
		(unsigned int)objinfo->public_rw_ptr) + (unsigned int)objinfo->private_rw_ptr;
	  *client_reloc_addr = client_reloc;
	}
      break;
    case R_ARM_COPY:
#if 0  /* Do this later */
      _dl_fdprintf(2, "Doing copy for symbol ");
      if(symtab_index) _dl_fdprintf(2, strtab + symtab[symtab_index].st_name);
      _dl_fdprintf(2, "\n");
      _dl_memcpy((void *) symtab[symtab_index].st_value,
		 (void *) symbol_addr,
		 symtab[symtab_index].st_size);
#endif
      break;
    default:
      _dl_fdprintf(2, "%s: can't handle reloc type ", _dl_progname);
#ifdef VERBOSE_DLINKER
      _dl_fdprintf(2, "%s ", _dl_reltypes[reloc_type]);
#endif
      if (symtab_index)
	_dl_fdprintf(2, "'%s'\n", strtab + symtab[symtab_index].st_name);
      _dl_exit(1);
    }
  }

  return goof;
}


/* This is done as a separate step, because there are cases where
   information is first copied and later initialized.  This results in
   the wrong information being copied.  Someone at Sun was complaining about
   a bug in the handling of _COPY by SVr4, and this may in fact be what he
   was talking about.  Sigh. */

/* No, there are cases where the SVr4 linker fails to emit COPY relocs
   at all */

#ifndef BROKEN_LINKER
int _dl_parse_copy_information(struct dyn_elf * xpnt, int rel_addr,
       int rel_size, int type){
  int i;
  char * strtab;
  int reloc_type;
  int goof = 0;
  Elf32_Sym * symtab;
  Elf32_Rel * rpnt;
  unsigned int * reloc_addr;
  unsigned int symbol_addr;
  struct elf_resolve *tpnt;
  int symtab_index;
  /* Now parse the relocation information */

  tpnt = xpnt->dyn;

  rpnt = (Elf32_Rel *) (rel_addr + tpnt->loadaddr);
  rel_size = rel_size / sizeof(Elf32_Rel);

  symtab =  (Elf32_Sym *) (tpnt->dynamic_info[DT_SYMTAB] + tpnt->loadaddr);
  strtab = ( char *) (tpnt->dynamic_info[DT_STRTAB] + tpnt->loadaddr);

  for(i=0; i< rel_size; i++, rpnt++)
  {
  struct elf_resolve *lib;

    reloc_addr = (unsigned int *) (tpnt->loadaddr + (int)rpnt->r_offset);
    reloc_type = ELF32_R_TYPE(rpnt->r_info);
    if(reloc_type != R_ARM_COPY) continue;
    symtab_index = ELF32_R_SYM(rpnt->r_info);
    symbol_addr = 0;
    if(!symtab_index && tpnt->libtype == program_interpreter) continue;
    if(symtab_index)
    {
      if(tpnt->libtype == program_interpreter &&
	 _dl_symbol(strtab + symtab[symtab_index].st_name))
	continue;

      symbol_addr = (unsigned int)
	_dl_find_hash(strtab + symtab[symtab_index].st_name,
			      xpnt->next, &lib, NULL, 1);

      if(!symbol_addr || !lib)
      {
	_dl_fdprintf(2, "%s: can't resolve symbol '%s'\n",
		   _dl_progname, strtab + symtab[symtab_index].st_name);
	goof++;
      }
      else
      {
      unsigned int *got = (unsigned int *)(lib->dynamic_info[DT_PLTGOT] + lib->loadaddr);
      struct som_rt_elem *objinfo = *((struct som_rt_elem **)got[2]) + got[1];

        if ((unsigned int)symbol_addr >= (unsigned int)objinfo->public_rw_ptr)
	{
	  /*
	   * If the symbol to be copied is within the R/W data segment of the library, then
	   * relocate its address to point to the one in the client's private R/W segment
	   * (which has already been initialised). The symbol may be in the text segment of
	   * the library, for example, if it's a constant string that is referenced in the
	   * client code. In this case, don't relocate.
	   */
	  symbol_addr = (symbol_addr - (unsigned int)objinfo->public_rw_ptr) + (unsigned int)objinfo->private_rw_ptr;
	}

        _dl_memcpy((char *) symtab[symtab_index].st_value,(char *) symbol_addr,
		 symtab[symtab_index].st_size);
      }
    }
  }
  return goof;
}
#endif
