/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/tty.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tty.c,v 1.8 2003/04/05 09:33:57 alex Exp $";
#endif

/* System V tty device driver for RISC OS.  */

#define _BSD_SOURCE

#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>
#include <unixlib/tty.h>
#include <unixlib/os.h>
#include <unixlib/dev.h>
#include <sys/select.h>
#include <swis.h>
#include <unixlib/features.h>
#include <unixlib/fd.h>
#include <pthread.h>

#define IGNORE(x) x = x

/* Store the number of characters actually displayed for this single
   character in the del buffer.  When a backspace/delete is processed,
   then we know how many characters to rub out.  cx is a pointer to the
   current position within the buffer and sx is the number of displayed
   characters on this line.
   When a newline is written, cx and sx are reset to 0.  MAX_INPUT must be
   a power of 2, otherwise the logical expression below will not provide a
   circular buffer.  */

#if MAX_INPUT != 256
#error "MAX_INPUT has changed. Ensure it is a power of 2 and correct this."
#endif

#define __ttyx(tty, x)		/* increment cursor posn. by (x) */ \
	(tty->sx += (x), \
	tty->del[tty->cx = (tty->cx + 1) & (MAX_INPUT - 1)] = (x))

static int __ttyicanon (const struct __unixlib_fd *file_desc, void *buf,
			int nbyte, struct tty *tty);
static int __ttyiraw (const struct __unixlib_fd *file_desc, void *buf,
		      int nbyte, struct tty *tty);

static int __ttyinput (int c, tcflag_t iflag);
static void __ttyecho (struct tty *tty, int c, tcflag_t oflag, tcflag_t lflag);
static void __ttydel (struct tty *tty, tcflag_t lflag);
static void __ttytab (struct tty *tty, tcflag_t oflag);
static void __ttycr (struct tty *tty, tcflag_t oflag);
static void __ttynl (struct tty *tty, tcflag_t oflag);


static cc_t ttydefchars[NCCS] =
{
  CEOF, /* 0 */
  CEOL, /* 1 */
  CEOL, /* 2 */
  CERASE, /* 3 */
  CWERASE, /* 4 */
  CKILL, /* 5 */
  CREPRINT, /* 6 */

  CERASE, /* 7 */
  CINTR, /* 8 */
  CQUIT, /* 9 */
  CSUSP, /* 10 */
  CDSUSP, /* 11 */
  CSTART, /* 12 */

  CSTOP, /* 13 */
  CLNEXT, /* 14 */
  CDISCARD, /* 15 */
  CMIN, /* 16 */
  CTIME, /* 17 */
  CSTATUS, /* 18 */
  _POSIX_VDISABLE /* 19 */
};


/* Get console window size.  */
static void
__tty_console_gwinsz (struct winsize *win)
{
  static const int vars[] = {132, 134, 135, 133, 128, 130, 129, 131, -1};
  int values[(sizeof (vars) - 1) / sizeof (int)];
  int regs[10];

  if (__wimpprogram == 1)
    {
      char *size;
      int lines = 24, cols = 80;

      size = getenv ("LINES");
      if (size)
        {
          lines = atoi (size);

          if (lines <= 0)
            lines = 24;
        }

      size = getenv ("COLUMNS");
      if (size)
        {
          cols = atoi (size);

          if (cols <= 0)
            cols = 80;
        }

      win->ws_col = cols;
      win->ws_row = lines;
      win->ws_xpixel = cols * 8;
      win->ws_ypixel = lines * 16;

    }
  else
    {
      regs[0] = (int) vars;
      regs[1] = (int) values;
      __os_swi (OS_ReadVduVariables, regs);
      win->ws_col = values[1] - values[0] + 1;
      win->ws_row = values[3] - values[2] + 1;
      win->ws_xpixel = values[5] - values[4] + 1;
      win->ws_ypixel = values[7] - values[6] + 1;
  }
}

/* Set console window size.  */

