/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifndef __UNIXLIB_DEV_H
#define __UNIXLIB_DEV_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
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
#define DEV_TTY		1	/* tty */
#define DEV_PIPE	2	/* UnixLib pipe() */
#define DEV_NULL	3	/* /dev/null */
#define DEV_SOCKET	4	/* Freenet socket */
#define DEV_ZERO	5	/* /dev/zero */
#define DEV_RANDOM	6	/* /dev/random */

#define NDEV		7


struct dev
{
  void *(*open) (struct __unixlib_fd *__fd, const char *__fname, int __mode);
  int (*close) (struct __unixlib_fd *__fd);
  int (*read) (struct __unixlib_fd *__fd, void *__data, int __nbyte);
  int (*write) (struct __unixlib_fd *__fd, const void *__data, int __nbyte);
  __off_t (*lseek) (struct __unixlib_fd *__fd, __off_t __lpos, int __whence);
  int (*ioctl) (struct __unixlib_fd *__fd, int __request, void *__arg);
  int (*select)(struct __unixlib_fd *__fd, int __fd1, fd_set *__read,
		fd_set *__write, fd_set *__except);
  int (*stat) (const char *filename, struct stat *buf);
  int (*fstat) (int fd, struct stat *buf);
};

extern struct dev __dev[NDEV];

extern const dev_t __getdevtype (const char *__filename);

extern void *__fsopen (struct __unixlib_fd *__fd, const char *__fname,
		       int __mode);
extern int __fsclose (struct __unixlib_fd *__fd);
extern int __fsread (struct __unixlib_fd *__fd, void *__data, int __nbyte);
extern int __fswrite (struct __unixlib_fd *__fd, const void *__data,
		      int __nbyte);
extern __off_t __fslseek (struct __unixlib_fd *__fd, __off_t __lpos,
			  int __whence);
#ifdef __UNIXLIB_NO_COMMON_DEV
extern int __fsioctl (struct __unixlib_fd *__fd, int __request, void *__arg);
extern int __fsselect (struct __unixlib_fd *__fd, int __fd1,
		       fd_set *__read, fd_set *__write,
		       fd_set *__except);
#endif
extern int __fsstat (const char *__filename, struct stat *__buf);
extern int __fsfstat (int __fd, struct stat *__buf);

extern void *__ttyopen	     (struct __unixlib_fd *__fd,
			      const char *file, int __mode);
extern int __ttyclose	     (struct __unixlib_fd *__fd);
extern int __ttyread	     (struct __unixlib_fd *__fd,
			      void *buf, int __nbyte);
extern int __ttywrite	     (struct __unixlib_fd *__fd,
			      const void *buf, int __nbyte);
#ifdef __UNIXLIB_NO_COMMON_DEV
extern __off_t __ttylseek    (struct __unixlib_fd *__fd,
			      __off_t __lpos, int __whence);
#endif
extern int __ttyioctl	     (struct __unixlib_fd *__fd,
			      int __request, void *__arg);
extern int __ttyselect	     (struct __unixlib_fd *__fdriptor,
			      int __fd1, fd_set *__read,
			      fd_set *__write, fd_set *__except);

extern void *__pipeopen	     (struct __unixlib_fd *__fd,
			      const char *file, int __mode);
extern int __pipeclose	     (struct __unixlib_fd *__fd);
extern int __piperead	     (struct __unixlib_fd *__fd,
			      void *__data, int __nbyte);
extern int __pipewrite	     (struct __unixlib_fd *__fd,
			      const void *__data, int __nbyte);
#ifdef __UNIXLIB_NO_COMMON_DEV
extern __off_t __pipelseek   (struct __unixlib_fd *__fd,
			      __off_t __lpos, int __whence);
extern int __pipeioctl	     (struct __unixlib_fd *__fd,
			      int __request, void *__arg);
#endif
extern int __pipeselect	     (struct __unixlib_fd *__fdriptor,
			      int __fd1, fd_set *__read,
			      fd_set *__write, fd_set *__except);

extern void *__nullopen	     (struct __unixlib_fd *__fd,
			      const char *file, int __mode);
extern int __nullclose	     (struct __unixlib_fd *__fd);
extern int __nullread	     (struct __unixlib_fd *__fd,
			      void *__data, int __nbyte);
extern int __nullwrite	     (struct __unixlib_fd *__fd,
			      const void *__data, int __nbyte);
#ifdef __UNIXLIB_NO_COMMON_DEV
extern __off_t __nulllseek   (struct __unixlib_fd *__fd,
			      __off_t __lpos, int __whence);
extern int __nullioctl	     (struct __unixlib_fd *__fd,
			      int __request, void *__arg);
extern int __nullselect	     (struct __unixlib_fd *__fdriptor,
			      int __fd1, fd_set *__read,
			      fd_set *__write, fd_set *__except);
#endif
extern int __nullstat (const char *__filename, struct stat *__buf);
extern int __nullfstat (int __fd, struct stat *__buf);

extern void *__sockopen	     (struct __unixlib_fd *__fd,
			      const char *file, int __mode);
extern int __sockclose	     (struct __unixlib_fd *__fd);
extern int __sockread	     (struct __unixlib_fd *__fd,
			      void *__data, int __nbyte);
extern int __sockwrite	     (struct __unixlib_fd *__fd,
			      const void *__data, int __nbyte);
#ifdef __UNIXLIB_NO_COMMON_DEV
extern __off_t __socklseek   (struct __unixlib_fd *__fd,
			      __off_t __lpos, int __whence);
#endif
extern int __sockioctl	     (struct __unixlib_fd *__fd,
			      int __request, void *__arg);
extern int __sockselect	     (struct __unixlib_fd *__fdriptor,
			      int __fd1, fd_set *__read,
			      fd_set *__write, fd_set *__except);

#ifndef __UNIXLIB_NO_COMMON_DEV
/* Return ESPIPE */
extern __off_t __commonlseek (struct __unixlib_fd *__fd,
			      __off_t __lpos, int __whence);
/* Return EINVAL */
extern int __commonioctl     (struct __unixlib_fd *__fd,
			      int __request, void *__arg);
/* Return ready to read, ready to write, no execptional conditions */
extern int __commonselect    (struct __unixlib_fd *__fdriptor,
			      int __fd, fd_set *__read,
			      fd_set *__write, fd_set *__except);
#endif

extern int __zeroread (struct __unixlib_fd *__file_desc, void *__data, int __nbyte);

extern void *__randomopen (struct __unixlib_fd *__file_desc, const char *__file, int __mode);
extern int __randomread (struct __unixlib_fd *__file_desc, void *__data, int __nbyte);

__END_DECLS

#endif
