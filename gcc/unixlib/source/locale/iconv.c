/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/iconv.c,v $
 * $Date: 2004/10/05 15:24:37 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: iconv.c,v 1.3 2004/10/05 15:24:37 peter Exp $";
#endif

#include <stdlib.h>

#include <swis.h>
#include <iconv.h>
#include <unixlib/os.h>

#define ERROR_BASE 0x81b900

#define ICONV_NOMEM (ERROR_BASE+0)
#define ICONV_INVAL (ERROR_BASE+1)
#define ICONV_2BIG  (ERROR_BASE+2)
#define ICONV_ILSEQ (ERROR_BASE+3)


static int iconv_error(_kernel_oserror *err)
{
  __seterr (err);

  switch (err->errnum) {
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


iconv_t iconv_open(const char *tocode, const char *fromcode)
{
  iconv_t ret;
  _kernel_oserror *err;

  if ((err = __os_cli("RMEnsure Iconv 0.01 RMload System:Modules.Iconv")) != NULL
      || (err = __os_cli("RMEnsure Iconv 0.01 Error iconv support requires the Iconv module 0.01 or newer")) != NULL
      || (err = _swix(Iconv_Open, _INR(0,1) | _OUT(0), tocode, fromcode, &ret)) != NULL)
    {
      return (iconv_t)iconv_error(err);
    }

  return ret;
}


size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf,
           size_t *outbytesleft)
{
  size_t ret;
  _kernel_oserror *err;

  err = _swix(Iconv_Iconv, _INR(0,4) | _OUT(0), cd, inbuf, inbytesleft, outbuf, outbytesleft, &ret);
  if (err)
    {
      return (size_t)iconv_error(err);
    }

  return ret;
}


int iconv_close(iconv_t cd)
{
  int ret;
  _kernel_oserror *err;

  err = _swix(Iconv_Close, _IN(0) | _OUT(0), cd, &ret);
  if (err)
    {
      return iconv_error(err);
    }

  return ret;
}
