/*
 * POSIX Standard 5.6: File Characteristics <sys/stat.h>.
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#ifndef __SYS_STAT_H
#define __SYS_STAT_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_time_t
#include <time.h>

/* The Single Unix specification says that some more types are
   available here.  */
#ifndef __dev_t_defined
typedef __dev_t dev_t;
#define __dev_t_defined
#endif

#ifndef __gid_t_defined
typedef __gid_t gid_t;
#define __gid_t_defined
#endif

#ifndef __ino_t_defined
typedef __ino_t ino_t;
#define __ino_t_defined
#endif

#ifndef __mode_t_defined
typedef __mode_t mode_t;
#define __mode_t_defined
#endif

#ifndef __nlink_t_defined
typedef __nlink_t nlink_t;
#define __nlink_t_defined
#endif

#ifndef __off_t_defined
typedef __off_t off_t;
#define __off_t_defined
#endif

#ifndef __uid_t_defined
typedef __uid_t uid_t;
#define __uid_t_defined
#endif

#ifndef __blkcnt_t_defined
typedef __blkcnt_t blkcnt_t;
#define __blkcnt_t_defined
#endif

#ifndef __blksize_t_defined
typedef __blksize_t blksize_t;
#define __blksize_t_defined
#endif

__BEGIN_DECLS

/* Currently struct stat/stat64 are the same.  */
#define __DEFINE_STAT(stattype) \
  struct stattype \
  { \
    __dev_t 	st_dev;			/* Device containing the file.  */ \
    __ino_t 	st_ino;			/* File serial number.  */ \
    __mode_t 	st_mode;		/* File mode.  */ \
    __nlink_t	st_nlink;		/* Link count.  */ \
    __uid_t	st_uid;			/* User ID of the file's owner.  */ \
    __gid_t	st_gid;			/* Group ID of the file's group. */ \
    __dev_t 	st_rdev;		/* Device number, if device.  */ \
    __off_t 	st_size;		/* Size of file, in bytes.  */ \
    __time_t	st_atime;		/* Time of last access.  */ \
    unsigned long int st_atime_usec; \
    __time_t	st_mtime;		/* Time of last modification.  */ \
    unsigned long int st_mtime_usec; \
    __time_t	st_ctime;		/* Time of last status change.  */ \
    unsigned long int st_ctime_usec; \
    __blksize_t	st_blksize;		/* Optimal block size for I/O.  */ \
  /*  unsigned long int st_nblocks; / * Number of 512-byte blocks allocated.  */ \
    __blkcnt_t	st_blocks;		/* Number of 512-byte blocks allocated.  */ \
  }
#define _STATBUF_ST_BLKSIZE /* Tell code we have this stat member. */

__DEFINE_STAT(stat);
__DEFINE_STAT(stat64);

/* Bit masks.  */

/* Extract the file type code portion of a mode value.  */
#define S_IFMT		0170000

/* File type code for a FIFO or pipe.  */
#define S_IFIFO 	0010000
#define S_IFPORT	S_IFIFO
/* File type code for a terminal type device file.  */
#define S_IFCHR 	0020000
/* File type code for a directory. */
#define S_IFDIR 	0040000
/* File type code for a block-oriented device file (disk file).  */
#define S_IFBLK 	0060000
/* File type code for a regular file.  */
#define S_IFREG 	0100000
/* File type code for a symbolic link.  */
#define S_IFLNK 	0120000
/* File type code for a socket.  */
#define S_IFSOCK        0140000

/* Protection bits.  */

/* This is the set-user-ID on execute bit.  */
#define S_ISUID 	04000
/* This is the set-group-ID on execute bit.  */
#define S_ISGID 	02000
/* Save swapped text after use (sticky). */
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
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
/* Return nonzero if the file is a terminal type device.  */
#define S_ISCHR(x) (((x) & S_IFMT) == S_IFCHR)
/* Return nonzero if the file is a block special file (like a disk). */
#define S_ISBLK(x) (((x) & S_IFMT) == S_IFBLK)
/* Return nonzero if the file is a regular file.  */
#define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
/* Return nonzero if the file is a FIFO file for pipe.  */
#define S_ISFIFO(x) (((x) & S_IFMT) == S_IFIFO)
/* Return nonzero if the file is a symbolic link.  */
#define S_ISLNK(x) (((x) & S_IFMT) == S_IFLNK)
/* Return nonzero if the file is a socket.  */
#define S_ISSOCK(x) (((x) & S_IFMT) == S_IFSOCK)

#ifndef __USE_FILE_OFFSET64
extern int stat (const char *__restrict __filename,
		 struct stat *__restrict __buf)
     __THROW __nonnull ((1, 2));

extern int fstat (int __fd, struct stat *__buf)
     __THROW __nonnull ((2));
#else
# ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH (stat, (__const char *__restrict __file,
                                  struct stat *__restrict __buf), stat64)
     __nonnull ((1, 2));
extern int __REDIRECT_NTH (fstat, (int __fd, struct stat *__buf), fstat64)
     __nonnull ((2));
# else
#  define stat stat64
#  define fstat fstat64
# endif
#endif
#ifdef __USE_LARGEFILE64
extern int stat64 (const char *__restrict __filename,
		   struct stat64 *__restrict __buf)
     __THROW __nonnull ((1, 2));
extern int fstat64 (int __fd, struct stat64 *__buf)
     __THROW __nonnull ((2));
#endif

#if defined __USE_BSD || defined __USE_XOPEN_EXTENDED
# ifndef __USE_FILE_OFFSET64
extern int lstat (const char *__filename, struct stat *__buf)
     __THROW __nonnull ((1, 2));
# else
#  ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH (lstat,
                           (__const char *__restrict __file,
                            struct stat *__restrict __buf), lstat64)
     __nonnull ((1, 2));
#  else
#   define lstat lstat64
#  endif
# endif
# ifdef __USE_LARGEFILE64
extern int lstat64 (const char *__filename,
		    struct stat64 *__buf) __THROW __nonnull ((1, 2));
# endif
#endif

/* Set file access permissions for file to mode.  */
extern int chmod (const char *__file, __mode_t __mode)
     __THROW __nonnull ((1));

#if defined __USE_BSD || defined __USE_XOPEN_EXTENDED
/* Set file access permissions of the file fd is open on to mode.  */
extern int fchmod (int __fd, __mode_t __mode) __THROW;
#endif

/* Set the file creation mask of the current process to mask,
   return the old creation mask.  */
extern __mode_t umask (__mode_t __mask) __THROW;

/* Create a new directory named path, with permission bits mode.  */
extern int mkdir (const char *__path, __mode_t __mode)
     __THROW __nonnull ((1));

#if defined __USE_MISC || defined __USE_BSD || defined __USE_XOPEN_EXTENDED
/* Create a device file named path, with permission and special bits mode
   and device number dev.  */
extern int mknod (const char *__path, __mode_t __mode, __dev_t __dev)
     __THROW __nonnull ((1));
#endif

/* Create a new FIFO named path, with permission bits mode.  */
extern int mkfifo (const char *__path, __mode_t __mode)
     __THROW __nonnull ((1));

__END_DECLS

#endif
