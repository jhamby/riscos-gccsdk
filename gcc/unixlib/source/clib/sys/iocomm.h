/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/iocomm,v $
 * $Date: 1997/10/09 19:59:54 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_IOCOMM_H
#define __SYS_IOCOMM_H

/* Freenet programmers library - sys/iocomm.h - 23/5/95 */


/*
 * Mask for the part of the request that holds the argument size
 */
#define IOCPARM_MASK 0x7f

/*
 * The type flags for ioctl() requests
 */
#define IOC_OUT      0x40000000       /* Parameter is an input */
#define IOC_IN       0x80000000       /* Parameter is an output */
#define IOC_INOUT    (IOC_IN|IOC_OUT) /* Parameter is both input and output */

/*
 * The macros used to define ioctl() requests
 */
#define _IOR(x,y,t)  (IOC_OUT  |((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)
#define _IOW(x,y,t)  (IOC_IN   |((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)
#define _IOWR(x,y,t) (IOC_INOUT|((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)

#endif
