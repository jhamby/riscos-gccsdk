/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/termios,v $
 * $Date: 1998/06/25 22:12:31 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

/* termios.h: POSIX Standard 7.1-2 General Terminal Interface.  */

#ifndef __TERMIOS_H
#define __TERMIOS_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Get the system-dependent definitions of `struct termios', `tcflag_t',
   `cc_t', `speed_t', and all the macros specifying the flag bits.  */
/* These macros are also defined in some ioctls.h files (with numerically
   identical values), but this serves to shut up cpp's complaining. */

#ifdef MDMBUF
#undef MDMBUF
#endif
#ifdef FLUSHO
#undef FLUSHO
#endif
#ifdef PENDIN
#undef PENDIN
#endif


#ifdef ECHO
#undef ECHO
#endif
#ifdef TOSTOP
#undef TOSTOP
#endif
#ifdef NOFLSH
#undef NOFLSH
#endif


/* These definitions match those used by the 4.4 BSD kernel.
   If the operating system has termios system calls or ioctls that
   correctly implement the POSIX.1 behavior, there should be a
   system-dependent version of this file that defines `struct termios',
   `tcflag_t', `cc_t', `speed_t' and the `TC*' constants appropriately.  */



/* Input modes.  */
#define	IGNBRK	(1 << 0)	/* Ignore break condition.  */
#define	BRKINT	(1 << 1)	/* Signal interrupt on break.  */
#define	IGNPAR	(1 << 2)	/* Ignore characters with parity errors.  */
#define	PARMRK	(1 << 3)	/* Mark parity and framing errors.  */
#define	INPCK	(1 << 4)	/* Enable input parity check.  */
#define	ISTRIP	(1 << 5)	/* Strip 8th bit off characters.  */
#define	INLCR	(1 << 6)	/* Map NL to CR on input.  */
#define	IGNCR	(1 << 7)	/* Ignore CR.  */
#define	ICRNL	(1 << 8)	/* Map CR to NL on input.  */
#define	IXON	(1 << 9)	/* Enable start/stop output control.  */
#define	IXOFF	(1 << 10)	/* Enable start/stop input control.  */
#define	IXANY	(1 << 11)	/* Any character will restart after stop.  */
#define	IMAXBEL	(1 << 13)	/* Ring bell when input queue is full.  */

/* Output modes.  */
#define	OPOST	(1 << 0)	/* Perform output processing.  */
#define	ONLCR	(1 << 1)	/* Map NL to CR-NL on output.  */
#define	OXTABS	(1 << 2)	/* Expand tabs to spaces.  */
#define	ONOEOT	(1 << 8)	/* Discard EOT (^D) on output.  */

/* Control modes.  */
#define	CIGNORE	(1 << 0)	/* Ignore these control flags.  */
#define	CSIZE	(CS5|CS6|CS7|CS8)	/* Number of bits per byte (mask).  */
#define	CS5	0		/* 5 bits per byte.  */
#define	CS6	(1 << 8)	/* 6 bits per byte.  */
#define	CS7	(1 << 9)	/* 7 bits per byte.  */
#define	CS8	(CS6|CS7)	/* 8 bits per byte.  */
#define	CSTOPB	(1 << 10)	/* Two stop bits instead of one.  */
#define	CREAD	(1 << 11)	/* Enable receiver.  */
#define	PARENB	(1 << 12)	/* Parity enable.  */
#define	PARODD	(1 << 13)	/* Odd parity instead of even.  */
#define	HUPCL	(1 << 14)	/* Hang up on last close.  */
#define	CLOCAL	(1 << 15)	/* Ignore modem status lines.  */
#define	CCTS_OFLOW	(1 << 16)	/* CTS flow control of output.  */
#define	CRTS_IFLOW	(1 << 17)	/* RTS flow control of input.  */
#define	MDMBUF		(1 << 20)	/* Carrier flow control of output.  */

