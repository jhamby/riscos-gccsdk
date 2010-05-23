/*
 * ANSI Standard 4.11: String Handling <string.h>.
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __STRING_H
#define __STRING_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_size_t
#define __need_NULL
#include <stddef.h>

__BEGIN_DECLS

__BEGIN_NAMESPACE_STD

/* Copy n bytes from src to dest.  */
extern void *memcpy (void *__restrict __dest, const void *__restrict __src,
		     size_t __n) __THROW __nonnull ((1, 2));

/* Copy n bytes from src to dest, guaranteeing correct
   behaviour for overlapping data.  */
extern void *memmove (void *__dest, const void *__src, size_t __n)
     __THROW __nonnull ((1, 2));

/* Set n bytes of s to c.  */
extern void *memset (void *__s, int __c, size_t __n) __THROW __nonnull ((1));

/* Compare n bytes of s1 and s2.  */
extern int memcmp (const void *__s1, const void *__s2, size_t __n)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Search n bytes of s for c.  */
extern void *memchr (const void *__s, int __c, size_t __n)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
__END_NAMESPACE_STD

#ifdef __USE_GNU
/* Search in S for C.  This is similar to `memchr' but there is no
   length limit.  */
extern void *rawmemchr (__const void *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Search N bytes of S for the final occurrence of C.  */
#define __memrchr memrchr
extern void *memrchr (__const void *__s, int __c, size_t __n)
      __THROW __attribute_pure__ __nonnull ((1)) __wur;
#endif


__BEGIN_NAMESPACE_STD
/* Copy SRC to DEST.  */
extern char *strcpy (char *__restrict __dest, const char *__restrict __src)
     __THROW __nonnull ((1, 2));

/* Copy no more than n chars of src to dest.  */
extern char *strncpy (char *__restrict __dest, const char *__restrict __src,
		      size_t __n) __THROW __nonnull ((1, 2));

/* Append SRC onto DEST.  */
extern char *strcat (char *__restrict __dest, const char *__restrict __src)
     __THROW __nonnull ((1, 2));

/* Append no more than n chars from src to dest. */
extern char *strncat (char *__restrict __dest, const char *__restrict __src,
		      size_t __n) __THROW __nonnull ((1, 2));

/* Compare S1 and S2.  */
extern int strcmp (const char *__s1, __const char *__s2)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Compare N characters of S1 and S2.  */
extern int strncmp (const char *__s1, __const char *__s2, size_t __n)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Compare the collated forms of S1 and S2.  */
extern int strcoll (const char *__s1, __const char *__s2)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Transforms 'src' using the collation transformation
   determined by the locale currently selected for collation,
   and stores the transformed string in the array 'dest'. Up
   to 'n' characters are stored (including terminating NUL
   character).  */
extern size_t strxfrm (char *__restrict __dest,
		       const char *__restrict __src, size_t __n)
     __THROW __nonnull ((2));

/* Find the first occurrence of c in s. */
extern char *strchr (const char *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Find the last occurrence of c in s.  */
extern char *strrchr (const char *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Return the length of the initial segment of s that consists
   entirely of chars in accept.  */
extern size_t strspn (const char *__s, const char *__accept)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Return the length of the initial segment of s that consists
   entirely of chars not in reject.  */
extern size_t strcspn (const char *__s, const char *__reject)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Find the first occurence in s of any char in accept.  */
extern char *strpbrk (const char *__s, const char *__accept)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Find the first occurrence of s in s1.  */
extern char *strstr (const char *__s, const char *__s1)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Divide s into tokens separated by chars in delim.  */
extern char *strtok (char *__restrict __s, const char *__restrict __delim)
     __THROW __nonnull ((2)) __wur;

__END_NAMESPACE_STD

#if defined __USE_POSIX || defined __USE_MISC
/* Re-entrant version of strtok.  */
extern char *strtok_r (char *__restrict __s, const char *__restrict __delim,
		       char **__restrict __save_ptr)
     __THROW __nonnull ((2, 3));
#endif

__BEGIN_NAMESPACE_STD
/* Return the length of s. */
extern size_t strlen (const char *__s)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

#ifdef __USE_GNU
/* Find the length of STRING, but scan at most MAXLEN characters.
   If no '\0' terminator is found in that many characters, return MAXLEN.  */
extern size_t strnlen (const char *__string, size_t __maxlen)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
#endif

__END_NAMESPACE_STD

/* Compare S1 and S2, ignoring case.  */
extern int stricmp (const char *, const char *) __THROW __wur;

/* Compare n chars of S1 and S2, ignoring case.  */
extern int strnicmp (const char *, const char *, size_t) __THROW __wur;

extern char *strichr (const char *, int) __THROW __wur;
extern char *strrichr (const char *, int) __THROW __wur;

#ifdef __USE_GNU
/* Copy src to dest return a pointer to the terminating null
   character of dest.  */
extern char *stpcpy (char *__restrict __dest, const char *__restrict __src)
     __THROW __nonnull ((1, 2));

/* Copy no more than n chars of src to dest. Return a pointer
   to the terminating null character of dest. */
extern char *stpncpy (char *__restrict __dest, const char *__restrict __src,
		      size_t __n) __THROW __nonnull ((1, 2));
#endif


/* BSD enhancements.  */

#if defined __USE_SVID || defined __USE_BSD || defined __USE_XOPEN
/* Copy no more than n bytes of src to dest, stopping when c is found.
   Return the position in dest one byte past where c was copied,
   or null if C was not in the string.  */
extern void *memccpy (void *__dest, const void *__src, int __c, size_t __n)
     __THROW __nonnull ((1, 2));
#endif

#if defined __USE_SVID || defined __USE_BSD || defined __USE_XOPEN_EXTENDED
/* Duplicate s, returning an identical malloc'd string.  */
extern char *strdup (const char *__s)
     __THROW __attribute_malloc__ __nonnull ((1)) __wur;
#endif

#ifdef __USE_BSD
/* Return the next DELIM-delimited token from *STRINGP,
   terminating it with a '\0', and update *STRINGP to point past it.  */
extern char *strsep (char **__restrict __stringp,
		     const char *__restrict __delim)
     __THROW __nonnull ((1, 2)) __wur;
#endif

/* GNU enhancements.  */

#ifdef __USE_GNU
/* This function is similar to `strdup' but always copies at most
   __n characters into the newly allocated string.

   If the length of __s is more than __n, then `strndup' copies just
   the first __n characters and adds a closing null terminator.
   Otherwise all characters are copied and the string is terminated.

   This function is different to `strncpy' in that it always
   terminates the destination string.  */
extern char *strndup (const char *__s, size_t __n)
     __THROW __attribute_malloc__ __nonnull ((1)) __wur;

#endif

/* This function is similar to `strchr'.  But it returns a pointer to
   the closing NUL byte in case C is not found in S.  */
extern char *strchrnul (const char *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Return a string describing the meaning of the signal number sig.  */
extern char *strsignal (int __sig) __THROW __wur;

__BEGIN_NAMESPACE_STD
/* Return the descriptive error message string for an error code.  */
extern char *strerror (int __errnum) __THROW __wur;
__END_NAMESPACE_STD

/* Set N bytes of S to 0.  */
extern void bzero (void *__s, size_t __n) __THROW __nonnull ((1));

#if defined __USE_BSD
/* Copy N bytes of SRC to DEST (like memmove, but args reversed).  */
extern void bcopy (const void *__src, void *__dest, size_t __n)
     __THROW __nonnull ((1, 2));

/* Compare N bytes of S1 and S2 (same as memcmp).  */
extern int bcmp (const void *__s1, __const void *__s2, size_t __n)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Find the first occurrence of C in S (same as strchr).  */
extern char *index (const char *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Find the last occurrence of C in S (same as strrchr).  */
extern char *rindex (const char *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

/* Return the position of the first bit set in I, or 0 if none are set.
   The least-significant bit is position 1, the most-significant 32.  */
extern int ffs (int __i) __THROW __attribute__ ((__const__)) __wur;

/* The following two functions are non-standard but necessary for non-32 bit
   platforms.  */
# ifdef __USE_GNU
extern int ffsl (long int __l) __THROW __attribute__ ((__const__)) __wur;

__extension__ extern int ffsll (long long int __ll)
     __THROW __attribute__ ((__const__)) __wur;
# endif

/* Compare S1 and S2, ignoring case.  */
extern int strcasecmp (const char *__s1, const char *__s2)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;

/* Compare no more than N chars of S1 and S2, ignoring case.  */
extern int strncasecmp (const char *__s1, const char *__s2, size_t __n)
     __THROW __attribute_pure__ __nonnull ((1, 2)) __wur;
#endif /* Use BSD.  */

#if defined __USE_XOPEN2K || defined __USE_MISC
/* Re-entrant version of strerror */
extern int strerror_r (int __errnum, char *__strerrbuf, size_t __buflen)
     __THROW __nonnull ((2));
#endif

# ifndef basename
/* Return the file name within directory of FILENAME.  We don't
   declare the function if the `basename' macro is available (defined
   in <libgen.h>) which makes the XPG version of this function
   available.  */
extern char *basename (__const char *__filename) __THROW __nonnull ((1));
# endif

__END_DECLS

#endif
