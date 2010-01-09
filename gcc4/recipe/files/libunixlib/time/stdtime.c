/* __standard_time ()
 * Written by Nick Burrett on 13 July 1997.
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <time.h>
#include <locale.h>
#include <swis.h>

#include <internal/os.h>
#include <unixlib/local.h>

/* Common function for ctime() and asctime().  */
/* Not thread safe, but none of its callers need be either */
char *
__standard_time (const char *riscos_time, char *local_buffer)
{
  static char static_buffer[64];
  char *p, *result;
  int regs[10];

  result = local_buffer ? local_buffer : static_buffer;

  /* Standard time format is: "Tue May 21 13:46:22 1992\n"  */

  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)result;
  regs[3] =  local_buffer ? 26 : sizeof (static_buffer); /* 26 bytes according to ctime_r, asctime_r man page */
  regs[4] = (int)"%W3 %M3 %DY %24:%MI:%SE %CE%YR";
  __os_swi (Territory_ConvertDateAndTime, regs);
  p = (char *)regs[1];
  *p++ = '\n';
  *p = '\0';

  return result;
}
