/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/strxfrm.c,v $
 * $Date: 2000/07/15 14:52:20 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strxfrm.c,v 1.1.1.1 2000/07/15 14:52:20 nick Exp $";
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
