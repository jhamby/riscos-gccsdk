/*
 * ANSI Standard 4.10: General Utilities <stdlib.h>.
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2000-2012 UnixLib Developers
 */

#ifndef __STDLIB_H
#define __STDLIB_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_size_t
#define __need_wchar_t
#define __need_NULL
#include <stddef.h>

__BEGIN_DECLS

__BEGIN_NAMESPACE_STD
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
__END_NAMESPACE_STD

#if defined __USE_ISOC99
__BEGIN_NAMESPACE_C99
/* Returned by `lldiv'.  */
__extension__
typedef struct
  {
    long long int quot;	/* Quotient.  */
    long long int rem;	/* Remainder.  */
  } lldiv_t;
__END_NAMESPACE_C99
#endif

/* The largest number rand will return (same as INT_MAX).  */
#define	RAND_MAX	2147483647


/* Successful exit status.  */
#define EXIT_SUCCESS 0
/* Failing exit status.  */
#define EXIT_FAILURE 1

/* Maximum length of a multibyte character in the current locale.  */
#define	MB_CUR_MAX	1

__BEGIN_NAMESPACE_STD
/* Abort execution and generate a core-dump.  This function raises the
   signal SIGABRT.  */
extern void abort (void) __attribute__ ((__noreturn__));

/* Register a function to be called when exit is called.  */
extern int atexit (void (*__atexit_function) (void)) __THROW __nonnull ((1));

/* Terminate the program with status. Call all functions registerd
   by atexit.  */
extern void exit (int __status) __THROW __attribute__ ((__noreturn__));
__END_NAMESPACE_STD

#ifdef __USE_ISOC99
__BEGIN_NAMESPACE_C99
/* Terminate the program with status.  Don't call any functions
   registerd by atexit.  */
extern void _Exit (int __status) __THROW __attribute__ ((__noreturn__));
__END_NAMESPACE_C99
#endif


__BEGIN_NAMESPACE_STD
/* Return the value of environment variable 'name'.  */
extern char *getenv (const char *__name) __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_STD

#if defined __USE_SVID || defined __USE_XOPEN
/* Put string, which is of the form "NAME=VALUE" in the environment.  */
extern int putenv (char *__string) __THROW __nonnull ((1));
#endif

#if defined __USE_BSD || defined __USE_XOPEN2K
/* Set NAME to VALUE in the environment.
   If REPLACE is nonzero, overwrite an existing value.  */
extern int setenv (const char *__name, const char *__value, int __replace)
     __THROW __nonnull ((2));

/* Remove NAME from the environment.  */
extern int unsetenv (const char *__name) __THROW;
#endif

#ifdef __USE_MISC
#  ifndef __TARGET_SCL__
/* The `clearenv' was planned to be added to POSIX.1 but probably
   never made it.  Nevertheless the POSIX.9 standard (POSIX bindings
   for Fortran 77) requires this function.  */
extern int clearenv (void) __THROW;
#  endif
#endif

#if defined __USE_MISC || defined __USE_XOPEN_EXTENDED
#  ifndef __TARGET_SCL__
/* Generate a unique temporary file name for temp.  */
extern char *mktemp (char *__template) __THROW __nonnull ((1)) __wur;

/* As for mktemp but returns an open file descriptor on the file.
   This function is a possible cancellation points and therefore not
   marked with __THROW.  */
extern int mkstemp(char *__template) __nonnull ((1)) __wur;
#  endif
#endif

__BEGIN_NAMESPACE_STD
/* Execute the given line via the CLI. See _kernel_system () in kernel.h.  */
extern int system (const char *__command) __THROW __wur;
__END_NAMESPACE_STD

#if defined __USE_BSD || defined __USE_XOPEN_EXTENDED
#  ifndef __TARGET_SCL__
/* Canonicalise a filename */
extern char *realpath (const char *__file_name, char *__resolved_name)
     __THROW __wur;
#  endif
#endif

__BEGIN_NAMESPACE_STD
/* Allocate nmemb elements of size bytes each. Initialise
   all members to zero.  */
