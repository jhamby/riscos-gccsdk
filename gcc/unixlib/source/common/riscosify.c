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

static const char wimp_scrap[] = "<Wimp$ScrapDir>";

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

static char *copy_or_null (char *to, const char *from, const char *buf_end);

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

      str = buf + regs[2];
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
    if (def_map[j].defined == 0)
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
sdirseg (const char **in_p, char **out_p, const char *buf_end)
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
	  out = copy_or_null (out, __sdir[j].riscos_name, buf_end);

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

/* Copy 'from' to 'to'. Return NULL if buffer overflowed. */
static char *
copy_or_null (char *to, const char *from, const char *buf_end)
{
  while (to <= buf_end)
    if (!(*to++ = *from++))
      return to - 1;

  return NULL;
}

/* Copy from in to out, translating characters as necessary.
   Also handles ,xyz extensions and suffix swapping.
   Return NULL if buffer overflowed. */
static char *
translate_or_null (int create_dir, int flags,
                   char *buffer, const char *buf_end, int *filetype,
                   char *out, const char *in, int path)
{
  /* in points to a (possibly partial) unix pathname.
     If it was absolute, it will have had any leading '/'s stripped, and any
     RISC OS specific parts will already have been copied */

  const char *start = in;      /* The start of our input */
  const char *last_slash = in; /* The next character after the last '/'
                                  discovered in the input */
  const char *previous_slash = NULL; /* The previous value of last_slash */
  char *last_out_slash = out;  /* The position in the output that corresponds
                                  to last_slash in the input */
  const char *last_dot = NULL; /* The last '.' found in the input */
  char *last_out_dot = NULL;   /* The position in the output that corresponds
                                  to the last '.' found in the input */
  const char *suffix;

#ifdef DEBUG
  __os_print ("-- translate_or_null:\r\n");
  __os_print ("output buffer = '");
  *out = '\0';
  __os_print (buffer);
  __os_print ("'\r\nremaining input = '");
  __os_print (in);
  __os_print ("'\r\n");
#endif

  if (out > buf_end)
    return NULL;

  while (*in)
    {
      switch (*in)
        {
        case '.':
          if (in[1] == '/')
            {
              /* Skip any ./
                 If it is at the beginning the output a @. */

              if (in == start && !path)
               {
                 if (out + 2 > buf_end)
                   return NULL;

                 *out++ = '@';
                 *out++ = '.';

                 last_slash = in + 1;
                 last_out_slash = out;

               }

              in += 2;
              last_slash = in;
            }
          else if (in > start && in[-1] == '/' && in[1] == '\0')
            {
              /* Skip any /. at the end of a filename
                 The check against start ensures we don't read earlier
                 than the start of the input buffer */
              in++;
            }
          else if (in[1] == '.' && in[2] == '/')
            {
              /* Change ../ to ^. */
              in += 3;
              last_slash = in;

              if (out + 2 > buf_end)
                return NULL;

              *out++ = '^';
              *out++ = '.';
              last_out_slash = out;
            }
          else if (in > start && in[-1] == '/' && in[1] == '.' && in[2] == '\0')
            {
              /* Change .. to ^ when at the end of a filename
                 The check against start ensures we don't read earlier
                 than the start of the input buffer */

              in += 2;

              if (previous_slash == start)
                {
                  /* Make a special case for foo/.. as RISC OS will canonicalise
                     this to an empty string, and then complain about the lack
                     of filename */

                  out = buffer;
                  *out++ = '@';
                  *out = '\0';

                }
              else
                {

                  if (out + 1 > buf_end)
                    return NULL;

                  *out++ = '^';
                }
            }
          else
            {
              /* Just a . as part of a filename */
              last_dot = in++;
              last_out_dot = out;
              *out++ = '/';

              if (out > buf_end)
                return NULL;
            }

          break;

        case '/':
          /* Strip out any multiple slashes */
          while (in[1] == '/')
            in++;

          previous_slash = last_slash;

          last_slash = ++in;

          /* Copy as a '.', unless nothing else has been output yet or there
             is already a . */
          if (out > buffer && out[-1] != '.')
            *out++ = '.';

          last_out_slash = out;

          if (out > buf_end)
            return NULL;

          break;

        default:
          /* Translate all other characters as appropriate */
          *out++ = __filename_char_map[(unsigned char) *in++];

          if (out > buf_end)
            return NULL;

        }
    }

  /* If the input ended in any / then remove them. */
  while (out > buffer + 1 && out[-1] == '.')
    out--;

  if (out == buffer && !path)
    {
      /* Translate an empty filename into the current dir */
      if (out +1 > buf_end)
        return NULL;

      *out++ = '@';
    }

#ifdef DEBUG
  *out = '\0';
  __os_print ("before ,xyz and suffix checking '");
  __os_print (buffer);
  __os_print ("'\r\n");
#endif

  /* Use MimeMap to find a filetype to match the filename
     extension.  e.g.  file.html -> 0xfaf */
  if ((flags & __RISCOSIFY_FILETYPE_SET) &&
      filetype != NULL && last_dot != NULL)
    {
       _kernel_swi_regs regs;

      regs.r[0] = MMM_TYPE_DOT_EXTN; /* Input extension */
      regs.r[1] = (int)last_dot;
      regs.r[2] = MMM_TYPE_RISCOS; /* Output filetype */

      /* If there's an error, then the filetype will remain
         __RISCOSIFY_FILETYPE_NOT_FOUND.  */
      if (! _kernel_swi (MimeMap_Translate, &regs, &regs))
        *filetype = regs.r[3];
    }

  /* Check if we have "blabla,xyz" as filename where `xyz' is a
     valid 12 bit hex number.  Only perform the check if
     __RISCOSIFY_FILETYPE_EXT is set and xyz != 0xfff.  However
     relax the latter condition if __RISCOSIFY_FILETYPE_FFF_EXT is
     set.  */
  if (out - last_out_slash > 4 && out[-4] == ','
      && isxdigit (out[-3])
      && isxdigit (out[-2])
      && isxdigit (out[-1])
      && (flags & __RISCOSIFY_FILETYPE_EXT))
    {
      int ftype = 0;
      int i;

      for (i = -3; i < 0; i++)
        ftype = (ftype << 4) + ((out[i] <= '9')
                                ? (out[i] - '0')
                                : (toupper (out[i]) - 'A' + 10));

      if (filetype != NULL
          && (ftype != 0xFFF || (flags & __RISCOSIFY_FILETYPE_FFF_EXT)))
        *filetype = ftype;

      /* Remove the comma and extension from the output */
      out -= 4;
    }

  /* Terminate the output */
  *out = '\0';

  /* See if the leafname suffix matches a known suffix */
  if (last_out_dot && !(flags & __RISCOSIFY_NO_SUFFIX))
    suffix = __sfixfind (last_out_dot + 1);
  else
    suffix = NULL;

  if (suffix)
    {
      /* Suffix matches, so do suffix swapping */
      int leaf_len;
      int i;

      /* Rewind output buffer to just after the last directory separator copied.
         Could be the start of the buffer if no dir separators encountered */
      out = last_out_slash;

      /* Copy the new suffix */
      while (*suffix)
        {
          *out++ = *suffix++;

          if (out > buf_end)
            return NULL;
        }

      if (create_dir)
        {
          int regs[6];

          /* Terminate the output buffer */
          *out = '\0';

          /* Create the directory if it doesn't exist.  */
          if (! __os_file (OSFILE_READCATINFO_NOPATH, buffer, regs)
              && ! regs[0])
            {
              regs[4] = 0;   /* Default number of entries in dir.  */
              __os_file (8, buffer, regs);
            }
        }

      /* Copy the leafname without suffix */
      leaf_len = last_out_dot - last_out_slash;

      if (out + leaf_len > buf_end)
        return NULL;

      *out++ = '.';

      for (i = 0; i < leaf_len; i++)
        *out++ = last_slash[i];

    }

  /* Terminate the output */
  *out = '\0';

#ifdef DEBUG
  __os_print ("final output '");
  __os_print (buffer);
  __os_print ("'\r\n");
#endif

  return out;
}


