/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/filio,v $
 * $Date: 1997/10/09 19:59:54 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_FILIO_H
#define __SYS_FILIO_H

/* Freenet programmers library - sys/filio.h - 23/5/95 */
/* Updated 20/2/97 with FIOSLEEPTW */

#ifndef __SYS_IOCOMM_H
#include <sys/iocomm.h>
#endif

#define FIONREAD _IOR('f', 127, int)   /* Get number of bytes to read */
#define FIONBIO  _IOW('f', 126, int)   /* Set/clear non-blocking i/o */
#define FIOASYNC _IOW('f', 125, int)   /* Set/clear asynchronous i/o */
#define FIOSLEEPTW _IOW('f', 121, int)   /* Set/clear OS_Upcall 6 use */

#endif
