/*
This file implements some system-specific functions, as described in
crtc.c, for DJGPP (using PDCurses).

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

#include <pc.h>
#include <go32.h>

/* We could also use the PDCurses version (by removing the following
   define and function), but this function will distinguish between
   left and right modifiers better. */
#define HAVE_CRT_GETSHIFTSTATE
int crt_getshiftstate ()
{
  int state = crt_VirtualShiftState;
  unsigned short bios_state;
  /* read a word from BIOS memory at linear address 0x417 (i.e., 0x40:0x17) */
  _dosmemgetw (0x417, 1, &bios_state);
  if (bios_state & 2) state |= shLeftShift;
  if (bios_state & 1) state |= shRightShift;
  if (bios_state & 4) state |= (bios_state & 0x100) ? shLeftCtrl : shRightCtrl;
  if (bios_state & 8) state |= (bios_state & 0x200) ? shLeftAlt  : shRightAlt;
  if (bios_state & 0x1000) state |= shExtra;
  return state;
}

#define HAVE_CRT_SOUND
void crt_sound (unsigned Hz)
{
  sound (Hz);
}

void crt_nosound ()
{
  nosound ();
}
