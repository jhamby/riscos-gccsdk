/*
 * miscellaneous functions
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#include <stdio.h>
#include "decaof.h"

#ifdef BSD
#define memcmp(a,b,c)	bcmp(a,b,c)
#endif

/*
 * return last element in pathname
 */
char *
basename(s)
	char *s;
{
	char *cptr = s + strlen(s);
	while (cptr > s) {
		if (*cptr == PATHSEP)
			return(++cptr);
		cptr--;
	}
	return (s);
}

/*
 * locate a chunk entry by chunk ID
 */
struct chunkent *
find_ent(hdr, ents, name)
	struct chunkhdr *hdr;
	struct chunkent *ents;
	char *name;
{
	register i;

	for (i = 0; i < hdr->numchunks; i++)
		if (memcmp(ents[i].chunkid, name, 8) == 0)
			return (&ents[i]);
	return (NULL);
}
