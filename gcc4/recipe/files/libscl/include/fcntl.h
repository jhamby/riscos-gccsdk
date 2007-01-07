/* fcntl.h extension header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef __FCNTL_H
#define __FCNTL_H

#ifndef __SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* File access modes for open() and fcntl()  */

/* These fcntlbits are derived from BSD 4.4 */

#define O_RDONLY 0 /* Open for read only.  */
#define O_WRONLY 1 /* Open for write only.  */
#define O_RDWR 2 /* Open for read/write.  */

/* Mask for file access modes.  */
#define	O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)

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

/* Strict POSIX doesn't allow this. */
#define O_BINARY	0x2000
#define O_TEXT		0x1000

#define O_PIPE		0x4000 /* UnixLib specific */
#define O_UNLINKED	0x8000 /* UnixLib specific - unlink file on close */



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
   The remaining arguments are interpreted depending on CMD.  */
extern int fcntl (int fd, int cmd, ...);

/* Open FILE and return a new file descriptor for it, or -1 on error.
   OFLAG determines the type of access used.  If O_CREAT is on OFLAG,
   the third argument is taken as a `mode_t', the mode of the created file.  */
extern int open (const char *file, int oflag, ...);

/* Create and open FILE, with mode MODE.
   This takes an `int' MODE argument because that is
   what `mode_t' will be widened to.  */
extern int creat (const char *file, __mode_t mode);

#ifdef __cplusplus
	}
#endif

#endif
