/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/grp.h,v $
 * $Date: 2004/04/12 13:03:37 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __GRP_H
#define __GRP_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_size_t
#include <stddef.h>

#if !defined __gid_t_defined
typedef __gid_t gid_t;
#define __gid_t_defined
#endif

__BEGIN_DECLS

#define __need_FILE
#include <stdio.h>

struct group
{
  char *gr_name; /* Group name.  */
  char *gr_passwd; /* Group password.  */
  __gid_t gr_gid; /* Group ID.  */
  char **gr_mem; /* Group members.  */
};

/* Rewind the group-file stream.  */
extern void setgrent (void) __THROW;

/* Close the group-file stream.  */
extern void endgrent (void) __THROW;

/* Read an entry from the group-file stream, opening it if necessary.  */
extern struct group *getgrent (void) __THROW;

/* Read an entry from the password-file stream, opening it if
   necessary (re-entrant version).  */
extern int getgrent_r (struct group *__restrict __result_buf,
		       char *__restrict __buffer, size_t __buflen,
		       struct group **__restrict __result) __THROW;

/* Read an entry from stream.  */
extern struct group *fgetgrent (FILE *__stream) __THROW;

/* Read an entry from stream (re-entrant version).  */
extern int fgetgrent_r (FILE *__restrict __stream,
			struct group *__restrict __result_buf,
			char *__restrict __buffer, size_t __buflen,
			struct group **__restrict __result) __THROW;

/* Search for an entry with a matching group ID.  */
extern struct group *getgrgid (gid_t __gid) __THROW;

/* Search for an entry with a matching group ID (re-entrant version).  */
extern int getgrgid_r (gid_t __gid, struct group *__resbuf, char *__buffer,
		       size_t __buflen, struct group **__result) __THROW;

/* Search for an entry with a matching group name.  */
extern struct group *getgrnam (const char *__name) __THROW;

/* Search for an entry with a matching group name (re-entrant version).  */
extern int getgrnam_r (const char *__restrict __name,
		       struct group *__restrict __result_buf,
		       char *__restrict __buffer, size_t __buflen,
		       struct group **__restrict __result) __THROW;

/* Get a list of all supplementary groups this user is in */
extern int getgroups (int __gidsetlen, gid_t *__gidset) __THROW;

/* Set the list of supplementary groups this user is in */
extern int setgroups (int __ngroups, const gid_t *__gidset) __THROW;

/* Build a list of all groups the user is in, then call setgroups on the list*/
extern int initgroups (const char *__name, gid_t __basegid) __THROW;

#ifdef __UNIXLIB_INTERNALS
/* UnixLib group implementation function.  */
extern struct group *__grpread (FILE *__stream, struct group *__grp,
				char *__buf, size_t __buflen) __THROW;
#endif

__END_DECLS

#endif
