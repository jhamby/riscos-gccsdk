/*
C routines of the RTS

Copyright (C) 1991-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         Peter Gerwinski <peter@gerwinski.de>
         Frank Heckenbach <frank@pascal.gnu.de>

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

As a special exception, if you link this file with files compiled
with a GNU compiler to produce an executable, this does not cause
the resulting executable to be covered by the GNU General Public
License. This exception does not however invalidate any other
reasons why the executable file might be covered by the GNU General
Public License.
*/

#include "rts.h"

#ifdef HAVE_MATH_H
#include <math.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#if defined(HAVE_ENVIRON) && !defined(environ)
extern char **environ;
#endif

/* Substitutes and placeholders for declarations not present on all systems */

#ifndef HAVE_SINL
#define sinl sin
#endif
#ifndef HAVE_COSL
#define cosl cos
#endif
#ifndef HAVE_SQRTL
#define sqrtl sqrt
#endif
#ifndef HAVE_LOGL
#define logl log
#endif
#ifndef HAVE_EXPL
#define expl exp
#endif
#ifndef HAVE_POWL
#define powl pow
#endif
#ifndef HAVE_ATANL
#define atanl atan
#endif
#ifndef WIFEXITED
#define WIFEXITED(S) (((S) & 0xff) == 0)
#endif
#ifndef WEXITSTATUS
#define WEXITSTATUS(S) ((((unsigned) (S)) & 0xff00) >> 8)
#endif
#ifndef WIFSIGNALED
#define WIFSIGNALED(S) (((S) & 0xff) != 0 && ((S) & 0xff) != 0x7f)
#endif
#ifndef WTERMSIG
#define WTERMSIG(S) ((S) & 0x7f)
#endif
#ifndef WIFSTOPPED
#define WIFSTOPPED(S) (((S) & 0xff) == 0x7f)
#endif
#ifndef WSTOPSIG
#define WSTOPSIG(S) ((((unsigned) (S)) & 0xff00) >> 8)
#endif
#ifndef SA_RESTART
#define SA_RESTART 0
#endif
#ifndef SV_INTERRUPT
#define SV_INTERRUPT 0
#endif
#ifndef SIG_DFL
#define SIG_DFL ((void *) 0)
#endif
#ifndef SIG_IGN
#define SIG_IGN ((void *) 1)
#endif
#ifndef SIG_ERR
#define SIG_ERR ((void *) -1)
#endif
#ifndef SIGHUP
#define SIGHUP 0
#endif
#ifndef SIGINT
#define SIGINT 0
#endif
#ifndef SIGQUIT
#define SIGQUIT 0
#endif
#ifndef SIGILL
#define SIGILL 0
#endif
#ifndef SIGABRT
#define SIGABRT 0
#endif
#ifndef SIGFPE
#define SIGFPE 0
#endif
#ifndef SIGKILL
#define SIGKILL 0
#endif
#ifndef SIGSEGV
#define SIGSEGV 0
#endif
#ifndef SIGPIPE
#define SIGPIPE 0
#endif
#ifndef SIGALRM
#define SIGALRM 0
#endif
#ifndef SIGTERM
#define SIGTERM 0
#endif
#ifndef SIGUSR1
#define SIGUSR1 0
#endif
#ifndef SIGUSR2
#define SIGUSR2 0
#endif
#ifndef SIGCHLD
#ifdef SIGCLD
#define SIGCHLD SIGCLD
#else
#define SIGCHLD 0
#endif
#endif
#ifndef SIGCONT
#define SIGCONT 0
#endif
#ifndef SIGSTOP
#define SIGSTOP 0
#endif
#ifndef SIGTSTP
#define SIGTSTP 0
#endif
#ifndef SIGTTIN
#define SIGTTIN 0
#endif
#ifndef SIGTTOU
#define SIGTTOU 0
#endif
#ifndef SIGTRAP
#define SIGTRAP 0
#endif
#ifndef SIGIOT
#define SIGIOT 0
#endif
#ifndef SIGEMT
#define SIGEMT 0
#endif
#ifndef SIGBUS
#define SIGBUS 0
#endif
#ifndef SIGSYS
#define SIGSYS 0
#endif
#ifndef SIGSTKFLT
#define SIGSTKFLT 0
#endif
#ifndef SIGURG
#define SIGURG 0
#endif
#ifndef SIGIO
#define SIGIO 0
#endif
#ifndef SIGPOLL
#define SIGPOLL 0
#endif
#ifndef SIGXCPU
#define SIGXCPU 0
#endif
#ifndef SIGXFSZ
#define SIGXFSZ 0
#endif
#ifndef SIGVTALRM
#define SIGVTALRM 0
#endif
#ifndef SIGPROF
#define SIGPROF 0
#endif
#ifndef SIGPWR
#define SIGPWR 0
#endif
#ifndef SIGINFO
#define SIGINFO 0
#endif
#ifndef SIGLOST
#define SIGLOST 0
#endif
#ifndef SIGWINCH
#define SIGWINCH 0
#endif
#ifndef ILL_RESAD_FAULT
#define ILL_RESAD_FAULT (-1)
#endif
#ifndef ILL_PRIVIN_FAULT
#define ILL_PRIVIN_FAULT (-1)
#endif
#ifndef ILL_RESOP_FAULT
#define ILL_RESOP_FAULT (-1)
#endif
#ifndef FPE_INTOVF_TRAP
#define FPE_INTOVF_TRAP (-1)
#endif
#ifndef FPE_INTDIV_TRAP
#define FPE_INTDIV_TRAP (-1)
#endif
#ifndef FPE_SUBRNG_TRAP
#define FPE_SUBRNG_TRAP (-1)
#endif
#ifndef FPE_FLTOVF_TRAP
#ifdef FPE_FLTOVF_FAULT
#define FPE_FLTOVF_TRAP FPE_FLTOVF_FAULT
#else
#define FPE_FLTOVF_TRAP (-1)
#endif
#endif
#ifndef FPE_FLTDIV_TRAP
#ifdef FPE_FLTDIV_FAULT
#define FPE_FLTDIV_TRAP FPE_FLTDIV_FAULT
#else
#define FPE_FLTDIV_TRAP (-1)
#endif
#endif
#ifndef FPE_FLTUND_TRAP
#ifdef FPE_FLTUND_FAULT
#define FPE_FLTUND_TRAP FPE_FLTUND_FAULT
#else
#define FPE_FLTUND_TRAP (-1)
#endif
#endif
#ifndef FPE_DECOVF_TRAP
#define FPE_DECOVF_TRAP (-1)
#endif

