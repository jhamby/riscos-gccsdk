/*
 * AS an assembler for ARM
 * Copyright (c) 2011-2013 GCCSDK Developers
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * filename.c
 */

//#define TEST
#ifndef TEST
#  include "config.h"
#else
//#  define __riscos__
#endif

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#ifdef TEST
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "common.h"
#ifndef TEST
#  include "error.h"
#endif
#include "filename.h"

typedef enum
{
  eDontKnow,
  eRISCOS,
  eUnix
} PathStyle;

#ifdef __riscos__
#  define NAT_DIR_STR "."
#  define NAT_DIR_CHR '.'
#else
#  define NAT_DIR_STR "/"
#  define NAT_DIR_CHR '/'
#endif

static char *
add_to_out (const char *toadd, size_t toaddsize, char *out, size_t *outsize,
            bool swapdirsep)
{
  if (toaddsize > *outsize)
    {
      *outsize = 0;
      return NULL;
    }
  *outsize -= toaddsize;

  while (toaddsize--)
    {
      char c = *toadd++;
      if (swapdirsep)
	{
	  if (c == '.')
	    c = '/';
	  else if (c == '/')
	    c = '.';
	}
      *out++ = c;
    }
  return out;
}

static char *
add_dir (const char *dir, size_t dirsize, char *out, size_t *outsize,
	 bool swapdirsep)
{
#ifdef __riscos__
  if (dirsize == 0)
    return add_to_out ("$" NAT_DIR_STR, sizeof ("$" NAT_DIR_STR)-1, out, outsize, false);
  if (dirsize == 1 && dir[0] == '.')
    return out;
  if (dirsize == 2 && dir[0] == '.' && dir[1] == '.')
    return add_to_out ("^" NAT_DIR_STR, sizeof ("^" NAT_DIR_STR)-1, out, outsize, false);
#else
  if (dirsize == 1)
    {
      switch (dir[0])
	{
	  case '$':
	    dir = "";
	    dirsize = 0;
	    break;
	  case '^':
	    dir = "..";
	    dirsize = sizeof ("..")-1;
	    break;
	  case '.': // Ignore Unix current directory.
	  case '@': // Ignore RISC OS current directory.
	  case '%': // Ignore library indiation (FIXME: acceptable ?).
	    return out;
	}
    }
  if (dirsize == 2 && dir[0] == '.' && dir[1] == '.')
    swapdirsep = false;
#endif

  out = add_to_out (dir, dirsize, out, outsize, swapdirsep);

  /* Add native directory separator.  */
  out = add_to_out (NAT_DIR_STR, sizeof (NAT_DIR_STR)-1, out, outsize, false);
  return out;
}

static bool
get_dir (const char **in, PathStyle pathStyle, char *out, size_t outsize)
{
  const char *lr = *in + strlen (*in);
  const char *limit;
  if (pathStyle == eRISCOS)
    {
      /* When the path is considered to be RISC OS style, consider ending
	 "foo.bar" as one part.  */
      const char *l;
      for (l = lr; l != *in && *l != '.' && *l != ':'; --l)
	/* */;
      if (l != *in && *l == '.')
	{
	  for (--l; l != *in && *l != '.' && *l != ':'; --l)
	    /* */;
	  /* Eliminate: "$.foo", "^.foo", "@.foo" and "%.foo".  */
	  if (l[1] == '.'
	      && (l[0] == '$' || l[0] == '^' || l[0] == '@' || l[0] == '%'))
	    l = lr;
	}
      if (l == *in)
	limit = lr;
      else
	{
	  const char *sr = strchr (*in, '.');
	  limit = (sr != NULL && sr <= l) ? sr : lr;
	}
    }
  else
    {
      const char *su = strchr (*in, '/');
      limit = (su != NULL) ? su : lr;
    }

  size_t tocpy = limit - *in;
  if (tocpy < outsize)
    {
      memcpy (out, *in, tocpy);
      out[tocpy] = '\0';
      *in += tocpy;
      if (**in != '\0')
	*in += 1;
      return false;
    }
  return true;
}

/**
 * Converts an Unix/RISC OS filename to a native filename (including support
 * for RISC OS paths under Unix).
 */
