/* string.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, Nick Burrett.  */

#ifndef __STRING_H
#define __STRING_H

#ifndef __STDDEF_H
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Copy n bytes from src to dest.  */
extern void *memcpy (void *__dest, const void *__src, size_t __n);

/* Copy n bytes from src to dest, guaranteeing correct
   behaviour for overlapping data.  */
extern void *memmove (void *__dest, const void *__src, size_t __n);

/* Copy src to dest. */
extern char *strcpy (char *__dest, const char *__src);

/* Copy no more than n chars of src to dest.  */
extern char *strncpy (char *__dest, const char *__src, size_t __n);

/* Append src onto dest.  */
extern char *strcat (char *__dest, const char *__src);

/* Append no more than n chars from src to dest. */
extern char *strncat (char *__dest, const char *__src, size_t __n);

/* Compare n bytes of s1 and s2.  */
extern int memcmp (const void *__s1, const void *__s2, size_t __n);

/* Compare s1 and s2.  */
extern int strcmp (const char *__s1, const char *__s2);

/* Compare n chars of s1 and s2.  */
extern int strncmp (const char *__s1, const char *__s2, size_t __n);

/* Similar to strcmp but uses the collating sequence of the
   current locale for collation (LC_COLLATE).  */
extern int strcoll (const char *__s1, const char *__s2);

/* Transforms 'string' using the collation transformation
   determined by the locale currently selected for collation,
   and stores the transformed string in the array 'to'. Up
   to 'size' characters are stored (including terminating null
   character).  */
extern size_t strxfrm (char *__to, const char *__string, size_t __size);

/* Search n bytes of s for c.  */
extern void *memchr (const void *__s, int __c, size_t __n);

/* Find the first occurrence of c in s. */
extern char *strchr (const char *__s, int __c);

/* Return the length of the initial segment of s that consists
   entirely of chars not in reject.  */
extern size_t strcspn (const char *__s, const char *__reject);

/* Find the first occurence in s of any char in accept.  */
extern char *strpbrk (const char *__s, const char *__accept);

/* Find the last occurrence of c in s.  */
extern char *strrchr (const char *__s, int __c);

/* Return the length of the initial segment of s that consists
   entirely of chars in accept.  */
extern size_t strspn (const char *__s, const char *__accept);

/* Find the first occurrence of s in s1.  */
extern char *strstr (const char *__s, const char *__s1);

/* Divide s into tokens separated by chars in delim.  */
extern char *strtok (char *__s, const char *__delim);

/* Set n bytes of s to c.  */
extern void *memset (void *__s, int __c, size_t __n);

/* Return the descriptive error message string for an error code.  */
extern char *strerror (int __errnum);

/* Return the length of s. */
extern size_t strlen (const char *__s);

#ifdef __cplusplus
}
#endif

#endif
