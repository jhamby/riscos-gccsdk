/*
 * termios.h: POSIX Standard 7.1-2 General Terminal Interface.
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __TERMIOS_H
#define __TERMIOS_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

/* Get the system-dependent definitions of `struct termios', `tcflag_t',
   `cc_t', `speed_t', and all the macros specifying the flag bits.  */
typedef __speed_t speed_t;
typedef __tcflag_t tcflag_t;
typedef __cc_t cc_t;

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


/* Input flags - software input processing.  */
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

/* Output modes - software output proecssing.  */
#define	OPOST	(1 << 0)	/* Perform output processing.  */
#define	ONLCR	(1 << 1)	/* Map NL to CR-NL on output.  */
#define	OXTABS	(1 << 2)	/* Expand tabs to spaces.  */
#define	ONOEOT	(1 << 3)	/* Discard EOT (^D) on output.  */
#define OCRNL	(1 << 4)	/* Map CR to NL on output.  */
#define ONOCR	(1 << 5)	/* No CR output at column 0.  */
#define ONLRET	(1 << 6)	/* NL performs CR function.  */

/* Control flags - hardware control of terminal. */
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
#define CDTR_IFLOW	(1 << 18)	/* DTR flow control of input.  */
#define CDSR_OFLOW	(1 << 19)	/* DSR flow control of output.  */
#define CCAR_OFLOW	(1 << 20)	/* DCD flow control of output.  */
#define	MDMBUF		(1 << 20)	/* Carrier flow control of output.  */

/* Local modes.  */
#ifdef __USE_MISC
#define ECHOKE          0x00000001      /* visual erase for line kill */
#endif  /*_POSIX_SOURCE */
#define ECHOE           0x00000002      /* visually erase chars */
#define ECHOK           0x00000004      /* echo NL after line kill */
#define ECHO            0x00000008      /* enable echoing */
#define ECHONL          0x00000010      /* echo NL even if ECHO is off */
#ifdef __USE_MISC
#define ECHOPRT         0x00000020      /* visual erase mode for hardcopy */
#define ECHOCTL         0x00000040      /* echo control chars as ^(Char) */
#endif  /*_POSIX_SOURCE */
#define ISIG            0x00000080      /* enable signals INTR, QUIT, [D]SUSP */
#define ICANON          0x00000100      /* canonicalize input lines */
#ifndef _POSIX_SOURCE
#define ALTWERASE       0x00000200      /* use alternate WERASE algorithm */
#endif  /*_POSIX_SOURCE */
#define IEXTEN          0x00000400      /* enable DISCARD and LNEXT */
#define EXTPROC         0x00000800      /* external processing */
#define XCASE           0x00001000
#define TOSTOP          0x00400000      /* stop background jobs from output */
#ifndef _POSIX_SOURCE
#define FLUSHO          0x00800000      /* output being flushed (state) */
#define NOKERNINFO      0x02000000      /* no kernel output from VSTATUS */
#define PENDIN          0x20000000      /* XXX retype pending input (state) */
#endif  /*_POSIX_SOURCE */
#define NOFLSH          0x80000000      /* don't flush after interrupt */

/* Special control characters.  These are indexes into the c_cc[]
   character array.  The [xxx] is the flag that enables this
   control character.  */
#define	VEOF	0	/* End-of-file character [ICANON].  */
#define	VEOL	1	/* End-of-line character [ICANON].  */
#define	VEOL2	2	/* Second EOL character [ICANON].  */
#define	VERASE	3	/* Erase character [ICANON].  */
#define	VWERASE	4	/* Word-erase character [ICANON].  */
#define	VKILL	5	/* Kill-line character [ICANON].  */
#define	VREPRINT 6	/* Reprint-line character [ICANON].  */
#define VERASE2 7	/* Erase character [ICANON].  */

#define	VINTR	8	/* Interrupt character [ISIG].  */
#define	VQUIT	9	/* Quit character [ISIG].  */
#define	VSUSP	10	/* Suspend character [ISIG].  */
#define	VDSUSP	11	/* Delayed suspend character [ISIG].  */
#define	VSTART	12	/* Start (X-ON) character [IXON, IXOFF].  */
#define	VSTOP	13	/* Stop (X-OFF) character [IXON, IXOFF].  */
#define	VLNEXT	14	/* Literal-next character [IEXTEN].  */
#define	VDISCARD 15	/* Discard character [IEXTEN].  */
#define	VMIN	16	/* Minimum number of bytes read at once [!ICANON].  */
#define	VTIME	17	/* Time-out value (tenths of a second) [!ICANON].  */
#define	VSTATUS	18	/* Status character [ICANON].  */
#define	NCCS	20	/* Value duplicated in <hurd/tioctl.defs>.  */

/* Input and output baud rates.  */
#ifdef __USE_MISC
# define CBAUD		0x0000001F
#endif
#define	 B0		0x00000000	/* Hang up.  */
#define	 B50		0x00000001	/* 50 baud.  */
#define	 B75		0x00000002	/* 75 baud.  */
#define	 B110		0x00000003	/* 110 baud.  */
#define	 B134		0x00000004	/* 134.5 baud.  */
#define	 B150		0x00000005	/* 150 baud.  */
#define	 B200		0x00000006	/* 200 baud.  */
#define	 B300		0x00000007	/* 300 baud.  */
#define	 B600		0x00000008	/* 600 baud.  */
#define	 B1200		0x00000009	/* 1200 baud.  */
#define	 B1800		0x0000000A	/* 1800 baud.  */
#define	 B2400		0x0000000B	/* 2400 baud.  */
#define	 B4800		0x0000000C	/* 4800 baud.  */
#define	 B9600		0x0000000D	/* 9600 baud.  */
#define	 B19200		0x0000000E	/* 19200 baud.  */
#define	 B38400		0x0000000F	/* 38400 baud.  */
#ifdef __USE_MISC
# define EXTA		B19200
# define EXTB		B38400
#endif
#ifdef __USE_MISC
# define CBAUDEX	0x00000010
#endif
#define	 B7200		0x00000010	/* 7200 baud.  */
#define	 B14400		0x00000011	/* 14400 baud.  */
#define	 B28800		0x00000012	/* 28800 baud.  */
#define	 B57600		0x00000013	/* 57600 baud.  */
#define	 B76800		0x00000014	/* 76800 baud.  */
#define	 B115200	0x00000015	/* 115200 baud.  */
#define	 __MAX_BAUD	B115200

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
  tcflag_t c_iflag;

  /* Output modes.  */
  tcflag_t c_oflag;

  /* Control modes.  */
  tcflag_t c_cflag;

  /* Local modes.  */
  tcflag_t c_lflag;

  /* Control characters.  */
  cc_t c_cc[NCCS];

  /* Input and output baud rates.  */
  speed_t c_ispeed, c_ospeed;
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

__END_DECLS

#endif
