/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/pwd.h,v $
 * $Date: 2003/07/29 23:04:27 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* POSIX Standard 9.2.2 User Database Access <pwd.h>.  */

#ifndef __PWD_H
#define __PWD_H 1

#ifndef __UNIXLIB_FEATURES
#include <unixlib/features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_size_t
#include <stddef.h>

#define __need_FILE
#include <stdio.h>

#if !defined __gid_t_defined
typedef __gid_t gid_t;
#define __gid_t_defined
#endif

#if !defined __uid_t_defined
typedef __uid_t uid_t;
#define __uid_t_defined
#endif

__BEGIN_DECLS

struct passwd
{
  char	*pw_name;  /* Username.  */
  char	*pw_passwd;  /* Password.  */
  __uid_t pw_uid;  /* User ID.  */
  __gid_t pw_gid;  /* Group ID.  */
  char	*pw_gecos;  /* Real name.  */
  char	*pw_dir;  /* Home directory.  */
  char	*pw_shell;  /* Shell program.  */

};

/* System V functions.  */

/* Rewind the password-file stream.  */
extern void setpwent (void) __THROW;

/* Close the password-file stream.  */
extern void endpwent (void) __THROW;

/* Read an entry from the password-file stream, opening it if necessary.  */
extern struct passwd *getpwent (void) __THROW;

/* Read an entry from the password-file stream, opening it if
   necessary (re-entrant version).  */
extern int getpwent_r (struct passwd *__restrict __result_buf,
		       char *__restrict __buffer, size_t __buflen,
		       struct passwd **__restrict __result) __THROW;

/* Read an entry from stream.  */
extern struct passwd *fgetpwent (FILE *__stream) __THROW;

/* Read an entry from stream (re-entrant version).  */
extern int fgetpwent_r (FILE *__restrict __stream,
			struct passwd *__restrict __result_buf,
			char *__restrict __buffer, size_t __buflen,
			struct passwd **__restrict __result) __THROW;

extern int putpwent (const struct passwd *__restrict __p,
		     FILE *__restrict __stream) __THROW;

/* POSIX functions.  */

/* Search for an entry with a matching user ID.  */
extern struct passwd *getpwuid (__uid_t __uid) __THROW;

/* Search for an entry with a matching user ID (re-entrant version).  */
extern int getpwuid_r (__uid_t __uid, struct passwd *__restrict __resbuf,
		       char *__restrict __buffer,
		       size_t __buflen,
		       struct passwd **__restrict __result) __THROW;

/* Search for an entry with a matching username.  */
extern struct passwd *getpwnam (const char *__name) __THROW;

/* Search for an entry with a matching username (re-entrant version).  */
extern int getpwnam_r (const char *__restrict __name,
		       struct passwd *__restrict __result_buf,
		       char *__restrict __buffer, size_t __buflen,
		       struct passwd **__restrict __result) __THROW;


/* Re-construct the password-file line for the given uid in the
   given buffer.  */
extern int getpw (__uid_t __uid, char *__buf) __THROW;

#ifdef __UNIXLIB_INTERNALS
/* UnixLib pwd implementation function.  */
extern struct passwd *__pwdread (FILE *__stream, struct passwd *__ppwd,
				 char *__buf, size_t __buflen);

/* Default values for passwd struct. */
extern struct passwd *__pwddefault (void);
#endif

__END_DECLS

#endif