const char *
FN_AnyToNative (const char *in, unsigned pathidx, char *buf, size_t bufsize,
		bool *state, FN_eOutputType outtype)
{
#ifndef __riscos__
  /* On non-RISC OS platform, detect '<' + RISC OS OS variable + '>' and if so
     expand the RISC OS variable (with '$' being substituted by '_' and all
     in upcased characters) by treating this as an environment variable.  */
  char *in_exp = NULL; /* Will be expanded input buffer.  */
  char *lt;
  while ((lt = strchr (in, '<')) != NULL)
    {
      char *gt = strchr (lt, '>');
      if (gt == NULL)
	break;
      char *varbuf = malloc (gt - (lt + 1) + 1);
#ifndef TEST
      if (varbuf == NULL)
	Error_OutOfMem ();
#endif
      /* Update variable name and substitute '$' by '_'.  */
      char *o, *i;
      for (o = varbuf, i = lt + 1; i != gt; ++o, ++i)
	*o = *i == '$' ? '_' : toupper ((unsigned char)*i);
      *o = '\0';
      char *vardef = getenv (varbuf);
      if (vardef == NULL)
	{
#ifndef TEST
	  /* No such variable defined. Warn, though we may want to error.  */
	  Error (ErrorWarning, "Unknown environment variable '%s'", varbuf);
#endif
	  free (varbuf);
	  break;
	}
      free (varbuf);

      size_t len_vardef = strlen (vardef);
      size_t len_new = strlen (in) + len_vardef - (gt + 1 - lt) + 1;
      char *in_new = malloc (len_new);
#ifndef TEST
      if (in_new == NULL)
	Error_OutOfMem ();
#endif
      memcpy (in_new, in, lt - in);
      memcpy (in_new + (lt - in), vardef, len_vardef);
      strcpy (in_new + (lt - in) + len_vardef, gt + 1);

      free (in_exp);
      in = in_exp = in_new;
    }
#endif

  char *out = buf;
  size_t outsize = bufsize;

  if (bufsize < 2)
    {
#ifndef __riscos__
      free (in_exp);
#endif
      return NULL;
    }
  buf[0] = '\0';

  PathStyle pathStyle;

  /* RISC OS $ root to be found ? I.e.:
   *   ADFS::Disc.$.Foo.Bar or /ADFS::Disc.$/Foo/Bar
   */
  const char *roroot = strstr (in, ".$");
  if (roroot != NULL)
    {
      if (in[0] == '/')
	{
	  ++in;
	  pathStyle = eUnix;
	}
      else
	pathStyle = eRISCOS;
#ifndef __riscos__
      out = add_dir (NULL, 0, out, &outsize, false);
#endif
      out = add_dir (in, roroot + sizeof (".$")-1 - in, out, &outsize, false);
      in = roroot + sizeof (".$")-1;
      if (in[0] == '.' || in[0] == '/')
	++in;
    }
  else if (in[0] == '/')
    {
      out = add_dir (NULL, 0, out, &outsize, false);
      ++in;
      pathStyle = eUnix;
    }
  else if ((in[0] == '$' || in[0] == '@' || in[0] == '%')
	   && (in[1] == '.' || in[1] == '\0'))
    pathStyle = eRISCOS;
  else
    {
      const char *s;
      if ((s = strstr (in, "..")) != NULL
	  && (s == in || s[-1] == '/')
	  && (s[sizeof ("..")-1] == '/' || s[sizeof ("..")-1] == '\0'))
	pathStyle = eUnix;
      else if (in[0] == '.' && (in[1] == '/' || in[1] == '\0'))
	pathStyle = eUnix;
      else if ((s = strchr (in, '^')) != NULL
	       && (s == in || s[-1] == '.')
	       && (s[sizeof ("^")-1] == '.' || s[sizeof ("^")-1] == '\0'))
	pathStyle = eRISCOS;
      else
	pathStyle = eDontKnow;

      /* Detect RISC OS path usage.  */
#ifndef __riscos__
      char path[48];
      for (const char *i = in; *i && i != in + sizeof (path) - (sizeof ("_PATH")-1); ++i)
#else
      for (const char *i = in; *i; ++i)
#endif
	{
	  if (*i == '.' || *i == '/')
	    break;
	  if (*i == ':')
	    {
	      /* RISC OS path detected.  */
	      pathStyle = eRISCOS;
#ifndef __riscos__
	      strcpy (&path[i - in], "_PATH");
	      const char *e = getenv (path);
	      if (e != NULL)
		{
		  /* Select the path part pathidx.  */
		  while (e && e[0] && pathidx)
		    {
		      e = strchr (e, ':');
		      if (e)
			{
			  ++e;
			  --pathidx;
			}
		    }
		  if (e && e[0])
		    {
		      const char *ee = strchr (e, ':');
		      size_t toadd = ee ? (size_t)(ee - e) : strlen (e);
		      /* Strip off any ending slash (if any).  */
		      if (toadd && e[toadd - 1] == '/')
			--toadd;
		      out = add_to_out (e, toadd, out, &outsize, false);
		      /* Add native directory separator.  */
		      out = add_to_out (NAT_DIR_STR, sizeof (NAT_DIR_STR)-1, out, &outsize, false);

		      in = i + 1;
		    }
		}
#else
	      out = add_to_out (in, i + 1 - in, out, &outsize, false);
	      in = i + 1;
#endif
	      break;
	    }
#ifndef __riscos__
	  path[i - in] = *i == '$' ? '_' : toupper ((unsigned char)*i);
#endif
	}
    }

  if (pathidx != 0)
    {
#ifndef __riscos__
      free (in_exp);
#endif
      return NULL;
    }

  /* If we're not sure at this point what the incoming path style is, assume
     Unix and request another iteration (where we will go for RISC OS path
     style).  */
  if (pathStyle == eDontKnow)
    {
      if (*state)
	{
	  *state = false;
	  pathStyle = eUnix;
	}
      else
	{
	  *state = true;
	  pathStyle = eRISCOS;
	}
    }
  else
    assert (*state == false);

  char dirPart[256];
  while (*in)
    {
      if (get_dir (&in, pathStyle, dirPart, sizeof (dirPart)))
	{
#ifndef __riscos__
	  free (in_exp);
#endif
	  return NULL; /* Error.  */
	}
      /* Check on one dot being present, and a non-empty part before and after
         that dot.  */
      char *dot;
      if (in[0] == '\0'
	  && (dot = strchr (dirPart, '.')) != NULL
	  && dirPart[0] != '.' && dot[1] != '\0' && strchr (dot + 1, '.') == NULL)
	{
	  switch (outtype)
	    {
	      case eB_DirSep_A:
		out = add_to_out (dot + 1, strlen (dot + 1), out, &outsize, false);
		out = add_to_out (NAT_DIR_STR, sizeof (NAT_DIR_STR)-1, out, &outsize, false);
		out = add_to_out (dirPart, dot - dirPart, out, &outsize, false);
		break;

	      case eA_Slash_B:
		*dot = '/';
		/* Fall through.  */

	      case eA_Dot_B:
		out = add_dir (dirPart, strlen (dirPart), out, &outsize, false);
		break;
	    }
	}
      else
	{
#ifdef __riscos__
	  const bool swapdotslash = pathStyle != eRISCOS;
#else
	  const bool swapdotslash = pathStyle != eUnix;
#endif
	  out = add_dir (dirPart, strlen (dirPart), out, &outsize, swapdotslash);
	}
    }

  if (out == NULL)
    {
#ifndef __riscos__
      free (in_exp);
#endif
      return NULL;
    }

  /* Strip off trailing directory separator.  */
  if (out != buf && out[-1] == NAT_DIR_CHR)
    out[-1] = '\0';

  if (buf[0] == '\0')
    {
#ifdef __riscos__
      buf[0] = '@';
#else
      buf[0] = '.';
#endif
      buf[1] = '\0';
    }
  
#ifndef __riscos__
  free (in_exp);
#endif
  return buf;
}

