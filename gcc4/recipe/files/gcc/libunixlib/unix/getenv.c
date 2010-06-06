/* Manipulate Unix-style environment variables.
   Copyright (c) 2004-2010 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <swis.h>

#include <internal/os.h>
#include <internal/unix.h>
#include <pthread.h>

/* #define DEBUG 1 */
#ifdef DEBUG
#  include <sys/debug.h>
#endif
 
/* This global variable is our current environment.  This can be inherited
   from parent (then __ul_global.last_environ is NULL), or allocated ourselves
   (then __ul_global.last_environ is non-NULL).  */
char **environ = NULL;


/* Get environment value from OS and copy into BUF and return a pointer to BUF.
   If no match found then return NULL.  If BUF is NULL, then allocate space
   as necessary, otherwise BUFLEN is length of supplied buffer.
   If buf is NULL and call is not NULL then an enumeration is taking place,
   so only return variables that are prefixed by UnixEnv$ or have no $ in
   their name.  You should continue enumerating until *call is NULL on exit
   and ignore NULL results during this enumeration loop.  */
char *
__getenv_from_os (const char *name, char *buf, size_t buflen, char **call)
{
  char *new_buf;
  _kernel_oserror *err;
  int regs[10];
  int namelen = 0;
  int origcall = call ? (int)*call : 0;
  char *found = NULL;

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
      regs[3] = origcall;
      regs[4] = 0;
      __os_swi (OS_ReadVarVal, regs);
      buflen = -regs[2];	/* equivalent to ~regs[2] + 1;  */

      if (call)
        {
          /* We are trying to enumerate everything that needs adding
             to the environment, so filter out variables that we are
             not interested in. */
          char *dollar;

          *call = found = (char *)regs[3];
          if (!found) return NULL;

          namelen = strlen (found) + 1;
          dollar = strchr (found, '$');

          /* Anything with two $ in must be a RISC OS variable. */
          if (dollar && strchr (dollar + 1, '$'))
            return NULL;

          if (dollar)
            {
              if (strncmp (found, __UNIX_ENV_PREFIX,
			   sizeof (__UNIX_ENV_PREFIX) - 1) == 0)
                {
                  /* We need to strip the prefix off before adding it to the
                     environment. */
                  found += sizeof (__UNIX_ENV_PREFIX) - 1;
                  namelen -= sizeof (__UNIX_ENV_PREFIX) - 1;
                }
              else
                {
                  /* Anything with a $ that is not prefixed with UnixEnv$ is
                     a RISC OS variable. */
                  return NULL;
                }
            }

          buflen += namelen;
        }

      /* We cannot find the length of environment variables which are macro
	 expanded, so we need to loop until buffer is large enough to hold
	 the expanded value. The buffer is doubled in size each iteration.  */
      do
	{
	  if ((buf = malloc (buflen)) == NULL)
	    return NULL;
	  regs[0] = (int) name;
	  regs[1] = (int) buf + namelen;
	  /* One less than buf size so can zero terminate below.  */
	  regs[2] = buflen - 1 - namelen;
	  regs[3] = origcall;
	  regs[4] = 3;
	  err = __os_swi (OS_ReadVarVal, regs);
	  if (!err)
	    {
	      if (call)
	        {
	          memcpy(buf, found, namelen - 1);
	          buf[namelen - 1] = '=';
	        }

	      /* Zero terminate and return trimmed buffer.  */
	      buf[regs[2] + namelen] = '\0';
	      new_buf = realloc (buf, namelen + regs[2] + 1);
	      return new_buf ? new_buf : buf;
	    }
	  /* Contains nothing of use, so bin it and start afresh next loop.  */
	  free (buf);
	  buflen = buflen << 1;
	}
      while (* (int *) regs[0] == 0x1E4);
      /* Buffer overflow error is (as per usual for Acorn) ill defined.
         It appears that on buffer overflow R[2] == R[2] entry (ie
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
	__ul_seterr (err, EOPSYS);
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
	    __ul_seterr (err, EOPSYS);
	  return NULL;
	}
    }

  buf[regs[2]] = '\0';
  return buf;
}

/* Lookup NAME in the environment and return value if found.  */
static char *
__getenv_from_env (const char *name)
{
  const size_t len = strlen (name);
  char **ep;

  PTHREAD_UNSAFE

#ifdef DEBUG
  debug_printf ("-- __getenv_from_env: name='%s', environ=%p\n", name, environ);
#endif

  if (environ != NULL)
    for (ep = environ; *ep; ep++)
      if (!strncmp (*ep, name, len) && (*ep)[len] == '=')
	return &(*ep)[len + 1];

  return NULL;
}


/* Return the integer value of NAME, from the RISC OS global environment.  */
int
__intenv (const char *name)
{
  char intbuf[16];

  if (__getenv_from_os (name, intbuf, sizeof (intbuf), NULL) != NULL)
    return (int) __decstrtoui (intbuf, NULL);
  else
    return 0;
}

/* Lookup an environment variable. If the name contains a $ then we assume
   it is a RISC OS system variable and get it's value from the OS. If the
   name doesn't contain a $ then we look the value up in the program's
   private environment.
   Return found value or NULL.  */
char *
getenv (const char *name)
{
  if (strchr (name, '$'))
    {
      static char *buf = NULL;

      free (buf);
      buf = __getenv_from_os (name, NULL, 0, NULL);

      return buf;
    }
  else
    return __getenv_from_env (name);
}