typedef void (*TSignalHandler) (int);

TSignalHandler _p_SIG_DFL = SIG_DFL, _p_SIG_IGN = SIG_IGN, _p_SIG_ERR = SIG_ERR;

int _p_SIGHUP    = SIGHUP,    _p_SIGINT  = SIGINT,  _p_SIGQUIT  = SIGQUIT,
    _p_SIGILL    = SIGILL,    _p_SIGABRT = SIGABRT, _p_SIGFPE   = SIGFPE,
    _p_SIGKILL   = SIGKILL,   _p_SIGSEGV = SIGSEGV, _p_SIGPIPE  = SIGPIPE,
    _p_SIGALRM   = SIGALRM,   _p_SIGTERM = SIGTERM, _p_SIGUSR1  = SIGUSR1,
    _p_SIGUSR2   = SIGUSR2,   _p_SIGCHLD = SIGCHLD, _p_SIGCONT  = SIGCONT,
    _p_SIGSTOP   = SIGSTOP,   _p_SIGTSTP = SIGTSTP, _p_SIGTTIN  = SIGTTIN,
    _p_SIGTTOU   = SIGTTOU,   _p_SIGTRAP = SIGTRAP, _p_SIGIOT   = SIGIOT,
    _p_SIGEMT    = SIGEMT,    _p_SIGBUS  = SIGBUS,  _p_SIGSYS   = SIGSYS,
    _p_SIGSTKFLT = SIGSTKFLT, _p_SIGURG  = SIGURG,  _p_SIGIO    = SIGIO,
    _p_SIGPOLL   = SIGPOLL,   _p_SIGXCPU = SIGXCPU, _p_SIGXFSZ  = SIGXFSZ,
    _p_SIGVTALRM = SIGVTALRM, _p_SIGPROF = SIGPROF, _p_SIGPWR   = SIGPWR,
    _p_SIGINFO   = SIGINFO,   _p_SIGLOST = SIGLOST, _p_SIGWINCH = SIGWINCH;

