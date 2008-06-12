/* string.h standard header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
   Copyright (c) 2008 UnixLib Developers
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

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

/* Compare S1 and S2, ignoring case.  */
extern int stricmp (const char *__s1, const char *__s2);

/* Compare n chars of S1 and S2, ignoring case.  */
extern int strnicmp (const char *__s1, const char *__s2, size_t __n);

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

/* Duplicate s, returning an identical malloc'd string.  */
extern char *strdup (const char *__s)
  __attribute__ ((__malloc__));


#ifdef __cplusplus
}
#endif

#endif
