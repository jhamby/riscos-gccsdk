/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/objattr.c,v $
 * $Date: 2005/04/14 15:17:23 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: objattr.c,v 1.6 2005/04/14 15:17:23 nick Exp $";
#endif

#include <errno.h>

#include <swis.h>

#include <unixlib/os.h>
#include <unixlib/local.h>
#include <internal/swiparams.h>

/* Get an object's filetype, object type, etc and do some common checks.
   Returns nonzero and sets errno on error. Returns riscosified filename
   in __buffer. objtype, ftype, etc may be NULL if not needed. */
int
__object_get_attrs (const char *ux_file, char *buffer, size_t buf_len,
                    int *objtype, int *ftype, int *loadaddr, int *execaddr,
                    int *length, int *attr)
{
  _kernel_oserror *err;
  int regs[10], sftype, aftype;

  if (ux_file == NULL)
    return __set_errno (EINVAL);

  /* Convert the filename. We don't want to guess the filetype of the file
     based on the extension, as this may cause the filetype check later on
     to fail. */
  if (!__riscosify (ux_file, 0,
                    __get_riscosify_control () | __RISCOSIFY_FILETYPE_NOT_SET,
                    buffer, buf_len, &sftype))
    return __set_errno (ENAMETOOLONG);

  /* Get catalogue information.  */
  err = __os_file (OSFILE_READCATINFO_NOPATH, buffer, regs);
  if (err)
    {
      __ul_seterr (err, 0);
      return __set_errno (EIO);
    }

  /* Does the file has a filetype (at this point we aren't even sure that
     the file exists but that's not a problem, see next 'if' on regs[0]
     contents further on) ?  */
  if ((regs[2] & 0xfff00000U) == 0xfff00000U)
    aftype = (regs[2] >> 8) & 0xfff;
  else
    aftype = __RISCOSIFY_FILETYPE_NOTFOUND;

  if (objtype)
    *objtype = regs[0];

  if (ftype)
    *ftype = aftype;

  if (loadaddr)
    *loadaddr = regs[2];

  if (execaddr)
    *execaddr = regs[3];

  if (length)
    *length = regs[4];

  if (attr)
    *attr = regs[5];

  /* Fail if file doesn't exist or (if specified) filetype is different.  */
  if (regs[0] == 0 ||
    ((regs[0] == 1 || (regs[0] == 3 && __get_feature_imagefs_is_file ())) &&
      sftype != __RISCOSIFY_FILETYPE_NOTFOUND && sftype != aftype))
    return __set_errno (ENOENT);

  return 0;
}

/* Sets an objects filetype and attributes (either can be set to
   __ATTR_NOTSPECIFIED to leave unchanged).
   Returns nonzero and sets errno on error.  */
int
__object_set_attrs (const char *ux_file, char *buffer, size_t buf_len,
                    int ftype, int attr)
{
  _kernel_oserror *err;
  int regs[10], sftype;

  if (ux_file == NULL)
    return __set_errno (EINVAL);

  if (!__riscosify_std (ux_file, 0, buffer, buf_len, &sftype))
    return __set_errno (ENAMETOOLONG);

  /* Set catalogue information.  */
  if (ftype != __ATTR_NOTSPECIFIED)
    {
      regs[2] = ftype;
      err = __os_file (OSFILE_WRITECATINFO_FILETYPE, buffer, regs);
      if (err)
        {
          __ul_seterr (err, 0);
          return __set_errno (EIO);
        }
    }

  if (attr != __ATTR_NOTSPECIFIED)
    {
      regs[5] = attr;
      err = __os_file (OSFILE_WRITECATINFO_ATTR, buffer, regs);
      if (err)
        {
          __ul_seterr (err, 0);
          return __set_errno (EIO);
        }
    }

  return 0;
}

