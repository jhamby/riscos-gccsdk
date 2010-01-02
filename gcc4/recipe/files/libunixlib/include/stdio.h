/*
 * ANSI Standard 4.9: Input/Output <stdio.h>.
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __STDIO_H

#if !defined __need_FILE
#define __STDIO_H
#endif

#include <features.h>
#include <unixlib/types.h>
#define __need_size_t
#define __need_NULL
#include <stddef.h>

#if !defined __FILE_defined && (defined __STDIO_H || defined __need_FILE)
#define __FILE_defined
typedef struct __iobuf FILE;
#endif
#undef __need_FILE

#ifdef __STDIO_H

#define __need___va_list
#include <stdarg.h>

#define __need_pthread_t
#include <pthread.h>

__BEGIN_DECLS

__BEGIN_NAMESPACE_STD
typedef __off_t fpos_t;
__END_NAMESPACE_STD

#define TEMP_FAILURE_RETRY(x) x

/* Maximum number of files that can be open at once.
   Keep in sync with <limits.h>, _POSIX_OPEN_MAX.  */
#define FOPEN_MAX	64

/* Maximum length of a filename.
   Keep in sync with <limits.h>, _POSIX_NAME_MAX.  */
#define FILENAME_MAX	252

/* Default buffer size.  */
#define BUFSIZ		4096

/* End of file character.  */
#define EOF		(-1)

/* The possibilities for the third argument to fseek.
   <unistd.h> has the same definitions.  */

/* Seek from beginning of file.  */
#ifndef SEEK_SET
#define SEEK_SET	0
#endif
/* Seek from current position.  */
#ifndef SEEK_CUR
#define SEEK_CUR	1
#endif
/* Seek from end of file.  */
#ifndef SEEK_END
#define SEEK_END	2
#endif

/* The mode of file input/output. */
typedef union
{
  struct
  {
    unsigned int __read:1; /* Open for reading.  */
    unsigned int __write:1; /* Open for writing.  */
    unsigned int __append:1; /* Open for appending.  */
    unsigned int __binary:1; /* Opened binary.  */
    unsigned int __create:1; /* Create the file.  */
    unsigned int __exclusive:1; /* Error if it already exists.  */
    unsigned int __truncate:1; /* Truncate the file on opening.  */
  } __bits;
  unsigned char __allbits;
} __io_mode;

struct __iobuf
{
  unsigned int __magic;  /* Magic number for stream validation.  */
  unsigned char *i_ptr;
  unsigned char *i_base;
  int i_cnt;
  unsigned char *o_ptr;
  unsigned char *o_base;
  int o_cnt;
  size_t __bufsize; /* size of buffer.  */
  fpos_t __offset; /* current file position of underlying file descr. */
  __io_mode __mode; /* file access mode */
  unsigned char __pushedchar; /* character that has been pushed back */
  int __pushedi_cnt; /* position of i_cnt before char was pushed back */
  unsigned int __pushedback:1; /* ungetc has pushed back a character */
  unsigned int __eof:1; /* end of file encountered */
  unsigned int __error:1; /* error encountered */
  unsigned int __iuserbuf:1; /* buffer from user (should not be freed) */
  unsigned int __ouserbuf:1; /* buffer from user (should not be freed) */
  unsigned int __linebuf:1; /* flush on newline */
  unsigned int __ispipe:1; /* nonzero if opened by popen */
  unsigned int __string_istream:1; /* nonzero if string input stream */
  int fd; /* File descriptor.  */
  pthread_mutex_t lock; /* For multi-threaded I/O.  */
  FILE *next; /* next FILE in the linked list */
};

/* fill buffer */
extern int __filbuf (FILE *__stream) __THROW;
/* flush buffer */
extern int __flsbuf (int __c, FILE *__stream) __THROW;

/* Standard streams.  ANSI says these are macros.  */
extern FILE *__stdin, *__stdout, *__stderr;
#define stdin __stdin
#define stdout __stdout
#define stderr __stderr

#define _IOBF		0000070

/* The possibilities for the third argument to setvbuf.  */

/* No buffering.  */
#define _IONBF		0000010
/* Line buffering.  */
#define _IOLBF		0000020
/* Full buffering.  */
#define _IOFBF		0000040

__BEGIN_NAMESPACE_STD

/* Return the EOF indicator for stream.  */
extern int feof (FILE *__stream) __THROW;

