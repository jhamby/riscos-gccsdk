/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/getenv.c,v $
 * $Date: 2004/01/06 00:17:37 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getenv.c,v 1.5 2004/01/06 00:17:37 joty Exp $";
#endif

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <swis.h>
#include <pthread.h>

/* #define DEBUG 1 */

/* This global variable is also initialised in unix.c when we are a child.  */
char **environ = NULL;

/* If this variable is not a null pointer we allocated the current
   environment.  */
char **__last_environ = NULL;

/* Unused function to generate a __UNIX_ENV_PREFIX$PID$ prefix.  */
#if 0
/* Return prefix for environment variables used in the OS environment.
   This is done, because RISC OS does not have a private per task
   environment, but we need this to support some Unix programs.  */
static const char *
__env_os_prefix (void)
{
  static char __env_os_prefix[] = __UNIX_ENV_PREFIX "\0000000000$";

  /* If this is the first call, then establish the standard prefix.  The pid
     is not set in the string and the first character of the pid is '\0'.  */
  if (__env_os_prefix[sizeof (__UNIX_ENV_PREFIX) - 1] == '\0')
    {
      /* Perform an itoa inline, to avoid unneccessarily pulling in stdio.  */
      int pid;
      char *s2 = &__env_os_prefix[sizeof (__env_os_prefix) - 2];
      char *s1 = &__env_os_prefix[sizeof (__UNIX_ENV_PREFIX) - 1];

      if (__u->status.has_parent)
	pid = __u->ppid;
      else
	pid = __u->pid;
      do
	{
	  *--s2 = (pid % 10) + '0';
	  pid  = pid / 10;
	}
      while (pid > 0);
      /* Shift buffer down to end of Unix prefix.  */
      while (*s1++ = *s2++)
	;
  }

  return __env_os_prefix;
}
#endif

/* Put string, which is of the form "NAME=VALUE", into the OS environment.
   Use name of "__UNIX_ENV_PREFIX$NAME" as the name of the OS environment
   variable. If NAME contains a $, then we use the name 'as is'.  This is so
   that those names are global in the OS environment.  */
static int
__addenv_unix_to_os (const char *string)
{
  const char * const equal = strchr (string, '=');
  const char *os_env_var = strchr (string, '$');
  char *buf;
  const char *prefix;
  _kernel_oserror *err;
  int regs[10];

  if (os_env_var >= string && os_env_var < equal)
    prefix = "";
  else
    prefix = __UNIX_ENV_PREFIX;

  buf = malloc (strlen (prefix) + (equal - string) + 1);
  if (!buf)
    return -1;
  strcpy (buf, prefix);
  strncat (buf, string, (equal - string));

  regs[0] = (int) buf;
  regs[1] = (int) equal + 1;
  regs[2] = strlen (equal + 1);
  regs[3] = 0;
  regs[4] = 0;
  err = __os_swi (OS_SetVarVal, regs);
  free (buf);
  if (err)
    {
      __seterr (err);
      return -1;
    }
  return 0;
}

/* Remove NAME from the OS environment. Returns -1 on failure or not found.  */
int
__remenv_from_os (const char *name)
{
  _kernel_oserror *err;
  int regs[10];

  regs[0] = (int) name;
  regs[1] = 0;
  regs[2] = -1;
  regs[3] = 0;
  regs[4] = 0;
  /* Do not set the errno if the variable was not found. */
  err = __os_swi (OS_SetVarVal, regs);
  return (err ? -1 : 0);
}

/* Get environment value from OS and copy into BUF and return a pointer to BUF.
   If no match found then return NULL.  If BUF is NULL, then allocate space
   as necessary, otherwise BUFLEN is length of supplied buffer.  */
