/* parent.c
   Simple program to use its first argument as new process to start and catch its
   output and show it to its stdout after that child process has died.  This
   excerises the pipe()/dup()/OS redirection/etc.  You should have output like:

   Executing an UnixLib ELF child binary:
     *parent_ul child_ul_elf
     vvv BEGIN child, no output may appear from this point...
     ^^^ END child, ...until here.
     vvv BEGIN piped output of child, following should be non-empty.
     line_via_stdout 0x3a240
     line_via_stderr 0x3b2a8
     ^^^ END piped output of child.

   Executing an UnixLib Absolute child binary:
     *parent_ul child_ul_abs
     vvv BEGIN child, no output may appear from this point...
     ^^^ END child, ...until here.
     vvv BEGIN piped output of child, following should be non-empty.
     line_via_stdout 0x2e1c8
     line_via_stderr 0x2f230
     ^^^ END piped output of child.

   Executing a SharedCLibrary Absolute child binary:
     *parent_ul child_scl
     vvv BEGIN child, no output may appear from this point...
     ^^^ END child, ...until here.
     vvv BEGIN piped output of child, following should be non-empty.
     line_via_stdout 00009084
     line_via_stderr 000090ac
     ^^^ END piped output of child

   Executing an OSCLI command as child:
     *parent_ul help
     vvv BEGIN child, no output may appear from this point...
     ^^^ END child, ...until here.
     vvv BEGIN piped output of child, following should be non-empty.
     ==> Help on keyword Help (OSCommands)
     *Help <subjects> attempts to give useful information on the selected topics. Special keywords include:
     Commands        List all the available utility commands
     FileCommands    List all the filing system-specific commands
     Modules         List the module titles
     Syntax          Explain the syntax message format
     ^^^ END piped output of child.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define __STRINGIFY__(s) #s
#define STRINGIFY(s) __STRINGIFY__(s) 

static void
error (const char *err)
{
  fprintf(stderr, "%s", err);
  exit(EXIT_FAILURE);
}

static void
exec_command (char **argv)
{
  /* Run the program.  */
  execvp (argv[0], argv);

  if (errno)
    perror(__FILE__ ":" STRINGIFY(__LINE__));

  _exit (127);
}

static void
child_execute_job (int stdin_fd, int stdout_fd, char **argv)
{
  if (stdin_fd != 0)
    (void) dup2 (stdin_fd, 0);
  if (stdout_fd != 1)
    (void) dup2 (stdout_fd, 1);
  if (stdin_fd != 0)
    (void) close (stdin_fd);
  if (stdout_fd != 1)
    (void) close (stdout_fd);

  /* Run the command.  */
  exec_command (argv);
}

int
main (int argc, char *argv[])
{
  int pipedes[2];
  int pid;

  if (argc != 2)
    {
      fprintf (stderr, "Syntax: %s <child exe>\n", argv[0]);
      return EXIT_FAILURE;
    }

  if (pipe (pipedes) < 0)
    {
      perror(__FILE__ ":" STRINGIFY(__LINE__));
      return EXIT_FAILURE;
    }

  pid = vfork ();
  if (pid < 0)
    perror(__FILE__ ":" STRINGIFY(__LINE__));
  else if (pid == 0)
    {
      printf ("vvv BEGIN child, no output may appear from this point...\n");
      child_execute_job (0, pipedes[1], &argv[1]);
    }
  else
    {
      /* We are the parent.  */

      /* Close the write side of the pipe.  */
      (void) close (pipedes[1]);

      printf ("^^^ END child, ...until here.\n");

      printf ("vvv BEGIN piped output of child, following should be non-empty.\n");
      /* Set up and read from the pipe.  */
      while (1)
        {
          char b;
          int i = read (pipedes[0], &b, 1);
          if (i <= 0)
            {
              if (errno != 0)
                perror(__FILE__ ":" STRINGIFY(__LINE__));
              break;
            }
          if (1)
            fputc (b, stdout);
          else
            printf("Read byte: 0x%x %c\n", b, b >= 32 ? b : '.');
        }
      printf ("^^^ END piped output of child.\n");

      /* Close the read side of the pipe.  */
      (void) close (pipedes[0]);
    }

  return EXIT_SUCCESS;
}

