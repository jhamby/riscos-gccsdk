/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/funcallerr.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: funcallerr.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <signal.h>
#include <unixlib/unix.h>
#include <unixlib/os.h>

int
__funcall_error (const char *file, int line, unsigned int addr)
{
  char buf[11];
  char *s = buf + 11;

  __os_print ("fatal error: Attempt to call a function at address below 0x");
  __os_prhex ((unsigned int) __base);
  __os_print ("\n\ror a function at a non word-aligned address in a UnixLib function.\r\nFile: ");
  __os_print (file);
  __os_print (", line: ");
  do
    {
      *--s = (line % 10) + '0';
      line /= 10;
    }
  while (line != 0);
  __os_print (s);
  __os_print (", function address: 0x");
  __os_prhex (addr);
  __os_nl ();

  raise (SIGABRT);
  return 0;
}
