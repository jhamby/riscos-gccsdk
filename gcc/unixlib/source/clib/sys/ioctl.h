/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/ioctl,v $
 * $Date: 1997/12/17 22:02:52 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_IOCTL_H
#define __SYS_IOCTL_H

/* These macros are also defined in termios.h files (with numerically
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


#define	TIOCGSIZE	TIOCGWINSZ
#define	TIOCSSIZE	TIOCSWINSZ

/* Ioctl's have the command encoded in the lower word, and the size of
   any in or out parameters in the upper word.  The high 3 bits of the
   upper word are used to encode the in/out status of the parameter.  */
#define	IOCPARM_MASK	0x1fff		/* parameter length, at most 13 bits */
#define	IOCPARM_LEN(x)	(((x) >> 16) & IOCPARM_MASK)
#define	IOCBASECMD(x)	((x) & ~IOCPARM_MASK)
#define	IOCGROUP(x)	(((x) >> 8) & 0xff)

#define	IOCPARM_MAX	NBPG		/* max size of ioctl, mult. of NBPG */
#define	IOC_VOID	0x20000000	/* no parameters */
#define	IOC_OUT		0x40000000	/* copy out parameters */
#define	IOC_IN		0x80000000	/* copy in parameters */
#define	IOC_INOUT	(IOC_IN|IOC_OUT)
#define	IOC_DIRMASK	0xe0000000	/* mask for IN/OUT/VOID */

#define _IOC(inout,group,num,len) \
	(inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num))
#define	_IO(g,n)	_IOC(IOC_VOID,	(g), (n), 0)
#define	_IOR(g,n,t)	_IOC(IOC_OUT,	(g), (n), sizeof(t))
#define	_IOW(g,n,t)	_IOC(IOC_IN,	(g), (n), sizeof(t))
/* this should be _IORW, but stdio got there first */
#define	_IOWR(g,n,t)	_IOC(IOC_INOUT,	(g), (n), sizeof(t))

#define	TIOCMODG	_IOR('t', 3, int)	/* get modem control state */
#define	TIOCMODS	_IOW('t', 4, int)	/* set modem control state */
#define		TIOCM_LE	0001		/* line enable */
#define		TIOCM_DTR	0002		/* data terminal ready */
#define		TIOCM_RTS	0004		/* request to send */
#define		TIOCM_ST	0010		/* secondary transmit */
#define		TIOCM_SR	0020		/* secondary receive */
#define		TIOCM_CTS	0040		/* clear to send */
#define		TIOCM_CAR	0100		/* carrier detect */
#define		TIOCM_CD	TIOCM_CAR
#define		TIOCM_RNG	0200		/* ring */
#define		TIOCM_RI	TIOCM_RNG
#define		TIOCM_DSR	0400		/* data set ready */
						/* 8-10 compat */
#define	TIOCEXCL	_IO('t', 13)		/* set exclusive use of tty */
#define	TIOCNXCL	_IO('t', 14)		/* reset exclusive use of tty */
						/* 15 unused */
#define	TIOCFLUSH	_IOW('t', 16, int)	/* flush buffers */
						/* 17-18 compat */
#define	TIOCGETA	_IOR('t', 19, struct termios) /* get termios struct */
#define	TIOCSETA	_IOW('t', 20, struct termios) /* set termios struct */
#define	TIOCSETAW	_IOW('t', 21, struct termios) /* drain output, set */
#define	TIOCSETAF	_IOW('t', 22, struct termios) /* drn out, fls in, set */
#define	TIOCGETD	_IOR('t', 26, int)	/* get line discipline */
#define	TIOCSETD	_IOW('t', 27, int)	/* set line discipline */
						/* 127-124 compat */
#define	TIOCSBRK	_IO('t', 123)		/* set break bit */
#define	TIOCCBRK	_IO('t', 122)		/* clear break bit */
#define	TIOCSDTR	_IO('t', 121)		/* set data terminal ready */
#define	TIOCCDTR	_IO('t', 120)		/* clear data terminal ready */
#define	TIOCGPGRP	_IOR('t', 119, int)	/* get pgrp of tty */
#define	TIOCSPGRP	_IOW('t', 118, int)	/* set pgrp of tty */
						/* 117-116 compat */
