/*
 * endiandef.h  defines for local host byte sex.
 * Copyright © 1992 Niklas Röjemo
 */
#ifndef ENDIANDEF_H
#define ENDIANDEF_H
#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define armword(x) (x)
#define ourword(x) (x)
#else
unsigned armword (unsigned val);
unsigned ourword (unsigned val);
#endif
#endif /* __ENDIANDEF_H */