static void
__tty_console_swinsz (struct winsize *win)
{
  static const int vars[] = {132, 135, 128, 131, 4, 5, -1};
  int values[(sizeof (vars) - 1) / sizeof (int)];
  int j;
  int regs[10];

  PTHREAD_UNSAFE

  regs[0] = (int) vars;
  regs[1] = (int) values;
  __os_swi (OS_ReadVduVariables, regs);
  __os_vdu (28);
  __os_vdu (values[0]);
  __os_vdu (values[1] + win->ws_row - 1);
  __os_vdu (values[0] + win->ws_col - 1);
  __os_vdu (values[1]);
  __os_vdu (24);
  j = values[2];
  j <<= values[4];
  __os_vdu (j & 0xff);
  __os_vdu (j >> 8);
  j = values[3] - (win->ws_ypixel - 1);
  j <<= values[5];
  __os_vdu (j & 0xff);
  __os_vdu (j >> 8);
  j = values[2] + win->ws_xpixel - 1;
  j <<= values[4];
  __os_vdu (j & 0xff);
  __os_vdu (j >> 8);
  j = values[3];
  j <<= values[5];
  __os_vdu (j & 0xff);
  __os_vdu (j >> 8);
#ifdef SIGWINCH
  /* Raise the 'Window size change' signal to notify any applications
     that might be interested.  */
  raise (SIGWINCH);
#endif
}

/* Read console.  */
static void
__tty_console_gterm (struct termios *term)
{
  int regs[3];

  PTHREAD_UNSAFE

  /* Get `Interrupt key' and state of `Interrupt key'.  */
  __os_byte (0xdc, 0, 0xff, regs);
  term->c_cc[VINTR] = regs[1];
  __os_byte (0xe5, 0, 0xff, regs);
  if (regs[1])
    term->c_lflag &= ~ISIG; /* Disable signals.  */
  else
    term->c_lflag |= ISIG; /* Enable signals.  */
}

/* Set console.  */
static void
__tty_console_sterm (struct termios *term)
{
  PTHREAD_UNSAFE

  /* Set `Interrupt key' and state of `Interrupt key'.  */
  __os_byte (0xdc, term->c_cc[VINTR], 0, NULL);
  if (term->c_lflag & ISIG)
    /* Enable signals.  */
    __os_byte (0xe5, 0, 0, NULL);
  else
    __os_byte (0xe5, 0xff, 0, NULL);
}

#if __FEATURE_DEV_RS423
struct baud_map
{
  int baud_rate;
  int internal_rate;
};

static const struct baud_map baud_rate[] =
{
  { 0, 0 }, { 50, 9 }, { 75, 1 }, { 110, 10 }, { 134, 11 },
  { 150, 2 }, { 200, 0 }, { 300, 3 }, { 600, 12 }, { 1200, 4 },
  { 1800, 13 }, { 2400, 5 }, { 4800, 6 }, { 9600, 7 },
  { 19200, 8 }, { 38400, 16 }, { 57600, 17 }, { 115200, 18 },
  { -1, -1 }
};

static int
convert_baud_rate (__speed_t baud)
{
  int x;

  /* Map 4 baud rate to RISC OS internal number.  */
  x = 0;
  while (baud_rate[x].baud_rate != -1)
    {
      if (baud == baud_rate[x].baud_rate)
        return baud_rate[x].internal_rate;
      x++;
    };
  return 0;
}

/* Set RS423.  */
static void
__tty_423_sterm (struct termios *term)
{
  int control;
  tcflag_t cflag;
  int regs[10];

  PTHREAD_UNSAFE

  /* Set baud rate for receive and transmit and data format.  */

  regs[0] = 5;
  regs[1] = convert_baud_rate (term->__ispeed);
  if (regs[1] != 0)
    __os_swi (OS_SerialOp, regs);

  regs[0] = 6;
  regs[1] = convert_baud_rate (term->__ospeed);
  if (regs[1] != 0)
    __os_swi (OS_SerialOp, regs);

  /* Get the control mode.  */
  cflag = term->c_cflag;

  /* Set the appropriate number of bits per byte.  */
  if ((cflag & CSIZE) == CS5) /* 5 bits per byte.  */
    control = 3;
  else if ((cflag & CSIZE) == CS6) /* 6 bits per byte.  */
    control = 2;
  else if ((cflag & CSIZE) == CS7) /* 7 bits per byte.  */
    control = 1;
  else /* CS8 (8 bits per byte) and anything else.  */
    control = 0;

  if (cflag & CSTOPB)
    /* Two stop bits instead of one.  */
    control |= 0x04;
  if (cflag & PARENB)
    {
      /* Parity enable.  */
      control |= 0x08;
      if (!(cflag & PARODD))
        /* Odd parity.  */
	control |= 0x10;
    }
  regs[0] = 1;
  regs[1] = control;
  __os_swi (OS_SerialOp, regs);
}
#endif