/* Return the error indicator for stream.  */
extern int ferror (FILE *__stream) __THROW;

extern void clearerr (FILE *__stream) __THROW;

#define feof(stream) ((stream)->__eof != 0)
#define ferror(stream) ((stream)->__error != 0)

#ifdef __USE_MISC
/* Faster versions when locking is not required.  */
extern void clearerr_unlocked (FILE *__stream) __THROW;
#endif

/* Print a message describing the meaning of the value of errno.
   This function is a cancellation point.  */
extern void perror (const char *__s);

/* Open a file and create a new stream for it.
   This function is a cancellation point.  */
extern FILE *fopen (const char *__restrict __filename,
		    const char *__restrict __mode);

/* Open a file, replacing an existing stream with it.
   This function is a cancellation point.  */
extern FILE *freopen (const char *__restrict __filename,
		      const char *__restrict __mode,
		      FILE *__restrict __stream);

/* Close stream, or all streams if stream is null.
   This function is a cancellation point.  */
extern int fclose (FILE *__stream);

/* Flush stream, or all streams if stream is null.
   This function is a cancellation point.  */
extern int fflush (FILE *__stream);

/* Clears the buffer of a given stream. */
extern int fpurge (FILE *__stream);

/* Read chunks of generic data from stream.  This function is a cancellation
   point.  */
extern size_t fread (void *__restrict __ptr, size_t __size,
		     size_t __n, FILE *__restrict __stream);

/* Write chunks of generic data to stream.
   This function is a cancellation point.  */
extern size_t fwrite (const void *__restrict __ptr, size_t __size,
		      size_t __n, FILE *__restrict __stream);

/* If buf is null, make stream unbuffered.
   If not null, use buffer buf of size BUFSIZ.  */
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __THROW;

/* Make stream use buffering mode 'mode'.
   If buf is not NULL, use n bytes of it for buffering;
   else allocate an internal buffer n bytes long.  */
extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
		    int __modes, size_t __n) __THROW;

/* Push a character back onto the input buffer of stream.
   This function is a cancellation point.  */
extern int ungetc (int __c, FILE *__stream);

/* Get stream's position.
   This function is a cancellation point.  */
extern int fgetpos (FILE *__stream, fpos_t *__pos);

/* Set stream's position.
   This function is a cancellation point.  */
extern int fsetpos (FILE *__stream, const fpos_t *__pos);

/* Seek to a certain position on stream.
   This function is a cancellation point.  */
extern int fseek (FILE *__stream, long int __off, int __whence);

/* Return the current position of stream.
   This function is a cancellation point.  */
extern long ftell (FILE *__stream);

/* Rewind to the beginning of stream.
   This function is a cancellation point.  */
extern void rewind (FILE *__stream);
__END_NAMESPACE_STD

#if defined __USE_LARGEFILE || defined __USE_XOPEN2K
/* This function is a cancellation point.  */
extern int fseeko (FILE *__stream, __off_t __off, int __whence);

/* This function is a cancellation point.  */
extern __off_t ftello (FILE *__stream) __nonnull ((1));
#endif

__BEGIN_NAMESPACE_STD
/* This function is a cancellation point.  */
extern int getc (FILE *__stream) __nonnull ((1));

/* This function is a cancellation point.  */
extern int getchar (void);

/* Read a character from stream.  This function is a cancellation point.   */
extern int fgetc (FILE *__stream) __nonnull ((1));
__END_NAMESPACE_STD


/* Read a character from stdin.  */
#define getchar() getc(stdin)

#if defined __USE_POSIX || defined __USE_MISC

/* These functions are cancellation points.  */
extern int getc_unlocked (FILE *__stream) __nonnull ((1));
extern int getchar_unlocked (void);

/* Read a character from stream.  */
#define getc_unlocked(f) \
	((--((f)->i_cnt) >= 0 ? *((f)->i_ptr)++ : __filbuf(f)))
#define getchar_unlocked() getc_unlocked(stdin)

extern int fputc_unlocked (int __c, FILE *__stream) __nonnull ((2));

extern int putc_unlocked (int __c, FILE *__stream) __nonnull ((2));
#endif

__BEGIN_NAMESPACE_STD

/* Write a character to stream.  This function is a cancellation point.  */
extern int putc (int __c, FILE *__stream) __nonnull ((2));

/* Write a character to stream.  This function is a cancellation point.  */
extern int fputc (int __c, FILE *__stream) __nonnull ((2));

