/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/os.h,v $
 * $Date: 2000/07/15 14:52:15 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __SYS_OS_H
#define __SYS_OS_H

#ifndef __KERNEL_H
#include <kernel.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define os_error _kernel_oserror

/* Setup console I/O.  */
extern int os_console (void);
/* Setup RS423 I/O.  */
extern int os_423 (void);

extern int os_vdu (int);
extern int os_get (void);
/* Returns -1 on timeout.  */
extern int os_inkey (int);
/* Flush keyboard buffer.  */
extern int os_keyflush (void);

/* 423 times are in centiseconds */

extern int os_423vdu (int);
extern int os_423get (void);
/* Returns -1 on timeout.  */
extern int os_423inkey (int);
/* Flush RS423 buffer.  */
extern int os_423flush (void);
extern int os_423break (int);

extern _kernel_oserror *os_swi (int,int * /* 10 reg */ );

extern _kernel_oserror *os_byte (int,int,int,int * /* 3 reg */ );
extern _kernel_oserror *os_word (int,void *);

extern _kernel_oserror *os_prhex (int); 	/* %8x format hex output */
extern _kernel_oserror *os_prdec (int);
extern _kernel_oserror *os_print (const char *);
extern _kernel_oserror *os_write (const char *,int);
extern _kernel_oserror *os_nl (void);		/* newline */

extern _kernel_oserror *os_cli (const char *);

extern _kernel_oserror *os_file (int,const char *,int * /* 6 reg */ );

/* OS_Find */

extern _kernel_oserror *os_fopen (int,const char *,int * /* 1 reg */ );
extern _kernel_oserror *os_fclose (int);

/* OS_GBPB */

extern _kernel_oserror *os_fread (int,void *,int,int * /* 5 reg */ );
extern _kernel_oserror *os_fwrite (int,const void *,int,int * /* 5 reg */ );

extern _kernel_oserror *os_args (int,int,int,int * /* 3 reg */ );

extern _kernel_oserror *os_fsctrl (int,const char *,char *,int);

#ifdef __UNIXLIB_INTERNALS
/* Flag RISC OS error to UNIX.  */
extern void __seterr (const _kernel_oserror *);
#endif

#ifdef __cplusplus
	}
#endif

#endif