#ifdef TEST
typedef struct
{
  const char *in;
  const char *ref[2][3][2][3]; /* Unix/RISC OS, path idx, first/second round, eA_Dot_B/eB_DirSep_A/eA_Slash_B */
} TestData_t;
static const TestData_t TestData[] =
{
  {
    "foo", /* Input */
    {
      { /* Output for Unix */
        { { "foo", "foo", "foo" }, { "foo", "foo", "foo" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "foo", "foo", "foo" }, { "foo", "foo", "foo" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "foo/bar", /* Input */
    {
      { /* Output for Unix */
        { { "foo.bar", "foo.bar", "foo.bar" }, { "foo/bar", "foo/bar", "foo/bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "foo/bar", "foo/bar", "foo/bar" }, { "foo.bar", "foo.bar", "foo.bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "foo.bar", "bar/foo", "foo/bar" }, { "foo.bar", "bar/foo", "foo/bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "foo.bar", "bar.foo", "foo/bar" }, { "foo.bar", "bar.foo", "foo/bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "doh/foo/bar", /* Input */
    {
      { /* Output for Unix */
        { { "doh.foo.bar", "doh.foo.bar", "doh.foo.bar" }, { "doh/foo/bar", "doh/foo/bar", "doh/foo/bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "doh/foo/bar", "doh/foo/bar", "doh/foo/bar" }, { "doh.foo.bar", "doh.foo.bar", "doh.foo.bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "doh.foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "doh/foo.bar", "doh/bar/foo", "doh/foo/bar" }, { "doh.foo.bar", "doh.foo.bar", "doh.foo.bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "doh.foo.bar", "doh.bar.foo", "doh.foo/bar" }, { "doh/foo/bar", "doh/foo/bar", "doh/foo/bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "doh/foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "doh/foo.bar", "bar/doh/foo", "doh/foo/bar" }, { "doh/foo.bar", "doh/bar/foo", "doh/foo/bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "doh/foo.bar", "bar.doh/foo", "doh/foo/bar" }, { "doh.foo.bar", "doh.bar.foo", "doh.foo/bar" } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/foo", /* Input */
    {
      { /* Output for Unix */
        { { "/foo", "/foo", "/foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.foo", "$.foo", "$.foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/foo/.", /* Input */
    {
      { /* Output for Unix */
        { { "/foo", "/foo", "/foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.foo", "$.foo", "$.foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/foo/bar", /* Input */
    {
      { /* Output for Unix */
        { { "/foo/bar", "/foo/bar", "/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.foo.bar", "$.foo.bar", "$.foo.bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "/foo.bar", "/bar/foo", "/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.foo.bar", "$.bar.foo", "$.foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/doh/foo/bar", /* Input */
    {
      { /* Output for Unix */
        { { "/doh/foo/bar", "/doh/foo/bar", "/doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.doh.foo.bar", "$.doh.foo.bar", "$.doh.foo.bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/doh.foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "/doh.foo.bar", "/doh.foo.bar", "/doh.foo.bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.doh/foo/bar", "$.doh/foo/bar", "$.doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/doh/foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "/doh/foo.bar", "/doh/bar/foo", "/doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.doh.foo.bar", "$.doh.bar.foo", "$.doh.foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "ADFS::Disc.$.Doh.A/B.Foo.Bar", /* Input */
    {
      { /* Output for Unix */
        { { "/ADFS::Disc.$/Doh/A.B/Foo.Bar", "/ADFS::Disc.$/Doh/A.B/Bar/Foo", "/ADFS::Disc.$/Doh/A.B/Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "ADFS::Disc.$.Doh.A/B.Foo.Bar", "ADFS::Disc.$.Doh.A/B.Bar.Foo", "ADFS::Disc.$.Doh.A/B.Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "/ADFS::Disc.$/Doh/A.B/Foo/Bar", /* Input */
    {
      { /* Output for Unix */
        { { "/ADFS::Disc.$/Doh/A.B/Foo/Bar", "/ADFS::Disc.$/Doh/A.B/Foo/Bar", "/ADFS::Disc.$/Doh/A.B/Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "ADFS::Disc.$.Doh.A/B.Foo.Bar", "ADFS::Disc.$.Doh.A/B.Foo.Bar", "ADFS::Disc.$.Doh.A/B.Foo.Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "$.^.Doh.A/B.^.Foo.Bar", /* Input */
    {
      { /* Output for Unix */
        { { "/../Doh/A.B/../Foo.Bar", "/../Doh/A.B/../Bar/Foo", "/../Doh/A.B/../Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.^.Doh.A/B.^.Foo.Bar", "$.^.Doh.A/B.^.Bar.Foo", "$.^.Doh.A/B.^.Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "^.^.Doh.A/B.^.Foo.Bar", /* Input */
    {
      { /* Output for Unix */
        { { "../../Doh/A.B/../Foo.Bar", "../../Doh/A.B/../Bar/Foo", "../../Doh/A.B/../Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "^.^.Doh.A/B.^.Foo.Bar", "^.^.Doh.A/B.^.Bar.Foo", "^.^.Doh.A/B.^.Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "@.^.Doh.A/B.^.Foo.Bar", /* Input */
    {
      { /* Output for Unix */
        { { "../Doh/A.B/../Foo.Bar", "../Doh/A.B/../Bar/Foo", "../Doh/A.B/../Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "@.^.Doh.A/B.^.Foo.Bar", "@.^.Doh.A/B.^.Bar.Foo", "@.^.Doh.A/B.^.Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "%.^.Doh.A/B.^.Foo.Bar", /* Input */
    {
      { /* Output for Unix */
        { { "../Doh/A.B/../Foo.Bar", "../Doh/A.B/../Bar/Foo", "../Doh/A.B/../Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "%.^.Doh.A/B.^.Foo.Bar", "%.^.Doh.A/B.^.Bar.Foo", "%.^.Doh.A/B.^.Foo/Bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "$.doh", /* Input */
    {
      { /* Output for Unix */
        { { "/doh", "/doh", "/doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "$.doh", "$.doh", "$.doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "^.doh", /* Input */
    {
      { /* Output for Unix */
        { { "../doh", "../doh", "../doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "^.doh", "^.doh", "^.doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "@.doh", /* Input */
    {
      { /* Output for Unix */
        { { "doh", "doh", "doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "@.doh", "@.doh", "@.doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "%.doh", /* Input */
    {
      { /* Output for Unix */
        { { "doh", "doh", "doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "%.doh", "%.doh", "%.doh" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "DoesNotExit:foo", /* Input */
    {
      { /* Output for Unix */
        { { "DoesNotExit:foo", "DoesNotExit:foo", "DoesNotExit:foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "DoesNotExit:foo", "DoesNotExit:foo", "DoesNotExit:foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "DoesNotExit:foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "DoesNotExit:foo.bar", "bar/DoesNotExit:foo", "DoesNotExit:foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "DoesNotExit:foo.bar", "DoesNotExit:bar.foo", "DoesNotExit:foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "DoesNotExit:foo/bar", /* Input */
    {
      { /* Output for Unix */
        { { "DoesNotExit:foo.bar", "DoesNotExit:foo.bar", "DoesNotExit:foo.bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "DoesNotExit:foo/bar", "DoesNotExit:foo/bar", "DoesNotExit:foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "DoesNotExit:doh.foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "DoesNotExit:doh/foo.bar", "DoesNotExit:doh/bar/foo", "DoesNotExit:doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "DoesNotExit:doh.foo.bar", "DoesNotExit:doh.bar.foo", "DoesNotExit:doh.foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "DoesNotExit:doh/foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "DoesNotExit:doh/foo.bar", "bar/DoesNotExit:doh/foo", "DoesNotExit:doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "DoesNotExit:doh/foo.bar", "DoesNotExit:bar.doh/foo", "DoesNotExit:doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "test:foo", /* Input */
    {
      { /* Output for Unix */
        { { "/first_path/to/foo", "/first_path/to/foo", "/first_path/to/foo" }, { NULL, NULL, NULL } },
        { { "/second_path/to/foo", "/second_path/to/foo", "/second_path/to/foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "test:foo", "test:foo", "test:foo" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "test:foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "/first_path/to/foo.bar", "/first_path/to/bar/foo", "/first_path/to/foo/bar" }, { NULL, NULL, NULL } },
        { { "/second_path/to/foo.bar", "/second_path/to/bar/foo", "/second_path/to/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "test:foo.bar", "test:bar.foo", "test:foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "test:foo/bar", /* Input */
    {
      { /* Output for Unix */
        { { "/first_path/to/foo.bar", "/first_path/to/foo.bar", "/first_path/to/foo.bar" }, { NULL, NULL, NULL } },
        { { "/second_path/to/foo.bar", "/second_path/to/foo.bar", "/second_path/to/foo.bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "test:foo/bar", "test:foo/bar", "test:foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "test:doh.foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "/first_path/to/doh/foo.bar", "/first_path/to/doh/bar/foo", "/first_path/to/doh/foo/bar" }, { NULL, NULL, NULL } },
        { { "/second_path/to/doh/foo.bar", "/second_path/to/doh/bar/foo", "/second_path/to/doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "test:doh.foo.bar", "test:doh.bar.foo", "test:doh.foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "test:doh/foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "/first_path/to/doh/foo.bar", "/first_path/to/bar/doh/foo", "/first_path/to/doh/foo/bar" }, { NULL, NULL, NULL } },
        { { "/second_path/to/doh/foo.bar", "/second_path/to/bar/doh/foo", "/second_path/to/doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "test:doh/foo.bar", "test:bar.doh/foo", "test:doh/foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "tEsT:^.foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "/first_path/to/../foo.bar", "/first_path/to/../bar/foo", "/first_path/to/../foo/bar" }, { NULL, NULL, NULL } },
        { { "/second_path/to/../foo.bar", "/second_path/to/../bar/foo", "/second_path/to/../foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "tEsT:^.foo.bar", "tEsT:^.bar.foo", "tEsT:^.foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "../foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "../foo.bar", "../bar/foo", "../foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "^.foo.bar", "^.bar.foo", "^.foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "../../../foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "../../../foo.bar", "../../../bar/foo", "../../../foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "^.^.^.foo.bar", "^.^.^.bar.foo", "^.^.^.foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    ".", /* Input */
    {
      { /* Output for Unix */
        { { ".", ".", "." }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "@", "@", "@" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "././.", /* Input */
    {
      { /* Output for Unix */
        { { ".", ".", "." }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "@", "@", "@" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  },
  {
    "./././foo.bar", /* Input */
    {
      { /* Output for Unix */
        { { "foo.bar", "bar/foo", "foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      },
      { /* Output for RISC OS */
        { { "foo.bar", "bar.foo", "foo/bar" }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
        { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
      }
    }
  }
};

static void
str_set (const char **out, const char *s)
{
  if (*out)
    free ((void *)*out);
  *out = s ? strdup (s) : NULL;
}

static void
str_pr (const char *s)
{
  if (s)
    printf ("\"%s\"", s);
  else
    printf ("NULL");
}

static void
ref_proutput (const char *rslt[3][2][3])
{
  for (unsigned pathidx = 0; pathidx != 3; ++pathidx)
    {
      printf ("        { ");
      for (unsigned iter = 0; iter != 2; ++iter)
	{
	  printf ("{ ");
	  for (unsigned type = 0; type != 3; ++type)
	    {
	      str_pr (rslt[pathidx][iter][type]);
	      if (type + 1 != 3)
		printf (", ");
	    }
	  if (iter + 1 != 2)
	    printf (" }, ");
	  else
	    printf (" }");
	}
      if (pathidx + 1 != 3)
	printf (" },\n");
      else
	printf (" }\n");
    }
}

//#define TEST_MAKEREF

int
main (int argc, char *argv[])
{
  setenv ("TEST_PATH", "/first_path/to/:/second_path/to", 1);
#ifndef __riscos__
  printf ("Testing Unix path output:\n");
#else
  printf ("Testing RISC OS path output:\n");
#endif
  const char *result[3][2][3] =
    {
      { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
      { { NULL, NULL, NULL }, { NULL, NULL, NULL } },
      { { NULL, NULL, NULL }, { NULL, NULL, NULL } }
    };
  char outbuf[64];
  for (unsigned i = 0; i != sizeof (TestData) / sizeof (TestData[0]); ++i)
    {
      for (unsigned pathidx = 0; pathidx != 3; ++pathidx)
	{
	  bool stateArr[3] = { false, false, false };
	  const char *out[3];

	  out[0] = FN_AnyToNative (TestData[i].in, pathidx, outbuf, sizeof (outbuf)-1, &stateArr[0], eA_Dot_B);
	  str_set (&result[pathidx][0][0], out[0]);

	  out[1] = FN_AnyToNative (TestData[i].in, pathidx, outbuf, sizeof (outbuf)-1, &stateArr[1], eB_DirSep_A);
	  str_set (&result[pathidx][0][1], out[1]);

	  out[2] = FN_AnyToNative (TestData[i].in, pathidx, outbuf, sizeof (outbuf)-1, &stateArr[2], eA_Slash_B);
	  str_set (&result[pathidx][0][2], out[2]);

	  assert (stateArr[1] == stateArr[0] && stateArr[2] == stateArr[0]);
	  assert (!(out[0] == NULL && out[1] == NULL && out[2] == NULL && stateArr[0]));

	  if (stateArr[0])
	    {
	      out[0] = FN_AnyToNative (TestData[i].in, pathidx, outbuf, sizeof (outbuf)-1, &stateArr[0], eA_Dot_B);
	      str_set (&result[pathidx][1][0], out[0]);

	      out[1] = FN_AnyToNative (TestData[i].in, pathidx, outbuf, sizeof (outbuf)-1, &stateArr[1], eB_DirSep_A);
	      str_set (&result[pathidx][1][1], out[1]);

	      out[2] = FN_AnyToNative (TestData[i].in, pathidx, outbuf, sizeof (outbuf)-1, &stateArr[2], eA_Slash_B);
	      str_set (&result[pathidx][1][2], out[2]);

	      assert (stateArr[1] == stateArr[0] && stateArr[2] == stateArr[0]);
	      assert (!stateArr[0]);
	    }
	  else
	    {
	      str_set (&result[pathidx][1][0], NULL);
	      str_set (&result[pathidx][1][1], NULL);
	      str_set (&result[pathidx][1][2], NULL);
	    }
	}
#ifdef TEST_MAKEREF
      /* Output current results as reference (so it can easily be pasted back
         in this source).  */
      printf ("  {\n");
      printf ("    "); str_pr (TestData[i].in); printf (", /* Input */\n");
#  ifndef __riscos__
      const unsigned platform_toupdate = 0;
#  else
      const unsigned platform_toupdate = 1;
#  endif
      printf ("    {\n");
      for (unsigned platform = 0; platform != 2; ++platform)
	{
	  printf ("      { /* Output for %s */\n", platform ? "RISC OS" : "Unix");
	  if (platform == platform_toupdate)
	    ref_proutput (result);
	  else
	    ref_proutput (TestData[i].ref[platform]);
	  if (platform + 1 != 2)
	    printf ("      },\n");
	  else
	    printf ("      }\n");
	}
      printf ("    }\n");
      if (i + 1 != sizeof (TestData) / sizeof (TestData[0]))
	printf ("  },\n");
      else
	printf ("  }\n");
#else
      /* Verify current implementation using builtin reference.  */
#  ifndef __riscos__
      const unsigned platform_tocheck = 0;
#  else
      const unsigned platform_tocheck = 1;
#  endif
      bool anydiff = false;
      for (unsigned pathidx = 0; pathidx != 3; ++pathidx)
	{
	  for (unsigned iter = 0; iter != 2; ++iter)
	    {
	      for (unsigned type = 0; type != 3; ++type)
		{
		  const char *act = result[pathidx][iter][type];
		  const char *ref = TestData[i].ref[platform_tocheck][pathidx][iter][type];
		  if (((act == NULL) ^ (ref == NULL))
		      || act != NULL && strcmp (act, ref))
		    anydiff = true;
		}
	    }
	}
      if (anydiff)
	{
	  printf ("NOT OK: %s\n", TestData[i].in);
	  printf ("  Got:\n");
	  ref_proutput (result);
	  printf ("  Expected:\n");
	  ref_proutput (TestData[i].ref[platform_tocheck]);
	}
      else
	printf ("OK: %s\n", TestData[i].in);
#endif
    }
  return 0;
}
#endif
