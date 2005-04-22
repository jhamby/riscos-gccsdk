/* Generic system definitions to improve compiler/source compatibility.  */

/* Copyright (C) 1992,93,94,95,96,97,98,99,
   2000,2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef	__SYS_CDEFS_H
#define	__SYS_CDEFS_H	1

/* We are almost always included from features.h. */
#ifndef __UNIXLIB_FEATURES_H
# include <features.h>
#endif

#ifdef __GNUC__

/* GCC can always grok prototypes.  For C++ programs we add throw()
   to help it optimize the function calls.  But this works only with
   gcc 2.8.x and egcs.  For gcc 3.2 and up we even mark C functions
   as non-throwing using a function attribute since programs can use
   the -fexceptions options for C code as well.

   If compiling UnixLib with a C++ compiler, we disable the option
   as it breaks compatibility between function prototype and definition.
   UnixLib generally isn't compiled with a C++ compiler but the additional
   type checking can be useful in finding small errors.  */
# if !defined __cplusplus && __GNUC_PREREQ (3, 3)
#  define __THROW       __attribute__ ((__nothrow__))
#  define __NTH(fct)    __attribute__ ((__nothrow__)) fct
# else
#  if defined __cplusplus && ! defined (__UNIXLIB_INTERNALS)
#   define __THROW      throw ()
#   define __NTH(fct)   fct throw ()
#  else
#   define __THROW
#   define __NTH(fct)   fct
#  endif
# endif

#elif defined(__CC_NORCROFT)
/* The Norcroft compiler supports the __inline keyword.  Allow it
   to support the GCC variant.  */
# define __inline__ __inline

# define __const const
# define __signed signed
# define __volatile volatile
# define __THROW
# define __NTH(fct) fct

#elif defined(__LCC__)
# define __inline__		/* No inline functions.  */
# define __inline               /* No inline functions.  */
# define __THROW
# define __NTH(fct)	fct
# define __const        const
# define __signed       signed
# define __volatile     volatile

#else /* Unknown compiler.  */
#error "Unrecognised/unsupported system compiler."
#endif

/* Some user header file might have defined this before.  */
#undef	__P
#undef	__PMT
#define __P(args)      args
#define __PMT(args)    args

/* For these things, GCC behaves the ANSI way normally,
   and the non-ANSI way under -traditional.  */
#define __CONCAT(x,y)	x ## y
#define __STRING(x)	#x

/* This is not a typedef so `const __ptr_t' does the right thing.  */
#define __ptr_t void *
#define __long_double_t  long double


/* C++ needs to know that types and declarations are C, not C++.  */
#ifdef	__cplusplus
# define __BEGIN_DECLS	extern "C" {
# define __END_DECLS	}
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif


/* The standard library needs the functions from the ISO C90 standard
   in the std namespace.  At the same time we want to be safe for
   future changes and we include the ISO C99 code in the non-standard
   namespace __c99.  The C++ wrapper header take case of adding the
   definitions to the global namespace.  */
#if defined __cplusplus && defined _GLIBCPP_USE_NAMESPACES
# define __BEGIN_NAMESPACE_STD	namespace std {
# define __END_NAMESPACE_STD	}
# define __USING_NAMESPACE_STD(name) using std::name;
# define __BEGIN_NAMESPACE_C99	namespace __c99 {
# define __END_NAMESPACE_C99	}
# define __USING_NAMESPACE_C99(name) using __c99::name;
#else
/* For compatibility we do not add the declarations into any
   namespace.  They will end up in the global namespace which is what
   old code expects.  */
# define __BEGIN_NAMESPACE_STD
# define __END_NAMESPACE_STD
# define __USING_NAMESPACE_STD(name)
# define __BEGIN_NAMESPACE_C99
# define __END_NAMESPACE_C99
# define __USING_NAMESPACE_C99(name)
#endif


/* Support for bounded pointers.  */
#ifndef __BOUNDED_POINTERS__
# define __bounded	/* nothing */
# define __unbounded	/* nothing */
# define __ptrvalue	/* nothing */
#endif


/* Support for flexible arrays.  */
#if __GNUC__
# define __flexarr	[]
#else /* ! __GNUC__ */
# if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#  define __flexarr	[]
# else
/* Some other non-C99 compiler.  Approximate with [1].  */
#  define __flexarr	[1]
# endif
#endif /* __GNUC__ */

/* GCC has various useful declarations that can be made with the
   `__attribute__' syntax.  All of the ways we use this do fine if
   they are omitted for compilers that don't understand it. */
#ifndef __GNUC__
#define __attribute__(xyz)	/* Ignore */
#endif

/* At some point during the gcc 2.96 development the `malloc' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if __GNUC_PREREQ (2,96)
# define __attribute_malloc__ __attribute__ ((__malloc__))
#else
# define __attribute_malloc__ /* Ignore */
#endif

/* At some point during the gcc 2.96 development the `pure' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if __GNUC_PREREQ (2,96)
# define __attribute_pure__ __attribute__ ((__pure__))
#else
# define __attribute_pure__ /* Ignore */
#endif

/* At some point during the gcc 3.1 development the `used' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if __GNUC_PREREQ (3,1)
# define __attribute_used__ __attribute__ ((__used__))
# define __attribute_noinline__ __attribute__ ((__noinline__))
#else
# define __attribute_used__ __attribute__ ((__unused__))
# define __attribute_noinline__ /* Ignore */
#endif

