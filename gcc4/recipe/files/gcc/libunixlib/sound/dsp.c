/*
 * Implement OSS/soundcard.h support
 *
 * Written by Peter Naulls <peter@chocky.org>
 * and Christopher Martin <belles@internode.on.net>
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
 * Copyright (c) 2004-2012 UnixLib Developers
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

#include <internal/os.h>
#include <internal/dev.h>
#include <internal/swiparams.h>
#include <pthread.h>

#include "DRender.h"

#define DRENDERER_BUFFER_SIZE 512
#define DRENDERER_CSEC_TO_BUFFER 500

static int dr_channels  = 2;
static int dr_format    = AFMT_S16_LE;
static int dr_frequency = 44100;

static int dr_buffers = 0;     /* Number of buffers in use */
static int dr_fragscale = 1;


/* Avoid warnings, but don't advertise */
void __dsp_exit (void);

/* Close /dev/dsp upon exit if it's still open.  This is mostly
   to ensure that DigitalRenderer is in a good state after
   an Alt-Break or an exception.  */
void
__dsp_exit (void)
{
  DRender_Deactivate ();
  DRender_NumBuffers (0);
}


static const _kernel_oserror *
check_state (int *state)
{
  if ((*state = DRender_ReadState ()) == -1)
    {
      const _kernel_oserror *err;
#if 0 /* Better to use OS_Module directly to try (re)loading the module */
      if (   (err = SWI_OS_CLI ("RMEnsure DigitalRenderer 0.55 "
                                "RMLoad System:Modules.DRenderer")) != NULL
          || (err = SWI_OS_CLI ("RMEnsure DigitalRenderer 0.55 Error 16_10F "
                                "Sound support requires DigitalRenderer 0.55 "
                                "or newer")) != NULL
         )
        return err;
#else
      if ((err = DRender_LoadModule (NULL)) != NULL)
        return err;
#endif
      *state = DRender_ReadState (); /* Is there the possibility of an error
                                        returning -1 at this moment? */
    }
  return NULL;
}


static const _kernel_oserror *
activate_defaults (void)
{
  const _kernel_oserror *err;
  if (dr_format == AFMT_S16_LE)
    {
      if ((err = DRender_Activate16 (dr_channels, DRENDERER_BUFFER_SIZE,
                                     dr_frequency, DRActivate_Restore)) == NULL)
        {
          /* The SWI calls in this block only work after DRender_Activate16 */
          int freq = DRender_GetFrequency ();
          if (freq > 0)
            dr_frequency = freq;
          /* Stereo samples intended for /dev/dsp will be in left-right order */
          DRender_SampleFormat (DRFORMAT_S16LR);
        }
    }
  else
    {
      err = DRender_Activate (dr_channels, DRENDERER_BUFFER_SIZE,
                              1e6 / dr_frequency, NULL);
    }
  return err;
}


static const _kernel_oserror *
set_defaults (struct __unixlib_fd *fd, int channels, int format, int frequency,
              int buffers)
{
  const _kernel_oserror *err;
  int old_state;

  if ((err = check_state (&old_state)) != NULL)
    return err;

  if ((old_state & DRState_Active) && (old_state != -1))
    DRender_Deactivate ();

  if (channels)
    dr_channels = channels;
  if (frequency)
    dr_frequency = frequency;
  if (format)
    dr_format = format;

  if (!buffers)
    buffers = dr_buffers;
  if (!buffers)
    {
      /* This still isn't quite correct as the buffers should
         really change if the frequency is changed, however as
         the default will buffer for a relatively long time
         anyway I don't think it matters. */
      buffers = (int)( (double)DRENDERER_CSEC_TO_BUFFER
                     / ( (double)(DRENDERER_BUFFER_SIZE * 100)
                       / dr_frequency
                       )
                     )
                + 1;
    }
  if ((dr_buffers = DRender_NumBuffers (buffers)) < 0)
    dr_buffers = 0;

  /* Set to fill with zero when out of data,
     but no upcalls nor blocking on overflow */
  DRender_StreamFlags (DRStream_OverrunNull, 0);

#if 0 /* Defer re-activating until samples are written to the stream */
  if ((old_state & DRState_Active) && (old_state != -1))
    return activate_defaults ();
#endif

  return NULL;
}


void *
__dspopen (struct __unixlib_fd *fd, const char *file, int mode)
{
  const _kernel_oserror *err = NULL;
  dr_fragscale = 1;
#if 0
  /*
    This is the sensible place to call activate_defaults() but the results of
    doing so are terrible(!) for FFplay and Mplayer on RISC OS 5.18 for
    BeagleBoard. Because applications open /dev/dsp and THEN start mucking about
    with ioctls on it, the BeagleBoard is continually deactivating and
    reactivating the sound system, taking forever and emitting high-pitched
    whistles. So instead, we defer the call to activate_defaults() until an
    attempt to write to the stream.
  */
  {
    int state;
    if (   (err = set_defaults (fd, 2, 2, 44100, 0)) == NULL
        && (err = check_state (&state)) == NULL
        && !(state & DRState_Active)
       )
      err = activate_defaults ();
  }
#else
  /*
    Set the defaults but don't activate them yet.
  */
  err = set_defaults (fd, 2, 2, 44100, 0);
#endif
  if (err)
    return (void *) __ul_seterr (err, EOPSYS);
  return (void *) 1; /* Dummy value */
}


