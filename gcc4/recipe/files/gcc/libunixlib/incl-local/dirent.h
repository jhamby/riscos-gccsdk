/* Internal UnixLib dirent.h
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __DIRENT_H
#include_next <dirent.h>
#endif

#if !defined(__INTERNAL_DIRENT_H) && defined(__DIRENT_H)
#define __INTERNAL_DIRENT_H

__BEGIN_DECLS

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

  int fd; /* directory file descriptor.  */

  struct dirent dirent; /* last directory entry read using readdir */
};

/* Nonzero if stream is a valid stream.  */
#define __validdir(stream) ((stream) != NULL && (stream)->__magic == _DIRMAGIC)

__END_DECLS

#endif
