/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/getopt,v $
 * $Date: 1997/10/19 21:50:54 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getopt,v 1.4 1997/10/19 21:50:54 unixlib Exp $";
#endif

/*
 * Copyright (c) 1987, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
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

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getopt.c	8.3 (Berkeley) 4/27/95";
#endif /* LIBC_SCCS and not lint */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int opterr = 1;			/* If error message should be printed.  */
int optind = 1;			/* Index into parent argv vector.  */
int optopt;			/* Character checked for validity.  */
int optreset;			/* Reset getopt.  */
char *optarg;			/* Argument associated with option.  */

#define	BADCH	(int)'?'
#define	BADARG	(int)':'
#define	EMSG	""

/*
   getopt --
	Parse argc/argv argument vector.  */

int
getopt (int nargc, char * const *nargv, const char *ostr)
{
  static char *place = EMSG;		/* Option letter processing.  */
  char *oli;				/* Option letter list index.  */
  
  if (optreset || *place == '\0')	/* Update scanning pointer.  */
    {
      optreset = 0;
      if (optind >= nargc || *(place = nargv[optind]) != '-')
	{
	  place = EMSG;
	  return -1;
	}
      if (place[1] && *++place == '-')	/* Found "--".  */
	{
	  ++optind;
	  place = EMSG;
	  return -1;
	}
    }
  optopt = *place++;			/* Option letter okay?  */
  if (optopt == ':' || (oli = strchr (ostr, optopt)) == NULL)
    {
      /* If the user didn't specify '-' as an option, assume it means -1.  */
      if (optopt == (int)'-')
	return -1;
      if (*place == '\0')
	++optind;
      if (opterr && *ostr != ':')
	fprintf (stderr, "%s: illegal option -- %c\n", nargv[0], optopt);
      return BADCH;
    }
  if (*++oli != ':')			/* Don't need argument.  */
    {
      optarg = NULL;
      if (*place == '\0')
	++optind;
    }
  else					/* Need an argument.  */
    {
      if (*place)			/* No white space.  */
	optarg = place;
      else if (nargc <= ++optind)	/* No arg.  */
	{
	  place = EMSG;
	  if (*ostr == ':')
	    return BADARG;
	  if (opterr)
	    fprintf (stderr, "%s: option requires an argument -- %c\n",
		     nargv[0], optopt);
	   return BADCH;
	}
      else				/* White space.  */
	optarg = nargv[optind];
      place = EMSG;
      ++optind;
    }
  return optopt;			/* Dump back option letter.  */
}
