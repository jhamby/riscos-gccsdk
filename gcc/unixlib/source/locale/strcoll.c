/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/strcoll.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcoll.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <locale.h>
#include <string.h>
#include <unixlib/os.h>
#include <swis.h>

int
strcoll (const char *s1, const char *s2)
{
  int regs[10];

  regs[0] = __locale_territory[LC_COLLATE];
  regs[1] = (int)s1;
  regs[2] = (int)s2;
  regs[3] = 0;

  __os_swi (Territory_Collate, regs);
  return regs[0];
}
