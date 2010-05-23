/* UnixLib internal device implementation.
   Copyright (c) 2002-2010 UnixLib Developers.  */

#ifdef __TARGET_SCL__
#  error "SCL has no dev support"
#endif

#ifndef __INTERNAL_DEV_H
#define __INTERNAL_DEV_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __INTERNAL_FD_H
#include <internal/fd.h>
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
#define DEV_CUSTOM	8	/* Custom FD behaviour */

#define NDEV		9

/* Call a device function, ensuring we don't refer to an invalid device */
#define dev_funcall(type, func, args) __funcall ((*(__dev[(type) < NDEV ? (type) : DEV_NULL].func)), args)


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
  int (*lstat) (const char *filename, struct stat *buf);
};

/* Keep track of an fd_set of socket handles for use with select()  */
extern fd_set __socket_fd_set;

extern const struct dev __dev[NDEV];

extern dev_t __getdevtype (const char *__filename, int __ro_control);

/* RISC OS file system support */
extern void *__fsopen (struct __unixlib_fd *__fd, const char *__fname, int __mode);
extern int __fsclose (struct __unixlib_fd *__fd);
extern int __fsread (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __fswrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern __off_t __fslseek (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
extern int __fsstat (const char *__filename, struct stat *__buf);
extern int __fsfstat (int __fd, struct stat *__buf);
extern int __fslstat (const char *__filename, struct stat *__buf);

/* /dev/tty, /dev/console, /dev/rs423, /dev/ttyS0 support */
extern void *__ttyopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __ttyclose (struct __unixlib_fd *__fd);
extern int __ttyread (struct __unixlib_fd *__fd, void *buf, int __nbyte);
extern int __ttywrite (struct __unixlib_fd *__fd, const void *buf, int __nbyte);
extern int __ttyioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);
extern int __ttyselect (struct __unixlib_fd *__fdriptor, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);

/* UnixLib pipe() support */
extern void *__pipeopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __pipeclose (struct __unixlib_fd *__fd);
extern int __piperead (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __pipewrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern int __pipeselect (struct __unixlib_fd *__fdriptor, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);

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

/* socket support */
extern void *__sockopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __sockclose (struct __unixlib_fd *__fd);
extern int __sockread (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __sockwrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern int __sockioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);
extern int __sockselect (struct __unixlib_fd *__fdriptor, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);

/* /dev/zero support */
extern int __zeroread (struct __unixlib_fd *__file_desc, void *__data, int __nbyte);

/* /dev/random, /dev/urandom support */
extern void *__randomopen (struct __unixlib_fd *__file_desc, const char *__file, int __mode);
extern int __randomread (struct __unixlib_fd *__file_desc, void *__data, int __nbyte);

/* /dev/dsp support */
extern void *__dspopen (struct __unixlib_fd *__file_desc, const char *__file, int __mode);
extern int __dspclose (struct __unixlib_fd *__fd);
extern int __dspwrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern __off_t __dsplseek (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
extern int __dspioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);

/* /dev/custom support */
struct dev *__unixlib_getdev(int fd);

extern void *__customopen (struct __unixlib_fd *__fd, const char *file, int __mode);
extern int __customclose (struct __unixlib_fd *__fd);
extern int __customread (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __customwrite (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
extern __off_t __customlseek (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
extern int __customioctl (struct __unixlib_fd *__fd, unsigned long __request, void *__arg);
extern int __customselect (struct __unixlib_fd *__fd, int __fd1, fd_set *__read, fd_set *__write, fd_set *__except);
extern int __customstat (const char *filename, struct stat *buf);
extern int __customfstat (int fd, struct stat *buf);
extern int __customlstat (const char *filename, struct stat *buf);

__END_DECLS

#endif
