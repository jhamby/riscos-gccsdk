/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/fnmatch.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX Standard 1003.2-1992: Filename matching <fnmatch.h>.  */

#ifndef	__FNMATCH_H
#define	__FNMATCH_H 1

#ifdef __cplusplus
extern "C" {
#endif


/* Bits for the 'flags' argument to fnmatch().  */

/* No wildcard can ever match `/'.  */
#define	FNM_PATHNAME	(1 << 0)
/* Backslashes don't quote special chars.  */
#define	FNM_NOESCAPE	(1 << 1)
/* Leading `.' is matched only explicitly.  */
#define	FNM_PERIOD	(1 << 2)

#define	FNM_NOMATCH	1

/* Match 'string' against the filename pattern 'pattern',
   returning zero if it matches, FNM_NOMATCH if not.  */
extern int fnmatch (const char *pattern, const char *string, int flags);

#ifdef __cplusplus
}
#endif

#endif
