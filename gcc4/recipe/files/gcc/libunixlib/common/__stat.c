/* Common stat operation.
   Copyright (c) 2002-2012 UnixLib Developers.  */

#include <time.h>
#include <sys/stat.h>

#include <unixlib/types.h>
#include <internal/local.h>
#ifndef __TARGET_SCL__
#  include <internal/dev.h>
#endif

/* struct stat's st_dev is filled in at stat(), fstat() and lstat() time.
   struct stat's st_ino is filled in by the __stat callers in dev.c.  */
int
__stat (int objtype, int loadaddr, int execaddr, int length, int attr, struct stat *buf)
{
  /* Calculate the file mode.  */
  __mode_t mode = __get_protection (attr);

  switch (objtype)
    {
      case 1: /* Regular file.  */
	/* The number of hard links to the file. For RISC OS, there is only
	   one hard link since only one directory can point to the file.
	   Symbolic links are not counted in the total.  */
	buf->st_nlink = 1;
	/* It is a regular file.  */
	mode |= S_IFREG;
	/* ASR &FFFtttDD maps to FFFFFttt
	   all filetypes are treated as +x except "text" and "data" */
	switch (loadaddr >> 8)
	  {
	    case (int)0xfffffFFF:	/* Text. */
	    case (int)0xfffffFFD:	/* Data. */
	      break;
	    default:
	      /* set any X bits from any set R bits, by masking R bits and then
		 shifting.  */
	      mode |= (mode & (S_IRUSR | S_IRGRP | S_IROTH)) >> 2;
	  }
	break;

      case 2: /* Normal directory.  */
	/* Real Unix directories never contain less than 2 links, so fudge it.
	   This fudge makes RISC OS Perl behave like Amiga Perl.  */
	buf->st_nlink = 2;
	/* Get round a filing system bug, set the attributes for
	   owner read, write and exec.  */
	mode |= S_IFDIR | S_IRWXU;
	/* Directory size is normally 2048 bytes, but directories in image
	   filing systems have unreliable sizes. For example, ArcFS directories
	   are normal 2*32-1 (or -1) bytes, SparkFS typically 0 and zip files
	   the compressed size of the directory tree under the directory.
	   Now, GNU diff uses the returned size as the initial estimate for
	   a buffer to hold all the filenames, so it is better to return an
	   underestimate of the size. We choose an arbitrary maximum size
	   of 32,768 and set the actual size to 0.  */
	if ((size_t) length > 32768)
	  length = 0;
	break;

      case 3: /* Image directory (RISC OS 3 and above).  */
	/* Get round a filing system bug (as above).  */
	mode |= S_IRWXU;
#ifndef __TARGET_SCL__
        if (__get_feature_imagefs_is_file ())
	  {
	    mode |= S_IFREG;
	    /* Who said that we were a real Unix dir?  */
	    buf->st_nlink = 1;
	  }
        else
#endif
          {
	    mode |= S_IFDIR;
	    buf->st_nlink = 2;
	    /* Fudge directory size (as above).  */
	    if ((size_t) length > 32768)
	      length = 0;
	  }
	break;
    }

#ifndef __TARGET_SCL__
  switch (buf->st_dev)
    {
      case DEV_TTY:
	/* Terminal type device file.  */
	mode |= S_IFCHR;
	break;
      case DEV_PIPE:
	/* Pipe or a FIFO port.  */
	mode |= S_IFIFO;
	break;
      case DEV_SOCKET:
	/* Socket.  */
	mode |= S_IFSOCK;
	break;
    }
#endif

  buf->st_mode = mode;

  /* User ID of the file's owner.  */
  buf->st_uid = 1;
  /* Group ID of the file's group.  */
  buf->st_gid = 1;
  /* If the descriptor is a device, the device number.  */
  buf->st_rdev = buf->st_dev;
  /* Size of file, in bytes.  */
  buf->st_size = length;
  /* Disc space that the file occupies, measured in units of 512
     byte blocks.  */
  buf->st_blocks = ((unsigned long int)length + 511) / 512;
  /* Optimal block size for reading or writing this file.  Programs
     usually use as a suitable parameter for setvbuf.  */
  buf->st_blksize = 4096;

  if ((((unsigned int) loadaddr) >> 20) == 0xfff)	/* date stamped file */
    {
      time_t filetime = __cvt_riscos_time ((unsigned int)(loadaddr & 0xff),
					   (unsigned int)execaddr);

      buf->st_atime = buf->st_mtime = buf->st_ctime = filetime;
    }
  else
    buf->st_atime = buf->st_mtime = buf->st_ctime = 0;

  /* Fractional parts of the file access times. Not supported on RISC OS.  */
  buf->st_atime_usec = buf->st_mtime_usec = buf->st_ctime_usec = 0;

  return 0;
}
