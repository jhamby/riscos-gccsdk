/* errno.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, 1999, 2001, 2003 Nick Burrett.  */

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

#define EINVAL 4 /* Invalid argument. */
#define ENOSYS 5 /* Invalid argument. */

#define __SYS_NERR 5

#endif