void *
__ttyopen (struct __unixlib_fd *file_desc, const char *file, int mode)
{
  struct tty *tty;
  struct termios *term;
  int type;

  IGNORE (mode);
  IGNORE (file_desc);

  if (file[5] == 'c')
    type = TTY_CON; /* /dev/console */
#if __FEATURE_DEV_RS423
  else if (file[5] == 'r' || strcmp(file + 5, "ttyS0") == 0)
    type = TTY_423; /* /dev/rs423 */
#endif
  else if (file[5] == 't')
    type = __u->status.tty_type; /* /dev/tty */
  else
    return (void *) -1;

  tty = __u->tty + type;
  __u->status.tty_type = type;

  term = tty->t;
  /* Input mode is set to:
       a) Signal interrupt on break.
       b) Map CR to NL on input.
       c) Ring bell when input queue is full.  */
  term->c_iflag = BRKINT | ICRNL | IMAXBEL;
  /* Output mode is set to:
       a) Perform output processing.
       b) Map NL to CR-NL on output.
       c) Expand tabs to spaces.  */
  term->c_oflag = OPOST | ONLCR | OXTABS;
  /* For consoles, the control mode is:
       a) 38400 baud
       b) 8 bits per byte
       c) Enable receiver.
       d) Hang up on last close.
       e) Ignore modem status lines.
     For RS423, the control mode is:
       a) 9600 baud.
       b) 8 bits per byte.
       c) Enable receiver.
       d) Hang up on last close.
       e) Ignore modem status lines.  */
  if (type == TTY_CON)
    {
      term->c_cflag = CS8 | CREAD | HUPCL | CLOCAL;
      term->__ispeed = term->__ospeed = (speed_t) B38400;
    }
#if __FEATURE_DEV_RS423
  else if (type == TTY_423)
    {
      term->c_cflag = CS8 | CREAD | HUPCL | CLOCAL;
      term->__ispeed = term->__ospeed = (speed_t) B9600;
    }
#endif

  /* Local mode is set to:
       a) Enable signals.
       b) Do erase and kill processing.
       c) Enable echo.  */
  term->c_lflag = ISIG | ICANON | ECHO;

  memcpy (term->c_cc, ttydefchars, NCCS);

  /* RS423 is set up later.  */
  if (type == TTY_CON)
    {
      __tty_console_gterm (term);
      __tty_console_gwinsz (tty->w);
      /* Set function pointers used for low level operations.  */
      tty->out = __os_vdu;
      tty->in = __os_get;
      tty->scan = __os_inkey;
      tty->init = __os_console;
      tty->flush = __os_keyflush;
    }
#if __FEATURE_DEV_RS423
  else if (type == TTY_423)
    {
      struct winsize *win = tty->w;
      win->ws_col = 80;
      win->ws_row = 25;
      win->ws_xpixel = win->ws_ypixel = 0;
      tty->out = __os_423vdu;
      tty->in = __os_423get;
      tty->scan = __os_423inkey;
      tty->init = __os_423;
      tty->flush = __os_423flush;
    }
#endif

  tty->ptr = tty->buf;
  tty->cnt = 0;
  tty->sx  = tty->cx = 0;
  tty->lookahead = -1;

  __funcall ((*(tty->init)), ());

#if __FEATURE_DEV_RS423
  /* Setup RS423.  */
  if (type == TTY_423)
    __tty_423_sterm (tty->t);
#endif

  return (void *) type;
}

int
__ttyclose (struct __unixlib_fd *file_desc)
{
  struct tty *tty = __u->tty + (int) file_desc->handle;

  tty->ptr = tty->buf;
  tty->cnt = 0;
  tty->sx  = tty->cx = 0;

  return 0;
}


int
__ttyread (struct __unixlib_fd *file_desc, void *buf, int nbyte)
{
  struct tty *tty = __u->tty + (int) file_desc->handle;

  return (tty->t->c_lflag & ICANON)
	  ? __ttyicanon (file_desc, buf, nbyte, tty)
	  : __ttyiraw (file_desc, buf, nbyte, tty);
}