/* Write a character to stdout.  This function is a cancellation point.  */
extern int putchar (int __c);

/* Get a newline-terminated string of finite length from stream.
   This function is a cancellation point.  */
extern char *fgets (char *__s, int __n, FILE *__stream)
     __nonnull ((1, 3));

/* Get a newline-terminated string from stdin, removing the newline.  */
extern char *gets (char *__s);

/* Write a string to stream.  */
extern int fputs (const char *__restrict __s, FILE *__restrict __stream)
     __THROW __nonnull ((1, 2));

/* Write a string, followed by a newline, to stdout.  */
extern int puts (const char *__s) __THROW __nonnull ((1));

__END_NAMESPACE_STD

#ifdef __USE_GNU
extern char *fgets_unlocked (char *__restrict __s, int __n,
			     FILE *__restrict __stream);
#define fgets_unlocked fgets
#endif

/* Formatted I/O */

__BEGIN_NAMESPACE_STD

/* Write formatted output to s from argument list arg.  */
extern int vsprintf (char *__restrict __s,
		     const char *__restrict __format, __gnuc_va_list __arg)
     __THROW;

/* Write formatted output to stream from arg list arg.  This function
   is a cancellation point.  Thread-safe version.  */
extern int vfprintf (FILE *__restrict __stream,
		     const char *__restrict __format, __gnuc_va_list __arg);

/* Write formatted output to stdio from arg list arg.  This function
   is a cancellation point.  */
extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

/* Write formatted output to s.  */
extern int sprintf (char *__restrict __s,
		    const char *__restrict __format, ...) __THROW;

__END_NAMESPACE_STD

#if defined __USE_BSD || defined __USE_ISOC99 || defined __USE_UNIX98
__BEGIN_NAMESPACE_C99
/* Write formatted output to s.  limit is the maximum number of characters
   to produce.  */
extern int snprintf (char *__restrict __s, size_t __limit,
		     const char *__restrict __format, ...)
     __THROW __attribute__ ((__format__ (__printf__, 3, 4)));

/* Write formatted output to s from argument list arg.  limit is the
   maximum number of characters to produce.  */
extern int vsnprintf (char *__restrict __s, size_t __limit,
		      const char *__restrict __format,
		      __gnuc_va_list __arg)
     __THROW __attribute__ ((__format__ (__printf__, 3, 0)));
__END_NAMESPACE_C99
#endif

#ifdef __USE_GNU
/* Write formatted output to s.  */
extern int asprintf (char **__restrict __ptr,
		     const char *__restrict __format, ...)
     __THROW __attribute__ ((__format__ (__printf__, 2, 3)));

/* Write formatted output to s from argument list arg.  */
extern int vasprintf (char **__restrict __ptr,
		      const char *__restrict __format, __gnuc_va_list __arg)
     __THROW __attribute__ ((__format__ (__printf__, 2, 0)));
#endif

__BEGIN_NAMESPACE_STD
/* Write formatted output to stream.  This function is a cancellation
   point.  */
extern int fprintf (FILE *__restrict __stream,
		    const char *__restrict __format, ...);

/* Write formatted output to stdout.  This function is a cancellation
   point.  */
extern int printf (const char *__restrict __format, ...);

/* Read formatted input from s.  */
extern int sscanf (const char *__restrict __s,
		   const char *__restrict __format, ...) __THROW;

/* Read formatted input from stream.  This function is a cancellation
   point.  */
extern int fscanf (FILE *__restrict __stream,
		   const char *__restrict __format, ...);

/* Read formatted input from stdin.  This function is a cancellation
   point.  */
extern int scanf (const char *__restrict __format, ...);

__END_NAMESPACE_STD

#ifdef __USE_ISOC99
__BEGIN_NAMESPACE_C99

/* Read formatted input from stdin into argument list arg.  This
   function is a cancellation point.  */
extern int vscanf (const char *__restrict __format, __gnuc_va_list __ap)
     __attribute__ ((__format__ (__scanf__, 1, 0)));

/* Read formatted input from stream into argument list arg.  This
   function is a cancellation point.  */
extern int vfscanf (FILE *__restrict __stream,
		    const char *__restrict __format, __gnuc_va_list __ap)
     __attribute__ ((__format__ (__scanf__, 2, 0)));