char *
__getenv_from_os (const char *name, char *buf, size_t buflen)
{
  char *new_buf;
  _kernel_oserror *err;
  int regs[10];

  /* Check illegal argument combination.  */
  if (buflen == 0 && buf != NULL)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  if (buf == NULL)
    {
      /* Find size of buffer required.  */
      regs[0] = (int) name;
      regs[1] = 0;
      regs[2] = -1;
      regs[3] = 0;
      regs[4] = 0;
      __os_swi (OS_ReadVarVal, regs);
      buflen = -regs[2];	/* equivalent to ~regs[2] + 1;  */
      /* We cannot find the length of environment variables which are macro
	 expanded, so we need to loop until buffer is large enough to hold
	 the expanded value. The buffer is doubled in size each iteration.  */
      do
	{
	  if ((buf = malloc (buflen)) == NULL)
	    return NULL;
	  regs[0] = (int) name;
	  regs[1] = (int) buf;
	  /* One less than buf size so can zero terminate below.  */
	  regs[2] = buflen - 1;
	  regs[3] = 0;
	  regs[4] = 3;
	  err = __os_swi (OS_ReadVarVal, regs);
	  if (!err)
	    {
	      /* Zero terminate and return trimmed buffer.  */
	      buf[regs[2]] = '\0';
	      new_buf = realloc (buf, regs[2] + 1);
	      return new_buf ? new_buf : buf;
	    }
	  /* Contains nothing of use, so bin it and start afresh next loop.  */
	  free (buf);
	  buflen = buflen << 1;
	}
      while (* (int *) regs[0] == 0x1E4);
      /* Buffer overflow error is (as per usual for Acorn) ill defined.
         It apeears that on buffer overflow R[2] == R[2] entry (ie
         *X*OS_ReadVarVal has just written the last possible character that
         will fit into the buffer, and upon checking to see that it can fit the
         next in it returns the error). Interestingly, for a macro that expands
         to 55 characters, a buffer of 55 gives an overflow error. A buffer of
         56 is needed, but only 55 bytes are written. D'oh.

         Seems safer to test for the error number 0x1E4 which as per usual is
         not documented as the code for buffer overflow, in case we get back
         R2 unaltered due to some other wierd error.  */

      /* The variable isn't a macro and OS_ReadVarVal returned an error.
	 Do not set the errno if the variable was not found. */
      if (regs[2] != 0)
	__seterr (err);
      return NULL;
    }
  else
    {
      regs[0] = (int) name;
      regs[1] = (int) buf;
      /* One less than buf size so can zero terminate below.  */
      regs[2] = buflen - 1;
      regs[3] = 0;
      regs[4] = 3;
      err = __os_swi (OS_ReadVarVal, regs);
      if (err)
	{
	  /* Do not set the errno if the variable was not found. */
	  if (regs[2] != 0)
	    __seterr (err);
	  return NULL;
	}
    }

  buf[regs[2]] = '\0';
  return buf;
}

/* Lookup NAME in the OS environment. First, if NAME does not contain a '$',
   look it up as __UNIX_ENV_PREFIX$NAME and if that fails or NAME does contain
   a '$', then lookup as NAME. This is not symmetric with __remenv_unix_from_os
   or __addenv_unix_to_os and so maybe lookup of NAME not containing a '$'
   should be tried only with the prefix.  */
static char *
__getenv_unix_from_os (const char *name, char *buf, size_t buflen)
{
  if (strchr (name, '$') == NULL)
    {
      const size_t len = sizeof (__UNIX_ENV_PREFIX) + strlen (name);
      char *rval;
      char *ro_name = malloc (len);

      if (ro_name == NULL)
	return NULL;
      strcpy (ro_name, __UNIX_ENV_PREFIX);
      strcat (ro_name, name);
      rval = __getenv_from_os (ro_name, buf, buflen);
      free (ro_name);
      if (rval != NULL)
	return rval;
    }
  return __getenv_from_os (name, buf, buflen);
}

/* Lookup NAME in the environment and return value if found.  */
char *
__chkenv (const char *name)
{
  const size_t len = strlen (name);
  char **ep;

  PTHREAD_UNSAFE

#ifdef DEBUG
  __os_print ("-- chkenv: name='"); __os_print (name);
  __os_print ("', environ="); __os_prhex ((int) environ);
  __os_nl ();
#endif

  if (environ != NULL)
    for (ep = environ; *ep; ep++)
      if (!strncmp (*ep, name, len) && (*ep)[len] == '=')
	return &(*ep)[len + 1];

  return NULL;
}

