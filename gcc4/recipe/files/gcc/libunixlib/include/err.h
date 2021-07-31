/*
 * 4.4BSD formatted error messages
 * Copyright (c) 2012 UnixLib Developers
 */

#ifndef __ERR_H
#define __ERR_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need___va_list
#include <stdarg.h>

__BEGIN_DECLS

/* Display a formatted error message on the standard error output, or on
   another file specified using the err_set_file() function.  In all cases,
   the last component of the program name, a colon character, and a space are
   output.
   If the FMT argument is not NULL, the printf(3)-like formatted error message
   is output.
   This is followed by the error message obtained from strerror(3) based on a
   supplied error code value CODE or the global variable errno, preceded by
   another colon and space unless the FMT argument is NULL.
   The output is terminated by a newline character.  */
extern void warn (const char *__fmt, ...)
     __attribute__ ((__fmt__ (__printf__, 1, 2)));
extern void warnc (int __code, const char *__fmt, ...)
     __attribute__ ((__fmt__ (__printf__, 2, 3)));
extern void vwarn (const char *__fmt, __gnuc_va_list)
     __attribute__ ((__fmt__ (__printf__, 1, 0)));
extern void vwarnc (int __code, const char *__fmt, __gnuc_va_list)
     __attribute__ ((__fmt__ (__printf__, 2, 0)));

/* Like warn and vwarn, but without colon, space and error message.  */
extern void warnx (const char *__fmt, ...)
     __attribute__ ((__fmt__ (__printf__, 1, 2)));
extern void vwarnx (const char *__fmt, __gnuc_va_list)
     __attribute__ ((__fmt__ (__printf__, 1, 0)));

/* Like warn, warnc, vwarn, vwarnc, warnx and vwarnx but then exit with
   EVAL.  */
extern void err (int __eval, const char *__fmt, ...)
     __attribute__ ((__noreturn__, __fmt__ (__printf__, 2, 3)));
extern void errc (int __eval, int __code, const char *__fmt, ...)
     __attribute__ ((__noreturn__, __fmt__ (__printf__, 3, 4)));
extern void verr (int __eval, const char *__fmt, __gnuc_va_list)
     __attribute__ ((__noreturn__, __fmt__ (__printf__, 2, 0)));
extern void verrc (int __eval, int __code, const char *__fmt, __gnuc_va_list)
     __attribute__ ((__noreturn__, __fmt__ (__printf__, 3, 0)));
extern void errx (int __eval, const char *__fmt, ...)
     __attribute__ ((__noreturn__, __fmt__ (__printf__, 2, 3)));
extern void verrx (int __eval, const char *__fmt, __gnuc_va_list)
     __attribute__ ((__noreturn__, __fmt__ (__printf__, 2, 0)));

#if 0
/* Not yet supported.  */

/* Sets the output stream used by the other functions.  Its vfp argument must
   be either a pointer to an open stream (possibly already converted to void *)
   or a null pointer (in which case the output stream is set to standard
   error).  */
extern void err_set_file (void *__vfp);

/* Can be used to specify a function which is called before exit(3) to perform
   any necessary cleanup; passing a null function pointer for exitf resets the
   hook to do nothing.  */
extern void err_set_exit (void (*__exitf)(int));
#endif

__END_DECLS

#endif

