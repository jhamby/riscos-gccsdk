/* Define byte-sex for the host system.
   Copyright (c) 2004-2012 UnixLib Developers.  */

#ifndef	_ENDIAN_H
#define	_ENDIAN_H	1

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

/* ARM can be either big or little endian.  */
#ifdef __ARMEB__
#define __BYTE_ORDER __BIG_ENDIAN
#else
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

/* Define the floating-point compatibility macros.  */

#define __FLOAT_BIT_ORDER __LITTLE_ENDIAN
#if defined(__ARM_EABI__) || (!defined(__SOFTFP__) && defined(__VFP_FP__))
/* ARM/VFP format.  */
#  define __FLOAT_WORD_ORDER __BYTE_ORDER
#else
/* ARM/FPA format (always is in a big-endian order).  */
#  define __FLOAT_WORD_ORDER __BIG_ENDIAN
#endif

#if defined __USE_BSD && !defined __ASSEMBLER__
/* Conversion interfaces.  */
# include <bits/byteswap.h>

# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define htobe16(x) __bswap_16 (x)
#  define htole16(x) (x)
#  define be16toh(x) __bswap_16 (x)
#  define le16toh(x) (x)

#  define htobe32(x) __bswap_32 (x)
#  define htole32(x) (x)
#  define be32toh(x) __bswap_32 (x)
#  define le32toh(x) (x)

#  define htobe64(x) __bswap_64 (x)
#  define htole64(x) (x)
#  define be64toh(x) __bswap_64 (x)
#  define le64toh(x) (x)

# else
#  define htobe16(x) (x)
#  define htole16(x) __bswap_16 (x)
#  define be16toh(x) (x)
#  define le16toh(x) __bswap_16 (x)

#  define htobe32(x) (x)
#  define htole32(x) __bswap_32 (x)
#  define be32toh(x) (x)
#  define le32toh(x) __bswap_32 (x)

#  define htobe64(x) (x)
#  define htole64(x) __bswap_64 (x)
#  define be64toh(x) (x)
#  define le64toh(x) __bswap_64 (x)
# endif
#endif

#endif