int _p_ILL_RESAD_FAULT  = ILL_RESAD_FAULT,
    _p_ILL_PRIVIN_FAULT = ILL_PRIVIN_FAULT,
    _p_ILL_RESOP_FAULT  = ILL_RESOP_FAULT,
    _p_FPE_INTOVF_TRAP  = FPE_INTOVF_TRAP,
    _p_FPE_INTDIV_TRAP  = FPE_INTDIV_TRAP,
    _p_FPE_SUBRNG_TRAP  = FPE_SUBRNG_TRAP,
    _p_FPE_FLTOVF_TRAP  = FPE_FLTOVF_TRAP,
    _p_FPE_FLTDIV_TRAP  = FPE_FLTDIV_TRAP,
    _p_FPE_FLTUND_TRAP  = FPE_FLTUND_TRAP,
    _p_FPE_DECOVF_TRAP  = FPE_DECOVF_TRAP;

static void check_errno ()
{
  switch (errno)
    {
      case ERANGE : _p_error (700); /* Overflow in exponentiation */
      case EDOM   : _p_error (701); /* In `x pow y', x must be >= 0 if y < 0 and y is not an integer */
      default     : _p_internal_error_integer (922, (long int) errno); /* undocumented errno value %d in exponentiation */
    }
}

Boolean _p_isinf (long double) GCC_CONST;
Boolean _p_isinf (long double x)
{
  #ifdef HAVE_ISINFL
  return isinfl (x) ? TRUE : FALSE;
  #else
  return (x + x == x) && (x != 0);
  #endif
}

Boolean _p_isnan (long double) GCC_CONST;
Boolean _p_isnan (long double x)
{
  #ifdef HAVE_ISNANL
  return isnanl (x) ? TRUE : FALSE;
  #else
  return (((x == 0) ? 1 : 0) + ((x < 0) ? 1 : 0) + ((x > 0) ? 1 : 0)) != 1;
  #endif
}

void _p_frexp (long double, int*, long double *);
void _p_frexp (long double x, int *exp, long double *mantissa)
{
  #ifdef HAVE_FREXPL
  *mantissa = frexpl (x, exp);
  #else
  *mantissa = frexp (x, exp);
  #endif
}

double _p_sin (double);
double _p_sin (double x)
{
  return sin (x);
}

double _p_sinh (double);
double _p_sinh (double x)
{
  return sinh (x);
}

double _p_cos (double);
double _p_cos (double x)
{
  return cos (x);
}

double _p_cosh (double);
double _p_cosh (double x)
{
  return cosh (x);
}

double _p_arctan (double);
double _p_arctan (double x)
{
  return atan (x);
}

double _p_arctan2 (double, double);
double _p_arctan2 (double y, double x)
{
  return atan2 (y, x);
}

double _p_sqrt (double);
double _p_sqrt (double x)
{
  if (x < 0.0) _p_error (708); /* argument to `Sqrt' is < 0 */
  return sqrt (x);
}

double _p_ln (double);
double _p_ln (double x)
{
  if (x <= 0.0) _p_error (707); /* argument to `Ln' is <= 0 */
  return log (x);
}

double _p_exp (double);
double _p_exp (double x)
{
  double ret;
  errno = 0;
  ret = exp (x);
  if (errno) DO_RETURN_ADDRESS (check_errno ());
  return ret;
}

double _p_expon (double, double);
double _p_expon (double x, double y)
{
  double rval;
  errno = 0;
  rval = pow (x, y);
  if (errno) DO_RETURN_ADDRESS (check_errno ());
  return rval;
}

long double _p_l_sin (long double);
long double _p_l_sin (long double x)
{
  return sinl (x);
}

