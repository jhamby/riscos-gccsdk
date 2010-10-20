/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2008 GCCSDK Developers
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
 * help_eval.c
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "error.h"
#include "eval.h"
#include "global.h"
#include "help_eval.h"
#include "symbol.h"

static void
fixValueTag (Value *value)
{
  /* Remove symbols with factor == 0  */
  LateInfo **c;
  for (c = &value->ValueLate.late; *c;)
    {
      if (!(*c)->factor)
	*c = (*c)->next;
      else
	c = &(*c)->next;
    }				/* Decide type of value */
  value->Tag = (value->ValueLate.late) ? ValueLateLabel : ValueInt;
}

void
help_evalNegLate (Value *value)
{
  if (value->Tag & ValueLateLabel)
    for (LateInfo *l = value->ValueLate.late; l; l = l->next)
      l->factor = -l->factor;
}

void
help_evalSubLate (Value *lvalue, const Value *rvalue)
{
  if (rvalue->Tag == ValueInt)	/* No late info in rvalue */
    return;
  if (lvalue->Tag == ValueInt)
    {				/* No late info in lvalue */
      lvalue->Tag = rvalue->Tag;	/* Just move rvalue late info to lvalue */
      lvalue->ValueLate.late = rvalue->ValueLate.late;
      /* But remember to change sign */
      help_evalNegLate (lvalue);
      return;
    }
  /* Both lvalue and rvalue have late info */
  LateInfo *rnext;
  for (LateInfo *r = rvalue->ValueLate.late; r; r = rnext)
    {
      rnext = r->next;
      LateInfo *l;
      for (l = lvalue->ValueLate.late; l; l = l->next)
	{
	  if (l->symbol == r->symbol)
	    {
	      l->factor -= r->factor;
	      break;
	    }
	}			/* New symbol for lvalue, so move from rvalue late info */
      /* And remember to change sign */
      if (!l)
	{			/* Didn't find it */
	  l = lvalue->ValueLate.late;
	  (lvalue->ValueLate.late = r)->next = l;
	  r->factor = -r->factor;
	}
    }
  fixValueTag (lvalue);
}

void
help_evalAddLate (Value *lvalue, const Value *rvalue)
{
  if (rvalue->Tag == ValueInt)	/* Nothing late info in rvalue */
    return;
  if (lvalue->Tag == ValueInt)
    {				/* No late info in lvalue */
      lvalue->Tag = rvalue->Tag;	/* Just move rvalue late to lvalue */
      lvalue->ValueLate.late = rvalue->ValueLate.late;
      return;
    }
  /* Both lvalue and rvalue have late info */
  LateInfo *rnext;
  for (LateInfo *r = rvalue->ValueLate.late; r; r = rnext)
    {
      rnext = r->next;
      LateInfo *l;
      for (l = lvalue->ValueLate.late; l; l = l->next)
	{
	  if (l->symbol == r->symbol)
	    {
	      l->factor += r->factor;
	      break;
	    }
	}			/* New symbol for lvalue, so move from rvalue late info */
      if (!l)
	{			/* Didn't find it */
	  l = lvalue->ValueLate.late;
	  (lvalue->ValueLate.late = r)->next = l;
	}
    }
  fixValueTag (lvalue);
}
