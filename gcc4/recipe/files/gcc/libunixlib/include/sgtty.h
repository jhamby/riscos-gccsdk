/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SGTTY_H
#define __SGTTY_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#include <sys/ioctl.h>

__BEGIN_DECLS

/* Fill in *PARAMS with terminal parameters associated with FD.  */
extern int gtty (int __fd, struct sgttyb * __params);

/* Set the terminal parameters associated with FD to *PARAMS.  */
extern int stty (int __fd, const struct sgttyb *__params);

__END_DECLS

#endif
