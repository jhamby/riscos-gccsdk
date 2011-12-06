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

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Byte (int __a, int __xin, int __yin, int *__xoutp, int *__youtp)
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
  if (__xoutp && !err)
    *__xoutp = xout;
  if (__youtp && !err)
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

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_CLI (const char *__cmd)
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

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_File_ReadCatInfo (const char *__filename, unsigned *__objtype,
			 unsigned *__loadaddr, unsigned *__execaddr,
			 unsigned *__objlen, unsigned *__attr)
{
  register const char *filename __asm ("r1") = __filename;
  register const _kernel_oserror *err __asm ("r0");
  register unsigned objtype __asm ("r1");
  register unsigned loadaddr __asm ("r2");
  register unsigned execaddr __asm ("r3");
  register unsigned objlen __asm ("r4");
  register unsigned attr __asm ("r5");
  __asm__ volatile ("MOV\tr0, #17\n\t"
		    "SWI\t%[SWI_XOS_File]\n\t"
		    "MOV\tr1, r0\n\t"
                    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (objtype), "=r" (loadaddr),
		      "=r" (execaddr), "=r" (objlen), "=r" (attr)
		    : "r" (filename), [SWI_XOS_File] "i" (OS_File | (1<<17))
		    : "r14", "cc");
  if (!err)
    {
      if (__objtype)
	*__objtype = objtype;
      if (__loadaddr)
	*__loadaddr = loadaddr;
      if (__execaddr)
	*__execaddr = execaddr;
      if (__objlen)
	*__objlen = objlen;
      if (__attr)
	*__attr = attr;
    }
  return err;
}

extern _kernel_oserror *__os_fopen (int, const char *, int * /* 1 reg */ )
     __THROW __nonnull ((2, 3)) __wur;

extern _kernel_oserror *__os_fclose (int) __THROW;

extern _kernel_oserror *__os_fread (int, void *, int, int * /* 5 reg */ )
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_fwrite (int, const void *, int,
                                     int * /* 5 reg */ )
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_fsctrl (int, const char *, int, int)
     __THROW __nonnull ((2)) __wur;

extern _kernel_oserror *__os_setfiletype (const char *fname, int filetype)
     __THROW __nonnull ((1)) __wur;

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_ChangeRedirection (int __fh_in, int __fh_out,
			  int *__prev_fh_in, int *__prev_fh_out)
{
  register int fh_in __asm ("r0") = __fh_in;
  register int fh_out __asm ("r1") = __fh_out;
  register int prev_fh_in __asm ("r0");
  register int prev_fh_out __asm ("r1");
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XOS_ChangeRedirection]\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    : "=r" (prev_fh_in), "=r" (prev_fh_out), [err] "=r" (err)
		    : "r" (fh_in), "r" (fh_out),
		      [SWI_XOS_ChangeRedirection] "i" (OS_ChangeRedirection | (1<<17))
		    : "r14", "cc");
  if (__prev_fh_in && !err)
    *__prev_fh_in = prev_fh_in;
  if (__prev_fh_out && !err)
    *__prev_fh_out = prev_fh_out;
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_DDEUtils_SetCLSize (size_t __len)
{
  register size_t len __asm ("r0") = __len;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XDDEUtils_SetCLSize]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (len), [SWI_XDDEUtils_SetCLSize] "i" (DDEUtils_SetCLSize | (1<<17))
		    : "r14", "cc");
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_DDEUtils_SetCL (const char *__cli)
{
  register const char *cli __asm ("r0") = __cli;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XDDEUtils_SetCL]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (cli), [SWI_XDDEUtils_SetCL] "i" (DDEUtils_SetCL | (1<<17))
		    : "r14", "cc");
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_DDEUtils_GetCLSize (size_t *__len)
{
  register size_t len __asm ("r0");
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XDDEUtils_GetCLSize]\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    : [err] "=r" (err), "=r" (len)
		    : [SWI_XDDEUtils_GetCLSize] "i" (DDEUtils_GetCLSize | (1<<17))
		    : "r14", "cc");
  if (__len && !err)
    *__len = len;
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_DDEUtils_GetCL (char *__cli)
{
  register char *cli __asm ("r0") = __cli;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XDDEUtils_GetCL]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (cli),
		      [SWI_XDDEUtils_GetCL] "i" (DDEUtils_GetCl | (1<<17))
		    : "r14", "cc", "memory");
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_DDEUtils_Prefix (const char *__dir)
{
  register const char *dir __asm ("r0") = __dir;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XDDEUtils_Prefix]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (dir),
		      [SWI_XDDEUtils_Prefix] "i" (DDEUtils_Prefix | (1<<17))
		    : "r14", "cc");
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_DDEUtils_ReadPrefix (int __ao, const char **__prefix)
{
  register int ao __asm ("r0") = __ao;
  register const char *prefix __asm ("r0");
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XDDEUtils_ReadPrefix]\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    : [err] "=r" (err), "=r" (prefix)
		    : "r" (ao),
		      [SWI_XDDEUtils_ReadPrefix] "i" (DDEUtils_ReadPrefix | (1<<17))
		    : "r14", "cc");
  if (__prefix && !err)
    *__prefix = prefix;
  return err;
}

/* Canonicalise pathname.
   PATHNAME will be canonicalised and returned in BUF (being BUFSIZE bytes
   deep).  When BUFSIZE is too small, REQ_XTRABUFSIZE will be non-zero,
   otherwise it will be zero.
   When PATHVAR is non-NULL, it will be used for canonicalisation.  */
