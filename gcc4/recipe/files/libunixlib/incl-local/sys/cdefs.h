/* Internal UnixLib sys/cdefs.h
 * Copyright (c) 2008-2010 UnixLib Developers
 */

#ifndef _SYS_CDEFS_H
#include_next <sys/cdefs.h>
#endif

#if !defined(__INTERNAL_SYS_CDEFS_H) && defined(_SYS_CDEFS_H)
#define	__INTERNAL_SYS_CDEFS_H

__BEGIN_DECLS

#define strong_alias(name, aliasname) _strong_alias(name, aliasname)
#define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));

/* This comes between the return type and function name in
   a function definition to make that definition weak.  */
#define weak_function __attribute__ ((weak))
#define weak_const_function __attribute__ ((weak, __const__))

#define weak_alias(name, aliasname) _weak_alias(name, aliasname)
#define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));

/* For the future, we may want to support ELF symbol visibility which will
   help to reduce the symbol tables of dynamic libraries and speed up
   dynamic linking. FIXME */
#define attribute_hidden __attribute__ ((visibility ("hidden")))
#define hidden_def(name) __asm__(".hidden "#name);
#define libm_hidden_def(name) /**/
#define INTDEF(name) /**/
#define libc_hidden_def(name) hidden_def(name)

/* On some platforms we can make internal function calls (i.e., calls of
   functions not exported) a bit faster by using a different calling
   convention.  */
#ifndef internal_function
# define internal_function	/* empty */
#endif

__END_DECLS

#endif
