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
 * resolving a symlink, the prescence of a colon is assumed to indicate an
 * absolute path. Symlinks can link to each other forming a chain upto a depth of
 * 64. This is an arbitrary limit, but some limit was required in case a chain of
 * links pointed back to itself creating an otherwise infinite loop.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef CROSS_COMPILE
#include <kernel.h>
#endif

#define	LINK_FILETYPE	0x1C8

static const char const LINK[] = "LINK";

static unsigned int symbolic = 0;
static unsigned int force = 0;
static unsigned int verbose = 0;

static void settype(const char *file)
{
#ifdef CROSS_COMPILE
_kernel_swi_regs regs;

  regs.r[0] = 18;
  regs.r[1] = (int)file;
  regs.r[2] = LINK_FILETYPE;
  _kernel_swi(0x8, &regs, &regs);
#endif
}

/*
  Return 1 if objects exists otherwise 0.
*/
static int exists(const char *file)
{
FILE *f;
int res = 0;

  if ((f = fopen(file, "r")) != NULL)
  {
    res = 1;
    fclose(f);
  }

  return res;
}

static void help(void)
{
  printf("Usage: ln [OPTION]... TARGET [LINK_NAME]\n"
	 "Create a link to the specified TARGET with optional LINK_NAME.\n"
	 "If LINK_NAME is omitted, a link with the same basename as the TARGET is\n"
	 "created in the current directory. Create hard links by default, symbolic\n"
	 "links with --symbolic. When creating hard links, TARGET must exist.\n"
	 "\n"
	 "  -f, --force                 remove existing destination files\n"
	 "  -s, --symbolic              make symbolic links instead of hard links\n"
	 "  -v, --verbose               print name of each file before linking\n"
	 "      --help                  display this help and exit\n"
	 "      --version               output version information and exit\n");
}

static void version(void)
{
  printf("ln v1.01\n"
	 "Written by Lee Noar\n");
}

static void invalid_option(const char *option)
{
  printf("ln: unrecognised option '%s'\n",option);
  printf("Try 'ln --help' for more information.\n");
}

int main(int argc, char *argv[])
{
int i, target_len;
FILE *link_file = NULL;
char *target = NULL, *link_name = NULL, *target_arg = NULL, *link_arg = NULL;

  for (i = 1; i < argc; i++)
  {
    if (*(argv[i]) == '-')
    {
      if (strcmp(argv[i] + 1, "s") == 0 || strcmp(argv[i] + 1, "-symbolic") == 0)
        symbolic = 1;
      else if (strcmp(argv[i] + 1, "f") == 0 || strcmp(argv[i] + 1, "-force") == 0)
        force = 1;
      else if (strcmp(argv[i] + 1, "v") == 0 || strcmp(argv[i] + 1, "-verbose") == 0)
        verbose = 1;
      else if (strcmp(argv[i] + 1, "-help") == 0)
      {
        help();
        return 0;
      }
      else if (strcmp(argv[i] + 1, "-version") == 0)
      {
        version();
        return 0;
      }
      else
      {
        invalid_option(argv[i]);
        return 0;
      }
    }
    else if (target_arg == NULL)
      target_arg = argv[i];
    else
    {
      link_name = link_arg = (char *)malloc(strlen(argv[i]) + 1);
      if (!link_name)
      {
        printf("ln: Out of memory\n");
        goto err;
      }
      strcpy(link_name, argv[i]);
    }
  }

  if (!target_arg)
  {
    printf("ln: Missing file argument\n");
    goto err;
  }

  target = target_arg;

  if (!link_name)
  {
  char *sep;

    sep = strrchr(target, '.');
    if (!sep)
      sep = strrchr(target, ':');
    if (sep)
      sep++;
    else
    {
      printf("ln: Unable to determine LINK_NAME\n");
      goto err;
    }
    link_name = (char *)malloc(strlen(sep) + 3);
    if (!link_name)
    {
      printf("ln: Out of memory\n");
      goto err;;
    }

    /* Make sure riscosify knows that this is a RISC OS name */
    strcpy(link_name, "@.");
    strcat(link_name, sep);
    link_arg = link_name + 2;
  }

  if (verbose)
  {
    if (symbolic)
      printf("create symbolic link '%s' to '%s'\n", link_name, target);
    else
      printf("create hard link '%s' to '%s'\n", link_name, target);
  }

  if (strcmp(link_name, target) == 0)
  {
    printf("ln: TARGET and LINK_NAME cannot be the same\n");
    goto err;
  }

  if (!symbolic && !exists(target))
  {
    printf("ln: '%s': No such file or directory\n",target);
    goto err;
  }

  if (exists(link_name))
  {
    if (!force)
    {
      printf("ln: '%s': File exists\n", link_arg);
      goto err;
    }
    else
    {
      /* If the link already exists then make sure we don't follow it
         and overwrite the target instead by mistake */
      remove(link_name);
    }
  }

  link_file = fopen(link_name, "wb");
  if (!link_file)
  {
    printf("ln: Unable to create link '%s'\n",link_arg);
    goto err;
  }

  fwrite(LINK, 1, 4, link_file);

  target_len = strlen(target);
  fwrite(&target_len, 4, 1, link_file);
  fwrite(target, 1, target_len, link_file);
  fclose(link_file);

  settype(link_name);

  free(link_name);

  return 0;

err:
  if (link_name)
    free(link_name);
  if (link_file)
    fclose(link_file);

  return 1;
}
