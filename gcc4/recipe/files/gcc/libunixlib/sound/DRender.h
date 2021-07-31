/*
 * DRender.h - DigitalRenderer interface
 *
 * Written by
 *  Andreas Dehmel <zarquon@t-online.de>
 *
 * Rewritten for inclusion in GCCSDK's libunixlib by
 *  Christopher Martin <belles@internode.on.net>
 *
 * (C) 1997-2000 Andreas Dehmel (zarquon@t-online.de)
 * (C) 2012 GCCSDK Developers
 *
 * This file was part of wimplib. wimplib is released under the Lesser
 * GNU Public License (LGPL). For more information see the License
 * provided with the library release.
 *
 * Currently part of the GCCSDK project.
 *
 * Note that the extern functions defined herein and implemented in DRender.S
 * are intended to be called **only** when the processor is in USR mode.
 * On the plus side, they will restore flags when called in 26-bit mode.
 */

#ifndef _DIGITAL_RENDERER_H_
#define _DIGITAL_RENDERER_H_

#ifndef __KERNEL_H
#  include <kernel.h>
#endif

#define DRState_Active		(1<<0)
#define DRState_NeedData	(1<<1)
#define DRState_Overflow	(1<<2)
#define DRState_Paused		(1<<4)
#define DRState_16bit		(1<<5)

#define DRStream_OverrunNull	(1<<0)	/* fill buffer with 0 on an overrun */
#define DRStream_IssueUpCall	(1<<1)	/* use upcalls for waiting */
#define DRStream_OverBlock	(1<<2)	/* block on stream overflow,
					   else truncate */

#define DRActivate_Restore	(1<<0)	/* restore previous handler on exit */
#define DRActivate_Callback	(1<<31) /* use call back buffer fill code
					   supplied, else use polling */

#define DRFORMAT_UNDEF		0
#define DRFORMAT_8ULAW		1
#define DRFORMAT_S16RL		2	/* Acorn native: right then left */
#define DRFORMAT_S16LR		3	/* /dev/dsp native: left then right */

#define DigiRendChunk		0x6F700 /* X flag set */