long double _p_l_cos (long double);
long double _p_l_cos (long double x)
{
  return cosl (x);
}

long double _p_l_arctan (long double);
long double _p_l_arctan (long double x)
{
  return atanl (x);
}

long double _p_l_sqrt (long double);
long double _p_l_sqrt (long double x)
{
  if (x < 0.0) _p_error (708); /* argument to `Sqrt' is < 0 */
  return sqrtl (x);
}

long double _p_l_ln (long double);
long double _p_l_ln (long double x)
{
  if (x <= 0.0) _p_error (707); /* argument to `Ln' is <= 0 */
  return logl (x);
}

long double _p_l_exp (long double);
long double _p_l_exp (long double x)
{
  long double ret;
  errno = 0;
  ret = expl (x);
  if (errno) DO_RETURN_ADDRESS (check_errno ());
  return ret;
}

long double _p_l_expon (long double, long double);
long double _p_l_expon (long double x, long double y)
{
  long double rval;
  errno = 0;
  rval = powl (x, y);
  if (errno) DO_RETURN_ADDRESS (check_errno ());
  return rval;
}

Char _p_gpc_upcase (Char) GCC_CONST;
Char _p_gpc_upcase (Char ch)
{
  return toupper (ch);
}

Char _p_gpc_locase (Char) GCC_CONST;
Char _p_gpc_locase (Char ch)
{
  return tolower (ch);
}

Boolean _p_isupcase (Char) GCC_CONST;
Boolean _p_isupcase (Char ch)
{
  return isupper (ch) != 0;
}

Boolean _p_islocase (Char) GCC_CONST;
Boolean _p_islocase (Char ch)
{
  return islower (ch) != 0;
}

Boolean _p_isalphanum (Char) GCC_CONST;
Boolean _p_isalphanum (Char ch)
{
  return isalnum (ch) != 0;
}

Boolean _p_isprintable (Char) GCC_CONST;
Boolean _p_isprintable (Char ch)
{
  return isprint (ch) != 0;
}

void _p_sleep (int);
void _p_sleep (int Seconds)
{
  int i = Seconds;
  do
    i = sleep (i);
  while (i != 0);
}

void _p_unix_time_to_time (UnixTimeType, int *, int *, int *, int *, int *, int *);
void _p_unix_time_to_time (UnixTimeType Time, int *Year, int *Month, int *Day, int *Hour, int *Minute, int *Second)
{
  if (Time >= 0)
    {
      time_t seconds = (time_t) Time;
      struct tm *gnu = localtime (&seconds);
      *Year   = gnu->tm_year;
      *Month  = gnu->tm_mon + 1; /* 1 = January */
      *Day    = gnu->tm_mday;
      *Hour   = gnu->tm_hour;
      *Minute = gnu->tm_min;
      *Second = gnu->tm_sec;
      if (*Year < 1000) *Year += 1900;
    }
  else
    *Year = *Month = *Day = *Hour = *Minute = *Second = - 1;
}

UnixTimeType _p_time_to_unix_time (int, int, int, int, int, int);
UnixTimeType _p_time_to_unix_time (int Year, int Month, int Day, int Hour, int Minute, int Second)
{
  struct tm gnu; /* It is a trademark after all }:--)= */
  memset (&gnu, 0, sizeof (gnu));
  gnu.tm_isdst = -1;
  gnu.tm_year  = Year - 1900;
  gnu.tm_mon   = Month - 1; /* 1 = January */
  gnu.tm_mday  = Day;
  gnu.tm_hour  = Hour;
  gnu.tm_min   = Minute;
  gnu.tm_sec   = Second;
  return (UnixTimeType) mktime (&gnu);
}

UnixTimeType _p_get_unix_time (int *);
UnixTimeType _p_get_unix_time (int *MicroSecond)
{
  #ifdef HAVE_GETTIMEOFDAY
  struct timeval tval;
  if (!gettimeofday (&tval, 0))
    {
      if (MicroSecond) *MicroSecond = tval.tv_usec;
      return tval.tv_sec;
    }
  #elif defined (HAVE_TIME)
  if (MicroSecond) *MicroSecond = 0;
  return (UnixTimeType) time ((time_t *) 0);
  #endif
  if (MicroSecond) *MicroSecond = 0;
  return - 1;
}

