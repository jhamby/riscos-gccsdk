/*
RTS initialization. Signal handlers.

Copyright (C) 1991-99 Free Software Foundation, Inc.

This file is part of the GNU Pascal Library. The GNU Pascal
Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

The GNU Pascal Library is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; see the file COPYING.LIB.
If not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "rts.h"
#include "fdr.h"
#include <sys/stat.h>
#include <locale.h>

#ifdef HAVE_PROCESS_H
#include <process.h> /* some systems, e.g. mingw, need it for getpid */
#endif

int _p_signal   = 0; /* Use _p_sigcause if this signal appears */
int _p_sigcause = 0; /* Number of error message to give if _p_signal */
int _p_debug    = 0;
int _p_argc     = 1;
Boolean _p_force_direct_files = FALSE; /* TRUE if direct access routines should work only for direct access files. */
char *_p_argv_dummy [] = { "GPC program", NULL }, **_p_argv = _p_argv_dummy;
FileAssociation *_p_FileAssociation = NULL;

/* Set to 1 if you want to use the EOLn hack by default.
 *
 * If 0: EOLn will validate the file buffer if tst_UND
 *
 * If 1, when EOLn is tested when all of the following are true
 *       1) tst_UND
 *       2) tst_EMPTY (nothing has been read after reset)
 *       3) tst_TXT
 *       4) tst_TTY
 *       5) tst_LGET
 *
 *       If these are true, then the EOLn test return true.
 *
 *       If the EOLn is *not tested* it is false. This is
 *       to make the program with 'if EOLn then readln;' in the
 *       very beginning work, they skip the EOLn when they test
 *       it, if you don't test, you don't have to skip it.
 */
Boolean _p_eoln_reset_hack = FALSE;

