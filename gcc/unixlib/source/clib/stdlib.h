/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/stdlib.h,v $
 * $Date: 2004/04/15 22:21:02 $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

/* ANSI Standard 4.10: General Utilities <stdlib.h>.  */

#ifndef __STDLIB_H
#define __STDLIB_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#define __need_size_t
#define __need_wchar_t
#define __need_NULL
#include <stddef.h>

__BEGIN_DECLS

/* Returned by `div'.  */
typedef struct
  {
    int quot;	/* Quotient.  */
    int rem;	/* Remainder.  */
  } div_t;

/* Returned by `ldiv'.  */
typedef struct
  {
    long int quot;	/* Quotient.  */
    long int rem;	/* Remainder.  */
  } ldiv_t;

#ifdef __GNUC__
/* Returned by `lldiv'.  */
__extension__
typedef struct
  {
    long long int quot;	/* Quotient.  */
    long long int rem;	/* Remainder.  */
  } lldiv_t;
#endif

/* The largest number rand will return (same as INT_MAX).  */
#define	RAND_MAX	2147483647


/* Successful exit status.  */
#define EXIT_SUCCESS 0
/* Failing exit status.  */
#define EXIT_FAILURE 1

/* Maximum length of a multibyte character in the current locale.  */
#define	MB_CUR_MAX	1

/* Abort execution and generate a core-dump.  */
extern void abort (void) __attribute__ ((__noreturn__));

/* Register a function to be called when exit is called.  */
extern int atexit (void (*__atexit_function) (void));

/* Terminate the program with status. Call all functions registerd
   by atexit.  */
extern void exit (int __status) __attribute__ ((__noreturn__));

/* Terminate the program with status.  Don't call any functions
   registerd by atexit.  */
extern void _Exit (int __status) __attribute__ ((__noreturn__));

/* Return the value of environment variable 'name'.  */
extern char *getenv (const char *__name) __THROW;

/* Set NAME to VALUE in the environment.
   If REPLACE is nonzero, overwrite an existing value.  */
extern int setenv (const char *__name, const char *__value, int __replace)
     __THROW;

/* Remove NAME from the environment.  */
extern void unsetenv (const char *__name) __THROW;

/* The `clearenv' was planned to be added to POSIX.1 but probably
   never made it.  Nevertheless the POSIX.9 standard (POSIX bindings
   for Fortran 77) requires this function.  */
extern int clearenv (void) __THROW;

/* Put string, which is of the form "NAME=VALUE" in the environment.  */
extern int putenv (const char *__string) __THROW;

/* Execute the given line via the CLI.  */
extern int system (const char *__command) __THROW;

/* Canonicalise a filename */
extern char *realpath (const char *__file_name, char *__resolved_name)
     __THROW;

#ifdef __UNIXLIB_INTERNALS

/* Definitions for the atexit array of functions that are to be
   called when the process calls exit.  ANSI requires a minimum
   of 32, however C++ requires 33.  */
#define __MAX_ATEXIT_FUNCTION_COUNT 33

extern void (*__atexit_function_array[__MAX_ATEXIT_FUNCTION_COUNT]) (void);
extern int __atexit_function_count;

#endif  /* __UNIXLIB_INTERNALS */

/* Allocate nmemb elements of size bytes each. Initialise
   all members to zero.  */
extern void *calloc (size_t __nmemb, size_t __size)
     __THROW  __attribute_malloc__;

/* Free a block allocated by malloc, calloc or realloc.  */
extern void free (void *__ptr) __THROW;

/* Allocate __size bytes of memory.  */
extern void *malloc (size_t __size) __THROW __attribute_malloc__;

/* Re-allocate the previously malloc'd block, ptr, making the
   new block size bytes.  */
extern void *realloc (void *__ptr, size_t __size) __THROW __attribute_malloc__;

/* Allocate size bytes on a page boundary. The storage cannot be freed.  */
extern void *valloc (size_t __bytes) __THROW __attribute_malloc__;

