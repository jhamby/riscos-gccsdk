/* chroot ()
 * Copyright (c) 2002-2006 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

int
chroot (const char *path)
{
  path = path; 

  return __set_errno (ENOSYS); 
}
