/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/uname.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: uname.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
#endif

/* Ensure old names are visible.  */
#define __UNIXLIB_OLD_UNAME

#include <stdlib.h>
#include <sys/param.h>
#include <unixlib/local.h>

int __uname_control = __UNAME_DROP_VOWEL + __UNAME_LONG_TRUNC;

/* The name buffer. There is room for two full RISC OS names so that we can
   switch between the two buffers on alternate calls.  */
static char __unbuf[MAXPATHLEN << 1];

/* Convert a Unix format filename to RISC OS format.  If CFLAG is
   non-zero then missing subdirectories are automatically created.  */
char *
__uname (const char *name, int cflag)
{
  static char *_name = NULL;
  int flags = (__uname_control & __UNAME_LONG_TRUNC)
	      ? __RISCOSIFY_DONT_TRUNCATE : __RISCOSIFY_SHORT_TRUNCATE;

  if (! name)
    {
      __unbuf[0] = '\0';
      return __unbuf;
    }

  _name = (_name == __unbuf) ? (__unbuf + (sizeof(__unbuf)>>1)) : __unbuf;

  if (__uname_control & __UNAME_NO_PROCESS)
    flags |= __RISCOSIFY_NO_PROCESS;

  if (__uname_control & __UNAME_DROP_VOWEL)
    flags |= __RISCOSIFY_DROP_VOWEL;

  return __riscosify (name, cflag, flags, _name, MAXPATHLEN, NULL) ? _name : NULL;
}
