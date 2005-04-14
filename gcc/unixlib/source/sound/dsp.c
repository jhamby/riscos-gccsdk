/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sound/dsp.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

/* Implement OSS/soundcard.h support
 * Written by Peter Naulls <peter@chocky.org>
 *
 * This is essentially an emulation of the /dev/dsp device found under Linux
 * and other systems and the OSS interface.
 *
 * To achieve this, we make use of the DigitalRenderer module, which we feed
 * sample data we're given via the device write function.  Originally, we
 * used the DRender: device provided by the module, but that proved problematic
 * because it would not start playing until its buffer was full, and too
 * small buffers meant playing glitches.  Using the streaming interface
 * is much more responsive.
 *
 * We support 16-bit and 8-bit ulaw in stereo and mono.
 *
 * n.b. I don't believe that 8-bit currently works properly.
 */

#include <swis.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <unixlib/os.h>
#include <unixlib/dev.h>
#include <internal/swiparams.h>
#include <pthread.h>

#define IGNORE(x) {(void) x;}

#define DRENDERER_BUFFER_SIZE 512
#define DRENDERER_CSEC_TO_BUFFER 500


/* If we've called atexit */
static int handler_installed = 0;
/* If we're currently registered with DigitalRenderer */
static int dr_registered = 0;

static int dr_channels  = 2;
static int dr_format    = AFMT_S16_LE;
static int dr_frequency = 44100;

static int dr_buffers;     /* Number of buffers in use */
static int dr_fragscale  = 1;


/* Close /dev/dsp upon exit if it's still open.  This is mostly
   to ensure that DigitalRenderer is in a good state after
   an Alt-Break or an exception */


void dsp_exit(void)
{
  if (dr_registered)
    {
       _kernel_swi_regs regs;

       regs.r[0] = 0;
      _kernel_swi(DigitalRenderer_NumBuffers, &regs, &regs);
      _kernel_swi(DigitalRenderer_Deactivate, &regs, &regs);
    }
}


static _kernel_oserror *set_defaults(struct __unixlib_fd *fd, int channels, int format, int frequency, int buffers)
{
  _kernel_swi_regs regs;
  _kernel_oserror *err;

  if (channels)  dr_channels  = channels;
  if (frequency) dr_frequency = frequency;
  if (format)    dr_format    = format;

  /* It seems it's neccessary to remove our handler and readd it to change some
     values - I think there is a bug in RISC OS 5 regarding this.  */

  if (!handler_installed)
    {
      handler_installed = 1;
      atexit(dsp_exit);
    }
  else
    {
     _kernel_swi(DigitalRenderer_Deactivate, &regs, &regs);
    }

  dr_registered = 0;

  if (buffers)
    dr_buffers = buffers;
  else
    dr_buffers = (int)((double)DRENDERER_CSEC_TO_BUFFER / ((double)DRENDERER_BUFFER_SIZE * 100.0 / dr_frequency)) + 1;

  regs.r[0] = dr_buffers;
  if ((err = _kernel_swi(DigitalRenderer_NumBuffers, &regs, &regs)))
    return err;

  /* Set to fill with zero when out of data */
  regs.r[0] = 1;
  regs.r[1] = ~1;
  _kernel_swi(DigitalRenderer_StreamFlags, &regs, &regs);

  if (dr_format == AFMT_S16_LE)
    {
      regs.r[0] = dr_channels;
      regs.r[1] = DRENDERER_BUFFER_SIZE;
      regs.r[2] = dr_frequency;
      regs.r[3] = 1;

      if ((err = _kernel_swi(DigitalRenderer_Activate16, &regs, &regs)))
        return err;

     if (!_kernel_swi(DigitalRenderer_GetFrequency, &regs, &regs))
       dr_frequency = regs.r[0];

    }
  else
    {
      regs.r[0] = dr_channels;
      regs.r[1] = DRENDERER_BUFFER_SIZE;
      /* This is probably not correct */
      regs.r[2] = dr_frequency / 1000;
      regs.r[3] = 0;

      if ((err = _kernel_swi(DigitalRenderer_Activate, &regs, &regs)))
        return err;
    }


  dr_registered = 1;

  return NULL;
}


void *__dspopen (struct __unixlib_fd *fd, const char *file, int mode)
{
  _kernel_oserror *err;

  IGNORE(file);
  IGNORE(mode);

  dr_fragscale = 1;

  if ((err = __os_cli("RMEnsure DigitalRenderer 0.51 RMLoad System:Modules.DRenderer")) != NULL
      || (err = __os_cli("RMEnsure DigitalRenderer 0.51 Error 16_10F Sound support requires DigitalRenderer 0.51 or newer")) != NULL
      || (err = set_defaults(fd, 2, 2, 44100, 0)) != NULL)
    {
      __seterr (err);
      return (void *) -1;
    }

  return (void *)1; /* Dummy value */
}


