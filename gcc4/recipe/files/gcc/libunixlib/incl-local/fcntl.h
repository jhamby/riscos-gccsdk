/* Internal UnixLib fcntl.h
 * Copyright (c) 2012 UnixLib Developers
 */

#ifndef __FCNTL_H
#include_next <fcntl.h>
#endif

#if !defined(__INTERNAL_FCNTL_H) && defined(__FCNTL_H)
#define __INTERNAL_FCNTL_H

__BEGIN_DECLS

/* Note bits 12 and 13 were once defined for O_BINARY and O_TEXT and no longer
   used.  */
#define O_PIPE		0x4000 /* UnixLib specific */
#define O_UNLINKED	0x8000 /* UnixLib specific - unlink file on close */

__END_DECLS

#endif

