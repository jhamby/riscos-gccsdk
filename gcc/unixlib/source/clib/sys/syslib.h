/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/syslib,v $
 * $Date: 2000/07/03 11:32:43 $
 * $Revision: 1.16 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __SYS_SYSLIB_H
#define __SYS_SYSLIB_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __SYS_OS_H
#include <sys/os.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __UNIXLIB_INTERNALS

#ifndef __GNUC__
#undef  __attribute__
#define __attribute__(x) /* Ignore */
#endif

extern int __funcall_error(const char *, int, unsigned int);
#ifndef __PARANOID
#define __funcall(f,p) ((f)p)
#else
#define __funcall(f,p) \
  ((((void *)(f) >= __base) && (((unsigned int)(f) & ~3) == (unsigned int)(f)) \
   ? 0 : __funcall_error(__FILE__,__LINE__,(unsigned int)(f))), (f)p)
#endif

/* C initialization.  */
extern void _main (void);

/* Assembler exit.  */
extern void __exit (int) __attribute__ ((__noreturn__));
extern void __exit_no_code (void) __attribute__ ((__noreturn__));

#define STAKSIZ 2048

/* __break is initialised to __lomem & __stack to __himem - STAKSIZ;
 * __stack is extended downwards in 512 byte chunks by x$stack_overflow()
 * and __break is extended upwards by brk() and sbrk(). The sl
 * register is kept equal to __stack + 256. Should x$stack_overflow()
 * attempt to extend __stack below __break then SIGEMT is raised.
 * Should brk() or sbrk() be asked to extend __break above __stack
 * then they return with ENOMEM. */

extern char *__cli;		/* command line from OS_GetEnv */

extern void __svccli(char *);	/* strcpy __cli in SVC mode */

extern void *__base;		/* BASE = Image$$RO$$Base */
extern void *__lomem;		/* LOMEM = Image$$RW$$Limit */
extern void *__himem;		/* HIMEM from OS_GetEnv */
extern void *__rwlimit;

extern void *__break;		/* end of data area */
extern void *__real_break;	/* real end of data area */
extern void *__stack;		/* bottom of stack */
extern void *__stack_limit;
extern int __codeshift;
extern int __dynamic_num;
extern void __dynamic_area_exit(void);

extern void __munmap_all (void);	/* Deallocate all mappings.  */

/* Zero if we are not executing within a TaskWindow
   Non-zero if we are executing within a TaskWindow.  */
extern int __taskwindow;
/* Zero if we are running in command mode (not as a WIMP program)
   Non-zero if we are running as a WIMP program.  */
extern int __wimpprogram;

extern unsigned int __time[2];	/* start time */

#define __OS_ARTHUR	0xA0
#define __OS_RISCOS_200 0xA1
#define __OS_RISCOS_201 0xA2
#define __OS_RISCOS_300 0xA3
#define __OS_RISCOS_310 0xA4
#define __OS_RISCOS_350	0xA5
#define	__OS_RISCOS_360 0xA6
#define __OS_RISCOS_370 0xA7
#define __OS_RISCOS_400 0xA8

/* setjmp() and longjmp() modify their behaviour according to __fpflag */

/* FP flag reflecting Floating Point presence or not.  */
extern int __fpflag;

/* Stop the interval timers.  */
extern void __stop_itimers (void);

/* OS_ChangeEnvironment is used to set up exception handlers. These
 * handlers use OS_CallBack to raise signals in the foreground process.
 * RTFM (RISC OS PRM - 'The Program Environment') for more info. */

/* __reset() performs all the functions of exit(), but returns without
 * exiting - note that after a call to __reset() all handlers are
 * in their original state, and *all* stdio & UNIX I/O has been shut down.
 * You should use __exit() to exit after __reset(). */

/* Reset handlers, etc. back to original state.  */
extern void __restore_calling_environment_handlers (void);

/* Reset unixlib.  */
extern void __reset (void);


/* Struct handler contains all the information about an
   OS_ChangeEnvironment handler.  */

struct handler
{
  /* Address of handler (0 = default).  */
  void (*handler)(void);
  /* R12/0 handler arg. (0 = default).  */
  unsigned int handler_r12;
  /* Buffer pointer (0 = default).  */
  void *buffer;
};

/* Struct env contains all the info. needed to restore
   the RISC OS environment prior to a call to OS_Exit.  */

#define __ENVIRONMENT_HANDLERS 17

struct env
{
  /* Handlers 0-16.  */
  struct handler handlers[__ENVIRONMENT_HANDLERS];
};

/* This C environment.  */
extern struct env __c_environment[1];
/* The old (calling) environment.  */
extern struct env __calling_environment[1];

/* OS_ChangeEnvironment.  */
extern _kernel_oserror *__read_environment_handler (int, struct handler *);

/* OS_ChangeEnvironment */
extern _kernel_oserror *__write_environment_handler (int, struct handler *);

/* vfork() & exec() */

extern void __vret(int);	/* return from child - calls __vexit() */

extern int *__vfork(void);	/* fork to child context */
extern int *__vexit(int);	/* restore to parent context */

extern void __exret(void);	/* return from __exec() - calls __vret() */

extern void (*__exptr)(char *); /* pointer to __exec() routine */
extern int __exlen;		/* length of __exec() routine */
extern _kernel_oserror *__exerr;
extern int __exshift;		/* __exec() shift */

/* vfork weak link.  */

extern void (*___vret) (int);

/* stdio weak links.  */

extern void (*___stdioinit) (void);
extern void (*___stdioexit) (void);

#endif  /* __UNIXLIB_INTERNALS */

#ifdef __cplusplus
}
#endif

#endif