int _p_get_cpu_time (int *);
void _p_init_time (void);
#ifdef HAVE_GETRUSAGE
int _p_get_cpu_time (int *MicroSecond)
{
  int ms;
  struct rusage u;
  getrusage (RUSAGE_SELF, &u);
  ms = u.ru_utime.tv_usec + u.ru_stime.tv_usec;
  if (MicroSecond) *MicroSecond = ms % 1000000;
  return (int) u.ru_utime.tv_sec + u.ru_stime.tv_sec + ms / 1000000;
}

void _p_init_time ()
{
}
#else
static clock_t initial_clocks = 0;
int _p_get_cpu_time (int *MicroSecond)
{
  clock_t clocks = clock () - initial_clocks;
  if (MicroSecond) *MicroSecond = ((int) ((((long long int) clocks) * 1000000) / CLOCKS_PER_SEC)) % 1000000;
  return (int) (clocks / CLOCKS_PER_SEC);
}

void _p_init_time ()
{
  initial_clocks = clock ();
}
#endif

Boolean _p_WIfExited (int) GCC_CONST;
Boolean _p_WIfExited (int Status)
{
  return WIFEXITED (Status) ? TRUE : FALSE;
}

int _p_WExitStatus (int) GCC_CONST;
int _p_WExitStatus (int Status)
{
  return WEXITSTATUS (Status);
}

Boolean _p_WIfSignaled (int) GCC_CONST;
Boolean _p_WIfSignaled (int Status)
{
  return WIFSIGNALED (Status) ? TRUE : FALSE;
}

int _p_WTermSig (int) GCC_CONST;
int _p_WTermSig (int Status)
{
  return WTERMSIG (Status);
}

Boolean _p_WIfStopped (int) GCC_CONST;
Boolean _p_WIfStopped (int Status)
{
  (void) Status;
  return WIFSTOPPED (Status) ? TRUE : FALSE;
}

int _p_WStopSig (int) GCC_CONST;
int _p_WStopSig (int Status)
{
  (void) Status;
  return WSTOPSIG (Status);
}

int _p_csystem (char *);
int _p_csystem (char *CmdLine)
{
  return system (CmdLine);
}

char **_p_get_start_environment (char **);
char **_p_get_start_environment (char **ValueIfNotFound)
{
  #if defined(HAVE_ENVIRON) || defined(environ)
  (void) ValueIfNotFound;
  return environ;
  #else
  return ValueIfNotFound;
  #endif
}

/* The environment handling of the different libcs is a whole big mess:
   - The environment passed to main() may or may not survive putenv() calls
     (e.g., not under DJGPP). It does not even contain a meaningful value
     on some systems (e.g., not under mingw).
   - There may or may not be an environ or __environ variable containing
     the current environment.
   - If it exists, assignments to it can cause segfaults (e.g. glibc).
   - The putenv() function (POSIX) expects a static string, so the caller
     has to make a copy, but libc does not free it when the variable is
     unset or overwritten.
   - OTOH, there's no guarantee if and when the caller may free the string
     after it's not needed any more.
   - Functions like execl() or system() access the (internal) current
     environment. Furthermore, system() is quite system specific (esp.
     on Unix vs. Dos systems), so it can't be easily reprogrammed using
     another environment.
   This function tries to make the best of the situation, so don't
   complain that it's a whole big mess as well. ;*/
void _p_csetenv (char *, char *, char *, Boolean);
void _p_csetenv (char *VarName, char *Value, char *NewEnvCString, Boolean UnSet)
{
  (void) VarName;
  (void) Value;
  (void) NewEnvCString;
  (void) UnSet;
  #ifdef HAVE_UNSETENV
  if (UnSet)
    unsetenv (VarName);
  else
  #endif
  #ifdef HAVE_SETENV
    setenv (VarName, Value, 1);
  #elif defined (HAVE_PUTENV)
    {
      #ifdef HAVE_GETENV
      char *OldValue = getenv (VarName);
      if ((!OldValue && !UnSet) || _p_strcmp (Value, OldValue) != 0)
      #endif
        putenv (_p_strdup (NewEnvCString));
    }
  #endif
}

