/* Define byte-sex for the host system.
   Copyright (c) 2004-2010 UnixLib Developers.  */

#ifndef	__ENDIAN_H
#define	__ENDIAN_H	1

#include <features.h>

/* Definitions for byte order, according to significance of bytes, from low
   addresses to high addresses.  The value is what you get by putting '4'
   in the most significant byte, '3' in the second most significant byte,
   '2' in the second least significant byte, and '1' in the least
   significant byte.  */

#define	__LITTLE_ENDIAN	1234
#define	__BIG_ENDIAN	4321
#define	__PDP_ENDIAN	3412

/* Byte order for RISC OS is little endian.  */
#define __BYTE_ORDER __LITTLE_ENDIAN

#ifdef __USE_BSD
/* For compatibility with BSD.  */
#  define	LITTLE_ENDIAN	__LITTLE_ENDIAN
#  define	BIG_ENDIAN	__BIG_ENDIAN
#  define	PDP_ENDIAN	__PDP_ENDIAN
#  define	BYTE_ORDER	__BYTE_ORDER
#endif
 
/* Define the floating-point compatibility macros.  */

#ifdef __ARM_EABI__
/* ARM/VFP (do not use __VFP_FP__ instead of __ARM_EABI__ as this not set for
   soft-float builds).  */
#  define __FLOAT_BIT_ORDER __LITTLE_ENDIAN
#  define __FLOAT_WORD_ORDER __LITTLE_ENDIAN
#else
/* ARM/FPA format uses an inconsistent mix of little-endian words,
   stored in a big endian order.  */
#  define __FLOAT_BIT_ORDER __LITTLE_ENDIAN
#  define __FLOAT_WORD_ORDER __BIG_ENDIAN
#endif

#endif
