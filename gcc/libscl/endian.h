/* endian.h.
   An extension, not part of the original SharedCLibrary distribution.

   (c) Copyright 1997, Nick Burrett.  */

#ifndef __ENDIAN_H
#define __ENDIAN_H 1

/* Definitions for byte order, according to significance of bytes, from low
   addresses to high addresses.  The value is what you get by putting '4'
   in the most significant byte, '3' in the second most significant byte,
   '2' in the second least significant byte, and '1' in the least
   significant byte.  */

#define __LITTLE_ENDIAN	1234
#define __BIG_ENDIAN	4321
#define __PDP_ENDIAN	3412

/* Byte order for RISC OS is little endian.  */
#define __BYTE_ORDER __LITTLE_ENDIAN

/* For compatibility with BSD.  */
#define	LITTLE_ENDIAN	__LITTLE_ENDIAN
#define	BIG_ENDIAN	__BIG_ENDIAN
#define	PDP_ENDIAN	__PDP_ENDIAN
#define	BYTE_ORDER	__BYTE_ORDER

#endif
