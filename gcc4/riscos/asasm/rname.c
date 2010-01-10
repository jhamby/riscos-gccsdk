/* RISC OS to Unix format file name conversion.
   (c) Copyright 1996, 2000, 2002 Nick Burrett.

   This source provides one function:
      char *riscos_to_unix (const char *riscos, char *unix)

   A filename passed in 'riscos' is translated and stored in a
   buffer 'unix'. A pointer to 'unix' is also return on exit
   from the function.  The user must have previously declared
   a suitably sized buffer for the Unix format filename.

   Last modified: 16 August 2002 <T.E.Baldwin99@memebers.leeds.ac.uk>  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "rname.h"

/* A list of prefixes we will try to swap round.  */

static const char * const prefixes[] =
{
  "f", "for", "fpp", "p", "pas", "ph", "gpi", "asm",
  "cc", "cxx", "cpp", "c++", "c", "m", "rpo", "Hdr",
  "i", "ii", "icc", "h", "hh", "s", "l", "o", "y",
  "ads", "adb", "ada", "ali", "adc", "xrb", "xrs", 0
};

static int
get_directory_name (const char *input, char *output)
{
#ifdef __riscos__
  const char *t;
#endif

  if (*input == '\0')
    return 0;

#ifdef __riscos__
  /* RISC OS directory names are delimited by a '.'.
     We must check for a few Unix styles though.  */
  t = strchr (input, '.');
  if (t != NULL)
    {
      if (input[0] == '.')
	{
	  if (input[1] == '/')	/* current directory */
	    t = input + 1;
	  else if (input[1] == '.' && input[2] == '/')	/* parent directory */
	    t = input + 2;
	}
      strncpy (output, input, t - input);
      output[t - input] = '\0';
      return 1;
    }
#endif
  /* If we reach here, we have two possibilities:
     1. fname
     2. directory/fname

     both don't need any conversion. No.1 has both compatible with RISC OS
     and Unix, and no. 2 is already in Unix form.  */
  strcpy (output, input);
  return 0;
}

static char *
add_directory_name (char *o, const char *i)
{
#ifdef DEBUG
  printf ("add_dir_name: i = '%s'\n", i);
#endif

  /* Root directory ($) can be ignored since output is only a '/'.
     The backslash is automatically output at the end of the string.  */
  if (i[1] == '\0')
    {
      switch (i[0])
	{
	case '$':
	  /* Do nothing for the root directory becuase a '/' is
	     automatically added at the end.  */
	  break;
	case '^':
	  /* Parent directory. In Unix this is '../' */
	  *o++ = '.';
	  *o++ = '.';
	  break;
	case '@':
	case '.':
	  /* Currently selected directory. In Unix this is './' */
	  *o++ = '.';
	  break;
	case '%':
	  /* The library directory.  In Unix this is '/lib'.  */
	  /* Oh no it isn't, <something>/bin is! - TEB */
	  *o++ = '/';
	  *o++ = 'l';
	  *o++ = 'i';
	  *o++ = 'b';
	  break;
	default:
	  /* Cope with one letter directories.  */
	  *o++ = i[0];
	  break;
	}
    }
  else
    {
      strcpy (o, i);
      o += strlen (i);
    }
  /* Look out for the :^ and :/ constructs.  */
  if (o[-2] == ':' && o[-1] == '^')
  {
    o[-1] = '/';
    *o++ = '.';
    *o++ = '.';
  }
  if (o[-2] == ':' && o[-1] == '/')
    *o++ = '.';
  else
    *o++ = '/';

  *o++ = '\0';
  return o - 1;
}

static char *
add_directory_and_prefix (char *output, const char *dir, const char *prefix)
{
  /* Output in the form: 'dir.prefix' */
  strcpy (output, dir);
  output += strlen (dir);
  *output++ = '.';
  strcpy (output, prefix);
  output += strlen (prefix);
  *output++ = '\0';
  return output - 1;
}

