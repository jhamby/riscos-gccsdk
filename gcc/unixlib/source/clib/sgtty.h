/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/sgtty,v $
 * $Date: 1997/12/17 22:02:50 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SGTTY_H
#define __SGTTY_H 1

#include <sys/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Fill in *PARAMS with terminal parameters associated with FD.  */
extern int gtty (int __fd, struct sgttyb * __params);

/* Set the terminal parameters associated with FD to *PARAMS.  */
extern int stty (int __fd, const struct sgttyb *__params);

#ifdef __cplusplus
}
#endif

#endif
