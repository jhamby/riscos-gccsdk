/* unistd.h.

   This header is not part of the normal SharedCLibrary but is
   provided here for compatibility with some awkwardly assuming
   programs.
   Copyright (c) 1999 Nick Burrett.  */

#ifndef __UNISTD_H
#define __UNISTD_H 1

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Close the file descriptor fd.  */
extern int close (int __fd);

/* Read nbytes into buf from fd.  */
extern __ssize_t read (int __fd, void *__buf, size_t __nbytes);

/* Write n bytes of buf to fd.  */
extern __ssize_t write (int __fd, const void *__buf, size_t __nbytes);

/* Seek to file position of fd.  */
__off_t lseek (int fd, __off_t offset, int whence);


#ifdef __cplusplus
}
#endif

#endif
