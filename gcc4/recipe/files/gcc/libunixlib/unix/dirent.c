/* Perform operations on a directory.
   Copyright (c) 2002-2013 UnixLib Developers.  */

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <swis.h>

#include <unixlib/local.h>
#include <internal/unix.h>
#include <internal/os.h>
#include <internal/swiparams.h>
#include <internal/local.h>
#include <pthread.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

/* Size of OS_GBPB buffer. If this is too small, directory reading
   performance is severly reduced. A 1K buffer is sufficient until
   long filenames. It should at least to be big enough to contain
   1 OS_GBPB 10 record otherwise we loop forever !  */
#define CACHE_SIZE 1024

/* It is a common mistake to assume that the offsets returned from
   OS_GBPB is monotonic increasing with unity steps from the start
   offset 0 onwards.  Accoring to the PRMs, the only thing you may
   be sure of is that offset 0 starts the GBPB enumeration and
   offset -1 returned indicates the end of the enumeration.
   We assign two 'random' values to represent directories '.' and
   '..' (only be used when we do want unix <-> RISC OS filename
   processing) and hope those values will never be returned from
   OS_GBPB.  */
#define GBPB_FAKE_CURRENTDIR	0x2A0E180B
#define GBPB_FAKE_PARENTDIR	0x04B63067
#define GBPB_START_ENUM		0x00000000
#define GBPB_END_ENUM		0xFFFFFFFF

/* After preaching the passion, now the hack : when we're going to do
   the reverse suffix swapping, we will have up to two outstanding OS_GBPB
   sessions : one in the main dir and another in the suffix dir.
   How we're going to give an 'off_t' result in telldir describing these
   two offsets in OS_GBPB ? Well, we are going to rely on unity monotonic
   increase of the offsets and this up to GBPB_MAX_ENUM.  If these
   conditions are not fullfilled, we stop the enumeration.
   Another solution is to return table index numbers which, when presented
   to seekdir, will get looked up in a table giving the two internal dir
   offsets.  */
#define GBPB_MAX_ENUM		0x00010000

static DIR *dir_head = NULL;

/* Invalidate a directory stream.  */
static void
invalidate (DIR *stream)
{
  PTHREAD_UNSAFE

  DIR *next = stream->next;
  memset(stream, 0, sizeof (DIR));
  stream->next = next;
  stream->fd = -1;
}

/* Make a new directory stream with RISC OS name 'name' and starting
   the OS_GBPB enum at offset 'offset'.  */
static DIR *
newstream (const char *name, __off_t offset)
{
  PTHREAD_UNSAFE

  DIR *stream = dir_head;
  /* Look for a previously created stream that is now finished free.  */
  while (stream && stream->__magic == _DIRMAGIC)
    stream = stream->next;

  if (stream == NULL)
    {
      /* None to reuse, so create a new one and place it
	 at the head of the list.  */
      stream = (DIR *) malloc (sizeof (DIR));
      if (stream == NULL)
	return NULL;
      stream->next = dir_head;
      dir_head = stream;
    }

  invalidate (stream);
  stream->__magic = _DIRMAGIC;

  /* stream created, now filling it in with meaningful values.  */

  /* Canonicalise the name in case the user changes the PSD.  */
  if ((stream->dd_name_can = __canonicalise_path (name, NULL)) == NULL)
    {
      invalidate (stream);
      return NULL;
    }

  /* Initialise a directory cache to speed up reading.  */
  stream->dir_cache = malloc (CACHE_SIZE);
  if (stream->dir_cache == NULL)
    {
      free ((void *)stream->dd_name_can);
      invalidate (stream);
      return NULL;
    }

  /* Initialise the directory stream.  */
  stream->gbpb_off = stream->dd_off = offset;
#ifdef DEBUG
  debug_printf ("Stream 0x%p created : gbpb_off 0x%x and dd_off 0x%x\n",
		stream, stream->gbpb_off, stream->dd_off);
#endif
  /* Already done by invalidate() :
  stream->suffix = NULL;
  stream->dd_suf_off = GBPB_START_ENUM;
  stream->dir_cache_index = NULL;
  stream->do_read = 0;
  */

  return stream;
}


