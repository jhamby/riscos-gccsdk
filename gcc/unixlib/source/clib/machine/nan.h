/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/machine/nan.h,v $
 * $Date: 2000/07/15 14:52:14 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef	__MACHINE_NAN_H
#define	__MACHINE_NAN_H	1

/* IEEE Not A Number.  */

extern const char __nan[8];
#define NAN (*(const double *) __nan)

#endif
