#include <stdio.h>

int main (int argc, char *argv[], char *env[])
{
  int x;

  fprintf (stdout, "goodbye stdout world\n");
  fprintf (stderr, "goodbye stderr world\n");

  printf ("argc = %d, argv = %x, env = %x\n",
  	  argc, (int) argv, (int) env);
  for (x = 0; x < argc; x++)
    printf ("argv[%d] = '%s'\n", x, argv[x]);

  return 0;
}