char *_p_c_strsignal (int);
char *_p_c_strsignal (int signal)
{
  #ifdef HAVE_STRSIGNAL
  return strsignal (signal);
  #else
  static char buf [64];
  sprintf (buf, "signal #%i", signal);
  return buf;
  #endif
}

int _p_pid (void);
int _p_pid (void)
{
  return getpid ();
}

Boolean _p_kill (int, int);
Boolean _p_kill (int PID, int Signal)
{
  #ifdef HAVE_KILL
  return (kill (PID, Signal) == 0) ? TRUE : FALSE;
  #else
  return FALSE;
  #endif
}

Boolean _p_sigaction (int, TSignalHandler, Boolean, Boolean, TSignalHandler *, Boolean *);
Boolean _p_sigaction (int Signal, TSignalHandler Handler, Boolean Restart, Boolean UnlessIgnored,
                      TSignalHandler *POldHandler, Boolean *POldRestart)
{
  TSignalHandler OldHandler = SIG_ERR;
  int OldRestart = FALSE, result = FALSE;
  if (Signal != 0 && Handler != SIG_ERR)
    {
      #ifdef HAVE_SIGACTION
      struct sigaction Action, OldAction;
      Action.sa_handler = Handler;
      sigemptyset (&Action.sa_mask);
      Action.sa_flags = Restart ? SA_RESTART : 0;
      if (!UnlessIgnored)
        result = sigaction (Signal, &Action, &OldAction) == 0;
      else if (sigaction (Signal, NULL, &OldAction) == 0)
        {
          if (OldAction.sa_handler == SIG_IGN)
            result = TRUE;
          else
            result = sigaction (Signal, &Action, &OldAction) == 0;
        }
      OldHandler = OldAction.sa_handler;
      OldRestart = !!(OldAction.sa_flags & SA_RESTART);
      #elif defined (HAVE_SIGVEC)
      struct sigvec Action, OldAction;
      Action.sv_handler = Handler;
      Action.sv_mask = 0;
      Action.sv_flags = Restart ? 0 : SV_INTERRUPT;
      if (!UnlessIgnored)
        result = sigvec (Signal, &Action, &OldAction) == 0;
      else if (sigvec (Signal, NULL, &OldAction) == 0)
        {
          if (OldAction.sv_handler == SIG_IGN)
            result = TRUE;
          else
            result = sigvec (Signal, &Action, &OldAction) == 0;
        }
      OldHandler = OldAction.sv_handler;
      OldRestart = !(OldAction.sv_flags & SV_INTERRUPT);
      #elif defined (HAVE_SIGNAL)
      OldHandler = signal (Signal, Handler);
      if (OldHandler != SIG_ERR)
        {
          if (UnlessIgnored && OldHandler == SIG_IGN)
            signal (Signal, SIG_IGN);
          #ifdef HAVE_SIGINTERRUPT
          else
            siginterrupt (Signal, !Restart);
          #endif
          (void) Restart;
          result = TRUE;
        }
      #else
      (void) Restart;
      (void) UnlessIgnored;
      #endif
    }
  if (POldHandler)
    *POldHandler = OldHandler;
  if (POldRestart)
    *POldRestart = OldRestart;
  return result;
}

int _p_waitpid (int, int *, Boolean);
int _p_waitpid (int PID, int *WStatus, Boolean Block)
{
  int wpid;
  #ifdef HAVE_WAITPID
  do
    wpid = waitpid (PID, WStatus, Block ? 0 : WNOHANG);
  while
    #ifdef EINTR
    (wpid < 0 && errno == EINTR);
    #else
    (0);
    #endif
  #else
  wpid = - 1;
  #endif
  if (wpid <= 0) *WStatus = 0;
  return wpid;
}

