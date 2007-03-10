/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __STRINGS_H
#define __STRINGS_H 1

/* We don't need and should not read this file if <string.h> was already
   read. The one exception being that if __USE_BSD isn't defined, then
   these aren't defined in string.h, so we need to define them here.  */
#if !defined __STRING_H || !defined __USE_BSD

# include <features.h>
# define __need_size_t
# include <stddef.h>

__BEGIN_DECLS

/* Same as strchr.  */
extern char *index (const char *__s, int __c);

/* Same as strrchr.  */
extern char *rindex (const char *__s, int __c);

/* Same as memcmp.  */
extern int bcmp (const void *__s1,const void *__s2, size_t __n);

/* Copy n bytes of src to dest.  */
extern void bcopy (const void *__src, void *__dest, size_t __n);

/* Set n bytes of s to 0.  */
extern void bzero (void *__s, size_t __n);

/* Compare S1 and S2, ignoring case.  */
extern int strcasecmp (const char *__s1, const char *__s2);

/* Compare n chars of S1 and S2, ignoring case.  */
extern int strncasecmp (const char *__s1, const char *__s2, size_t __n);


/* Return the position of the first bit set in I, or 0 if none are set.
   The least-significant bit is position 1, the most-significant 32.  */
extern int ffs (int __i);

__END_DECLS

#endif /* string.h  */

#endif /* strings.h  */
