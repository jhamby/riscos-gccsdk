/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/assert,v $
 * $Date: 1997/12/17 22:27:06 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

/* ANSI Standard: 4.2 Diagnostics <assert.h>.  */

/* It is possible to turn assertions off and on in different functions within
   a translation unit by definining (or undefining) NDEBUG and including
   <assert.h> again.  */

#ifndef __ASSERT_H
#define __ASSERT_H

#ifndef __GNUC__
#undef  __attribute__
#define __attribute__(x) /* Ignore */
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void assert (int) __attribute__ ((__noreturn__));

extern void __assert_fail (char *__message,
			   const char *__file,
			   int __line,
			   const char *__function) __attribute__ ((__noreturn__));

#ifdef __cplusplus
}
#endif

#else
#undef assert
#endif /* __ASSERT_H */

#ifdef NDEBUG
#define assert(x) ((void) 0)
#else
#ifdef __GNUC__
/* If compiled under GCC, we can also output the function name.  */
#define assert(expr) \
  ((void)((expr) ||  \
	  (__assert_fail (#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__), 0)))
#else

#ifdef __STDC__
#define assert(expr) \
  ((void)((expr) || (__assert_fail (#expr, __FILE__, __LINE__, NULL), 0)))
#else
#define assert(expr) \
  ((void)((expr) || (__assert_fail ("expr", __FILE__, __LINE__, NULL), 0)))
#endif

#endif /* __GNUC__ */

#endif /* NDEBUG */
