/* ARM/RISC OS specifics, common to all stages of compilation.
   Copyright (C) 1997 Free Software Foundation, Inc.
   Contributed by Nick Burrett (nick.burrett@btinternet.com)

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA. */

/*
 * $Log: riscos_fix.c,v $
 * Revision 1.1.1.1  2000/07/15 14:49:37  nick
 * GCC Software Development Kit
 *
 * Revision 1.1  1997/04/27 18:28:57  Nick
 * Initial revision
 *
 */

/* Bits for UnixLib - will need to be changed later.  */
int __uname_dont_pack = 1;
