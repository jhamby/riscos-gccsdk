/* getcwd.c - test the getcwd functions.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int
dir (const char *directory)
{
  char buffer[512];
  char *s;
  size_t len;

  if (directory != NULL)
    {
      printf ("-- chdir  '%s'\n", directory);
      chdir (directory);
    }
  s = getcwd (buffer, sizeof (buffer));
  if (s == NULL)
    {
      printf ("-- getcwd returned NULL (it shouldn't): %s\n", strerror (errno));
      return EXIT_FAILURE;
    }
  printf ("-- getcwd '%s'\n", buffer);

  len = strlen (buffer) + 1;
  /* Linux extension.  */
  s = getcwd (NULL, len);
  if (s == NULL)
    {
      printf ("-- getcwd returned NULL (it shouldn't): %s\n", strerror (errno));
      return EXIT_FAILURE;
    }
  printf ("-- getcwd '%s'\n", buffer);
  free (s);

  s = getcwd (NULL, 0);
  if (s == NULL)
    {
      printf ("-- getcwd returned NULL (it shouldn't): %s\n", strerror (errno));
      return EXIT_FAILURE;
    }
  printf ("-- getcwd '%s'\n", buffer);
  free (s);

  return EXIT_SUCCESS;
}

int
main (void)
{
  if (dir (NULL) != EXIT_SUCCESS
      || dir ("$") != EXIT_SUCCESS
      || dir ("$.arm-riscos-aof") != EXIT_SUCCESS
      || dir ("$.arm-riscos-aof.gcc-2_95_2.gcc.cp") != EXIT_SUCCESS
      || dir ("$.gcc-2_95_2.gcc") != EXIT_SUCCESS)
    return EXIT_FAILURE;

  printf ("ok\n");
  return EXIT_SUCCESS;
}

