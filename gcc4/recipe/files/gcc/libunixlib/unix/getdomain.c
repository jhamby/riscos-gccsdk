/* getdomainname (), setdomainname ()
 * Written by Peter Burwood, July 1997
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

/* getdomainname() returns
   1: <Inet$LocalDomain> if it exists.
   2: <Inet$HostName> with the part before the first '.' removed if it
   contains a '.' and Inet$LocalDomain does not exist.  */
int
getdomainname (char *name, size_t len)
{
  if (!name)
    return __set_errno (EFAULT);

  const char *domainname = getenv ("Inet$LocalDomain");
  if (domainname == NULL)
    {
      /* Domain isn't defined, so we try using the HostName and hope that it
         is a fully qualified name.  */
      domainname = getenv ("Inet$HostName");
      if (domainname && (domainname = strchr (domainname, '.')) != NULL)
        domainname++;
      /* Time to give up trying to get domain name.
         Say that operation is not supported.  */
      if (domainname == NULL)
        return __set_errno (ENOSYS);
    }

  if (strlen (domainname) >= len)
    {
      /* Brain dead behaviour requires copying as much of name as possible
         into buffer and not zero terminating.  */
      memcpy (name, domainname, len);
      return __set_errno (ENAMETOOLONG);
    }

  strcpy (name, domainname);
  return 0;
}
