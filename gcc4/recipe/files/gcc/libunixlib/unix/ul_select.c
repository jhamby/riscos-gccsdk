/* Unixlib select() implementation.
 * Copyright 1997-2010 UnixLib Developers.
 */

#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <swis.h>
#include <sys/time.h>

#include <internal/unix.h>
#include <internal/dev.h>
#include <internal/os.h>
#include <unixlib/local.h>
#include <unixlib/types.h>
#include <pthread.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

fd_set __socket_fd_set;
static int __socket_fd_set_initialised = 0;

#ifdef DEBUG
#include <stdio.h>
static void
dump_fd_set (int nfds, const fd_set *iset, FILE *file)
{
  int fd = 0;

  if (!iset)
    return;

  fprintf (file, "%X = ", *(const int *) iset);
  for (; fd < nfds; fd++)
    fputc (FD_ISSET (fd, iset) ? '!' : '.', file);
  fputc ('\n', file);
}
#endif

/* Convert a bit pattern of UnixLib FDs in ISET to a bit pattern of socket
   FDs in OSET and set MAX_FD to the maximum socket FD+1 if it is greater
   than the incoming value of MAX_FD. Only scan UnixLib FDs up to NFDS-1. */
static void
__convert_fd_set (int nfds, const fd_set *iset, fd_set *oset, int *max_fd)
{
#define WORD_BITS 32
  /* Round up to whole words */
  int words = (nfds + WORD_BITS - 1) / WORD_BITS;

  FD_ZERO (oset);

  while (words-- > 0)
    {
      /* Check for bits set in both passed in set to select() and socket
         fds.  */
      int bothset = ((const int *)iset)[words] & ((int *)&__socket_fd_set)[words];
      if (bothset)
        {
          /* Cast to 'unsigned int' in order to have more efficient code.  */
          int bits;
          if ((bits = ((unsigned int)nfds % WORD_BITS)) == 0)
            bits = WORD_BITS;

          while (bits-- > 0)
            {
              if ((bothset >> bits) & 1)
                {
                  int fd = words * WORD_BITS + bits;

                  if (getfd (fd)->devicehandle)
                    {
                      int sock_fd = (int)(getfd (fd)->devicehandle->handle);

                      FD_SET (sock_fd, oset);

                      if (sock_fd + 1 > *max_fd)
                        *max_fd = sock_fd + 1;
                   }
                }
            }
        }
    }
#ifdef DEBUG
  fprintf (stderr, "Max sock fd is number %d\n", *max_fd);
#endif
}

static void
__return_fd_set (int nfds, fd_set *iset, const fd_set *oset)
{
  /* Round up to whole words */
  int words = (nfds + WORD_BITS - 1) / WORD_BITS;

  while (words-- > 0)
    {
      int bothset;

      /* Only check sockets in the original input set.  */
      bothset = ((const int *)iset)[words] & ((int *)&__socket_fd_set)[words];

      if (bothset)
        {
          /* Cast to 'unsigned int' in order to have more efficient code.  */
          int bits;
          if ((bits = ((unsigned int)nfds % WORD_BITS)) == 0)
            bits = WORD_BITS;

          while (bits-- > 0)
            {
              int fd = words * WORD_BITS + bits;

              if (getfd (fd)->devicehandle)
                {
                  int sock_fd = (int)(getfd (fd)->devicehandle->handle);

                  if (getfd (fd)->devicehandle->type == DEV_SOCKET && !FD_ISSET (sock_fd, oset))
                    FD_CLR (fd, iset);
                }
            }
        }
    }
}

/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
   after waiting the interval specified therein.  Returns the number of ready
   descriptors, or -1 for errors.  */

