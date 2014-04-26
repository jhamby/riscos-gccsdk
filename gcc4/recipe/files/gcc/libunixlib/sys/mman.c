/* Map or unmap memory.
   Copyright (c) 1996-2014 UnixLib Developers.  */

/* Definitions for BSD-style memory management.  Generic/4.4 BSD version.  */
/* sys/mman.c: Written by Peter Burwood, 1 November 1996, June 1997  */

/* Add support for mapping to files.
   Start with /dev/zero - needs support in unix/dev.c, unix/open.c & elsewhere.
   Then read support for real files (what happens when file is changed, need to
   remap the memory ?).
   Then support for writes - need to synchronise with buffered data
*/

#include <sys/mman.h>
#include <sys/types.h>
#include <swis.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <internal/os.h>
#include <internal/unix.h>
#include <internal/swiparams.h>
#include <pthread.h>


/* could store the number and the next pointer at the beginning or end of the
   mmap'ed area. The end is better for alignment purposes - so also need length
   maybe not a good idea, since len request is likely to be a multiple of 1024.

   Since will only allow a fixed maximum number of mmap'ed regions, declare an
   array.  Requests are likely to be a power of 2 when called from malloc which
   will request an integral number of pages. It will already have allocated a
   word for its length field, so if we stored extra information ourselves we
   would need an extra page. If malloc was called with a request for an
   integral number of pages then we would waste two pages. */
typedef struct mmap_info {
  caddr_t   addr;
  int	    number;
  size_t    len;
  int	    prot;
  int       flags;
  int       fd;
  off_t     offset;
} mmap_info;

#define IMMAP { 0, -1, 0, 0, 0, -1, 0 }
#define MAX_MMAPS 8
static mmap_info mmaps[MAX_MMAPS] = {
  IMMAP, IMMAP, IMMAP, IMMAP, IMMAP, IMMAP, IMMAP, IMMAP
};

#define page_align(gbl, len) \
  (((len) + (gbl)->pagesize - 1) & ~((gbl)->pagesize - 1))

extern void __mmap_page_copy (caddr_t dst, caddr_t src, int len);

static char coredump_dir[256];

void
__init_coredump (const char *dir)
{
  size_t dirlen = strlen (dir);

  /* Allow at least 16 bytes for the coredump filenames inside user supplied
     directory.  */
  if (dirlen < sizeof (coredump_dir)-16)
    strcpy (coredump_dir, dir);
}

const _kernel_oserror *
__unixlib_write_coredump (const char *dir)
{
  if (dir != NULL)
    __init_coredump (dir);

  size_t dirlen = strlen (coredump_dir);
  if (dirlen == 0)
    return NULL;

  /* Remove ending dot if there is one.  */
  if (coredump_dir[dirlen - 1] == '.')
    coredump_dir[--dirlen] = '\0';
  
  /* Create the coredump directory, with default number of entries.  */
  const _kernel_oserror *roerr;
  if ((roerr = SWI_OS_File_CreateDirectory (coredump_dir)) != NULL)
    return roerr;

  /* Write application space data.  */
  strcpy (coredump_dir + dirlen, ".app");
  const struct ul_memory *mem = &__ul_memory;
  roerr = SWI_OS_File_SaveBlockLoadExec (coredump_dir, 0x8000 /* load */,
					 mem->rwlomem /* exec */, 0x8000 /* start */,
					 mem->appspace_limit /* end */);

  /* Write main DA block (if there is one).  */
  if (__ul_global.dynamic_num != -1)
    {
      strcpy (coredump_dir + dirlen, ".da_main");
      if (roerr == NULL)
	roerr = SWI_OS_File_SaveBlockLoadExec (coredump_dir, mem->dalomem /* load */,
					       mem->dabreak /* exec */, mem->dalomem /* start */,
					       mem->dalimit /* end */);
    }

  /* Write all mmap DA blocks (when there).  */
  strcpy (coredump_dir + dirlen, ".da_X");
  for (int i = 0; i < MAX_MMAPS; i++)
    {
      if (mmaps[i].number != -1)
	{
	  coredump_dir[dirlen + sizeof (".da_")-1] = '0' + i;
	  if (roerr == NULL)
	    roerr = SWI_OS_File_SaveBlockLoadExec (coredump_dir, (int) mmaps[i].addr,
						   (int) mmaps[i].addr, (int) mmaps[i].addr,
						   (int) mmaps[i].addr + mmaps[i].len);
	}
    }

  coredump_dir[dirlen] = '\0';
  return roerr;
}