/* Open 'name' as a directory stream. */
DIR *
opendir (const char *ux_name)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (ux_name == NULL)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* We don't want suffix swapping here.  */
  char name[_POSIX_PATH_MAX];
  if (!__riscosify (ux_name, 0,
		    __get_riscosify_control() | __RISCOSIFY_NO_SUFFIX,
		    name, sizeof (name), NULL))
    {
      (void) __set_errno (ENAMETOOLONG);
      return NULL;
    }

#ifdef DEBUG
  debug_printf ("-- opendir: ux_name=%s, ro name=%s\n", ux_name, name);
#endif

  switch (__object_exists_ro (name))
    {
      case 0:
	(void) __set_errno (ENOENT);
	return NULL;
      case 1:
	(void) __set_errno (ENOTDIR);
	return NULL;
    }

  /* If leafname of 'name' is one of the suffixes and __riscosify_control
     & __RISCOSIFY_NO_REVERSE_SUFFIX is clear, then we're about to enum
     a dir which doesn't really exist from the unix point of view.  */
  if (!(__get_riscosify_control () & __RISCOSIFY_NO_REVERSE_SUFFIX))
    {
      const char *leafname, *i;

#ifdef DEBUG
      debug_printf ("Suffix handling !\n");
#endif
      for (i = leafname = name; *i; /* nothing */)
	if (*i++ == '.')
	  leafname = i;

#ifdef DEBUG
      debug_printf ("name 0x%p, leafname 0x%p\n", name, leafname);
      debug_printf ("name '%s', leafname '%s'\n", name, leafname);
#endif
      if (__sfixfind (leafname, strlen (leafname)))
	{
	  (void) __set_errno (ENOENT);
	  return NULL;
	}
    }

  /* Get a new DIR stream.  */
  DIR *stream = newstream (name,
			   (__get_riscosify_control () & __RISCOSIFY_NO_PROCESS)
			     ? GBPB_START_ENUM : GBPB_FAKE_CURRENTDIR);
  /* stream = NULL when newstream() failed.  */

  return stream;
}

