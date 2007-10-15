/* ln.c
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 *
 * Small utility to create the symlink files used by the RISC OS dynamic linking
 * system. This allows the same file, eg, a shared library, to be referred to by
 * more than one name without having to duplicate it. The format of the files is:
 *
 * +0: 'LINK' - file ID
 * +4: size in bytes of following target filename
 * +8: filename of target
 *
 * The target filename is assumed to be in RISC OS format and can be either
 * absolute or relative to the directory containing the symlink file. When
 * resolving a symlink, the presence of a colon is assumed to indicate an
 * absolute path. Symlinks can link to each other forming a chain upto a depth of
 * 64. This is an arbitrary limit, but some limit was required in case a chain of
 * links pointed back to itself creating an otherwise infinite loop.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __riscos
#  include <kernel.h>
#else
#  include <sys/stat.h>
#endif

#define	LINK_FILETYPE	0x1C8

static const char const LINK[] = "LINK";

static unsigned int symbolic = 0;
static unsigned int force = 0;
static unsigned int verbose = 0;

static void
settype (const char *file)
{
#ifdef __riscos
  _kernel_swi_regs regs;

  regs.r[0] = 18;
  regs.r[1] = (int) file;
  regs.r[2] = LINK_FILETYPE;
  _kernel_swi (0x8, &regs, &regs);
#endif
}

/* Return 1 if object exists as file, otherwise 0.  */
static int
exists_as_file (const char *file)
{
#ifdef __riscos
  _kernel_swi_regs regs;

  regs.r[0] = 17;
  regs.r[1] = (int) file;
  if (_kernel_swi (0x8, &regs, &regs) == NULL
      && regs.r[0] == 1)
    return 1;
#else
  struct stat buf;

  if (stat (file, &buf) == 0)
    return S_ISREG (buf.st_mode);
#endif

  return 0;
}

/* Return 1 if object exists as directory, otherwise 0.  */
static int
exists_as_dir (const char *dir)
{
#ifdef __riscos
  _kernel_swi_regs regs;

  regs.r[0] = 17;
  regs.r[1] = (int) dir;
  if (_kernel_swi (0x8, &regs, &regs) == NULL
      && (regs.r[0] == 2 || regs.r[0] == 3))
    return 1;
#else
  struct stat buf;

  if (stat (dir, &buf) == 0)
    return S_ISDIR (buf.st_mode);
#endif

  return 0;
}

static void
help (void)
{
  printf ("Usage: ln [OPTION]... TARGET LINK_NAME        (1st form)\n"
	  "  or:  ln [OPTION]... TARGET                  (2nd form)\n"
	  "  or:  ln [OPTION]... TARGET... DIRECTORY     (3rd form)\n"
	  "In the 1st form, create a link to TARGET with the name LINK_NAME.\n"
	  "In the 2nd form, create a link to TARGET in the current directory.\n"
	  "In the 3rd form, create links to each TARGET in DIRECTORY.\n"
	  "Create hard links by default, symbolic links with --symbolic.\n"
	  "When creating hard links, TARGET must exist.\n"
	  "\n"
	  "  -f, --force                 remove existing destination files\n"
	  "  -s, --symbolic              make symbolic links instead of hard links\n"
	  "  -v, --verbose               print name of each file before linking\n"
	  "      --help                  display this help and exit\n"
	  "      --version               output version information and exit\n");
}

static void
version (void)
{
  printf ("ln " VERSION "\n"
	  "Copyright (C) 2007 GCCSDK Developers\n"
	  "\n"
	  "Written by Lee Noar.\n");
}

static void
invalid_option (const char *option)
{
  printf ("ln: unrecognised option '%s'\n", option);
  printf ("Try 'ln --help' for more information.\n");
}

