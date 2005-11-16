/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/strcoll.c,v $
 * $Date: 2002/02/14 15:56:35 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
