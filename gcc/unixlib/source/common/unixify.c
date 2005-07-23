/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/unixify.c,v $
 * $Date: 2005/07/20 18:29:30 $
 * $Revision: 1.18 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#include <errno.h>
#include <string.h>
#include <unixlib/local.h>
#include <internal/swiparams.h>
#include <unixlib/os.h>
#include <stdlib.h>
#include <ctype.h>
#include <swis.h>
#include <stdio.h>

/* #define DEBUG */

static int
add_filetype (char *buffer, char *out, char *out_end, int filetype, int unixify_flags)
{
  int ft_extension_needed = 1;

  if (!(unixify_flags & __RISCOSIFY_FILETYPE_NOT_SET))
    {
      const char *fn_extension;

      /* Check if we don't have a *filename* extension which already maps
	 via MimeMap to the filetype 'filetype'.  If so, no need to
	 add the RISC OS filetype again using *filetype* extension.  */
      for (fn_extension = out - 1;
	   fn_extension != buffer
	   && *fn_extension != '/' && *fn_extension != '.';
	   --fn_extension)
	;
      if (*fn_extension == '.')
	{
	  int regs[10];

	  /* We have a filename extension at 'fn_extension'.  */
	  regs[0] = MMM_TYPE_DOT_EXTN; /* Input extension */
	  regs[1] = (int)fn_extension;
	  regs[2] = MMM_TYPE_RISCOS; /* Output extension */

	  /* When there is no MimeMap error and the filetype returned
	     matches 'filetype', we don't want filetype extension.  */
	  if (! __os_swi (MimeMap_Translate, regs)
	      && regs[3] == filetype)
	    ft_extension_needed = 0;
	}
    }

  if (ft_extension_needed)
    {
      /* We need to add filetype ",xyz".  */
      if ((out + 4) >= out_end)
	return 0;

      *out++ = ',';
      *out++ = "0123456789abcdef"[(filetype >> 8) & 0xf];
      *out++ = "0123456789abcdef"[(filetype >> 4) & 0xf];
      *out++ = "0123456789abcdef"[(filetype >> 0) & 0xf];
      *out = '\0';
    }

  return 1;
}


static int
get_directory_name (const char *input, char *output)
{
  const char *t = NULL;

  if (*input == '\0')
    {
      *output = '\0';
      return 0;
    }

  /* RISC OS directory names are delimited by a '.'.
     We must check for a few Unix styles though.  */
  t = strchr (input, '.');
  if (t != NULL)
    {
      if (input[0] == '.')
	{
	  if (input[1] == '\0')        /* current directory, end of path */
	    t = NULL;
	  else if (input[1] == '/')    /* current directory, not end of path */
	    t = input + 1;
	  else if (input[1] == '.')
	   {
	     if (input[2] == '\0')     /* parent directory, end of path */
	       t = NULL;
	     else if (input[2] == '/') /* parent directory, not end of path */
	       t = input + 2;
	   }
	}
    }

  if (t != NULL)
    {
      strncpy (output, input, t - input);
      output[t - input] = '\0';
      return 1;
    }
  else
    {
    /* If we reach here, we have four possibilities:
       1. fname
       2. directory/fname
       3. .
       4. ..
       none of which need any conversion. No.1 has both compatible with
       RISC OS and Unix, and nos. 2-4 are already in Unix form.  */
      strcpy (output, input);
      return 0;
    }
}

