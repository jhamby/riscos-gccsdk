/* Rename a file.
   Copyright (c) 2005-2012 UnixLib Developers.  */

#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <swis.h>

#include <unixlib/local.h>
#include <internal/local.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <internal/swiparams.h>
#include <pthread.h>

int
rename (const char *old_name, const char *new_name)
{
  const _kernel_oserror *err;

  PTHREAD_UNSAFE

  if (old_name == NULL || new_name == NULL)
    return __set_errno (EINVAL);

  int oftype;
  char ofile[MAXPATHLEN];
  if (!__riscosify_std (old_name, 0, ofile, sizeof (ofile), &oftype))
    return __set_errno (ENAMETOOLONG);

  int oftype_a = (oftype == __RISCOSIFY_FILETYPE_NOTFOUND) ? 0xfff : oftype;

  /* Does the old file exist ?  */
  if (!__object_exists_raw (ofile))
    return __set_errno (ENOENT);

  /* Let __riscosify_std () create the directory if necessary. This is so
     rename ("foo", "foo.c") works when "c" does not exist.  */
  int nftype;
  char nfile[MAXPATHLEN];
  if (!__riscosify_std (new_name, 1, nfile, sizeof (nfile), &nftype))
    return __set_errno (ENAMETOOLONG);

  int nftype_a = (nftype == __RISCOSIFY_FILETYPE_NOTFOUND) ? 0xfff : nftype;

  /* If old and new are existing links to the same file, rename() immediately
     returns zero (success) and does nothing else.

     The only case where this will be true under RISC OS (with no hard links)
     is when old and new point to the same file name.
     RISC OS allows file systems to be case sensitive or insenstive, so
     strictly if we want to compare two names (coping with image files) we'd
     have to figure out case-sensitivity status for each dir in the path!
     POSIX demands case-sensitive file systems, so lets just use strcmp.  */

  if (strcmp (nfile, ofile) == 0)
    goto try_filetyping;

  if (__isdir_raw (ofile))
    {
      /* Directories can not have a filetype.  */
      if (oftype != __RISCOSIFY_FILETYPE_NOTFOUND)
        return __set_errno (ENOENT);

      /* old_name is a directory. If new_name exists, and it is a
	 directory that is empty, we can delete it.  */
      if (__object_exists_raw (nfile))
	{
	  if (__isdir_raw (nfile))
	    return __set_errno (EISDIR);
	  /* Attempt to delete the directory.  */
	  if ((err = SWI_OS_File_DeleteObject (nfile)) != NULL)
	    return __set_errno (ENOTEMPTY);
	}
    }
  else
    {
      /* old_name is a file. If new_name exists, then it
	 is removed during the renaming operation. If
	 new_name is a directory, rename will fail.  */
      if (__object_exists_raw (nfile))
	{
	  if (__isdir_raw (nfile))
	    return __set_errno (EISDIR);
	  /* We can't use unlink() as it might delete the suffix dir */
	  if ((err = SWI_OS_File_DeleteObject (nfile)) != NULL)
	    return __ul_seterr (err, EOPSYS);
	}
    }

  /* FIXME: How can we determine that a file is on a different device
     or filing system or that the source file is currently open ?
     Ideally st_dev needs fixing in stat().  */

  /* Perform the rename.  */
  int regs[10];
  regs[0] = 25;
  regs[1] = (int)ofile;
  regs[2] = (int)nfile;
  err = __os_swi (OS_FSControl, regs);
  if (err)
    {
      /* We check on RISC OS error 176 which probably is caused by a rename
         across file systems.  */
      if (err->errnum == 176)
	return __set_errno (EXDEV);
      /* RISC OS error 0xD6 "'foo' not found" is given when ofile does not
	 exist.  A RISC OS FS error 0x1xxD6 "Not found" indicates not all
	 directories of nfile exist.  Both conditions should return ENOENT.  */
      if (err->errnum == 0xD6 || (err->errnum & 0xFFFF00FF) == 0x100D6)
	return __set_errno (ENOENT);
      return __ul_seterr (err, EOPSYS);
    }

try_filetyping:
  if (oftype_a != nftype_a)
    {
      /* We need to set the filetype of the file.  */
      if ((err = SWI_OS_File_WriteCatInfoFileType (nfile, nftype_a)) != NULL)
        return __ul_seterr (err, EOPSYS);
    }

  /* Delete the suffix swap dir if it is now empty */
  __unlinksuffix (ofile); /* ofile is corrupted by this call */

  return 0;
}
