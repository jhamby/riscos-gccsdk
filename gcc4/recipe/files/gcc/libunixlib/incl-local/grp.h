/* Internal UnixLib grp.h
 * Copyright (c) 2002-2008 UnixLib Developers
 */

#ifndef __GRP_H
#include_next <grp.h>
#endif

#if !defined(__INTERNAL_GRP_H) && defined(__GRP_H)
#define __INTERNAL_GRP_H

__BEGIN_DECLS

/* UnixLib group implementation function.  */
extern struct group *__grpread (FILE *__stream, struct group *__grp,
				char *__buf, size_t __buflen)
     __nonnull ((1, 2, 3)) __wur;

__END_DECLS

#endif
