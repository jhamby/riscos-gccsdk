/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/uname,v $
 * $Date: 1997/10/09 20:00:53 $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: uname,v 1.10 1997/10/09 20:00:53 unixlib Exp $";
#endif

#include <errno.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/param.h>
#include <unistd.h>
#include <sys/os.h>
#include <sys/swis.h>

/* Try and extract the help version from the module help string.  */
static void
extract_module_version (char *version, const void *module_base)
{
  char *help_string;

  /* Write default value.  */
  strcpy (version, "1.0");

  if (module_base == NULL)
    return;

  help_string = (char *)module_base + ((unsigned int*)module_base)[5];

  if (help_string)
    {
      help_string = strchr (help_string, '\t');
      if (help_string)
	{
	  ++help_string;
	  if (help_string[0] == '\t')
	    ++help_string;
	  while (*help_string && *help_string != ' ')
	    *version++ = *help_string++;
	  *version = '\0';
	}
    }
}

/* Put information about the system in 'name'.  */
int
uname (struct utsname *name)
{
  int save;
  int regs[10];

  if (name == NULL)
    return __set_errno (EINVAL);

  /* Network name of the computer.  */
  save = errno;
  if (gethostname (name->nodename, sizeof (name->nodename)) < 0)
    {
      if (errno != ENAMETOOLONG)
	return -1;
      /* The name was truncated, so zero terminate.  */
      (void) __set_errno (save);
      name->nodename[sizeof (name->nodename) - 1] = '\0';
    }

  /* The name of the operating system in use.  */
  strcpy (name->sysname, "riscos");

  /* Type of hardware that is in use.  */
  strcpy (name->machine, "arm-acorn");

  /* Get the operating system version number.  We do this by
     extracting the version from the help string for the UtilityModule.
     This has a module version number the same as the OS version.  */
  regs[0] = 18;
  regs[1] = (int)"UtilityModule";
  if (os_swi (OS_Module, regs) != NULL)
    regs[3] = 0;

  /* Release level of the operating system.  */
  extract_module_version (name->release, (void *) regs[3]);

  /* Version level within the release of the operating system.
     Default to 1.0 unless ROMPatches exists.  */

  /* Get the module number of ROMPatches (if it exists).
     OS_Module will return this value in regs[1].  */
  regs[0] = 18;
  regs[1] = (int)"ROMPatches";
  if (os_swi (OS_Module, regs) != NULL)
    {
      /* ROMPatches doesn't exist.  */
      regs[3] = 0;
    }

  extract_module_version (name->version, (void *) regs[3]);

  return 0;
}
