/* Rename a file.
   Copyright (c) 2005-2013 UnixLib Developers.  */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <unixlib/local.h>
#include <internal/local.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <internal/swiparams.h>
#include <pthread.h>

int
rename (const char *old_name, const char *new_name)
{
  PTHREAD_UNSAFE

  if (old_name == NULL || new_name == NULL)
    return __set_errno (EFAULT);

  if (old_name[0] == '\0' || new_name[0] == '\0')
    return __set_errno (ENOENT);

  int oftype;
  char ofile[MAXPATHLEN];
  if (!__riscosify_std (old_name, 0, ofile, sizeof (ofile), &oftype))
    return __set_errno (ENAMETOOLONG);
  /* Fetch real objtype and filetype of old_name.  */
  unsigned otype, oloadaddr;
  if (SWI_OS_File_ReadCatInfo (ofile, &otype, &oloadaddr, NULL, NULL, NULL) != NULL)
    otype = 0;
  unsigned oftype_ro;
  if ((oloadaddr & 0xfff00000U) == 0xfff00000U)
    oftype_ro = (oloadaddr >> 8) & 0xfff;
  else
    oftype_ro = __RISCOSIFY_FILETYPE_NOTFOUND;

  /* Let __riscosify_std () create the directory if necessary. This is so
     rename ("foo", "foo.c") works when "c" does not exist.  */
  int nftype;
  char nfile[MAXPATHLEN];
  if (!__riscosify_std (new_name, 1, nfile, sizeof (nfile), &nftype))
    return __set_errno (ENAMETOOLONG);
  int ntype = __object_exists_ro (nfile);

  /* Does the old file/directory exist ? If so, and the new object exists
     as well, is it the same type ? */
  if (otype == 0
      || (oftype != __RISCOSIFY_FILETYPE_NOTFOUND && oftype != oftype_ro))
    {
      /* FIXME: if one of the directories in ofile does not exist, return
	 ENOTDIR instead.  */
      return __set_errno (ENOENT);
    }
  else if (otype == 1 && ntype == 2)
    return __set_errno (EISDIR);
  else if (otype == 2
	   && (ntype == 1 || nftype != __RISCOSIFY_FILETYPE_NOTFOUND))
    return __set_errno (ENOTDIR);

  /* Do a RISC OS rename.  If that works ok, we're done.  This will
     deal with ofile and nfile being the same or only being different
     case-wise for both case-sensitive and case-insensitive filing systems.  */
  const _kernel_oserror *err;
  if ((err = SWI_OS_FSControl_Rename (ofile, nfile)) != NULL)
    {
      switch (err->errnum & 0xFFFF00FF)
	{
	  case 0x100D6:
	    /* A RISC OS FS error 0x1xxD6 "Not found" indicates not all
	       directories of nfile exist.  */
	    return __set_errno (ENOENT);
	  case 0x100B0:
	    /* A RISC OS FS error 0x1xxB0 "Bad RENAME" indicates we're moving
	       a directory into in its subdirectory.  FIXME: check on otype == 2 needed ?  */
	    return __set_errno (EINVAL);
	}

      if (ntype == 0)
	return __ul_seterr (err, EOPSYS); /* FIXME: ok ? */

      /* Straight rename failed, so delete nfile (now we're sure ofile and
	 nfile aren't the same file/directory) and retry.  */
      if (SWI_OS_File_DeleteObject (nfile) != NULL)
	return __set_errno (ntype == 2 ? ENOTEMPTY : EPERM); /* FIXME: could be EROFS ? */

      /* Perform the rename again.  */
      if ((err = SWI_OS_FSControl_Rename (ofile, nfile)) != NULL)
	{
	  /* We check on RISC OS error 0xB0 "Bad RENAME" which probably is
	     caused by a rename across file systems.  */
	  if (err->errnum == 0xB0)
	    return __set_errno (EXDEV);
	  if ((err->errnum & 0xFFFF00FF) == 0x100B0)
	    return __set_errno (EINVAL);
	  return __ul_seterr (err, EOPSYS); /* FIXME: ok ? */
	}
    }

  int oftype_a = (oftype == __RISCOSIFY_FILETYPE_NOTFOUND) ? 0xfff : oftype;
  int nftype_a = (nftype == __RISCOSIFY_FILETYPE_NOTFOUND) ? 0xfff : nftype;
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