/* Read the next entry from the directory stream. POSIX.4a version.  */
int
readdir_r (DIR *stream, struct dirent *entry, struct dirent **result)
{
  _kernel_oserror *err;
  int slen;
  const int riscosify_ctl = __get_riscosify_control ();

  PTHREAD_UNSAFE_CANCELLATION

  *result = NULL;

#ifdef DEBUG
  debug_printf ("readdir_r: stream 0x%p : start user offset 0x%x\n",
			stream, stream->dd_off);
#endif

  if (!__validdir (stream))
    return EBADF;

  /* Enumerating the suffix swapped directory ?  */
  if (stream->suffix)
    {
      struct dirent *sresult;

      /* Perhaps we no longer can fetch entries from our suffix dir ?  */
      if (stream->suffix->dd_off != GBPB_END_ENUM)
	{
#ifdef DEBUG
	  debug_printf ("doing enum of suffix stream...\n");
#endif
	  /* Directories inside RISC OS suffix directories may not
	     come out of a dirent enum.  */
	  do
	    {
	      readdir_r (stream->suffix, entry, result);
	    } while ((sresult = *result) != NULL
		     && sresult->d_type == DT_DIR);

	  if (sresult != NULL)
	    {
	      char x,y,z;
	      char *str;
#ifdef DEBUG
	      fprintf (stderr, "suffix stream data at 0x%p\n", sresult);
#endif
	      /* When __RISCOSIFY_FILETYPE_EXT is set in __riscosify_control,
		 map <filename,xyz> to <filename>.d_name,xyz with xyz a
		 valid hex number.  */
	      if ((riscosify_ctl & __RISCOSIFY_FILETYPE_EXT)
		  && sresult->d_namlen > sizeof(",xyz")-1
		  && sresult->d_name[sresult->d_namlen - 4] == ','
		  && isxdigit (x = sresult->d_name[sresult->d_namlen - 3])
		  && isxdigit (y = sresult->d_name[sresult->d_namlen - 2])
		  && isxdigit (z = sresult->d_name[sresult->d_namlen - 1]))
		{
		  str = sresult->d_name + sresult->d_namlen - 4;
		  *str++ = '.';
		  str = stpcpy (str, strrchr(stream->suffix->dd_name_can, '.') + 1);
		  *str++ = ',';
		  *str++ = x;
		  *str++ = y;
		  *str++ = z;
		  *str = '\0';
		}
	      else
		{
		  str = sresult->d_name + sresult->d_namlen;
		  *str++ = '.';
		  str = stpcpy (str, strrchr(stream->suffix->dd_name_can, '.') + 1);
		}

	      sresult->d_namlen = str - sresult->d_name;
	      sresult->d_fileno = __get_file_ino (stream->suffix->dd_name_can,
						  sresult->d_name);

	      return 0;
	    }
	}

      /* Suffix dir enumeration done ?  */
      if (stream->suffix->dd_off == GBPB_END_ENUM)
	{
#ifdef DEBUG
	  fprintf (stderr, "got last (or no) element from suffix stream\n");
#endif
	  closedir (stream->suffix);
	  stream->suffix = NULL;
	}

      /* Continue with enum of the main dir if suffix dir enum didn't
	 return a valid entry.  */
    }

#ifdef DEBUG
  fprintf (stderr,
	   "readir_r: stream 0x%p : enum user offset 0x%x, system offset 0x%x\n",
	   stream, stream->dd_off, stream->gbpb_off);
#endif
  switch (stream->dd_off)
    {
      case GBPB_FAKE_CURRENTDIR:
      case GBPB_FAKE_PARENTDIR:
        {
          int x = 0;
          /* Fake directory '.' or '..'.  */
          if (stream->dd_off == GBPB_FAKE_PARENTDIR)
            entry->d_name[x++] = '.'; /* Fake directory '..'  */

          entry->d_name[x++] = '.';

          entry->d_name[x] = '\0';
          entry->d_namlen = x;

          /* These directories are (hard) links to directories.  */
          entry->d_type = DT_DIR;

          stream->dd_off = stream->gbpb_off =
            (stream->gbpb_off == GBPB_FAKE_CURRENTDIR)
            ? GBPB_FAKE_PARENTDIR : GBPB_START_ENUM;
          break;
        }

      case GBPB_END_ENUM:
        return 0;

      default:
        {
          char *str;
          /* Does the cache needs a refill ?  */
          if (stream->do_read == 0)
            {
              int regs[10];
                /* Read entries from the directory. We loop until we have read at
                  least one entry or we have reached the end of the directory.
                  This is because OS_GBPB is allowed to do nothing due to
                  external constraints imposed by some filing systems
                  (PRM 2-70).  */
              do
                {
                  regs[0] = 10;
                  regs[1] = (int) stream->dd_name_can;
                  regs[2] = (int) stream->dir_cache;
                  /* Max entries possible given zero termination of names.  */
                  regs[3] = CACHE_SIZE / (sizeof(__os_gbpb_10) + 1);
                  regs[5] = CACHE_SIZE;
                  regs[4] = (int) stream->gbpb_off;
                  regs[6] = 0; /* Match all names.  */
 
                  err = __os_swi (OS_GBPB, regs);
                  if (err)
		    return __ul_seterr (err, EIO);

#ifdef DEBUG
                  fprintf (stderr,
                          "Start enum 0x%x, got 0x%x with stop enum 0x%x\n",
                          stream->gbpb_off, regs[3], regs[4]);
#endif
                  if (regs[4] == GBPB_END_ENUM)
                    {
                      if (regs[3] == 0)
                        {
                          stream->gbpb_off = stream->dd_off = GBPB_END_ENUM;
                          return 0;
                        }
                      }
                  else
                    {
                      /* Check if we have a monotonic unit increase OS_GBPB
                        offset, if not, we don't do anything.  */
                      if (stream->gbpb_off + regs[3] != regs[4])
                        {
                          stream->gbpb_off = stream->dd_off = GBPB_END_ENUM;
                          return 0;
                        }

                      /* We can't enum more than GBPB_MAX_ENUM-1 objects.  */
                      if ((unsigned int)regs[4] >= GBPB_MAX_ENUM)
                        {
                          regs[3] -= (unsigned int)regs[4] - GBPB_MAX_ENUM;
                          if (regs[3] <= 0)
                            {
                              stream->gbpb_off = stream->dd_off = GBPB_END_ENUM;
                              return 0;
                            }

                          regs[4] = (int)GBPB_END_ENUM;
                        }
                    }

                  stream->gbpb_off = regs[4];
                } while (regs[3] == 0);

              stream->do_read = regs[3];
              stream->dir_cache_index = stream->dir_cache;
            } /* cache refill */

          /* Point 'filename' to the correct directory entry.  */

#ifdef DEBUG
          fprintf (stderr, "Raw RO name '%s', entry at 0x%p\n",
                  stream->dir_cache_index->obj_name, entry);
#endif
          /* Copy name direct if flag is set.  */
          if (riscosify_ctl & __RISCOSIFY_NO_PROCESS)
            {
              str = stpcpy (entry->d_name, stream->dir_cache_index->obj_name);
              slen = (offsetof (__os_gbpb_10, obj_name)
                      + (str - entry->d_name) + 1 + 3) & -4;
            }
          else
            {
              const char *from = stream->dir_cache_index->obj_name;
              const char *end = &entry->d_name[sizeof(entry->d_name) - 1];
              char ch;

              /* Convert RISC OS name to Unix format.
                Map  '/' => '.', '?' => '#'
                see common.c.riscosify for the table.  */
              str = entry->d_name;
              do
                {
                  ch = *from++;
                  *str++ = __filename_char_map[(unsigned char) ch];
                } while (ch != '\0' && str <= end);
              /* Ensure we still have a C zero terminated string.  */
              *--str = '\0';

              slen = (offsetof (__os_gbpb_10, obj_name)
                      + (str - entry->d_name) + 1 + 3) & -4;

              /* Need to add filetype extension (for straight files or
		 image files if they are treated as files).  */
              if ((riscosify_ctl & __RISCOSIFY_FILETYPE_EXT)
                  && (stream->dir_cache_index->obj_type == 1 ||
                      (stream->dir_cache_index->obj_type == 3
                      && __get_feature_imagefs_is_file ()))
                  && (stream->dir_cache_index->load_address & 0xfff00000U) == 0xfff00000U)
                {
                  int filetype;

                  filetype = (stream->dir_cache_index->load_address >> 8) & 0xfff;
                  if (filetype != 0xfff && str+4 <= end)
                    {
                      int ft_extension_needed = 1;

                      if (!(riscosify_ctl & __RISCOSIFY_FILETYPE_NOT_SET))
                        {
                          char *fn_extension;

                          for (fn_extension = str - 1;
                              fn_extension != entry->d_name && *fn_extension != '.';
                              --fn_extension)
                            ;

                          if (*fn_extension == '.')
                            {
                              int regs[10];

                              /* We have a filename extension at
				 'fn_extension'.  */
                              regs[0] = MMM_TYPE_DOT_EXTN; /* Input extension */
                              regs[1] = (int)fn_extension;
                              regs[2] = MMM_TYPE_RISCOS; /* Output extension */

                              /* When there is no MimeMap error and the
                                filetype returned matches 'filetype', we don't
                                want filetype extension.  */
                              if (! __os_swi (MimeMap_Translate, regs)
                                  && regs[3] == filetype)
                                ft_extension_needed = 0;
                            }
                        }

                      if (ft_extension_needed)
                        {
                          *str++ = ',';
                          *str++ = "0123456789abcdef"[filetype >> 8];
                          *str++ = "0123456789abcdef"[(filetype >> 4) & 0xf];
                          *str++ = "0123456789abcdef"[filetype & 0xf];
                          *str = '\0';
                        }
                    }
                }
            }

          entry->d_type = (stream->dir_cache_index->obj_type == 1 ||
                          (stream->dir_cache_index->obj_type == 3
                            && __get_feature_imagefs_is_file ()))
                          ? DT_REG : DT_DIR;

          entry->d_namlen = str - entry->d_name;
          /* 'entry' is now correctly filled in except for the d_fileno field.  */

          /* Update the stream params.  */
          stream->dir_cache_index = (__os_gbpb_10 *)(((char *)stream->dir_cache_index) + slen);
          stream->do_read--;
          /* Did the previous OS_GBPB enum indicated the end ?  */
          if (stream->do_read == 0 && stream->gbpb_off == GBPB_END_ENUM)
            stream->dd_off = GBPB_END_ENUM;
          else
            stream->dd_off++;

#ifdef DEBUG
          fprintf (stderr, "reverse suffix %d, obj_type %d, d_name '%s'\n",
                  !(riscosify_ctl & __RISCOSIFY_NO_REVERSE_SUFFIX),
                  entry->d_type == DT_DIR, entry->d_name);
#endif
          /* Check for reverse suffix dir swapping.  */
          if (!(riscosify_ctl & __RISCOSIFY_NO_REVERSE_SUFFIX)
              && entry->d_type == DT_DIR
              && __sfixfind (entry->d_name, entry->d_namlen))
            {
              char name[_POSIX_PATH_MAX];
              char *str = stpcpy (name, stream->dd_name_can);
              *str++ = '.';
              strcpy(str, entry->d_name);

              stream->suffix = newstream (name , stream->dd_suf_off);
              stream->dd_suf_off = GBPB_START_ENUM;

              /* Get the first entry in the suffix directory.  */
#ifdef DEBUG
              fprintf (stderr, "First entry in suffix dir\n");
#endif
              readdir_r (stream, entry, result);
#ifdef DEBUG
              fprintf (stderr,
                      "Result of first entry query in suffix dir : 0x%p\n",
                      *result);
#endif
              return 0;
            }
          break;
        }
    }


  /* The file serial number. This is the same as the st_ino member
     that stat will return for the file.  */
  entry->d_fileno = __get_file_ino (stream->dd_name_can, entry->d_name);

  *result = entry;
  return 0;
}

