/*
 * AS an assembler for ARM
 * Copyright (c) 2011 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "config.h"

#include "error.h"
#include "frame.h"
#include "input.h"

/**
 * Implements FUNCTION / PROC :
 * label FUNCTION [{reglist1} [, {reglist2}]]
 */
bool
c_function (void)
{
  /* FIXME: not yet supported.  */
  error (ErrorWarning, "FUNCTION/PROC is not yet supported.");
  Input_Rest ();
  return false;
}

/**
 * Implements ENDFUNC / ENDP
 */
bool
c_endfunc (void)
{
  /* FIXME: not yet supported.  */
  error (ErrorWarning, "ENDFUNC/ENDP is not yet supported.");
  Input_Rest ();
  return false;
}

/**
 * Implements FRAME
 */
bool
c_frame (void)
{
  /* FIXME: not yet supported.  */
  error (ErrorWarning, "FRAME is not yet supported.");
  Input_Rest ();
  return false;
}
