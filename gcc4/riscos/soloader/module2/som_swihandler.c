/* som_swihandler.c
 *
 * Copyright 2006-2017 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include "somanager.h"
#include "som.h"
#include "som_register.h"
#include "som_alloc.h"
#include "som_utilswis.h"
#include "som_symlinks.h"
#include "som_array.h"
#include "som_elf.h"
#include "som_map.h"

_kernel_oserror *
module_swihandler (int number, _kernel_swi_regs * r, void *pw)
{
  _kernel_oserror *err = NULL;

  switch (number)
    {
    case SOM_Alloc - SOM_00:
      /* On entry: r0 = size required.  */
      {
	void *block;

	if ((err = som_alloc (r->r[0], &block)) != NULL)
	  break;

	r->r[0] = (int) block;
      }
      break;

    case SOM_Free - SOM_00:
      /* On entry: r0 = block to free.  */
      err = som_free ((void *) r->r[0]);
      break;

    case SOM_AllocLib - SOM_00:
      /* On entry: r0 = size required.  */
      {
	void *block;

	if ((err = som_alloc_lib (r->r[0], &block)) != NULL)
	  break;

	r->r[0] = (int) block;
      }
      break;

    case SOM_RegisterObject - SOM_00:
      err = som_register_object (r);
      break;

    case SOM_DeregisterClient - SOM_00:
      err = som_deregister_client ();
      break;

    case SOM_DeregisterSharedObject - SOM_00:
      err = som_deregister_shared_object (r);
      break;

    case SOM_QueryObject - SOM_00:
      /* Retrieve information about the library whose handle is
       * given. The data is placed in a user supplied buffer.
       *
       * entry:
       *  r0 = handle of library
       *  r1 = pointer to buffer to return library information in
       *   r1 +  0 = base address
       *   r1 +  4 = pointer to library's read/write segment
       *   r1 +  8 = pointer to client copy of read/write segment (if
       *             applicable)
       *   r1 + 12 = size of read/write segment
       *   r1 + 16 = offset of GOT from start of read/write segment
       *   r1 + 20 = offset of bss area from start of read/write segment
       *   r1 + 24 = size of bss area
       *   r1 + 28 = pointer to name (Read only)
       *   r1 + 32 = flags
       *  r2 = flags
       *   bit 0 - set to search current client object list
       *           clear to search global object list
       * exit:
       *  All registers preserved if object found and no error, or
       *  r0 = pointer to error block
       */
      err = som_query_object ((som_handle) r->r[0], (som_objinfo *) r->r[1],
			      (unsigned int) r->r[2]);
      break;

    case SOM_IterateObjects - SOM_00:
      err = som_iterate_objects (r);
      break;

    case SOM_GOTFromAddr - SOM_00:
      err = som_got_from_addr (r);
      break;

    case SOM_HandleFromAddr - SOM_00:
      err = som_handle_from_addr (r);
      break;

    case SOM_HandleFromName - SOM_00:
      /* SWI "SOM_HandleFromName"
       *
       * Given ptr to library name, return handle of library from global
       * list.
       * entry:
       *  r0 = ptr to name (Read Only)
       * exit:
       *  r0 = handle or 0 for failure
       */
      r->r[0] = som_handle_from_name ((const char *) r->r[0]);
      break;

    case SOM_ResolveSymlinks - SOM_00:
      {
	/* Return allocated buffer containing resolved filename.
	 * Result can be freed with SWI "SOM_Free"
	 * entry:
	 *  r0 = ptr to symlink filename
	 * exit:
	 *  If error, r0 = ptr to error block otherwise r0 = ptr to
	 *  allocated buffer.
	 */
	char *filename_ret;

	if ((err = som_resolve_links ((const char *) r->r[0],
				      &filename_ret)) == NULL)
	  r->r[0] = (int) filename_ret;
      }
      break;

    case SOM_GenerateRuntimeArray - SOM_00:
      err = som_generate_runtime_array ();
      break;

    case SOM_Reloc - SOM_00:
      som_reloc (r);
      break;

    case SOM_ELFFile - SOM_00:
      {
	/* r0 = reason
	 *   0 - Open file
	 *     Entry:
	 *      R1 = filename
	 *     Exit:
	 *      R0 = handle or pointer to error block
	 *   1 - Load file
	 *     Entry:
	 *       R1 = handle
	 *     Exit:
	 *       R0 = NULL or pointer to error block
	 *   2 - Close file
	 *     Entry:
	 *       R1 = handle
	 *   3 - Alloc segments
	 *     Entry:
	 *       R1 = handle
	 *   4 - Return ABI string
	 *     Entry:
	 *       R1 = handle
	 *     Exit:
	 *       R0 = Pointer to ABI string or pointer to error block
	 *   5 - Return TRUE if ABI is armeabihf
	 *     Entry:
	 *       R1 = handle
	 *     Exit:
	 *       R0 = 1 if armeabihf, 0 if abi-2.0 or pointer to error block
	 *   6 - Return public info from the open ELF file
	 *     Entry:
	 *       R1 = handle
	 *       R2 = pointer to som_object_info struct
	 *     Exit:
	 *       block in R2 filled with public info (name only valid while file open)
	 *       R0 = 0, or pointer to error block
	 */
	switch (r->r[0])
	{
	  case reason_code_SOM_ELFFILE_OPEN:
	    {
	      elf_file *file;
	      if ((err = som_alloc (sizeof (elf_file), (void **) &file)) != NULL)
		{
		  err = somerr_no_memory;
		  break;
		}

	      elffile_init(file);
	      if ((err = elffile_open ((const char *)r->r[1], file)) == NULL)
		r->r[0] = (int)file;
	    }
	    break;
	  case reason_code_SOM_ELFFILE_LOAD:
	    {
	      elf_file *file = (elf_file *)r->r[1];

	      if (!file)
		return somerr_bad_param;

	      err = elffile_load (file, /*init_bss=*/true);
	    }
	    break;
	  case reason_code_SOM_ELFFILE_CLOSE:
	    {
	      elf_file *file = (elf_file *)r->r[1];
	      if (file)
		{
		  elffile_close (file);
		  som_free (file);
		}
	    }
	    break;
	  case reason_code_SOM_ELFFILE_ALLOC:
	    {
	      elf_file *file = (elf_file *)r->r[1];

	      err = elffile_alloc_segments (file);
	    }
	    break;
	  case reason_code_SOM_ELFFILE_GET_ABI:
	    {
	      elf_file *file = (elf_file *)r->r[1];
	      const char *abi;

	      err = elffile_get_abi (file, &abi);
	      if (!err)
		r->r[0] = (int)abi;
	    }
	    break;
	  case reason_code_SOM_ELFFILE_IS_ARM_AAPCS:
	    {
	      elf_file *file = (elf_file *)r->r[1];
	      bool is_armeabihf;

	      err = elffile_is_armeabihf (file, &is_armeabihf);
	      if (!err)
		r->r[0] = is_armeabihf;
	    }
	    break;
	  case reason_code_SOM_ELFFILE_GET_PUBLIC_INFO:
	    {
	      elf_file *file = (elf_file *)r->r[1];
	      som_objinfo *info = (som_objinfo *)r->r[2];

	      err = elffile_get_public_info (file, info);
	    }
	    break;
	}
      }
      break;
    case SOM_Location - SOM_00:
      /*
       * Entry:
       *  r0 = address
       * Exit:
       *  r0 = pointer to library name or NULL
       *  r1 = offset in to library or 0
       */
      som_location((som_PTR)r->r[0], (const char **)&r->r[0], (unsigned *)&r->r[1]);
      break;
    default:
      err = error_BAD_SWI;
      break;
    }

  return err;
}
