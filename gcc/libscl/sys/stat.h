/* sys/stat.h extension header for the RISC OS SharedCLibrary.
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

#ifndef __SYS_STAT_H
#define __SYS_STAT_H

#ifdef __cplusplus
extern "C" {
#endif

struct stat
  {
  __dev_t 	st_dev; /* Device containing the file.  */
  __ino_t 	st_ino; /* File serial number.  */
  __mode_t 	st_mode; /* File mode.  */
  __nlink_t	st_nlink; /* Link count.  */
  __uid_t	st_uid; /* User ID of the file's owner.  */
  __gid_t	st_gid; /* Group ID of the file's group. */
  __dev_t 	st_rdev; /* Device number, if device.  */
  __off_t 	st_size; /* Size of file, in bytes.  */
  __time_t	st_atime; /* Time of last access.  */
  unsigned long int st_atime_usec;
  __time_t	st_mtime; /* Time of last modification.  */
  unsigned long int st_mtime_usec;
  __time_t	st_ctime; /* Time of last status change.  */
  unsigned long int st_ctime_usec;
  unsigned long int st_blksize; /* Optimal block size for I/O.  */
#define _STATBUF_ST_BLKSIZE /* Tell code we have this member. */
  unsigned long int st_blocks; /* Number of 512-byte blocks allocated.  */
  };

/* Bit masks.  */

/* Extract the file type code portion of a mode value.  */
#define S_IFMT		0770000

/* File type code for a FIFO or pipe.  */
#define S_IFIFO 	0010000
#define S_IFPORT	S_IFIFO
/* File type code for a terminal type device file.  */
#define S_IFCHR 	0020000
/* File type code for a directory. */
#define S_IFDIR 	0040000
/* File type code for a block-oriented device file (disk file).  */
#define S_IFBLK 	0100000
/* File type code for a regular file.  */
#define S_IFREG 	0200000
/* File type code for a symbolic link.  */
#define S_IFLNK 	0400000
/* This is the set-user-ID on execute bit.  */
#define S_ISUID 	0004000
/* This is the set-group-ID on execute bit.  */
#define S_ISGID 	0002000
/* Socket.  */
#define S_IFSOCK        0140000

/* Protection bits.  */

/* Save swapped text after use. */
#define S_ISVTX 	01000
/* Read by owner.  */
#define S_IREAD         0400
/* Write by owner.  */
#define S_IWRITE	0200
/* Execute by owner.  */
#define S_IEXEC		0100

/* Execute (for ordinary files) or search (for directories)
   permission bit for the owner of the file.  */
#define S_IXUSR		S_IEXEC
/* Write permission bit for the owner of the file.  */
#define S_IWUSR	        S_IWRITE
/* Read permission bit for the owner of the file.  */
#define S_IRUSR		S_IREAD

/* Read, write and execute by owner.  */
#define S_IRWXU (S_IREAD | S_IWRITE | S_IEXEC)

#define S_IRGRP	(S_IRUSR >> 3)	/* Read by group.  */
#define S_IWGRP	(S_IWUSR >> 3)	/* Write by group.  */
#define S_IXGRP	(S_IXUSR >> 3)	/* Execute by group.  */
/* Read, write, and execute by group.  */
#define S_IRWXG	(S_IRWXU >> 3)

#define S_IROTH	(S_IRGRP >> 3)	/* Read by others.  */
#define S_IWOTH	(S_IWGRP >> 3)	/* Write by others.  */
#define S_IXOTH	(S_IXGRP >> 3)	/* Execute by others.  */
/* Read, write, and execute by others.  */
#define S_IRWXO	(S_IRWXG >> 3)



/* Return nonzero if the file is a directory.  */
#define S_ISDIR(x) (((x) & S_IFSOCK) == S_IFDIR)
/* Return nonzero if the file is a terminal type device.  */
#define S_ISCHR(x) ((x) & S_IFCHR)
/* Return nonzero if the file is a block special file (like a disk). */
#define S_ISBLK(x) (((x) & S_IFSOCK) == S_IFBLK)
/* Return nonzero if the file is a regular file.  */
#define S_ISREG(x) ((x) & S_IFREG)
/* Return nonzero if the file is a FIFO file for pipe.  */
#define S_ISFIFO(x) ((x) & S_IFIFO)
/* Return nonzero if the file is a symbolic link.  */
#define S_ISLNK(x) ((x) & S_IFLNK)
/* Return nonzero if the file is a socket.  */
#define S_ISSOCK(x) (((x) & S_IFSOCK) == S_IFSOCK)

extern int stat (const char *__filename, struct stat *__buf);
extern int lstat (const char *__filename, struct stat *__buf);
extern int fstat (int __fd, struct stat *__buf);

/* Set file access permissions for file to mode.  */
extern int chmod (const char *file, __mode_t mode);

/* Set file access permissions of the file fd is open on to mode.  */
extern int fchmod (int fd, __mode_t mode);

/* Set the file creation mask of the current process to mask,
   return the old creation mask.  */
extern __mode_t umask (__mode_t mask);

/* Create a new directory named path, with permission bits mode.  */
extern int mkdir (const char *path, __mode_t mode);

#ifdef __cplusplus
	}
#endif

#endif
