/* iconv_open (), iconv (), iconv_close ()
 * Written by Peter Naulls
 * Copyright (c) 2004-2011 UnixLib Developers
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

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_Iconv_Open (const char *__tocode, const char *__fromcode, iconv_t *resultp)
{
  register const char *tocode __asm ("r0") = __tocode;
  register const char *fromcode __asm ("r1") = __fromcode;
  register const _kernel_oserror *err __asm ("r0");
  register iconv_t result __asm ("r1");
  __asm__ volatile ("SWI\t%[SWI_XIconv_Open]\n\t"
		    "MOV\tr1, r0\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (result)
		    : "r" (tocode), "r" (fromcode),
		      [SWI_XIconv_Open] "i" (Iconv_Open | (1<<17))
		    : "r14", "cc");
  *resultp = result;
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_Iconv_Iconv (iconv_t __cd, char **__inbuf, size_t *__inbytesleft,
		 char **__outbuf, size_t *__outbytesleft, size_t *resultp)
{
  register iconv_t cd __asm ("r0") = __cd;
  register char **inbuf __asm ("r1") = __inbuf;
  register size_t *inbytesleft __asm ("r2") = __inbytesleft;
  register char **outbuf __asm ("r3") = __outbuf;
  register size_t *outbytesleft __asm ("r4") = __outbytesleft;
  register const _kernel_oserror *err __asm ("r0");
  register size_t result __asm ("r1");
  __asm__ volatile ("SWI\t%[SWI_XIconv_Iconv]\n\t"
		    "MOV\tr1, r0\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (result)
		    : "r" (cd), "r" (inbuf), "r" (inbytesleft), "r" (outbuf),
		      "r" (outbytesleft), [SWI_XIconv_Iconv] "i" (Iconv_Iconv | (1<<17))
		    : "r14", "cc", "memory");
  *resultp = result;
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_Iconv_Close (iconv_t __cd)
{
  register iconv_t cd __asm ("r0") = __cd;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XIconv_Close]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (cd), [SWI_XIconv_Close] "i" (Iconv_Close | (1<<17))
		    : "r14", "cc");
  return err;
}

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
  err = SWI_OS_CLI ("RMEnsure Iconv 0.04 RMload System:Modules.Iconv");
  if (err)
    return (iconv_t) __ul_seterr (err, EOPSYS);

  err = SWI_OS_CLI ("RMEnsure Iconv 0.04 Error 16_10F iconv support requires the Iconv module 0.04 or newer");
  if (err)
    return (iconv_t) __ul_seterr (err, EOPSYS);

  iconv_t result;
  if ((err = SWI_Iconv_Open (tocode, fromcode, &result)) != NULL)
    return (iconv_t) iconv_error (err);

  return result;
}


size_t
iconv (iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf,
       size_t *outbytesleft)
{
#ifndef __TARGET_SCL__
  PTHREAD_SAFE_CANCELLATION
#endif

  const _kernel_oserror *err;
  size_t result;
  if ((err = SWI_Iconv_Iconv (cd, inbuf, inbytesleft, outbuf, outbytesleft,
			      &result)) != NULL)
    return (size_t) iconv_error (err);
  return result;
}


int
iconv_close (iconv_t cd)
{
#ifndef __TARGET_SCL__
  PTHREAD_SAFE_CANCELLATION
#endif

  const _kernel_oserror *err;
  return (err = SWI_Iconv_Close (cd)) != NULL ? iconv_error (err) : 0;
}