/* Local modes.  */
#define	ECHOKE	(1 << 0)	/* Visual erase for KILL.  */
#define	_ECHOE	(1 << 1)	/* Visual erase for ERASE.  */
#define	ECHOE	_ECHOE
#define	_ECHOK	(1 << 2)	/* Echo NL after KILL.  */
#define	ECHOK	_ECHOK
#define	_ECHO	(1 << 3)	/* Enable echo.  */
#define	ECHO	_ECHO
#define	_ECHONL	(1 << 4)	/* Echo NL even if ECHO is off.  */
#define	ECHONL	_ECHONL
#define	ECHOPRT	(1 << 5)	/* Hardcopy visual erase.  */
#define	ECHOCTL	(1 << 6)	/* Echo control characters as ^X.  */
#define	_ISIG	(1 << 7)	/* Enable signals.  */
#define	ISIG	_ISIG
#define	_ICANON	(1 << 8)	/* Do erase and kill processing.  */
#define	ICANON	_ICANON
#define	ALTWERASE (1 << 9)	/* Alternate WERASE algorithm.  */
#define	_IEXTEN	(1 << 10)	/* Enable DISCARD and LNEXT.  */
#define	IEXTEN	_IEXTEN
#define	EXTPROC	(1 << 11)	/* External processing.  */
#define	_TOSTOP	(1 << 22)	/* Send SIGTTOU for background output.  */
#define	TOSTOP	_TOSTOP
#define	FLUSHO	(1 << 23)	/* Output being flushed (state).  */
#define	NOKERNINFO (1 << 25)	/* Disable VSTATUS.  */
#define	PENDIN	(1 << 29)	/* Retype pending input (state).  */
#define	_NOFLSH	(1 << 31)	/* Disable flush after interrupt.  */
#define	NOFLSH	_NOFLSH

/* Control characters.  */
#define	VEOF	0		/* End-of-file character [ICANON].  */
#define	VEOL	1		/* End-of-line character [ICANON].  */
#define	VEOL2	2		/* Second EOL character [ICANON].  */
#define	VERASE	3		/* Erase character [ICANON].  */
#define	VWERASE	4		/* Word-erase character [ICANON].  */
#define	VKILL	5		/* Kill-line character [ICANON].  */
#define	VREPRINT 6		/* Reprint-line character [ICANON].  */
#define	VINTR	8		/* Interrupt character [ISIG].  */
#define	VQUIT	9		/* Quit character [ISIG].  */
#define	VSUSP	10		/* Suspend character [ISIG].  */
#define	VDSUSP	11		/* Delayed suspend character [ISIG].  */
#define	VSTART	12		/* Start (X-ON) character [IXON, IXOFF].  */
#define	VSTOP	13		/* Stop (X-OFF) character [IXON, IXOFF].  */
#define	VLNEXT	14		/* Literal-next character [IEXTEN].  */
#define	VDISCARD 15		/* Discard character [IEXTEN].  */
#define	VMIN	16		/* Minimum number of bytes read at once [!ICANON].  */
#define	VTIME	17		/* Time-out value (tenths of a second) [!ICANON].  */
#define	VSTATUS	18		/* Status character [ICANON].  */
#define	NCCS	20		/* Value duplicated in <hurd/tioctl.defs>.  */

/* Input and output baud rates.  */
#define	B0	0		/* Hang up.  */
#define	B50	50		/* 50 baud.  */
#define	B75	75		/* 75 baud.  */
#define	B110	110		/* 110 baud.  */
#define	B134	134		/* 134.5 baud.  */
#define	B150	150		/* 150 baud.  */
#define	B200	200		/* 200 baud.  */
#define	B300	300		/* 300 baud.  */
#define	B600	600		/* 600 baud.  */
#define	B1200	1200		/* 1200 baud.  */
#define	B1800	1800		/* 1800 baud.  */
#define	B2400	2400		/* 2400 baud.  */
#define	B4800	4800		/* 4800 baud.  */
#define	B9600	9600		/* 9600 baud.  */
#define	B19200	19200		/* 19200 baud.  */
#define	B38400	38400		/* 38400 baud.  */
#define	EXTA	19200
#define	EXTB	38400


#define _IOT_termios /* Hurd ioctl type field.  */ \
  _IOT (_IOTS (tcflag_t), 4, _IOTS (cc_t), NCCS, _IOTS (speed_t), 2)

/* Values for the OPTIONAL_ACTIONS argument to `tcsetattr'.  */
#define	TCSANOW		0	/* Change immediately.  */
#define	TCSADRAIN	1	/* Change when pending output is written.  */
#define	TCSAFLUSH	2	/* Flush pending input before changing.  */
#define	TCSASOFT	0x10	/* Flag: Don't alter hardware state.  */

/* Values for the QUEUE_SELECTOR argument to `tcflush'.  */
#define	TCIFLUSH	1	/* Discard data received but not yet read.  */
#define	TCOFLUSH	2	/* Discard data written but not yet sent.  */
#define	TCIOFLUSH	3	/* Discard all pending data.  */