static int
__ttyicanon (const struct __unixlib_fd *file_desc, void *buf, int nbyte,
	     struct tty *tty)
{
  int c, i;
  char *ttybuf;
  int nflag;
  const tcflag_t iflag = tty->t->c_iflag;
  const tcflag_t oflag = tty->t->c_oflag;
  const tcflag_t lflag = tty->t->c_lflag;
  const cc_t * const cc = tty->t->c_cc;
  const int ceof = cc[VEOF];
  const int ceol = (cc[VEOL] != '\0' ? cc[VEOL] : -1);
  const int ceol2 = (cc[VEOL2] != '\0' ? cc[VEOL2] : -1);

#define F_LNEXT		000001
#define F_MAX		000002
#define F_NDELAY	000004

  if (tty == __u->tty)
    __os_byte (0xe5, 0xff, 0, 0);		/* Disable SIGINT.  */

ret:

  if (tty->cnt != 0)
    {
      if (tty == __u->tty)
	__os_byte (0xe5, 0, 0, 0);	/* Re-enable SIGINT.  */

      i = (nbyte > tty->cnt) ? tty->cnt : nbyte;
      memcpy (buf, tty->ptr, i);
      tty->cnt -= i;
      tty->ptr += i;
      return i;
    }

  nflag = (file_desc->fflag & O_NDELAY) ? F_NDELAY : 0;

  ttybuf = tty->ptr = tty->buf;
  tty->cx = 0;
  i = 0;

  for (;;)
    {
      if (i < MAX_INPUT)
	nflag &= ~F_MAX;
      else
	nflag |= F_MAX;
      if (tty->lookahead < 0)
	{
	  if (nflag & F_NDELAY)
	    {
	      c = __funcall ((*(tty->scan)), (0));
	      
	      if (c < 0)
		{
	          if (__taskwindow)
	            {
                      _kernel_swi_regs regs;
		      
                      regs.r[0] = 6; /* Taskwindow sleep.  */
                      regs.r[1] = 0; /* Just yield.  */
		      
                      _kernel_swi(OS_UpCall, &regs, &regs);
		    }
		  
	          goto eol;
		}
            }
	  else
	    {
	      c = __funcall ((*(tty->in)), ());
	    }
	}
      else
	{
	  c = tty->lookahead;
	  tty->lookahead = -1;
	}

      if (c == '\r' && iflag & IGNCR)
	continue;
      c = __ttyinput (c, iflag);
      if (!(nflag & F_LNEXT))
	{
	  if (c == '\n' || c == ceof || c == ceol || c == ceol2)
	    goto eol;
	  if (c == cc[VLNEXT])
	    {
	      nflag |= F_LNEXT;
	      continue;
	    }
	  if (c == cc[VERASE])
	    {
	      if (i > 0)
		{
		  __ttydel (tty, lflag);
		  i--;
		}
	      continue;
	    }
	  if (c == cc[VWERASE])
	    {
	      while (--i >= 0 && isspace (ttybuf[i]))
		__ttydel (tty, lflag);
	      i++;
	      while (--i >= 0 && !isspace (ttybuf[i]))
		__ttydel (tty, lflag);
	      i++;
	      continue;
	    }
	  if (c == cc[VKILL])
	    {
	      while (--i >= 0)
		__ttydel (tty, lflag);
	      i++;
	      if (lflag & ECHOK)
		__ttynl (tty, oflag);
	      continue;
	    }
	  if (c == cc[VREPRINT])
	    {
	      int j;
	      __ttyecho (tty, c, oflag, lflag);
	      __ttynl (tty, oflag);
#if 0
	      if (!(oflag & ONLRET))
		__ttycr (tty, oflag);
#endif
	      for (j = 0; j < i; j++)
		__ttyecho (tty, ttybuf[j], oflag, lflag);
	      continue;
	    }
	  if (lflag & ISIG)
	    {
	      if (c == cc[VINTR])
		{
		  raise (SIGINT);
		  continue;
		}
	      if (c == cc[VQUIT])
		{
		  raise (SIGQUIT);
		  continue;
		}
	    }
	}
      else
	nflag &= ~F_LNEXT;
      if (nflag & F_MAX)
	{
	  if (iflag & IMAXBEL)
	    __funcall ((*(tty->out)), ('\007'));
	}
      else
	{
	  __ttyecho (tty, c, oflag, lflag);
	  ttybuf[i++] = c;
	}
    }

eol:

  if (c != ceof && c >= 0)
    if (!(nflag & F_MAX))
      {
	__ttyecho (tty, c, oflag, lflag);
	ttybuf[i++] = c;
      }

