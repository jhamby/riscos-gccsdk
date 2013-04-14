/* __stat () : common stat operation.
   Copyright (c) 2002-2013 UnixLib Developers.  */

#include <sys/stat.h>

#include <unixlib/types.h>
#include <internal/local.h>
#ifndef __TARGET_SCL__
#  include <internal/dev.h>
#endif

/* struct stat's st_dev is filled in at stat(), fstat() and lstat() time.
   struct stat's st_ino is filled in by the __stat callers in dev.c.  */
int
__stat (unsigned objtype, unsigned loadaddr, unsigned execaddr,
        unsigned length, unsigned attr, struct stat *buf)
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
      /* FIXME: What about DEV_DSP, DEV_CUSTOM ? */
      case DEV_TTY:
      case DEV_NULL:
      case DEV_ZERO:
      case DEV_RANDOM:
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

  if ((loadaddr & 0xFFF00000) == 0xFFF00000)	
    {
      /* Date stamped file.  */
      __int64_t ftime = __cvt_riscos_time_csec (((__int64_t)(loadaddr & 0xff) << 32)
						+ execaddr);
      buf->st_atim.tv_sec = ftime / 100;
      buf->st_atim.tv_nsec = ftime % 100;
      if (buf->st_atim.tv_nsec < 0)
	{
	  buf->st_atim.tv_nsec = 100 + buf->st_atim.tv_nsec;
	  buf->st_atim.tv_sec -= 1;
	}
      buf->st_atim.tv_nsec *= 10000000;
    }
  else
    {
      /* Load/exec file.  */
      buf->st_atim.tv_sec = 0;
      buf->st_atim.tv_nsec = 0;
    }
  buf->st_ctim = buf->st_mtim = buf->st_atim;

  return 0;
}
