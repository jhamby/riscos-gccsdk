/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

/* #define DEBUG */

#include <stdio.h>
#include <string.h>
#include <unixlib/local.h>
#include <unixlib/unix.h>
#include <unixlib/os.h>
#include <swis.h>
#include <stdlib.h>
#include <ctype.h>
#include <unixlib/swiparams.h>

#ifndef __GNUC__
#define __inline__ /**/
#endif

/* Special directory list.  */
struct sdir
{
  const char *name;
  const char *riscos_name;
};

typedef struct
{
  int defined;
  const struct sdir sdir;
} __sdir_default_map;

static const char wimp_scrap[] = "<Wimp$ScrapDir>.";

/* /tmp is so common and can't guarantee that $.tmp exists.
   /pipe is used in some programs and can't guarantee that $.pipe exists.  */
static __sdir_default_map def_map[] =
{
  {
    0,
    { "pipe", wimp_scrap },
  },
  {
    0,
    { "tmp", wimp_scrap },
  }
};

/* Maximum number of special directories. This should be dynamic, but
   I cannot be arsed to make it dynamic.  */
#define MAXSDIR 16

/* Special directory list.  */
static struct sdir __sdir[MAXSDIR];

/* Assumes little endian, sizeof (int) == 4 */

struct sfix
{
  struct sfix *next;
  char suffix[1];
};

/* Size of suffix table.  Must be a power of two.  */
#define SFIXSIZE 32

/* Special suffix list.

   This may still change, as the current find API allows the returned string
   to differ from the supplied string. eg "*.cpp" could map to "cc.*".  */
