/****************************************************************************
 *
 * $Source: $
 * $Date: $
 * $Revision: $
 * $State: $
 * $Author: $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: $";
#endif

#include <stdlib.h>

#include <swis.h>
#include <iconv.h>
#include <unixlib/os.h>

iconv_t iconv_open(const char *tocode, const char *fromcode)
{
  iconv_t ret;
  _kernel_oserror *err;

  if ((err = __os_cli("RMEnsure Iconv 0.01 RMload System:Modules.Iconv")) != NULL
      || (err = __os_cli("RMEnsure Iconv 0.01 Error iconv support requires the Iconv module 0.01 or newer")) != NULL
      || (err = _swix(Iconv_Open, _INR(0,1) | _OUT(0), tocode, fromcode, &ret)) != NULL)
    {
      __seterr (err);
      return (iconv_t)(-1);
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
      __seterr (err);
      return (size_t)(-1);
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
      __seterr (err);
      return -1;
    }

  return ret;
}
