/*
 * DRender.h - DigitalRenderer interface
 *
 * Written by
 *  Andreas Dehmel <zarquon@t-online.de>
 *
 * This file is part of wimplib. wimplib is released under the Lesser
 * GNU Public License (LGPL). For more information see the License
 * provided with the library release.
 */

#ifndef _DIGITAL_RENDERER_H_
#define _DIGITAL_RENDERER_H_


#ifdef __cplusplus
extern "C" {
#endif

#define DRState_Active		1
#define DRState_NeedData	2
#define DRState_Overflow	4


/* Digital Renderer SWI calls */
extern _kernel_oserror *DigitalRenderer_Activate(int Channels, int Length, int SamPeriod);

extern _kernel_oserror *DigitalRenderer_Deactivate(void);

extern _kernel_oserror *DigitalRenderer_Pause(void);

extern _kernel_oserror *DigitalRenderer_Resume(void);

extern _kernel_oserror *DigitalRenderer_GetTables(unsigned char **LinToLog, unsigned char **LogScale);

extern int DigitalRenderer_ReadState(void);

extern _kernel_oserror *DigitalRenderer_NewSample(const unsigned char *Sample);

extern _kernel_oserror *DigitalRenderer_New16BitSample(const short *Sample);

extern void DigitalRenderer_BufferStatistics(int *BuffersPlayed, int *BuffersGiven);

extern int  DigitalRenderer_NumBuffers(int num);

extern void DigitalRenderer_StreamSamples(const unsigned char *Sample, int num);

extern void DigitalRenderer_Stream16BitSamples(const unsigned short *Sample, int num);

extern int  DigitalRenderer_StreamStatistics(void);

extern int  DigitalRenderer_StreamFlags(int eor, int and);

#define DRFORMAT_UNDEF	0
#define DRFORMAT_8ULAW	1
#define DRFORMAT_16LIN	2
#define DRFORMAT_16SWAP	3

extern void DigitalRenderer_SetDefaults(int *channels, int *format, int *period, int *buffsize, int *numbuff, int *freq);

extern _kernel_oserror *DigitalRenderer_Activate16(int channels, int buffsize, int freq, int flags);

extern int DigitalRenderer_GetFrequency(void);

extern _kernel_oserror *DigitalRenderer_ActivateAuto(int channels, int buffsize, int freq);

extern int DigitalRenderer_SampleFormat(int format);

#ifdef __cplusplus
}
#endif

#endif