#define	TIOCOUTQ	_IOR('t', 115, int)	/* output queue size */
#define	TIOCSTI		_IOW('t', 114, char)	/* simulate terminal input */
#define	TIOCNOTTY	_IO('t', 113)		/* void tty association */
#define	TIOCPKT		_IOW('t', 112, int)	/* pty: set/clear packet mode */
#define		TIOCPKT_DATA		0x00	/* data packet */
#define		TIOCPKT_FLUSHREAD	0x01	/* flush packet */
#define		TIOCPKT_FLUSHWRITE	0x02	/* flush packet */
#define		TIOCPKT_STOP		0x04	/* stop output */
#define		TIOCPKT_START		0x08	/* start output */
#define		TIOCPKT_NOSTOP		0x10	/* no more ^S, ^Q */
#define		TIOCPKT_DOSTOP		0x20	/* now do ^S ^Q */
#define		TIOCPKT_IOCTL		0x40	/* state change of pty driver */
#define	TIOCSTOP	_IO('t', 111)		/* stop output, like ^S */
#define	TIOCSTART	_IO('t', 110)		/* start output, like ^Q */
#define	TIOCMSET	_IOW('t', 109, int)	/* set all modem bits */
#define	TIOCMBIS	_IOW('t', 108, int)	/* bis modem bits */
#define	TIOCMBIC	_IOW('t', 107, int)	/* bic modem bits */
#define	TIOCMGET	_IOR('t', 106, int)	/* get all modem bits */
#define	TIOCREMOTE	_IOW('t', 105, int)	/* remote input editing */
#define	TIOCGWINSZ	_IOR('t', 104, struct winsize)	/* get window size */
#define	TIOCSWINSZ	_IOW('t', 103, struct winsize)	/* set window size */
#define	TIOCUCNTL	_IOW('t', 102, int)	/* pty: set/clr usr cntl mode */
#define		UIOCCMD(n)	_IO('u', n)		/* usr cntl op "n" */
#define	TIOCCONS	_IOW('t', 98, int)		/* become virtual console */
#define	TIOCSCTTY	_IO('t', 97)		/* become controlling tty */
#define	TIOCEXT		_IOW('t', 96, int)	/* pty: external processing */
#define	TIOCSIG		_IO('t', 95)		/* pty: generate signal */
#define TIOCDRAIN	_IO('t', 94)		/* wait till output drained */

#define TTYDISC		0		/* termios tty line discipline */
#define	TABLDISC	3		/* tablet discipline */
#define	SLIPDISC	4		/* serial IP discipline */


/* Compatibility with 4.3 BSD terminal driver.
   From 4.4 <sys/ioctl_compat.h>.  */

