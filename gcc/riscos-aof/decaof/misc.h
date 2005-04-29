/*
 * miscellaneous function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __MISC_H
#define __MISC_H

#include "cproto.h"

#ifdef RISCOS3
char *basename P__((char *s));
#endif
struct chunkent *find_ent P__((struct chunkhdr *hdr, struct chunkent *ents, char *name));

#endif /* __MISC_H */
