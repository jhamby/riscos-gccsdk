/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

/* POSIX Standard 5.1.2: Directory Operations <dirent.h> */

#ifndef __DIRENT_H
#define __DIRENT_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#define __need_size_t
#include <stddef.h>

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

/* This isn't really how ADFS stores files in a directory, but
   since no I/O is permitted on directories anyway this doesn't
   really matter.  */

struct dirent
{
  __ino_t	d_fileno;		/* file number of entry */
  size_t	d_namlen;		/* length of d_name */
  unsigned char d_type;			/* file type, possibly unknown */

  /* FIXME:  Should be a macro constant, but causing difficulties
     with namespace at the moment.  */
  char		d_name[256];	/* name */
};

/* For backwards compatibility with BSD.  */
#define d_ino	d_fileno

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

typedef struct __dir_stream DIR;

#ifdef __UNIXLIB_INTERNALS

/* Magic number to fill __magic.  */
#define _DIRMAGIC 0xf7b2bace

typedef struct
{
  __uint32_t load_address;
  __uint32_t exec_address;
  __uint32_t length;
  __uint32_t file_attrib;
  __uint32_t obj_type;
  const char obj_name[1];
} __os_gbpb_10;

struct __dir_stream
{
  unsigned int __magic; /* magic number to protect our streams */
  DIR *next;
  DIR *suffix; /* used when doing reverse suffix dir mapping.  */
  __off_t dd_suf_off; /* start offset used in suffix dir.  */

  char *dd_name_can; /* canonicalised name (riscos format) */

  __off_t dd_off; /* offset of next read (used for seeking) */
  __off_t gbpb_off; /* offset used for os_gbpb */
  __os_gbpb_10 *dir_cache; /* pointer to a cache of directory filenames */
  __os_gbpb_10 *dir_cache_index; /* index in that cache for the next file  */
  __u_int do_read; /* zero if we need to fill the directory cache */

  struct dirent dirent; /* last directory entry read using readdir */
};

#if __INTEGRITY_CHECK
/* Nonzero if stream is a valid stream.  */
#define __validdir(stream) (stream != NULL && stream->__magic == _DIRMAGIC)
#else
#define __validdir(stream) (1)
#endif

#endif /* __UNIXLIB_INTERNALS */

/* Open a directory stream on name. Return a dir stream on
   the directory, or NULL if it could not be opened.  */
extern DIR *opendir (const char *__name);

/* Read a directory entry from dirp.
   Return a pointer to a struct dirent describing the entry,
   or NULL for EOF or error.  The storage returned may be overwritten
   by a later readdir call on the same DIR stream, or by closedir.  */
extern struct dirent *readdir (DIR *__dirp);

/* Reentrant version of readdir.  */
extern int readdir_r (DIR *__dirp, struct dirent *__entry,
		      struct dirent **__result);

/* Return the current position of dirp.  */
extern __off_t telldir (DIR *__dirp);

/* Seek to position pos on dirp.  */
extern void seekdir (DIR *__dirp, __off_t __pos);

/* Rewind DIRP to the beginning of the directory.  */
extern void rewinddir (DIR *__dirp);

/* Close the directory stream dirp. Return 0 if successful,
   -1 if not.  */
extern int closedir (DIR *__dirp);

/* Function to compare two `struct dirent's alphabetically.  */
extern int alphasort (const struct dirent **__a, const struct dirent ** __b);

/* Scan the directory dir, calling 'select' on each directory entry.
   Entries for which 'select' returns nonzero are individually malloc'd,
   sorted using qsort with 'cmp', and collected in a malloc'd array in
   *namelist.  Returns the number of entries selected, or -1 on error.  */

extern int scandir (const char *__dir,
		    struct dirent ***__namelist,
		    int (*__select)(const struct dirent *),
		    int (*__cmp)(const struct dirent **,
				 const struct dirent **));

#if 0
/* Read directory entries from fd into buf, reading at most nbytes.
   Reading starts at offset *basep, and *basep is updated with the new
   position after reading.  Returns the number of bytes read; zero when at
   end of directory; or -1 for errors.  */
extern ssize_t getdirentries (int __fd, char *__buf,
			      size_t __nbytes, __off_t *__basep);
#endif

__END_DECLS

#endif