static int
is_prefix (const char *name)
{
  const char *t1;
  int x;
  /* If there is more than one dot left in the filename, then this
     cannot be the prefix.  */
  if ((t1 = strchr (name, '.')) != strrchr (name, '.'))
    return 0;
  x = 0;
  while (prefixes[x] != 0)
    {
      /* Prefixes must be compared in a case-insensitive manner.  */
      if (strncasecmp (prefixes[x], name, t1 - name) == 0)
	return 1;
      x++;
    }
  return 0;
}

static int
one_dot (const char *name)
{
  int x = 0;

  while (*name)
    if (*name++ == '.')
      x++;
  return (x == 1) ? 1 : 0;
}

char *
riscos_to_unix (const char *filename, char *output)
{
  char *o;
  const char *i;
  char tempbuf[256];
  char *temp;
  int flag = 0;

  o = output;
  i = filename;
  *o = '\0';

  /* Fast case. Look for a `.', `..' or `/'.  */
  if (filename[0] == '.' && filename[1] == '\0')
    {
      *output++ = '.';
      *output = '\0';
      return output;
    }

  if (filename[0] == '.' && filename[1] == '.' && filename[2] == '\0')
    {
      *output++ = '.';
      *output++ = '.';
      *output = '\0';
      return output;
    }

  if (filename[0] == '.' && filename[1] == '.'
      && filename[2] == '/' && filename[3] == '\0')
    {
      *output++ = '.';
      *output++ = '.';
      *output = '\0';
      return output;
    }

  if (filename[0] == '/' && filename[1] == '\0')
    {
      *output++ = '/';
      *output = '\0';
      return output;
    }

  if (filename[0] == '.' && filename[1] == '/' && filename[2] == '\0')
    {
      *output++ = '.';
      *output = '\0';
      return output;
    }

  /* If we take a file name like:
     IDEFS::HD.$.Work.gcc.gcc-272.config.arm.c.rname
     we would like to convert it to:
     /IDEFS::HD.$/Work/gcc/gcc-272/config/arm/rname.c

     Firstly try and locate a '.$'. Anything before this just specifies
     a file system.  */
  temp = strstr (filename, ".$");
  if (temp != NULL)
    {
      /* We've found a '.$' */
      if (*i != '/')
        *output++ = '/';
      temp+= 2;
      while (i != temp)
	*output++ = *i++;
      /* Copy across the '.$' */
      *output++ = '/';
      i++;
    }
#ifndef __riscos__
  else
    {
      /* In a cross-compiling environment we need to find a way of supporting
	 RISC OS filenames such as DeskLib:foobar.h.  I think the best solution
	 is to look for a environment variable of similar name (i.e.
	 DESKLIB_PATH) which (if found) we will substitute into the resulting
	 file name in place of the `DeskLib:'.  */
      temp = strchr (filename, ':');
      if (temp)
	{
	  char env[1024];
	  const char *ef;
	  int x;
	  for (x = 0, ef = filename; ef != temp; ef++)
	    env[x++] = toupper (*ef);
	  strcpy (env + x, "_PATH");
	  char *e = getenv (env);
#ifdef DEBUG
	  printf ("looking for `%s' (%s)\n", env, (e) ? "found" : "not found");
#endif
	  if (e)
	    {
	      /* Found a matching environment variable.  */
	      output = stpcpy (output, e);
	      *output++ = '/';
	      i += temp - filename + 1;
	    }
	}
    }
#endif

  while (get_directory_name (i, tempbuf))
    {
#ifdef DEBUG
      printf ("i = '%s', tempbuf = '%s'\n", i, tempbuf);
#endif
      if (one_dot (i))
	{
	  char name[128];

	  /* We've processed enough of the filename to be left with the
	     following combinations:
	     1. fname.prefix  e.g. fred.c
	     2. prefix.fname  e.g. c.fred
	     3. fname.nonprefix  e.g. fred.jim  */
	  i += strlen (tempbuf) + 1;
	  get_directory_name (i, name);

	  if (is_prefix (name))
	    {
	      /* Method 1.
	         name contains the prefix.
	         tempbuf contains the filename.  */
	      output = add_directory_and_prefix (output, tempbuf, name);
	    }
	  else if (is_prefix (tempbuf))
	    {
	      /* Method 2.
	         tempbuf contains the prefix.
	         name contains the filename.  */
	      /* printf ("i = '%s', t = '%s', t1 = '%s'\n", i, t, t1); */
	      output = add_directory_and_prefix (output, name, tempbuf);
	    }
	  else
	    {
	      /* Method 3. No prefixes in the filename.
	         tempbuf contains the first section.
	         name contains the last bit.  */
#ifndef __riscos__
	      output = add_directory_name (output, tempbuf);
	      output[-1] = '.';
	      output = add_directory_name (output, name);
#else
	      output = add_directory_name (output, tempbuf);
	      output = add_directory_name (output, name);
#endif
	      /* Remove the final backslash automatically added in by
	         add_directory_name.  */
	      output[-1] = '\0';
	    }
	  flag = 1;
	}
      else
	{
	  output = add_directory_name (output, tempbuf);
	  /* Add 1 to get past dot.  */
	  i += strlen (tempbuf) + 1;
	}
    }
#ifdef DEBUG
  printf ("final i = '%s', tempbuf = '%s'\n", i, tempbuf);
#endif

  if (!flag)
    {
      strcpy (output, tempbuf);
    }
#ifdef TEST
  printf ("input = '%s'\noutput = '%s'\n", filename, o);
#endif
  return o;
}

