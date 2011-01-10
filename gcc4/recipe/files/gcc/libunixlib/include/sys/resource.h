/*
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#ifndef	__SYS_RESOURCE_H
#define __SYS_RESOURCE_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#include <bits/resource.h>

__BEGIN_DECLS

#if defined __USE_GNU && !defined __cplusplus
typedef enum __rlimit_resource __rlimit_resource_t;
typedef enum __rusage_who __rusage_who_t;
typedef enum __priority_which __priority_which_t;
#else
typedef int __rlimit_resource_t;
typedef int __rusage_who_t;
typedef int __priority_which_t;
#endif

/* Put the soft and hard limits for RESOURCE in *RLIMITS.
   Returns 0 if successful, -1 if not (and sets errno).  */
extern int getrlimit (__rlimit_resource_t, struct rlimit *) __THROW;

/* Set the soft and hard limits for RESOURCE to *RLIMITS.
   Only the super-user can increase hard limits.
   Return 0 if successful, -1 if not (and sets errno).  */
extern int setrlimit (__rlimit_resource_t, const struct rlimit *) __THROW;

/* Return resource usage information on a process.  */
extern int getrusage (__rusage_who_t, struct rusage *) __THROW;

/* Return the highest priority of any process.  */
extern int getpriority (__priority_which_t, __id_t __who) __THROW;

/* Set the priority of all processes.  */
extern int setpriority (__priority_which_t, __id_t __who, int __value) __THROW;

__END_DECLS

#endif /* resource.h  */

