/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/locale/c/strxfrm,v $
 * $Date: 1997/10/09 20:00:03 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strxfrm,v 1.2 1997/10/09 20:00:03 unixlib Exp $";
#endif

#include <locale.h>
#include <string.h>
#include <sys/os.h>
#include <sys/swis.h>

size_t
strxfrm (char *to, const char *from, size_t size)
{
  int regs[10];

  regs[0] = __locale_territory[LC_COLLATE];
  regs[1] = (int)to;
  regs[2] = (int)from;
  regs[3] = size;

  os_swi (Territory_TransformString, regs);
  return regs[0];
}
