/* Paranoid error function.
   Copyright (c) 2004, 2005, 2006, 2007, 2008 UnixLib Developers.  */

#include <signal.h>

#include <internal/unix.h>
#include <internal/os.h>

/* Don't use debug_printf () here as we don't want to pull in dvsprintf.c
   routines in static builds.  */
int
__funcall_error (const char *file, int line, unsigned int addr)
{
  char buf[11];
  char *s = buf + sizeof(buf);

  __os_print ("fatal error: Attempt to call a function at address below 0x");
  __os_prhex ((unsigned int) __ul_memory.robase);
  __os_print (", or higher than ");
  __os_prhex ((unsigned int) __ul_memory.appspace_himem);
  __os_print (" or a function at a non word-aligned address in a UnixLib function.\r\nFile: ");
  __os_print (file);
  __os_print (", line: ");
  do
    {
      *--s = (line % 10) + '0';
      line /= 10;
    }
  while (line != 0);
  __os_print (s);
  __os_print (", function address: 0x");
  __os_prhex (addr);
  __os_nl ();

  raise (SIGABRT);
  return 0;
}
