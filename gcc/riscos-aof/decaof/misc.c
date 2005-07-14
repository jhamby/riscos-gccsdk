/*
 * miscellaneous functions
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "config.h"

#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#elif HAVE_STRINGS_H
#include <strings.h>
#endif

#include "decaof.h"
#include "misc.h"

/*
 * return last element in pathname
 */
#ifndef HAVE_BASENAME
char *
basename(char *s)
{
	char *cptr = s + strlen(s);
	while (cptr > s) {
		if (*cptr == '/')
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
