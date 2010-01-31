/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __INTERNAL_TTY_H
#define __INTERNAL_TTY_H

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

/* Beware of backwards compatibility when altering this structure.
   It is referenced by SUL and other processes */
struct tty
  {
  int type;             /* tty type */
  unsigned int refcount;
  struct termios t[1];
  struct winsize w[1];
  int sx,cx;		/* screen x, character x */
  int cnt;		/* number of characters in input buffer */
  char *ptr;		/* read pointer in input buffer */
  int lookahead;	/* [1 byte or -1] lookahead to allow select() */
  char buf[MAX_INPUT];  /* input buffer */
  char del[MAX_INPUT];  /* number of displayed characters for character cx */
  };


__END_DECLS

#endif