Boolean _p_setpgid (int, int);
Boolean _p_setpgid (int Process, int ProcessGroup)
{
  #ifdef HAVE_SETPGID
  return (setpgid (Process, ProcessGroup) == 0) ? TRUE : FALSE;
  #else
  return FALSE;
  #endif
}

Boolean _p_tcsetpgrp (int, int);
Boolean _p_tcsetpgrp (int File, int ProcessGroup)
{
  #ifdef HAVE_TCSETPGRP
  return (tcsetpgrp (File, ProcessGroup) == 0) ? TRUE : FALSE;
  #else
  (void) File;
  (void) ProcessGroup;
  return FALSE;
  #endif
}

int _p_tcgetpgrp (int);
int _p_tcgetpgrp (int File)
{
  #ifdef HAVE_TCGETPGRP
  return tcgetpgrp (File);
  #else
  (void) File;
  return -1;
  #endif
}

#if defined (HAVE_GETPWNAM) || defined (HAVE_GETPWUID) || defined (HAVE_GETPWENT)
static void
_p_pwl2c (const struct passwd *p, TCPasswordEntry *Entry)
{
  Entry -> UserName = _p_strdup (p -> pw_name);
  Entry -> RealName =
    #ifdef HAVE_PW_GECOS
    _p_strdup (p -> pw_gecos);
    #else
    NULL;
    #endif
  Entry -> Password =
    #ifdef HAVE_PW_PASSWD
    _p_strdup (p -> pw_passwd);
    #else
    NULL;
    #endif
  Entry -> HomeDirectory = _p_strdup (p -> pw_dir);
  Entry -> Shell = _p_strdup (p -> pw_shell);
  Entry -> UID = p -> pw_uid;
  Entry -> GID = p -> pw_gid;
}
#endif

Boolean _p_cgetpwnam (char *, TCPasswordEntry *);
Boolean _p_cgetpwnam (char *UserName, TCPasswordEntry *Entry)
{
  #ifdef HAVE_GETPWNAM
  struct passwd *p = getpwnam (UserName);
  if (!p) return FALSE;
  _p_pwl2c (p, Entry);
  return TRUE;
  #else
  return FALSE;
  #endif
}

Boolean _p_cgetpwuid (int, TCPasswordEntry *);
Boolean _p_cgetpwuid (int UID, TCPasswordEntry *Entry)
{
  #ifdef HAVE_GETPWUID
  struct passwd *p = getpwuid (UID);
  if (!p) return FALSE;
  _p_pwl2c (p, Entry);
  return TRUE;
  #else
  return FALSE;
  #endif
}

int _p_cgetpwent (TCPasswordEntry **);
int _p_cgetpwent (TCPasswordEntry **Entries)
{
  int Count = 0;
  #if defined (HAVE_GETPWENT) && defined (HAVE_SETPWENT) && defined (HAVE_ENDPWENT)
  struct passwd *p;
  setpwent ();
  *Entries = NULL;
  do
    {
      p = getpwent ();
      if (!p) break;
      _p_gpc_reallocmem ((void **) Entries, (++Count) * sizeof (TCPasswordEntry));
      _p_pwl2c (p, &((*Entries) [Count - 1]));
    }
  while (1);
  endpwent ();
  #endif
  return Count;
}

void _p_init_malloc (void (*) (char *));
void _p_init_malloc (void (*warn_proc) (char *))
{
  (void) warn_proc;
  #if 0
  malloc_init (0, warn_proc);
  #endif
}

void _p_init_misc (void);
void _p_init_misc ()
{
  #ifdef HAVE_SETLOCALE
  setlocale (LC_ALL, "");
  #endif
}

void _p_exit (int, Boolean) NORETURN;
void _p_exit (int n, Boolean AbortFlag)
{
  (void) AbortFlag;
  if (_p_error_fd >= 0)
    close (_p_error_fd); /* just to be sure */
  #ifdef HAVE_ABORT
  if (AbortFlag)
    abort ();
  #endif
  /* if the exit status would be 0 due to truncation, make it 1 in order
     to not indicate success erroneously */
  exit (n + (n != 0 && (n & 0xff) == 0));
}
