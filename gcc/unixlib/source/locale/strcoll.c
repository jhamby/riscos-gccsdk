/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/strcoll.c,v $
 * $Date: 2000/07/15 14:52:20 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcoll.c,v 1.1.1.1 2000/07/15 14:52:20 nick Exp $";
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