/* Free all mmapped memory.  This is called from __dynamic_area_exit.
   At that point, the thread system is not running.  */
void
__munmap_all (void)
{
  for (int i = 0; i < MAX_MMAPS; i++)
    {
      if (mmaps[i].number != -1)
	munmap (mmaps[i].addr, 0);
    }
}

/* Map addresses starting near ADDR and extending for LEN bytes.  from
   OFFSET into the file FD describes according to PROT and FLAGS.  If ADDR
   is nonzero, it is the desired mapping address.  If the MAP_FIXED bit is
   set in FLAGS, the mapping will be at ADDR exactly (which must be
   page-aligned); otherwise the system chooses a convenient nearby address.
   The return value is the actual mapping address chosen or (caddr_t) -1
   for errors (in which case `errno' is set).  A successful `mmap' call
   deallocates any previous mapping for the affected region.  */

caddr_t
mmap (caddr_t addr, size_t len, int prot, int flags, int fd, off_t offset)
{
  struct ul_global *gbl = &__ul_global;

  PTHREAD_UNSAFE

  /* We don't support MAP_FIXED.  */
  if (flags & MAP_FIXED)
    return (caddr_t) __set_errno (((size_t)addr & (gbl->pagesize - 1)) ? EINVAL : ENOSYS);

  /* We don't support MAP_COPY.  */
  if (flags & MAP_COPY)
    return (caddr_t) __set_errno (ENOSYS);

  /* We don't support MAP_PRIVATE. However, we will just use MAP_SHARED instead
     rather than changing all the user code calling us.  */

  /* We only support PROT_READ or PROT_WRITE.  */
  if (((prot & PROT_EXEC) == PROT_EXEC)
      || ((prot & (PROT_READ | PROT_WRITE)) == 0))
    return (caddr_t) __set_errno (ENOSYS);

  /* Find spare mmap_info index.  */
  int i;
  for (i = 0; i < MAX_MMAPS; i++)
    {
      if (mmaps[i].number == -1)
	break;
    }

  /* Maximum number of mmap sections reached.  */
  if (i == MAX_MMAPS)
    return (caddr_t) __set_errno (ENOMEM);

  len = page_align (gbl, len);

  /* Create the dynamic area.  */
  int regs[10];
  regs[0] = 0;
  regs[1] = -1;
  regs[2] = len;
  regs[3] = -1;
  regs[6] = 0;
  regs[7] = 0;
  /* The DA name could be the same name used in sys/_syslib.s so Virtualise
     can be enabled/disabled for the area. Using the name with " MMap" appended
     is useful though to separate the areas from the more general purpose
     heap.  */
  char namebuf[128];

  if (&__dynamic_da_name)
    regs[8] = (int) __dynamic_da_name;
  else
    {
      size_t name_len = strlen (program_invocation_short_name);
      if (name_len + sizeof (" MMap") > sizeof (namebuf))
	name_len -= sizeof (namebuf) - sizeof (" MMap");
      memcpy (namebuf, program_invocation_short_name, name_len);
      memcpy (namebuf + name_len, " MMap", sizeof (" MMap"));
      regs[8] = (int)namebuf;
    }

  /* Check whether we're mapping a file or just memory */
  if (fd != -1)
    {
      /* Always use read/write access, otherwise we won't be able to
	 read/write the file data.  */
      regs[4] = 0x80;

      /* Assume that mmap'd files won't be enlarged very often */
      regs[5] = regs[2];
    }
  else
    {
      /* Non-zero offset only makes sense for mmap onto files.  */
      if (offset != 0)
	return (caddr_t) __set_errno (EINVAL);

      if (prot & PROT_READ)
	{
	  if (prot & PROT_WRITE)
	    regs[4] = 0x80;
	  else
	    regs[4] = 0x81;
	}
      else
	regs[4] = 0x82;

      /* We allocate triple the requested memory as the virtual limit to help
	 with mremap which is used by realloc.  */
      regs[5] = regs[2] * 3;
    }

  if (__os_swi (OS_DynamicArea, regs))
    return (caddr_t) __set_errno (ENOMEM);

  mmaps[i].number = regs[1];
  mmaps[i].addr	  = (caddr_t)regs[3];
  mmaps[i].len	  = len;
  mmaps[i].prot	  = prot;
  mmaps[i].flags  = flags;
  mmaps[i].fd     = fd;
  mmaps[i].offset = offset;

  if (fd == -1)
    return mmaps[i].addr;

  /* Read in the relevant section of the file */
  int count = 0;

  off_t oldpos = lseek (fd, 0, SEEK_CUR);

  lseek (fd, offset, SEEK_SET);

  while (count < len)
    {
      ssize_t size = read (fd, mmaps[i].addr + count, len - count);

      if (size < 0)
	{
	  int save_errno = errno;
	  munmap (mmaps[i].addr, len);
	  lseek (fd, oldpos, SEEK_SET);
	  return (caddr_t) __set_errno (save_errno);
	}
      else if (size == 0)
	break;

      count += size;
    }
  lseek (fd, oldpos, SEEK_SET);

  return mmaps[i].addr;
}

