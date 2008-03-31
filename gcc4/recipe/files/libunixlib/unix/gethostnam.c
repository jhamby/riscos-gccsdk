/* Get internet hostname for machine.
   Copyright (c) 2003, 2005, 2008 UnixLib Developers.  */

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <internal/os.h>

/* gethostname() returns
   1: <Inet$HostName> if Inet$HostName contains a dot
   2: <Inet$HostName>.<Inet$LocalDomain> (checking that Inet$LocalDomain
      exists, removing the double dot if necessary)
   3: <Inet$HostName> if it exists but <Inet$LocalDomain> doesn't
   4: "acorn<station>" if Inet$Host doesn't exist [ using fakehostname() ]
 */

static int
fakehostname (char *name, size_t len)
{
  int regs[3];
  _kernel_oserror *err;
  char buf[8] = "acorn";
  static const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                               '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  /* Create a string that consists of "acorn<econet station number>".
     Read station number from CMOS location 1.  */
  if ((err = __os_byte (161, 0, 0, regs)) != NULL)
    return __ul_seterr (err, 1);

  buf[7] = 0;
  buf[6] = hex[regs[1] & 0xf];
  buf[5] = hex[(regs[1] >> 4) & 0xf];

  if (strlen (buf) >= len)
    {
      /* Brain dead behaviour requires copying as much of name as possible
	 into buffer and not zero terminating.  */
      strncpy (name, buf, len);
      return __set_errno (ENAMETOOLONG);
    }

  strcpy (name, buf);
  return 0;
}

int
gethostname (char *name, size_t len)
{
  const char *hostname = getenv ("Inet$HostName");
  size_t hostname_len;
  const char *domain;

  if (name == NULL)
    return __set_errno (EFAULT);

  if (hostname == NULL)
    return fakehostname (name, len);

  hostname_len = strlen (hostname);

  if (hostname_len >= len)
    {
      /* Brain dead behaviour requires copying as much of name as possible
	 into buffer and not zero terminating.  */
      strncpy (name, hostname, len);
      return __set_errno (ENAMETOOLONG);
    }

  strcpy (name, hostname);

  if (strchr (name, '.') != NULL)
    /* Found a '.' - assume that we have the fully qualified name */
    return 0;

  domain = getenv ("Inet$LocalDomain");
  if (domain == NULL)
    /* Domain isn't defined - the unqualified name is going to have to do. */
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
      strncpy (name, domain, len);
      return __set_errno (ENAMETOOLONG);
    }

  strcpy (name, domain);
  return 0;
}

/* Set the name of the host machine.  */

int
sethostname (const char *name, size_t length)
{
  /* Bit of a difficult one to set under RISC OS. Set errno
     and return -1 which indicates that the calling process
     is not privileged enough to set the host name.  */
  name = name;
  length = length;
  return __set_errno (EPERM);
}
