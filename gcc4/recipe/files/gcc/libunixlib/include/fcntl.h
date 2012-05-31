/*
 * POSIX Standard 6.5: File Control Operations <fcntl.h>
 * Copyright (c) 2000-2012 UnixLib Developers
 */

#ifndef __FCNTL_H
#define __FCNTL_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __USE_XOPEN
#include <sys/stat.h>
#endif

__BEGIN_DECLS

/* File access modes for open() and fcntl()  */

#if 0
/* These fcntlbits are derived from POSIX.1  */

#define O_OMASK 3
#define O_RDONLY 0x0001
#define O_WRONLY 0x0002
#define O_RDWR (O_RDONLY | O_WRONLY)
#define	O_ACCMODE O_RDWR

/* Open for exec.  */
#define O_EXEC 0x0004
#define O_READ O_RDONLY
#define O_WRITE O_WRONLY

#else
/* These fcntlbits are derived from BSD 4.4 */

#define O_RDONLY 0 /* Open for read only.  */
#define O_WRONLY 1 /* Open for write only.  */
#define O_RDWR 2 /* Open for read/write.  */

/* Mask for file access modes.  */
#define	O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)

#endif

/* Bits OR'd into the second argument to open.  */

/* Create file if it doesn't exist.  */
#define O_CREAT 	0x0200
/* Fail if file already exists.  */
#define O_EXCL		0x0800
/* Truncate file to zero length.  */
#define O_TRUNC	    	0x0400
/* Send SIGIO to owner when data is ready.  */
#define O_ASYNC		0x0040
/* Synchronous writes.  */
#define O_FSYNC		0x0080
#define O_SYNC		O_FSYNC
/* Open with shared file lock.  */
#define O_SHLOCK	0x0010
/* Open with shared exclusive lock.  */
#define O_EXLOCK	0x0020

/* File status flags for open and fcntl.  */

/* Writes append to the file.  */
#define O_APPEND	0x0008
/* Non-blocking I/O.  */
#define O_NONBLOCK      0x0004
#define O_NDELAY	O_NONBLOCK

/* close on exec() flag - must be bit 8 */
#define O_EXECCL	0x0100

#ifndef _POSIX_SOURCE
/* Strict POSIX doesn't allow this and UnixLib won't do anything special
   when set.  */
#define O_BINARY	0
#define O_TEXT		0
#endif

/* Bits 12-15 (incl.) are reserved.  */

/* Don't make terminal device controlling terminal */
#define O_NOCTTY       0x10000

/* Large file flag - not fully supported but needed for some libraries */
#define O_LARGEFILE 0x20000


/* Duplicate file descriptor.  */
#define F_DUPFD 	0
/* Return file descriptor flags.  */
#define F_GETFD 	1
/* Set file descriptor flags.  */
#define F_SETFD 	2
/* Read file status flags.  */
#define F_GETFL 	3
/* Set file status flags.  */
#define F_SETFL 	4
/* Get owner (receiver of SIGIO).  */
#define F_GETOWN        5
/* Set owner (receiver of SIGIO).  */
#define F_SETOWN        6
/* Get record locking info.  */
#define F_GETLK	        7
/* Set record locking info (non-blocking).  */
#define F_SETLK		8
/* Set record locking info (blocking).  */
#define F_SETLKW	9
/* Get unlink on close (UnixLib internal - currently implements tmpfile). */
#define F_GETUNL	10
/* Set unlink on close (could be used to emulate open() then unlink trick). */
#define F_SETUNL	11

/* Bits in the file status flags returned by F_GETFL.  */
#define FREAD		1
#define	FWRITE		2

/* Traditional BSD names the O_* bits.  */
#define FASYNC		O_ASYNC
#define FCREAT		O_CREAT
#define FEXCL		O_EXCL
#define FTRUNC		O_TRUNC
#define FNOCTTY		O_NOCTTY
#define FFSYNC		O_FSYNC
#define FSYNC		O_SYNC
#define FAPPEND		O_APPEND
#define FNONBLOCK	O_NONBLOCK
#define FNDELAY		O_NDELAY


