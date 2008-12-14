/* Functions for RISC OS as target machine for GNU C compiler.
   Copyright (C) 1997, 1999, 2003, 2004, 2005 Free Software Foundation, Inc.
   Contributed by Nick Burrett <nick@sqrt.co.uk>,
   Alex Waugh <alex@alexwaugh.com> and John Tytgat <John.Tytgat@aaug.net>.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "safe-ctype.h"

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef CROSS_COMPILE
# include <string.h>
# include <limits.h>
# include <time.h>
# include <netdb.h>
# include <sys/socket.h>
# include <netinet/in.h>
#else
# include <swis.h>
# include <kernel.h>
# include <unixlib/local.h>
#endif

#include "cpplib.h"

#define THROWBACK_WARNING 0
#define THROWBACK_ERROR 1

#ifdef CROSS_COMPILE
/* The syslog facilities and priorities.  */
# define PRI_WARNING (1 * 8 + 4)
# define PRI_ERROR   (1 * 8 + 3)
#else
/* The DDEUtils module throwback error category codes.  */
# define DDEUTILS_THROWBACK_INFORMATION         -1
# define DDEUTILS_THROWBACK_WARNING              0
# define DDEUTILS_THROWBACK_ERROR                1
# define DDEUTILS_THROWBACK_SERIOUS_ERROR        2

/* The DDEUtils module throwback reason codes.  */
# define DDEUTILS_THROWBACK_REASON_PROCESSING    0
# define DDEUTILS_THROWBACK_REASON_ERROR_DETAILS 1
# define DDEUTILS_THROWBACK_REASON_INFO_DETAILS  2
#endif

#ifndef CROSS_COMPILE
/* The full RISC OS canonicalised pathname of the current error file.  */
static char *arm_error_file = NULL;
#endif

#if 0
/* Unique reference number for current file.
   Used to determine whether DDEUtils module needs telling we
   have changed the error file.  */
static int arm_error_file_ref = -1;
#endif

#ifdef CROSS_COMPILE
static int arm_throwback_socket = 0;
#endif

/* Control status of throwback,
   -1 => no throwback possible
    0 => throwback uninitialised,
    1 => throwback initialised
 */
static int arm_throwback_started = 0;

static void arm_throwback_finish (void);

static void
arm_throwback_start (void)
{
#ifdef CROSS_COMPILE
  struct hostent *hp;
  struct servent *servptr;
  struct sockaddr_in name;
  char *hostname;
  int port;

  hostname = getenv ("THROWBACK_HOST");
  if (hostname == NULL)
    {
      arm_throwback_started = -1;
      return;
    }

  hp = gethostbyname (hostname);
  if (hp == NULL)
    {
      arm_throwback_started = -1;
      return;
    }

  memset (&name, 0, sizeof (name));
  memcpy (&name.sin_addr, hp->h_addr, hp->h_length);
  name.sin_family = AF_INET;

  servptr = getservbyname ("syslog", "udp");
  if (servptr == NULL)
    port = 514;
  else
    port = servptr->s_port;

  name.sin_port = htons (port);

  if ((arm_throwback_socket = socket (AF_INET, SOCK_DGRAM, 0)) < 0
      || connect(arm_throwback_socket, (struct sockaddr *)&name,
		 sizeof (struct sockaddr_in)) < 0)
    {
      fprintf (stderr, "warning: failed to start syslog throwback\n");
      arm_throwback_started = -1;
      return;
    }

  atexit (arm_throwback_finish);
  arm_throwback_started = 1;
#else
  _kernel_swi_regs rin, rout;

  /* Initialise the DDEUtils module for throwback.  */
  if (!_kernel_swi (DDEUtils_ThrowbackStart, &rin, &rout))
    atexit (arm_throwback_finish);

  arm_throwback_started = 1;
#endif
}

#ifndef CROSS_COMPILE
/* Tell DDEUtils that we are processing a new file.
   The DDE documentation is unclear, but does suggest that this
   message should be sent if the filename changes.  */
static void
arm_throwback_new_file (const char *fname)
{
  _kernel_swi_regs rin, rout;

  rin.r[0] = DDEUTILS_THROWBACK_REASON_PROCESSING;
  rin.r[2] = (int) fname;
  if (_kernel_swi (DDEUtils_ThrowbackSend, &rin, &rout))
    arm_throwback_started = -1;
}
#endif