static struct sfix *__sfix[SFIXSIZE] =
{
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

/* Declared at end of file, close to code that accesses it.  */
static const size_t truncate[4];

static char *copy_or_null (char *to, const char *from, size_t len);

/* For all UnixFS$/#* global environment values, set up a mapping as
   requested by the user to map Unix directories to RISC OS directories.
   See __uname.  Called from unix/features.c.  */
void
__sdirinit (void)
{
  int i, j;
  int regs[10];
  char buf[MAXPATHLEN];

  regs[1] = (int) buf;
  regs[3] = 0;

  for (i = 0; i < MAXSDIR; i++)
    {
      char *str;

      /* Use #* so "UnixFS$/" is not matched.  */
      regs[0] = (int) "UnixFS$/#*";
      /* Two less than buf size so can zero terminate below and add '.'.  */
      regs[2] = sizeof (buf) - 2;
      regs[4] = 3;

      /* OS_ReadVarVal fails when no more values found, so exit loop. */
      if (__os_swi (OS_ReadVarVal, regs) != NULL)
	break;

      /* If it ends in ':' it's a path, else add '.'  */
      str = buf + regs[2];
      if (*str != ':')
	*str++ = '.';

      *str = '\0';

      /* Save name and value.  */
      str = strdup ((char *) regs[3] + sizeof ("UnixFS$"));
      if (str == NULL)
	break;
      __sdir[i].name = str;

      str = strdup (buf);
      if (str == NULL)
	break;
      __sdir[i].riscos_name = str;

      /* Check defaults and mark them defined if found.  */
      for (j = sizeof (def_map) / sizeof (__sdir_default_map) - 1; j; j--)
	if (stricmp (__sdir[i].name, def_map[j].sdir.name) == 0)
	  def_map[j].defined = 1;
    }

  /* Add default special directories if not specified by user.  */
  for (j = sizeof (def_map) / sizeof (__sdir_default_map) - 1; j; j--)
    if (def_map[i].defined == 0)
      __sdir[i++] = def_map[j].sdir;

  /* Terminate list.  */
  if (i < MAXSDIR)
    __sdir[i].name = NULL;
}

static __inline__ int
sfix_hash (const char *suffix)
{
  return *suffix & (SFIXSIZE - 1);
}

/* Convert a string of suffixes separated by colons to a list of
   suffixes.  Called from unix/features.c.  */
void
__sfixinit (const char *list)
{
  int i;
  char ch;

  /* Free all memory previously allocated in __sfix.  */
  for (i = 0; i < SFIXSIZE; i++)
    {
      struct sfix *entry;

      for (entry = __sfix[i]; entry;)
        {
          struct sfix *next = entry->next;
          free (entry);
          entry = next;
        }
      __sfix[i] = NULL;
    }

  while ((ch = *list++))
    {
      const char *start;
      size_t length;
      struct sfix *entry;
      int where;

      if (ch == ':')
	continue;

      start = list-1;
      while ((ch = *list++) && ch != ':')
        ;

      length = list - start;	/* Includes ':' or '\0'.  */
      entry = malloc (length + sizeof (struct sfix *));
      if (entry == NULL)
	return;

      memcpy (entry->suffix, start, --length);
      entry->suffix[length] = '\0';
      where = sfix_hash (entry->suffix);
#ifdef DEBUG
      __os_prhex (where); __os_print (" '");
      __os_print (entry->suffix); __os_print ("'\r\n");
#endif
      entry->next = __sfix[where];
      __sfix[where] = entry;

      if (!ch)
        return;	/* Last suffix terminated on '\0'.  */
    }
}

/* End of init code.  */

enum sdirseg_result
{
  sdirseg_no_match,
  sdirseg_match,
  sdirseg_buf_to_small
};

/* sdirseg(&s1,&s2,len) checks s1 to see if it is a special directory name.
   If it is, it copies the translation into s2 and advances s1 to the
   next segment which starts with '/' or '\0'.
   Returns one of enum sdir_result.  */

static enum sdirseg_result
sdirseg (const char **in_p, char **out_p, size_t len)
{
  const char *in, *s3;
  char *out = *out_p;
  char ch;
  int j;

  for (j = 0; j < MAXSDIR; j++)
    {
      s3 = __sdir[j].name;
      if (s3 == NULL)
	break;
      in = *in_p;
      while ((ch = *s3++) != '\0' && ch == *in++)
	;

      /* If we're at the end of s3, then the final in++ didn't occur.  */
      if (ch == '\0' && ((ch = *in) == '\0' || ch == '/'))
	{
	  out = copy_or_null (out, __sdir[j].riscos_name, len);

	  if (!out)
	    return sdirseg_buf_to_small;

	  *in_p = in;
	  *out_p = out;
	  return sdirseg_match;
	}
    }
  return sdirseg_no_match;
}

const char *
__sfixfind (const char *sfix)
{
  int where = sfix_hash (sfix);
  struct sfix *entry = __sfix[where];

  while (entry)
    {
      if (strcmp (entry->suffix, sfix) == 0)
        return entry->suffix;

      /* Modifiying this struct would allow, e.g., mapping .C to cc.  */
      entry = entry->next;
    }

  /* Failed.  */
  return NULL;
}

/* stpncpy() but only writes one '\0'. Return NULL if buffer overflowed.  */
static char *
copy_or_null (char *to, const char *from, size_t len)
{
  while (len--)
    if (!(*to++ = *from++))
      return to - 1;

  return NULL;
}

/* Call __riscosify with __riscosfy_control as the flags.
   Place adjacent to __riscosify to help with cache hits.  */
#undef __riscosify_std
char *
__riscosify_std (const char *name, int create_dir,
		char *buffer, size_t buf_len, int *filetype)
{
  return __riscosify (name, create_dir, __get_riscosify_control (), buffer,
  		      buf_len, filetype);
}

enum can_csd
{
  no,
  doubtful,
  yes,
  yes_root
};

/* Convert Unix filenames/pathnames in NAME to RISC OS format creating the
   final directory if necessary and CREATE_DIR is non-zero.
   Pass RISC OS pathnames through unchanged.
   Returns pointer to terminating '\0' in buffer,
   or NULL if there was a problem.  */
char *
__riscosify (const char *name, int create_dir,
	     int flags, char *buffer, size_t buf_len, int *filetype)
{
  int absolute_path;
  char current, next;
  const char *buf_end = buffer + buf_len; /* One char beyond the end.  */
  char *out = buffer;
  const char *in0;
  enum can_csd can_csd_etc = no;

  /* Check for silly user input and return errors appropriately.  */
  if ((flags & ~__RISCOSIFY_MASK))
    {
      /* Don't pull in assert function.  */
      fprintf (stderr, "\n__riscosify: assertion failed: ((flags & ~__RISCOSIFY_MASK) == 0)\n");
      fflush (stderr);
      abort ();
    }

  if (! name || ! buffer || ! buf_len)
    return NULL;

  if (filetype != NULL)
    *filetype = __RISCOSIFY_FILETYPE_NOTFOUND;

  /* We can safely make the assumption that here is space for at least
     one character in the buffer.  */

  /* If the user has specified this flag, then we only need to copy
     the filename to the destination buffer.  */
  if ((flags & __RISCOSIFY_NO_PROCESS) || *name == '\0')
    return copy_or_null (buffer, name, buf_len);

  /* Unix absolute paths have a leading slash e.g.
	/work/!unixlib37/source/common/riscosify.c.

     Any other path type is a relative path e.g.
	../fred/jim/harry.c
	./jim/harry.c
	jim/harry.c  */
  absolute_path = (name[0] == '/');

  /* Strip off all leading '/'  */
  while ((current = *name++) == '/')
    ;

  /* We must be careful when converting Unix filenames into RISC OS.
     The following filename formats must be accepted and translated
     appropriately:

	  Unix format			RISC OS format
       1. /idefs::hd.$/fred/preset.s	idefs::hd.$.fred.s.preset
       2. c/jimmyhill			c.jimmyhill
       3. gcc:/getopt.c			gcc:getopt.c
       4. /lib/something/cc		%.something.cc
       5. ./././c.cool			c.cool
       6. ../../../c.cool		^.^.^.c.cool
       7. ../cool.c			^.c.cool
       8. <GCC$dir>.cc.smart		<GCC$dir>.cc.smart
       9. <GCC$dir>.smart.cc		<GCC$Dir>.cc.smart
      10. /arm/rname.c			$.arm.c.rname

    It is quite acceptable for the user to pass filenames of this
    format on the command line (or within scripts).  */

  in0 = --name;

  /* Path names and disc names.

     Filing system names are alphanumeric or '_'.  Experimentation suggests
     path names may only be alphanumeric or '_'.

     Note, we do not attempt to handle -fs- type pathnames, since the
     use of them is more or less deprecated (see PRM 2-11).  */
  if (isalnum (current) || current == '_')
    {
      /* Move through the filename, looking for the next character that
	 isn't alphanumeric or an underscore.  Situations where this
	 might occur are:
	   1. idefs::		(disc absolute path)
	   2. nfs#station::	(network absolute path)
	   3. gcc:		(path variable).  */
      while (isalnum (next = *++in0) || next == '_')
	;

      /* Skip special fields in the pathname (i.e. the nfs#station::).
	 These start with '#' and can contain almost anything except ':'.  */
      if (next == '#')
	while ((next = *++in0) != '\0' && next != ':')
	  ;

      if (next == ':')
	{
#ifdef DEBUG
	  __os_print ("Found a `idefs:'\r\n\t\tbefore name = `");
	  __os_print (name); __os_print ("'\r\n\t\tafter name = `");
	  __os_print (in0); __os_print ("'\r\n");
#endif
	  /* Determine whether we are looking at a file system absolute
	     path, or a pseudo-filing system generated from a path
	     variable.  */
	  can_csd_etc = yes;
	  /* Probably just found a filing system or path - if below we decide
	     that we need to check for a full RISC OS pathname, then at this
	     point we know that
	     a: it's looking promising.
	     b: all possibilities (ie '$', '@' etc) are valid.  */

	  if (in0[1] == ':')
	    {
	      in0 += 2;	/* Pass over "::" and copy it to the output buffer.  */

#ifdef DEBUG
	      __os_print ("Found a `idefs::'\r\n\t\tbefore name = `");
	      __os_print (name); __os_print ("'\r\n\t\tafter name = `");
	      __os_print (in0); __os_print ("'\r\n");
#endif
	      if ((out + (in0 - name)) > buf_end)
		return NULL;
	      memcpy (out, name, in0 - name);
	      out += in0 - name;
	      name = in0;

	      /* Move through the disc name, looking for the terminating
		 '.'.  If the pathname started with a slash then
		 look out for a terminating slash e.g.
		    1  "/idefs::hd/work" - starts with a '/' - so must end with
		       '/'. In this case translate '.' in "discname" to '/'

		    2a "idefs::hd.$/work" - discname is "hd" (no problem)
		    2b "idefs::hd.$.work" - discname is "hd" (no problem)

		    3a "idefs::hd.work" - disname cannot be "hd" as '.' is not
		       followed by '$' - keep going to '/'
		    3b "idefs::hd/work" - only ambiguous case - will it become:
			     ":hd/work.$.full.path"
			     ":hd/work/!UnixLib37/Changelog" ?
		       cannot know - must guess. Current is latter option.  */


	      /* ".$" is always recognised as the end of the discname.  */
	      can_csd_etc = absolute_path ? no		/* Case 1.  */
					  : doubtful;	/* Assume case 3, but
		try to figure out whether we have case 2 or 3.  */
	      while ((next = *in0) && next != '/')
		{
		  if (next == '.')
		    {
		      if (in0[1] == '$')
			{
			  if (can_csd_etc == doubtful)
			    can_csd_etc = yes_root;
			    /* Have not passed a previous '.' before meeting
			       "$.", so is possible full pathname. Whether it
			       *is* actually followed by '.' will be determined
			       below.  */
			  break;
			}
		      else
			can_csd_etc = no;
			/* Definately not a native full pathname. Case 3.  */
		    }
		  in0++;
		}

	      if (can_csd_etc != yes_root)
		{
		  /* Case 1 or 3.
		     Map '.' to '/' in discname as we go.
		     Filing systems are [A-Za-z0-9_] which are not translated
		     by the table.  */
#ifdef DEBUG
		  __os_print ("Found a `idefs::hd'\r\n\t\tbefore name = `");
		  __os_print (name); __os_print ("'\r\n\t\tafter name = `");
		  __os_print (in0); __os_print ("'\r\n");
#endif
		  if ((out + (in0 - name)) >= buf_end)
		    return NULL;
		  do
		    {
		      *out++ = __filename_char_map[(unsigned char) *name++];
		    } while (name < in0);

		  /* At this point: *name == / . or \0.  */
		  if (out >= buf_end)
		    return NULL;
		  if (*name == '\0')
		    *out++ = '\0';
		  else
		    {
		      /* Skip over terminating '/' or '.'
		         (will only be '.' as part of ".$" found
		          after a '.').  */
		      name++;
		      /* There's a useful side effect here for when we
			 are looking out for the various RISC OS pathname
			 symbols ($, % etc) since we can automatically
			 assume that no characters are going to appear
			 before these symbols. e.g.
			  idefs::hd.work.$/work/!unixlib37
			  idefs::hd/$/work/!unixlib37
			 name will point to '$'.  */
		      *out++ = '.';	/* Terminate discname.  */
		    }
		}
	      else
		{
		  /* Case 2 - Looks like a full pathname starting
		     "idefs::hd.$.work"

		     "idefs::hd.@" is invalid
		     "/idefs::hd.$/work" would be treated as a discname "hd.$"
		     (case 1) and won't get here.  */
		  in0++;
		  /* Include the '.' in what we copy. (eg the one between
		     "hd" and "work".  */

		  /* Test that there is enough space to hold this filename
		     segment in the output buffer.  */
		  if ((out + (in0 - name)) > buf_end)
		    return NULL;

#ifdef DEBUG
		  __os_print ("Found a `idefs::hd.'\r\n\t\tbefore name = `");
		  __os_print (name); __os_print ("'\r\n\t\tafter name = `");
		  __os_print (in0); __os_print ("'\r\n");
#endif
		  /* Copy that segment of the filename into the output
		     RISC OS filename.  */
		  memcpy (out, name, in0 - name);
		  out += in0 - name;
		  name += in0 - name;
		}
#ifdef DEBUG
	      *out = '\0';
	      __os_print ("Output buffer is `"); __os_print (buffer);
	      __os_print ("'\r\nInput buffer is `"); __os_print (name);
	      __os_print ("'\r\n");
#endif
	    }
	  else
	    {
	      /* We could have a filename of one of the following forms:
		   1. gcc:o.libgcc
		   2. gcc:/o.libgcc
		   3. gcc:/o.libgcc
		   4. gcc:./o.libgcc
		   5. gcc:../o.libgcc
		   6. gcc:^.o.libgcc

		 It's quite feasible to precede each of those filenames with
		 a '/'.  We know if this has happened because absolute_path
		 will be set.

		 All we care to do here is place the 'gcc:' part of the
		 filename into the RISC OS translated version.  We'll
		 also make life easier by removing the '/' if it follows
		 the colon (cases 2 and 3).  */

	      /* in0 currently points to the ':'.  Increment by one so
		 the whole 'gcc:' is correctly copied across.  */
	      in0++;

	      /* Test that there is enough space to hold this filename
		 segment in the output buffer.  */
	      if ((out + (in0 - name)) > buf_end)
		return NULL;

#ifdef DEBUG
	      __os_print ("Found a `gcc:'\r\n\t\tbefore name = `");
	      __os_print (name); __os_print ("'\r\n\t\tafter name = `");
	      __os_print (name + (in0 - name)); __os_print ("'\r\n");
#endif

	      /* Copy the 'gcc:' segment of the filename into the output
		 RISC OS filename.  */
	      memcpy (out, name, in0 - name);
	      out += in0 - name;
	      name += in0 - name;

	      /* Skip the slash, since it is useless after a colon. It also
		 means we can rule out a RISC OS pathname.  */
	      if (*name == '/')
		{
		  name++;
		  can_csd_etc = no;
		}
	    }
	}
      else
	{
	  /* If we've reached here, the during the filename scan (above)
	     we have come across a slash or a dot.  Since colons and hashes
	     are usually part of RISC OS absolute names we can assume that
	     the filename never contained something like idefs::hd.  Therefore
	     no action need be taken.  */
	}
    }

  /* Check for a filename that looks like:
       <GCC$Dir>.cc.smart.

     Essentially we blindly copy all characters between the two chevrons.
     The filename could look like
       <GCC$Dir>/cc.smart
       <GCC$Dir>.cc.smart
       <GCC$Dir>.config

     ok to terminate on the '>' for the first two, as "/cc.smart" will be
     trapped by the check for suffixes as directories and ".cc.smart fits the
     rule that there is more than one "." and no "/" so it is assumed to be
     a RISC OS filename. However "<GCC$Dir>.config" would be translated as
     "<GCC$Dir>/config", which is very very unlikely to be what was intented.
     Hence copy a "." if ">." is found. ".cc.smart" will become "cc.smart",
     which the check routine later will interpret identically with
     ".cc.smart" (remeber that we've copied across that first ".").  */

  if (current == '<')
    {
      /* Skip characters up until the terminating '>'.  */
      while ((next = *++in0) && next != '>')
	;

      if (next)
        in0++;	/* Make sure that we copy the trailing '>' here.  */

      /* Test that there is enough space to hold this filename
	 segment in the output buffer.  */
      if ((out + (in0 - name)) > buf_end)
	return NULL;

#ifdef DEBUG
      __os_print ("Found a `<'. before name = `");
      __os_print (name); __os_print ("', after name = `");
      __os_print (name + (in0 - name)); __os_print ("'\r\n");
#endif
      /* Copy the '<GCC$Dir>' segment of the filename into the output
	 RISC OS filename.  */
      memcpy (out, name, in0 - name);
      out += in0 - name;
      name += in0 - name;

      /* If '>' is follwed by '.' or '/' add a trailing '.' to output name.
         next still contains the '>' or '\0' that terminated the while()
         loop.  */
      if (next && (*in0 == '.' || *in0 == '/'))
        {
          if (out == buf_end)
            return NULL;
          *out++ = '.';
#ifdef DEBUG
          __os_print ("Appended trailing `"); __os_vdu (*in0);
          __os_print ("' as .\r\n");
#endif
          name++;
        }
    }

  /* By the time we've reached here we should have filtered out some
     very RISC OS specific features i.e.
	1. idefs::hd.
	2. net#station::
	3. gcc:
	4. <gcc$dir>

     thus ending up with a filename that looks something like:
	1. fred/preset.s
	2. c/jimmyhill
	3. getopt.c
	4. /lib/something/cc
	5. ./././c.cool
	6. ../../../c.cool
	7. ../cool.c
	8. cc.smart
	9. smart.cc
       10. /arm/rname.c
       11. $.work.!unixlib37
       12. %.cc1

     Experimentation reveals that a bare FS/path name can be followed by
     any one of '$', '@', '%', '\' and '&' whereas a discname may only be
     followed by '$'.  We can use this to sift out native filenames.  */

  if ((current = *name) && name[1] == '.')
    {
      /* something followed by '.' -  */
      if ((can_csd_etc == yes_root && current == '$')
	  || (can_csd_etc == yes_root
	      && (current == '$' || current == '@' || current == '%'
		  || current == '\\' || current == '&')))
	{
	  /* "$." or "@." or "%." or "\\." or "&." means native RISC OS.
	     Copy it verbatim.  */
	  return copy_or_null (out, name, buf_end - out);
	}
    }

  if (current == '$' || current == '@' || current == '%'
      || current == '\\' || current == '&')
    {
      *out++ = current;
      *out++ = '.';
      current = *++name;
      /* If input filename was something like @.c, then ensure we
         consume the `.'.  */
      if (current == '.')
        current = *++name;
    }

  next = *(name + 1);

#ifdef DEBUG
  *out = '\0';
  __os_print ("Special path check: output buff is `"); __os_print (buffer);
  __os_print ("', input buff is `"); __os_print (name); __os_print ("'\r\n");
#endif
  if (absolute_path)
    {
      /* It's an absolute path. Check for special paths, starting with /dev  */
      if (current == 'd' && name[1] == 'e' && name[2] == 'v' && name[3] == '/'
	  && name[4] != '\0')
	{
	  /* Copy to the destination string as {device}:  e.g. /dev/tty
	     would become tty:.  */

	  out = copy_or_null (out, name + 4, buf_end - out - 2);
	  if (out == NULL)
	    return NULL;

	  *out++ = ':';
	  *out = '\0';
	  return out;
	}

      /* /usr/xxx and /var/xxx. Try matching xxx segment.  */
      if (((current == 'u' && name[1] == 's')
	   || (current == 'v' && name[1] == 'a'))
	  && name[2] == 'r' && name[3] == '/')
	{
	  name += 4;
	  switch (sdirseg (&name, &out, buf_end - out))
	    {
	    case sdirseg_buf_to_small:
	      return NULL;

	    case sdirseg_match:
	      goto main;

	    default:
	      name -= 4;
	    }
	}

      /* Try matching against a user defined /xxx.  */
      switch (sdirseg (&name, &out, buf_end - out))
	{
	case sdirseg_buf_to_small:
	  return NULL;

	case sdirseg_match:
	  goto main;

	case sdirseg_no_match:
	  if (out == buffer)
	    {
	      if (buf_end - out < 2)
		return NULL;
	      /* Nothing written yet, "absolute" path.  */
	      *out++ = '$';
	      *out++ = '.';
	    }
	}
    }
  else
    {
      /* Relative path.
	 At this point out == buffer since anything else has jumped to main.

	 If name starts with a dot and the next character is either
	 non existant or a '/' then we must be specifing the currently
	 selected directory.  Everything else is dealt with below.  */
      if (current == '.' && ((next = name[1]) == '/' || next == '\0'))
	{
	  if (buf_end - out < 2)
	    return NULL;
	  *out++ = '@';
	  *out++ = '.';
	  name++;
	}
    }


#ifdef DEBUG
  __os_print ("Long route - \r\n");
#endif

main:

#ifdef DEBUG
  *out = '\0';
  __os_print ("entering main loop: output buff is `"); __os_print (buffer);
  __os_print ("', input buff is `"); __os_print (name); __os_print ("'\r\n");
#endif
  /* At this point the output buffer
     -1: Can't be empty if the input is exhausted
      0: May be full!
      1: Contains either
	 $. @. %. ^. \. &.
	 Path:
	 FS::Disc.
	 Some::Full.$.Pathspec.for.a.special.dir.
	 Nothing (the input didn't start '/' or with a recognised path: name)

      2: probably isn't '\0' terminated.

     out points to the next location in the buffer to write
     (if the buffer is full this will be the byte immediately after)

     The input may be exhausted.  */

  /* At this point, we should be left with an input filename that looks
     like:
	1. work.!unixlib37.test.c.riscosify
	2. work/!unixlib37/test/riscosify.c
	3. bin/as

     We have to make some important assumptions here:
	1. A filename containing more than one dot and no slashes is
	   a RISC OS filename.
	2. A filename containing more than one slash and only one dot is a
	   Unix filename.

     This assumption falls down on filenames like:
	gcc-2.7.2.2.tar.gz
  */

  for (;;)
    {
      const char *last_dot, *last_comma;
      const char *suffix;
      size_t to_copy;
      int drop_vowels;

      current = *name++;

      if (current == '/')
	continue;

      if (current == '\0')
	{
	   /* If the input is exhausted blat the last character
	      (which will be a dot) and exit.  */

	  if (out[-1] == '.')
	    --out;
	  else if (out == buf_end)
	    return NULL;
	  *out = '\0';

	  /* Return for converted Unix file/path.  */
	  return out;
	}

      next = *name;

      if (current == '.')
	{
#ifdef DEBUG
       	  __os_print ("Dot filenames `"); __os_vdu (current);
       	  __os_print ("' `"); __os_vdu (next); __os_print ("' in `");
       	  __os_print (name); __os_print ("'\r\n");
#endif

	  switch (next)
	    {
	    case '/':	  /* Just consume ./ */
	      name++;	  /* and fall through.  */
	    case '\0':
	      continue;

	    case '.':
	      /* Convert ../ to the RISC OS up directory '^.'. */
	      switch (name[1])
		{
		case '/':
		  name++;  /* and fall through.  */
		case '\0':
		  if (buf_end - out < 2)
		    return NULL;
		  *out++ = '^';
		  *out++ = '.';
		  name++;
		  continue;
		}
	    }
	}

      /* OK. "Normal" filename
	 current == name[-1]
	 next == *name  */
      last_dot = NULL;
      last_comma = NULL;
      drop_vowels = 0;
      in0 = name;

      /* Locate the last '.' and `,' in what is left of the name.  */
      while (next != '\0' && next != '/')
	{
	  if (next == '.')
	    last_dot = in0;
	  else if (next == ',')
	    last_comma = in0;
	  next = *++in0;
	}

      /* Use MimeMap to find a filetype to match the filename
         extension.  e.g.  file.html -> 0xfaf */
      if ((flags & __RISCOSIFY_FILETYPE_SET) &&
          filetype != NULL && last_dot != NULL)
        {
           _kernel_swi_regs regs;

          regs.r[0] = MMM_TYPE_DOT_EXTN; /* Input extension */
          regs.r[1] = (int)last_dot;
          regs.r[2] = MMM_TYPE_RISCOS; /* Output filetype */

          if (_kernel_swi(MimeMap_Translate, &regs, &regs)) {
            /* Default to text if there's an error */
            *filetype = 0xFFF;
          } else {
            *filetype = regs.r[3];
          }
        }

      /* Check if we have "blabla,xyz" as filename where `xyz' is a
         valid 12 bit hex number.  Only perform the check if
         __RISCOSIFY_FILETYPE_EXT is set and xyz != 0xfff.  However
         relax the latter condition if __RISCOSIFY_FILETYPE_FFF_EXT is
         set.  */
      if (last_comma && next == '\0' && (in0 - last_comma) == 4
          && isxdigit (last_comma[1])
          && isxdigit (last_comma[2])
          && isxdigit (last_comma[3])
          && (flags & __RISCOSIFY_FILETYPE_EXT))
        {
          int ftype = 0, cnt;

          for (cnt = 1; cnt < 4; cnt ++)
            ftype = (ftype << 4) + ((last_comma[cnt] <= '9')
              	       	     	    ? (last_comma[cnt] - '0')
              	       	     	    : (toupper (last_comma[cnt])
              	       	     	       - 'A' + 10));

	  if (filetype != NULL
	      && (ftype != 0xFFF || (flags & __RISCOSIFY_FILETYPE_FFF_EXT)))
	    *filetype = ftype;
        }
      else
        last_comma = NULL;

      if (last_dot && !(flags & __RISCOSIFY_NO_SUFFIX))
        {
          /* Handle case "/rafs::testcvs/$/myproject/checkout.o,ffd"  */
          if (last_comma)
            *((char *) last_comma) = '\0';
          suffix = __sfixfind (last_dot + 1);
          if (last_comma)
            *((char *) last_comma) = ',';
        }
      else
	suffix = NULL;

      if (suffix)
	{
	  out = copy_or_null (out, suffix, buf_end - out - 1);
	  if (out == NULL)
	    return NULL;

	  if (create_dir)
	    {
	      int regs[6];

	      /* Create the directory if it doesn't exist.  */
	      if (! __os_file (OSFILE_READCATINFO_NOPATH, buffer, regs)
	          && ! regs[0])
		{
		  regs[4] = 0;   /* Default number of entries in dir.  */
		  __os_file (8, buffer, regs);
		}
	    }

	  *out++ = '.';
	}
      else
	{
	  /* At this point in __uname() there is code to check whether
	     the name to last_dot exists as a directory.
	     This means that
	         !Perl/lib/File.pm
	     maps to either
		 !Perl.lib.File.pm
	     or
		 !Perl.lib.File/pm
	     depending on whether the directory !Perl.lib.File exists.
	     [From the choice of filenames above guess who is writing this]

	     IMHO this is naïve:
	     0: The directory doesn't exist
	     1: Write the file out as !Perl/lib/File.pm (!Perl.lib.File/pm)
	     2: Create the directory to house more files
	     3: Try to read back !Perl/lib/File.pm and it's "not there"

	     d'oh.

	     However, its removal may have unexpected results in some
	     programs (its inclusion has unexpected results in some), hence
	     it is switchable).  */

	  if (last_dot && !(flags & __RISCOSIFY_DONT_CHECK_DIR))
	    {
	      size_t length = last_dot - --name;
	      /* name now points to first char of interest.
		 length includes that training '.' (or room for '\0').  */
	      if (out + length >= buf_end)
		return NULL;

	      /* Copy everything up to that last '.' to the output buffer.  */
	      memcpy (out, name++, length);
	      /* Restore name.  */
	      out[length] = '\0';

	      if (!(flags & __RISCOSIFY_CHECK_DIR_IS_SUFFIX)
		  || __sfixfind (out))
		{
		  /* Perform check if:
		     either __RISCOSIFY_CHECK_DIR_IS_SUFFIX is unset
		     or the name is in the suffix list.  */

		  if (__isdir_raw (buffer))
		    {
		      out += length;
		      name += length;
		      *out++ = '.';
		      /* __uname at this point copied the last section
			 verbatim into the output buffer.
			 This seems a trifle dodgy - all that is know is that
			 it contians none of '.' '/' '\0'
			 By looping from this point we let the next pass deal
			 with any special characters that may be found.  */
		      continue;
		    }
		}
	    }
	  /* Didn't meet the criteria for checking, or directory not
	     found. */
	  last_dot = (last_comma != NULL) ? last_comma : in0;
	}

      /* last_dot is the first character *not* to copy
	 want name-1 to last_dot-1.  */

      to_copy = truncate[__RISCOSIFY_TRUNCATE_VALUE(flags)];

      name--; /* Copying from name - 1.  */
      if (to_copy < (size_t) (last_dot - name))
	drop_vowels = flags & __RISCOSIFY_DROP_VOWEL;
      else
	to_copy = last_dot - name;

      if (to_copy < (size_t)(buf_end - out) && !drop_vowels)
	{
	  char *dst = out;
	  out += to_copy;
	  /* Plenty of space, fast loop, copying at least one char.  */
	  do
	    {
	      to_copy--;
	      dst[to_copy] = __filename_char_map[(unsigned char) name[to_copy]];
	    }
	  while (to_copy);
	}
      else
	{
	  /* May run out of space, may be dropping vowels.
	     May be extremely lucky - dropping vowels may not overrun
	     buffer, whereas simple truncate would.  We do not go for
	     a simple truncate because calls with different size buffers
	     would yield different results and that would be very
	     confusing.  */
	  do
	    {
	      /* We know that there is at least one valid character.  */
	      if (drop_vowels
		  && (current == 'a' || current == 'e' || current == 'i'
		      || current == 'o' || current == 'u'))
		continue;

	      if (out == buf_end)  /* No room.  */
		return NULL;

	      to_copy--;
	      *out++ = __filename_char_map[(unsigned char) current];
	    }
	  while (current = *++name, ((name < last_dot) && to_copy));
	}

      if (out == buf_end)
	return NULL;

      name = (last_comma != NULL) ? (last_comma + 4) : in0;
      *out++ = '.';
      /* At this point "My brain hurts".  */
    }		/* Loop again.  */
  /* Never reached */
}

static const size_t truncate[4] = {INT_MAX, 55, 19, 10};

/* This table has
   '/' and '.' transposed
   '?' and '#' transposed
   0x01 - ' ', '"', '$', '%', '&', ':', '@', '\\', '^', '_', '|' and 0x7F
   all mapping to '_'.

   If you are going to be a stickler for following the PRM or you use L format
   or earlier then 0x80 - 0xFF should also be mapped to '_'.

   This table is also used to 'go the other way' in dirent and getcwd.  */

const char __filename_char_map[256] =
{
  '\0',  '_',  '_',  '_',  '_',  '_',  '_',  '_',
   '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',
   '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',
   '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',
   '_',  '!',  '_',  '?',  '_',  '_',  '_', '\'',
   '(',  ')',  '*',  '+',  ',',  '-',  '/',  '.',
   '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
   '8',  '9',  '_',  ';',  '<',  '=',  '>',  '#',
   '_',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
   'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
   'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
   'X',  'Y',  'Z',  '[',  '_',  ']',  '_',  '_',
   '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
   'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
   'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
   'x',  'y',  'z',  '{',  '_',  '}',  '~',  '_',

  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
   ' ',  '¡',  '¢',  '£',  '¤',  '¥',  '¦',  '§',
   '¨',  '©',  'ª',  '«',  '¬',  '­',  '®',  '¯',
   '°',  '±',  '²',  '³',  '´',  'µ',  '¶',  '·',
   '¸',  '¹',  'º',  '»',  '¼',  '½',  '¾',  '¿',
   'À',  'Á',  'Â',  'Ã',  'Ä',  'Å',  'Æ',  'Ç',
   'È',  'É',  'Ê',  'Ë',  'Ì',  'Í',  'Î',  'Ï',
   'Ð',  'Ñ',  'Ò',  'Ó',  'Ô',  'Õ',  'Ö',  '×',
   'Ø',  'Ù',  'Ú',  'Û',  'Ü',  'Ý',  'Þ',  'ß',
   'à',  'á',  'â',  'ã',  'ä',  'å',  'æ',  'ç',
   'è',  'é',  'ê',  'ë',  'ì',  'í',  'î',  'ï',
   'ð',  'ñ',  'ò',  'ó',  'ô',  'õ',  'ö',  '÷',
   'ø',  'ù',  'ú',  'û',  'ü',  'ý',  'þ',  'ÿ'
};


