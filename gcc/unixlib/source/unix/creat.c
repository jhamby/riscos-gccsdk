/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/creat,v $
 * $Date: 1997/10/09 20:00:49 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: creat,v 1.6 1997/10/09 20:00:49 unixlib Exp $";
#endif

#include <fcntl.h>
#include <unixlib/types.h>

int
creat (const char *file, __mode_t mode)
{
  return open (file, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
