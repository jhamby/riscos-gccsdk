/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/pwd.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX Standard 9.2.2 User Database Access <pwd.h>.  */

#ifndef __PWD_H
#define __PWD_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __STDDEF_H
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __FILE_declared
#define __FILE_declared 1
typedef struct __iobuf FILE;
#endif

struct passwd
{
  /* Username.  */
  char	*pw_name;
  /* Password.  */
  char	*pw_passwd;
  /* User ID.  */
  __uid_t pw_uid;
  /* Group ID.  */
  __gid_t pw_gid;
  /* Real name.  */
  char	*pw_gecos;
  /* Home directory.  */
  char	*pw_dir;
  /* Shell program.  */
  char	*pw_shell;
};

/* System V functions.  */

/* Rewind the password-file stream.  */
extern void setpwent (void);

/* Close the password-file stream.  */
extern void endpwent (void);

/* Read an entry from the password-file stream, opening it if necessary.  */
extern struct passwd *getpwent (void);

/* Read an entry from the password-file stream, opening it if
   necessary (re-entrant version).  */
extern int getpwent_r (struct passwd *result_buf, char *buffer,
		       size_t buflen, struct passwd **result);

/* Read an entry from stream.  */
extern struct passwd *fgetpwent (FILE *stream);

/* Read an entry from stream (re-entrant version).  */
extern int fgetpwent_r (FILE *stream, struct passwd *result_buf, char *buffer,
			size_t buflen, struct passwd **result);

extern int putpwent (const struct passwd *p, FILE *stream);

/* POSIX functions.  */

/* Search for an entry with a matching user ID.  */
extern struct passwd *getpwuid (__uid_t uid);

/* Search for an entry with a matching user ID (re-entrant version).  */
extern int getpwuid_r (__uid_t uid, struct passwd *resbuf, char *buffer,
		       size_t buflen, struct passwd **result);

/* Search for an entry with a matching username.  */
extern struct passwd *getpwnam (const char *name);

/* Search for an entry with a matching username (re-entrant version).  */
extern int getpwnam_r (const char *name, struct passwd *result_buf,
		       char *buffer, size_t buflen, struct passwd **result);


/* Re-construct the password-file line for the given uid in the
   given buffer.  */
extern int getpw (__uid_t uid, char *buf);

/* UnixLib pwd implementation function.  */
extern struct passwd *__pwdread (FILE *, struct passwd *);

#ifdef __cplusplus
}
#endif

#endif
