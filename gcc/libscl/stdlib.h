/* stdlib.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, Nick Burrett.  */


#ifndef __STDLIB_H
#define __STDLIB_H

#ifndef __STDDEF_H
#include <stddef.h>
#endif

#ifndef __ERRNO_H
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* GCC has various useful declarations that can be made with the
   __attribute__ syntax.  Disable its use for other compilers.  */
#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__(x) /* Ignore.  */
#endif
#endif

/* Returned by `div'.  */
typedef struct div_t
{
  int quot, rem;
} div_t;

/* Returned by `ldiv'.  */
typedef struct ldiv_t
{
  long int quot, rem;
} ldiv_t;

#ifdef __EXIT_FAILURE
#define EXIT_FAILURE __EXIT_FAILURE

#else
/* Failing exit status.  */
#define EXIT_FAILURE 1
#endif
/* Successful exit status.  */
#define EXIT_SUCCESS 0

/* The largest number rand will return (same as INT_MAX).  */
#define RAND_MAX 0x7fffffff

#define _ANSI_RAND_MAX 0x7fff

/* Maximum length of a multibyte character in the current locale.  */
#define MB_CUR_MAX 1

/* Similar to the strtod function, except that it need not detect
   overflow and underflow errors.  */
extern double atof (const char *__string);

/* Similar to the strtol function with a base argument of 10,
   except that it need not detect overflow errors.  */
extern long int atol (const char *__string);

/* Similar to atol() but it returns an 'int' instead of a 'long int'.  */
extern int atoi (const char *__string);

/* The strtod (string-to-double) function converts the initial part
   of 'string' to a floating-point number.

   If 'tailptr' is not NULL, strtol will store a pointer to the remaining
   characters in the string in '*tailptr'.  */
extern double strtod (const char *__string, char **__tailptr);

/* The strtol (string-to-long) function converts the initial part
   of 'string' to a signed integer, which is returned as a value
   of 'long int'.  If 'base' is zero, decimal is assumed, unless
   the digits being with '0' (specifying octal) or '0x'/'0X' (specifying
   hexadecimal).  Otherwise 'base' must have a value between 2 and 35.

   If 'tailptr' is not NULL, strtol will store a pointer to the remaining
   characters in the string in '*tailptr'.  */
extern long int strtol (const char *__string, char **__tailptr, int __base);

/* The strtoul (string-to-unsigned-long) function is like strtol
   except it deals with usigned numbers.  No + or - sign may
   appear before the number.  */
extern unsigned long int strtoul (const char *__string, char **__tailptr,
				  int __base);

/* Returns the next pseudo-random number in the series. The value is
   in the range from 0 to RAND_MAX.  */
extern int rand (void);

/* Establish 'seed' as the seed for a new series of pseudo-random
   numbers.  The default seed is 1.  Truly random numbers can
   be achieved by srand (time (0)).  */
extern void srand (unsigned int __seed);

extern int _ANSI_rand (void);
extern void _ANSI_srand (unsigned int __seed);

/* Allocate a block long enough to contain a vector of 'count'
   elements, each of size 'eltsize'. Its contents are cleared
   to zero before 'calloc' returns.  */
extern void *calloc (size_t __count, size_t __eltsize);

/* Deallocates the block of storage pointed at by 'ptr'.  */
extern void free (void *__ptr);

/* Return a pointer to a newly allocated block 'size' bytes
   long, or a null pointer if the block could not be allcated.  */
extern void *malloc (size_t __size);

/* Change the size of the block whose address is 'ptr' to be 'newsize'.  */
extern void *realloc (void *__ptr, size_t __newsize);

/* Cause abnormal program termination.  This function raises the
   signal SIGABRT.  */
extern void abort (void) __attribute__ ((__volatile__));

/* Register the function 'function' to be called at normal program
   termination.  The function is called with no arguments.  */
extern int atexit (void (*__function) (void));

/* Terminate a process with status 'status'. This function does
   not return.  This function executes all functions registered
   with atexit().  */
extern void exit (int __status) __attribute__ ((__volatile__));

/* Return a string that is the value of the environment variable
   'name'.  */
extern char *getenv (const char *__name);

/* Execute 'command'. See _kernel_system () in kernel.h.  */
extern int system (const char *__command);

/* Search the sorted array 'array' for an object that is equivalent
   to 'key'. The array contains 'count' elements, each of which is
   size 'size' bytes.

   The 'compare' function is used to perform the comparison.  This
   function is called with two pointer arguments and should return
   an integer less than, equal to, or greater than zero corresponding
   to whether its first argument is considered less than, equal to,
   or greater than its second argument.  */
extern void *bsearch (const void *__key, const void *__array,
		      size_t __count, size_t __size,
		      int (*__compare) (const void *, const void *));

/* Sort the array 'array'. The array contains 'count' elements,
   each of which is of size 'size'.

   The 'compare' function is similar in functionality to the bsearch
   compare function.  */
extern void qsort (void *__array, size_t __count, size_t __size,
		   int (*__compare) (const void *, const void *));

/* Return the non-negative version of x.  */
extern int abs (int __x) __attribute__ ((__const__));

/* Integer divide x by y returning the quotient and remainder in
   a div_t structure.  */
extern div_t div (int __x, int __y) __attribute__ ((__const__));

/* Return the non-negative version of x. This is the long int equivalent
   of abs().  */
extern long int labs (long int __x) __attribute__ ((__const__));

/* Integer divide x by y returning the quotient and remainder in
   a ldiv_t structure.  This is the long int version of div().  */
extern ldiv_t ldiv (long int __x, long int __y) __attribute__ ((__const__));

/* Return the number of bytes that make up the multibyte character
   beginning at 'string', never examining more than 'size' bytes.  */
extern int mblen (const char *__string, size_t __size);

/* Convert the first multibyte character beginning at 'string' to
   its corresponding wide character code.  The result is stored
   in '*result'.  mbtowc never examines more than 'size' bytes.  */
extern int mbtowc (wchar_t *__result, const char *__string, size_t __size);

/* Convert the wide character code 'wchar' to its corresponding
   multibyte character sequence and store the result in 'string'.  */
extern int wctomb (char *__string, wchar_t __wchar);

/* Convert a string of multibyte characters (string) to a
   wide character array (string), storing not more than 'size'
   wide characters.  */
extern size_t mbstowcs (wchar_t *__wstring, const char *__string,
			size_t __size);

/* Convert the null-terminated wide character array 'wstring'
   into a string containing multibyte characters, storing not
   more than 'size' bytes.  */
extern size_t wcstombs (char *__string, const wchar_t *__wstring,
			size_t __size);

#ifdef __cplusplus
}
#endif

#endif
