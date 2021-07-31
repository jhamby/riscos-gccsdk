/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __ALLOCA_H
#define __ALLOCA_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_size_t
#include <stddef.h>

__BEGIN_DECLS

#undef alloca

/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca (size_t __size);

#ifdef __GNUC__
#define alloca(size) __builtin_alloca (size)
#endif

__END_DECLS

#endif
