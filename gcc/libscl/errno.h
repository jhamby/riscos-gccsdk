/* errno.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, 1999 Nick Burrett.  */

#ifndef __ERRNO_H
#define __ERRNO_H

#ifndef __errno
#define __errno errno
extern volatile int errno;
#endif

/* Domain error. An input argument is outside the domain over
   which the mathematical function is defined.  */
#define EDOM 1

/* Range error. The result of a function cannot be represented
   as a double floating point value.  */
#define ERANGE 2

/* An unrecognised signal is caught by the default signal handler.  */
#define ESIGNUM 3

#define __SYS_NERR 3

#define EEXIST 4
#define EISDIR 5
#define EINVAL 6

#endif
