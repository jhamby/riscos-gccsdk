#if defined(UNIXLIB) || defined(CROSS_COMPILE)

int dummy = 0;

#else

/* uname.c (c) Copyright 1992 Niklas Röjemo */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "kernel.h"
#include "os.h"
#include "strdup.h"

#define EXTRA(x) ((x)<<1)	/* Reserve double space */

extern int objasm;

char *
special1 (char const *dotpos, char const *end)
{
  int s = end - dotpos;
  if (s == 1)
    switch (*dotpos)
      {
      case 'c':
	return "c";
      case 'h':
	return "h";
      case 'p':
	return "p";
      case 'o':
	return "o";
      case 's':
	return "s";
      case 'm':
	return "m";
      case 't':
	return "t";
      case 'M':
	return "mc";
      }
  else
    {
      if (!strncmp (dotpos, "mc", s))
	return "mc";
    }
  return 0;
}

char *
special2 (char const *dotpos, char const *end)
{
  int s = end - dotpos;
  if (s == 1)
    switch (*dotpos)
      {
      case 'l':
	return "l";
      case 'y':
	return "y";
      }
  else
    {
      if (!strncmp (dotpos, "tt", s))
	return "tt";
    }
  return 0;
}


static int sizeName;		/* Length of buffer */
static char *newName;		/* Name buffer */

char *
uname (const char *name, int PrefixDir)
{
  static const char *tmp = 0;
  char const *src;
  char *dst;
  char const *dotpos, *end, *pre;
  int c;

  if (!name)
    return ("");		/* What no filename ! */

  if (EXTRA (c = strlen (name)) > sizeName)
    {				/* Reserve space */
      if (sizeName)
	free (newName);
      sizeName = c + 100;	/* Add 100 increases chance we don't need to allocate again */
      if ((newName = malloc (sizeName)) == 0)
	{
	  sizeName = 0;
	  return 0;
	}
    }

  if (strchr (name, ':'))
    {
      if (!tmp)
	tmp = strdup (getenv ("Wimp$ScrapDir"));
      if (strncmp (name, tmp, strlen (tmp)) == 0 && name[strlen (tmp)] == '/')
	{
	  /* it's a temporary file */
	  src = name + strlen (tmp) - 4;
	  dst = newName;
	  goto is_temp;
	}
      else
	{
	  /* not many sane Unix filenames include ':'... */
	  strcpy (newName, name);
	  return newName;
	}
    }

  src = name;
  dst = newName;

  if ((c = *src) == 0)
    {				/* name == "" */
      *dst++ = 0;
      return newName;
    }

  if (c == '<' || c == ':'	/* No sane unixfiles start with < or : */
      || (src[1] == '.' &&	/* $. @. %. ^. &. is not a unixfile    */
	  (c == '$' || c == '@' || c == '%' || c == '^' || c == '&')))
    {
      if (PrefixDir && c == '@' && getenv ("Prefix$Dir"))
	{
	  strcpy (newName, "<Prefix$Dir>");
	  strcat (newName, name + 1);
	}
      else
	strcpy (newName, name);
      return newName;
    }

  if (c == '/')
    {				/* Probably a unixfile */
      if (strncmp (src, "/tmp/", 5) == 0)
	{			/* if file is going into /tmp, check for */
	  char *tmpdir;
	is_temp:
	  tmpdir = getenv ("UnixFS$/tmp");
	  tmpdir = (!tmpdir || *tmpdir == 0) ? "<Wimp$ScrapDir>" : "<UnixFS$/tmp>";
	  /* UnixFS$/tmp, else put in <wimp$Scrapdir> */
	  strcpy (dst, tmpdir);
	  dst += strlen (tmpdir);
	  src += 4;
	}
      else
	*dst++ = '$';		/* change / to $. The dot is set in the while loop below */
    }
  else
    {
      if (PrefixDir && getenv ("Prefix$Dir"))
	{
	  strcpy (newName, "<Prefix$Dir>");
	  dst += strlen ("<Prefix$Dir>");
	}
      else
	*dst++ = '@';		/* insert current dir */
      if (objasm)
	*dst++ = '.';
    }
  if (objasm)
    {
      strcpy (dst, name);
      return newName;
    }

  while (*src)
    {
      while (*src == '/')	/* change multiple / to . */
	src++;
      *dst++ = '.';

      if (src[0] == '.' && src[1] == '.' && (src[2] == '/' || !src[2]))
	{
	  *dst++ = '^';		/* change .. to ^ */
	  src += 2;
	  continue;
	}
      if (src[0] == '.' && (src[1] == '/' || !src[1]))
	{
	  dst--;		/* remove the previous . in newName */
	  src++;		/* and skip . in name */
	  continue;
	}

      end = dotpos = src;	/* locate end of this filename */
      while ((c = *end) != 0 && c != '/')
	{			/* and the position of the last dot in the name */
	  if (c == '.')
	    dotpos = end;
	  end++;
	}
      if ((pre = special2 (src, end)) != 0)
	{			/* This is a /-prefix  ! */
	  if (dst > newName)
	    dst[-1] = '/';
	  while ((*dst++ = *pre++) != 0)
	    ;
	  dst--;
	  src = end;
	}
      else
	{
	  if (dotpos++ > src && dotpos < end)
	    {			/* We have a base.suffix */
	      if ((pre = special1 (dotpos, end)) != 0)
		{
		  while ((*dst++ = *pre++) != 0)	/* This is a special suffix */
		    ;		/* now prefix */
		  dst[-1] = '.';
		  dotpos--;	/* point to dot */
		  while (src < dotpos)	/* Change all other dots into _ */
		    if ((*dst++ = *src++) == '.')	/* when copying */
		      dst[-1] = '_';
		  src = end;
		}
	      else
		{		/* is it a /prefix? */
		  if ((pre = special1 (src, --dotpos)) != 0)
		    {		/* But the prefix is special ! */
		      while (src < end)		/* Copy as is  */
			*dst++ = *src++;
		    }
		  else
		    {
		      while (src < dotpos)	/* Change all dots except the last into _ */
			if ((*dst++ = *src++) == '.')
			  dst[-1] = '_';
		      *dst++ = '/';
		      src++;
		      while (src < end)		/* And now the suffix */
			*dst++ = *src++;
		    }
		}
	    }
	  else
	    {			/* No dot or .base or base. */
	      while (src < end)	/* Change all dots into _ */
		if ((*dst++ = *src++) == '.')
		  dst[-1] = '_';
	    }
	}
    }
  *dst = 0;


  /* a quick and dirty way to ensure the directory exists */
  while (dst > newName && *dst != '.')
    dst--;
  if (*dst == '.')
    {
      *dst = '\0';
      cdir (newName);
      *dst = '.';
    }

  return newName;
}

#endif /* UNIXLIB */