#ifdef __cplusplus
extern "C" {
#endif


/*
    Given NULL, tries to load the module from its standard location of
    "System:Modules.DRenderer". You may give an alternative path to the module.
    Returns NULL if the module is found and loaded successfully.
*/
extern const _kernel_oserror *
DRender_LoadModule(const char * path);


/*
    Digital Renderer SWI calls.
    Consult the DigitalRenderer documentation for information about these calls.
*/

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_Activate (int Channels, int Length, int SamPeriod,
		  void (*Callback)(void))
{
    register const _kernel_oserror *err __asm("r0");
    register int c           __asm("r0") = Channels;
    register int l           __asm("r1") = Length;
    register int p           __asm("r2") = SamPeriod;
    register void (*f)(void) __asm("r3") = Callback;
    __asm__ volatile ("swi      %[XDR_Activate] \n\t"
                      "movvc    r0,#0"
                      : "=r" (err)
                      : "r" (c), "r" (l), "r" (p), "r" (f),
                        [XDR_Activate] "i" (DigiRendChunk)
                      : "r14", "cc");
    return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_Deactivate (void)
{
    register const _kernel_oserror *err __asm("r0");
    __asm__ volatile ("swi      %[XDR_Deactivate] \n\t"
                      "movvc    r0,#0"
                      : "=r" (err)
                      : [XDR_Deactivate] "i" (DigiRendChunk + 1)
                      : "r14", "cc");
    return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_Pause (void)
{
    register const _kernel_oserror *err __asm("r0");
    __asm__ volatile ("swi      %[XDR_Pause]  \n\t"
                      "movvc    r0,#0"
                      : "=r" (err)
                      : [XDR_Pause] "i" (DigiRendChunk + 2)
                      : "r14", "cc");
    return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_Resume (void)
{
    register const _kernel_oserror *err __asm("r0");
    __asm__ volatile ("swi      %[XDR_Resume] \n\t"
                      "movvc    r0,#0"
                      : "=r" (err)
                      : [XDR_Resume] "i" (DigiRendChunk + 3)
                      : "r14", "cc");
    return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_GetTables (unsigned char **LinToLog, unsigned char **LogScale)
{
    register const _kernel_oserror *err __asm("r0");
    __asm__ volatile ("swi     %[XDR_GetTables] \n\t"
                      "strvc   r0,%[LinToLog]   \n\t"
                      "strvc   r1,%[LogScale]   \n\t"
                      "movvc   r0,#0"
                      : [LinToLog] "=m" (*LinToLog),
                        [LogScale] "=m" (*LogScale), "=&r" (err)
                      : [XDR_GetTables] "i" (DigiRendChunk + 4)
                      : "r1", "r14", "cc");
    return err;
}

/*
    Returns -1 on error.
*/
static __inline__ int __attribute__ ((always_inline))
DRender_ReadState (void)
{
    register int state __asm("r0");
    __asm__ volatile ("swi     %[XDR_ReadState]  \n\t"
                      "mvnvs   r0,#0"
                      : "=r" (state)
                      : [XDR_ReadState] "i" (DigiRendChunk + 5)
                      : "r14", "cc");
    return state;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_NewSample (const unsigned char *Sample)
{
    register const _kernel_oserror *err __asm("r0");
    register const unsigned char *s __asm("r0") = Sample;
    __asm__ volatile ("swi     %[XDR_NewSample]  \n\t"
                      "movvc   r0,#0"
                      : "=r" (err)
                      : "m" (*s), "r" (s),
                        [XDR_NewSample] "i" (DigiRendChunk + 6)
                      : "r14", "cc");
    return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_New16BitSample (const short *Sample)
{
    register const _kernel_oserror *err __asm("r0");
    register const short *s __asm("r0") = Sample;
    __asm__ volatile ("swi     %[XDR_New16BitSample]  \n\t"
                      "movvc   r0,#0"
                      : "=r" (err)
                      : "m" (*s), "r" (s),
                        [XDR_New16BitSample] "i" (DigiRendChunk + 7)
                      : "r14", "cc");
    return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_BufferStatistics (int *BuffersPlayed, int *BuffersGiven)
{
    register const _kernel_oserror *err __asm("r0");
    __asm__ volatile ("swi     %[XDR_BufferStatistics]  \n\t"
                      "strvc   r0,%[BuffersPlayed]      \n\t"
                      "strvc   r1,%[BuffersGiven]       \n\t"
                      "movvc   r0,#0"
                      : [BuffersPlayed] "=m" (*BuffersPlayed),
                        [BuffersGiven] "=m" (*BuffersGiven), "=&r" (err)
                      : [XDR_BufferStatistics] "i" (DigiRendChunk + 8)
                      : "r1", "r14", "cc");
    return err;
}

/*
    Call with -1 to read, or 0 to turn off the streaming interface,
    or >0 to initialise the streaming interface.
    Returns the number of buffers currently allocated, or -1 on error.
*/
static __inline__ int __attribute__ ((always_inline))
DRender_NumBuffers (int num)
{
    register int n __asm("r0") = num;
    __asm__ volatile ("swi     %[XDR_NumBuffers]  \n\t"
                      "mvnvs   r0,#0"
                      : "+r" (n)
                      : [XDR_NumBuffers] "i" (DigiRendChunk + 9)
                      : "r14", "cc");
    return n;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_StreamSamples (const unsigned char *Sample, int num)
{
    register const _kernel_oserror *err __asm("r0");
    register const unsigned char *s __asm("r0") = Sample;
    register int n __asm("r1") = num;
    __asm__ volatile ("swi     %[XDR_StreamSamples]    \n\t"
                      "movvc   r0,#0"
                      : "=r" (err)
                      : "m" (*s), "r" (s), "r" (n),
                        [XDR_StreamSamples] "i" (DigiRendChunk + 10)
                      : "r14", "cc");
    return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_Stream16BitSamples (const unsigned short *Sample, int num)
{
    register const _kernel_oserror *err __asm("r0");
    register const unsigned short *s __asm("r0") = Sample;
    register int n __asm("r1") = num;
    __asm__ volatile ("swi     %[XDR_Stream16BitSamples]   \n\t"
                      "movvc   r0,#0"
                      : "=r" (err)
                      : "m" (*s), "r" (s), "r" (n),
                        [XDR_Stream16BitSamples] "i" (DigiRendChunk + 11)
                      : "r14", "cc");
    return err;
}

static __inline__ int __attribute__ ((always_inline))
DRender_StreamStatistics (void) /* returns -1 on error */
{
    register int n __asm("r0");
    __asm__ volatile ("swi     %[XDR_StreamStatistics] \n\t"
                      "mvnvs   r0,#0"
                      : "=r" (n)
                      : [XDR_StreamStatistics] "i" (DigiRendChunk + 12)
                      : "r14", "cc");
    return n;
}

/*
    Returns old stream flags, or -1 on error.
*/
static __inline__ int __attribute__ ((always_inline))
DRender_StreamFlags (int eor, int and)
{
    register int r0 __asm("r0") = eor;
    register int r1 __asm("r1") = and;
    __asm__ volatile ("swi     %[XDR_StreamFlags]  \n\t"
                      "mvnvs   r0,#0"
                      : "+r" (r0)
                      : "r" (r1), [XDR_StreamFlags] "i" (DigiRendChunk + 13)
                      : "r14", "cc");
    return r0;
}

extern const _kernel_oserror *
DRender_SetDefaults (int *channels, int *format, int *period, int *buffsize,
		     int *numbuff, int *freq);

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_Activate16 (int channels, int buffsize, int freq, int flags)
{
    register const _kernel_oserror *err __asm("r0");
    register int c __asm("r0") = channels;
    register int b __asm("r1") = buffsize;
    register int f __asm("r2") = freq;
    register int s __asm("r3") = flags;
    __asm__ volatile ("swi      %[XDR_Activate16] \n\t"
                      "movvc    r0,#0"
                      : "=r" (err)
                      : "r" (c), "r" (b), "r" (f), "r" (s),
                        [XDR_Activate16] "i" (DigiRendChunk + 15)
                      : "r14", "cc");
    return err;
}

/*
    Returns 0 if DigitalRenderer is off, -1 on error.
*/
static __inline__ int __attribute__ ((always_inline))
DRender_GetFrequency (void)
{
    register int freq __asm("r0");
    __asm__ volatile ("swi     %[XDR_GetFrequency] \n\t"
                      "mvnvs   r0,#0"
                      : "=r" (freq)
                      : [XDR_GetFrequency] "i" (DigiRendChunk + 16)
                      : "r14", "cc");
    return freq;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
DRender_ActivateAuto (int channels, int buffsize, int freq)
{
    register const _kernel_oserror *err __asm("r0");
    register int c __asm("r0") = channels;
    register int b __asm("r1") = buffsize;
    register int f __asm("r2") = freq;
    __asm__ volatile ("swi      %[XDR_ActivateAuto] \n\t"
                      "movvc    r0,#0"
                      : "=r" (err)
                      : "r" (c), "r" (b), "r" (f),
                        [XDR_ActivateAuto] "i" (DigiRendChunk + 17)
                      : "r14", "cc");
    return err;
}

/*
    Call with 0 to read.
    Returns -1 on error.
*/
static __inline__ int __attribute__ ((always_inline))
DRender_SampleFormat (int format)
{
    register int f __asm("r0") = format;
    __asm__ volatile ("swi     %[XDR_SampleFormat]  \n\t"
                      "mvnvs   r0,#0"
                      : "+r" (f)
                      : [XDR_SampleFormat] "i" (DigiRendChunk + 18)
                      : "r14", "cc");
    return f;
}

#ifdef __cplusplus
}
#endif

#endif
