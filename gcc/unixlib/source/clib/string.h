/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/string.h,v $
 * $Date: 2000/07/15 14:52:12 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* ANSI Standard 4.11: String Handling <string.h>.  */

#ifndef __STRING_H
#define __STRING_H

#ifndef __STDDEF_H
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Copy n bytes from src to dest.  */
extern void *memcpy (void *dest, const void *src, size_t n);

/* Copy n bytes from src to dest, guaranteeing correct
   behaviour for overlapping data.  */
extern void *memmove (void *dest, const void *src, size_t n);

/* Set n bytes of s to c.  */
extern void *memset (void *s, int c, size_t n);

/* Compare n bytes of s1 and s2.  */
extern int memcmp (const void *s1, const void *s2, size_t n);

/* Search n bytes of s for c.  */
extern void *memchr (const void *s, int c, size_t n);

/* Copy src to dest. */
extern char *strcpy (char *dest, const char *src);

/* Copy no more than n chars of src to dest.  */
extern char *strncpy (char *dest, const char *src, size_t n);

/* Copy src to dest return a pointer to the terminating null
   character of dest.  */
extern char *stpcpy (char *dest, const char *src);

/* Copy no more than n chars of src to dest. Return a pointer
   to the terminating null character of dest. */
extern char *stpncpy (char *dest, const char *src, size_t n);

/* Append src onto dest.  */
extern char *strcat (char *dest, const char *src);

/* Append no more than n chars from src to dest. */
extern char *strncat (char *dest, const char *src, size_t n);

/* Compare s1 and s2.  */
extern int strcmp (const char *s1, const char *s2);

/* Compare n chars of s1 and s2.  */
extern int strncmp (const char *s1, const char *s2, size_t n);

/* Compare two strings according to the locale's collating rules */
extern int strcoll (const char *s1, const char *s2);
/* Transform a string according to the locale's collating rules */
extern size_t strxfrm (char *to, const char *from, size_t size);

/* Find the first occurrence of c in s. */
extern char *strchr (const char *s, int c);

/* Find the last occurrence of c in s.  */
extern char *strrchr (const char *s, int c);

/* Return the length of the initial segment of s that consists
   entirely of chars in accept.  */
extern size_t strspn (const char *s, const char *accept);

/* Return the length of the initial segment of s that consists
   entirely of chars not in reject.  */
extern size_t strcspn (const char *s, const char *reject);

/* Find the first occurence in s of any char in accept.  */
extern char *strpbrk (const char *s, const char *accept);

/* Find the first occurrence of s in s1.  */
extern char *strstr (const char *s, const char *s1);

/* Divide s into tokens separated by chars in delim.  */
extern char *strtok (char *s, const char *delim);
/* Re-entrant version of strtok.  */
extern char *strtok_r (char *s, const char *delim, char **save_ptr);

/* Return the length of s. */
extern size_t strlen (const char *s);

/* Compare S1 and S2, ignoring case.  */
extern int stricmp(const char *,const char *);

/* Compare n chars of S1 and S2, ignoring case.  */
extern int strnicmp(const char *,const char *,size_t);

extern char *strichr(const char *,int);
extern char *strrichr(const char *,int);



/* BSD enhancements.  */

/* Copy no more than n bytes of src to dest, stopping when c is found.
   Return the position in dest one byte past where c was copied,
   or null if C was not in the string.  */
extern void *memccpy (void *dest, const void *src, int c, size_t n);

/* Duplicate s, returning an identical malloc'd string.  */
extern char *strdup (const char *s);

/* Same as strchr.  */
extern char *index (const char *s, int c);

/* Same as strrchr.  */
extern char *rindex (const char *s, int c);

/* Same as memcmp.  */
extern int bcmp (const void *s1,const void *s2, size_t n);

/* Copy n bytes of src to dest.  */
extern void bcopy (const void *src, void *dest, size_t n);

/* Set n bytes of s to 0.  */
extern void bzero (void *s, size_t n);

#if 0
/* Return the position of the first bit set in I, or 0 if none are set.
   The least-significant bit is position 1, the most-significant 32.  */
extern int ffs (int i);
#endif

/* Compare S1 and S2, ignoring case.  */
extern int strcasecmp (const char *s1, const char *s2);

/* Compare n chars of S1 and S2, ignoring case.  */
extern int strncasecmp (const char *s1, const char *s2, size_t n);

/* Return the next DELIM-delimited token from *STRINGP,
   terminating it with a '\0', and update *STRINGP to point past it.  */
extern char *strsep (char **stringp, const char *delim);

/* GNU enhancements.  */

/* Return a string describing the meaning of the signal number sig.  */
extern const char *strsignal (int sig);

/* Return the descriptive error message string for an error code.  */
extern char *strerror (int errnum);

#ifdef __cplusplus
	}
#endif

#endif
