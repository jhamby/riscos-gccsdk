/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/float.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __FLOAT_H
#define __FLOAT_H

#define FLT_RADIX	2
#define FLT_ROUNDS	1

#define FLT_MANT_DIG	24
#define FLT_DIG 	6
#define FLT_EPSILON	1.19209290e-7F
#define FLT_MIN_EXP	(-125)
#define FLT_MIN_10_EXP	(-37)
#define FLT_MIN 	1.17549435e-38F
#define FLT_MAX_EXP	128
#define FLT_MAX_10_EXP	38
#define FLT_MAX 	3.40282347e+38F

#define DBL_MANT_DIG	53
#define DBL_DIG 	15
#define DBL_EPSILON	2.2204460492503131e-16
#define DBL_MIN_EXP	(-1021)
#define DBL_MIN_10_EXP	(-307)
#define DBL_MIN 	2.22507385850720138e-308
#define DBL_MAX_EXP	1024
#define DBL_MAX_10_EXP	308
#define DBL_MAX 	1.79769313486231571e+308

#define LDBL_MANT_DIG	53
#define LDBL_DIG	15
#define LDBL_EPSILON	2.2204460492503131e-16L
#define LDBL_MIN_EXP	(-1021)
#define LDBL_MIN_10_EXP (-307)
#define LDBL_MIN	2.22507385850720138e-308L
#define LDBL_MAX_EXP	1024
#define LDBL_MAX_10_EXP 308
#define LDBL_MAX	1.79769313486231571e+308L

#endif
