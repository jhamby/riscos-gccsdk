/* Test program to check if environment variables are correctly inherited
   from parent -> child and if the migration from Unix environment variable
   -> RISC OS system variable is happening.  */

/* Output should be:

     *run ram:$.testenv
     ** I'm /RAM::RamDisc0.$/testenv
     -- get definition
     test: (null)
     test2: (null)
     my$test: (null)
     my$test2: (null)
     -- start definition
     -- sleep 2 secs

     ** I'm /RAM::RamDisc0.$/testenv
     -- get definition
     test: testval
     test2: someval
     my$test: testval
     my$test2: someval
     -- start definition
     -- sleep 2 secs

     ** I'm /RAM::RamDisc0.$/testenv
     -- get definition
     test: testval
     test2: someval
     my$test: testval
     my$test2: someval
     -- start definition
     -- sleep 2 secs

   Afterwards 'my$test' and 'my$test2' RISC OS system variables should be
   defined, so if you rerun in this stage the program again, you should
   have as output:

     *run ram:$.testenv
     ** I'm /RAM::RamDisc0.$/testenv
     -- get definition
     test: (null)
     test2: (null)
     my$test: testval
     my$test2: someval
     -- start definition
     -- sleep 2 secs

     ** I'm /RAM::RamDisc0.$/testenv
     -- get definition
     test: testval
     test2: someval
     my$test: testval
     my$test2: someval
     -- start definition
     -- sleep 2 secs

     ** I'm /RAM::RamDisc0.$/testenv
     -- get definition
     test: testval
     test2: someval
     my$test: testval
     my$test2: someval
     -- start definition
     -- sleep 2 secs

 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
main (int argc, char *argv[], char **env)
{
  char *eP, **ePP;

  printf ("** I'm %s\n", argv[0]);
  puts ("-- get definition");

  /* 'test' and 'test2' environment variables will be inherited from
     RISC OS and/or UnixLib parent program.  */
  printf ("test: %s\n", getenv ("test"));
  printf ("test2: %s\n", getenv ("test2"));

  /* Likewise for 'my$test' and 'mytest2'.  */
  printf ("my$test: %s\n", getenv ("my$test"));
  printf ("my$test2: %s\n", getenv ("my$test2"));

  /* Dump given environment:  */
  printf ("vvv main() environment: %p\n", env);
  if (env)
    for (ePP = env; (eP = *ePP) != NULL; ++ePP)
      printf ("Env: <%s>\n", eP);
  printf ("^^^\n");
  printf ("vvv 'environ' environment: %p (pointer value stored at %p)\n", environ, &environ);
  if (environ)
    for (ePP = environ; (eP = *ePP) != NULL; ++ePP)
      printf ("Env: <%s>\n", eP);
  printf ("^^^\n");

  puts ("-- start definition");

  /* Defining 'test' and 'test2' will stick to UnixLib environment.
     You won't see this definition as RISC OS system variable.  */
  setenv ("test", "testval", 1);
  putenv ("test2=someval");

  /* However, defining 'my$test' and 'my$test2'  (i.e. containing a '$')
     will make sure that those definitions also appear as RISC OS
     system variables.  */
  setenv ("my$test", "testval", 1);
  putenv ("my$test2=someval");

  puts ("-- sleep 2 secs");
  sleep (2);

  fputc ('\n', stdout);
#if 1
  execv ("./testenv", NULL);
#else
  execv (argv[0], NULL);
#endif

  return 0;
}