/* If set, cause the file descriptor to be closed if an exec function
   is used.  Initially, set clear.  */
#define FD_CLOEXEC	O_EXECCL

/* The structure describing an advisory lock.  This is the type of the third
   argument to `fcntl' for the F_GETLK, F_SETLK, and F_SETLKW requests.  */
struct flock
  {
    /* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.  */
    short int l_type;
    /* Where `l_start' is relative to (like `lseek').  */
    short int l_whence;
    /* Offset where the lock begins.  */
    __off_t l_start;
    /* Size of the locked area; zero means until EOF.  */
    __off_t l_len;
    /* Process holding the lock.  */
    short int l_pid;
  };

/* Values for the `l_type' field of a `struct flock'.  */
#define	F_RDLCK	1	/* Read lock.  */
#define	F_WRLCK	2	/* Write lock.  */
#define	F_UNLCK	3	/* Remove lock.  */


#ifndef	R_OK
/* Straight from <unistd.h>.  */

/* Values for the second argument to access.
   These may be OR'd together.  */

/* Test for read permission.  */
#define	R_OK	4
/* Test for write permission.  */
#define	W_OK	2
/* Test for execute permission.  */
#define	X_OK	1
/* Test for existence.  */
#define	F_OK	0
#endif


/* Do the file control operation described by CMD on FD.
   The remaining arguments are interpreted depending on CMD.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int fcntl (int __fd, int __cmd, ...);

/* Open FILE and return a new file descriptor for it, or -1 on error.
   OFLAG determines the type of access used.  If O_CREAT is on OFLAG,
   the third argument is taken as a `mode_t', the mode of the created file.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
#ifndef __USE_FILE_OFFSET64
extern int open (__const char *__file, int __oflag, ...) __nonnull ((1));
#else
# ifdef __REDIRECT
extern int __REDIRECT (open, (__const char *__file, int __oflag, ...), open64)
     __nonnull ((1));
# else
#  define open open64
# endif
#endif
#ifdef __USE_LARGEFILE64
extern int open64 (__const char *__file, int __oflag, ...) __nonnull ((1));
#endif

/* Create and open FILE, with mode MODE.  This takes an `int' MODE
   argument because that is what `mode_t' will be widened to.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
#ifndef __USE_FILE_OFFSET64
extern int creat (__const char *__file, __mode_t __mode) __nonnull ((1));
#else
# ifdef __REDIRECT
extern int __REDIRECT (creat, (__const char *__file, __mode_t __mode),
		       creat64) __nonnull ((1));
# else
#  define creat creat64
# endif
#endif
#ifdef __USE_LARGEFILE64
extern int creat64 (__const char *__file, __mode_t __mode) __nonnull ((1));
#endif

#if !defined F_LOCK && (defined __USE_MISC || (defined __USE_XOPEN_EXTENDED \
					       && !defined __USE_POSIX))
/* NOTE: These declarations also appear in <unistd.h>; be sure to keep both
   files consistent.  Some systems have them there and some here, and some
   software depends on the macros being defined without including both.  */

/* `lockf' is a simpler interface to the locking facilities of `fcntl'.
   LEN is always relative to the current file position.
   The CMD argument is one of the following.  */

# define F_ULOCK 0	/* Unlock a previously locked region.  */
# define F_LOCK  1	/* Lock a region for exclusive use.  */
# define F_TLOCK 2	/* Test and lock a region for exclusive use.  */
# define F_TEST  3	/* Test a region for other processes locks.  */

# ifndef __USE_FILE_OFFSET64
extern int lockf (int __fd, int __cmd, __off_t __len);
# else
#  ifdef __REDIRECT
extern int __REDIRECT (lockf, (int __fd, int __cmd, __off64_t __len), lockf64);
#  else
#   define lockf lockf64
#  endif
# endif
# ifdef __USE_LARGEFILE64
extern int lockf64 (int __fd, int __cmd, __off64_t __len);
# endif
#endif

__END_DECLS

#endif
