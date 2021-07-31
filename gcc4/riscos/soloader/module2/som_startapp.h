/* som_startapp.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_STARTAPP_H
#define SOM_STARTAPP_H

#include "som.h"

/* This is a low level function written in ARM code. It's job is to clean
 * up the SVC stack by flattening it, change to USR mode, set up frame and
 * stack registers and then enter either the user program or the dynamic
 * loader as defined by the entry point.
 * It never returns.
 */
extern void som_start_app (som_PTR entry, unsigned int sp, int stack_size)
	__attribute__ ((noreturn));

#endif
