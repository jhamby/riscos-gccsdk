/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/machine/h/nan,v $
 * $Date: 1999/11/16 13:31:17 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef	__MACHINE_NAN_H
#define	__MACHINE_NAN_H	1

/* IEEE Not A Number.  */

extern const char __nan[8];
#define NAN (*(const double *) __nan)

#endif