  tty->cnt = i;
  if (tty->cnt != 0 && !(nflag & F_NDELAY))
    goto ret;

  if (tty == __u->tty)
    __os_byte (0xe5, 0, 0, 0);	/* Re-enable SIGINT.  */

  if (tty->cnt == 0 && c != ceof)
    return __set_errno (EAGAIN);

  return 0;

#undef F_LNEXT
#undef F_MAX
#undef F_NDELAY
}

static int
__ttyiraw (const struct __unixlib_fd *file_desc, void *buf, int nbyte,
	   struct tty *tty)
{
  int c, i;
  char *ttybuf;
  int nflag;
  const tcflag_t iflag = tty->t->c_iflag;
  const tcflag_t oflag = tty->t->c_oflag;
  const tcflag_t lflag = tty->t->c_lflag;
  const cc_t * const cc = tty->t->c_cc;
  unsigned int vm, vt;

#define F_NSCAN		000001

  nflag = 0;

  if (file_desc->fflag & O_NDELAY)
    vm = vt = 0;
  else
    {
      vm = (cc[VMIN] && cc[VTIME]) ? 1 : cc[VMIN];
      vt = cc[VTIME] * 10;
    }
  if (cc[VMIN] && !cc[VTIME])
    nbyte = vm, nflag |= F_NSCAN;

  ttybuf = buf;
  i = 0;

  while (i < nbyte)
    {
      if (tty->lookahead >= 0)
	{
	  c = tty->lookahead;
	  tty->lookahead = -1;
	}
      else
	{
	  if (nflag & F_NSCAN)
            {
	      c = __funcall ((*(tty->scan)), (0));
	      
	      if (c < 0)
	        {
	          if (__taskwindow)
	            {
                      _kernel_swi_regs regs;
		      
                      regs.r[0] = 6; /* Taskwindow sleep. */
                      regs.r[1] = 0; /* Just yield.  */
		      
                      _kernel_swi(OS_UpCall, &regs, &regs);
                    }
		  
	          if (i >= vm)
	            return i;
	          else
	            continue;
	        }
            }
	  else
	    {
	      c = __funcall ((*(tty->in)), ());
	    }
	}
      if (c == '\r' && iflag & IGNCR)
	continue;
      c = __ttyinput (c, iflag);
      if (lflag & ISIG)
	{
	  if (c == cc[VINTR] && tty != __u->tty)
	    {
	      raise (SIGINT);
	      continue;
	    }
	  if (c == cc[VQUIT])
	    {
	      raise (SIGQUIT);
	      continue;
	    }
	}
      __ttyecho (tty, c, oflag, lflag);
      ttybuf[i++] = c;
    }

  return i;

#undef F_NSCAN
}


int
__ttywrite (struct __unixlib_fd *file_desc, const void *buf, int nbyte)
{
  struct tty *tty = __u->tty + (int) file_desc->handle;
  int i = 0, c;
  const char *s = buf;
  const tcflag_t oflag = tty->t->c_oflag;
  const tcflag_t lflag = tty->t->c_lflag;
  const cc_t * const cc = tty->t->c_cc;
  int (*const out) (int) = tty->out;

  while (i < nbyte)
    {
      c = s[i++];
#if 0
      if ((oflag & (OPOST | OLCUC)) == (OPOST | OLCUC))
	c = tolower (c);
#endif

      if (c == cc[VERASE])
	__ttydel (tty, lflag);
      else if (c == '\t')
	__ttytab (tty, oflag);
      else if (c == '\n')
	__ttynl (tty, oflag);
      else if (c == '\r')
	__ttycr (tty, oflag);
      else
	{
	  __ttyx (tty, 1);
	  __funcall ((*out), (c));
	}
    }

  return nbyte;
}


static int
__ttyinput (int c, tcflag_t iflag)
{
  if (iflag & ISTRIP)
    c &= 0x7f;
#if 0
  if (iflag & IUCLC)
    c = tolower (c);
#endif
  if (iflag & INLCR)
    if (c == '\n')
      c = '\r';
  if (iflag & ICRNL)
    if (c == '\r')
      c = '\n';
  return c;
}

