/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/statfs.h,v $
 * $Date: 2002/09/24 21:02:37 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Definitions for getting information about a filesystem.  */

#ifndef	__SYS_STATFS_H
#define	__SYS_STATFS_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

__BEGIN_DECLS

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
extern int statfs (const char *file, struct statfs *buf) __THROW;

/* Return information about the filesystem containing the file fd
   refers to.  */
extern int fstatfs (int fd, struct statfs *buf) __THROW;

__END_DECLS

#endif