/* Send details of a specific error to DDEUtils module.  */
static void
arm_throwback_error (const char *fname, int level,
		     int line_number, const char *error, int errorlen)
{
#ifdef CROSS_COMPILE
  char msg[1024];
  int len;
  char hostname[100];
  char timestamp[20];
  time_t timer;
  char pathname[PATH_MAX];
  int syslog_lvl;

  if (gethostname (hostname, sizeof (hostname)) < 0)
    strcpy (hostname, "unknown");

  time (&timer);
  strftime (timestamp, sizeof (timestamp), "%a %d %H:%M:%S", localtime (&timer));

  if (realpath (fname, pathname) == NULL)
    snprintf (pathname, sizeof (pathname), "%s", fname);

  syslog_lvl = (level == THROWBACK_ERROR) ? PRI_ERROR : PRI_WARNING;
  for (len = snprintf (msg, sizeof (msg), "<%d>%s %s throwback %s:%d: ",
		       syslog_lvl, timestamp, hostname, pathname, line_number);
       len < (int) sizeof (msg) && errorlen > 0;
       ++error, --errorlen)
    {
      if (ISPRINT (*error))
        msg[len++] = *error;
    }

  if (len > (int) sizeof (msg))
    len = sizeof (msg);

  if (send (arm_throwback_socket, msg, len, 0) < 0)
    arm_throwback_started = -1;
#else
  _kernel_swi_regs rin, rout;

  rin.r[0] = (level == DDEUTILS_THROWBACK_INFORMATION)
	      ? DDEUTILS_THROWBACK_REASON_INFO_DETAILS
	      : DDEUTILS_THROWBACK_REASON_ERROR_DETAILS;
  rin.r[1] = 0;
  rin.r[2] = (int) fname;
  rin.r[3] = line_number;
  rin.r[4] = (level == DDEUTILS_THROWBACK_INFORMATION) ? 0 : level;
  rin.r[5] = (int) error;
  if (_kernel_swi (DDEUtils_ThrowbackSend, &rin, &rout))
    arm_throwback_started = -1;
#endif
}

/* Called via atexit().  */
static void
arm_throwback_finish (void)
{
#ifdef CROSS_COMPILE
  /* Close the socket.  */
  close (arm_throwback_socket);
  arm_throwback_socket = 0;
#else
  _kernel_swi_regs rin, rout;

  /* Tell DDEUtils that we have finished throwing errors.  */
  if (arm_throwback_started > 0)
    _kernel_swi (DDEUtils_ThrowbackEnd, &rin, &rout);
  arm_throwback_started = 0;
#endif
}

#ifndef CROSS_COMPILE
/* Convert from Unix name to RISC OS name and canonicalise the name
   so that throwback knows the full pathname of the file.
   Return the converted filename, stored in arm_error_file, or NULL.  */
static char *
riscos_canonicalise_filename (const char *sname)
{
  char *s;
  _kernel_swi_regs rin, rout;
  char filename[1024];

  /* It's possible that the filename will be in Unix format.
     Convert it into RISC OS format.  */
  s = __riscosify (sname, 0, 0, filename, sizeof (filename), NULL);
  /* Filename translation failed, so return NULL.  */
  if (s == NULL)
    return NULL;

  /* The first call will calculate the size of buffer needed to
     store the canonicalised filename.  */
  rin.r[0] = 37;
  rin.r[1] = (int) filename;
  rin.r[2] = 0;
  rin.r[3] = 0;
  rin.r[4] = 0;
  rin.r[5] = 0;
  if (!_kernel_swi (OS_FSControl, &rin, &rout) && rout.r[5] < 0)
    {
      arm_error_file = xrealloc (arm_error_file, 1 - rout.r[5]);
      /* Now perform the canonicalisation and store in a buffer
         which we know is large enough.  */
      rin.r[1] = (int) filename;
      rin.r[2] = (int) arm_error_file;
      rin.r[3] = 0;
      rin.r[4] = 0;
      rin.r[5] = 1 - rout.r[5];
      if (!_kernel_swi (OS_FSControl, &rin, &rout) && rout.r[5] == 1)
        return arm_error_file;
    }

  /* Canonicalisation failed, so free arm_error_file and set it to null.  */
  free (arm_error_file);
  arm_error_file = NULL;
  return NULL;
}
#endif

