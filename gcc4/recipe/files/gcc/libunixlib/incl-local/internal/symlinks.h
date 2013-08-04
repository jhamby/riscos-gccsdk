/*
 * Copyright (c) 2009-2013 UnixLib Developers
 */

#ifndef __INTERNAL_SYMLINKS_H
#define __INTERNAL_SYMLINKS_H

#if __UNIXLIB_SYMLINKS

#include <features.h>

/* Offically allocated filetype.  */
#define	SYMLINK_FILETYPE	0x1C8

/* Arbitrary limit to prevent cyclical links. */
#define SYMLINK_MAX_LINKS	64

/* ASCII representation of 'LINK'.  */
#define SYMLINK_ID		0x4B4E494C
#define SYMLINK_ID_STR		"LINK"

/* Filenames passed and the symlinks itself are always RISC OS filenames.  */
int
__resolve_symlinks (const char *filename_in, char *filename_out,
		    size_t fn_out_size, int one_step_only);

#endif

#endif
