/* som_utilswis.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_UTILSWIS_H
#define SOM_UTILSWIS_H

#include <kernel.h>
#include "som.h"

/* Retrieve information about the library whose handle is
   given. The data is placed in a user supplied buffer.  */
extern _kernel_oserror *som_query_object (som_handle handle,
					  som_objinfo *buffer,
					  unsigned int flags);

/* SWI "SOM_IterateObjects"
 *
 * Allow a client's objects to be found.
 * entry:
 *  r0 = Reason code:
 *	0 = return first object in client list
 *	1 = return next object in list after that given in r1
 *	2 = return previous object in list before that given in r1
 *	3 = return last object in client list
 *  r1 = for r0 = 1 or r0 = 2 - internal handle as returned in r1
 * exit:
 *  r0 = handle of requested object (NULL if there is no object to return)
 *  r1 = internal handle of requested object for use as input in subsequent
 *	 calls
 *	 (NULL if there is no object to return)
 *
 * (Note: internal handle is actually address of som_object structure)
 */
extern _kernel_oserror *som_iterate_objects (_kernel_swi_regs *regs);

/* SWI "SOM_GOTFromAddr"
 *
 * Given an address return the client's private GOT of the library that
 * contains it.
 * entry:
 *  r0 = address
 * exit:
 *  r0 = ptr to GOT or 0 if failed
 */
extern _kernel_oserror *som_got_from_addr (_kernel_swi_regs *regs);

/* SWI "SOM_HandleFromAddr"
 *
 * Given an address return the handle of the library that contains it.
 * entry:
 *  r0 = address
 * exit:
 *  r0 = handle
 */
extern _kernel_oserror *som_handle_from_addr (_kernel_swi_regs *regs);

/* SWI "SOM_HandleFromName"
 *
 * Given ptr to library name, return handle of library from global list.
 * entry:
 *  r0 = ptr to name (Read Only)
 * exit:
 *  r0 = handle or 0 for failure
 */
extern som_handle som_handle_from_name (const char *name);

/* SWI "SOM_Reloc"
 *
 * Given an address, determine if it exists within a library's R/W data
 * segment. If this proves to be the case, then convert to the client's
 * private copy and return as the result, otherwise, return what was given.
 *
 * entry:
 *  r0 = address to relocate
 * exit:
 *  r0 = address relocated if possible, otherwise preserved.
 */
extern void som_reloc (_kernel_swi_regs *regs);

#endif
