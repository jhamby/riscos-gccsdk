/* libfile wrapper. Unix to RISC OS file name translation of the
   arguments. */

#include <stdio.h>
#include <unixlib/local.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
  int x;
  char buf[512];
  char *cmd = (char *) malloc (65536);

  strcpy (cmd, "$.bin.libfile1");
  for (x = 1; x < argc; x++)
    {
      __riscosify (argv[x], 0, 0, buf, sizeof (buf));
      strcat (cmd, " ");
      strcat (cmd, buf);
      /* printf ("%s ", nargv[x]); */
    }
  /* printf ("\n"); */
  execl ("*", "", cmd, 0);
  /* execve ("", nargv, environ); */
  return 0;
}