extern void *calloc (size_t __nmemb, size_t __size)
     __THROW  __attribute_malloc__ __wur;

/* Free a block allocated by malloc, calloc or realloc.  */
extern void free (void *__ptr) __THROW;

/* Allocate __size bytes of memory.  */
extern void *malloc (size_t __size) __THROW __attribute_malloc__ __wur;

/* Re-allocate the previously malloc'd block, ptr, making the
   new block size bytes.  */
/* __attribute_malloc__ is not used, because if realloc returns
   the same pointer that was passed to it, aliasing needs to be allowed
   between objects pointed by the old and new pointers.  */
extern void *realloc (void *__ptr, size_t __size)
     __THROW __wur;
__END_NAMESPACE_STD


#if defined __USE_BSD || defined __USE_XOPEN_EXTENDED
#  ifndef __TARGET_SCL__
/* Allocate size bytes on a page boundary. The storage cannot be freed.  */
extern void *valloc (size_t __bytes) __THROW __attribute_malloc__ __wur;
#  endif
#endif

#ifndef __TARGET_SCL__
extern void *memalign (size_t __alignment,
		       size_t __bytes) __THROW __attribute_malloc__ __wur;
extern void cfree (void *__mem) __THROW;
extern int malloc_trim (size_t) __THROW;
#endif

__BEGIN_NAMESPACE_STD
/* Return a random integer between 0 and RAND_MAX (System V interface).  */
extern int rand (void) __THROW;

/* Seed the random number generator with the given number. The default seed
   is 1.  More randomness can be achieved by srand (time (0)).   */
extern void srand (long __seed) __THROW;

/* The following is not available in SharedCLibrary.  */
#if 0
#ifdef __TARGET_SCL__
#  define _ANSI_RAND_MAX 0x7fff

extern int _ANSI_rand (void);
extern void _ANSI_srand (unsigned int __seed);
#endif
#endif

__END_NAMESPACE_STD


#if defined __USE_SVID || defined __USE_XOPEN_EXTENDED || defined __USE_BSD
#  ifndef __TARGET_SCL__
# include <sys/types.h> /* we need int32_t... */
/* Return a random integer between 0 and RAND_MAX (BSD interface).  */
extern long int random (void) __THROW;

/* Seed the random number generator (BSD interface).  */
extern void srandom (unsigned int __seed) __THROW;

/* Initialize the random number generator to use state buffer STATEBUF,
   of length STATELEN, and seed it with SEED.  Optimal lengths are 8, 16,
   32, 64, 128 and 256, the bigger the better; values less than 8 will
   cause an error and values greater than 256 will be rounded down.  */
extern char *initstate (unsigned int __seed, char *__statebuf,
                        size_t __statelen) __THROW __nonnull ((2));

/* Switch the random number generator to state buffer STATEBUF,
   which should have been previously initialized by `initstate'.  */
extern char *setstate (char *__statebuf) __THROW __nonnull ((1));
#  endif
#endif

__BEGIN_NAMESPACE_STD
/* Return the non-negative version of x.  */
extern int abs (int __x) __THROW __attribute__ ((__const__)) __wur;
/* Return the non-negative version of x. This is the long int equivalent
   of abs().  */
extern long int	labs (long int __x) __THROW __attribute__ ((__const__)) __wur;

/* Return numerator divided by denominator.  */
extern div_t div (int __numer, int __denom)
     __THROW __attribute__ ((__const__)) __wur;
/* Integer divide x by y returning the quotient and remainder in
   a ldiv_t structure.  This is the long int version of div().  */
extern ldiv_t ldiv (long __numer, long __denom)
     __THROW __attribute__ ((__const__)) __wur;
__END_NAMESPACE_STD

#ifdef __USE_ISOC99
__BEGIN_NAMESPACE_C99

__extension__ extern long long int llabs (long long int __x)
     __THROW __attribute__ ((__const__)) __wur;
     
__extension__ extern lldiv_t lldiv (long long __numer, long long __denom)
     __THROW __attribute__ ((__const__)) __wur;
__END_NAMESPACE_C99
#endif

