/*
 * endiandef.h  defines for local host byte sex.
 * Copyright © 1992 Niklas Röjemo
 */
#ifndef ENDIANDEF_H
#define ENDIANDEF_H

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#elif HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#endif

#ifdef __BYTE_ORDER
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define armword(x) (x)
#define ourword(x) (x)
#else
unsigned armword (unsigned val);
unsigned ourword (unsigned val);
#endif /* __BYTE_ORDER != __LITTLE_ENDIAN */
#elif BYTE_ORDER
#define armword(x) (x)
#define ourword(x) (x)
#else
unsigned armword (unsigned val);
unsigned ourword (unsigned val);
#endif /* BYTE_ORDER != LITTLE_ENDIAN */
#endif /* BYTE_ORDER */

#endif /* __ENDIANDEF_H */
