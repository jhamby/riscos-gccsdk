/*
 * ANSI Standard: 4.2 Diagnostics <assert.h>.
 * Copyright (c) 2000-2006 GCCSDK Developers
 */

/* It is possible to turn assertions off and on in different functions within
   a translation unit by definining (or undefining) NDEBUG and including
   <assert.h> again.  */

#ifndef __ASSERT_H
#define __ASSERT_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

extern void assert (int) __THROW __attribute__ ((__noreturn__));

/* __assert2 is also used by Norcroft C compiler.  */
extern void __assert2 (const char *__message,
		       const char *__function,
		       const char *__file,
		       int __line)
     __THROW __attribute__ ((__noreturn__));

__END_DECLS

#else
# undef assert
# undef __ASSERT_CAST
#endif /* __ASSERT_H */

#if defined __cplusplus && defined __GNUC__
#define __ASSERT_CAST static_cast<void>
#else
#define __ASSERT_CAST (void)
#endif

#ifdef NDEBUG
# define assert(x) (__ASSERT_CAST (0))
#else
# ifdef __GNUC__
/* If compiled under GCC, we can also output the function name.  */
#  define assert(expr) \
    (__ASSERT_CAST ((expr) || (__assert2 (#expr, __PRETTY_FUNCTION__, __FILE__, __LINE__), 0)))
# else

#  ifdef __STDC__
/* __func__ is defined by Norcroft C.  */
#   define assert(expr) \
     (__ASSERT_CAST ((expr) || (__assert2 (#expr, __func__, __FILE__, __LINE__), 0)))
#  else
#   define assert(expr) \
     (__ASSERT_CAST ((expr) || (__assert2 ("expr", NULL, __FILE__, __LINE__), 0)))
#  endif

# endif /* __GNUC__ */

#endif /* NDEBUG */
