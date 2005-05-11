/*
 * miscellaneous function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 * Copyright 2005 GCCSDK Developers
 */

#ifndef MISC_HEADER_INCLUDED
#define MISC_HEADER_INCLUDED

#ifndef HAVE_BASENAME
extern char *basename (char *s);
#endif

struct chunkent *find_ent (struct chunkhdr *hdr, struct chunkent *ents,
			   const char *name);

#endif /* MISC_HEADER_INCLUDED */
