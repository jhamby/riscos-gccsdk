/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/os.h,v $
 * $Date: 2004/10/17 16:24:44 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: joty $
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
extern int __os_console (void);
/* Setup RS423 I/O.  */
extern int __os_423 (void);

extern int __os_vdu (int);
extern int __os_get (void);
/* Returns -1 on timeout.  */
extern int __os_inkey (int);
/* Flush keyboard buffer.  */
extern int __os_keyflush (void);

/* 423 times are in centiseconds */

extern int __os_423vdu (int);
extern int __os_423get (void);
/* Returns -1 on timeout.  */
extern int __os_423inkey (int);
/* Flush RS423 buffer.  */
extern int __os_423flush (void);
extern int __os_423break (int);

extern _kernel_oserror *__os_swi (int,int * /* 10 reg */ );
extern _kernel_oserror *__os_byte (int,int,int,int * /* 3 reg */ );
extern _kernel_oserror *__os_word (int,void *);
extern _kernel_oserror *__os_prhex (int); 	/* %8x format hex output */
extern _kernel_oserror *__os_prdec (int);
extern _kernel_oserror *__os_print (const char *);
extern _kernel_oserror *__os_write (const char *,int);
extern _kernel_oserror *__os_nl (void);		/* newline */
extern _kernel_oserror *__os_cli (const char *);
extern _kernel_oserror *__os_file (int,const char *,int * /* 6 reg */ );
extern _kernel_oserror *__os_fopen (int,const char *,int * /* 1 reg */ );
extern _kernel_oserror *__os_fclose (int);
extern _kernel_oserror *__os_fread (int,void *,int,int * /* 5 reg */ );
extern _kernel_oserror *__os_fwrite (int,const void *,int,int * /* 5 reg */ );
extern _kernel_oserror *__os_args (int,int,int,int * /* 3 reg */ );
extern _kernel_oserror *__os_fsctrl (int,const char *,char *,int);

/* Flag RISC OS error to UNIX. When NULL is given as parameter, __seterr()
   doesn't do anything. Sets UnixLib's errno to EOPSYS too and takes
   a copy of the RISC OS error block.  */
extern void __seterr (const _kernel_oserror *);
#endif

__END_DECLS

#endif
