/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/locale/c/strcoll,v $
 * $Date: 1997/10/09 20:00:02 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcoll,v 1.2 1997/10/09 20:00:02 unixlib Exp $";
#endif

#include <locale.h>
#include <string.h>
#include <sys/os.h>
#include <sys/swis.h>

int
strcoll (const char *s1, const char *s2)
{
  int regs[10];

  regs[0] = __locale_territory[LC_COLLATE];
  regs[1] = (int)s1;
  regs[2] = (int)s2;
  regs[3] = 0;

  os_swi (Territory_Collate, regs);
  return regs[0];
}