int
__dspclose (struct __unixlib_fd *fd)
{
  if (fd->devicehandle->handle)
    {
      const _kernel_oserror *err;
      if ((err = DRender_Deactivate ()) != NULL)
        return __ul_seterr (err, EOPSYS);
      DRender_NumBuffers (0);
    }
  return 0;
}


__off_t
__dsplseek (struct __unixlib_fd *fd, __off_t lpos, int whence)
{
  /* Do nothing, just so programs don't complain.  */
  return 0;
}


int
__dspwrite (struct __unixlib_fd *fd, const void *data, int nbyte)
{
  const _kernel_oserror *err;

#if 0
  /*
    Normally, one would simply assume that the stream was already actived by
    __dspopen(). That would be sensible. Normally, we wouldn't waste time on any
    kind of status checking or activation at this point. But that was before
    RISC OS 5.18 landed on the BeagleBoard...
  */
#else
  /*
    On the plus side, if DigitalRenderer gets deactivated somehow, this will try
    to reactivate it again... A small plus...
  */
  {
    int state;
    if (   (err = check_state (&state)) == NULL
        && !(state & DRState_Active)
       )
      err = activate_defaults ();
    if (err)
      return __ul_seterr (err, EOPSYS);
  }
#endif

  int buffer_byte_size = DRENDERER_BUFFER_SIZE * dr_channels;
  if (dr_format == AFMT_S16_LE)
    buffer_byte_size <<= 1;

  int left = nbyte;
  while (left > 0)
    {
      while (DRender_StreamStatistics () >= dr_buffers)
        {
          __pthread_enable_ints ();
          pthread_yield ();
          __pthread_disable_ints ();
        }

      void *next = (void*)((size_t)data + nbyte - left);
      int towrite = ((left < buffer_byte_size) ? left : buffer_byte_size);
      /* Size is in samples, counting left and right as separate samples
         so it is always divided by 2 for 16 bit samples regardless of
         the number of channels */
      if ((err = (dr_format == AFMT_S16_LE)
                 ? DRender_Stream16BitSamples (next, towrite >> 1)
                 : DRender_StreamSamples (next, towrite)) != NULL)
        return __ul_seterr (err, EOPSYS);

      left -= buffer_byte_size;
    }

  return nbyte;
}


int
__dspioctl (struct __unixlib_fd *fd, unsigned long request, void *arg)
{
  request &= 0xffff;

  /* Do nothing */
  if (request == (SNDCTL_DSP_RESET & 0xffff)
      || request == (SNDCTL_DSP_SYNC & 0xffff))
    return 0;

  if (!arg)
    return __set_errno (EINVAL);

  switch (request)
    {
      case SNDCTL_DSP_SPEED & 0xffff:
        {
          if (!set_defaults (fd, 0, 0, *((int *)arg), 0))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_SETFMT & 0xffff:
        {
          int format = *((int *)arg);

          /* DigitalRenderer supports 8-bit ulaw and 16-bit signed linear */
          if ((format == AFMT_MU_LAW || format == AFMT_S16_LE)
              && !set_defaults (fd, 0, format, 0, 0))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_STEREO & 0xffff:
        {
          if (!set_defaults (fd, (*((int *)arg) == 1) ? 2 : 1, 0, 0, 0))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_GETBLKSIZE & 0xffff:
        {
          int blksize = DRENDERER_BUFFER_SIZE * dr_channels;

          if (dr_format == AFMT_MU_LAW)
            blksize <<= 1;
          *((int *)arg) = blksize;

          return 0;
        }

      case SNDCTL_DSP_CHANNELS & 0xffff:
        {
          if (!set_defaults (fd, *((int *)arg), 0, 0, 0))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_SETFRAGMENT & 0xffff:
        {
          int fragspec  = *((int *)arg);
          int fragments = fragspec >> 16;
          int fragsize  = 1 << (fragspec & 0xffff);

          /* fragsize is in bytes, but the buffer size used
             by DigitalRenderer is in samples */
          dr_fragscale = (fragsize / (DRENDERER_BUFFER_SIZE * dr_channels));
          if (dr_format == AFMT_S16_LE)
            dr_fragscale >>= 1;

          if (dr_fragscale == 0)
            dr_fragscale = 1;
          fragments *= dr_fragscale;

          if (!set_defaults (fd, 0, 0, 0, fragments))
            return 0;

          return __set_errno (EINVAL);
        }

      case SNDCTL_DSP_GETFMTS & 0xffff:
        *((int *)arg) = AFMT_MU_LAW | AFMT_S16_LE;
        return 0;

      case SNDCTL_DSP_GETOSPACE & 0xffff:
        {
          int waiting = DRender_StreamStatistics ();
          if (waiting < 0)
            waiting = 0;

          struct audio_buf_info *info = arg;
          info->fragments = (dr_buffers - waiting) / dr_fragscale;
          info->fragstotal = dr_buffers / dr_fragscale;
          info->fragsize = DRENDERER_BUFFER_SIZE * dr_fragscale * dr_channels;
          info->bytes = (dr_buffers - waiting) * DRENDERER_BUFFER_SIZE * dr_channels;
          if (dr_format == AFMT_S16_LE)
            {
              info->fragsize <<= 1;
              info->bytes <<= 1;
            }
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