/* Read formatted input from 's' into argument list arg.  */
extern int vsscanf (const char *__restrict __s,
		    const char *__restrict __format, __gnuc_va_list __ap)
     __THROW __attribute__ ((__format__ (__scanf__, 2, 0)));

__END_NAMESPACE_C99
#endif

/* How long an array of chars must be to be passed to tmpnam.  */
#define L_tmpnam	255

/* The minimum number of unique filenames generated by tmpnam.  */
#define TMP_MAX 	217672836

__BEGIN_NAMESPACE_STD
/* Remove file filename.  */
extern int remove (const char *__filename) __THROW;

/* Rename file oldname to newname.  */
extern int rename(const char *__old, const char *__newname) __THROW;

/* Create a temporary file and open it read/write.
   This function is a cancellation point.  */
extern FILE *tmpfile (void);


/* Generate a temporary filename.  */
extern char *tmpnam (char *__s) __THROW;

__END_NAMESPACE_STD

#ifdef __USE_MISC
/* Re-entrant version of tmpnam(). 's' must not be null.  */
extern char *tmpnam_r (char *__s) __THROW;
#endif

#if defined __USE_SVID || defined __USE_XOPEN
/* Return a pointer to a malloc'ed unique temporary filename, including
   pathname.  */
extern char *tempnam (const char *__dir, const char *__prefix)
     __THROW __attribute_malloc__;
#endif

/* Generate a unique temporary file name for temp.  */
extern char *mktemp(char *__temp) __THROW;

/* As for mktemp but returns an open file descriptor on the file.  */
extern int mkstemp(char *__temp) __THROW;

/* System V enhancements.  */

#if defined __USE_SVID || defined __USE_MISC \
    || (defined __USE_XOPEN && !defined __USE_XOPEN2K)
/* Get a word (int) from stream.  */
extern int getw (FILE *__stream);

/* Write a word (int) to stream.  */
extern int putw (int __w, FILE *__stream);
#endif

#if defined __USE_SVID || defined __USE_XOPEN
/* Default path prefix for tempnam and tmpnam.  */
#define P_tmpdir "/tmp"
#endif

/* BSD enhancements.  */

#ifdef __USE_BSD
/* If BUF is NULL, make STREAM unbuffered.
   Else make it use SIZE bytes of BUF for buffering.  */
extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
		       size_t __size) __THROW;

/* Make STREAM line-buffered.  */
extern void setlinebuf (FILE *__stream) __THROW;
#endif

extern int sys_nerr;
/* This is also defined in <errno.h> */
extern const char *sys_errlist[];

/* POSIX enhancements.  */

#ifdef __USE_POSIX
/* Create a new stream that refers to an existing system file descriptor.  */
extern FILE *fdopen (int __fd, const char *__modes) __THROW;

/* Return the system file descriptor for stream.  */
extern int fileno (FILE *__stream) __THROW;
#define fileno(f)	((f)->fd)

#define L_ctermid 16

/* Return the name of the controlling terminal.  */
extern char *ctermid (char *__s) __THROW;

#endif

#ifdef __USE_XOPEN
/* Return the name of the current user.  */
extern char *cuserid (char *__s);
#endif

/* POSIX 2 enhancements.  */

#if (defined __USE_POSIX2 || defined __USE_SVID  || defined __USE_BSD || \
     defined __USE_MISC)
/* Create a new stream connected to a pipe running the given command.
   This function is a cancellation point.  */
extern FILE *popen (const char *__command, const char *__modes);

/* Close a stream opened by popen and return the status of its child.
   This function is a cancellation point.  */
extern int pclose (FILE *__stream);
#endif

/* GNU extenstions.  */

#ifdef __USE_GNU
/* Read an entire line from stream (upto newline), storing the text in
   a buffer and storing the buffer address in *lineptr.  */
extern __ssize_t getline (char **__restrict __lineptr,
			  size_t *__restrict __n,
			  FILE *__restrict __stream);

/* Similar to getline except that the line terminator doesn't
   have to be a newline.  */
extern __ssize_t getdelim (char **__restrict __lineptr,
			   size_t *__restrict __n,
			   int __delimiter,
			   FILE *__restrict __stream);
#endif

#if defined __USE_XOPEN && !defined __USE_XOPEN2K && !defined __USE_GNU
#include <getopt.h>
#endif

__END_DECLS

#endif /* __STDIO_H */
#endif /* ! __STDIO_H */
