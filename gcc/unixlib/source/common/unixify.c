/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/unixify.c,v $
 * $Date: 2003/08/18 22:35:36 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#include <errno.h>
#include <string.h>
#include <unixlib/local.h>
#include <unixlib/swiparams.h>
#include <stdlib.h>
#include <ctype.h>
#include <swis.h>

/* Call __unixify with __riscosify_flags as the unixify_flags.  */
char *__unixify_std (const char *name, char *buffer, size_t buflen,
     		     int filetype)
{
  return __unixify (name, __get_riscosify_control (), buffer, buflen,
		    filetype);
}

/* Convert RO_PATH into a Unix style pathname and store in BUFFER. If BUFFER
   is non-null then it is at least BUF_LEN long. If BUFFER is null, then a
   buffer is malloc'ed to store the Unix pathname. Return a pointer to the
   BUFFER (when not NULL) or the malloc block (when NULL), or null on
   failure.  */
char *
__unixify (const char *ro_path, int unixify_flags, char *buffer,
  	   size_t buf_len, int filetype)
{
  char *buf_end, *out;
  const char *in0;
  char current, next;
  int can_be_discname = 1;
  const char *const in_buf = buffer; /* Null if we malloc the buffer.  */

  if (!buf_len)
    return NULL;

  if (buffer == NULL)
    {
      buf_len = strlen (ro_path) + 2;

      /* Allow for the ,xyz filetype extension.  */
      if ((unixify_flags & __RISCOSIFY_FILETYPE_EXT) != 0
          && filetype != __RISCOSIFY_FILETYPE_NOTSPECIFIED)
        buf_len += 4;

      if ((buffer = malloc (buf_len)) == NULL)
	return NULL;
    }

  if (unixify_flags & __RISCOSIFY_NO_PROCESS)
    {
      size_t len = strlen (ro_path);
      /* Equal sizes would leave no room for terminating '\0'  */
      if (len >= buf_len)
	goto buf_overflow;

      return (char *) memcpy (buffer, ro_path, len + 1);
    }

  /* buf_len > 0 and buffer != NULL.  */
  out = buffer;
  buf_end = buffer + buf_len;

  *out = '/';

  /* Path names and disc names.
     Filing system names are alphanumeric or '_'
     Experimentation suggests path names may only be alphanumeric or '_'.
     Note, we do not attempt to handle -fs- type pathnames, since the
     use of them is virtually deprecated (see PRM 2-11).  */
  if (isalnum (current = *ro_path) || current == '_')
    {
      in0 = ro_path;
      while (isalnum (next = *++in0) || next == '_')
	;

      /* Skip special fields in the pathname. These start with '#' and can
	 contain **anything** except ':'.  */
      if (next == '#')
	while ((next = *++in0) != '\0' && next != ':')
	  ;

      if (next == ':')
	{
	  /* It's a path.  */
	  size_t pathlen = in0 + 1 - ro_path;

	  if (pathlen >= buf_len)  /* >= (1 char less) due to leading '/'.  */
	    goto buf_overflow;

	  out++; /* Use that '/' already there.  */
	  memcpy (out, ro_path, pathlen);
	  ro_path += pathlen;
	  out += pathlen;
	  current = *ro_path;
	  can_be_discname = 1;
	}
      else
	can_be_discname = 0;
    }

  /* A comment describing what we are doing here would be nice.
     current == ':' => we've got fsname::something.  */
  if (can_be_discname && current == ':')
    {
      if (buffer == out)
	out++; /* Use that '/' already there.  */

      while (current && current != '.')
	{
	  /* I don't think that riscosify is symetrical yet
	     discnames containing '/' will confuse it.  */

	  if (current == '/')
	    current = '.';

	  if (out < buf_end)
	    *out++ = current;

	  current = *++ro_path;
	}

      /* out = buf_len means we are exactly full
	 out > buf_len means a bug!  */
      if (out >= buf_end)
	goto buf_overflow;

      if (current == '.')
	{
	  *out++ = '/';
	  ro_path++;
	}
    }

  /* At this point we either have
     /fs:
     /fs::disc/
     /:disc/

     (ie nothing)
     and we are pointing at not-a-'.' (possibly '\0').  */

  while ((current = *ro_path) != '\0' && out < buf_end)
    {
      next = *(ro_path + 1);
      if (next == '.' || next == '\0')
	{
	  switch (current)
	    {
	    case '$':
	    case '%':
	    case '&':
	    case '\\':
	      if (buffer == out)
		out++; /* Use that '/' already there.  */

	      if (out < buf_end)
		*out++ = current;
	      ro_path++;

	      if (next == '.')
		{
		  if (out < buf_end)
		    *out++ = '/';
		  ro_path++;
		}
	      continue;

	    case '^':
	      if (out < buf_end)
		*out++ = '.';
	      /* Drop through...  */

	    case '@':
	      if (out < buf_end)
		*out++ = '.';
	      ro_path++;

	      if (next == '.')
		{
		  if (out < buf_end)
		    *out++ = '/';
		  ro_path++;
		}
	      continue;
	    }
	}
      /* End of checks for $ ^ @  */

      /* Copy until end of name or end of directory ('.').  */
      while (current != '\0' && current != '.')
	{
	  if (out < buf_end)
	    *out++ = __filename_char_map[(unsigned char) current];

	  current = *++ro_path;
	}

      if (current == '.')
	{
	  if (out < buf_end)
	    *out++ = '/';
	  ++ro_path;
	}
    }

  if (out < buf_end)
    {
      *out = '\0';

      /* Do suffix swapping, if needed.  */
      if ((unixify_flags & __RISCOSIFY_NO_REVERSE_SUFFIX) == 0)
        {
          char *after_suffix;

          /* Skip leafname.  */
          for (after_suffix = out - 1;
               after_suffix != buffer && *after_suffix != '/';
               after_suffix --)
            ;
          if (*after_suffix == '/')
            {
              char *before_suffix;

              for (before_suffix = after_suffix - 1;
                   before_suffix != buffer && *before_suffix != '/';
                   before_suffix --)
                ;

              if (before_suffix == buffer || *before_suffix++ == '/')
                {
                  /* before_suffix is "c/file"
                     after_suffix is "/file"  */
                  *after_suffix = '\0'; /* temporary.  */
                  if (__sfixfind (before_suffix, after_suffix - before_suffix))
                    {
                      /* We need to do reverse suffix swapping.  */
                      char suffix[32];

                      memcpy (suffix, before_suffix,
                      	      after_suffix - before_suffix + 1);
                      strcpy (before_suffix, after_suffix + 1);
                      before_suffix[out - after_suffix - 1] = '.';
                      strcpy (before_suffix + (out - after_suffix - 1) + 1,
                      	      suffix);
                    }
                  else
                    *after_suffix = '/';
                }
            }
        }

      /* Do filetype extension, if needed.  */
      if ((unixify_flags & __RISCOSIFY_FILETYPE_EXT) != 0
          && filetype != __RISCOSIFY_FILETYPE_NOTSPECIFIED)
        {
          int ft_extension_needed = 1;

          if (!(unixify_flags & __RISCOSIFY_FILETYPE_NOT_SET))
            {
              char *fn_extension;

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
                  _kernel_swi_regs regs;

                  /* We have a filename extension at 'fn_extension'.  */
                  regs.r[0] = MMM_TYPE_DOT_EXTN; /* Input extension */
                  regs.r[1] = (int)fn_extension;
                  regs.r[2] = MMM_TYPE_RISCOS; /* Output extension */

                  /* When there is no MimeMap error and the filetype returned
                     matches 'filetype', we don't want filetype extension.  */
                  if (! _kernel_swi (MimeMap_Translate, &regs, &regs)
                      && regs.r[3] == filetype)
                    ft_extension_needed = 0;
                }
            }

          if (ft_extension_needed)
            {
              /* We need to add filetype ",xyz".  */
              if ((out + 4) >= buf_end)
                goto buf_overflow;

              *out++ = ',';
              *out++ = "0123456789abcdef"[(filetype >> 8) & 0xf];
              *out++ = "0123456789abcdef"[(filetype >> 4) & 0xf];
              *out++ = "0123456789abcdef"[(filetype >> 0) & 0xf];
              *out = '\0';
            }
        }

      return buffer;
    }

buf_overflow:
  /* Free the buffer if we allocated it, though buffer overflow should not
     be possible if we did allocate it.  */
  if (in_buf == NULL)
    free (buffer);
  (void) __set_errno (ERANGE);
  return NULL;
}
