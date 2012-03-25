/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2012 GCCSDK Developers
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
 *
 * commands.c
 */

#include "config.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "directive_misc.h"
#include "error.h"
#include "expr.h"
#include "input.h"
#include "main.h"
#include "output.h"
#include "phase.h"

/**
 * Implements IDFN.
 */
bool
c_idfn (void)
{
  free ((void *)idfn_text);
  if ((idfn_text = strdup (Input_Rest ())) == NULL)
    errorOutOfMem();
  return false;
}

/**
 * Implements ASSERT.
 */
bool
c_assert (void)
{
  /* It's important we evaluate the assert expression during both passes and
     only check its result in the second pass.  During the first pass we can
     have local forward references which need to be solved at the end of the
     first pass.  */
  const Value *value = exprBuildAndEval (ValueBool);
  if (gPhase != ePassOne)
    {
      switch (value->Tag)
	{
	  case ValueBool:
	    if (!value->Data.Bool.b)
	      error (ErrorError, "Assertion failed");
	    break;

	  default:
	    error (ErrorError, "ASSERT expression must be boolean");
	    break;
	}
    }
  
  return false;
}

/**
 * Implementation for:
 *   ! <arithmetic expression>, <string expression>
 *   INFO <arithmetic expression>, <string expression>
 *
 * When <arithmetic expression> evaluates to 0, <string expression> is
 * outputed as is.  When it evaluates to non-0, <string expression> is given
 * as error.
 */
bool
c_info (void)
{
  const Value *value = exprBuildAndEval (ValueInt | ValueFloat);
  if (value->Tag != ValueInt && value->Tag != ValueFloat)
    {
      error (ErrorError, "INFO expression must be arithmetic");
      return false;
    }
  bool giveErr = (value->Tag == ValueInt && value->Data.Int.i != 0)
		   || (value->Tag == ValueFloat && fabs (value->Data.Float.f) >= 0.00001);

  skipblanks();
  if (!Input_Match (',', false))
    {
      error (ErrorError, "Missing , in INFO directive");
      return false;
    }

  const Value *message = exprBuildAndEval (ValueString);
  if (message->Tag != ValueString)
    {
      error (ErrorError, "INFO message must be a string");
      return false;
    }

  /* Give output during pass one.  */
  if (gPhase == ePassOne)
    {
      if (giveErr)
	error (ErrorError, "%.*s", (int)message->Data.String.len, message->Data.String.s);
      else
	printf ("%.*s\n", (int)message->Data.String.len, message->Data.String.s);
    }
  return false;
}

/**
 * Implements SUBT (subtitle) / TTL (title).
 */
bool
c_title (void)
{
  Input_Rest ();
  /* Do nothing right now.  This command is for the benefit of error reporting.  */
  return false;
}

/**
 * Implements AOF : selects AOF output format.
 */
bool
c_aof (void)
{
  /* Not supported in AAsm compatibility mode.  */
  if (option_abs)
    return true;
  error (ErrorError, "Directive %s to select output format is not supported.  Use command line option instead.", "AOF");
  return false;
}

/**
 * Implements AOUT : selects the AOUT output format.
 * Note we do not support the aout output format.
 */
bool
c_aout (void)
{
  /* Not supported in AAsm compatibility mode.  */
  if (option_abs)
    return true;
  error (ErrorError, "Directive %s to select output format is not supported.", "AOUT");
  return false;
}