/* Throwback interface to GNU family of compilers.  */
void
arm_error_throwback (int lvl, const char *file, int line, const char *s,
		     va_list *va)
{
  char msg[256];

  if (!TARGET_THROWBACK)
    return;
  
  /* If the filename is not specified or "", then return.  */
  if (file == NULL || *file == '\0')
    return;

  vsnprintf (msg, sizeof (msg), s, *va);

  /* Initialise throwback.  */
  if (!arm_throwback_started)
    arm_throwback_start ();

  if (arm_throwback_started > 0
#ifndef CROSS_COMPILE
      && riscos_canonicalise_filename (file)
#endif
     )
    {
      size_t flen, slen;
      char sline[16];
      const char *p;

#ifndef CROSS_COMPILE
      arm_throwback_new_file (arm_error_file);
#endif

      flen = strlen (file);
      slen = sprintf (sline, "%d: ", line);
	    
      /* Errors generated by the C++ compiler and preprocessor can stretch
         over multiple lines.  DDEUtils uses the newline as a string
         terminator.  We must call arm_throwback_error() for as many times
         as there are newlines.  */
      for (p = msg; *p != '\0'; /* */)
	{
          const char *msg;

          /* Skip filename, line and level (warning/error) in the 'p' message:  */
          if (!strncmp (p, file, flen) && p[flen] == ':')
            {
              p += flen + 1; /* Skip filename + ":" */
              if (!strncmp (p, sline, slen))
                {
                  p += slen; /* Skip line num + ": " */
		  if (!strncmp (p, "error: ", sizeof ("error: ")-1))
		    p += sizeof ("error: ")-1;
                  else if (!strncmp (p, "warning: ", sizeof ("warning: ")-1))
                    p += sizeof ("warning: ")-1; /* Skip level */
                }
            }

	  for (msg = p; *p != '\0' && *p != '\n'; ++p)
	    /* */;
	  if (*p == '\n')
	    p++;

	  arm_throwback_error (
#ifdef CROSS_COMPILE
			       file,
#else
			       arm_error_file,
#endif
                               CPP_DL_WARNING_P(lvl) ? THROWBACK_WARNING : THROWBACK_ERROR,
			       line, msg, p - msg);
	}
    }
}

#if 0
/* Error category mapping for GNAT errors.  */
typedef enum
{
  cat_none,
  cat_warning,
  cat_error
} category;

/* Throwback an error from GNAT.
   The column parameter is unused, but maybe the throwback system will
   be able to use it sometime in the future.  */
void
arm_gnat_error_throwback (text, sname, sfile, line, column, cat)
  char *text;		/* The error message.  */
  char *sname;		/* The file containing the error.  */
  int  sfile;		/* A unique reference number for the file.  */
  int  line;		/* The line number of the error.  */
  int  column;		/* The column of the error.  */
  category cat;		/* The category of the error.  */
{
  /* If the filename is not specified or "", then return.  */
  if (sname == NULL || *sname == '\0')
    return;
  /* Initialise throwback.  */
  if (!arm_throwback_started)
    arm_throwback_start();

  if (arm_throwback_started > 0)
    {
      /* If this is a new file, then work out the RISC OS name and
         canonicalise the name so that throwback knows the full pathname
         of the file.  */
      if (sfile != arm_error_file_ref)
	{
	  arm_error_file_ref = sfile;
	  if (riscos_canonicalise_filename (sname))
	    arm_throwback_new_file (arm_error_file);
	  else
	    arm_throwback_started = -1;
	}
      /* Promote category none into category error.  */
      if (cat == cat_none)
        cat = cat_error;
      /* Throwback working, so send the message.  */
      if (arm_throwback_started > 0)
	arm_throwback_error (arm_error_file,
			     (cat == cat_none
			      ? DDEUTILS_THROWBACK_INFORMATION
			      : (cat == cat_warning
			         ? DDEUTILS_THROWBACK_WARNING : DDEUTILS_THROWBACK_ERROR)),
			     line, text);
    }
}
#endif

