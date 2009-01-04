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
 * lit.c
 */

#include "config.h"
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "error.h"
#include "fix.h"
#include "input.h"
#include "lit.h"
#include "put.h"
#include "reloc.h"

static LitList *cache = NULL;

static LitInfo *
litFind (LitInfo * p, RelocTag tag, WORD extra, int notbefore, Value value)
{
  for (; p; p = p->next)
    {
      if (p->reloc.lineno < 0 && p->reloc.offset < notbefore)
	return 0;
      if (p->reloc.Tag == tag && p->reloc.extra == extra && valueEqual (&p->reloc.value, &value))
	return p;
    }
  return 0;
}

static LitList *
litListNew (LitList * next, int offset)
{
  LitList *newLitList;

  if (cache)
    {
      newLitList = cache;
      cache = newLitList->next;
    }
  else
    newLitList = malloc (sizeof (LitList));
  if (newLitList)
    {
      newLitList->next = next;
      newLitList->offset = offset;
      newLitList->lineno = inputLineNo;
    }
  else
    errorOutOfMem ("litListNew");
  return newLitList;
}

static LitInfo *
litInfoNew (LitInfo * more, RelocTag tag, WORD extra, Value value)
{
  LitInfo *newLitList = malloc (sizeof (LitInfo));
  if (newLitList)
    {
      newLitList->next = more;
      newLitList->used = 0;
      newLitList->reloc.Tag = tag;
      newLitList->reloc.lineno = -1;
      newLitList->reloc.offset = -1;
      newLitList->reloc.extra = extra;
      newLitList->reloc.value = valueCopy (value);
    }
  else
    errorOutOfMem ("litInfoNew");
  return newLitList;
}

static void
litNew (RelocTag tag, WORD extra, int offset, int notbefore, Value value)
{
  LitInfo *p = litFind (areaCurrentSymbol->area.info->lits, tag, extra, notbefore, value);
  if (!p)
    {
      p = litInfoNew (areaCurrentSymbol->area.info->lits, tag, extra, value);
      areaCurrentSymbol->area.info->lits = p;
    }
  p->used = litListNew (p->used, offset);
}

void
litInt (int size, Value value)
{
  if (areaCurrentSymbol)
    {
      litNew (RelocImmN, size, areaCurrentSymbol->value.ValueInt.i, areaCurrentSymbol->value.ValueInt.i - 4095, value);
    }
  else
    error (ErrorError, TRUE, "No area defined");
}

void
litOrg (LitInfo * li)
{
  LitList *ll, *nll;
  unsigned char *image = areaCurrentSymbol->area.info->image;
  BOOL fp;

  for (; li; li = li->next)
    {
      if (li->reloc.lineno < 0)
	{
	  li->reloc.offset = areaCurrentSymbol->value.ValueInt.i;
	  relocAdd (&li->reloc);
	}
      fp = li->reloc.value.Tag.t == ValueFloat;
      for (ll = li->used; ll; ll = nll)
	{
	  WORD w;
	  int offset = ll->offset;
	  nll = ll->next;
	  ll->next = cache;
	  cache = ll;
	  w = (image[offset + 3] << 24) | (image[offset + 2] << 16) | (image[offset + 1] << 8) | image[offset + 0];
	  if (fp)
	    w = fixCopOffset (ll->lineno, w, li->reloc.offset - ll->offset - 8);
	  else
	    w = fixCpuOffset (ll->lineno, w, li->reloc.offset - ll->offset - 8);
	  image[offset + 3] = (w >> 24) & 0xff;
	  image[offset + 2] = (w >> 16) & 0xff;
	  image[offset + 1] = (w >> 8) & 0xff;
	  image[offset + 0] = w & 0xff;
	}
      if (li->reloc.lineno < 0)
	{
	  li->reloc.lineno = inputLineNo;
	  if (fp)
	    putDataFloat (li->reloc.extra, li->reloc.value.ValueFloat.f);
	  else
	    putData (li->reloc.extra, li->reloc.value.ValueInt.i);
	}
      li->used = 0;
    }
}
