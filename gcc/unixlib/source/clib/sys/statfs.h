/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/statfs.h,v $
 * $Date: 2000/07/15 14:52:16 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* Definitions for getting information about a filesystem.  */

#ifndef	__SYS_STATFS_H
#define	__SYS_STATFS_H 1

#ifdef __cplusplus
extern "C" {
#endif

struct statfs
{
  unsigned int f_type;
  unsigned int f_bsize;
  unsigned int f_blocks;
  unsigned int f_bfree;
  unsigned int f_bavail;
  unsigned int f_files;
  unsigned int f_ffree;
  __fsid_t f_fsid;
  unsigned int f_namelen;
  unsigned int f_spare[6];
};

/* Return information about the filesystem on which FILE resides.  */
extern int statfs (const char *file, struct statfs *buf);

/* Return information about the filesystem containing the file fd
   refers to.  */
extern int fstatfs (int fd, struct statfs *buf);

#ifdef __cplusplus
}
#endif

#endif
