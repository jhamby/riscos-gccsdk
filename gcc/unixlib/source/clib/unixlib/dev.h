/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/dev.h,v $
 * $Date: 2004/10/23 17:23:36 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifndef __UNIXLIB_DEV_H
#define __UNIXLIB_DEV_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __UNIXLIB_FD_H
#include <unixlib/fd.h>
#endif

#include <sys/select.h>
#include <sys/stat.h>

__BEGIN_DECLS


/* major device numbers */

#define DEV_RISCOS	0	/* RISC OS file system */
#define DEV_TTY		1	/* tty                 */
#define DEV_PIPE	2	/* UnixLib pipe()      */
#define DEV_NULL	3	/* /dev/null           */
#define DEV_SOCKET	4	/* Network socket      */
#define DEV_ZERO	5	/* /dev/zero           */
#define DEV_RANDOM	6	/* /dev/random         */
#define DEV_DSP		7	/* /dev/dsp            */

#define NDEV		8


struct dev
{
  void *(*open) (struct __unixlib_fd *__fd, const char *__fname, int __mode);
  int (*close) (struct __unixlib_fd *__fd);
  int (*read) (struct __unixlib_fd *__fd, void *__data, int __nbyte);
  int (*write) (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
  __off_t (*lseek) (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
  int (*ioctl) (struct __unixlib_fd *__fd, unsigned long __request,
		void *__arg);
  int (*select) (struct __unixlib_fd *__fd, int __fd1, fd_set *__read,
		fd_set *__write, fd_set *__except);
  int (*stat) (const char *filename, struct stat *buf);
  int (*fstat) (int fd, struct stat *buf);
};

/* Keep track of an fd_set of socket handles for use with select()  */
extern fd_set __socket_fd_set;

extern const struct dev __dev[NDEV];

extern const dev_t __getdevtype (const char *__filename);

/* RISC OS file system support */
extern void *__fsopen (struct __unixlib_fd *__fd, const char *__fname, int __mode);
extern int __fsclose (struct __unixlib_fd *__fd);
extern int __fsread (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __fswrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern __off_t __fslseek (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
extern int __fsstat (const char *__filename, struct stat *__buf);
extern int __fsfstat (int __fd, struct stat *__buf);

/* /dev/tty, /dev/console, /dev/rs423, /dev/ttyS0 support */
extern void *__ttyopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __ttyclose (struct __unixlib_fd *__fd);
extern int __ttyread (struct __unixlib_fd *__fd, void *buf, int __nbyte);
extern int __ttywrite (struct __unixlib_fd *__fd, const void *buf, int __nbyte);
extern int __ttyioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);
extern int __ttyselect (struct __unixlib_fd *__fdriptor, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);

#if __UNIXLIB_FEATURE_PIPEDEV
/* UnixLib pipe() support */
extern void *__pipeopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __pipeclose (struct __unixlib_fd *__fd);
extern int __piperead (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __pipewrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern int __pipeselect (struct __unixlib_fd *__fdriptor, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);
#endif

/* /dev/null support */
extern void *__nullopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __nullclose (struct __unixlib_fd *__fd);
extern int __nullread (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __nullwrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern __off_t __nulllseek (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
extern int __nullioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);
extern int __nullselect (struct __unixlib_fd *__fd, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);
extern int __nullstat (const char *filename, struct stat *buf);
extern int __nullfstat (int fd, struct stat *buf);

#if __UNIXLIB_FEATURE_SOCKET
/* socket support */
extern void *__sockopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __sockclose (struct __unixlib_fd *__fd);
extern int __sockread (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __sockwrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern int __sockioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);
extern int __sockselect (struct __unixlib_fd *__fdriptor, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);
#endif

/* /dev/zero support */
extern int __zeroread (struct __unixlib_fd *__file_desc, void *__data, int __nbyte);

/* /dev/random, /dev/urandom support */
extern void *__randomopen (struct __unixlib_fd *__file_desc, const char *__file, int __mode);
extern int __randomread (struct __unixlib_fd *__file_desc, void *__data, int __nbyte);

/* /dev/dsp support */
extern void *__dspopen (struct __unixlib_fd *__file_desc, const char *__file, int __mode);
extern int __dspclose (struct __unixlib_fd *__fd);
extern __off_t __dsplseek (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
extern int __dspioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);

__END_DECLS

#endif
