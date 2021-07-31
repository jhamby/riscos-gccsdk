/* child.c
   Small executable to print one line to stdout and stderr, closes stdout and
   stderr and repeat printing one line to stdout and stderr.  This can be
   used to check redirection of the former two print lines.  The latter two
   print lines should never be seen in redirction nor in visual output.
   This program can be compiled with UnixLib and SCL.  */

#include <stdio.h>

int
main(int argc, char *argv[])
{
  fprintf(stdout, "line_via_stdout %p\n", stdout);
  fprintf(stderr, "line_via_stderr %p\n", stderr);

  fclose(stdout);
  fclose(stderr);

  fprintf(stdout, "this_stdout_line_may_not_be_seen %p\n", stdout);
  fprintf(stdout, "this_stderr_line_may_not_be_seen %p\n", stderr);

  return 0;
}

