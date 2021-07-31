/* $NetBSD: mv.c,v 1.45 2016/02/28 10:59:29 mrg Exp $ */

/*
 * Copyright (c) 1989, 1993, 1994
 * The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ken Smith of The State University of New York at Buffalo.
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
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int fflg, iflg, vflg;
static int do_move(char *, char *);

int
mvcmd(int argc, char *argv[])
{
 int ch, len, rval;
 char *p, *endp;
 struct stat sb;
 char path[MAXPATHLEN + 1];
 size_t baselen;

 while ((ch = getopt(argc, argv, "ifv")) != -1)
  switch (ch) {
  case 'i':
   fflg = 0;
   iflg = 1;
   break;
  case 'f':
   iflg = 0;
   fflg = 1;
   break;
  case 'v':
   vflg = 1;
   break;
  default:
   warnx("usage: mv [-fiv] source target\n"
     "       mv [-fiv] source ... directory\nThis dash builtin has rename capability only, (within one FS)\n");
  optind = 0;
  optopt = 0;
  return 1;
  }
 argc -= optind;
 argv += optind;
 optind = 0;
 if (argc < 2){
  (void)fprintf(stderr,"mv: %d arguments but minimum = 2\n", argc);
  return 1;
   }
 /*
  * If the stat on the target fails or the target isn't a directory,
  * try the move.  More than 2 arguments is an error in this case.
  */
 if (stat(argv[argc - 1], &sb) || !S_ISDIR(sb.st_mode)) {
  if (argc > 2){
   (void)fprintf(stderr, "mv: %d arguments but target is not a directory\n", argc);
   return 1;
  }
  return(do_move(argv[0], argv[1]));
 }

 /* It's a directory, move each file into it. */
 // strlcpy could be added to system.c to use this
 // baselen = strlcpy(path, argv[argc - 1], sizeof(path));

 baselen = strlen(argv[argc - 1]);
 if (baselen >= sizeof(path))
 errx(1, "%s: destination pathname too long", argv[argc - 1]);
 memcpy(path, argv[argc - 1], baselen);
 endp = &path[baselen];
 if (!baselen || *(endp - 1) != '/') {
  *endp++ = '/';
  ++baselen;
 }
 for (rval = 0; --argc; ++argv) {
  p = *argv + strlen(*argv) - 1;
  while (*p == '/' && p != *argv)
   *p-- = '\0';
  if ((p = strrchr(*argv, '/')) == NULL)
   p = *argv;
  else
   ++p;

  if ((baselen + (len = strlen(p))) >= MAXPATHLEN) {
   warnx("%s: destination pathname too long", *argv);
   rval = 1;
  } else {
   memmove(endp, p, len + 1);
   if (do_move(*argv, path))
    rval = 1;
  }
 }
 return(rval);
 /* NOTREACHED */
}

static int
do_move(char *from, char *to)
{
 struct stat sb;

 /*
  * (1) If the destination path exists, the -f option is not specified
  * and either of the following conditions are true:
  *
  * (a) The permissions of the destination path do not permit
  *     writing and the standard input is a terminal.
  * (b) The -i option is specified.
  *
  * the mv utility shall write a prompt to standard error and
  * read a line from standard input.  If the response is not
  * affirmative, mv shall do nothing more with the current
  * source file...
  */
 if (!fflg && !access(to, F_OK)) {
  int ask = 1;
  int ch;

  if (iflg) {
   if (access(from, F_OK)) {
    warn("rename %s", from);
    return (1);
   }
   (void)fprintf(stderr, "overwrite %s? ", to);
   //  } else if (stdin_ok && access(to, W_OK) && !stat(to, &sb)) {
     } else if (access(to, W_OK) && !stat(to, &sb)) {
   if (access(from, F_OK)) {
    warn("rename %s", from);
    return (1);
   }
  } else
   ask = 0;
  if (ask) {
   if ((ch = getchar()) != EOF && ch != '\n') {
    int ch2;
    while ((ch2 = getchar()) != EOF && ch2 != '\n')
     continue;
   }
   if (ch != 'y' && ch != 'Y')
    return (0);
  }
 }

 /*
  * (2) If rename() succeeds, mv shall do nothing more with the
  * current source file.  If it fails for any other reason than
  * EXDEV, mv shall write a diagnostic message to the standard
  * error and do nothing more with the current source file.
  *
  * (3) If the destination path exists, and it is a file of type
  * directory and source_file is not a file of type directory,
  * or it is a file not of type directory, and source file is
  * a file of type directory, mv shall write a diagnostic
  * message to standard error, and do nothing more with the
  * current source file...
  */
 if (!rename(from, to)) {
  if (vflg)
   printf("%s -> %s\n", from, to);
  return (0);
 }

 if (errno != EXDEV) {
  warn("rename %s to %s", from, to);
  return (1);
 }

 /*
  * (4) If the destination path exists, mv shall attempt to remove it.
  * If this fails for any reason, mv shall write a diagnostic
  * message to the standard error and do nothing more with the
  * current source file...
  */
 if (!lstat(to, &sb)) {
  if ((S_ISDIR(sb.st_mode)) ? rmdir(to) : unlink(to)) {
   warn("can't remove %s", to);
   return (1);
  }
 }

 /*
  * (5) The file hierarchy rooted in source_file shall be duplicated
  * as a file hierarchy rooted in the destination path...
  */
 if (lstat(from, &sb)) {
  warn("%s", from);
  return (1);
 }
return (0);
}

