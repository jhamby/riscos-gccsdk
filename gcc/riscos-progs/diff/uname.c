/* uname.c (c) Copyright 1990 H.Rogers */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "sys/unix.h"
#include "sys/param.h"
#include "sys/os.h"

/* I know this is fucking incomprehensible code - it's fast. ;-) */

/* This bit map controls how uname processes filenames
 * 0 : 1 => No processing done at all
 * 1 : 0 => Truncate each path element at 10 chars, 1 => max length is 55 chars
       (for the longfiles module)
 * 2 : 1 => If path element is too long, drop vowels before truncating
 */

int __uname_control = 1;/* 4+2 */

static char *__unbuf;		/* the name buffer */

/* __sdirseg(s1,&s2) checks s1 to see if it is a special directory name
 * if it is, it copies the translation into s2, returning the next segment
 * if not, it returns 0 */

static char *
__sdirseg (register char *s1, char **s2_)
{
  register char *s2 = *s2_, *s3, *s4;
  register int i, j;

  for (j = 0; j < MAXSDIR; j++)
    {
      if (!(s3 = __sdir[j].name))
	break;
      s3--, s4 = s1 - 1;
      while (++s4, ((i = *++s3) && i == *s4));
      if (!i && (!(i = *s4) || i == '/'))
	{
	  s3 = __sdir[j].riscos_name;
	  while (*s2++ = *s3++);
	  s2--;
	  *s2_ = s2;
	  return (s4);
	}
    }
  return (0);
}

/* __sfixseg(s1) returns -1 if s1 is a special suffix, 0 if not */

static int
__sfixseg (register char *s1)
{
  register char *s3, *s4;
  register int i, j;

  for (j = 0; j < MAXSFIX; j++)
    {
      if (!(s3 = __sfix[j]))
	break;
      s3--, s4 = s1 - 1;
      while (++s4, ((i = *++s3) && i == *s4));
      if (!i && (!(i = *s4) || i == '/'))
	return (-1);
    }
  return (0);
}

#define MAXNAMLEN       10
#define LONG_MAXNAMLEN       55

