/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/os.h,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __UNIXLIB_OS_H
#define __UNIXLIB_OS_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __KERNEL_H
#include <kernel.h>
#endif

__BEGIN_DECLS

#ifdef __UNIXLIB_INTERNALS

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

extern _kernel_oserror *__os_byte (int a, int x, int y,
				   int *result /* 3 reg */ )
     __THROW;

extern _kernel_oserror *__os_word (int,void *)
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_prhex (int); 	/* %8x format hex output */
extern _kernel_oserror *__os_prdec (int);

extern _kernel_oserror *__os_print (const char *) __nonnull ((1));
extern _kernel_oserror *__os_write (const char *,int) __nonnull ((1));

extern _kernel_oserror *__os_nl (void);		/* newline */
extern _kernel_oserror *__os_cli (const char *) __nonnull ((1));

extern _kernel_oserror *__os_file (int,const char *,int * /* 6 reg */ )
     __THROW __nonnull ((2, 3)) __wur;

extern _kernel_oserror *__os_fopen (int,const char *,int * /* 1 reg */ )
     __THROW __nonnull ((2, 3)) __wur;

extern _kernel_oserror *__os_fclose (int) __THROW;

extern _kernel_oserror *__os_fread (int,void *,int,int * /* 5 reg */ )
     __THROW __nonnull ((2, 4)) __wur;

extern _kernel_oserror *__os_fwrite (int,const void *,int,int * /* 5 reg */ )
     __THROW __nonnull ((2, 4)) __wur;

extern _kernel_oserror *__os_args (int,int,int,int * /* 3 reg */ )
     __THROW __nonnull ((4)) __wur;

extern _kernel_oserror *__os_fsctrl (int,const char *,char *,int)
     __THROW __nonnull ((2, 3)) __wur;

extern _kernel_oserror *__os_setfiletype (const char *fname, int filetype)
     __THROW __nonnull ((1)) __wur;

/* Flag RISC OS error to UNIX. When NULL is given as parameter, __seterr()
   doesn't do anything. Sets UnixLib's errno to EOPSYS too and takes
   a copy of the RISC OS error block.  */
extern void __seterr (const _kernel_oserror *);
#endif

__END_DECLS

#endif