#ifdef TEST
#ifdef __riscos__
#include <unixlib/local.h>

static void test (const char *input)
{
  char out[256], name[256];
  int flags = __RISCOSIFY_DONT_TRUNCATE;

  riscos_to_unix (input, out);
  __riscosify (out, 0, flags, name, sizeof (name));
  printf ("uname = %s\n", name);
}
#else
static void test (const char *input)
{
  char out[256];
  riscos_to_unix (input, out);
}
#endif

int
main (void)
{
  test ("machmode.def");
  test ("DeskLib:foobar.h");
  test ("IDEFS::HD.$.Work.gcc.gcc-272.config.arm.c.rname");
  test ("IDEFS::HD.$.Work.gcc.gcc-272.config.arm.for.rname");
  test ("gcc.gcc-272.config.arm.cpp.rname");
  test ("ali.rname");
  test ("rname.xrb");
  test ("config/arm/rname.c");
  test ("/arm/rname.c");
  test ("$.fred.jim.harry.c.smart");
  test ("^.harry.c.smart");
  test ("@.ohyeah.c.smart");
  test ("<GCC$Dir>.funky.monky.cc.smart");
  test ("^.^.^.^.cc.up.we.go.cc.cool");
  test ("cc.c++.c.for.lots.of.prefixes.cc.cool");
  test ("../cc.cool");
  test ("./././c.cool");
  test ("../../../c.smart");
  test ("funky.cold.medina.c");
  test ("a.c");
  test ("c.a");
  test ("%.make");
  test ("%.something.in.the.library");
  test ("aa.c");
  test ("c.aa");
  test ("hello.world");
  test ("objects/EltNode-h");
  test ("objects.EltNode-m");
  test ("GCC:objc/list.h");
  test ("gcc:^.getopt.c");
  test ("/gcc:/../getopt.c");
  test ("gcc:/../getopt.c");
  test ("gcc:/getopt.c");
  test ("/idefs::hd.$.fred.preset.s");
  test ("/idefs::hd.$/fred/preset.s");
  test ("c/jimmyhill");
  test (".");
  test ("../../arm-riscos-aof/libiberty/libiberty");
  test ("../../bin/arm-riscos-aof/2_95_2/apcs26/hard/arch2/unixlib/libio");
  test ("Internet:sys.h.time");
  return 0;
}
#endif
