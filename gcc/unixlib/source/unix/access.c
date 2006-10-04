/* access ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

int
access (const char *path, int mode)
{
  struct stat stats;

  if (stat (path, &stats))
    return -1;

  if (mode & F_OK)
    return 0;  /* File exists.  */

  /* Clear any bad bits.  */
  mode &= (X_OK | W_OK | R_OK);

  /* Write access.  */
  if (mode & W_OK)
    if (!(stats.st_mode & (S_IWUSR | S_IWGRP)))
      return __set_errno (EACCES);

  /* Read/execute access.  */
  if (mode & (R_OK | X_OK))
    if (!(stats.st_mode & (S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP)))
      return __set_errno (EACCES);

  return 0;
}