/* Deallocate any mapping for the region starting at ADDR and extending LEN
   bytes.  Returns 0 if successful, -1 for errors (and sets errno).  */
int
munmap (caddr_t addr, size_t len)
{
  PTHREAD_UNSAFE

  int i;
  for (i = 0; i < MAX_MMAPS; i++)
    {
      if (mmaps[i].addr == addr)
	break;
    }

  /* If we couldn't find the mmap mapping, then return an error.  */
  if (i == MAX_MMAPS)
    return __set_errno (EINVAL);

  /* Write back file contents if necessary
     All the docs I've seen suggest that we only need to write
     stuff back on msync(), but some software (e.g. mkimage)
     seems to rely on the writeback occuring un munmap().  */
  if (mmaps[i].fd != -1
      && (mmaps[i].prot & PROT_WRITE) && !(mmaps[i].flags & MAP_PRIVATE))
    {
      /* Assume that the user only wants 'len' bytes writing.  */
      off_t oldpos = lseek (mmaps[i].fd, 0, SEEK_CUR);
      lseek (mmaps[i].fd, mmaps[i].offset, SEEK_SET);
      write (mmaps[i].fd, addr, len);
      lseek (mmaps[i].fd, oldpos, SEEK_SET);
    }

  /* Unmap by deleting the dynamic area.  */
  int regs[10];
  regs[0] = 1;
  regs[1] = mmaps[i].number;
  mmaps[i].addr = 0;
  mmaps[i].number = -1;
  _kernel_oserror *err;
  if ((err = __os_swi (OS_DynamicArea, regs)) != NULL)
    return __ul_seterr (err, EOPSYS); /* Failed to delete area.  */

  return 0;
}

/* Change the memory protection of the region starting at ADDR and
   extending LEN bytes to PROT.  Returns 0 if successful, -1 for errors
   (and sets errno).  */
int
mprotect (caddr_t addr __attribute__ ((__unused__)),
	  size_t len __attribute__ ((__unused__)),
	  int prot __attribute__ ((__unused__)))
{
  return __set_errno (ENOSYS);
}

/* this is supposed to be able to work on non-mmaped memory too. */

/* Synchronize the region starting at ADDR and extending LEN bytes with the
   file it maps.  Filesystem operations on a file being mapped are
   unpredictable before this is done.  */
