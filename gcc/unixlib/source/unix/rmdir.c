/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/rmdir.c,v $
 * $Date: 2003/10/06 19:00:01 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: rmdir.c,v 1.6 2003/10/06 19:00:01 joty Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <unixlib/os.h>
#include <swis.h>

#include <unixlib/local.h>
#include <internal/swiparams.h>

int
rmdir (const char *ux_directory)
{
  char directory[_POSIX_PATH_MAX];
  int regs[10], objtype, attr;
  int riscosify_control, rtrn_get_attrs;

  /* We don't want to do suffix swapping for directory objects.  */
  riscosify_control = __get_riscosify_control ();
  __set_riscosify_control (riscosify_control | __RISCOSIFY_NO_SUFFIX);

  rtrn_get_attrs = __object_get_attrs (ux_directory,
				       directory, sizeof (directory),
                                       &objtype,
				       NULL, NULL, NULL, NULL, &attr);

  /* Restore suffix swapping status.  */
  __set_riscosify_control (riscosify_control);

  if (rtrn_get_attrs)
    return -1;

  /* Images and directories have bit 1 set. Clear implies file.  */
  if ((objtype & 2) == 0)
    return __set_errno (ENOTDIR);

  /* Directory is locked against deletion if bit 3 is set.  */
  if (attr & (1 << 3))
    return __set_errno (EACCES);

  /* Explicitly check that DIRECTORY is empty, otherwise non-empty image
     files can be zapped. We loop because OS_GBPB is allowed to do nothing
     due to external constraints imposed by some filing systems (PRM 2-70).  */
  do
    {
      char scratch_buf[NAME_MAX];
      _kernel_oserror *err;

      regs[0] = 9;
      regs[1] = (int) directory;
      regs[2] = (int) scratch_buf;
      regs[3] = 1;
      regs[4] = 0;
      regs[5] = sizeof (scratch_buf);
      regs[6] = 0;	/* Match anything.  */

      err = __os_swi (OS_GBPB, regs);
      if (err)
	{
	  /* Oh shit. Let's pretend that it's not empty.  */
	  __seterr (err);
	  return __set_errno (ENOTEMPTY);
	}
      if (regs[3] > 0)
	return __set_errno (ENOTEMPTY);
    }
  while (regs[4] != -1);

  /* The directory must be empty to get here, so delete it.  */
  regs[0] = 6;
  if (__os_swi (OS_File, regs) == NULL)
    return 0;

  /* Hmm, can't delete it, it's not locked, it is empty => read only fs.  */
  return __set_errno (EROFS);
}
