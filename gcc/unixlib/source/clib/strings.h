/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/strings.h,v $
 * $Date: 2002/12/22 18:22:28 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __STRINGS_H
#define __STRINGS_H 1

#ifdef __USE_BSD

# ifndef __UNIXLIB_FEATURES_H
#  include <unixlib/features.h>
# endif
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
#endif

__END_DECLS

#endif
