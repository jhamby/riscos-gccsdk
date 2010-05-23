/* Get internet hostname for machine.
   Copyright (c) 2003-2010 UnixLib Developers.  */

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <internal/os.h>

static int
fakehostname (char *name, size_t len)
{
  /* Create a string that consists of "acorn<econet station number>".
     Read station number from CMOS location 1.  */
  const _kernel_oserror *err;
  int regs[3];
  if ((err = __os_byte (161, 0, 0, regs)) != NULL)
    return __ul_seterr (err, 1);

  const char buf[8] =
  {
    'a', 'c', 'o', 'r', 'n',
    "0123456789ABCDEF"[(regs[1] >> 4) & 0xf],
    "0123456789ABCDEF"[regs[1] & 0xf],
    '\0'
  };
  if (len < 8)
    {
      /* Brain dead behaviour requires copying as much of name as possible
	 into buffer and not zero terminating.  */
      memcpy (name, buf, len);
      return __set_errno (ENAMETOOLONG);
    }

  strcpy (name, buf);
  return 0;
}

/* gethostname() returns
   1: <Inet$HostName> if Inet$HostName contains a dot
   2: <Inet$HostName>.<Inet$LocalDomain> (checking that Inet$LocalDomain
      exists, removing the double dot if necessary)
   3: <Inet$HostName> if it exists but <Inet$LocalDomain> doesn't
   4: "acorn<station>" if Inet$Host doesn't exist [ using fakehostname() ] */
int
gethostname (char *name, size_t len)
{
  if (name == NULL)
    return __set_errno (EFAULT);

  const char *hostname = getenv ("Inet$HostName");
  if (hostname == NULL)
    return fakehostname (name, len);

  size_t hostname_len = strlen (hostname);

  if (hostname_len >= len)
    {
      /* Brain dead behaviour requires copying as much of name as possible
	 into buffer and not zero terminating.  */
      strncpy (name, hostname, len);
      return __set_errno (ENAMETOOLONG);
    }

  strcpy (name, hostname);

  if (strchr (name, '.') != NULL)
    /* Found a '.' - assume that we have the fully qualified name.  */
    return 0;

  const char *domain = getenv ("Inet$LocalDomain");
  if (domain == NULL)
    /* Domain isn't defined - the unqualified name is going to have to do.  */
    return 0;

  if (*domain == '.')
    domain++;

  /* We can fit a '.' in the buffer since the buffer is zero terminated.  */
  name += hostname_len;
  len = len - hostname_len - 1;
  *name++ = '.';

  if (strlen (domain) >= len)
    {
      /* Brain dead behaviour requires copying as much of name as possible
	 into buffer and not zero terminating.  */
      memcpy (name, domain, len);
      return __set_errno (ENAMETOOLONG);
    }

  strcpy (name, domain);
  return 0;
}