#ifdef USE_OLD_TTY
# undef  TIOCGETD
# define TIOCGETD	_IOR('t', 0, int)	/* get line discipline */
# undef  TIOCSETD
# define TIOCSETD	_IOW('t', 1, int)	/* set line discipline */
#else
# define OTIOCGETD	_IOR('t', 0, int)	/* get line discipline */
# define OTIOCSETD	_IOW('t', 1, int)	/* set line discipline */
#endif
#define	TIOCHPCL	_IO('t', 2)		/* hang up on last close */
#define	TIOCGETP	_IOR('t', 8,struct sgttyb)/* get parameters -- gtty */
#define	TIOCSETP	_IOW('t', 9,struct sgttyb)/* set parameters -- stty */
#define	TIOCSETN	_IOW('t',10,struct sgttyb)/* as above, but no flushtty*/
#define	TIOCSETC	_IOW('t',17,struct tchars)/* set special characters */
#define	TIOCGETC	_IOR('t',18,struct tchars)/* get special characters */
#define		TANDEM		0x00000001	/* send stopc on out q full */
#define		CBREAK		0x00000002	/* half-cooked mode */
#define		LCASE		0x00000004	/* simulate lower case */
#define		ECHO		0x00000008	/* echo input */
#define		CRMOD		0x00000010	/* map \r to \r\n on output */
#define		RAW		0x00000020	/* no i/o processing */
#define		ODDP		0x00000040	/* get/send odd parity */
#define		EVENP		0x00000080	/* get/send even parity */
#define		ANYP		0x000000c0	/* get any parity/send none */
#define		NLDELAY		0x00000300	/* \n delay */
#define			NL0	0x00000000
#define			NL1	0x00000100	/* tty 37 */
#define			NL2	0x00000200	/* vt05 */
#define			NL3	0x00000300
#define		TBDELAY		0x00000c00	/* horizontal tab delay */
#define			TAB0	0x00000000
#define			TAB1	0x00000400	/* tty 37 */
#define			TAB2	0x00000800
#define		XTABS		0x00000c00	/* expand tabs on output */
#define		CRDELAY		0x00003000	/* \r delay */
#define			CR0	0x00000000
#define			CR1	0x00001000	/* tn 300 */
#define			CR2	0x00002000	/* tty 37 */
#define			CR3	0x00003000	/* concept 100 */
#define		VTDELAY		0x00004000	/* vertical tab delay */
#define			FF0	0x00000000
#define			FF1	0x00004000	/* tty 37 */
#define		BSDELAY		0x00008000	/* \b delay */
#define			BS0	0x00000000
#define			BS1	0x00008000
#define		ALLDELAY	(NLDELAY|TBDELAY|CRDELAY|VTDELAY|BSDELAY)
#define		CRTBS		0x00010000	/* do backspacing for crt */
#define		PRTERA		0x00020000	/* \ ... / erase */
#define		CRTERA		0x00040000	/* " \b " to wipe out char */
#define		TILDE		0x00080000	/* hazeltine tilde kludge */
#define		MDMBUF		0x00100000	/*start/stop output on carrier*/
#define		LITOUT		0x00200000	/* literal output */
#define		TOSTOP		0x00400000	/*SIGSTOP on background output*/
#define		FLUSHO		0x00800000	/* flush output to terminal */
#define		NOHANG		0x01000000	/* (no-op) was no SIGHUP on carrier drop */
#define		L001000		0x02000000
#define		CRTKIL		0x04000000	/* kill line with " \b " */
#define		PASS8		0x08000000
#define		CTLECH		0x10000000	/* echo control chars as ^X */
#define		PENDIN		0x20000000	/* tp->t_rawq needs reread */
#define		DECCTQ		0x40000000	/* only ^Q starts after ^S */
#define		NOFLSH		0x80000000	/* no output flush on signal */
#define	TIOCLBIS	_IOW('t', 127, int)	/* bis local mode bits */
#define	TIOCLBIC	_IOW('t', 126, int)	/* bic local mode bits */
#define	TIOCLSET	_IOW('t', 125, int)	/* set entire local mode word */
#define	TIOCLGET	_IOR('t', 124, int)	/* get local modes */
#define		LCRTBS		(CRTBS>>16)
#define		LPRTERA		(PRTERA>>16)
#define		LCRTERA		(CRTERA>>16)
#define		LTILDE		(TILDE>>16)
#define		LMDMBUF		(MDMBUF>>16)
#define		LLITOUT		(LITOUT>>16)
#define		LTOSTOP		(TOSTOP>>16)
#define		LFLUSHO		(FLUSHO>>16)
#define		LNOHANG		(NOHANG>>16)
#define		LCRTKIL		(CRTKIL>>16)
#define		LPASS8		(PASS8>>16)
#define		LCTLECH		(CTLECH>>16)
#define		LPENDIN		(PENDIN>>16)
#define		LDECCTQ		(DECCTQ>>16)
#define		LNOFLSH		(NOFLSH>>16)
#define	TIOCSLTC	_IOW('t',117,struct ltchars)/* set local special chars*/
#define	TIOCGLTC	_IOR('t',116,struct ltchars)/* get local special chars*/
#define OTIOCCONS	_IO('t', 98)	/* for hp300 -- sans int arg */
#define	OTTYDISC	0
#define	NETLDISC	1
#define	NTTYDISC	2