static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_FSControl_Canonicalise (const char *__pathname, const char *__pathvar,
			       char *__buf, size_t __bufsize,
			       size_t *__req_xtrabufsize)
{
  register const char *pathname __asm ("r1") = __pathname;
  register char *buf __asm ("r2") = __buf;
  register const char *pathvar __asm ("r3") = __pathvar;
  register size_t bufsize __asm ("r5") = __bufsize;
  register const _kernel_oserror *err __asm ("r0");
  register int xtrabufsize __asm ("r5");
  __asm__ volatile ("MOV\tr0, #37\n\t"
		    "MOV\tr4, #0\n\t"
		    "SWI\t%[SWI_XOS_FSControl]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (xtrabufsize)
		    : "r" (pathname), "r" (buf), "r" (pathvar), "r" (bufsize),
		      [SWI_XOS_FSControl] "i" (OS_FSControl | (1<<17))
		    : "r4", "r14", "cc", "memory");
  if (__req_xtrabufsize && !err)
    *__req_xtrabufsize = xtrabufsize < 1 ? 1 - xtrabufsize : 0;
  return err;
}

/* Read filehandle's sequential file pointer.  */
static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Args_GetFilePtr (int __fhandle, int *__fileptr)
{
  register int fhandle __asm ("r1") = __fhandle;
  register const _kernel_oserror *err __asm ("r0");
  register int fileptr __asm ("r2");
  __asm__ volatile ("MOV\tr0, #0\n\t"
		    "SWI\t%[SWI_XOS_Args]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (fileptr)
		    : "r" (fhandle),
		      [SWI_XOS_Args] "i" (OS_Args | (1<<17))
		    : "r14", "cc");
  if (__fileptr && !err)
    *__fileptr = fileptr;
  return err;
}

/* Write filehandle's sequential file pointer.  */
static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Args_SetFilePtr (int __fhandle, int __fileptr)
{
  register int fhandle __asm ("r1") = __fhandle;
  register int fileptr __asm ("r2") = __fileptr;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("MOV\tr0, #1\n\t"
		    "SWI\t%[SWI_XOS_Args]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (fhandle), "r" (fileptr),
		      [SWI_XOS_Args] "i" (OS_Args | (1<<17))
		    : "r14", "cc");
  return err;
}  

/* Read filehandle's extent.  */
static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Args_GetExtent (int __fhandle, int *__extent)
{
  register int fhandle __asm ("r1") = __fhandle;
  register const _kernel_oserror *err __asm ("r0");
  register int extent __asm ("r2");
  __asm__ volatile ("MOV\tr0, #2\n\t"
		    "SWI\t%[SWI_XOS_Args]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (extent)
		    : "r" (fhandle),
		      [SWI_XOS_Args] "i" (OS_Args | (1<<17))
		    : "r14", "cc");
  if (__extent && !err)
    *__extent = extent;
  return err;
}

/* Write filehandle's extent.  */
static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Args_SetExtent (int __fhandle, int __extent)
{
  register int fhandle __asm ("r1") = __fhandle;
  register int extent __asm ("r2") = __extent;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("MOV\tr0, #3\n\t"
		    "SWI\t%[SWI_XOS_Args]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (fhandle), "r" (extent),
		      [SWI_XOS_Args] "i" (OS_Args | (1<<17))
		    : "r14", "cc");
  return err;
}  

/* Canonicalise file handle.  */
static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Args_Canonicalise (int __fhandle, char *__buf, size_t __bufsize,
			  size_t *__req_xtrabufsize)
{
  register int fhandle __asm ("r1") = __fhandle;
  register char *buf __asm ("r2") = __buf;
  register size_t bufsize __asm ("r5") = __bufsize;
  register const _kernel_oserror *err __asm ("r0");
  register int xtrabufsize __asm ("r5");
  __asm__ volatile ("MOV\tr0, #7\n\t"
		    "SWI\t%[SWI_XOS_Args]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (xtrabufsize)
		    : "r" (fhandle), "r" (buf), "r" (bufsize),
		      [SWI_XOS_Args] "i" (OS_Args | (1<<17))
		    : "r14", "cc", "memory");
  if (__req_xtrabufsize && !err)
    *__req_xtrabufsize = xtrabufsize < 1 ? 1 - xtrabufsize : 0;
  return err;
}

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Args_GetFileHandleStatus (int __fhandle, int *__status)
{
  register int fhandle __asm ("r1") = __fhandle;
  register const _kernel_oserror *err __asm ("r0");
  register int status __asm ("r2");
  __asm__ volatile ("MOV\tr0, #254\n\t"
		    "SWI\t%[SWI_XOS_Args]\n\t"
		    "MOV\tr2, r0\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (status)
		    : "r" (fhandle),
		      [SWI_XOS_Args] "i" (OS_Args | (1<<17))
		    : "r14", "cc", "memory");
  if (__status && !err)
    *__status = status;
  return err;
}

/* Flush filehandle(s).  */
static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_OS_Args_Flush (int __fhandle)
{
  register int fhandle __asm ("r1") = __fhandle;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("MOV\tr0, #255\n\t"
		    "SWI\t%[SWI_XOS_Args]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (fhandle),
		      [SWI_XOS_Args] "i" (OS_Args | (1<<17))
		    : "r14", "cc");
  return err;
}

/* Flag RISC OS error to UNIX and take a copy of the RISC OS error block.
   If 'err' is NULL, no action is taken.
   If 'new_errno' is non-zero, then UnixLib's errno is also set to 'new_errno'.
   This function returns -1 if there is an error to allow to function
   to be used as a return value in error situations */
extern int __ul_seterr (const _kernel_oserror *__err, int __new_errno)
     __THROW;

__END_DECLS

#endif
