/*
 * lit.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _lit_h
#define _lit_h

#ifndef _reloc_h
#include "reloc.h"
#endif

typedef struct LITLIST
{
  struct LITLIST *next;
  int offset;
  long int lineno;
}
LitList;

typedef struct LITINFO
{
  struct LITINFO *next;
  LitList *used;
  Reloc reloc;			/* reloc.lineno == -1 until placed, then line number of LTORG */
}
LitInfo;

void litInt (int size, Value value);
void litOrg (LitInfo * li);

#endif