/* File I/O.  */
#define	FIOCLEX		_IO('f', 1)		/* set close on exec on fd */
#define	FIONCLEX	_IO('f', 2)		/* remove close on exec */
#define FIONREAD	_IOR('f', 127, int)	/* Get number of bytes to read */
#define FIONBIO		_IOW('f', 126, int)	/* Set/clear non-blocking i/o */
#define FIOASYNC	_IOW('f', 125, int)	/* Set/clear asynchronous i/o */
#define	FIOSETOWN	_IOW('f', 124, int)	/* set owner */
#define	FIOGETOWN	_IOR('f', 123, int)	/* get owner */
#define FIOSLEEPTW	_IOW('f', 121, int)	/* Set/clear OS_Upcall 6 use */

/* Socket I/O.  */
#define SIOCSHIWAT	_IOW('s',  0, int)		/* Set high watermark */
#define SIOCGHIWAT	_IOR('s',  1, int)		/* Get high watermark */
#define SIOCSLOWAT	_IOW('s',  2, int)		/* Set low watermark */
#define SIOCGLOWAT	_IOR('s',  3, int)		/* Get low watermark */
#define SIOCATMARK	_IOR('s',  7, int)		/* At oob mark? */
#define	SIOCSPGRP	_IOW('s',  8, int)		/* set process group */
#define	SIOCGPGRP	_IOR('s',  9, int)		/* get process group */

#define	SIOCADDRT	_IOW('r', 10, struct ortentry)	/* add route */
#define	SIOCDELRT	_IOW('r', 11, struct ortentry)	/* delete route */
#define SIOCSIFADDR	_IOW('i', 12, struct ifreq)   /* Set ifnet address */
#define OSIOCGIFADDR    _IOWR('i', 13, struct ifreq)   /* Get ifnet address */
#define SIOCSIFDSTADDR  _IOW('i', 14, struct ifreq)   /* Set p-p address */
#define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq)   /* Get p-p address */
#define SIOCSIFFLAGS	_IOW('i', 16, struct ifreq)   /* Set ifnet flags */
#define SIOCGIFFLAGS   _IOWR('i', 17, struct ifreq)   /* Get ifnet flags */
#define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq)   /* Get broadcast addr */
#define SIOCSIFBRDADDR  _IOW('i', 19, struct ifreq)   /* Set broadcast addr */
#define OSIOCGIFCONF    _IOWR('i', 20, struct ifconf)  /* Get ifnet list */
#define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq)   /* Get net addr mask */
#define SIOCSIFNETMASK  _IOW('i', 22, struct ifreq)   /* Set net addr mask */
#define SIOCGIFMETRIC  _IOWR('i', 23, struct ifreq)   /* Get IF metric */
#define SIOCSIFMETRIC	_IOW('i', 24, struct ifreq)   /* Set IF metric */
#define	SIOCDIFADDR	_IOW('i', 25, struct ifreq)	/* delete IF addr */
#define	SIOCAIFADDR	_IOW('i', 26, struct ifaliasreq)	/* add/chg IF alias */
#define	SIOCSARP	_IOW('i', 30, struct arpreq)	/* set arp entry */
#define	OSIOCGARP	_IOWR('i',31, struct arpreq)	/* get arp entry */
#define	SIOCDARP	_IOW('i', 32, struct arpreq)	/* delete arp entry */
#define	SIOCGIFADDR	_IOWR('i',33, struct ifreq)	/* get ifnet address */
#define	SIOCGIFDSTADDR	_IOWR('i',34, struct ifreq)	/* get p-p address */
#define	SIOCGIFBRDADDR	_IOWR('i',35, struct ifreq)	/* get broadcast addr */
#define	SIOCGIFCONF	_IOWR('i',36, struct ifconf)	/* get ifnet list */
#define	SIOCGIFNETMASK	_IOWR('i',37, struct ifreq)	/* get net addr mask */
#define	SIOCGARP	_IOWR('i',38, struct arpreq)	/* get arp entry */
#define	SIOCADDMULTI	 _IOW('i', 49, struct ifreq)	/* add m'cast addr */
#define	SIOCDELMULTI	 _IOW('i', 50, struct ifreq)	/* del m'cast addr */

/* TTY I/O.  */
#if	defined(TIOCGETC) || defined(TIOCSETC)
/* Type of ARG for TIOCGETC and TIOCSETC requests.  */
struct tchars
{
  char t_intrc;			/* Interrupt character.  */
  char t_quitc;			/* Quit character.  */
  char t_startc;		/* Start-output character.  */
  char t_stopc;			/* Stop-output character.  */
  char t_eofc;			/* End-of-file character.  */
  char t_brkc;			/* Input delimiter character.  */
};

