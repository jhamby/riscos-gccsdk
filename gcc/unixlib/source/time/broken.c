/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/broken.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: broken.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <locale.h>
#include <sys/os.h>
#include <sys/swis.h>
#include <unixlib/local.h>

/* Convert broken time to 5-byte RISC OS time.  */
void
__cvt_broken_time (const struct tm *brokentime, char *riscos_time)
{
  unsigned int ordinals_buffer[15];
  int regs[10];

  ordinals_buffer[0] = 0;
  ordinals_buffer[1] = brokentime->tm_sec;
  ordinals_buffer[2] = brokentime->tm_min;
  ordinals_buffer[3] = brokentime->tm_hour;
  ordinals_buffer[4] = brokentime->tm_mday;
  ordinals_buffer[5] = brokentime->tm_mon + 1;
  ordinals_buffer[6] = brokentime->tm_year + 1900;
  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)ordinals_buffer;
  os_swi (Territory_ConvertOrdinalsToTime, regs);
}
