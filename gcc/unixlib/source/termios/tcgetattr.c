/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcgetattr.c,v $
 * $Date: 2001/09/11 16:53:58 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcgetattr.c,v 1.2.2.2 2001/09/11 16:53:58 admin Exp $";
#endif

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unixlib/unix.h>

/* Put the state of FD into *TERMIOS_P.  */
int
tcgetattr (int fd, struct termios *termios_p)
{
  struct sgttyb buf;
  struct tchars tchars;
  struct ltchars ltchars;
  int local;
#ifdef	TIOCGETX
  int extra;
#endif

  if (termios_p == NULL)
    return __set_errno (EINVAL);

  if (ioctl(fd, TIOCGETP, &buf) < 0 ||
      ioctl(fd, TIOCGETC, &tchars) < 0 ||
      ioctl(fd, TIOCGLTC, &ltchars) < 0 ||
#ifdef TIOCGETX
      ioctl(fd, TIOCGETX, &extra) < 0 ||
#endif
      ioctl(fd, TIOCLGET, &local) < 0)
    return -1;

  termios_p->__ispeed = __bsd_speeds[(unsigned char) buf.sg_ispeed];
  termios_p->__ospeed = __bsd_speeds[(unsigned char) buf.sg_ospeed];

  termios_p->c_iflag = 0;
  termios_p->c_oflag = 0;
  termios_p->c_cflag = 0;
  termios_p->c_lflag = 0;
  termios_p->c_oflag |= CREAD | HUPCL;
#ifdef	LPASS8
  if (local & LPASS8)
    termios_p->c_oflag |= CS8;
  else
#endif
    termios_p->c_oflag |= CS7;
  if (!(buf.sg_flags & RAW))
    {
      termios_p->c_iflag |= IXON;
      termios_p->c_cflag |= OPOST;
#ifndef	NOISIG
      termios_p->c_lflag |= ISIG;
#endif
    }
  if ((buf.sg_flags & (CBREAK | RAW)) == 0)
    termios_p->c_lflag |= ICANON;
  if (!(buf.sg_flags & RAW) && !(local & LLITOUT))
    termios_p->c_oflag |= OPOST;
  if (buf.sg_flags & CRMOD)
    termios_p->c_iflag |= ICRNL;
  if (buf.sg_flags & TANDEM)
    termios_p->c_iflag |= IXOFF;
#ifdef	TIOCGETX
  if (!(extra & NOISIG))
    termios_p->c_lflag |= ISIG;
  if (extra & STOPB)
    termios_p->c_cflag |= CSTOPB;
#endif

  switch (buf.sg_flags & (EVENP | ODDP))
    {
    case EVENP|ODDP:
      break;
    case ODDP:
      termios_p->c_cflag |= PARODD;
    default:
      termios_p->c_cflag |= PARENB;
      termios_p->c_iflag |= IGNPAR | INPCK;
      break;
    }
  if (buf.sg_flags & ECHO)
    termios_p->c_lflag |= ECHO;
  if (local & LCRTERA)
    termios_p->c_lflag |= ECHOE;
  if (local & LCRTKIL)
    termios_p->c_lflag |= ECHOK;
  if (local & LTOSTOP)
    termios_p->c_lflag |= TOSTOP;
  if (local & LNOFLSH)
    termios_p->c_lflag |= NOFLSH;

  termios_p->c_cc[VEOF] = tchars.t_eofc;
  termios_p->c_cc[VEOL] = '\n';
  termios_p->c_cc[VERASE] = buf.sg_erase;
  termios_p->c_cc[VKILL] = buf.sg_kill;
  termios_p->c_cc[VINTR] = tchars.t_intrc;
  termios_p->c_cc[VQUIT] = tchars.t_quitc;
  termios_p->c_cc[VSTART] = tchars.t_startc;
  termios_p->c_cc[VSTOP] = tchars.t_stopc;
  termios_p->c_cc[VSUSP] = ltchars.t_suspc;
  termios_p->c_cc[VMIN] = -1;
  termios_p->c_cc[VTIME] = -1;

  return 0;
}
