/* warn (), warnc (), vwarn (), vwarnc (), warnx (), vwarnx (), err (),
   errc (), verr (), verrc (), errx (), verrx ().
   Copyright (c) 2012 UnixLib Developers.  */

#include <err.h>
#include <errno.h>
#include <kernel.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <swis.h>

#ifndef __TARGET_SCL__
#  include <internal/unix.h>
#endif

/* To be used to call the v{warn|err}{c|x} version instead.  */
#define CALL_VA(call, fmt) \
  { \
    va_list ap; \
    va_start (ap, fmt); \
    call; \
    va_end (ap); \
  }

#ifdef __TARGET_SCL__
static __inline__ const char * __attribute__ ((always_inline))
get_cli (void)
{
#if 1
  register const char *cli __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XOS_GetEnv]\n\t"
		    "MOVVS\tr0, #0\n\t"
		    : "=r" (cli)
		    : [SWI_XOS_GetEnv] "i" (OS_GetEnv | (1<<17))
		    : "r14", "cc");
  return cli;
#else
  return _kernel_command_string ();
#endif
}
#endif

/**
 * \param code err code, only be used when do_code is non-zero.
 * \param fmt When non-NULL printf style formatting string.
 * \param do_code non-zero when code needs to be outputed as well.
 */
static void
output (int code, const char *fmt, va_list ap, int do_code)
{
  char namebuf[48];
#ifndef __TARGET_SCL__
  const char *progname = __get_program_name (__u->argv[0], namebuf,
					     sizeof(namebuf));
  if (progname == NULL)
    progname = "";
#else
  const char *cli = get_cli ();
  const char *progname;
  if (cli == NULL)
    progname = "";
  else
    {
      /* Skip any initial whitespace.  */
      while (*cli == ' ')
	cli++;

      /* Find the end of the program name.  Set 'start' to mark the
	 beginning of the program name. Use '.' and ':' as separation
	 characters to calculate this.  */
      const char *start = cli;
      while (*cli != ' ' && *cli != '\0')
	{
	  if (*cli == '.' || *cli == ':')
	    /* Point to the character after the separation char.  */
	    start = cli + 1;
	  cli++;
	}
      const char * const end = cli;

      /* Copy the program name into 'out'.  Its bounds should now be
	 marked out by 'start' and 'end'.  */
      char *out = namebuf;
      size_t namebuf_len = sizeof (namebuf);
      while (--namebuf_len && start != end)
	*out++ = *start++;

      /* Zero terminate it.  */
      *out = '\0';

      progname = namebuf;
    }
#endif

  fprintf (stderr, "%s", progname);
  if (fmt)
    {
      fprintf (stderr, ": ");
      vfprintf (stderr, fmt, ap);
    }
  if (do_code)
    fprintf (stderr, ": %s", strerror (code));
  fputc ('\n', stderr);
}

void
warn (const char *fmt, ...)
{
  CALL_VA (vwarnc (errno, fmt, ap), fmt);
}

void
warnc (int code, const char *fmt, ...)
{
  CALL_VA (vwarnc (code, fmt, ap), fmt);
}

void
warnx (const char *fmt, ...)
{
  CALL_VA (vwarnx (fmt, ap), fmt);
}

void
vwarn (const char *fmt, va_list ap)
{
  vwarnc (errno, fmt, ap);
}

void
vwarnc (int code, const char *fmt, va_list ap)
{
  output (code, fmt, ap, 1);
}

void
vwarnx (const char *fmt, va_list ap)
{
  output (0, fmt, ap, 1);
}

void
err (int eval, const char *fmt, ...)
{
  CALL_VA (verrc (eval, errno, fmt, ap), fmt);
}

void
errc (int eval, int code, const char *fmt, ...)
{
  CALL_VA (verrc (eval, code, fmt, ap), fmt);
}

void
errx (int eval, const char *fmt, ...)
{
  CALL_VA (verrx (eval, fmt, ap), fmt);
}

void
verr (int eval, const char *fmt, va_list ap)
{
  verrc (eval, errno, fmt, ap);
}

void
verrc (int eval, int code, const char *fmt, va_list ap)
{
  vwarnc (code, fmt, ap);
  exit (eval);
}

void
verrx (int eval, const char *fmt, va_list ap)
{
  vwarnx (fmt, ap);
  exit (eval);
}

