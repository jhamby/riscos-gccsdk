/*
 * miscellaneous function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __MISC_H
#define __MISC_H

#include "cproto.h"

char *basename P__((char *s));
struct chunkent *find_ent P__((struct chunkhdr *hdr, struct chunkent *ents, char *name));

#endif /* __MISC_H */
