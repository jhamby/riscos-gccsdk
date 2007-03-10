#ifndef __ULIMIT_H
#define __ULIMIT_H

#include <features.h>

/* Constants used as the first parameter for `ulimit'.  They denote limits
   which can be set or retrieved using this function.  */
enum
{
  UL_GETFSIZE = 1,                      /* Return limit on the size of a file,
                                           in units of 512 bytes.  */
  UL_SETFSIZE,                          /* Set limit on the size of a file to
                                           second argument.  */
  __UL_GETMAXBRK,                       /* Return the maximum possible address
                                           of the data segment.  */
  __UL_GETOPENMAX                       /* Return the maximum number of files
                                           that the calling process can open.*/
};

__BEGIN_DECLS

/* Function depends on first parameter.
   1 = Return the limit on the size of a file, in units of 512 bytes.
   2 = Set the limit on the size of a file to new limit.  Only the
       super-user can increase the limit.
   3 = Return the maximum possible address of the data segment.
   4 = Return the maximum number of files that the calling process can open.  */
extern long int ulimit (int __cmd, ...) __THROW;

__END_DECLS

#endif
