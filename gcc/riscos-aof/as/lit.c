/*
 * lit.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <stdlib.h>
#include <stdint.h>
#include "error.h"
#include "input.h"
#include "area.h"
#include "reloc.h"
#include "put.h"
#include "fix.h"
#include "lit.h"

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

static LitList *cache = 0;

static LitList *
litListNew (LitList * next, int offset)
{
  LitList *new;
  if (cache)
    {
      new = cache;
      cache = new->next;
    }
  else
    new = malloc (sizeof (LitList));
  if (new)
    {
      new->next = next;
      new->offset = offset;
      new->lineno = inputLineNo;
    }
  else
    errorOutOfMem ("litListNew");
  return new;
}

static LitInfo *
litInfoNew (LitInfo * more, RelocTag tag, WORD extra, Value value)
{
  LitInfo *new = malloc (sizeof (LitInfo));
  if (new)
    {
      new->next = more;
      new->used = 0;
      new->reloc.Tag = tag;
      new->reloc.lineno = -1;
      new->reloc.offset = -1;
      new->reloc.extra = extra;
      new->reloc.value = valueCopy (value);
    }
  else
    errorOutOfMem ("litInfoNew");
  return new;
}

static void 
litNew (RelocTag tag, WORD extra, int offset, int notbefore, Value value)
{
  LitInfo *p = litFind (areaCurrent->area.info->lits, tag, extra, notbefore, value);
  if (!p)
    {
      p = litInfoNew (areaCurrent->area.info->lits, tag, extra, value);
      areaCurrent->area.info->lits = p;
    }
  p->used = litListNew (p->used, offset);
}

void 
litInt (int size, Value value)
{
  if (areaCurrent)
    {
      litNew (RelocImmN, size, areaCurrent->value.ValueInt.i, areaCurrent->value.ValueInt.i - 4095, value);
    }
  else
    error (ErrorError, TRUE, "No area defined");
}

void 
litOrg (LitInfo * li)
{
  LitList *ll, *nll;
  unsigned char *image = areaCurrent->area.info->image;
  BOOL fp;

  for (; li; li = li->next)
    {
      if (li->reloc.lineno < 0)
	{
	  li->reloc.offset = areaCurrent->value.ValueInt.i;
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