static char *
add_directory_name (char *o, const char *i)
{
#ifdef DEBUG
  printf ("add_dir_name: i = '%s'\n", i);
#endif

  /* Root directory ($) can be ignored since output is only a '/'.
     The backslash is automatically output at the end of the string.  */
  if (i[0] != '\0' && i[1] == '\0')
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
      int c = 0;
      while (i[c])
        {
          if (i[c] == '/')
            *o++ = '.';
          else
            *o++ = i[c];
          c++;
        }
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
  /* Output in the form: 'dir.prefix'
     Return ptr to \0 string terminating char in output buffer. */
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

  /* If there is more than one dot left in the filename, then this
     cannot be the prefix.  */
  if ((t1 = strchr (name, '.')) != strrchr (name, '.'))
    return 0;

  if (t1 == NULL)
    t1 = name + strlen(name);

  return __sfixfind(name, t1 - name) == NULL ? 0 : 1;
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


/* Call __unixify with __riscosify_flags as the unixify_flags.  */
char *
__unixify_std (const char *name, char *buffer, size_t buflen,
		     int filetype)
{
  return __unixify (name, __get_riscosify_control (), buffer, buflen,
		    filetype);
}

/* Convert RO_PATH into a Unix style pathname and store in BUFFER. If BUFFER
   is non-NULL then it is at least BUF_LEN long. If BUFFER is NULL, then a
   buffer is malloc'ed to store the Unix pathname. Return a pointer to the
   result, or NULL on failure.  */
char *
__unixify (const char *ro_path, int unixify_flags, char *buffer,
	   size_t buf_len, int filetype)
{
  char tempbuf[256];
  char *out_end, *out;
  char *temp;
  int flag, skip;
  const char *const in_buf = buffer; /* = NULL if we malloc the buffer.  */
  const char *input;

#ifdef DEBUG
  printf("__unixfy(%s, 0x%x, %p, %d, 0x%x)\n", ro_path, unixify_flags, buffer, buf_len, filetype);
#endif
  if (buffer == NULL)
    {
      buf_len = strlen (ro_path) + 10;
      /* FIXME. buf_len is just an estimate. It needs the length of ro_path
         + 1 for the null terminator
         + 1 for a leading /
         + 1 for add_directory_name to add an unecessary trailing /
         + 3 if the path has a %
         + 1 for every ^ in the path.
         The code really needs modifying to check it isn't overflowing the
         output buffer as it adds characters.  */

      /* Allow for the ,xyz filetype extension.  */
      if ((unixify_flags & __RISCOSIFY_FILETYPE_EXT) != 0
	  && filetype != __RISCOSIFY_FILETYPE_NOTSPECIFIED)
	buf_len += 4;

      if ((buffer = (char *) malloc (buf_len)) == NULL)
	return NULL;
    }
  else if (buf_len == 0)
    return NULL;

  if (unixify_flags & __RISCOSIFY_NO_PROCESS)
    {
      size_t len = strlen (ro_path);
      /* Equal sizes would leave no room for terminating '\0'  */
      if (len >= buf_len)
	goto buf_overflow;

      return (char *) memcpy (buffer, ro_path, len + 1);
    }

  input = ro_path;
  out = buffer;
  buffer[0] = '\0';
  out_end = buffer + buf_len;

  /* Fast case. Look for a `.', `..', '../', '/' or `./'.  */
  if (ro_path[0] == '.')
    {
      if (ro_path[1] == '\0')
        {
          *out++ = '.';
          *out = '\0';
          return out;
        }

      if (ro_path[1] == '.')
        {
          if (ro_path[2] == '\0'
              || (ro_path[2] == '/' && ro_path[3] == '\0'))
            {
              *out++ = '.';
              *out++ = '.';
              *out = '\0';
              return out;
            }
        }

      if (ro_path[1] == '/' && ro_path[2] == '\0')
        {
          *out++ = '.';
          *out = '\0';
          return out;
        }
    }

  if (ro_path[0] == '/' && ro_path[1] == '\0')
    {
      *out++ = '/';
      *out = '\0';
      return out;
    }

  /* If we take a file name like:
     IDEFS::HD.$.Work.gcc.gcc-272.config.arm.c.rname
     we would like to convert it to:
     /IDEFS::HD.$/Work/gcc/gcc-272/config/arm/rname.c

     Firstly try and locate a '.$'. Anything before this just specifies
     a file system.  */
  temp = strstr (ro_path, ".$");
  if (temp != NULL)
    {
      /* We've found a '.$' */
      if (*input != '/')
        *out++ = '/';
      temp += 2;
      while (input != temp)
	*out++ = *input++;

       /* Terminate only if there's no more.  Otherwise, get_directory_name
          gets it wrong for files in the root */
      if (*input == '\0')
        *out++ = '/';
    }
  else
    {
      temp = strchr (ro_path, ':');
      if (temp)
	{
	  /* Add a / to the start to remove any ambiguity when converting back
	     to RISC OS format */
	  if (*ro_path != '/')
	    *out++ = '/';
	}
    }

  flag = 0;
  while ((skip = get_directory_name (input, tempbuf)) != 0)
    {
#ifdef DEBUG
      printf ("input = '%s', tempbuf = '%s'\n", input, tempbuf);
#endif
      if (one_dot (input))
	{
	  char name[256];

	  /* We've processed enough of the filename to be left with the
	     following combinations:
	     1. fname.prefix  e.g. fred.c
	     2. prefix.fname  e.g. c.fred
	     3. fname.nonprefix  e.g. fred.jim  */
	  /* Add 1 to get past dot.  */
	  input += strlen (tempbuf) + 1;
	  get_directory_name (input, name);

	  if (is_prefix (name))
	    {
	      /* Method 1.
	         name contains the prefix.
	         tempbuf contains the filename.  */
	      out = add_directory_and_prefix (out, tempbuf, name);
	    }
	  else if (is_prefix (tempbuf))
	    {
	      /* Method 2.
	         tempbuf contains the prefix.
	         name contains the filename.  */
	      out = add_directory_and_prefix (out, name, tempbuf);
	    }
	  else
	    {
	      /* Method 3. No prefixes in the filename.
	         tempbuf contains the first section.
	         name contains the last bit.  */
	      out = add_directory_name (out, tempbuf);
	      out = add_directory_name (out, name);
	      /* Remove the final backslash automatically added in by
	         add_directory_name.  */
	      *--out = '\0';
	    }
	  flag = 1;
	}
      else
        {
	  out = add_directory_name (out, tempbuf);
	  /* Add 1 to get past dot.  */
	  input += strlen (tempbuf) + 1;
	}
    }
#ifdef DEBUG
  printf ("final i = '%s', tempbuf = '%s', flag %d\n", input, tempbuf, flag);
#endif

  if (!flag)
    {
      strcpy (out, tempbuf);
      out += strlen(out);
    }
#ifdef DEBUG
  printf ("input = '%s'\noutput = '%s'\n", ro_path, buffer);
#endif

   /* Do filetype extension, if needed.  */
  if ((unixify_flags & __RISCOSIFY_FILETYPE_EXT) == 0 ||
       filetype == __RISCOSIFY_FILETYPE_NOTSPECIFIED ||
       add_filetype(buffer, out, out_end, filetype, unixify_flags))
    return buffer;

buf_overflow:
  /* Free the buffer if we allocated it, though buffer overflow should not
     be possible if we did allocate it.  */
  if (in_buf == NULL)
    free (buffer);
  (void) __set_errno (ERANGE);
  return NULL;
}
