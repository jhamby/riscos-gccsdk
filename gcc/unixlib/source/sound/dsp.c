/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sound/dsp.c,v $
 * $Date: 2004/09/07 17:49:40 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

/* Implement OSS/soundcard.h support */

#include <swis.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <unixlib/os.h>
#include <unixlib/dev.h>
#include <unixlib/swiparams.h>


#define IGNORE(x) {(void) x;}


#define BUFFER_SIZE 512
#define FRAGMENTS   4

static _kernel_oserror *set_defaults(int channels, int format, int size, int frequency)
{
  _kernel_swi_regs regs;

  regs.r[0] = channels;
  regs.r[1] = format;
  regs.r[2] = 10;
  regs.r[3] = size;
  regs.r[4] = 43;
  regs.r[5] = frequency;

  return _kernel_swi(DigitalRenderer_SetDefaults, &regs, &regs);
}


void *__dspopen (struct __unixlib_fd *file_desc, const char *file, int mode)
{
  int handle;
  _kernel_oserror *err;

  IGNORE(file_desc);
  IGNORE(file);
  IGNORE(mode);

  __os_cli("RMEnsure DigitalRenderer 0.51 RMload System:Modules.DRenderer");

  if ((err = set_defaults(2, 2, BUFFER_SIZE, 44100)) ||
      (err = __os_fopen (OSFILE_OPENOUT, "DRender:", &handle)))
    {
      __seterr (err);
      return (void *) -1;
    }

  return (void *)handle;
}


int __dspclose (struct __unixlib_fd *fd)
{
  _kernel_oserror *err = __os_fclose ((int) fd->handle);

  return (! err) ? 0 : (__seterr (err), -1);
}


int __dspioctl (struct __unixlib_fd *fd, unsigned long request, void *arg)
{
  switch (request & 0xffff)
    {
      case SNDCTL_DSP_RESET & 0xffff:
      case SNDCTL_DSP_SYNC & 0xffff:
        /* Do nothing for now */
        return 0;

      case SNDCTL_DSP_SPEED & 0xffff:
        {
          int speed = *((int *)arg);

          if (!set_defaults(0, 0, 0, speed))
            return 0;
        }

      case SNDCTL_DSP_SETFMT & 0xffff:
        {
          int format = *((int *)arg);

          /* DigitalRenderer supports 8-bit ulaw and 16-bit signed linear */
          if (format & (AFMT_MU_LAW | AFMT_S16_LE))
            if (!set_defaults(0, (format == AFMT_MU_LAW) ? 1 : 2, 0, 0))
              return 0;
        }

      case SNDCTL_DSP_STEREO & 0xffff:
        {
          int format = *((int *)arg);

          if (!set_defaults((format == 1) ? 2 : 1, 0, 0, 0))
            return 0;
        }

      case SNDCTL_DSP_CHANNELS & 0xffff:
        {
          int channels = *((int *)arg);

          if (!set_defaults(channels, 0, 0, 0))
            return 0;
        }

      case SNDCTL_DSP_SETFRAGMENT & 0xffff:
        return 0;

      case SNDCTL_DSP_GETFMTS & 0xffff:
        *((int *)arg) = AFMT_MU_LAW | AFMT_S16_LE;
        return 0;

      case SNDCTL_DSP_GETOSPACE & 0xffff:
        {
          struct audio_buf_info *info = arg;

          info->fragments  = FRAGMENTS;
          info->fragstotal = FRAGMENTS;
          info->fragsize   = BUFFER_SIZE;
          info->bytes      = BUFFER_SIZE * FRAGMENTS;

          return 0;
        }

      case SNDCTL_DSP_GETCAPS & 0xffff:
        *((int *)arg) = 0; /*DSP_CAP_REALTIME;*/
        return 0;

      case SNDCTL_DSP_GETOPTR & 0xffff:
        {
          struct count_info *info = arg;

          memset(info, 0, sizeof(struct count_info));
          return 0;
        }
    }

  return __set_errno (EINVAL);
}

