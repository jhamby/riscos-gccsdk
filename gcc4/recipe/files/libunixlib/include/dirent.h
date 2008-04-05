/*
 * POSIX Standard 5.1.2: Directory Operations <dirent.h>
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __DIRENT_H
#define __DIRENT_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_size_t
#include <stddef.h>

#include <sys/types.h>

__BEGIN_DECLS

#define MAXNAMLEN 255

#ifdef __USE_XOPEN
#ifndef __ino_t_defined
typedef __ino_t ino_t;
#define __ino_t_defined
#endif
#endif

/* This isn't really how ADFS stores files in a directory, but
   since no I/O is permitted on directories anyway this doesn't
   really matter.  */
struct dirent
{
  /* File number of entry.  */
  ino_t	d_fileno;

  /* Length of d_name.  */
  size_t d_namlen;

  /* File type, possibly unknown.  */
  unsigned char d_type;

  /* FIXME:  Should be a macro constant, but causing difficulties
     with namespace at the moment.  */
  char		d_name[256];	/* name */
};

/* For backwards compatibility with BSD.  */
#define d_ino	d_fileno

#ifdef __USE_BSD
/* BSD file types for d_type.  */
enum
  {
    DT_UNKNOWN = 0,
    DT_FIFO = 1,
    DT_CHR = 2,
    DT_DIR = 4,
    DT_BLK = 6,
    DT_REG = 8,
    DT_LNK = 10,
    DT_SOCK = 12
  };

/* BSD macros to convert between stat structure types and
   directory types.  */
#define IFTODT(mode) (((mode) & 0170000) >> 12)
#define DTTOIF(dirtype) ((dirtype) << 12)
#endif

typedef struct __dir_stream DIR;

/* Open a directory stream on name. Return a dir stream on
   the directory, or NULL if it could not be opened.

   This is a cancellation point.  */
extern DIR *opendir (const char *__name) __nonnull ((1)) __wur;

/* Read a directory entry from dirp.
   Return a pointer to a struct dirent describing the entry,
   or NULL for EOF or error.  The storage returned may be overwritten
   by a later readdir call on the same DIR stream, or by closedir.

   This is a cancellation point.  */
extern struct dirent *readdir (DIR *__dirp) __nonnull ((1)) __wur;

/* Reentrant version of readdir.  This is a cancellation point.  */
extern int readdir_r (DIR *__restrict __dirp,
		      struct dirent *__restrict __entry,
		      struct dirent **__restrict __result)
     __nonnull ((1, 2, 3));

#if defined __USE_BSD || defined __USE_MISC || defined __USE_XOPEN
/* Return the current position of dirp.  */
extern long int telldir (DIR *__dirp) __THROW __nonnull ((1)) __wur;

/* Seek to position pos on dirp.  */
extern void seekdir (DIR *__dirp, long int __pos) __THROW __nonnull ((1));
#endif

/* Rewind DIRP to the beginning of the directory.  */
extern void rewinddir (DIR *__dirp) __THROW __nonnull ((1));

/* Close the directory stream dirp. Return 0 if successful,
   -1 if not.

   This is a cancellation point.  */
extern int closedir (DIR *__dirp) __nonnull ((1));

#if defined __USE_BSD || defined __USE_MISC

/* Return the file descriptor used by DIRP.  */
extern int dirfd (DIR *__dirp) __THROW __nonnull ((1));

/* Function to compare two `struct dirent's alphabetically.  */
extern int alphasort (const struct dirent **__a,
		      const struct dirent ** __b)
     __THROW __attribute_pure__ __nonnull ((1, 2));

/* Scan the directory dir, calling 'select' on each directory entry.
   Entries for which 'select' returns nonzero are individually malloc'd,
   sorted using qsort with 'cmp', and collected in a malloc'd array in
   *namelist.  Returns the number of entries selected, or -1 on error.  */
extern int scandir (const char *__restrict __dir,
		    struct dirent ***__restrict __namelist,
		    int (*__select)(const struct dirent *),
		    int (*__cmp)(const struct dirent **,
				 const struct dirent **))
     __nonnull ((1, 2));
#endif

#if 0
/* Read directory entries from fd into buf, reading at most nbytes.
   Reading starts at offset *basep, and *basep is updated with the new
   position after reading.  Returns the number of bytes read; zero when at
   end of directory; or -1 for errors.  */
extern ssize_t getdirentries (int __fd, char *__restrict __buf,
			      size_t __nbytes,
			      __off_t *__restrict __basep)
     __THROW __nonnull ((2, 4));
#endif

__END_DECLS

#endif
