/* config.h.  Generated automatically by configure.  */
/* config.in.  Generated automatically from configure.in by autoheader.  */
/*
Copyright (C) 2000 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA.
*/


/* Define if a limb is long long. */
/* #undef _LONG_LONG_LIMB */

/* Define if we have native implementation of function. */
/* #undef HAVE_NATIVE_ */
/* #undef HAVE_NATIVE_mpn_add */
/* #undef HAVE_NATIVE_mpn_add_1 */
/* #define HAVE_NATIVE_mpn_add_n 1 */
/* #undef HAVE_NATIVE_mpn_add_nc */
/* #define HAVE_NATIVE_mpn_addmul_1 1 */
/* #undef HAVE_NATIVE_mpn_addmul_1c */
/* #undef HAVE_NATIVE_mpn_addsub_n */
/* #undef HAVE_NATIVE_mpn_addsub_nc */
/* #undef HAVE_NATIVE_mpn_and_n */
/* #undef HAVE_NATIVE_mpn_andn_n */
/* #undef HAVE_NATIVE_mpn_bdivmod */
/* #undef HAVE_NATIVE_mpn_cmp */
/* #undef HAVE_NATIVE_mpn_com_n */
/* #define HAVE_NATIVE_mpn_copyd 1
#define HAVE_NATIVE_mpn_copyi 1
#define HAVE_NATIVE_mpn_divexact_by3c 1 */
/* #undef HAVE_NATIVE_mpn_divrem */
/* #define HAVE_NATIVE_mpn_divrem_1 1
#define HAVE_NATIVE_mpn_divrem_1c 1 */
/* #undef HAVE_NATIVE_mpn_divrem_2 */
/* #undef HAVE_NATIVE_mpn_divrem_newton */
/* #undef HAVE_NATIVE_mpn_divrem_classic */
/* #undef HAVE_NATIVE_mpn_dump */
/* #undef HAVE_NATIVE_mpn_gcd */
/* #undef HAVE_NATIVE_mpn_gcd_1 */
/* #undef HAVE_NATIVE_mpn_gcdext */
/* #undef HAVE_NATIVE_mpn_get_str */
/* #undef HAVE_NATIVE_mpn_hamdist */
/* #undef HAVE_NATIVE_mpn_invert_limb */
/* #undef HAVE_NATIVE_mpn_ior_n */
/* #undef HAVE_NATIVE_mpn_iorn_n */
/* #define HAVE_NATIVE_mpn_lshift 1
#define HAVE_NATIVE_mpn_mod_1 1
#define HAVE_NATIVE_mpn_mod_1c 1 */
/* #undef HAVE_NATIVE_mpn_mul */
/* #define HAVE_NATIVE_mpn_mul_1 1 */
/* #undef HAVE_NATIVE_mpn_mul_1c */
/* #define HAVE_NATIVE_mpn_mul_basecase 1 */
/* #undef HAVE_NATIVE_mpn_mul_n */
/* #undef HAVE_NATIVE_mpn_nand_n */
/* #undef HAVE_NATIVE_mpn_nior_n */
/* #undef HAVE_NATIVE_mpn_perfect_square_p */
/* #define HAVE_NATIVE_mpn_popcount 1 */
/* #undef HAVE_NATIVE_mpn_preinv_mod_1 */
/* #undef HAVE_NATIVE_mpn_random2 */
/* #undef HAVE_NATIVE_mpn_random */
/* #undef HAVE_NATIVE_mpn_rawrandom */
/* #define HAVE_NATIVE_mpn_rshift 1 */
/* #undef HAVE_NATIVE_mpn_scan0 */
/* #undef HAVE_NATIVE_mpn_scan1 */
/* #undef HAVE_NATIVE_mpn_set_str */
/* #undef HAVE_NATIVE_mpn_sqrtrem */
/*#define HAVE_NATIVE_mpn_sqr_basecase 1 */
/* #undef HAVE_NATIVE_mpn_sub */
/* #undef HAVE_NATIVE_mpn_sub_1 */
/* #define HAVE_NATIVE_mpn_sub_n 1 */
/* #define HAVE_NATIVE_mpn_sub_nc 1 
#define HAVE_NATIVE_mpn_submul_1 1
#define HAVE_NATIVE_mpn_submul_1c 1 */
/* #undef HAVE_NATIVE_mpn_udiv_w_sdiv */
/* #define HAVE_NATIVE_mpn_umul_ppmm 1
#define HAVE_NATIVE_mpn_udiv_qrnnd 1 */
/* #undef HAVE_NATIVE_mpn_xor_n */
/* #undef HAVE_NATIVE_mpn_xnor_n */

/* Define to 1 if you have the declaration of `optarg', and to 0 if you don't.
   */
#define HAVE_DECL_OPTARG 0

/* ./configure --enable-assert option, to enable some ASSERT()s */
/* #undef WANT_ASSERT */

/* Define if you have the <sys/sysctl.h> header file. */
#undef HAVE_SYS_SYSCTL_H

/* Define if you have the `strtoul' function. */
#define HAVE_STRTOUL 1

/* Name of package */
#define PACKAGE "gmp"

/* Define if you have the `sysctlbyname' function. */
/* #undef HAVE_SYSCTLBYNAME */

/* Define if the system has the type `void'. */
#define HAVE_VOID 1

/* Define if you have the `popen' function. */
#define HAVE_POPEN 1

/* ./configure --disable-alloca option, to use stack-alloc.c, not alloca */
/* #undef USE_STACK_ALLOC */

/* Define if cpp supports the ANSI # stringizing operator. */
#define HAVE_STRINGIZE 1

/* Define if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define if you have the `sysconf' function. */
#define HAVE_SYSCONF 1

/* Define if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define if you have the `processor_info' function. */
/* #undef HAVE_PROCESSOR_INFO */

/* Version number of package */
#define VERSION "3.1"

/* Define if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if a speed_cyclecounter exists (for the tune programs) */
#undef HAVE_SPEED_CYCLECOUNTER

/* Define if mpn/tests has calling conventions checking for the CPU */
#define HAVE_CALLING_CONVENTIONS 1

/* ./configure --enable-fft option, to enable FFTs for multiplication */
/* #undef WANT_FFT */

/* Define if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

