/* eventfd()
 * Copyright (c) 2020 Unixlib Developers
 * Written by Lee Noar
 */

#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <internal/fd.h>
#include <internal/unix.h>
#include <internal/dev.h>
#include <sys/eventfd.h>

#define MAX_COUNTER (UINT64_MAX - 1)

int
eventfd(unsigned int counter_start, int flags)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;

  PTHREAD_UNSAFE

  int fd =  __alloc_file_descriptor (0);
  if (fd == -1)
    return -1;

  struct __unixlib_fd *file_desc = getfd (fd);

  file_desc->devicehandle = sulproc->sul_malloc(sulproc->pid,
						sulproc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return __set_errno (ENOMEM);

  file_desc->devicehandle->handle = sulproc->sul_malloc(sulproc->pid,
							sizeof(eventfd_t));
  if (file_desc->devicehandle->handle == NULL)
    return __set_errno (ENOMEM);

  file_desc->devicehandle->type = DEV_EVENTFD;
  file_desc->devicehandle->refcount = 1;

  file_desc->fflag = O_RDWR;
  file_desc->dflag = flags;

  eventfd_t *count_ptr = (eventfd_t *)file_desc->devicehandle->handle;
  *count_ptr = counter_start;

  return fd;
}

int
__eventfd_read (struct __unixlib_fd *file_desc, void *data, int nbyte)
{
  if (nbyte < sizeof(eventfd_t))
    return __set_errno (EINVAL);

  eventfd_t *count_ptr = (eventfd_t *)file_desc->devicehandle->handle;
  for (;;)
    {
      eventfd_t counter = *count_ptr;
      if (counter != 0)
	{
	  if (file_desc->dflag & EFD_SEMAPHORE)
	    {
	      *((eventfd_t *)data) = 1;
	      *count_ptr = counter - 1;
	    }
	  else
	    {
	      *((eventfd_t *)data) = counter;
	      *count_ptr = 0;
	    }
	  return sizeof(eventfd_t);
	}

	/* If we're not blocking, then we break out of the loop and return EAGAIN.  */
	if (file_desc->dflag & EFD_NONBLOCK)
	  break;

	/* If we are blocking, then yield to other threads so the one doing the writing
	 * can signal when it's ready (by incrementing the counter).  */
	pthread_yield();
    }

  return __set_errno (EAGAIN);
}

int
__eventfd_write (struct __unixlib_fd *file_desc, const void *data, int nbyte)
{
  if (nbyte < sizeof(eventfd_t))
    return __set_errno (EINVAL);

  eventfd_t *count_ptr = (eventfd_t *)file_desc->devicehandle->handle;

  eventfd_t *inc_ptr = (eventfd_t *)data;
  eventfd_t inc = *inc_ptr;

  if (inc == UINT64_MAX)
    return __set_errno (EINVAL);

  for (;;)
    {
      eventfd_t counter = *count_ptr;

      eventfd_t remaining = MAX_COUNTER - counter;

      /* Check to see if the addition would overflow.  */
      if (remaining >= inc)
	{
	  /* The increment will not overflow, so do it and return number of bytes
	   * written.  */
	  *count_ptr = counter + inc;
	  return sizeof(eventfd_t);
	}

      /* The addition will overflow, if we're not blocking, then break out of
       * loop and return EAGAIN.  */
      if (file_desc->dflag & EFD_NONBLOCK)
	break;

      /* If we are blocking, then yield to other threads so that the one doing the
       * reading can signal when it's ready (by reducing the counter).  */
      pthread_yield();
    }

  return __set_errno (EAGAIN);
}

int
__eventfd_select (struct __unixlib_fd *file_desc, int fd, fd_set *read,
		  fd_set *write, fd_set *except)
{
  eventfd_t *count_ptr = (eventfd_t *)file_desc->devicehandle->handle;
  eventfd_t counter = *count_ptr;
  int num_fds = 0;

  if (read)
    {
      if (counter > 0)
	{
	  FD_SET (fd, read);
	  num_fds++;
	}
      else{
	FD_CLR (fd, read);}
    }

  if (write)
    {
      FD_SET (fd, write);
      num_fds++;
    }
  if (except)
    FD_CLR (fd, except);

  return num_fds;
}

int __eventfd_close (struct __unixlib_fd *file_desc)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;

  if (file_desc->devicehandle->handle)
    {
      sulproc->sul_free (sulproc->pid, file_desc->devicehandle->handle);
      file_desc->devicehandle->handle = NULL;
    }

  return 0;
}

/* These functions perform the read and write operations on an eventfd file descriptor,
 * returning 0 if the correct number of bytes was transferred, or -1 otherwise.
 */
int
eventfd_read(int fd, eventfd_t *value)
{
  return read (fd, value, sizeof (eventfd_t)) == sizeof (eventfd_t) ?
		0 : -1;
}

int
eventfd_write(int fd, eventfd_t value)
{
  return write (fd, &value, sizeof (eventfd_t)) == sizeof (eventfd_t) ?
		0 : -1;
}
