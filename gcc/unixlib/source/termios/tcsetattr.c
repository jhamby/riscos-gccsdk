/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcsetattr.c,v $
 * $Date: 2002/01/12 16:06:57 $
 * $Revision: 1.2.2.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcsetattr.c,v 1.2.2.3 2002/01/12 16:06:57 admin Exp $";
#endif

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unixlib/unix.h>

const speed_t __bsd_speeds[16] =
  {
    0,
    50,
    75,
    110,
    134,
    150,
    200,
    300,
    600,
    1200,
    1800,
    2400,
    4800,
    9600,
    19200,
    38400,
  };


/* Set the state of FD to *TERMIOS_P.  */
int
tcsetattr (int fd, int optional_actions, const struct termios *termios_p)
{
  struct sgttyb buf;
  struct tchars tchars;
  struct ltchars ltchars;
  int local;
#ifdef	TIOCGETX
  int extra;
#endif
  size_t i;

  if (ioctl (fd, TIOCGETP, &buf) < 0 ||
      ioctl (fd, TIOCGETC, &tchars) < 0 ||
      ioctl (fd, TIOCGLTC, &ltchars) < 0 ||
#ifdef	TIOCGETX
      ioctl (fd, TIOCGETX, &extra) < 0 ||
#endif
      ioctl (fd, TIOCLGET, &local) < 0)
    return -1;

  if (termios_p == NULL)
    return __set_errno (EINVAL);

  switch (optional_actions & ~TCSASOFT)
    {
    case TCSANOW:
      break;
    case TCSADRAIN:
      if (tcdrain (fd) < 0)
	return -1;
      break;
    case TCSAFLUSH:
      if (tcflush (fd, TCIFLUSH) < 0)
	return -1;
      break;
    default:
      return __set_errno (EINVAL);
    }

  buf.sg_ispeed = buf.sg_ospeed = (char) -1;
  for (i = 0; i <= sizeof (__bsd_speeds) / sizeof (__bsd_speeds[0]); ++i)
    {
      if (__bsd_speeds[i] == termios_p->__ispeed)
	buf.sg_ispeed = i;
      if (__bsd_speeds[i] == termios_p->__ospeed)
	buf.sg_ospeed = i;
    }
  if (buf.sg_ispeed == (char) -1 || buf.sg_ospeed == (char) -1)
    return __set_errno (EINVAL);

  buf.sg_flags &= ~(CBREAK|RAW);
  if (!(termios_p->c_lflag & ICANON))
    buf.sg_flags |= (termios_p->c_cflag & ISIG) ? CBREAK : RAW;
#ifdef	LPASS8
  if (termios_p->c_oflag & CS8)
    local |= LPASS8;
  else
    local &= ~LPASS8;
#endif
  if (termios_p->c_lflag & NOFLSH)
    local |= LNOFLSH;
  else
    local &= ~LNOFLSH;
  if (termios_p->c_oflag & OPOST)
    local &= ~LLITOUT;
  else
    local |= LLITOUT;
#ifdef	TIOCGETX
  if (termios_p->c_lflag & ISIG)
    extra &= ~NOISIG;
  else
    extra |= NOISIG;
  if (termios_p->c_cflag & CSTOPB)
    extra |= STOPB;
  else
    extra &= ~STOPB;
#endif
  if (termios_p->c_iflag & ICRNL)
    buf.sg_flags |= CRMOD;
  else
    buf.sg_flags &= ~CRMOD;
  if (termios_p->c_iflag & IXOFF)
    buf.sg_flags |= TANDEM;
  else
    buf.sg_flags &= ~TANDEM;

  buf.sg_flags &= ~(ODDP|EVENP);
  if (!(termios_p->c_cflag & PARENB))
    buf.sg_flags |= ODDP | EVENP;
  else if (termios_p->c_cflag & PARODD)
    buf.sg_flags |= ODDP;
  else
    buf.sg_flags |= EVENP;

  if (termios_p->c_lflag & ECHO)
    buf.sg_flags |= ECHO;
  else
    buf.sg_flags &= ~ECHO;
  if (termios_p->c_lflag & ECHOE)
    local |= LCRTERA;
  else
    local &= ~LCRTERA;
  if (termios_p->c_lflag & ECHOK)
    local |= LCRTKIL;
  else
    local &= ~LCRTKIL;
  if (termios_p->c_lflag & TOSTOP)
    local |= LTOSTOP;
  else
    local &= ~LTOSTOP;

  buf.sg_erase = termios_p->c_cc[VERASE];
  buf.sg_kill = termios_p->c_cc[VKILL];
  tchars.t_eofc = termios_p->c_cc[VEOF];
  tchars.t_intrc = termios_p->c_cc[VINTR];
  tchars.t_quitc = termios_p->c_cc[VQUIT];
  ltchars.t_suspc = termios_p->c_cc[VSUSP];
  tchars.t_startc = termios_p->c_cc[VSTART];
  tchars.t_stopc = termios_p->c_cc[VSTOP];

  if (ioctl (fd, TIOCSETP, &buf) < 0 ||
      ioctl (fd, TIOCSETC, &tchars) < 0 ||
      ioctl (fd, TIOCSLTC, &ltchars) < 0 ||
#ifdef	TIOCGETX
      ioctl (fd, TIOCSETX, &extra) < 0 ||
#endif
      ioctl (fd, TIOCLSET, &local) < 0)
    return -1;
  return 0;
}
