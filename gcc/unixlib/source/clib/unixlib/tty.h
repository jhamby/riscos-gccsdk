/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/tty.h,v $
 * $Date: 2002/09/24 21:02:37 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __UNIXLIB_TTY_H
#define __UNIXLIB_TTY_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __TERMIOS_H
#include <termios.h>
#endif

#ifndef __SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifndef __SYS_PARAM_H
#include <sys/param.h>
#endif

__BEGIN_DECLS

#define MAXTTY	2

#define TTY_CON 0
#define TTY_423 1

struct tty
  {
  struct termios t[1];
  struct winsize w[1];
  int (*out) (int);
  int (*in) (void);
  int (*scan) (int);
  int (*init) (void);
  int (*flush) (void);
  int sx,cx;		/* screen x, character x */
  int cnt;		/* number of characters in input buffer */
  char *ptr;		/* read pointer in input buffer */
  char buf[MAX_INPUT];  /* input buffer */
  char del[MAX_INPUT];  /* number of displayed characters for character cx */
  int lookahead;	/* [1 byte or -1] lookahead to allow select() */
  };

/* Note, the buf and del buffers are declared statically in the tty struct.
   This is so that an exec'd process which shares the tty struct doesn't
   free something allocated by the parent process.  Such a call to free would
   confuse the memory allocation system since the buffer wouldn't have been
   allocated by that process but by the parent process.  We could check the
   address of the buffer against __lomem and __break, but we might aswell
   just live with the statically allocated memory compared to the extra code
   we would need to have.  Lazy, probably, but it works okay and only costs
   MAXTTY * MAX_INPUT * 2 (= 1024 bytes in current implementation).  */

__END_DECLS

#endif