/* Values for the ACTION argument to `tcflow'.  */
#define	TCOOFF	1		/* Suspend output.  */
#define	TCOON	2		/* Restart suspended output.  */
#define	TCIOFF	3		/* Send a STOP character.  */
#define	TCION	4		/* Send a START character.  */

/* Compare a character C to a value VAL from the `c_cc' array in a
   `struct termios'.  If VAL is _POSIX_VDISABLE, no character can match it.  */
#define	CCEQ(val, c)	((c) == (val) && (val) != _POSIX_VDISABLE)


/* Defaults on "first" open.  */
#define	TTYDEF_IFLAG	(BRKINT | ISTRIP | ICRNL | IMAXBEL | IXON | IXANY)
#define TTYDEF_OFLAG	(OPOST | ONLCR | OXTABS)
#define TTYDEF_LFLAG	(ECHO | ICANON | ISIG | IEXTEN | ECHOE|ECHOKE|ECHOCTL)
#define TTYDEF_CFLAG	(CREAD | CS7 | PARENB | HUPCL)
#define TTYDEF_SPEED	(B9600)

/* Control Character Defaults.  */
#define CTRL(x)	(x&037)
#define	CEOF		CTRL('d')
#define	CEOL		((unsigned char)'\377')	/* XXX avoid _POSIX_VDISABLE */
#define	CERASE		0177
#define	CINTR		CTRL('c')
#define	CSTATUS		((unsigned char)'\377')	/* XXX avoid _POSIX_VDISABLE */
#define	CKILL		CTRL('u')
#define	CMIN		1
#define	CQUIT		034		/* FS, ^\ */
#define	CSUSP		CTRL('z')
#define	CTIME		0
#define	CDSUSP		CTRL('y')
#define	CSTART		CTRL('q')
#define	CSTOP		CTRL('s')
#define	CLNEXT		CTRL('v')
#define	CDISCARD 	CTRL('o')
#define	CWERASE 	CTRL('w')
#define	CREPRINT 	CTRL('r')
#define	CEOT		CEOF
/* compat */
#define	CBRK		CEOL
#define CRPRNT		CREPRINT
#define	CFLUSH		CDISCARD


/* Terminal control structure.  */
struct termios
{
  /* Input modes.  */
  __tcflag_t c_iflag;

  /* Output modes.  */
  __tcflag_t c_oflag;

  /* Control modes.  */
  __tcflag_t c_cflag;

  /* Local modes.  */
  __tcflag_t c_lflag;

  /* Control characters.  */
  __cc_t c_cc[NCCS];

  /* Input and output baud rates.  */
  __speed_t __ispeed, __ospeed;
};

/* Return the output baud rate stored in *TERMIOS_P.  */
extern __speed_t cfgetospeed (const struct termios *__termios_p);

/* Return the input baud rate stored in *TERMIOS_P.  */
extern __speed_t cfgetispeed (const struct termios *__termios_p);

/* Set the output baud rate stored in *TERMIOS_P to SPEED.  */
extern int cfsetospeed (struct termios *__termios_p, __speed_t __speed);

/* Set the input baud rate stored in *TERMIOS_P to SPEED.  */
extern int cfsetispeed (struct termios *__termios_p, __speed_t __speed);

/* Set both the input and output baud rates in *TERMIOS_OP to SPEED.  */
extern void cfsetspeed (struct termios *__termios_p, __speed_t __speed);

/* Put the state of FD into *TERMIOS_P.  */
extern int tcgetattr (int __fd, struct termios *__termios_p);

/* Set the state of FD to *TERMIOS_P.
   Values for OPTIONAL_ACTIONS (TCSA*) are in <termbits.h>.  */
extern int tcsetattr (int __fd, int __optional_actions,
			   const struct termios *__termios_p);


/* Set *TERMIOS_P to indicate raw mode.  */
extern void cfmakeraw (struct termios *__termios_p);

/* Send zero bits on FD.  */
extern int tcsendbreak (int __fd, int __duration);

/* Wait for pending output to be written on FD.  */
extern int tcdrain (int __fd);

/* Flush pending data on FD.
   Values for QUEUE_SELECTOR (TC{I,O,IO}FLUSH) are in <termbits.h>.  */
extern int tcflush (int __fd, int __queue_selector);

/* Suspend or restart transmission on FD.
   Values for ACTION (TC[IO]{OFF,ON}) are in <termbits.h>.  */
extern int tcflow (int __fd, int __action);


#ifdef __cplusplus
}
#endif

#endif