/* Program standard input, output and error */
struct Fdr
  _p_stdin  = { 0, 0, 0, 0, 0, 0, 0, "uninitialized Input",
                0, 0, FiNOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  _p_stdout = { 0, 0, 0, 0, 0, 0, 0, "uninitialized Output",
                0, 0, FiNOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  _p_stderr = { 0, 0, 0, 0, 0, 0, 0, "uninitialized StdErr",
                0, 0, FiNOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

FDR _p_current_stdin = NULL;

/* FDR list head pointers.
   Add an FDR to the list when reset/rewritten, remove it on close.
   List is needeed to flush buffered output to terminal
    1) when program crashes, dump everything before giving error message
    2) when something is read from terminal opened as a TEXT file
       (need to flush output from other TTY files before get)
*/
FDR _p_last_fdr  = NULL;
FDR _p_first_fdr = NULL;

void
_p_init_heap ()
{
  static int init_heap_done = 0;
  if (init_heap_done) return;
  init_heap_done++;
#if 0
  malloc_init (0, _p_heap_warning);
#endif
  _p_heap_init ();
}

void
_p_update_pid ()
{
  _p_pid = getpid ();
}

void
_p_init_misc ()
{
  _p_update_pid ();
  setlocale (LC_ALL, "");
}

void
_p_initialize_stderr ()
{
  static int init_stderr_done = 0;
  if (init_stderr_done) return;
  init_stderr_done++;
  _p_initfdr (&_p_stderr, "StdErr", 8, STD_FILE_FLAGS);
  _p_rewrite (&_p_stderr, NULL, 0);
}

void
_p_initialize_std_files ()
{
  static int init_std_files_done = 0;
  if (init_std_files_done) return;
  init_std_files_done++;
  _p_initfdr (&_p_stdout, "Output", 8, STD_FILE_FLAGS);
  _p_rewrite (&_p_stdout, NULL, 0);
  _p_initfdr (&_p_stdin, "Input", 8, STD_FILE_FLAGS);
  _p_reset   (&_p_stdin, NULL, 0);
}

/* One time routine that restores the original standard input if we are
 * not reading from there now. `-i' arguments use this. */
int
_p_restore_stdin (File)
FDR File;
{
  int ttyflag;
  if (!_p_current_stdin) return 0;
  _p_close (_p_current_stdin);
  _p_donefdr (_p_current_stdin);
  _p_dispose (_p_current_stdin);
  _p_current_stdin = NULL;
  m_FILNUM (File) = stdin;
  File->Handle = fileno (m_FILNUM (File));
  ttyflag = _p_checkfiletype (File, foReset, TRUE);
  if (File->ReadFunc == _p_f_read_tty || File->ReadFunc == _p_f_read)
    {
      if (ttyflag)
        File->ReadFunc = _p_f_read_tty;
      else
        File->ReadFunc = _p_f_read;
    }
  return 1;
}

void
_p_exit (n)
int n;
{
  _p_done_files ();
  /* if the exit status would be 0 due to truncation, make it 1 in order
     to not indicate success erroneously :-( */
  exit (n + (n != 0 && (n & 0xff) == 0));
}

void
_p_fflush (only_ttys)
Boolean only_ttys;
{
  FDR scan;
  /* flush buffers to synchronize output messages */
  for (scan = _p_first_fdr; scan; scan = m_NXTFDR(scan))
    /* DON'T flush stdin; SunOS starts to behave strangely :-) */
    if (m_STATUS(scan) & FiWRI && (!only_ttys || tst_TTY(scan)) && scan != &_p_stdin)
      _p_flush (scan);
}

void
_p_done_files ()
{
  FDR scan, next;
  _p_fflush (FALSE);
  /* clean up all open files */
  scan = _p_first_fdr;
  while (scan)
    {
      next = m_NXTFDR (scan); /* m_NXTFDR (scan) will be destroyed by _p_donefdr () */
      if (scan != &_p_stdin && scan != &_p_stdout && scan != &_p_stderr) /* @@ This is a kludge because of the wrong order of executing finalizers */
        _p_donefdr (scan);
      scan = next;
    }
}

#if BSD_RTS==1

#define SIGTOMASK(n) (1<<((n) - 1))

struct sig_expl
{
  int   sig, how, code, gpcn;
  char *expl;
} expls [] =
{
  {SIGHUP,  ABORT, NONE,             UND, "hangup"},
  {SIGINT,  ABORT, NONE,             FAST, "interrupt"},
  {SIGQUIT, ABORT, NONE,             UND, "quit"},
#ifdef vax
  {SIGILL,  ABORT, ILL_RESAD_FAULT,  UND, "reserved adressing"},
  {SIGILL,  ABORT, ILL_PRIVIN_FAULT, UND, "privileged instruction"},
  {SIGILL,  ABORT, ILL_RESOP_FAULT,  385, "reserved operand"},
#else
  {SIGILL,  ABORT, NONE,             UND, "illegal instruction"},
#endif
  {SIGTRAP, ABORT, NONE,             UND, "trap"},
  {SIGIOT,  ABORT, NONE,             UND, "iot"},
  {SIGEMT,  ABORT, NONE,             UND, "emt"},
#ifdef vax
  {SIGFPE,  ABORT, FPE_INTOVF_TRAP,  713, "integer overflow trap"},
  {SIGFPE,  ABORT, FPE_INTDIV_TRAP,  712, "integer divide by zero trap"},
  {SIGFPE,  ABORT, FPE_FLTOVF_TRAP,  715, "floating overflow trap"},
  {SIGFPE,  ABORT, FPE_FLTDIV_TRAP,  711, "floating divide by zero trap"},
  {SIGFPE, REPORT, FPE_FLTUND_TRAP,  716, "floating underflow trap"},
  {SIGFPE,  ABORT, FPE_DECOVF_TRAP,  UND, "decimal overflow"},
  {SIGFPE,  ABORT, FPE_SUBRNG_TRAP,  UND, "subscript"},
  {SIGFPE,  ABORT, FPE_FLTOVF_FAULT, 715, "floating overflow fault"},
  {SIGFPE,  ABORT, FPE_FLTDIV_FAULT, 711, "floating divide by zero fault"},
  {SIGFPE, REPORT, FPE_FLTUND_FAULT, 716, "floating underflow fault"},
#else
  {SIGFPE,  ABORT, NONE,             UND, "floating point trap"},
#endif
  {SIGKILL, ABORT, NONE,             UND, "kill"},
  {SIGBUS,  ABORT, NONE,             UND, "bus error"},
  {SIGSEGV, ABORT, NONE,             UND, "memory fault"},
  {SIGSYS,  ABORT, NONE,             UND, "bad system call"},
  {SIGPIPE, ABORT, NONE,             UND, "broken pipe"},
  {SIGALRM, ABORT, NONE,             UND, "alarm"},
  {SIGTERM, ABORT, NONE,             UND, "termination"},
  {SIGURG,  ABORT, NONE,             UND, "urgent"},
  {-1,      ABORT, NONE,             0,   0}
};

static int
sigvecn (sigs, handler)
int sigs;
void (*handler) ();
{
  struct sigvec sv, osv;
  int i, error = 0, omask = sigsetmask (-1);
  sv.sv_handler = (void *) handler;
  sv.sv_mask = sigs;
  sv.sv_onstack = 0;
  for (i = 0; i < 32; i++)
    if (sigs >> (i - 1) & 1)
      if (sigvec (i, &sv, &osv)) error++;
  sigsetmask (omask);
  return error;
}

static void
_p_handler (sig, code, scp)
int sig, code;
struct sigcontext *scp;
{
  struct sig_expl *p;
  int Warning;
  char *msg;
  for (p = expls; p->sig >= 0; p++)
    if (p->sig == sig && p->code == code)
      break;
  if (p->sig < 0) p = NULL;
  if (p && p->how == IGNORE)
    return;
  Warning = p && p->how == REPORT;
  D (1, fprintf(stderr, "_p_handler: warning %d\n", Warning));
  if (_p_signal == sig)
    {
      msg = _p_errmsg (_p_sigcause); /* Preset message number */
      _p_signal = 0;
    }
  else
    msg = (p ? ((msg = _p_errmsg (p->gpcn)) ? msg : p->expl) : NULL);
  _p_prmessage (msg, (p ? p->gpcn : UND), Warning);
  if (Warning) return;
  fprintf (stderr, (p && p->gpcn >= UND) ? "Exiting (with core dump)\n" : "Exiting\n");
  _p_fflush (FALSE);
  _cleanup (); /* In libc -> flsbuf.c */
  sigvecn (SIGTOMASK (SIGILL), SIG_DFL);
  if (p && p->gpcn >= UND)
    {
      sigsetmask (0);
      kill (getpid (), SIGILL);
    }
  _p_exit (1);
}

void _p_init_signals ()
{
  static int signals_done = 0;
  if (signals_done) return;
  signals_done++;
  sigvecn (SIGTOMASK (SIGHUP)
         | SIGTOMASK (SIGINT)
      /* | SIGTOMASK (SIGQUIT) */
         | SIGTOMASK (SIGILL)
         | SIGTOMASK (SIGTRAP)
         | SIGTOMASK (SIGIOT)
         | SIGTOMASK (SIGEMT)
         | SIGTOMASK (SIGFPE)
         | SIGTOMASK (SIGBUS)
         | SIGTOMASK (SIGSEGV)
         | SIGTOMASK (SIGSYS)
         | SIGTOMASK (SIGPIPE)
         | SIGTOMASK (SIGTERM)
         | SIGTOMASK (SIGXCPU)
         | SIGTOMASK (SIGXFSZ), _p_handler);
}

#else

void _p_init_signals ()
{
}

#endif
