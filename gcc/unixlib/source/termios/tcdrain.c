/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcdrain.c,v $
 * $Date: 2005/04/05 21:11:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>


/* Wait for pending output to be written on FD.  */
int
tcdrain (int fd)
{
  return 0;
}