/* gcc allows marking deprecated functions.  */
#if __GNUC_PREREQ (3,2)
# define __attribute_deprecated__ __attribute__ ((__deprecated__))
#else
# define __attribute_deprecated__ /* Ignore */
#endif

/* At some point during the gcc 2.8 development the `format_arg' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.
   If several `format_arg' attributes are given for the same function, in
   gcc-3.0 and older, all but the last one are ignored.  In newer gccs,
   all designated arguments are considered.  */
#if __GNUC_PREREQ (2,8)
# define __attribute_format_arg__(x) __attribute__ ((__format_arg__ (x)))
#else
# define __attribute_format_arg__(x) /* Ignore */
#endif

/* At some point during the gcc 2.97 development the `strfmon' format
   attribute for functions was introduced.  We don't want to use it
   unconditionally (although this would be possible) since it
   generates warnings.  */
#if __GNUC_PREREQ (2,97)
# define __attribute_format_strfmon__(a,b) \
  __attribute__ ((__format__ (__strfmon__, a, b)))
#else
# define __attribute_format_strfmon__(a,b) /* Ignore */
#endif

/* The nonull function attribute allows to mark pointer parameters which
   must not be NULL.  */
#if __GNUC_PREREQ (3,3)
# define __nonnull(params) __attribute__ ((__nonnull__ params))
#else
# define __nonnull(params)
#endif

/* If fortification mode, we warn about unused results of certain
   function calls which can lead to problems.  */
#if __GNUC_PREREQ (3,4)
# define __attribute_warn_unused_result__ \
   __attribute__ ((__warn_unused_result__))
# define __wur __attribute_warn_unused_result__
#else
# define __attribute_warn_unused_result__ /* empty */
#endif
#ifndef __wur
# define __wur /* Ignore */
#endif


/* It is possible to compile containing GCC extensions even if GCC is
   run in pedantic mode if the uses are carefully marked using the
   `__extension__' keyword.  But this is not generally available before
   version 2.8.  */
#if !__GNUC_PREREQ (2,8)
# define __extension__		/* Ignore */
#endif

/* __restrict is known in EGCS 1.2 and above. */
#if !__GNUC_PREREQ (2,92)
# define __restrict	/* Ignore */
#endif

/* ISO C99 also allows to declare arrays as non-overlapping.  The syntax is
     array_name[restrict]
   GCC 3.1 supports this.  */
#if __GNUC_PREREQ (3,1) && !defined __GNUG__
# define __restrict_arr	__restrict
#else
# ifdef __GNUC__
#  define __restrict_arr	/* Not supported in old GCC.  */
# else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define __restrict_arr	restrict
#  else
/* Some other non-C99 compiler.  */
#   define __restrict_arr	/* Not supported.  */
#  endif
# endif
#endif

#ifdef __UNIXLIB_INTERNALS

#if defined (__CC_NORCROFT) || defined (__LCC__)
/* The Norcroft and LCC compilers do not have support for any of these
   symbol modifiers.  To retain source code compatibility, we just
   define them to evaluate to nothing.  */

#define stong_alias(name, aliasname) /**/
#define _strong_alias(name, aliasname) /**/
#define weak_function /**/
#define weak_const_function /**/
#define weak_alias(name, aliasname) /**/
#define _weak_alias(name, aliasname) /**/
#define hidden_def(name) /**/
#define libm_hidden_def(name) /**/
#define INTDEF(name) /**/

#endif /* __CC_NORCROFT || __LCC__ */

#ifdef __GNUC__
/* The GCC compiler for RISC OS comes in two variants, an AOF compiler
   and an ELF compiler.  */

/* Both AOF and ELF variants support the aliasing of symbols.  */
#define strong_alias(name, aliasname) _strong_alias(name, aliasname)
#define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));

#ifdef __ELF__
/* The GCC/ELF compiler has support for everything.  */

/* This comes between the return type and function name in
   a function definition to make that definition weak.  */
# define weak_function __attribute__ ((weak))
# define weak_const_function __attribute__ ((weak, __const__))

#define weak_alias(name, aliasname) _weak_alias(name, aliasname)
#define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));

/* For the future, we may want to support ELF symbol visibility which will
   help to reduce the symbol tables of dynamic libraries and speed up
   dynamic linking.  */
#define hidden_def(name) /**/
#define libm_hidden_def(name) /**/
#define INTDEF(name) /**/

#else /* ! __ELF__ */

/* The GCC/AOF compiler does have a concept of weak symbols, but their
   application is a little more restricted and harder to work with
   naturally in the compiler.  We therefore only support weak symbols
   in hand-crafted assembler.  */

# define weak_function /**/
# define weak_const_function /**/

/* This is more of a compatibility feature for AOF/GCC so that we can build
   the same source files as ELF/GCC.  */
#define weak_alias(name, aliasname) strong_alias(name, aliasname)

#define hidden_def(name) /**/
#define libm_hidden_def(name) /**/
#define INTDEF(name) /**/

#endif /* ! __ELF__ */

#endif /* __GNUC__ */

/* On some platforms we can make internal function calls (i.e., calls of
   functions not exported) a bit faster by using a different calling
   convention.  */
#ifndef internal_function
# define internal_function	/* empty */
#endif

#endif /* __UNIXLIB_INTERNALS */


#endif	 /* sys/cdefs.h */
