/*
This file implements some system-specific functions, as described in
crtc.c, for a Linux/x86 system. Actually, they're probably not
x86-specific, but I've had no chance to test them on another
processor yet.

Copyright (C) 1998-2000 Free Software Foundation, Inc.

Author: Frank Heckenbach <frank@pascal.gnu.de>

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

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <asm/ioctls.h>
#include <linux/kd.h>

char *crt_setfont_command_80_25 = "setfont cp850-8x16 -u cp437",
     *crt_setfont_command_80_50 = "setfont cp850-8x8 -u cp437",
     *crt_setfont_command_40_25 = "setfont cp850-8x16 -u cp437", /* not supported without... */
     *crt_setfont_command_40_50 = "setfont cp850-8x8 -u cp437";  /* ...using SVGATextMode */

#define HAVE_CRT_SETTEXTMODE
static void crt_settextmode (Boolean Columns40, Boolean Lines50)
{
  /* This is similar to system(), but system() would get interrupted by SIGWINCH */
  pid_t pid = fork ();
  if (pid > 0)
    {
      /* parent */
      pid_t wpid;
      int wstatus;
      crt_Signaled = FALSE;
      do
        wpid = waitpid (pid, &wstatus, 0);
      while (wpid < 0 && errno == EINTR && !crt_Signaled);
      if (!crt_Signaled)
        napms (1000); /* give the kernel some time to change the size of
                         all consoles -- a SIGWINCH will interrupt this */
    }
  else if (pid == 0)
    {
      /* child */
      char *argv [4] = { "sh", "-c", NULL, NULL };
      argv [2] = Columns40
                 ? (Lines50 ? crt_setfont_command_40_50 : crt_setfont_command_40_25)
                 : (Lines50 ? crt_setfont_command_80_50 : crt_setfont_command_80_25);
      execve ("/bin/sh", argv, crt_get_c_environment ());
      _exit (127);
    }
}

#define HAVE_CRT_GETSHIFTSTATE
int crt_getshiftstate ()
{
  int state = crt_VirtualShiftState, arg = 6; /* TIOCLINUX function #6 */
  if (ioctl (0, TIOCLINUX, &arg) == 0)
    {
      if (arg & (1 | 16)) state |= shLeftShift;
      if (arg & (1 | 32)) state |= shRightShift;
      if (arg & 4)        state |= shCtrl;
      if (arg & 8)        state |= shLeftAlt;
      if (arg & 2)        state |= shRightAlt;
      if (arg & 64)       state |= shExtra;
    }
  return state;
}

#define HAVE_CRT_SOUND
void crt_sound (unsigned Hz)
{
  ioctl (1, KIOCSOUND, 1193181 / Hz);
}

void crt_nosound ()
{
  ioctl (1, KIOCSOUND, 0);
}
