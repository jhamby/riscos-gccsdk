/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/common/c/attrib,v $
 * $Date: 1999/11/16 13:26:49 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 * Written by Nick Burrett, 3 May 1997.
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: attrib,v 1.5 1999/11/16 13:26:49 admin Exp $";
#endif

#include <sys/stat.h>
#include <unixlib/types.h>
#include <unixlib/local.h>

/* Convert a file mode into a attribute mask suitable for
   passing to RISC OS.  */
int __set_protection (__mode_t mode)
{
  int attribute;

  attribute = (mode & S_IREAD) ? (1 << 0) : 0; /* Owner read */
  attribute |= (mode & S_IWRITE) ? (1 << 1) : 0; /* Owner write */
  attribute |= (mode & S_IROTH) ? (1 << 4) : 0; /* Public read */
  attribute |= (mode & S_IWOTH) ? (1 << 5) : 0; /* Public write */
  return attribute;
}

/* Convert a RISC OS attribute mask into a 'stat' compatible
   file access mode.  */
__mode_t __get_protection (int attribute)
{
  __mode_t mode;

  mode = (attribute & (1 << 5)) ? S_IWOTH : 0; /* Public write */
  mode |= (attribute & (1 << 0)) ? S_IREAD : 0; /* Owner read */
  mode |= (attribute & (1 << 1)) ? S_IWRITE : 0; /* Owner write */
  mode |= (attribute & (1 << 4)) ? S_IROTH : 0; /* Public read */
  return mode;
}