/* src.c.alloc thinks these are in stdio.h, but that feels wrong ... */
extern void *memalign (size_t __alignment,
		       size_t __bytes) __THROW __attribute_malloc__;
extern void cfree (void *__mem) __THROW;
extern int malloc_trim (size_t) __THROW;

/* Return a random integer between 0 and 2^31 (System V interface).  */
extern int rand (void) __THROW;

/* Seed the random number generator with the given number.  */
extern void srand (long __seed) __THROW;

/* Return a random integer between 0 and RAND_MAX (BSD interface).  */
extern long int random (void) __THROW;

/* Seed the random number generator (BSD interface).  */
extern void srandom (unsigned int __seed) __THROW;

/* Initialize the random number generator to use state buffer STATEBUF,
   of length STATELEN, and seed it with SEED.  Optimal lengths are 8, 16,
   32, 64, 128 and 256, the bigger the better; values less than 8 will
   cause an error and values greater than 256 will be rounded down.  */
extern char *initstate (unsigned int __seed, char *__statebuf,
                        size_t __statelen) __THROW;

/* Switch the random number generator to state buffer STATEBUF,
   which should have been previously initialized by `initstate'.  */
extern char *setstate (char *__statebuf) __THROW;

/* Return the absolute value of x.  */
extern int abs (int __x) __THROW __attribute__ ((__const__));
extern long int	labs (long int __x) __THROW __attribute__ ((__const__));

/* Return numerator divided by denominator.  */
extern div_t div (int __numer, int __denom)
     __THROW __attribute__ ((__const__));
extern ldiv_t ldiv (long __numer, long __denom)
     __THROW __attribute__ ((__const__));
#ifdef __GNUC__
__extension__
extern lldiv_t lldiv (long long __numer, long long __denom)
     __THROW __attribute__ ((__const__));
#endif

__BEGIN_NAMESPACE_STD
/* Convert a string to a floating-point number.  */
extern double atof (__const char *__nptr) __THROW __attribute_pure__;
/* Convert a string to an integer.  */
extern int atoi (__const char *__nptr) __THROW __attribute_pure__;
/* Convert a string to a long integer.  */
extern long int atol (__const char *__nptr) __THROW __attribute_pure__;
__END_NAMESPACE_STD

#if defined __USE_ISOC99 || (defined __GLIBC_HAVE_LONG_LONG && defined __USE_MISC)
__BEGIN_NAMESPACE_C99
/* Convert a string to a long long integer.  */
__extension__ extern long long int atoll (__const char *__nptr)
     __THROW __attribute_pure__;
__END_NAMESPACE_C99
#endif

__BEGIN_NAMESPACE_STD
/* Convert a string to a floating-point number.  */
extern double strtod (__const char *__restrict __nptr,
		      char **__restrict __endptr) __THROW;
__END_NAMESPACE_STD

#ifdef	__USE_ISOC99
__BEGIN_NAMESPACE_C99
/* Likewise for `float' and `long double' sizes of floating-point numbers.  */
extern float strtof (__const char *__restrict __nptr,
		     char **__restrict __endptr) __THROW;

extern long double strtold (__const char *__restrict __nptr,
			    char **__restrict __endptr) __THROW;
__END_NAMESPACE_C99
#endif

__BEGIN_NAMESPACE_STD
/* Convert a string to a long integer.  */
extern long int strtol (__const char *__restrict __nptr,
			char **__restrict __endptr, int __base) __THROW;
/* Convert a string to an unsigned long integer.  */
extern unsigned long int strtoul (__const char *__restrict __nptr,
				  char **__restrict __endptr, int __base)
     __THROW;
__END_NAMESPACE_C99

#if defined __USE_ISOC99 || (defined __GLIBC_HAVE_LONG_LONG && defined __USE_MISC)
__BEGIN_NAMESPACE_C99
/* Convert a string to a quadword integer.  */
__extension__
extern long long int strtoll (__const char *__restrict __nptr,
			      char **__restrict __endptr, int __base) __THROW;
