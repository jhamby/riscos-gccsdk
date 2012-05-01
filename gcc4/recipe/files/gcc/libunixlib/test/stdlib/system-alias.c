/* Tests if the alias-disabling % is supported in system().
   Output should be something like (assuming the is an executable
   ls available) :
   
--------------------------------
Tue,01 May 2012.14:19:05
--------------------------------
File1                   File2            File3
--------------------------------

 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int
main (void)
{
  system ("Alias Time echo XXXXXXXXXX");
  system ("Alias ls   echo XXXXXXXXXX");
  printf ("--------------------------------\n");
  int r;
  if ((r = system ("%Time")) != 0)
    printf ("system() = %d, errno %d, %s\n", r, errno, strerror (errno));
  printf ("--------------------------------\n");
  if ((r = system ("%ls")) != 0)
    printf ("system() = %d, errno %d, %s\n", r, errno, strerror (errno));
  printf ("--------------------------------\n");
  system ("UnAlias Time");
  system ("UnAlias ls");
  return 0;
}
