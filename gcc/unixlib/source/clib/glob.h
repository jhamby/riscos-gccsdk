/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __GLOB_H
#define __GLOB_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_size_t
#include <stddef.h>

#undef __ptr_t
#define __ptr_t void *

__BEGIN_DECLS

/* Bits set in the 'flags' argument to `glob'.  */
#define	GLOB_ERR	(1 << 0)/* Return on read errors.  */
#define	GLOB_MARK	(1 << 1)/* Append a slash to each name.  */
#define	GLOB_NOSORT	(1 << 2)/* Don't sort the names.  */
#define	GLOB_DOOFFS	(1 << 3)/* Insert PGLOB->gl_offs NULLs.  */
#define	GLOB_NOCHECK	(1 << 4)/* If nothing matches, return the pattern.  */
#define	GLOB_APPEND	(1 << 5)/* Append to results of a previous call.  */
#define	GLOB_NOESCAPE	(1 << 6)/* Backslashes don't quote metacharacters.  */
#define	GLOB_PERIOD	(1 << 7)/* Leading `.' can be matched by metachars.  */

#if !defined __USE_POSIX2 || defined __USE_BSD || defined __USE_GNU
/* GNU and BSD extensions to the glob function.  */
#define	GLOB_MAGCHAR	(1 << 8)/* Set in gl_flags if any metachars seen.  */
#define GLOB_ALTDIRFUNC	(1 << 9)/* Use gl_opendir et al functions.  */
#define GLOB_BRACE	(1 << 10)/* Expand "{a,b}" to "a" "b".  */
#define GLOB_NOMAGIC	(1 << 11)/* If no magic chars, return the pattern.  */
#define GLOB_TILDE	(1 << 12)/* Expand ~user and ~ to home directories.  */
#define GLOB_QUOTE (1 << 13) /* \ inhibits any meaning the following char has */
#define	__GLOB_FLAGS	(GLOB_ERR|GLOB_MARK|GLOB_NOSORT|GLOB_DOOFFS| \
			 GLOB_NOESCAPE|GLOB_NOCHECK|GLOB_APPEND|     \
			 GLOB_PERIOD|GLOB_ALTDIRFUNC|GLOB_BRACE|     \
			 GLOB_NOMAGIC|GLOB_TILDE)
#else
#define __GLOB_FLAGS   (GLOB_ERR|GLOB_MARK|GLOB_NOSORT|GLOB_DOOFFS| \
                        GLOB_NOESCAPE|GLOB_NOCHECK|GLOB_APPEND|     \
                        GLOB_PERIOD)
#endif

/* Error returns from `glob'.  */
#define	GLOB_NOSPACE	1	/* Ran out of memory.  */
#define GLOB_ABORTED	2	/* Read error.  */
#define	GLOB_ABEND	2	/* Read error.  */
#define	GLOB_NOMATCH	3	/* No matches found.  */
#define GLOB_NOSYS	4	/* Not implemented.  */

/* Need the definition of struct stat.  */
#ifdef __GNUC__
struct stat;
#endif /* !__GNUC__ */

/* Structure describing a globbing run.  */
typedef struct
  {
    /* Count of paths matched by the pattern.  */
    size_t gl_pathc;
    /* List of matched pathnames.  */
    char **gl_pathv;
    /* Slots to reserve in `gl_pathv'.  */
    size_t gl_offs;
    /* Set to FLAGS, maybe | GLOB_MAGCHAR.  */
    int gl_flags;
    /* Number of matches in current invoc. of glob */
    int gl_matchc;

    /* If the GLOB_ALTDIRFUNC flag is set, the following functions
       are used instead of the normal file access functions.  */
    void (*gl_closedir) (void *);
    struct dirent *(*gl_readdir) (void *);
    __ptr_t (*gl_opendir) (const char *);
    int (*gl_lstat) (const char *__restrict, struct stat *__restrict);
    int (*gl_stat) (const char *__restrict, struct stat *__restrict);
    int (*gl_errfunc) (const char *, int);
  } glob_t;

/* Do glob searching for 'pattern'. Results will be placed in 'pglob'.
   'errfunc' is called upon error.  Returns zero on success.  */
extern int glob (const char *__restrict __pattern, int __flags,
		 int (*__errfunc) (const char *, int),
		 glob_t *__restrict __pglob) __THROW;

/* Free storage allocated in 'pglob' by a previous call to glob.  */
extern void globfree (glob_t *__pglob) __THROW;

__END_DECLS

#endif
