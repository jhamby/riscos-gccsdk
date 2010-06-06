/* iconv_open (), iconv (), iconv_close ()
 * Written by Peter Naulls
 * Copyright (c) 2004-2010 UnixLib Developers
 */

#include <errno.h>
#include <iconv.h>
#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <stdlib.h>
#include <swis.h>

#ifndef __TARGET_SCL__
#  include <internal/unix.h>
#endif
#include <internal/os.h>

#define ERROR_BASE 0x81b900

#define ICONV_NOMEM (ERROR_BASE+0)
#define ICONV_INVAL (ERROR_BASE+1)
#define ICONV_2BIG  (ERROR_BASE+2)
#define ICONV_ILSEQ (ERROR_BASE+3)


static int
iconv_error (const _kernel_oserror *err)
{
  int uerr;

  switch (err->errnum)
    {
    case ICONV_NOMEM:
      uerr = ENOMEM;
      break;

    case ICONV_INVAL:
      uerr = EINVAL;
      break;

    case ICONV_2BIG:
      uerr = E2BIG;
      break;

    case ICONV_ILSEQ:
      uerr = EILSEQ;
      break;

    default:
      uerr = EINVAL;
      break;
    }

  return __ul_seterr (err, uerr);
}


iconv_t
iconv_open (const char *tocode, const char *fromcode)
{
#ifndef __TARGET_SCL__
  PTHREAD_SAFE_CANCELLATION
#endif

  const _kernel_oserror *err;
  err = __os_cli ("RMEnsure Iconv 0.04 RMload System:Modules.Iconv");
  if (err)
    return (iconv_t) __ul_seterr (err, EOPSYS);

  err = __os_cli ("RMEnsure Iconv 0.04 Error 16_10F iconv support requires the Iconv module 0.04 or newer");
  if (err)
    return (iconv_t) __ul_seterr (err, EOPSYS);

  int regs[10];
  regs[0] = (int) tocode;
  regs[1] = (int) fromcode;
  err = __os_swi (Iconv_Open, regs);
  if (err)
    return (iconv_t) iconv_error (err);

  return (iconv_t) regs[0];
}


size_t
iconv (iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf,
       size_t *outbytesleft)
{
#ifndef __TARGET_SCL__
  PTHREAD_SAFE_CANCELLATION
#endif

  int regs[10];
  regs[0] = (int) cd;
  regs[1] = (int) inbuf;
  regs[2] = (int) inbytesleft;
  regs[3] = (int) outbuf;
  regs[4] = (int) outbytesleft;
  const _kernel_oserror *err = __os_swi (Iconv_Iconv, regs);
  if (err)
    return (size_t) iconv_error (err);
  return regs[0];
}


int
iconv_close (iconv_t cd)
{
#ifndef __TARGET_SCL__
  PTHREAD_SAFE_CANCELLATION
#endif

  int regs[10];
  regs[0] = (int) cd;
  const _kernel_oserror *err = __os_swi (Iconv_Close, regs);
  if (err)
    return iconv_error (err);

  return regs[0];
}
