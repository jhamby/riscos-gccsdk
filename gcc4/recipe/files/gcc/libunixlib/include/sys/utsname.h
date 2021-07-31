/*
 * POSIX Standard 4.4: System Identification <sys/utsname.h>
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SYS_UTSNAME_H
#define __SYS_UTSNAME_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

/* Length of the entries in `struct utsname' is 65.  */
#define _UTSNAME_LENGTH 65

/* Linux provides as additional information in the `struct utsname'
   the name of the current domain.  Define _UTSNAME_DOMAIN_LENGTH
   to a value != 0 to activate this entry.  */
#define _UTSNAME_DOMAIN_LENGTH _UTSNAME_LENGTH

/* Structure describing the system and machine.  */
struct utsname
  {
    /* Name of the implementation of the operating system.  */
    char sysname[_UTSNAME_LENGTH];
    /* Name of this node on the network.  */
    char nodename[_UTSNAME_DOMAIN_LENGTH];
    /* Current release level of this implementation.  */
    char release[_UTSNAME_LENGTH];
    /* Current version level of this release.  */
    char version[_UTSNAME_LENGTH];
    /* Name of the hardware type the system is running on.  */
    char machine[_UTSNAME_LENGTH];
  };

/* Put information about the system in NAME.  */
extern int uname (struct utsname *__name) __THROW;

__END_DECLS

#endif
