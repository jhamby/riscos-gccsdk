/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/creat.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: creat.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

#include <fcntl.h>
#include <unixlib/types.h>

int
creat (const char *file, __mode_t mode)
{
  return open (file, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
