/* UnixLib popen() and pclose() implementation.
   Copyright 2001-2010 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/wait.h>

#include <internal/unix.h>
#include <pthread.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

/* FIXME, hang this off the FILE structure.  */
struct pwr
  {
    FILE *f;
    int p0;
    char *command;
    struct pwr *next;
  };

static struct pwr *__pwr = NULL;

/* FIXME. These calls to execl will me the child will not identify itself
   as a child with the current UnixLib and thus popen does not work.
   This is going to be fixed soon. PJB.  */
static void
__pexec (const char *command)
{
  int status;
  const char *path;
  if ((path = getenv ("SHELL")) == NULL)
    {
      if (*command == '*')
	status = execl (command, 0);
      else
	status = execl ("*", "", command, 0);
    }
  else
    {
      const char *shell = strrchr (path, '/');
      if (shell)
	shell++;
      else
	shell = path;
      status = execl (path, shell, "-c", command, 0);
    }
  if (status)
    _exit (EXIT_FAILURE);
}

FILE *
popen (const char *command, const char *mode)
{
  FILE *stream;
  int pipedes[2];

  PTHREAD_UNSAFE

  if (command == NULL || mode == NULL || (*mode != 'r' && *mode != 'w'))
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  if (pipe (pipedes) < 0)
    return NULL;

#ifdef DEBUG
  debug_printf ("-- popen(cmd='%s', mode='%s'\n", command, mode);
#endif

  /* mode == 'r' means current process will read from the file created by the
     command. So fork child to generate file.  */
  if (*mode == 'r')
    switch (vfork ())
      {
	case -1:
	  /* The fork failed.  */
	  close (pipedes[0]);
	  close (pipedes[1]);
	  return NULL;

	case 0:
	  /* We are the child. Set stdout to the pipe with dup2 and exec.  */
	  close (pipedes[0]);
	  dup2 (pipedes[1], 1);
	  close (pipedes[1]);
	  __pexec (command);
	  break;

	default:
	  /* We are the parent.  Close the unused side of the pipe and open
	     a stream on the used side to return.  */
	  close (pipedes[1]);
	  if (!(stream = fdopen (pipedes[0], mode)))
	    return NULL;
	  stream->__ispipe = 1;
	  return stream;
      }
  else
    {
      /* The current process will write to the pipe. Under RISC OS we cannot
	 fork the read command until the write command has finished
	 (via pclose), so we record the command for use in pclose.  */
      struct pwr *pwr;

      if (!(pwr = malloc (sizeof (struct pwr))))
	return NULL;
      if (!(pwr->command = strdup (command)))
	{
	  free (pwr);
	  return NULL;
	}
      if (!(stream = fdopen (pipedes[1], mode)))
	{
	  free (pwr);
	  free (pwr->command);
	  return NULL;
	}
      stream->__ispipe = 1;
      pwr->f = stream;
      pwr->p0 = pipedes[0];
      pwr->next = __pwr;
      __pwr = pwr;
      return stream;
    }

  /* Not reached.  */
  return NULL;
}

int
pclose (FILE * stream)
{
  int status;

  PTHREAD_UNSAFE

  if (!stream->__ispipe)
    return __set_errno (EINVAL);

  if (stream->__mode.__bits.__write)
    {
      /* The current process has written to the pipe, so now fork the child
	 to read from the pipe.  */
      static struct pwr *pwr;
      struct pwr *prev_pwr;

      /* Locate saved pipe command and remove it from the list.  */
      for (pwr = __pwr, prev_pwr = NULL;
	   pwr != NULL && pwr->f != stream;
	   pwr = pwr->next, prev_pwr = pwr)
	/* */;
      if (!pwr)
	return -1;
      if (prev_pwr)
	prev_pwr->next = pwr->next;
      else
	__pwr = pwr->next;

      fflush (stream);
      switch (vfork ())
	{
	  case -1:
	    /* The fork failed.  */
	    close (pwr->p0);
	    fclose (stream);
	    free (pwr->command);
	    free (pwr);
	    return -1;

	  case 0:
	    /* We are the child. Set stdin to the pipe with dup2.  */
	    /* The old code closed stream-fd here. I don't think we should
	       close it here, since we call fclose below.  -- PJB.  */
	    /* close (stream->fd);  */
	    dup2 (pwr->p0, 0);
	    close (pwr->p0);
	    __pexec (pwr->command);
	    break;

	  default:
	    /* We are the parent.  Close the pipe.  */
	    close (pwr->p0);
	    break;
	}
      free (pwr->command);
      free (pwr);
    }

  if (wait (&status) == -1)
    status = -1;

  if (fclose (stream))
    return -1;

  return status;
}
