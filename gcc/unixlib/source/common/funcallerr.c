/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/common/c/funcallerr,v $
 * $Date: 1997/10/09 19:59:59 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: funcallerr,v 1.2 1997/10/09 19:59:59 unixlib Exp $";
#endif

#include <signal.h>
#include <sys/syslib.h>
#include <sys/os.h>

int
__funcall_error (const char *file, int line, unsigned int addr)
{
  char buf[11];
  char *s = buf + 11;

  os_print ("fatal error: Attempt to call a function at address below 0x");
  os_prhex ((unsigned int) __base);
  os_print ("\n\ror a function at a non word-aligned address in a UnixLib function.\r\nFile: ");
  os_print (file);
  os_print (", line: ");
  do
    {
      *--s = (line % 10) + '0';
      line /= 10;
    }
  while (line != 0);
  os_print (s);
  os_print (", function address: 0x");
  os_prhex (addr);
  os_nl ();

  raise (SIGABRT);
  return 0;
}
