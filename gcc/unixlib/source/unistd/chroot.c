/**************************************************************************** 
 * 
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/Attic/chroot.c,v $
 * $Date: 2002/03/30 09:40:18 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 * 
 ***************************************************************************/ 
 
#include <errno.h> 
#include <unistd.h> 
 
int 
chroot (const char *path) 
{ 
  path = path; 
 
  return __set_errno (ENOSYS); 
} 
