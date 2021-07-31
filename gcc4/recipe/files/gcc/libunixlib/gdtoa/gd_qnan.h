/* Copyright (c) 2010 UnixLib Developers.  */

#if __ARM_EABI__
/* FIXME: needs to be verified ! */
#  define f_QNAN 0x7fc00000
#  define d_QNAN0 0x0
#  define d_QNAN1 0x7ff80000
#  define ld_QNAN0 0x0
#  define ld_QNAN1 0xc0000000
#  define ld_QNAN2 0x7fff
#  define ld_QNAN3 0x0
#  define ldus_QNAN0 0x0
#  define ldus_QNAN1 0x0
#  define ldus_QNAN2 0x0
#  define ldus_QNAN3 0xc000
#  define ldus_QNAN4 0x7fff
#else
#  define f_QNAN 0x7fc00000
#  define d_QNAN0 0x7ff80000
#  define d_QNAN1 0x0
#  define ld_QNAN0 0x7ff80000
#  define ld_QNAN1 0x0
#  define ld_QNAN2 0x0
#  define ld_QNAN3 0x0
#  define ldus_QNAN0 0x0
#  define ldus_QNAN1 0x7ff8
#  define ldus_QNAN2 0x0
#  define ldus_QNAN3 0x0
#  define ldus_QNAN4 0x0
#endif

