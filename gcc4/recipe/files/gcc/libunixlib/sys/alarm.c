/*
 * File taken from glibc.
 *  - SCL poison added.
 */
#ifdef __TARGET_SCL__
#  error "SCL build should not use (L)GPL code."
#endif

/* Copyright (C) 1991, 1992, 1994, 1997 Free Software Foundation, Inc.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

unsigned int
alarm (unsigned int seconds)
{
#if __UNIXLIB_FEATURE_ITIMERS
  struct itimerval old_timer, new_timer;
  unsigned int retval;

  new_timer.it_interval.tv_usec = 0;
  new_timer.it_interval.tv_sec = 0;
  new_timer.it_value.tv_usec = 0;
  new_timer.it_value.tv_sec = (time_t) seconds;
  if (setitimer (ITIMER_REAL, &new_timer, &old_timer) < 0)
    return 0;

  retval = old_timer.it_value.tv_sec;
  if (old_timer.it_value.tv_usec)
    ++retval;
  return retval;
#else
  return __set_errno (ENOSYS);
#endif
}

useconds_t
ualarm (useconds_t useconds, useconds_t interval)
{
#if __UNIXLIB_FEATURE_ITIMERS
  struct itimerval old_timer, new_timer;

  new_timer.it_interval.tv_sec = (time_t)interval / 1000000;
  new_timer.it_interval.tv_usec = (suseconds_t) interval - 1000000 * new_timer.it_interval.tv_sec;;
  new_timer.it_value.tv_sec = (time_t)useconds / 1000000;
  new_timer.it_value.tv_usec = (suseconds_t) useconds - 1000000 * new_timer.it_value.tv_sec;
  if (setitimer (ITIMER_REAL, &new_timer, &old_timer) < 0)
    return -1; /* errno is already set by setitimer() */

  return old_timer.it_value.tv_usec + (useconds_t)new_timer.it_value.tv_sec * 1000000;
#else
  return __set_errno (ENOSYS);
#endif
}
