/* som_symlinks.h
 *
 * Copyright 2007 GCCSDK Developers
 */

#ifndef SOM_SYMLINKS_H
#define SOM_SYMLINKS_H

#include <kernel.h>

/* Resolve a (potential) chain of symlinks.
 *
 * A symlink is a small file of type 0x1C8 which consists of a 4 byte ID word
 * LINK, followed by a 4 byte word giving the length of the following
 * filename. If the filename contains a colon, it is assumed to have an
 * absolute path and completely replaces the symlink filename. Otherwise it
 * is assumed to be relative to the symlink and is added to the symlink's
 * path. The process is repeated on the result until the filename is not a
 * symlink at which point it is returned to the caller.
 *
 * The resulting filename is passed back in a block of memory allocated by
 * SWI "SOM_Alloc" and can be freed with SWI "SOM_Free" when finished with.
 */
extern _kernel_oserror *som_resolve_links (const char *file_in,
					   char **filename_ret);

#endif