/* Convert a string to an unsigned quadword integer.  */
__extension__
extern unsigned long long int strtoull (__const char *__restrict __nptr,
					char **__restrict __endptr, int __base)
     __THROW;
__END_NAMESPACE_C99
#endif /* ISO C99 or GCC and use MISC.  */

#ifdef __USE_EXTERN_INLINES
/* Define inline functions which call the internal entry points.  */

__BEGIN_NAMESPACE_STD
extern __inline double
atof (__const char *__nptr) __THROW
{
  return strtod (__nptr, (char **) NULL);
}
extern __inline int
atoi (__const char *__nptr) __THROW
{
  return (int) strtol (__nptr, (char **) NULL, 10);
}
extern __inline long int
atol (__const char *__nptr) __THROW
{
  return strtol (__nptr, (char **) NULL, 10);
}
__END_NAMESPACE_STD

# if defined __USE_MISC || defined __USE_ISOC99
__BEGIN_NAMESPACE_C99
__extension__ extern __inline long long int
atoll (__const char *__nptr) __THROW
{
  return strtoll (__nptr, (char **) NULL, 10);
}
__END_NAMESPACE_C99
# endif
#endif /* Optimizing and Inlining.  */


/* Do a binary search for 'key' in 'base', which consists of
   'nmemb' elements of 'size' bytes each, using 'compare' to
   perform the comparisons.  */
extern void *bsearch (const void *__key, const void *__base,
		      size_t __nmemb, size_t __size,
		      int (*__compare)(const void *, const void *))
     __THROW;

/* Sort 'nmemb' elements of 'base', or 'size' bytes each.
   Use 'compare' to perform the comparisons.  */
extern void qsort (void *__base, size_t __nmemb, size_t __size,
		   int (*__compare)(const void *,const void *)) __THROW;

/* Return the length of a multibyte character in 's' which is
   no longer than n.  */
extern int mblen (const char *__s, size_t __n) __THROW;
extern size_t mbstowcs (wchar_t *__wchar, const char *__s, size_t __n) __THROW;
extern int mbtowc (wchar_t *__wchar, const char *__s, size_t __n) __THROW;
extern size_t wcstombs (char *__s, const wchar_t *__wchar, size_t __n) __THROW;
extern int wctomb (char *__s, wchar_t __wchar) __THROW;

#ifndef __ALLOCA_H
#include <alloca.h>
#endif

/* Efficient (?) internal decimal to ascii function used by printf() et al */
extern char *__dtoa(double __d, int __mode, int __ndigits,
		    int *__decpt, int *__sign, char **__rve) __THROW;

/* Parse comma separated suboption from 'option' and match against
   strings in 'tokens'. Return index with *value set to optional value.  */
extern int getsubopt (char **__option, const char *const *__tokens,
		      char **__value) __THROW;
extern char *suboptarg;

/* System V style 48-bit random number generator functions.  */

/* Return a non-negative, double-precision floating-point value in
   the range 0.0 to 1.0.  */
extern double drand48 (void) __THROW;
extern double erand48 (unsigned short int __xsubi[3]) __THROW;

/* Return non-negative, long integer in the range 0 to 2^31.  */
extern long int lrand48 (void) __THROW;
extern long int nrand48 (unsigned short int __xsubi[3]) __THROW;

/* Return signed, long integers in the range -2^31 to 2^31.  */
extern long int mrand48 (void) __THROW;
extern long int jrand48 (unsigned short int __xsubi[3]) __THROW;

/* Seed random number generator.  */
extern void srand48 (long int __seedval) __THROW;
extern unsigned short int *seed48 (unsigned short int __seed16v[3]) __THROW;
extern void lcong48 (unsigned short int __param[7]) __THROW;

__END_DECLS

#endif