/* Guess if filename is RISC OS or unix, then call copy_or_null or
   translate_or null as appropriate. Used for path vars and relative filenames
   where we cannot reliably work out what format it is in.
   If path is nonzero then buffer will already contain a path var, eg gcc:
   otherwise buffer will contain the chars upto the first dot or slash.
   Return NULL if buffer overflowed. */
static char *
guess_or_null (int create_dir, int flags, char *buffer, const char *buf_end,
               int *filetype, char *out, const char *in, const char *name,
               int path)
{
  char *last;
  char *penultimate;
  const char *orig_in = name;
  char *orig_out = buffer;
  char *slash = NULL;

#ifdef DEBUG
  __os_print ("-- guess_or_null:\r\n");
  __os_print ("output buffer = '");
  *out = '\0';
  __os_print (buffer);
  __os_print ("'\r\nremaining input = '");
  __os_print (in);
  __os_print ("'\r\n");
#endif

  /* Set orig_* to the bit after the pathvar, if any */
  if (path)
    {
      orig_out = out;
      orig_in = in;
    }

  /* Find the last two dots or slashes */
  last = NULL;
  penultimate = NULL;
  while (*in)
    {
      if (*in == '.')
        {
          penultimate = last;
          last = out;
        }
      else if (*in == '/')
        {
          penultimate = last;
          last = out;
          slash = out;
        }

      *out++ = *in++;

      if (out > buf_end)
        return NULL;
    }

  *out = '\0';

  if (last == NULL)
    {
      /* Just a leafname, so let translate_or_null deal with ,xyz etc. */
      return translate_or_null (create_dir, flags, buffer, buf_end,
                                filetype, orig_out, orig_in, path);
    }

  if (penultimate == NULL)
    penultimate = orig_out - 1;

  if (*last == '.')
    {
      *last = '\0';

      if (slash == NULL && __sfixfind (penultimate + 1))
        {
          /* foo.c.getopt so it's probably RISC OS, unless there was
             a slash earlier in the input */
          return copy_or_null (orig_out, orig_in, buf_end);
        }

      if (__sfixfind (last + 1))
        {
          /* foo/getopt.c so it's probably unix */
          return translate_or_null (create_dir, flags, buffer, buf_end,
                                    filetype, orig_out, orig_in, path);
        }
    }
  else
    {
      *last = '\0';

      if (__sfixfind (penultimate + 1))
        {
          /* foo/c/getopt so it's probably unix */
          return translate_or_null (create_dir, flags, buffer,
                                    buf_end, filetype, orig_out, orig_in, path);
        }

      if (__sfixfind (last + 1))
        {
          /* foo.getopt/c so it's probably RISC OS */
          return copy_or_null (orig_out, orig_in, buf_end);
        }
    }

  /* No known suffix found at the start or end.
     Could be RISC OS or unix, and there's no reliable way to tell.
     If it starts with a path var then assume it's RISC OS format, otherwise
     unix */
  if (path)
    return copy_or_null (orig_out, orig_in, buf_end);

  return translate_or_null (create_dir, flags, buffer, buf_end, filetype,
                            orig_out, orig_in, path);
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

char *
__riscosify (const char *name, int create_dir,
             int flags, char *buffer, size_t buf_len, int *filetype)
{
  const char *buf_end = buffer + buf_len - 1; /* The last char in the buffer. */
  const char *in = name;
  char *out = buffer;

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

#ifdef DEBUG
  __os_print ("-- __riscosify: '");
  __os_print (name);
  __os_print ("'\r\n");
#endif

  if (filetype != NULL)
    *filetype = __RISCOSIFY_FILETYPE_NOTFOUND;

  /* We can safely make the assumption that here is space for at least
     one character in the buffer.  */

  /* If the user has specified this flag, then we only need to copy
     the filename to the destination buffer.  */
  if (flags & __RISCOSIFY_NO_PROCESS)
    return copy_or_null (buffer, name, buf_end);

  /* We can tell quite a lot from the first character of the path */
  switch (*in)
    {
    case '/':
      /* The directory separator must be a slash. Could be an absolute unix
         pathname, or a unixified RISC OS path.

         /idefs::hd/$/fred/preset/s  idefs::hd.$.fred.preset.s
         /idefs::hd/$/fred/preset.s  idefs::hd.$.fred.s.preset
         /arm/rname.c                $.arm.c.rname
         /lib/something/cc           %.something.cc
         /<GCC$Dir>/config           <GCC$Dir>.config
         /gcc:/libgcc.o              gcc:o.libgcc
         /gcc:libgcc.o               gcc:o.libgcc
         /dev/tty                    tty:
         /usr/xxx                    xxx
         /xxx                        xxx */

      in++; /* Skip the '/' */

      if (*in == '<')
        {
          /* Must be a sysvar
             /<GCC$dir>/something */

          /* Copy everything upto the '>' */
          do
            {
              *out++ = *in++;

              if (out > buf_end)
                return NULL;
            }
          while (*in && *in != '>');

          if (out + 2 > buf_end)
            return NULL;

          if (*in)
            *out++ = *in++; /* Copy the '>' */

          if (*in == '/')
            {
              /* Copy the first '/' as a '.' */
              *out++ = '.';
              in++;
            }

          /* Skip any further '/'s */
          while (*in == '/')
            in++;

        }
      else
        {
          /* Not a sysvar */

          /* Copy everything upto the first '.', '/', ':' or '#' */
          while (*in && *in != ':' && *in != '.' && *in != '/' && *in != '#')
            {
              *out++ = *in++;

              if (out > buf_end)
                return NULL;
            }

          if (*in == '#')
            {
              /* Copy any fileswitch special field (i.e. the nfs#station::)
                 These start with '#' and can contain almost anything except ':' */
              while (*in && *in != ':')
                {
                  *out++ = *in++;

                  if (out > buf_end)
                    return NULL;
                }
            }

          if (*in == ':')
            {
              /* Must be a RISC OS filing system or path variable.
                 /idefs:: or /gcc: */

              *out++ = *in++; /* Copy the ':' */

              if (out > buf_end)
                return NULL;

              if (*in == ':')
                {
                  /* Must be a filing system
                     /idefs:: */

                  /* Copy upto the next '/'
                     /idefs::hd/ */
                  while (*in && *in != '/')
                    {
                      *out++ = *in++;

                      if (out > buf_end)
                        return NULL;
                    }

                  if (out + 3 > buf_end)
                    return NULL;

                  if (in[0] == '/' && in[1] == '$')
                    {
                      /* Copy the /$ as  .$ */
                      *out++ = '.';
                      *out++ = '$';
                      in += 2;
                    }

                  if (*in == '/')
                    {
                      /* Copy the / as . */
                      *out++ = '.';
                      in++;
                    }

                  /* Skip any further '/'s */
                  while (*in == '/')
                    in++;

                }
              else if (*in == '/')
                {
                  /* Must be a path var followed by a '/'
                     /gcc:/ */

                  /* Skip any '/'s */
                  do
                    {
                      in++;
                    }
                  while (*in == '/');

                }
              else
                {
                  /* Must be a path var
                     /gcc:something
                     in already points to the char after the ':', so nothing
                     else needs copying */
                }
            }
          else
            {
              /* Must be an absolute unix path, with no RISC OS specific
                 parts, so rewind to just after the initial slashes */
              int matched = 0;

              in = name;
              while (*in == '/')
                in++;

              out = buffer;

              /* Check for special paths, starting with /dev  */
              if (in[0] == 'd' && in[1] == 'e' && in[2] == 'v'
                  && in[3] == '/' && in[4] != '\0')
                {
                  /* Copy to the destination string as {device}:  e.g. /dev/tty
                     would become tty:.  */

                  out = copy_or_null (out, in + 4, buf_end);
                  if (out == NULL)
                    return NULL;

                  *out++ = ':';
                  *out = '\0';
                  return out;
                }

              /* /usr/xxx and /var/xxx. Try matching xxx segment.  */
              if (((  in[0] == 'u' && in[1] == 's')
                  || (in[0] == 'v' && in[1] == 'a'))
                  &&  in[2] == 'r' && in[3] == '/')
                {
                  in += 4;
                  switch (sdirseg (&in, &out, buf_end))
                    {
                    case sdirseg_buf_to_small:
                      return NULL;
                      break;

                    case sdirseg_match:
                      matched = 1;

                      /* If the matched segment was a path var then consume
                         any '/'s in the input to prevent them being copied
                         as '.'s (so we don't end up with gcc:.foo) */
                      if (out > buffer && out[-1] == ':')
                        while (*in == '/')
                          in++;

                      break;

                    default:
                      in -= 4;
                    }
                }

              if (! matched)
                {
                  /* Try matching against a user defined /xxx.  */
                  switch (sdirseg (&in, &out, buf_end))
                    {
                    case sdirseg_buf_to_small:
                      return NULL;
                      break;

                    case sdirseg_match:
                      matched = 1;

                      /* If the matched segment was a path var then consume
                         any '/'s in the input to prevent them being copied
                         as '.'s (so we don't end up with gcc:.foo) */
                      if (out > buffer && out[-1] == ':')
                        while (*in == '/')
                          in++;

                      break;

                    case sdirseg_no_match:
                      break;

                    }
                }

              if (! matched)
                {
                  /* Did not match any user defined paths, so treat it as a
                     path on the current filing system */
                  if (out + 2 > buf_end)
                    return NULL;

                  *out++ = '$';
                  *out++ = '.';
                }
            }
        }

      /* By this point, all RISC OS specific parts have been copied across and
         all that remains is something like foo/bar/baz.c */
      return translate_or_null (create_dir, flags, buffer, buf_end, filetype,
                                out, in, 1);

      break;


    case '$':
    case '@':
    case '%':
    case '\\':
    case '&':
    case '^':
      /* If the path starts with any of these chars then it must be a RISC OS
         path, although the dot and slashed could still be reversed.

         $.work.!unixlib  $.work.!unixlib
         $/work/!unixlib  $.work.!unixlib
         %.cc1            %.cc1
         @/fred.c         @.c.fred
         ^.fred           ^.fred */

      *out++ = *in++; /* We know there is at least one space in the buffer */

      /* The next char must be the directory separator */
      if (*in == '/')
        {
          if (out > buf_end)
            return NULL;

          /* Copy the / as . */
          *out++ = '.';
          in++;

          /* Skip any further '/'s */
          while (*in == '/')
            in++;

          return translate_or_null (create_dir, flags, buffer, buf_end,
                                    filetype, out, in, 1);
        }
      else
        {
          return copy_or_null (out, in, buf_end);
        }

      break;


    case '<':
      /* The path must start with a sysvar.
         <GCC$dir>.cc.smart    <GCC$dir>.cc.smart
         <GCC$dir>.smart.cc    <GCC$Dir>.smart.cc
         <GCC$Dir>/cc.smart    <GCC$Dir>.cc/smart
         <GCC$Dir>/smart.cc    <GCC$Dir>.cc.smart */

      /* Copy everything upto the '>' */
      do
        {
          *out++ = *in++;

          if (out > buf_end)
            return NULL;
        }
      while (*in && *in != '>');

      /* Copy the '>' */
      if (*in)
        *out++ = *in++;

      if (out > buf_end)
        return NULL;

      /* The char following the '>' must be the directory separator */
      if (*in == '/')
        {
          /* Copy the / as . */
          *out++ = '.';
          in++;

          /* Skip any further '/'s */
          while (*in == '/')
            in++;

          return translate_or_null (create_dir, flags, buffer, buf_end,
                                    filetype, out, in, 1);
        }
      else
        {
          return copy_or_null (out, in, buf_end);
        }

      break;


    case '.':
      /* RISC OS paths never start with a '.'
         ./././c.cool      @.c/cool
         ../../../c.cool   ^.^.^.c/cool
         ../cool.c         ^.c.cool
         .plan             /plan
         ..plan            //plan
         .                 @
         ..                ^
         ./.               @
         ./..              @.^ */

      if (in[1] == '\0')
        {
          if (out + 1 > buf_end)
            return NULL;

          *out++ = '@';
          *out = '\0';

          return out;
        }

      if (in[1] == '.' && in[2] == '\0')
        {
          if (out + 1 > buf_end)
            return NULL;

          *out++ = '^';
          *out = '\0';

          return out;
        }

      return translate_or_null (create_dir, flags, buffer, buf_end, filetype,
                                out, in, 0);

      break;


    default:
      /* Could be an absolute RISC OS path or a path variable, possibly with
         '.' and '/' swapped. Otherwise it must be a relative path, but could
         be either RISC OS or unix, with no easy way to tell.

         idefs::hd.$.fred/c  idefs::hd.$.fred/c
         idefs::hd/$/fred.c  idefs::hd.$.c.fred
         gcc:c.getopt        gcc:c.getopt
         gcc:/getopt.c       gcc:c.getopt
         fred.c              c.fred
         c/fred              c.fred
         c.fred              c.fred
         gcc:/getopt.c       gcc:getopt.c
         gcc:o.libgcc        gcc:o.libgcc
         gcc:/o.libgcc       gcc:o/libgcc
         foo/bar.s           foo.s.bar
         foo/bar.php         foo.bar/php
         gcc-2.7.2.2.tar.gz  gcc-2/7/2/2/tar/gz */

      /* Copy everything upto the first '.', '/', ':' or '#' */
      while (*in && *in != ':' && *in != '.' && *in != '/' && *in != '#')
        {
          *out++ = *in++;

          if (out > buf_end)
            return NULL;
        }

      if (*in == '#')
        {
          /* Copy any fileswitch special field (i.e. the nfs#station::)
             These start with '#' and can contain almost anything except ':' */
          while (*in && *in != ':')
            {
              *out++ = *in++;

              if (out > buf_end)
                return NULL;
            }
        }

      if (*in == ':')
        {
          /* Must be a RISC OS filing system or path var
             idefs:: or gcc: */

          *out++ = *in++; /* Copy the ':' */
          if (out > buf_end)
            return NULL;

          if (*in == ':')
            {
              /* Must be a filing system */
              while (*in && *in != '.' && *in != '/')
                {
                  *out++ = *in++; /* Copy the discname */

                  if (out > buf_end)
                    return NULL;
                }


              /* Copy the .$ or /$ as .$ */
              if (in[0] && in[1] == '$')
                {
                 /* Add the .$ */
                 if (out + 2 > buf_end)
                   return NULL;

                 *out++ = '.';
                 *out++ = '$';
                 in += 2;
                }

              if (*in == '/')
                {
                  /* Copy the / as . */
                  *out++ = '.';
                  in++;

                  /* Skip any further '/'s */
                  while (*in == '/')
                    in++;

                  return translate_or_null (create_dir, flags, buffer, buf_end,
                                            filetype, out, in, 1);
                }
              else
                {
                  return copy_or_null (out, in, buf_end);
                }
            }
          else
            {
              /* Must be a path var
                 gcc: */
              if (*in == '/')
                {
                  /* Skip over any '/'s */
                  while (*in == '/')
                    in++;

                  return translate_or_null (create_dir, flags, buffer, buf_end,
                                            filetype, out, in, 1);
                }
              else
                {
                  /* Have to make a guess if it is RISC OS or unix format */
                  return guess_or_null (create_dir, flags, buffer, buf_end,
                                        filetype, out, in, name, 1);

                }
            }
        }
      else
        {
          /* Must be a relative path. Could be RISC OS or unix, and there's no
             reliable way to tell. */

          return guess_or_null (create_dir, flags, buffer, buf_end, filetype,
                                out, in, name, 0);
        }
    }

  /* Not reached */
  return NULL;
}

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
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
   '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�'
};