/* Read the next entry from the directory stream. This version of readdir
   allows different DIR streams to be read simultaneously without corruption
   of the returned result. Multiple readers on the same DIR stream is not
   reentrant.  */
struct dirent *
readdir (DIR *stream)
{
  PTHREAD_SAFE_CANCELLATION

  if (!__validdir (stream))
    {
      (void) __set_errno (EBADF);
      return NULL;
    }

  /* This call relies upon readdir_r setting result to NULL on failure.  */
  struct dirent *result;
  readdir_r (stream, &stream->dirent, &result);
  return result;
}

/* Return the file position of the directory stream.  */
long int
telldir (DIR *stream)
{
  if (!__validdir (stream))
    return -1;

  if (stream->suffix && stream->suffix->dd_off != GBPB_END_ENUM)
    return (stream->dd_off - 1) + (stream->suffix->dd_off << 16);

  return stream->dd_off + (stream->dd_suf_off << 16);
}

/* Set the file position of the directory stream to pos.  */

void
seekdir (DIR *stream, long int pos)
{
  if (__validdir (stream))
    {
      /* If we were doing a suffix dir enumeration, close the suffix
	 stream.  If needed, a new one will be created automatically.  */
      if (stream->suffix != NULL)
	{
	  (void) closedir (stream->suffix);
	  stream->suffix = NULL;
	}

      /* Determing if 'pos' contains offset information for the suffix dir
	 enum. */
      long int sufpos;
      if (pos != GBPB_FAKE_CURRENTDIR
	  && pos != GBPB_FAKE_PARENTDIR
	  && pos != GBPB_END_ENUM
	  && pos >= GBPB_MAX_ENUM)
	{
	  sufpos = pos >> 16;
	  pos &= GBPB_MAX_ENUM - 1;
	}
      else
	sufpos = 0;

      stream->dd_suf_off = sufpos;
      stream->dd_off = stream->gbpb_off = pos;
      /* Force a re-cache of the directory entries.  */
      stream->do_read = 0;
    }
}