int __dspclose (struct __unixlib_fd *fd)
{
  _kernel_oserror *err = NULL;

  if (fd->devicehandle->handle) {
    _kernel_swi_regs regs;

    regs.r[0] = 0;
    if ((err = _kernel_swi(DigitalRenderer_NumBuffers, &regs, &regs)) ||
        (err = _kernel_swi(DigitalRenderer_Deactivate, &regs, &regs)))
      __seterr (err);
    else
      dr_registered = 0;
  }

  return err ? -1 : 0;
}


__off_t __dsplseek (struct __unixlib_fd *fd, __off_t lpos, int whence)
{
   /* Do nothing, just so programs don't complain */

  IGNORE(fd);
  IGNORE(lpos);
  IGNORE(whence);

  return 0;
}


int __dspwrite (struct __unixlib_fd *fd, const void *data, int nbyte)
{
  _kernel_oserror *err;
  _kernel_swi_regs regs;
  int left = nbyte;

  while (left > 0)
    {
      int towrite = (left < DRENDERER_BUFFER_SIZE) ? left : DRENDERER_BUFFER_SIZE;

      do {
        if ((err = _kernel_swi(DigitalRenderer_StreamStatistics, &regs, &regs)))
          {
            __seterr (err);
            return -1;
          }
    
        if (regs.r[0] < dr_buffers)
          break;
    
        __pthread_enable_ints();
        pthread_yield();
        __pthread_disable_ints();
      } while (1);
    
      regs.r[0] = (int)data + nbyte - left;
      regs.r[1] = towrite / dr_channels;

      if ((err = _kernel_swi((dr_format == AFMT_S16_LE) ? DigitalRenderer_Stream16BitSamples : DigitalRenderer_StreamSamples,
           &regs, &regs)))
        {
          __seterr (err);
          return -1;
        }

      left -= DRENDERER_BUFFER_SIZE;
    }

  return nbyte;
}


int __dspioctl (struct __unixlib_fd *fd, unsigned long request, void *arg)
{
  request &= 0xffff;

  /* Do nothing */
  if (request == (SNDCTL_DSP_RESET & 0xffff) || request == (SNDCTL_DSP_SYNC & 0xffff))
    return 0;

  if (!arg)
    return __set_errno (EINVAL);

  switch (request)
    {
      case SNDCTL_DSP_SPEED & 0xffff:
        {
          int speed = *((int *)arg);

          if (!set_defaults(fd, 0, 0, speed, 0))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_SETFMT & 0xffff:
        {
          int format = *((int *)arg);

          /* DigitalRenderer supports 8-bit ulaw and 16-bit signed linear */
          if (format == AFMT_MU_LAW || format == AFMT_S16_LE)
            if (!set_defaults(fd, 0, format, 0, 0))
              return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_STEREO & 0xffff:
        {
          int stereo = *((int *)arg);

          if (!set_defaults(fd, (stereo == 1) ? 2 : 1, 0, 0, 0))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_GETBLKSIZE & 0xffff:
        *((int *)arg) = DRENDERER_BUFFER_SIZE;
        return 0;

      case SNDCTL_DSP_CHANNELS & 0xffff:
        {
          int channels = *((int *)arg);

          if (!set_defaults(fd, channels, 0, 0, 0))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_SETFRAGMENT & 0xffff:
        {
          int fragspec  = *((int *)arg);
          int fragments = fragspec >> 16;
          int fragsize  = 1 << (fragspec & 0xffff);

          dr_fragscale = (fragsize / DRENDERER_BUFFER_SIZE);
          if (dr_fragscale == 0) dr_fragscale = 1;
          fragments *= dr_fragscale;

          if (!set_defaults(fd, 0, 0, 0, fragments))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_GETFMTS & 0xffff:
        *((int *)arg) = AFMT_MU_LAW | AFMT_S16_LE;
        return 0;

      case SNDCTL_DSP_GETOSPACE & 0xffff:
        {
          struct audio_buf_info *info = arg;
          _kernel_swi_regs regs;

           _kernel_swi(DigitalRenderer_StreamStatistics, &regs, &regs);

          info->fragments  = (dr_buffers - regs.r[0]) / dr_fragscale;
          info->fragstotal = dr_buffers / dr_fragscale;
          info->fragsize   = DRENDERER_BUFFER_SIZE * dr_fragscale;
          info->bytes      = DRENDERER_BUFFER_SIZE * dr_fragscale * dr_buffers;

          return 0;
        }

      case SNDCTL_DSP_GETCAPS & 0xffff:
        *((int *)arg) = 0; /*DSP_CAP_REALTIME;*/
        return 0;

      case SNDCTL_DSP_GETOPTR & 0xffff:
        {
          struct count_info *info = arg;

          /* TODO: add these values */
          info->bytes   = 0;
          info->blocks  = 0;
          info->ptr     = 0;
          return 0;
        }
    }

  return __set_errno (EINVAL);
}