char *
__uname (char *name, int cflag)
{
  static char *_name;
  register char *s1, *s2, *e1, *e2;
  register int h, i, j, k;
  int r[6];
  char *s2_[1];
  int maxnamlen;

  if (!name)
    return ("");

  if (!__unbuf)
    if (!(__unbuf = malloc (MAXPATHLEN << 1)))
      return (0);

  _name = (_name == __unbuf) ? (__unbuf + MAXPATHLEN) : __unbuf;
  if (__uname_control & 1)
    {
      return (strcpy (_name, name));
    }

  s1 = name;
  s2 = _name;

  if (!(k = *s1))
    {
      *s2++ = 0;
      return (_name);
    }

  maxnamlen = (__uname_control & 2) ? LONG_MAXNAMLEN : MAXNAMLEN;

  /* Process the filename.
     Check the first character of the filename for a '/'.  If found,
     move pointer on one byte.

     Get the value of the byte.  */

  if (k == '/')
    i = *++s1;
  else
    i = k;

  /* h points one character on from i.  */
  h = s1[1];

  /* Case1: Special characters in 'i': $ @ % ^ & and frontslash.  */
  if (i == '$' || i == '@' || i == '%' || i == '^' || i == '&' || i == '\\')
    {
      /* Copy characters from s1 to s2 until we hit a '/'.  */
      while ((j = *s1++) && j != '/')
	*s2++ = j;
      /* We used post indexed addressing which has gone one character too far.  */
      s1--;
      i = j;
      k = '.';
      /* Since we have done the above then there is not the existance
         of special fields in this filename e.g. /dev, /usr and /var.
         So branch past these checks.  */
      goto main;
    }

  /* Case 2: a ':' with a character after it but reject
     sequences :. and ::  */
  if (i == ':' && h && h != '.' && h != ':')
    {
      /* Copy the : and the character after to the destination string.
         Copy any further characters up to the next '/'.  */
      *s2++ = ':';
      *s2++ = h;
      s1 += 2;
      while ((j = *s1++) && j != '/')
	*s2++ = j;
      /* We used post indexed addressing which has gone one character too far.  */
      s1--;
      i = j;
      k = '.';
      /* Since we have done the above then there is not the existance
         of special fields in this filename e.g. /dev, /usr and /var.
         So branch past these checks.  */
      goto main;
    }

  /* Case 3: two consecutive characters but not a colon as the first i.e.
     the format abc:  */
  if (i && i != ':' && h)
    {
      e1 = s1;
      /* Increment e1 until we reach a / . or : */
      while ((j = *++e1) && j != '/' && j != '.' && j != ':');
      if (j == ':')
	{
	  /* We've come across a colon. Copy the characters just passed up
	     to the colon into the destination string.  */
	  /* we know its of the format abc: Now see if its an FS spec or path spec */
	  e1++;
	  while ((*s2++ = *s1), ++s1 < e1)
	    continue;
	  if (e1[0] == ':' || e1[0] == '/')
	    {
	      /* Copy into the destination string any characters up to the
	         next '/' */
	      while ((j = *s1++) && j != '/')
		*s2++ = j;
              /* We used post indexed addressing which has gone
              	 one character too far.  */
	      s1--;
	      i = j;
	      k = (s1 == e1) ? 0 : '.';
	    }
	  else
	    {
	      /* A path spec.  */
	      i = j = '/';
	      k = 0;
	      s1--;
	    }
	  /* Skip special first field checking.  */
	  goto main;
	}
    }

  /* Case 4: The path name does not start with a '/'.  */
  if (k != '/')
    {
      /* If it starts with a dot and the next character is either
         non existant or a '/' then we must be specifing the
         currently selected directory.  */
      if (i == '.' && (!h || h == '/'))
	{
	  *s2++ = '@';
	  s1 += 1;
	  i = h;
	  k = '.';
	}
      else
	k = 0;
      goto main;
    }

  /* dev/ */
  if (i == 'd' && h == 'e' && s1[2] == 'v' && s1[3] == '/')
    {
      /* Copy to the destination string as dev:  */
      s1 += 4;
      while (*s2++ = *s1++);
      s2[-1] = ':';
      s2[0] = 0;
      return (_name);
    }

  /* usr/ */
  if (i == 'u' && h == 's' && s1[2] == 'r' && s1[3] == '/' &&
      (*s2_ = s2, e1 = __sdirseg (s1 + 4, s2_)))
    {
      /* Copy in a path name relevant to usr/ */
      s1 = e1;
      s2 = *s2_;
      i = *s1;
      k = '.';
      goto main;
    }

  /* var/ */
  if (i == 'v' && h == 'a' && s1[2] == 'r' && s1[3] == '/' &&
      (*s2_ = s2, e1 = __sdirseg (s1 + 4, s2_)))
    {
      /* Copy in a path name relevant to var/ */
      s1 = e1;
      s2 = *s2_;
      i = *s1;
      k = '.';
      goto main;
    }

  if (*s2_ = s2, e1 = __sdirseg (s1, s2_))
    {
      s1 = e1;
      s2 = *s2_;
      i = *s1;
      k = '.';
      goto main;
    }

  /* Finally we must have started with a '/' so add a '$' to
     the start of the destination string.  */
  *s2++ = '$';
  k = '.';

main:

  if (i == '/')
    {
      /* Search through string until we have passed all '/'s */
      while ((i = *++s1) == '/');
      h = s1[1];
    }

  if (i && k)
    *s2++ = k;

  if (i)
    for (;;)
      {
        /* Do nothing with ./ */
	if (i == '.' && (h == '/' || !h))
	  {
	    --s2;
	    s1 += 1;
	    goto loop;
	  }

	/* Convert ../ to the RISC OS up directory '^'. */
	if (i == '.' && h == '.' && ((j = s1[2]) == '/' || !j))
	  {
	    *s2++ = '^';
	    s1 += 2;
	    goto loop;
	  }

        /* A redirection operator < */
	if (i == '<' && h)
	  {
	    e1 = s1;
	    while ((j = *++e1) && j != '>');
	    if (j == '>')
	      {
		while ((*s2++ = *s1), ++s1 <= e1);
		while ((j = *s1++) && j && j != '/')
		  *s2++ = j;
		s1--;
		goto loop;
	      }
	  }

	e1 = s1;
	while ((i = *++e1) && i != '/' && i != '.');

	if (i == '.')
	  {
	    e2 = e1;
	    while ((j = *++e2) && j != '/')
	      if (j == '.')
		e1 = e2;
	    if (e2 > ++e1 && __sfixseg (e1))
	      {
		k = e2 - e1;
		while ((*s2++ = *e1), ++e1 < e2);
		if (cflag)
		  {
		    *s2 = 0;
		    if (!os_file (0x05, _name, r) && !r[0])
		      os_file (0x08, _name, r);
		  }
		*s2++ = '.';
		e1 -= k + 1;
	      }
	    else
	      {
		register char *e3;

		e1--;
		e3 = s1;
		while ((*s2++ = *e3), ++e3 < e1);
		*s2 = 0;
		s2 -= (e1 - s1);
		if (!os_file (0x05, _name, r) && r[0] == 2)
		  {
		    while ((j = *s1++) && j != '/')
		      *s2++ = j;
		    s1--;
		    goto loop;
		  }
		e1 = e2;
	      }
	  }
	else
	  e2 = e1;

	i = maxnamlen;
	j = (e1 - s1) - i;
	while (s1 < e1 && i)
	  {
	    k = *s1++;
	    /* Replace dots with backslashes.  */
	    if (k == '.')
	      k = '/';
	    /* Replace @ $ % ^ & : " which are illegal characters in RISC OS
	       filenames with an underscore.  */
	    else if (k == '@' || k == '$' || k == '%' || k == '^' || \
		     k == '&' || k == ':' || k == '"' || !isgraph (k))
	      k = '_';
	    /* || k == '*' */
	    /* If requested remove vowels from the filename.  */
	    if (((__uname_control & 4) == 4) && j > 0 && i < maxnamlen)
	      if (k == 'a' || k == 'e' || k == 'i' || k == 'o' || k == 'u')
		{
		  j--;
		  continue;
		}
	    *s2++ = k;
	    i--;
	  }
	s1 = e2;

      loop:

	if ((i = *s1) == '/')
	  while ((i = *++s1) == '/');

	if (!i || (s2 - _name) > (MAXPATHLEN - maxnamlen - 2))
	  break;

	h = s1[1];
	*s2++ = '.';
      }

  if ((s2 - _name) <= MAXPATHLEN)
    *s2 = 0;
  else
    _name[MAXPATHLEN - 1] = 0;

  return (_name);
}
