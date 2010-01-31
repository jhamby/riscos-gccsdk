/* Internal UnixLib setjmp.h
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __SETJMP_H
#include_next <setjmp.h>
#endif

#if !defined(__INTERNAL_SETJMP_H) && defined(__SETJMP_H)
#define __INTERNAL_SETJMP_H

__BEGIN_DECLS

/* Internal functions.  */
extern void __sigsetjmp_helper (sigjmp_buf __state, int __savesigs);
extern void __siglongjmp_helper (const sigjmp_buf __state, int __savesigs);

__END_DECLS

#endif
