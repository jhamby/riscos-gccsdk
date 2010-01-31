/* uname ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/param.h>
#include <unistd.h>
#include <swis.h>

#include <internal/os.h>
#include <internal/unix.h>

/* Try and extract the help version from the module help string.  */
static void
extract_module_version (char *version, size_t max, const void *module_base)
{
  unsigned int help_offset;

  /* Write default value.  */
  if (max < sizeof ("1.0"))
    return;
  strcpy (version, "1.0");

  if (module_base == NULL)
    return;

  /* Is there a help string ?  */
  if ((help_offset = ((const unsigned int *)module_base)[5]) != 0)
    {
      const char *help_string;

      help_string = (const char *)module_base + help_offset;

      help_string = strchr (help_string, '\t');
      if (help_string)
        {
          /* According the the spec, only one or two TABs may be used.
             This is a little bit more foolproof.  */
          while (*++help_string == '\t')
            ;
          while (*help_string != '\0' && *help_string != ' ' && max-- > 1)
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
  unsigned int arch;

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
  strcpy (name->sysname, "RISC OS");

  /* Type of hardware that is in use.  */
  if ((arch = __get_cpu_arch ()) == 0)
    strcpy (name->machine, "arm");
  else
    {
      strcpy (name->machine, "armvxl");
      name->machine[4] = '0' + arch;
    }

  /* Get the operating system version number.  We do this by
     extracting the version from the help string for the UtilityModule.
     This has a module version number the same as the OS version.  */
  regs[0] = 18;
  regs[1] = (int)"UtilityModule";
  if (__os_swi (OS_Module, regs) != NULL)
    regs[3] = 0;

  /* Release level of the operating system.  */
  extract_module_version (name->release, sizeof (name->release), (const void *) regs[3]);

  /* Version level within the release of the operating system.
     - First try to figure out the RISC OS "part number".
     - If that fails, figure out if ROMPatches exists and take its version
       number.
     - If that fails too, default to "1.0".  */
  regs[0] = 9;
  regs[1] = 1;
  if (__os_swi (OS_ReadSysInfo, regs) == NULL && regs[0] != 0)
    {
      /* Is in the form : "##########-###" potentionally followed by
         a dash and a comment.  We're not interested in the latter.  */
      strncpy (name->version, (const char *) regs[0], 12);
      name->version[12] = '\0';
    }
  else
    {
      /* Get the module number of ROMPatches (if it exists).
         OS_Module will return this value in regs[1].  */
      regs[0] = 18;
      regs[1] = (int)"ROMPatches";
      if (__os_swi (OS_Module, regs) != NULL)
        regs[3] = 0;

      extract_module_version (name->version, sizeof (name->version), (const void *) regs[3]);
    }

  return 0;
}
