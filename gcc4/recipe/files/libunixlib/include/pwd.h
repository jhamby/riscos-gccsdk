/*
 * POSIX Standard 9.2.2 User Database Access <pwd.h>.
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __PWD_H
#define __PWD_H

#ifndef __UNIXLIB_FEATURES
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_size_t
#include <stddef.h>

#if defined __USE_SVID || defined __USE_GNU
#define __need_FILE
#include <stdio.h>
#endif

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

/* Search for an entry with a matching user ID.
   This function is a cancellation point.  */
extern struct passwd *getpwuid (__uid_t __uid) __wur;

/* Search for an entry with a matching username.
   This function is a cancellation point.  */
extern struct passwd *getpwnam (const char *__name) __nonnull ((1)) __wur;

#if defined __USE_SVID || defined __USE_MISC || defined __USE_XOPEN_EXTENDED
/* System V functions.  */

/* Rewind the password-file stream.
   This function is a cancellation point.  */
extern void setpwent (void);

/* Close the password-file stream.
   This function is a cancellation point.  */
extern void endpwent (void);

/* Read an entry from the password-file stream, opening it if necessary.
   This function is a cancellation point.  */
extern struct passwd *getpwent (void) __wur;
#endif

#ifdef  __USE_SVID
/* Read an entry from stream.
   This function is a cancellation point.  */
extern struct passwd *fgetpwent (FILE *__stream) __nonnull ((1)) __wur;

extern int putpwent (const struct passwd *__restrict __p,
		     FILE *__restrict __stream) __nonnull ((1, 2)) __wur;
#endif

#if defined __USE_POSIX || defined __USE_MISC

# if defined __USE_SVID || defined __USE_MISC
/* Read an entry from the password-file stream, opening it if
   necessary (re-entrant version).
   This function is a cancellation point.  */
extern int getpwent_r (struct passwd *__restrict __result_buf,
		       char *__restrict __buffer,
		       size_t __buflen,
		       struct passwd **__restrict __result)
     __nonnull ((1, 2, 4));
#endif

#ifdef __USE_SVID
/* Read an entry from stream (re-entrant version).
   This function is a cancellation point.  */
extern int fgetpwent_r (FILE *__restrict __stream,
			struct passwd *__restrict __result_buf,
			char *__restrict __buffer,
			size_t __buflen,
			struct passwd **__restrict __result)
     __nonnull ((1, 2, 3, 5));
#endif

/* POSIX functions.  */

/* Search for an entry with a matching user ID (re-entrant version).
   This function is a cancellation point.  */
extern int getpwuid_r (__uid_t __uid,
		       struct passwd *__restrict __resbuf,
		       char *__restrict __buffer,
		       size_t __buflen,
		       struct passwd **__restrict __result)
     __nonnull ((2, 3, 5));

/* Search for an entry with a matching username (re-entrant version).
   This function is a cancellation point.  */
extern int getpwnam_r (const char *__restrict __name,
		       struct passwd *__restrict __result_buf,
		       char *__restrict __buffer,
		       size_t __buflen,
		       struct passwd **__restrict __result)
     __nonnull ((1, 2, 3, 5));

#endif /* POSIX || MISC */

#ifdef __USE_GNU
/* Re-construct the password-file line for the given uid in the
   given buffer.
   This function is a cancellation point.  */
extern int getpw (__uid_t __uid, char *__buf);
#endif

__END_DECLS

#endif
