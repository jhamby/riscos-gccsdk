/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/utsname.h,v $
 * $Date: 2000/07/15 14:52:17 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX Standard 4.4: System Identification <sys/utsname.h> */

#ifndef __SYS_UTSNAME_H
#define __SYS_UTSNAME_H 1

#ifndef __SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Structure describing the system and machine.  */
struct utsname
  {
    /* Name of the implementation of the operating system.  */
    char sysname[16];
    /* Name of this node on the network.  */
    char nodename[MAXHOSTNAMELEN];
    /* Current release level of this implementation.  */
    char release[12];
    /* Current version level of this release.  */
    char version[12];
    /* Name of the hardware type the system is running on.  */
    char machine[32];
  };

/* Put information about the system in NAME.  */
extern int uname (struct utsname *__name);

#ifdef __cplusplus
}
#endif

#endif
