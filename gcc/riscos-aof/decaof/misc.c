/*
 * miscellaneous functions
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 * Copyright 2005 GCCSDK Developers
 */

#include "config.h"
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#elif HAVE_STRINGS_H
#include <strings.h>
#endif
#include "decaof.h"

/*
 * return last element in pathname
 */
#ifndef HAVE_BASENAME
char *
basename(char *s)
{
	char *cptr = s + strlen(s);
	while (cptr > s) {
		if (*cptr == PATHSEP)
			return(++cptr);
		cptr--;
	}
	return (s);
}
#endif

/*
 * locate a chunk entry by chunk ID
 */
struct chunkent *
find_ent(struct chunkhdr *hdr, struct chunkent *ents, const char *name)
{
	int i;

	for (i = 0; i < hdr->numchunks; i++)
		if (memcmp(ents[i].chunkid, name, 8) == 0)
			return (&ents[i]);
	return (NULL);
}
