/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/select.c,v $
 * $Date: 2002/12/13 15:01:59 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: select.c,v 1.4 2002/12/13 15:01:59 admin Exp $";
#endif

/* netlib/socket.c: Written by Peter Burwood, July 1997  */

#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>
#include <unixlib/os.h>
#include <swis.h>
#include <sys/time.h>
#include <unixlib/local.h>
#include <unixlib/types.h>
#include <pthread.h>


#ifdef DEBUG
#include <stdio.h>
static void
dump_fd_set (int nfds, const fd_set *iset, FILE *file)
{
  int fd = 0;

  if (!iset)
    return;

  fprintf (file, "%X = ", *(long *) iset);
  for (; fd < nfds; fd++)
    fputc (FD_ISSET (fd, iset) ? '!' : '.', file);
  fputc ('\n', file);
}
#endif

/* Convert a bit pattern of UnixLib FD's in ISET to a bit pattern of socket
   FD's in OSET and set MAX_FD to the maximum socket FD+1 if it is greater
   than the incoming value of MAX_FD. Only scan UnixLib FD's up to NFDS-1.
   Return 0 if conversion okay, -1 on failure.  */
static void
__convert_fd_set (int nfds, const fd_set *iset, fd_set *oset, int *max_fd)
{
  int fd = nfds, sock_fd;

  FD_ZERO (oset);
  while (fd--)
    {
      if ((FD_ISSET (fd, iset)) && (__socket_valid(fd) != -1))
	{
	  sock_fd = (int)__u->fd[fd].handle;
	  /* FD_SET evaluates first argument twice.  */
	  FD_SET (sock_fd, oset);
	  if (sock_fd + 1 > *max_fd)
	    *max_fd = sock_fd + 1;
	}
    }
#ifdef DEBUG
  fprintf (stderr, "Max sock fd is number %d\n", *max_fd);
#endif
}

static void
__return_fd_set (int nfds, fd_set *iset, const fd_set *oset)
{
  int fd = nfds, sock_fd;

#ifdef DEBUG
  fprintf (stderr, "Returning fds from %d downwards\n", nfds);
#endif
  while (fd--)
    {
      if ((FD_ISSET (fd, iset)) && (__socket_valid(fd) != -1))
	{
	  /* Only check sockets in the original input set.  */
	  sock_fd = (int)__u->fd[fd].handle;
	  /* Clear orginal set if internal set is clear for this FD.  */
	  if (!FD_ISSET (sock_fd, oset))
	    {
#ifdef DEBUG
	      fprintf (stderr, "FD_CLR fd = %d sock_fd = %d\n", fd, sock_fd);
#endif
	      FD_CLR(fd, iset);
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
  int sock_nfds = 0;
  int live_fds = 0;
  fd_set new_readfds, new_writefds, new_exceptfds;
  fd_set sock_readfds, sock_writefds, sock_exceptfds;
  fd_set newsock_readfds, newsock_writefds, newsock_exceptfds;
  unsigned int end = 0, now = 0;
  signed int remain;
  struct timeval poll = {0, 0};

  PTHREAD_UNSAFE_CANCELLATION

#ifdef DEBUG
  fprintf (stderr, "Entry:\t%d\t%p\t%p\t%p\t%p\n"
	   "sizeof(fd_set) = %d\t__FD_SETSIZE = %d\n", nfds, readfds, writefds,
	   exceptfds, timeout, sizeof(fd_set), __FD_SETSIZE);
#endif

  if (timeout)
    {
      /* 21474836.48 seconds will fit in 31 bits.  */
      if (timeout->tv_usec > 1000000 || timeout->tv_sec > 21474835)
	return __set_errno (EINVAL);

      /* OK, so we can't cope with anything more than roughly 248.55 days!  */
      now = clock ();
      end = now
	+ timeout->tv_sec * 100 + (50000 + timeout->tv_usec) / 1000000;
    }

  /* Limit number of bits to check rather than returning an error.  */
  if (nfds > FD_SETSIZE)
    nfds = FD_SETSIZE;

#ifdef DEBUG
  dump_fd_set (nfds, readfds, stderr);
  dump_fd_set (nfds, writefds, stderr);
#endif

  /* Convert the individual fdsets and set highest*/
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

  for (;;)
    {
      int fd = nfds;
      int result;

      /* At this point live_fds is 0, as are all new fd sets.  */

      while (fd--)
	{
	  struct __unixlib_fd *file_desc = &__u->fd[fd];
	  fd_set *read_p   = (readfds && FD_ISSET(fd, readfds)) ?
	    &new_readfds : NULL;
	  fd_set *write_p  = (writefds && FD_ISSET(fd, writefds)) ?
	    &new_writefds : NULL;
	  fd_set *except_p = (exceptfds && FD_ISSET(fd, exceptfds)) ?
	    &new_exceptfds : NULL;

	  /* Don't bother calling if not interested in this fd.  */
	  if (read_p || write_p || except_p)
	    {
#ifdef DEBUG
	      __os_print ("/");
#endif
	      result = __funcall ((*(__dev[file_desc->device].select)),
				  (file_desc, fd, read_p, write_p, except_p));
#ifdef DEBUG
	      __os_print ("\\");
#endif
	      if (result < 0)
		return -1;

	      live_fds += result;
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
      fprintf (stderr, "%d %p %p %p %p %d %d\n", sock_nfds,
	       readfds ? &newsock_readfds : readfds,
	       writefds ? &newsock_writefds : writefds,
	       exceptfds ? &newsock_exceptfds : exceptfds,
	       &poll, poll.tv_sec, poll.tv_usec );
#endif

      /* Internet 5.02 gets very very upset if you try to wait with
	 R0 = 0. It occasionally sets R13_user -= 4, and appears to mess up
	 the SVC stack too
       */
      result = sock_nfds ? _select (sock_nfds,
				    readfds ? &newsock_readfds : readfds,
				    writefds ? &newsock_writefds : writefds,
				    exceptfds ? &newsock_exceptfds : exceptfds,
				    &poll) : sock_nfds;
#ifdef DEBUG
      fprintf (stderr, "%d\n", result);
#endif

      if (result < 0)
	return -1;

      live_fds += result;

      if (live_fds)
	{
#ifdef DEBUG
	  __os_print ("Select is live\n\r");
#endif
	  break;	/* Something is live. Break and return.  */
	}

      if (timeout)
	{
	  /* Difference will always be < 2^31  */
	  remain = (int) end - clock ();

	  if (remain < 0)
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
	{
	  __os_print ("No timeout\n\r");
	}
      __os_print ("Loop\n\r");
#endif

      if (__taskwindow)
	{
	  int regs[10];

	  regs[0] = 6; /* Taskwindow sleep.  */
	  /* Yield.  This is the recommended value for taskwindow
	     sleeping with no poll word.  */
	  regs[1] = 0;

	  __os_swi (OS_UpCall, regs);
	}
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

  if (timeout)
    {
      now = clock () - now;

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

  return live_fds;
}