int
msync (caddr_t addr, size_t len)
{
  const struct ul_global *gbl = &__ul_global;

  PTHREAD_UNSAFE

  int i;
  for (i = 0; i < MAX_MMAPS; i++)
    {
      if (mmaps[i].addr <= addr && mmaps[i].addr + mmaps[i].len >= addr)
	break;
    }

  /* If we couldn't find the mmap mapping, then return an error.  */
  if (i == MAX_MMAPS)
    return __set_errno (EINVAL);

  /* Although it doesn't matter for us, still enforce the requirement for
     addr to be page aligned.  */
  if ((size_t)addr & (gbl->pagesize - 1))
    return __set_errno (EINVAL);

  /* Don't access areas outside the mapped range.  */
  if (addr + len > mmaps[i].addr + mmaps[i].len)
    return __set_errno (ENOMEM);

  /* Only valid if this is a file open for non-private writing.  */
  if (mmaps[i].fd == -1
      || !(mmaps[i].prot & PROT_WRITE) || (mmaps[i].flags & MAP_PRIVATE))
    return __set_errno (EINVAL);

  /* If no length specified, flush the whole mapping.  */
  if (!len)
    {
      len = mmaps[i].len;
      addr = mmaps[i].addr;
    }
  off_t oldpos = lseek (mmaps[i].fd, 0, SEEK_CUR);
  lseek (mmaps[i].fd, mmaps[i].offset + addr - mmaps[i].addr, SEEK_SET);
  int fail = write (mmaps[i].fd, addr, len) != len;
  lseek (mmaps[i].fd, oldpos, SEEK_SET);

  return fail ? __set_errno (EIO) : 0;
}

/* Advise the system about particular usage patterns the program follows
   for the region starting at ADDR and extending LEN bytes.  */
int
madvise (caddr_t addr __attribute__ ((__unused__)),
	 size_t len __attribute__ ((__unused__)),
	 int advice __attribute__ ((__unused__)))
{
  return __set_errno (ENOSYS);
}

/* Remap addresses mapped by the range [ADDR,ADDR+OLD_LEN) to new length
   NEW_LEN.  If MAY_MOVE is MREMAP_MAYMOVE the returned address may differ
   from ADDR.  The return value is the actual mapping address chosen or
   (caddr_t) -1 for errors (in which case `errno' is set).  */

caddr_t
mremap (caddr_t addr, size_t old_len, size_t new_len, int may_move)
{
  struct ul_global *gbl = &__ul_global;

  PTHREAD_UNSAFE

  int i;
  for (i = 0; i < MAX_MMAPS; i++)
    {
      if (mmaps[i].addr == addr)
	break;
    }

  /* If we couldn't find the mmap mapping, then return an error.  */
  if (i == MAX_MMAPS)
    return (caddr_t) __set_errno (EINVAL);

  /* For simplicity don't allow remapping of mapped files.  */
  if (mmaps[i].fd != -1)
    return (caddr_t) __set_errno (ENOSYS);

  old_len = page_align (gbl, old_len);

  /* Check correct length was passed.  */
  if (old_len != mmaps[i].len)
    return (caddr_t) __set_errno (EINVAL);

  new_len = page_align (gbl, new_len);
  int regs[10];
  regs[0] = mmaps[i].number;
  regs[1] = new_len - old_len;

  /* Check whether we actually need to alter the mmap area.  */
  if (regs[1] == 0)
    return addr;

  _kernel_oserror *err = __os_swi (OS_ChangeDynamicArea, regs);
  if (err)
    {
      /* If we were trying to reduce the mmap size and that failed, then
	 return an error.  */
      if (new_len < old_len)
	{
	  __ul_seterr (err, EOPSYS);
	  /* Soldier on since we did manage to remap the memory.  */
	  return addr;
	}
      if ((may_move & MREMAP_MAYMOVE) == 0)
	return (caddr_t) __ul_seterr (err, EOPSYS); /* XXX Should we return ENOMEM ? */

      /* Create a new mmap section to replace this section and copy over the
	 data.  */
      int old_area = mmaps[i].number;
      mmaps[i].number = -1;
      caddr_t new_addr = mmap (0, new_len, mmaps[i].prot, mmaps[i].flags, -1, 0);
      if (new_addr == (caddr_t)-1)
	{
	  /* If mmap failed, then keep the old area.  */
	  mmaps[i].addr	  = addr;
	  mmaps[i].number = old_area;
	  return (caddr_t) -1;
	}
      /* Fast page copy.  */
      __mmap_page_copy (new_addr, addr, old_len);
      regs[0] = 1;
      regs[1] = old_area;
      err = __os_swi (OS_DynamicArea, regs);
      /* Record any error, which may be due to the deletion of the
	 old area failing. We also soldier on, since we did manage
	 to remap the memory.  */
      if (err)
	__ul_seterr (err, EOPSYS);
      addr = new_addr;
    }

  return addr;
}