#define	_IOT_tchars	/* Hurd ioctl type field.  */ \
  _IOT (_IOTS (char), 6, 0, 0, 0, 0)
#endif

#if	defined(TIOCGLTC) || defined(TIOCSLTC)
/* Type of ARG for TIOCGLTC and TIOCSLTC requests.  */
struct ltchars
{
  char t_suspc;			/* Suspend character.  */
  char t_dsuspc;		/* Delayed suspend character.  */
  char t_rprntc;		/* Reprint-line character.  */
  char t_flushc;		/* Flush-output character.  */
  char t_werasc;		/* Word-erase character.  */
  char t_lnextc;		/* Literal-next character.  */
};

#define	_IOT_ltchars	/* Hurd ioctl type field.  */ \
  _IOT (_IOTS (char), 6, 0, 0, 0, 0)
#endif

/* Type of ARG for TIOCGETP and TIOCSETP requests (and gtty and stty).  */
struct sgttyb
{
  char sg_ispeed;		/* Input speed.  */
  char sg_ospeed;		/* Output speed.  */
  char sg_erase;		/* Erase character.  */
  char sg_kill;			/* Kill character.  */
  short int sg_flags;		/* Mode flags.  */
};

#define	_IOT_sgttyb	/* Hurd ioctl type field.  */ \
  _IOT (_IOTS (char), 6, _IOTS (short int), 1, 0, 0)

#if	defined(TIOCGWINSZ) || defined(TIOCSWINSZ)
/* Type of ARG for TIOCGWINSZ and TIOCSWINSZ requests.  */
struct winsize
{
  unsigned short int ws_row;	/* Rows, in characters.  */
  unsigned short int ws_col;	/* Columns, in characters.  */

  /* These are not actually used.  */
  unsigned short int ws_xpixel;	/* Horizontal pixels.  */
  unsigned short int ws_ypixel;	/* Vertical pixels.  */
};

#define	_IOT_winsize	/* Hurd ioctl type field.  */ \
  _IOT (_IOTS (unsigned short int), 4, 0, 0, 0, 0)
#endif

#if	defined (TIOCGSIZE) || defined (TIOCSSIZE)
/* The BSD-style ioctl constructor macros use `sizeof', which can't be used
   in a preprocessor conditional.  Since the commands are always unique
   regardless of the size bits, we can safely define away `sizeof' for the
   purpose of the conditional.  */
#  define sizeof(type) 0
#  if defined (TIOCGWINSZ) && TIOCGSIZE == TIOCGWINSZ
/* Many systems that have TIOCGWINSZ define TIOCGSIZE for source
   compatibility with Sun; they define `struct ttysize' to have identical
   layout as `struct winsize' and #define TIOCGSIZE to be TIOCGWINSZ
   (likewise TIOCSSIZE and TIOCSWINSZ).  */
struct ttysize
{
  unsigned short int ts_lines;
  unsigned short int ts_cols;
  unsigned short int ts_xxx;
  unsigned short int ts_yyy;
};
#define	_IOT_ttysize	_IOT_winsize
#  else
/* Suns use a different layout for `struct ttysize', and TIOCGSIZE and
   TIOCGWINSZ are separate commands that do the same thing with different
   structures (likewise TIOCSSIZE and TIOCSWINSZ).  */
struct ttysize
{
  int ts_lines, ts_cols;	/* Lines and columns, in characters.  */
};
#  endif
#  undef sizeof			/* See above.  */
#endif



#ifdef __cplusplus
extern "C" {
#endif

/* Union for generic ioctl() argument handling.  */
union ioctl_arg
{
  char *cparg;		/* Pointer to character */
  char carg;		/* Character */
  int  *iparg;		/* Pointer to integer */
  int  iarg;		/* Integer */
  long *lparg;		/* Pointer to long */
  long larg;		/* Long */
};

/* Perform the I/O control operation specified by 'request' on 'fd'.
   -1 usually indicates an error.  */
extern int ioctl (int __fd, int __request, void *arg);

#ifdef __cplusplus
}
#endif

#endif
