/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/machine/nan.h,v $
 * $Date: 2002/02/07 10:19:30 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef	__MACHINE_NAN_H
#define	__MACHINE_NAN_H	1

/* IEEE Not A Number.  */

extern const char __nan[8];
#define NAN (*(const double *)(const void *) __nan)

#endif
