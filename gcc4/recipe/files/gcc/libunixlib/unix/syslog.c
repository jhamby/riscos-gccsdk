/*
 * Copyright (c) 1983, 1988, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <swis.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#include <pthread.h>
#include <internal/unix.h>
#include <internal/os.h>

static int LogStat = 0;		/* status bits, set by openlog() */
static const char *LogTag = NULL;	/* string to tag the entry with */
static int LogFacility = LOG_USER;	/* default facility code */
static int LogMask = 0xff;	/* mask of priorities to be logged */

/*
 * syslog, vsyslog --
 *      print message on log file; output is intended for syslogd(8).
 */
void
syslog (int pri, const char *fmt,...)
{
  va_list ap;

  va_start (ap, fmt);
  vsyslog (pri, fmt, ap);
  va_end (ap);
}

void
vsyslog (int pri, const char *fmt, va_list ap)
{
  int cnt;
  char ch, *p, *t;
  time_t now;
  int fd, saved_errno;
  char *stdp = NULL, tbuf[2048], fmt_cpy[1024];
  char *msg;
  int regs[10];

  PTHREAD_UNSAFE

#define	INTERNALLOG	LOG_ERR|LOG_CONS|LOG_PERROR|LOG_PID
  /* Check for invalid bits. */
  if (pri & ~(LOG_PRIMASK | LOG_FACMASK))
    {
      syslog (INTERNALLOG,
	      "syslog: unknown facility/priority: %x", pri);
      pri &= LOG_PRIMASK | LOG_FACMASK;
    }

  /* Check priority against setlogmask values. */
  if (!LOG_MASK (LOG_PRI (pri)) & LogMask)
    return;

  saved_errno = errno;

  /* Set default facility if none specified. */
  if ((pri & LOG_FACMASK) == 0)
    pri |= LogFacility;

  /* Build the message. */
  (void) time (&now);
  p = tbuf + sprintf (tbuf, "<%d>", pri);
  p += strftime (p, sizeof (tbuf) - (p - tbuf), "%h %e %T ",
		 localtime (&now));
  if (LogStat & LOG_PERROR)
    stdp = p;
  if (LogTag == NULL)
    {
      char *leaf;
      /* We must strip out any <foo$dir>. or foo: before the leafname of
	 the executable, as the name passed to SysLog_Message is used
	 as part of a filename.  */
      leaf = strrchr(__u->argv[0],'.');
      if (leaf == NULL)
	leaf = __u->argv[0];
      else
	leaf ++;
      LogTag = strrchr(leaf,':');
      if (LogTag == NULL)
	LogTag = leaf;
      else
	LogTag ++;
    }
  if (LogTag != NULL)
    p += sprintf (p, "%s", LogTag);
  if (LogStat & LOG_PID)
    p += sprintf (p, "[%d]", getpid ());
  if (LogTag != NULL)
    {
      *p++ = ':';
      *p++ = ' ';
    }

  /* Substitute error message for %m. */
  for (t = fmt_cpy; (ch = *fmt); ++fmt)
    if (ch == '%' && fmt[1] == 'm')
      {
	++fmt;
	t += sprintf (t, "%s", strerror (saved_errno));
      }
    else
      *t++ = ch;
  *t = '\0';

  msg = p;
  p += vsprintf (p, fmt_cpy, ap);
  cnt = p - tbuf;

  /* Output to stderr if requested. */
  if (LogStat & LOG_PERROR)
    {
      (void) write (STDERR_FILENO, stdp, cnt - (stdp - tbuf));
      (void) write (STDERR_FILENO, (const void *) "\n", 1);
    }

  /* Ignore errors as SysLog module is probably not loaded.
     The mapping between Unix and RISC OS SysLog priorities is a bit
     arbitrary.  */
  static const int unx2ro[8] = { 20, 30, 40, 60, 80, 100, 150, 200 };
  regs[0] = (int) LogTag;
  regs[1] = (int) msg;
  regs[2] = unx2ro[LOG_PRI (pri)];
  __os_swi (SysLog_LogMessage, regs);

  /* Output the message to the console; don't worry about blocking,
   * if console blocks everything will.  Make sure the error reported
   * is the one from the syslogd failure.
   */
  if (LogStat & LOG_CONS &&
      (fd = open ("/dev/tty", O_WRONLY, 0)) >= 0)
    {
      (void) strcat (tbuf, "\r\n");
      cnt += 2;
      p = index (tbuf, '>') + 1;
      (void) write (fd, p, cnt - (p - tbuf));
      (void) close (fd);
    }
}


void
openlog (const char *ident, int logstat, int logfac)
{
  PTHREAD_UNSAFE

  if (ident != NULL)
    LogTag = ident;
  LogStat = logstat;
  if (logfac != 0 && (logfac & ~LOG_FACMASK) == 0)
    LogFacility = logfac;
}

void
closelog (void)
{
}

/* setlogmask -- set the log mask level */
int
setlogmask (int pmask)
{
  int omask;

  PTHREAD_UNSAFE

  omask = LogMask;
  if (pmask != 0)
    LogMask = pmask;
  return (omask);
}
