/*
 * Copyright (c) 2002-2008 UnixLib Developers
 */

#ifndef __GRP_H
#define __GRP_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#include <sys/types.h>

#define __need_size_t
#include <stddef.h>

#if (defined __USE_XOPEN || defined __USE_XOPEN2K) && !defined __gid_t_defined
typedef __gid_t gid_t;
#define __gid_t_defined
#endif

__BEGIN_DECLS

#if defined __USE_SVID || defined __USE_GNU
#define __need_FILE
#include <stdio.h>
#endif

struct group
{
  char *gr_name; /* Group name.  */
  char *gr_passwd; /* Group password.  */
  __gid_t gr_gid; /* Group ID.  */
  char **gr_mem; /* Group members.  */
};

/* Search for an entry with a matching group ID.
   This function is a cancellation point.  */
extern struct group *getgrgid (gid_t __gid) __wur;

/* Search for an entry with a matching group name.
   This function is a cancellation point.  */
extern struct group *getgrnam (const char *__name) __nonnull ((1)) __wur;

#if defined __USE_SVID || defined __USE_BSD || defined __USE_XOPEN_EXTENDED
/* Rewind the group-file stream.
   This function is a cancellation point.  */
extern void setgrent (void);

/* Close the group-file stream.
   This function is a cancellation point.  */
extern void endgrent (void);

/* Read an entry from the group-file stream, opening it if necessary.
   This function is a cancellation point.  */
extern struct group *getgrent (void) __wur;
#endif

#ifdef __USE_SVID
/* Read an entry from stream.  This function is a cancellation point.  */
extern struct group *fgetgrent (FILE *__stream) __nonnull ((1)) __wur;

/* Read an entry from stream (re-entrant version).
   This function is a cancellation point.  */
extern int fgetgrent_r (FILE *__restrict __stream,
			struct group *__restrict __result_buf,
			char *__restrict __buffer, size_t __buflen,
			struct group **__restrict __result)
     __nonnull ((1, 3, 5));
#endif

#if defined __USE_POSIX || defined __USE_MISC

#ifdef __USE_GNU
/* Read an entry from the password-file stream, opening it if
   necessary (re-entrant version).  This is a cancellation point.  */
extern int getgrent_r (struct group *__restrict __result_buf,
		       char *__restrict __buffer, size_t __buflen,
		       struct group **__restrict __result)
     __nonnull ((1, 2, 4));
#endif


/* Search for an entry with a matching group ID (re-entrant version).
   This function is a cancellation point.  */
extern int getgrgid_r (gid_t __gid, struct group *__resbuf, char *__buffer,
		       size_t __buflen, struct group **__result)
     __nonnull ((2, 3, 5));

/* Search for an entry with a matching group name (re-entrant version).
   This function is a cancellation point.  */
extern int getgrnam_r (const char *__restrict __name,
		       struct group *__restrict __result_buf,
		       char *__restrict __buffer, size_t __buflen,
		       struct group **__restrict __result)
     __nonnull ((1, 2, 3, 5));
#endif /* __USE_POSIX || __USE_MISC */

#ifdef __USE_BSD
/* Get a list of all supplementary groups this user is in.
   This function is a cancellation point.  */
extern int getgroups (int __gidsetlen, gid_t *__gidset) __nonnull ((2));

/* Set the list of supplementary groups this user is in */
extern int setgroups (int __ngroups, const gid_t *__gidset)
     __THROW __nonnull ((2));

/* Build a list of all groups the user is in, then call setgroups on the list.
   This function is a cancellation point.  */
extern int initgroups (const char *__name, gid_t __basegid)
     __nonnull ((1));

/* Return a list of groups the user is in */
extern int getgrouplist (const char *__user, gid_t __group,
    gid_t *__groups, int *__ngroups)
     __nonnull ((1,3,4));

#endif

__END_DECLS

#endif