static void
__ttyecho (struct tty *tty, int c, tcflag_t oflag, tcflag_t lflag)
{
  if (!((lflag & ECHO) || ((c == '\n') && (lflag & ECHONL))))
    return;

  if (c == '\t')
    __ttytab (tty, oflag);
  else if (c == '\n')
    __ttynl (tty, oflag);
  else if (c == '\r')
    __ttycr (tty, oflag);
  else if (iscntrl (c))
    {
      __ttyx (tty, 2);
      __funcall ((*(tty->out)), ('^'));
      __funcall ((*(tty->out)), ((c == 0x7f) ? '?' : (c + '@')));
    }
  else
    {
      __ttyx (tty, 1);
      __funcall ((*(tty->out)), (c));
    }
}

static void
__ttydel (struct tty *tty, tcflag_t lflag)
{
  int x;

  if (tty->sx != 0)
    {
      x = tty->del[tty->cx];
      tty->cx = (tty->cx - 1) & (MAX_INPUT - 1);
    }
  else
    x = 1;

  tty->sx = (tty->sx > x) ? (tty->sx - x) : 0;

  if (lflag & ECHO)
    {
      int (*const out) (int) = tty->out;

      if (lflag & ECHOE)
	while (x--)
	  {
	    __funcall ((*out), ('\b'));
	    __funcall ((*out), (' '));
	    __funcall ((*out), ('\b'));
	  }
      else
	while (x--)
	  {
	    __funcall ((*out), ('\177'));
	  }
    }
}

static void
__ttytab (struct tty *tty, tcflag_t oflag)
{
  if ((oflag & (OPOST | OXTABS)) == (OPOST | OXTABS))
    {
      int (*const out) (int) = tty->out;
      int x;

      x = (8 - (tty->sx & 0x7));
      __ttyx (tty, x);
      do
      {
	__funcall ((*(out)), (' '));
      } while (--x);
    }
  else
    {
      __ttyx (tty, 1);
      __funcall ((*(tty->out)), ('\t'));
    }
}

static void
__ttycr (struct tty *tty, tcflag_t oflag)
{
  tty->sx = 0;
#if 0
  if ((oflag & (OPOST | OCRNL)) == (OPOST | OCRNL))
    __funcall ((*(tty->out)), ('\n'));
  else if ((oflag & (OPOST | ONOCR)) != (OPOST | ONOCR))
#endif
  __funcall ((*(tty->out)), ('\r'));
}

static void
__ttynl (struct tty *tty, tcflag_t oflag)
{
  if (oflag & OPOST)
    {
      if (oflag & ONLCR)
	if (tty->sx
#if 0
	    || !(oflag & ONOCR)
#endif
	   )
	   __funcall ((*(tty->out)), ('\r'));
      if (oflag & (ONLCR
#if 0
      	 	   | ONLRET
#endif
      	 	  ))
	tty->sx = 0;
    }
  __funcall ((*(tty->out)), ('\n'));
}

#ifdef __UNIXLIB_NO_COMMON_DEV
off_t
__ttylseek (struct __unixlib_fd *file_desc, off_t lpos, int whence)
{
  IGNORE (lpos);
  IGNORE (whence);
  IGNORE (file_desc);
  return __set_errno (ESPIPE);
}
#endif

