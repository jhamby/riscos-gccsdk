/* Internal UnixLib stdio.h
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __STDIO_H
#include_next <stdio.h>
#endif

#if !defined(__INTERNAL_STDIO_H) && defined(__STDIO_H)
#define __INTERNAL_STDIO_H

__BEGIN_DECLS

#ifndef __TARGET_SCL__

/* Magic number to fill __magic.  */
#define _IOMAGIC 0xfe000000

/* Nonzero if stream is a valid stream.  */
#define __validfp(stream) ((stream) != NULL && (stream)->__magic == _IOMAGIC)

/* Invalidate a stream.  */
extern void __invalidate (FILE *__stream) __THROW __nonnull ((1));

/* Make a new stream.  */
extern FILE *__newstream (void) __THROW __wur;

/* Initialise a new stream.  */
extern FILE *__stream_init (int __fd, FILE *__stream)
     __THROW __nonnull ((2)) __wur;

extern int __flslbbuf (void) __THROW __wur;

/* Dissect the given mode string into an __io_mode.  */
extern __io_mode __getmode (const char *__mode) __THROW __nonnull ((1));

extern void __stdioinit (void);	/* Initialise stdin, stdout & stderr.  */
extern void __stdioexit (void);	/* Close streams & delete tmpfile().  */

/* Return the next character in the input buffer, keeping it in
   the input buffer.  If the buffer is empty, then fill it.  */
extern int __peek_char (FILE *__stream) __THROW;

/* All streams are in a linked list.
   This is the head of the list.  */
extern FILE *__iob_head;

/* Write formatted output to stream from arg list arg.  This function
   is a cancellation point.

   Non thread-safe version.   This function is for UnixLib internal
   use only.  */
extern int __vfprintf (FILE *__restrict __stream,
		       const char *__restrict __format, __gnuc_va_list __arg);

#endif

__END_DECLS

#endif