__BEGIN_NAMESPACE_STD
/* Convert a string to a floating-point number. Similar to the strtod
   function, except that it need not detect overflow and underflow
   errors.  */
extern double atof (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Convert a string to an integer. Similar to atol() but it returns an
   'int' instead of a 'long int'.  */
extern int atoi (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Convert a string to a long integer. Similar to the strtol function with
   a base argument of 10, except that it need not detect overflow errors.  */
extern long int atol (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
__END_NAMESPACE_STD

#if defined __USE_ISOC99 || (defined __GLIBC_HAVE_LONG_LONG && defined __USE_MISC)
__BEGIN_NAMESPACE_C99
/* Convert a string to a long long integer.  */
__extension__ extern long long int atoll (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
__END_NAMESPACE_C99
#endif

__BEGIN_NAMESPACE_STD
/* The strtod (string-to-double) function converts the initial part
   of 'string' to a floating-point number.
   If 'endptr' is not NULL, strtod will store a pointer to the remaining
   characters in the string in '*tailptr'.  */
extern double strtod (const char *__restrict __string,
		      char **__restrict __endptr)
     __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_STD

#ifdef	__USE_ISOC99
__BEGIN_NAMESPACE_C99
/* Likewise for `float' and `long double' sizes of floating-point numbers.  */
extern float strtof (const char *__restrict __nptr,
		     char **__restrict __endptr)
     __THROW __nonnull ((1)) __wur;

extern long double strtold (const char *__restrict __nptr,
			    char **__restrict __endptr)
     __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_C99
#endif

__BEGIN_NAMESPACE_STD
/* The strtol (string-to-long) function converts the initial part
   of 'string' to a signed integer, which is returned as a value
   of 'long int'.  If 'base' is zero, decimal is assumed, unless
   the digits being with '0' (specifying octal) or '0x'/'0X' (specifying
   hexadecimal).  Otherwise 'base' must have a value between 2 and 35.
   If 'tailptr' is not NULL, strtol will store a pointer to the remaining
   characters in the string in '*tailptr'.  */
extern long int strtol (const char *__restrict __nptr,
			char **__restrict __endptr, int __base)
     __THROW __nonnull ((1)) __wur;

/* The strtoul (string-to-unsigned-long) function is like strtol
   except it deals with usigned numbers.  No + or - sign may
   appear before the number.  */
extern unsigned long int strtoul (const char *__restrict __nptr,
				  char **__restrict __endptr, int __base)
     __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_STD

#if defined __USE_ISOC99 || (defined __GLIBC_HAVE_LONG_LONG && defined __USE_MISC)
__BEGIN_NAMESPACE_C99
/* Convert a string to a quadword integer.  */
__extension__
extern long long int strtoll (const char *__restrict __nptr,
			      char **__restrict __endptr, int __base)
     __THROW __nonnull ((1)) __wur;

/* Convert a string to an unsigned quadword integer.  */
__extension__
extern unsigned long long int strtoull (__const char *__restrict __nptr,
					char **__restrict __endptr, int __base)
     __THROW __nonnull ((1)) __wur;
__END_NAMESPACE_C99
#endif /* ISO C99 or GCC and use MISC.  */

#ifdef __USE_EXTERN_INLINES
/* Define inline functions which call the internal entry points.  */

__BEGIN_NAMESPACE_STD
__extern_inline double
__NTH (atof (__const char *__nptr))
{
  return strtod (__nptr, (char **) NULL);
}
__extern_inline int
__NTH (atoi (__const char *__nptr))
{
  return (int) strtol (__nptr, (char **) NULL, 10);
}
__extern_inline long int
__NTH (atol (__const char *__nptr))
{
  return strtol (__nptr, (char **) NULL, 10);
}
__END_NAMESPACE_STD

# if defined __USE_MISC || defined __USE_ISOC99
__BEGIN_NAMESPACE_C99
__extension__ __extern_inline long long int
__NTH (atoll (__const char *__nptr))
{
  return strtoll (__nptr, (char **) NULL, 10);
}
__END_NAMESPACE_C99
# endif
#endif /* Optimizing and Inlining.  */


__BEGIN_NAMESPACE_STD
/* Binary search the sorted array 'base' for an object that is equivalent
   to 'key'. The array contains 'nmemb' elements, each of which is
   size 'size' bytes.
   The 'compare' function is used to perform the comparison.  This
   function is called with two pointer arguments and should return
   an integer less than, equal to, or greater than zero corresponding
   to whether its first argument is considered less than, equal to,
   or greater than its second argument.  */
extern void *bsearch (const void *__key, const void *__base,
		      size_t __nmemb, size_t __size,
		      int (*__compare)(const void *, const void *))
     __nonnull ((1, 2, 5)) __wur;

/* Sort the array 'base'. The array contains 'nmemb' elements,
   each of which is of size 'size'.
   The 'compare' function is similar in functionality to the bsearch
   compare function.  */
extern void qsort (void *__base, size_t __nmemb, size_t __size,
		   int (*__compare)(const void *,const void *))
     __nonnull ((1, 4));

/* Return the number of bytes that make up the multibyte character
   beginning at 'string', never examining more than 'size' bytes.  */
extern int mblen (const char *__string, size_t __size) __THROW;

/* Convert a string of multibyte characters (string) to a
   wide character array (string), storing not more than 'size'
   wide characters.  */
extern size_t mbstowcs (wchar_t *__wstring, const char *__string, size_t __size) __THROW;

/* Convert the first multibyte character beginning at 'string' to
   its corresponding wide character code.  The result is stored
   in '*result'.  mbtowc never examines more than 'size' bytes.  */
extern int mbtowc (wchar_t *__wchar, const char *__string, size_t __size) __THROW;

/* Convert the null-terminated wide character array 'wstring'
   into a string containing multibyte characters, storing not
   more than 'size' bytes.  */
extern size_t wcstombs (char *__string, const wchar_t *__wstring, size_t __size) __THROW;

/* Convert the wide character code 'wchar' to its corresponding
   multibyte character sequence and store the result in 'string'.  */
extern int wctomb (char *__string, wchar_t __wchar) __THROW;

__END_NAMESPACE_STD

#ifndef __ALLOCA_H
#include <alloca.h>
#endif

/* Efficient (?) internal decimal to ascii function used by printf() et al */
extern char *__dtoa(double __d, int __mode, int __ndigits,
		    int *__decpt, int *__sign, char **__rve) __THROW;

#ifdef __USE_XOPEN_EXTENDED
#  ifndef __TARGET_SCL__
/* Parse comma separated suboption from 'option' and match against
   strings in 'tokens'. Return index with *value set to optional value.  */
extern int getsubopt (char **__restrict __option,
		      const char *const *__restrict __tokens,
		      char **__restrict __value)
     __THROW __nonnull ((1, 2, 3)) __wur;
extern char *suboptarg;
#  endif
#endif

#if defined __USE_SVID || defined __USE_XOPEN
#  ifndef __TARGET_SCL__
/* System V style 48-bit random number generator functions.  */

/* Return a non-negative, double-precision floating-point value in
   the range 0.0 to 1.0.  */
extern double drand48 (void) __THROW;
extern double erand48 (unsigned short int __xsubi[3]) __THROW __nonnull ((1));

/* Return non-negative, long integer in the range 0 to 2^31.  */
extern long int lrand48 (void) __THROW;
extern long int nrand48 (unsigned short int __xsubi[3])
     __THROW __nonnull ((1));

/* Return signed, long integers in the range -2^31 to 2^31.  */
extern long int mrand48 (void) __THROW;
extern long int jrand48 (unsigned short int __xsubi[3])
     __THROW __nonnull ((1));

/* Seed random number generator.  */
extern void srand48 (long int __seedval) __THROW;

extern unsigned short int *seed48 (unsigned short int __seed16v[3])
     __THROW __nonnull ((1));

extern void lcong48 (unsigned short int __param[7]) __THROW __nonnull ((1));
#  endif
#endif

__END_DECLS

#endif
