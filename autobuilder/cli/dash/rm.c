/* $NetBSD: rm.c,v 1.53 2013/04/26 18:43:22 christos Exp $ */

/*-
 * Copyright (c) 1990, 1993, 1994, 2003
 * The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int eval, fflag, iflag, vflag;
static int check(char *);
static void checkdot(char **);
static void rm_file(char **);

/*
 * For the sake of the `-f' flag, check whether an error number indicates the
 * failure of an operation due to an non-existent file, either per se (ENOENT)
 * or because its filename argument was illegal (ENAMETOOLONG, ENOTDIR).
 */
#define NONEXISTENT(x) \
    ((x) == ENOENT || (x) == ENAMETOOLONG || (x) == ENOTDIR)

/*
 * rm --
 * This rm is different from historic rm's, but is expected to match
 * POSIX 1003.2 behavior.  The most visible difference is that -f
 * has two specific effects now, ignore non-existent files and force
 *  file removal.
 */
int
rmcmd(int argc, char *argv[])
{
  int ch;
  
/* RISC OS, set the default to -f */
   fflag = 1;
   iflag = 0;
 while ((ch = getopt(argc, argv, "fiv?")) != -1)
  switch (ch) {
  case 'f':
   fflag = 1;
   iflag = 0;
  break; 
  case 'i':
   fflag = 0;
   iflag = 1;
   break;
  case 'v':
   vflag = 1;
   break;
  case '?':
  default:
   warnx("builtin rm usage:  [-i] [-v] file ...\n default is -f(orce), unless interactive [-i]\n");
  optind = 0;
  optopt = 0;
  return 1;
  }
 argc -= optind;
 argv += optind;

 if (argc < 1) {
   warnx("builtin rm: file(s) and/or empty directory(s) needed\n");
   optind = 0;
   return 1;
 }
 checkdot(argv);

 if (*argv)
 rm_file(argv);
 optind = 0;
 return(eval);
 /* NOTREACHED */
}

static void
rm_file(char **argv)
{
 struct stat sb;
 int rval;
 char *f;

 while ((f = *argv++) != NULL) {
/* Assume if can't stat the file, can't unlink it. */
  if (lstat(f, &sb)) {
  if (!fflag && !check(f))
   continue;
  }
  else if (S_ISDIR(sb.st_mode))
   rval = rmdir(f);
  else
   rval = unlink(f);
  if (rval && (!fflag || !NONEXISTENT(errno))) {
   warn("rm: %s", f);
   eval = 1;
  }
  if (vflag && rval == 0)
   (void)printf("%s\n", f);
 }
}

static int
check(char *path)
{
  int ch, first;

 /* Check -i first. */
 if (iflag) {
  (void)fprintf(stderr, "builtin rm: remove '%s'? ", path);
  (void)fflush(stderr);

 first = ch = getchar();
 while (ch != '\n' && ch != EOF)
  ch = getchar();
 return (first == 'y' || first == 'Y');
 }
 return 0;
}
/*
 * POSIX.2 requires that if "." or ".." are specified as the basename
 * portion of an operand, a diagnostic message be written to standard
 * error and nothing more be done with such operands.
 *
 * Since POSIX.2 defines basename as the final portion of a path after
 * trailing slashes have been removed, we'll remove them here.
 */
#define ISDOT(a) ((a)[0] == '.' && (!(a)[1] || ((a)[1] == '.' && !(a)[2])))
static void
checkdot(char **argv)
{
 char *p, **save, **t;
 int complained;

 complained = 0;
 for (t = argv; *t;) {
  /* strip trailing slashes */
  p = strrchr(*t, '\0');
  while (--p > *t && *p == '/')
   *p = '\0';

  /* extract basename */
  if ((p = strrchr(*t, '/')) != NULL)
   ++p;
  else
   p = *t;

  if (ISDOT(p)) {
   if (!complained++)
    warnx("builtin rm: \".\" and \"..\" may not be removed");
   eval = 1;
   for (save = t; (t[0] = t[1]) != NULL; ++t)
    continue;
   t = save;
  } else
   ++t;
 }
}