/* Reinitialise the directory stream. Reading will then
   start from the first entry.  */
void
rewinddir (DIR *stream)
{
  if (__validdir (stream))
    {
      if (stream->suffix != NULL)
	{
	  (void) closedir (stream->suffix);
	  stream->suffix = NULL;
	}

      stream->dd_suf_off = GBPB_START_ENUM;
      stream->dd_off = stream->gbpb_off
	      = (__get_riscosify_control () & __RISCOSIFY_NO_PROCESS)
		? GBPB_START_ENUM : GBPB_FAKE_CURRENTDIR;
      /* Force a re-cache of the directory entries.  */
      stream->do_read = 0;
    }
}

/* Close a DIR stream.  */
int
closedir (DIR *stream)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (!__validdir (stream))
    return __set_errno (EBADF);

  int result = (stream->suffix != NULL) ? closedir (stream->suffix) : 0;

  if (stream->fd != -1)
    close (stream->fd);

  /* Free any memory allocated for the stream, then invalidate
     it ready for re-use.  */
  free (stream->dir_cache);
  free ((void *)stream->dd_name_can);
  invalidate (stream);

  return result;
}


int
scandir (const char *dir, struct dirent ***namelist,
	 int (*sdselect)(const struct dirent *),
	 int (*cmp)(const struct dirent **, const struct dirent **))
{
  PTHREAD_UNSAFE_CANCELLATION

  DIR *dp = opendir(dir);
  if (dp == NULL)
    return -1;

  int save = errno;
  (void) __set_errno(0);

  struct dirent **v = NULL;
  size_t vsize = 0, i = 0;
  struct dirent *d;
  while ((d = readdir(dp)) != NULL)
    if (sdselect == NULL || (*sdselect)(d))
      {
	/* Ignore errors from select or readdir.  */
	(void) __set_errno(0);

	if (i == vsize)
	  {
	    struct dirent **newdir;
	    if (vsize == 0)
	      vsize = 10;
	    else
	      vsize *= 2;
	    newdir = (struct dirent **) realloc(v, vsize * sizeof(*v));
	    if (newdir == NULL)
	      break;
	    v = newdir;
	  }

	/* FIXME: this 256 should be a macro, but see comments in dirent.h.  */
	size_t dsize = &d->d_name[256] - (char *) d;
	struct dirent *vnew = (struct dirent *) malloc(dsize);
	if (vnew == NULL)
	  break;

	v[i++] = (struct dirent *) memcpy(vnew, d, dsize);
      }

  if (errno != 0)
    {
      save = errno;
      (void) closedir(dp);

      while (i > 0)
	free(v[--i]);
      free(v);

      return __set_errno (save);
    }

  (void) closedir(dp);
  (void) __set_errno(save);

  /* Sort the list if we have a comparison function to sort with.  */
  if (cmp != NULL && v != NULL)
    qsort(v, i, sizeof(*v), (int (*)(const void *,const void *))cmp);
  *namelist = v;
  return i;
}
