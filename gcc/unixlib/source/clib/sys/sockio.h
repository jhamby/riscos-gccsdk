/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/sockio,v $
 * $Date: 1998/01/29 20:16:27 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_SOCKIO_H
#define __SYS_SOCKIO_H

/* Freenet programmers interface - sys/sockio.h - 23/5/95 */

#define SIOCSHIWAT      _IOW('s',  0, int)            /* Set high watermark */
#define SIOCGHIWAT      _IOR('s',  1, int)            /* Get high watermark */
#define SIOCSLOWAT      _IOW('s',  2, int)            /* Set low watermark */
#define SIOCGLOWAT      _IOR('s',  3, int)            /* Get low watermark */
#define SIOCATMARK      _IOR('s',  7, int)            /* At oob mark? */

#define	SIOCADDRT	_IOW('r', 10, struct ortentry)	/* add route */
#define	SIOCDELRT	 _IOW('r', 11, struct ortentry)	/* delete route */
#define SIOCSIFADDR     _IOW('i', 12, struct ifreq)   /* Set ifnet address */
#define SIOCGIFADDR    _IOWR('i', 13, struct ifreq)   /* Get ifnet address */
#define SIOCSIFDSTADDR  _IOW('i', 14, struct ifreq)   /* Set p-p address */
#define SIOCGIFDSTADDR _IOWR('i', 15, struct ifreq)   /* Get p-p address */
#define SIOCSIFFLAGS    _IOW('i', 16, struct ifreq)   /* Set ifnet flags */
#define SIOCGIFFLAGS   _IOWR('i', 17, struct ifreq)   /* Get ifnet flags */
#define SIOCGIFBRDADDR _IOWR('i', 18, struct ifreq)   /* Get broadcast addr */
#define SIOCSIFBRDADDR  _IOW('i', 19, struct ifreq)   /* Set broadcast addr */
#define SIOCGIFCONF    _IOWR('i', 20, struct ifconf)  /* Get ifnet list */
#define SIOCGIFNETMASK _IOWR('i', 21, struct ifreq)   /* Get net addr mask */
#define SIOCSIFNETMASK  _IOW('i', 22, struct ifreq)   /* Set net addr mask */
#define SIOCGIFMETRIC  _IOWR('i', 23, struct ifreq)   /* Get IF metric */
#define SIOCSIFMETRIC   _IOW('i', 24, struct ifreq)   /* Set IF metric */
#define	SIOCADDMULTI	 _IOW('i', 49, struct ifreq)	/* add m'cast addr */
#define	SIOCDELMULTI	 _IOW('i', 50, struct ifreq)	/* del m'cast addr */

#endif
