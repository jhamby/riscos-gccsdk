/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/strxfrm.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strxfrm.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <locale.h>
#include <string.h>
#include <unixlib/os.h>
#include <swis.h>

size_t
strxfrm (char *to, const char *from, size_t size)
{
  int regs[10];

  regs[0] = __locale_territory[LC_COLLATE];
  regs[1] = (int)to;
  regs[2] = (int)from;
  regs[3] = size;

  __os_swi (Territory_TransformString, regs);
  return regs[0];
}