/* Return zero on success. Any other value on failure.  */
int
__ttyioctl (struct __unixlib_fd *file_desc, int request, void *arg)
{
  int type = (int) file_desc->handle;
  struct tty *tty = __u->tty + type;

  switch (request)
    {
    case TIOCMODG: /* Get modem control state.  */
      break;
    case TIOCMODS: /* Set modem control state.  */
      break;
    case TIOCEXCL: /* Set exclusive use of tty.  */
      return 0;
      break;
    case TIOCNXCL: /* Reset exclusive use of tty.  */
      return 0;
      break;
    case TIOCFLUSH: /* Flush buffers.  */
      __funcall ((*(tty->flush)), ());
      return 0;
      break;
    case TIOCGETA: /* Get termios struct.  */
      {
        struct termios *term = tty->t;

        if (type == TTY_CON)
	  __tty_console_gterm (term);
        memcpy (arg, term, sizeof (struct termios));
      }
      return 0;
      break;
    case TIOCSETA: /* Set termios struct.  */
    case TIOCSETAW: /* Drain output, set.  */
    case TIOCSETAF: /* Drain output, flush input, set.  */
      {
        struct termios *term = tty->t;

	if (request == TIOCSETAF)
	  __funcall ((*(tty->flush)), ());

        memcpy (term, arg, sizeof (struct termios));
        if (type == TTY_CON)
          __tty_console_sterm (term);
#if __FEATURE_DEV_RS423
        else if (type == TTY_423)
          __tty_423_sterm (term);
#endif
      }
      return 0;
      break;
      break;
      break;
    case TIOCGETD: /* Get line discipline.  */
      break;
    case TIOCSETD: /* Set line discipline.  */
      break;
    case TIOCSBRK: /* Set break bit.  */
#if __FEATURE_DEV_RS423
      if (type == TTY_423 && !arg)
	__os_423break (25);
#endif
      return 0;
      break;
    case TIOCCBRK: /* Clear break bit.  */
      break;
    case TIOCSDTR: /* Set data terminal ready.  */
      break;
    case TIOCCDTR: /* Clear data terminal ready.  */
      break;
    case TIOCGPGRP: /* Get pgrp of tty.  */
      *(int *)arg = __u->pgrp;
      break;
    case TIOCSPGRP: /* Set pgrp of tty.  */
      return __set_errno (EPERM);
      break;
    case TIOCOUTQ: /* Output queue size.  */
      break;
    case TIOCSTI: /* Simulate terminal input.  */
      break;
    case TIOCNOTTY: /* void tty association.  */
      break;
    case TIOCPKT: /* pty: set/clear packet mode.  */
      break;
    case TIOCSTOP: /* Stop output, like ^S.  */
      break;
    case TIOCSTART: /* Start output, like ^Q.  */
      break;
    case TIOCMSET: /* Set all modem bits.  */
      break;
    case TIOCMBIS: /* Bit set modem bits.  */
      break;
    case TIOCMBIC: /* Bit clear modem bits.  */
      break;
    case TIOCMGET: /* Get all modem bits.  */
      break;
    case TIOCREMOTE: /* Remove input editing.  */
      break;
    case TIOCLGET:  /* Get Local Modes */
      /* FIXME: This does nothing interesting - work in progress */
      *(int *)arg = 0;
      return 0;
      break;
    case TIOCLSET:  /* Set Local Modes */
      /* FIXME: This does nothing interesting - work in progress */
      return 0;
      break;
    case TIOCGETP: /* Get parameters - gtty */
      {
        struct termios *term = tty->t;
        struct sgttyb *gtty = (struct sgttyb *)arg;
	int flags = 0;

        gtty->sg_ispeed = term->__ispeed;
        gtty->sg_ispeed = term->__ospeed;
        gtty->sg_erase  = term->c_cc[VERASE];
        gtty->sg_kill   = term->c_cc[VKILL];

	if (! (term->c_lflag & ICANON))
	  flags |= (term->c_lflag & ISIG) ? CBREAK : RAW;

	if (term->c_lflag & ECHO)
	  flags |= ECHO;

	if ((term->c_oflag & OPOST) && (term->c_oflag & ONLCR))
	  flags |= CRMOD;

	gtty->sg_flags = flags;
      }
      return 0;
      break;
    case TIOCSETP: /* Set parameters - stty */
      {
        struct termios *term = tty->t;
        struct sgttyb *gtty = (struct sgttyb *) arg;
        int flags = gtty->sg_flags;

        term->__ispeed = gtty->sg_ispeed;
        term->__ospeed = gtty->sg_ispeed;
        term->c_cc[VERASE] = gtty->sg_erase;
        term->c_cc[VKILL] = gtty->sg_kill;

	term->c_iflag = ICRNL | IXON;
	term->c_oflag = 0;
	term->c_lflag = ISIG | ICANON;

	if (flags & CBREAK)
	  {
	    term->c_iflag = 0;
            term->c_lflag &= ~ICANON;
          }
	
	if (flags & ECHO)
	  term->c_lflag |= ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE | IEXTEN;
	
	if (flags & CRMOD)
	  term->c_oflag |= OPOST | ONLCR;

	if (flags & RAW)
	  {
            term->c_iflag = 0;
            term->c_lflag &= ~(ISIG | ICANON);
          }
	
	if (!(term->c_lflag & ICANON))
	  {
            term->c_cc[VMIN] = 1;
            term->c_cc[VTIME] = 0;
          }
      }
      return 0;
      break;
    case TIOCGETC: /* Get special characters */
      {
        struct tchars *chars = (struct tchars *)arg;
        struct termios *term = tty->t;

	chars->t_intrc = term->c_cc[VINTR];
	chars->t_quitc = term->c_cc[VQUIT];
	chars->t_startc = term->c_cc[VSTART];
	chars->t_stopc = term->c_cc[VSTOP];
	chars->t_eofc = term->c_cc[VEOF];
	chars->t_brkc = term->c_cc[VEOL];
      }
      return 0;
      break;
    case TIOCSETC: /* Set special characters */
      {
        struct tchars *chars = (struct tchars *)arg;
        struct termios *term = tty->t;

	term->c_cc[VINTR] = chars->t_intrc;
	term->c_cc[VQUIT] = chars->t_quitc;
	term->c_cc[VSTART] = chars->t_startc;
	term->c_cc[VSTOP] = chars->t_stopc;
	term->c_cc[VEOF] = chars->t_eofc;
	term->c_cc[VEOL] = chars->t_brkc;
      }
      return 0;
      break;
    case TIOCGLTC: /* Get special characters */
      {
        struct ltchars *chars = (struct ltchars *)arg;
        struct termios *term = tty->t;

        chars->t_suspc = term->c_cc[VSUSP];
        chars->t_dsuspc = term->c_cc[VDSUSP];
        chars->t_rprntc = term->c_cc[VREPRINT];
        chars->t_flushc = term->c_cc[VDISCARD];
        chars->t_werasc = term->c_cc[VWERASE];
        chars->t_lnextc = term->c_cc[VLNEXT];
      }
      return 0;
      break;
    case TIOCSLTC: /* Set special characters */
      {
        struct ltchars *chars = (struct ltchars *)arg;
        struct termios *term = tty->t;

	term->c_cc[VSUSP] = chars->t_suspc;
	term->c_cc[VDSUSP] = chars->t_dsuspc;
	term->c_cc[VREPRINT] = chars->t_rprntc;
	term->c_cc[VDISCARD] = chars->t_flushc;
	term->c_cc[VWERASE] = chars->t_werasc;
	term->c_cc[VLNEXT] = chars->t_lnextc;
      }
      return 0;
      break;
    case TIOCGWINSZ: /* Get window size.  */
      {
        struct winsize *win = tty->w;

	if (type == TTY_CON)
	  __tty_console_gwinsz (win);
	memcpy (arg, win, sizeof (struct winsize));
      }
      return 0;
      break;
    case TIOCSWINSZ: /* Set window size.  */
      {
        struct winsize *win = tty->w;

        memcpy (win, arg, sizeof (struct winsize));
        if (type == TTY_CON)
          __tty_console_swinsz (win);
      }
      return 0;
      break;
    case TIOCUCNTL: /* Pty: set/clr user control mode.  */
      break;
    case TIOCCONS: /* Become virtual console.  */
      return 0;
      break;
    case TIOCSCTTY: /* Become controlling tty.  */
      return 0;
      break;
    case TIOCEXT:  /* Pty: external processing.  */
      break;
    case TIOCSIG: /* Pty: generate signal.  */
      break;
    case TIOCDRAIN: /* Wait till output drained.  */
      break;
    default:
      return __set_errno (EINVAL);

    }

  return __set_errno (EINVAL);
}


int
__ttyselect (struct __unixlib_fd *file_desc, int fd,
	     fd_set *read, fd_set *write, fd_set *except)
{
  int type = (int) file_desc->handle;
  struct tty *tty = __u->tty + type;

  if (read)
    {
      /* Canonical mode, and buffer is not empty?  */
      if ((tty->t->c_lflag & ICANON) && (tty->cnt != 0))
	FD_SET(fd, read);
      else
	{
	  /* If no lookahead, then get some.  */
	  if (tty->lookahead < 0)
	    tty->lookahead = __funcall ((*(tty->scan)), (0));

	  if (tty->lookahead >= 0)
	    FD_SET(fd, read);
	  else
	    FD_CLR(fd, read);
	}
    }

  /* Assume (maybe wrong) that we can always write to the tty.  */
  if (write)
    FD_SET (fd, write);
  /* Only exceptional condition is Out-Of-Band data (on sockets).  */
  if (except)
    FD_CLR (fd, except);

  /* This may not be correct, but it is consistent with Internet 5
     select.  */
  return ((read && FD_ISSET(fd, read)) ? 1 : 0) + (write ? 1 : 0);
}
