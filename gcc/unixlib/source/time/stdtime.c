/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/stdtime,v $
 * $Date: 1997/10/09 20:00:46 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stdtime,v 1.3 1997/10/09 20:00:46 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 13 July 1997.  */

#include <time.h>
#include <locale.h>
#include <sys/os.h>
#include <sys/swis.h>
#include <unixlib/local.h>

/* Common function for ctime() and asctime().  */
char *
__standard_time (const char *riscos_time)
{
  static char result[64];
  char *p;
  int regs[10];

  /* Standard time format is: "Tue May 21 13:46:22 1992\n"  */

  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)result;
  regs[3] = sizeof (result);
  regs[4] = (int)"%W3 %M3 %DY %24:%MI:%SE %CE%YR";
  os_swi (Territory_ConvertDateAndTime, regs);
  p = (char *)regs[1];
  *p++ = '\n';
  *p++ = '\0';

  return result;
}
