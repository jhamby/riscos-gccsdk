/* som_runcom.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by John Tytgat
 */

#ifndef SOM_RUNCOM_H
#define SOM_RUNCOM_H

#include <kernel.h>

/* Examines the given ELF executable to determine whether it is dynamically
   or statically linked, and runs it accordingly.  */
extern _kernel_oserror *command_run (const char *arg_string, int argc);

#endif
