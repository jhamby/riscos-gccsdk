/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/iconv.c,v $
 * $Date: 2005/04/19 08:50:36 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <stdlib.h>

#include <swis.h>
#include <errno.h>
#include <iconv.h>
#include <unixlib/os.h>
#include <pthread.h>

#define ERROR_BASE 0x81b900

#define ICONV_NOMEM (ERROR_BASE+0)
#define ICONV_INVAL (ERROR_BASE+1)
#define ICONV_2BIG  (ERROR_BASE+2)
#define ICONV_ILSEQ (ERROR_BASE+3)


static int iconv_error (_kernel_oserror *err)
{
  __ul_seterr (err, 0);

  switch (err->errnum)
    {
    case ICONV_NOMEM:
      errno = ENOMEM;
      break;

    case ICONV_INVAL:
      errno = EINVAL;
      break;

    case ICONV_2BIG:
      errno = E2BIG;
      break;

    case ICONV_ILSEQ:
      errno = EILSEQ;
      break;

    default:
      errno = EINVAL;
      break;
    }
  
  return -1;
}


iconv_t iconv_open (const char *tocode, const char *fromcode)
{
  iconv_t ret;
  _kernel_oserror *err;
  int regs[10];

  PTHREAD_SAFE_CANCELLATION

  err = __os_cli ("RMEnsure Iconv 0.04 RMload System:Modules.Iconv");
  if (err)
    {
      __ul_seterr (err, 1);
      return (iconv_t) -1;
    }

  err = __os_cli ("RMEnsure Iconv 0.04 Error 16_10F iconv support requires the Iconv module 0.04 or newer");
  if (err)
    {
      __ul_seterr (err, 1);
      return (iconv_t) -1;
    }

  regs[0] = (int) tocode;
  regs[1] = (int) fromcode;
  err = __os_swi (Iconv_Open, regs);
  if (err)
    return (iconv_t) iconv_error (err);

  return (iconv_t) regs[0];
}


size_t iconv (iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf,
	      size_t *outbytesleft)
{
  int regs[10];
  _kernel_oserror *err;

  PTHREAD_SAFE_CANCELLATION

  regs[0] = (int) cd;
  regs[1] = (int) inbuf;
  regs[2] = (int) inbytesleft;
  regs[3] = (int) outbuf;
  regs[4] = (int) outbytesleft;
  err = __os_swi (Iconv_Iconv, regs);
  if (err)
    return (size_t) iconv_error (err);
  return regs[0];
}


int iconv_close(iconv_t cd)
{
  int regs[10];
  _kernel_oserror *err;

  PTHREAD_SAFE_CANCELLATION

  regs[0] = (int) cd;
  err = __os_swi (Iconv_Close, regs);
  if (err)
    return iconv_error (err);

  return regs[0];
}
