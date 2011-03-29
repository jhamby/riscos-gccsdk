/*
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#ifndef __INTERNAL_OS_H
#define __INTERNAL_OS_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __KERNEL_H
#include <kernel.h>
#endif

#ifndef __SWIS_H
#  include <swis.h>
#endif

__BEGIN_DECLS

/* Setup console I/O.  */
extern int __os_console (void) __THROW;

/* Setup RS423 I/O.  */
extern int __os_423 (void) __THROW;

extern int __os_vdu (int) __THROW;
extern int __os_get (void) __THROW;

/* Returns -1 on timeout.  */
extern int __os_inkey (int) __THROW;

/* Flush keyboard buffer.  */
extern int __os_keyflush (void) __THROW;

/* 423 times are in centiseconds */
extern int __os_423vdu (int) __THROW;
extern int __os_423get (void) __THROW;

/* Returns -1 on timeout.  */
extern int __os_423inkey (int) __THROW;

/* Flush RS423 buffer.  */
extern int __os_423flush (void) __THROW;
extern int __os_423break (int) __THROW;

extern _kernel_oserror *__os_swi (int swinum, int *regs /* 10 reg */ )
     __THROW __nonnull ((2));

static inline const _kernel_oserror *
__os_byte (int __a, int __xin, int __yin, int *__xoutp, int *__youtp)
{
  register int a __asm ("r0") = __a;
  register int xin __asm ("r1") = __xin;
  register int yin __asm ("r2") = __yin;
  register const _kernel_oserror *err __asm ("r0");
  register int xout __asm ("r1");
  register int yout __asm ("r2");
  __asm__ volatile ("SWI\t%[SWI_XOS_Byte]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (xout), "=r" (yout)
		    : "r" (a), "r" (xin), "r" (yin),
		      [SWI_XOS_Byte] "i" (OS_Byte | (1<<17))
		    : "r14", "cc");
  if (__xoutp)
    *__xoutp = xout;
  if (__youtp)
    *__youtp = yout;
  return err;
}

extern _kernel_oserror *__os_word (int, void *)
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_prhex (int); 	/* %8x format hex output */
extern _kernel_oserror *__os_prdec (int);

extern _kernel_oserror *__os_print (const char *) __nonnull ((1));
extern _kernel_oserror *__os_write (const char *, int) __nonnull ((1));

extern _kernel_oserror *__os_nl (void);		/* newline */

static inline const _kernel_oserror *
__os_cli (const char *__cmd)
{
  register const char *cmd __asm ("r0") = __cmd;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XOS_CLI]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (cmd), [SWI_XOS_CLI] "i" (OS_CLI | (1<<17))
		    : "r14", "cc");

  return err;
}

extern _kernel_oserror *__os_file (int, const char *, int * /* 6 reg */ )
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_fopen (int, const char *, int * /* 1 reg */ )
     __THROW __nonnull ((2, 3)) __wur;

extern _kernel_oserror *__os_fclose (int) __THROW;

extern _kernel_oserror *__os_fread (int, void *, int, int * /* 5 reg */ )
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_fwrite (int, const void *, int,
                                     int * /* 5 reg */ )
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_args (int, int, int, int * /* 3 reg */ )
     __THROW __wur;

extern _kernel_oserror *__os_fsctrl (int, const char *, int, int)
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_setfiletype (const char *fname, int filetype)
     __THROW __nonnull ((1)) __wur;

/* Flag RISC OS error to UNIX and take a copy of the RISC OS error block.
   If 'err' is NULL, no action is taken.
   If 'new_errno' is non-zero, then UnixLib's errno is also set to 'new_errno'.
   This function returns -1 if there is an error to allow to function
   to be used as a return value in error situations */
extern int __ul_seterr (const _kernel_oserror *__err, int __new_errno)
     __THROW;

__END_DECLS

#endif
