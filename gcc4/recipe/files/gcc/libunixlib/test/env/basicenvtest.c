/* Basic test for setenv(), unsetenv() and putenv().  */

#include <stdlib.h>
#include <stdio.h>

static
void syntax (void)
{
  fprintf (stderr, "Syntax: basicenvtest setenv <name> <value> 0|1\n"
                   "        basicenvtest unsetenv <name>\n"
                   "        basicenvtest putenv <putenv argument>\n");
  exit (EXIT_FAILURE);
}

static void
do_setenv (const char *name, const char *value, const char *replace)
{
  int repl;
  if (replace[0] == '0' && replace[1] == '\0')
    repl = 0;
  else if (replace[0] == '1' && replace[1] == '\0')
    repl = 1;
  else
    syntax ();

  printf ("setenv (%s, %s, %d) = %d\n", name, value, repl,
          setenv (name, value, repl));
}

static void
do_unsetenv (const char *name)
{
  printf ("unsetenv (%s) = %d\n", name, unsetenv (name));
}

static void
do_putenv (char *name)
{
  printf ("putenv (%s) = %d\n", name, putenv (name));
}

int
main (int argc, char *argv[])
{
  if (argc < 2)
    syntax ();
  else if (!strcmp (argv[1], "setenv") && argc == 5)
    do_setenv (argv[2], argv[3], argv[4]);
  else if (!strcmp (argv[1], "unsetenv") && argc == 3)
    do_unsetenv (argv[2]);
  else if (!strcmp (argv[1], "putenv") && argc == 3)
    do_putenv (argv[2]);
  else
    syntax ();

  return EXIT_SUCCESS;
}