int
main (int argc, char *argv[])
{
  int i, target_len;
  FILE *link_file = NULL;
  char *target = NULL, *link_name = NULL, *target_arg = NULL, *link_arg =
    NULL;

  for (i = 1; i < argc; i++)
    {
      if (*(argv[i]) == '-')
	{
	  if (strcmp (argv[i] + 1, "s") == 0
	      || strcmp (argv[i] + 1, "-symbolic") == 0)
	    symbolic = 1;
	  else if (strcmp (argv[i] + 1, "f") == 0
		   || strcmp (argv[i] + 1, "-force") == 0)
	    force = 1;
	  else if (strcmp (argv[i] + 1, "v") == 0
		   || strcmp (argv[i] + 1, "-verbose") == 0)
	    verbose = 1;
	  else if (strcmp (argv[i] + 1, "-help") == 0)
	    {
	      help ();
	      return EXIT_SUCCESS;
	    }
	  else if (strcmp (argv[i] + 1, "-version") == 0)
	    {
	      version ();
	      return EXIT_SUCCESS;
	    }
	  else
	    {
	      invalid_option (argv[i]);
	      return EXIT_SUCCESS;
	    }
	}
      else if (target_arg == NULL)
	target_arg = argv[i];
      else
	{
	  link_name = link_arg = (char *) malloc (strlen (argv[i]) + 1);
	  if (!link_name)
	    {
	      fprintf (stderr, "ln: Out of memory\n");
	      goto err;
	    }
	  strcpy (link_name, argv[i]);
	}
    }

  if (!target_arg)
    {
      fprintf (stderr, "ln: Missing file argument\n");
      goto err;
    }

  target = target_arg;

  if (!link_name)
    {
      char *sep;

#ifdef __riscos
      sep = strrchr (target, '.');
      if (!sep)
	sep = strrchr (target, ':');
#else
      sep = strrchr (target, '/');
#endif
      if (sep)
	sep++;
      else
	{
	  fprintf (stderr, "ln: Unable to determine LINK_NAME\n");
	  goto err;
	}
      link_name = (char *) malloc (strlen (sep) + 3);
      if (!link_name)
	{
	  fprintf (stderr, "ln: Out of memory\n");
	  goto err;
	}

#ifdef __riscos
      /* Make sure riscosify knows that this is a RISC OS name.  */
      strcpy (link_name, "@.");
#else
      strcpy (link_name, "./");
#endif
      strcat (link_name, sep);
      link_arg = link_name + 2;
    }

  if (verbose)
    {
      if (symbolic)
	printf ("create symbolic link '%s' to '%s'\n", link_name, target);
      else
	printf ("create hard link '%s' to '%s'\n", link_name, target);
    }

  if (strcmp (link_name, target) == 0)
    {
      fprintf (stderr, "ln: TARGET and LINK_NAME cannot be the same\n");
      goto err;
    }

  if (!symbolic && !exists_as_file (target) && !exists_as_dir (target))
    {
      fprintf (stderr, "ln: '%s': No such file or directory\n", target);
      goto err;
    }

  if (exists_as_dir (link_name))
    {
      char *new_link_name = malloc (strlen (link_name) + 1 + strlen (target) + 1);
      if (new_link_name == NULL)
        {
	  fprintf (stderr, "ln: Out of memory\n");
	  goto err;
        }
      strcpy (new_link_name, link_name);
#ifdef __riscos
      strcat (new_link_name, ".");
#else
      strcat (new_link_name, "/");
#endif
      strcat (new_link_name, target);
      free (link_name);
      if (link_name == link_arg)
        link_arg = link_name = new_link_name;
      else
        link_arg = (link_name = new_link_name) + 2;
    }

  if (exists_as_file (link_name))
    {
      if (!force)
	{
	  fprintf (stderr, "ln: '%s': File exists\n", link_arg);
	  goto err;
	}
      else
	{
	  /* If the link already exists then make sure we don't follow it
	     and overwrite the target instead by mistake.  */
	  remove (link_name);
	}
    }

  target_len = strlen (target);
  link_file = fopen (link_name, "w");
  if (link_file == NULL
      || fwrite (LINK, 4, 1, link_file) != 1
      || fputc ((target_len >>  0) & 0xFF, link_file) == EOF
      || fputc ((target_len >>  8) & 0xFF, link_file) == EOF
      || fputc ((target_len >> 16) & 0xFF, link_file) == EOF
      || fputc ((target_len >> 24) & 0xFF, link_file) == EOF
      || fwrite (target, target_len, 1, link_file) != 1)
    {
      fprintf (stderr, "ln: Unable to create link '%s'\n", link_arg);
      goto err;
    }

  fclose (link_file);
  link_file = NULL;

  settype (link_name);

  free (link_name);

  return EXIT_SUCCESS;

err:
  free (link_name);
  if (link_file)
    fclose (link_file);

  return EXIT_FAILURE;
}