int
select (int nfds, fd_set *readfds, fd_set *writefds,
	fd_set *exceptfds, struct timeval *timeout)
{
  const struct ul_global *gbl = &__ul_global;

#ifdef DEBUG
  fprintf (stderr, "Entry:\t%d\t%p\t%p\t%p\t%p\n"
	   "sizeof(fd_set) = %d\t__FD_SETSIZE = %d\n", nfds, readfds, writefds,
	   exceptfds, timeout, sizeof(fd_set), __FD_SETSIZE);
#endif

  if (!__socket_fd_set_initialised)
    {
      /* We need to scan all file descriptors to initialise as we could be
         a child program with sockets already open */
      FD_ZERO (&__socket_fd_set);

      const struct __sul_process *sulproc = gbl->sulproc;
      for (unsigned int i = 0; i < sulproc->maxfd; i++)
        if (getfd (i)->devicehandle && getfd (i)->devicehandle->type == DEV_SOCKET)
          FD_SET (i, &__socket_fd_set);

      __socket_fd_set_initialised = 1;
    }

  int zerotime = 0;
  unsigned int end = 0, now = 0;
  if (timeout)
    {
      if (!timeout->tv_usec && !timeout->tv_sec)
        zerotime = 1;
      else
        {
          /* 21474836.48 seconds will fit in 31 bits.  */
          if (timeout->tv_usec > 1000000 || timeout->tv_sec > 21474835)
            return __set_errno (EINVAL);

          /* OK, so we can't cope with anything more than roughly 248.55 days!  */
          now = clock ();
          end = now + timeout->tv_sec * 100
		    + (50000 + timeout->tv_usec) / 1000000;
        }
    }

  pthread_testcancel();
  __pthread_disable_ints();

  /* Limit number of bits to check rather than returning an error.  */
  if (nfds > FD_SETSIZE)
    nfds = FD_SETSIZE;

#ifdef DEBUG
  dump_fd_set (nfds, readfds, stderr);
  dump_fd_set (nfds, writefds, stderr);
#endif

  /* Convert the individual fdsets and set highest.  */
  int sock_nfds = 0;
  fd_set sock_readfds, sock_writefds, sock_exceptfds;
  fd_set new_readfds, new_writefds, new_exceptfds;
  if (readfds)
    {
      __convert_fd_set (nfds, readfds, &sock_readfds, &sock_nfds);
      FD_ZERO (&new_readfds);
    }
  if (writefds)
    {
      __convert_fd_set (nfds, writefds, &sock_writefds, &sock_nfds);
      FD_ZERO (&new_writefds);
    }
  if (exceptfds)
    {
      __convert_fd_set (nfds, exceptfds, &sock_exceptfds, &sock_nfds);
      FD_ZERO (&new_exceptfds);
    }

  /* Loop until one or more fds goes live.
     Breakout from middle if live fd(s) detected.
     Breakout from middle of loop if timeout.

     The plan is:
     readfds contains the supplied fds to check for read. This is kept intact
     until just before we return.
     new_readfds is used to build up the return value.
     (this will usually end up being zero every time that we loop)
     sock_readfds contains the converted socket handles
     socknew_readfds contains the output from socket_select.

     likewise for write and except.

     There is a lot of collusion here with DEV_SOCKET select, which returns
     1,1,1 for read,write,except but 0 live, so that __return_fd_set will
     merge socknew_readfds properly.  */

  fd_set newsock_readfds, newsock_writefds, newsock_exceptfds;
  int live_fds = 0;
  for (;;)
    {
      int fd = nfds;

      /* At this point live_fds is 0, as are all new fd sets.  */

      while (fd-- > 0)
	{
	  struct __unixlib_fd *file_desc = getfd (fd);

          if (file_desc->devicehandle)
            {
              int (*select_func) (struct __unixlib_fd *, int, fd_set *,
				  fd_set *, fd_set *) =
                  __dev[file_desc->devicehandle->type < NDEV ? file_desc->devicehandle->type : DEV_NULL].select;

              fd_set *read_p = (readfds && FD_ISSET(fd, readfds)) ? &new_readfds : NULL;
              fd_set *write_p = (writefds && FD_ISSET(fd, writefds)) ? &new_writefds : NULL;

              if (select_func == __nullselect)
                {
                  if (read_p)
                    {
                      live_fds++;
                      FD_SET(fd, read_p);
                    }
                  if (write_p)
                    {
                      live_fds++;
                      FD_SET(fd, write_p);
                    }
                }
              else
                {
                  fd_set *except_p = (exceptfds && FD_ISSET(fd, exceptfds)) ? &new_exceptfds : NULL;

                  if (select_func == __sockselect)
                    {
                      if (read_p)
			FD_SET (fd, read_p);
                      if (write_p)
			FD_SET (fd, write_p);
                      if (except_p)
			FD_SET (fd, except_p);
                      continue;
                    }

                  /* Don't bother calling if not interested in this fd.  */
                  if (read_p || write_p || except_p)
                    {
#ifdef DEBUG
	              __os_print ("/");
#endif
	              int result = __funcall (*select_func, (file_desc, fd, read_p, write_p, except_p));
#ifdef DEBUG
	              __os_print ("\\");
#endif
	              if (result < 0)
	                {
	                  __pthread_enable_ints();
		          return -1;
		        }

	              live_fds += result;
	           }
	        }
	    }
	  else
	    {
	      __pthread_enable_ints();
	      return __set_errno (EBADF);
	    }
	}

#ifdef DEBUG
      __os_nl();
#endif

      /* Copy these so that we don't corrupt the orginals.  */
      newsock_readfds = sock_readfds;
      newsock_writefds = sock_writefds;
      newsock_exceptfds = sock_exceptfds;

#ifdef DEBUG
      fprintf (stderr, "%d %p %p %p\n", sock_nfds,
	       readfds ? &newsock_readfds : readfds,
	       writefds ? &newsock_writefds : writefds,
	       exceptfds ? &newsock_exceptfds : exceptfds);
#endif

      /* Internet 5.02 gets very very upset if you try to wait with
	 R0 = 0. It occasionally sets R13_user -= 4, and appears to mess up
	 the SVC stack too.  */
      struct timeval poll = {0, 0};
      int result = sock_nfds ? _sselect (sock_nfds,
					 readfds ? &newsock_readfds : readfds,
					 writefds ? &newsock_writefds : writefds,
					 exceptfds ? &newsock_exceptfds : exceptfds,
					 &poll) : sock_nfds;
#ifdef DEBUG
      fprintf (stderr, "%d\n", result);
#endif

      if (result < 0)
        {
	  __pthread_enable_ints();
	  return -1;
	}

      live_fds += result;

      if (live_fds || zerotime)
	{
#ifdef DEBUG
	  __os_print ("Select is live/immediate return\n\r");
#endif
	  /* Something is live. Break and return.  */
	  break;
	}

      if (timeout)
	{
	  /* Difference will always be < 2^31  */
	  if (end < clock ())
	    {
#ifdef DEBUG
	      __os_print ("Select timeout\n\r");
#endif
	      /* Timeout.  */
	      timeout->tv_sec = timeout->tv_usec = 0;
	      /* Skip subtraction test below.  */
	      timeout = NULL;
	      break;
	    }
	}
#ifdef DEBUG
      else
	__os_print ("No timeout\n\r");
      __os_print ("Loop\n\r");
#endif

      if (gbl->taskwindow)
	{
	  int regs[10];

	  regs[0] = 6; /* Taskwindow sleep.  */
	  /* Yield.  This is the recommended value for taskwindow
	     sleeping with no poll word.  */
	  regs[1] = 0;
	  __os_swi (OS_UpCall, regs);
	}

      __pthread_enable_ints();
      pthread_yield();
      __pthread_disable_ints();
    }

  /* Copy the results back to the input fd sets.
     Merge in the socket results.  */
  if (readfds)
    {
      /* *readfds = new_readfds; was a 4 hour-to-find bug.
         Annoying user programs don't pass in a full size FD set.
         Copy back the minimum number of bytes needed.  */
      memcpy (readfds, &new_readfds, (nfds + 7) >> 3);
      if (sock_nfds)
	__return_fd_set (nfds, readfds, &newsock_readfds);
    }
  if (writefds)
    {
      memcpy (writefds, &new_writefds, (nfds + 7) >> 3);
      if (sock_nfds)
	__return_fd_set (nfds, writefds, &newsock_writefds);
    }
  if (exceptfds)
    {
      memcpy (exceptfds, &new_exceptfds, (nfds + 7) >> 3);
      if (sock_nfds)
	__return_fd_set (nfds, exceptfds, &newsock_exceptfds);
    }

#ifdef DEBUG
  dump_fd_set (nfds, readfds, stderr);
#endif

  if (timeout && !zerotime)
    {
      now = clock () - now;

      struct timeval poll;
      poll.tv_sec = now / 100;
      poll.tv_usec = (now % 100) * 10000;

      timeout->tv_usec -= poll.tv_usec;

      if (timeout->tv_usec < 0)
	{
	  timeout->tv_usec += 1000000;
	  timeout->tv_sec -= 1;
	}

      timeout->tv_sec -= poll.tv_sec;

      if ((int)timeout->tv_sec < 0)
	timeout->tv_usec = timeout->tv_sec = 0;
    }

  __pthread_enable_ints();
  return live_fds;
}
