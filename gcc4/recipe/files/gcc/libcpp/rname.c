/* RISC OS to Unix format file name conversion.
   (c) Copyright 1996, 2000, Nick Burrett.
   (c) Copyright 2007 GCCSDK Developers.

   This source provides one function:
      extern char *riscos_to_unix (char *riscos)
   
   In cross-compilation builds (CROSS_COMPILE set) there is support
   to map "MyPath:foo.h" into "/a/directory/path/foo.h" when environment
   variable MYPATH_PATH is set to "/a/directory/path".  The XYZ_PATH
   environment variable can be a comma separated search path.  */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

/* #define DEBUG */
/* #define CROSS_COMPILE */
/* #define TEST */

extern char *riscos_to_unix (char *riscos);

char *
riscos_to_unix (char *riscos)
{
#ifdef __riscos__
  return riscos;
#else
  const char *in;
  const char *pathprefix;

  for (in = riscos; *in && *in != ':'; ++in)
    /* */;
  pathprefix = (*in == ':') ? in : NULL;
#ifdef DEBUG
  printf ("In: '%s'\n", riscos);
  printf ("  pathprefix: '%s'\n", pathprefix ? pathprefix : "--NONE--");
#endif
  /* Quick case.  */
  if (pathprefix == NULL)
    return riscos;

  if (pathprefix - riscos < (unsigned int)(64 - sizeof ("_PATH")))
    {
      char env[64];
      const char *es;
      char *ed;
      const char *e;

      for (es = (*riscos == '/') ? riscos + 1 : riscos, ed = env;
	   es != pathprefix; ++es, ++ed)
	*ed = toupper (*es);
      strcpy (ed, "_PATH");
      e = getenv (env);
#ifdef DEBUG
      printf ("  path: '%s' gives '%s'\n", env, e ? e : "--NONE--");
#endif
      if (e != NULL)
	{
	  for (/* */; *e == ' ' || *e == ','; ++e)
	    /* */;
	  if (*e)
	    {
	      char *out = malloc (strlen (e) + strlen (riscos) + 1);
	      while (1)
	        {
	          const char *ed;
		  struct stat buf;
	          for (ed = e; *ed && *ed != ',' && *ed != ' '; ++ed)
		    /* */;
		  memcpy (out, e, ed - e);
		  if (pathprefix[1] != '/')
		    out[ed - e] = '/';
		  strcpy (out + (ed - e) + 1, pathprefix + 1);
		  for (/* */; *ed == ' ' || *ed == ','; ++ed)
	            /* */;
		  if (*ed == '\0' || !stat (out, &buf))
		    break;
		  e = ed;
		}
	      free (riscos);
	      return out;
	    }
        }
    }

  return riscos;
#endif
}

#ifdef TEST
#  ifdef __riscos__
#    include <unixlib/local.h>
#  endif
static void
test (const char *input, const char *expected)
{
  char *out;
#  ifdef __riscos__
  char ul_rname[256];
  char ul_uname[256];
#  endif

  printf ("Test: '%s'\n", input);
  out = riscos_to_unix (strdup (input));
#  ifdef __riscos__
  __unixify (input, 0, ul_uname, sizeof (ul_uname), 0);
  __riscosify (out, 0, 0, ul_rname, sizeof (ul_rname), NULL);
#  endif
  if (!strcmp (out, expected))
    printf ("  ...ok (%s)\n", expected);
  else
    printf ("  ...**NOT** OK: expected '%s', got '%s'\n", expected, out);
#  ifdef __riscos__
  /* The following test just shows that this implementation is slightly
     different than unixify.  */
  if (strcmp (out, ul_uname))
    printf ("  ...output is **NOT** same as unixify() gives: '%s'\n", ul_uname);
  /* This test just shows that going from RISC OS -> Unix -> RISC OS can
     not always be done without loss.  */
  if (strcmp (input, ul_rname))
    printf ("  ...input is **NOT** same as riscosify() gives: '%s'\n", ul_rname);
#  endif
  free (out);
}

int
main (void)
{
#ifdef CROSS_COMPILE
  setenv("TESTPATH_PATH", ", nopath/p, HostFS::bad HostFS::Disc/$/put/valid/path/here  ,last.pointless, (see-testpath-definition-in-source-to-remove-problem)", 1);
  setenv("MORETEST_PATH", "nospacesor_commas_should/return/whole-thing", 1);
  setenv("LASTTEST_PATH", "one two three, ,,thisone", 1);
  setenv("DESKLIB_PATH", "/home/adamr/desklib-aof/\\!DeskLib/include, /home/adamr/desklib-aof/\\!DeskLib/oldinclude", 1);
#endif

  /* XXX_PATH testing (CROSS_COMPILE only).  */
  test ("GCC:objc/list.h",
        "GCC:objc/list.h");
  test ("gcc:^.getopt.c",
        "gcc:^.getopt.c");
  test ("/gcc:/../getopt.c",
        "/gcc:/../getopt.c");
  test ("gcc:/../getopt.c",
        "gcc:/../getopt.c");
  test ("gcc:/getopt.c",
        "gcc:/getopt.c");
  test ("Lasttest:dir/whatever.c",
#ifdef CROSS_COMPILE
        "thisone/dir/whatever.c");
#else
        "/Lasttest:dir/whatever.c");
#endif
  test ("TestPath:inetdown.h",
#ifdef CROSS_COMPILE
        "(see-testpath-definition-in-source-to-remove-problem)/inetdown.h");
#else
        "/TestPath:inetdown.h");
#endif
  test ("Moretest:none",
#ifdef CROSS_COMPILE
        "nospacesor_commas_should/return/whole-thing/none");
#else
        "/Moretest:none");
#endif
  test ("Nopathhere:nothing.to.see.c.here",
#ifdef CROSS_COMPILE
        "Nopathhere:nothing.to.see.c.here");
#else
        "Nopathhere:nothing.to.see.c.here");
#endif
  test ("DeskLib:Wimp.h",
#ifdef CROSS_COMPILE
        "/home/adamr/desklib-aof/\\!DeskLib/oldinclude/Wimp.h");
#else
        "/DeskLib:Wimp.h");
#endif
  test ("DeskLib:WAssert.h",
#ifdef CROSS_COMPILE
        "/home/adamr/desklib-aof/\\!DeskLib/oldinclude/WAssert.h");
#else
        "/DeskLib:WAssert.h");
#endif
  return 0;
}
#endif