/* Add NAME=VALUE to the environment. If NAME is already in the environment,
   only add when replace is non-zero. If NAME is added to the environment
   and add_to_os is non-zero, then also add it to the RISC OS global
   environment.  FIXME, should add_to_os be determined from '$' in NAME ?  */
char *
__addenv (const char *name, const char *value, int replace, int add_to_os)
{
  char **ep = NULL;
  size_t envcnt = 0;
  const size_t namelen = strlen (name);
  const size_t valuelen = strlen (value) + 1;

  PTHREAD_UNSAFE

#ifdef DEBUG
  __os_print ("-- addenv: name='"); __os_print (name);
  __os_print ("', value='"); __os_print (value);
  __os_print ("', environ="); __os_prhex ((int) environ);
  __os_print ("', __last_environ="); __os_prhex ((int) __last_environ);
  __os_nl ();
#endif

  /* Search environment for old value.  */
  if (environ != NULL)
    {
      for (ep = environ; *ep; ++ep)
	if (!strncmp (*ep, name, namelen) && (*ep)[namelen] == '=')
	  break;
	else
	  ++envcnt;
    }

  if (environ == NULL || *ep == NULL)	/* Did not find old value.  */
    {
      char **new_environ;
      /* If we allocated the environ, we can extend it, else allocate it.
	 A memory leak is likely when a new environ is malloc'ed, but we
	 cannot free the old environ, since it may not have been malloc'ed.  */
      if (environ == __last_environ)
	new_environ = realloc (__last_environ, (envcnt + 2) * sizeof (char*));
      else
	if ((new_environ = malloc ((envcnt + 2) * sizeof (char*))) != NULL)
	  memcpy (new_environ, environ, envcnt * sizeof (char*));

      /* Failed to allocate the environment, so return NULL.  */
      if (new_environ == NULL)
	return NULL;

      if ((new_environ[envcnt] = malloc (namelen + 1 + valuelen)) == NULL)
	{
	  if (environ != __last_environ)
	    free (new_environ);
	  return NULL;
	}
      new_environ[envcnt + 1] = NULL;
      __last_environ = environ = new_environ;
      ep = new_environ + envcnt;
    }
  else if (!replace)
    return NULL;
  else
    {
      /* If we cannot use the old space, then allocate.  */
      if (strlen (*ep) + 1 < namelen + 1 + valuelen)
	{
	  char *str = malloc (namelen + 1 + valuelen);
	  /* Allocate buffer for this environment value, leaving old value
	     on failure.  Likely memory leak here, but we must not try
	     to free the old string, since it may not have been malloc'ed.  */
	  if (str == NULL)
	    return NULL;
	  *ep = str;
	}
      }

  /* Now store new value.  */
  memcpy (*ep, name, namelen);
  (*ep)[namelen] = '=';
  memcpy (&(*ep)[namelen + 1], value, valuelen);

  if (add_to_os)
    __addenv_unix_to_os (*ep);

  return (&(*ep)[namelen + 1]);
}

/* Return the integer value of NAME, from the RISC OS global environment.  */
int
__intenv (const char *name)
{
  char intbuf[16];

  if (__getenv_unix_from_os (name, intbuf, sizeof (intbuf)) != NULL)
    return (int) __decstrtoui (intbuf, 0);
  else
    return 0;
}

/* 1. Lookup NAME in the program's environment first and return its result
      if found.
   2. If NAME was not found in the program's environment, then look it up
      in the RISC OS global environment.  If found there, then add it to
      the program's environment.
   The reason for adding the value to the environment is because the
   RISC OS environment is global and we want an unchanging value.
   Return found value or NULL.  */
char *
getenv (const char *name)
{
  char *buf, *rval;

  buf = __chkenv (name);
  if (buf != NULL)
    return buf;

  buf = __getenv_unix_from_os (name, NULL, 0);
  if (buf == NULL)
    return NULL;

  rval = __addenv (name, buf, 1, 0);
  free (buf);

  return rval;
}
