/*
 * ANSI Standard 4.9: Input/Output <stdio.h>.
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __STDIO_H

#if !defined __need_FILE && !defined __need___FILE
# define __STDIO_H 1
# include <features.h>

__BEGIN_DECLS

# define __need_size_t
# define __need_NULL
# include <stddef.h>

# include <unixlib/types.h>
# define __need_FILE
# define __need___FILE
#endif

#if !defined __FILE_defined && defined __need_FILE
struct __iobuf;

__BEGIN_NAMESPACE_STD
typedef struct __iobuf FILE;
__END_NAMESPACE_STD
#if defined __USE_LARGEFILE64 || defined __USE_SVID || defined __USE_POSIX \
    || defined __USE_BSD || defined __USE_ISOC99 || defined __USE_XOPEN \
    || defined __USE_POSIX2
__USING_NAMESPACE_STD(FILE)
#endif

# define __FILE_defined 1
#endif
#undef __need_FILE

#if !defined ____FILE_defined && defined __need___FILE
typedef struct __iobuf __FILE;
# define ____FILE_defined 1
#endif
#undef	__need___FILE


#ifdef __STDIO_H

#define __need___va_list
#include <stdarg.h>

#ifndef __TARGET_SCL__
#  define __need_pthread_t
#  include <pthread.h>
#endif

__BEGIN_NAMESPACE_STD
typedef __off_t fpos_t;
__END_NAMESPACE_STD

#define TEMP_FAILURE_RETRY(x) x

/* Maximum number of files that can be open at once.
   Keep in sync with <limits.h>, _POSIX_OPEN_MAX.  */
#ifdef __TARGET_SCL__
#  define FOPEN_MAX	8
#else
#  define FOPEN_MAX	64
#endif
#ifdef __TARGET_SCL__
/* Number of open files that is supported by SCL.  */
#  define _SYS_OPEN 16
#endif

/* Maximum length of a filename.
   Keep in sync with <limits.h>, _POSIX_NAME_MAX.  */
#ifdef __TARGET_SCL__
#  define FILENAME_MAX	80
#else
#  define FILENAME_MAX	252
#endif

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
#ifdef __TARGET_SCL__
struct __iobuf
{
  unsigned char *__ptr;
  int __icnt;
  int __ocnt;
  int __flag;
  /* Following fields are internal.  */
  unsigned char *__base;
  int __file;
  long __pos;
  int __bufsiz;
  int __signature;
  struct __extradata *__extrap;
};
#else
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
#endif

/* fill buffer (for internal use only).  */
extern int __filbuf (FILE *__stream) __THROW;
/* flush buffer (for internal use only).  */
extern int __flsbuf (int __c, FILE *__stream) __THROW;

#ifdef __TARGET_SCL__
/* Standard streams.  */
extern FILE __iob[];

#  define stdin (&__iob[0])
#  define stdout (&__iob[1])
#  define stderr (&__iob[2])

#  define _IOEOF	0x40
#  define _IOERR	0x80
/* Full buffering.  */
#  define _IOFBF	0x100
/* Line buffering.  */
#  define _IOLBF	0x200
/* No buffering.  */
#  define _IONBF	0x400
/* Default buffer size.  */
#  define BUFSIZ	(4096)
#else
/* Standard streams.  ANSI says these are macros.  */
extern FILE *__stdin, *__stdout, *__stderr;
#  define stdin __stdin
#  define stdout __stdout
#  define stderr __stderr

#  define _IOBF		0000070

/* The possibilities for the third argument to setvbuf.  */

/* No buffering.  */
#  define _IONBF	0000010
/* Line buffering.  */
#  define _IOLBF	0000020
/* Full buffering.  */
#  define _IOFBF	0000040
#endif

__BEGIN_NAMESPACE_STD

/* Return nonzero if the end-of-file indicator for stream 'stream' is set.  */
extern int feof (FILE *__stream) __THROW;
#ifdef __TARGET_SCL__
#  define feof(stream) ((stream)->__flag & _IOEOF)
#else
#  define feof(stream) ((stream)->__eof != 0)
#endif

/* Return nonzero if the error indicator for the stream 'stream' is set.  */
extern int ferror (FILE *__stream) __THROW;
#ifdef __TARGET_SCL__
#  define ferror(stream) ((stream)->__flag & _IOERR)
#else
#  define ferror(stream) ((stream)->__error != 0)
#endif

/* Clears the end-of-file and error indicators for the stream
   'stream'.  */
extern void clearerr (FILE *__stream) __THROW;

#ifdef __USE_MISC
#  ifndef __TARGET_SCL__
/* Faster versions when locking is not required.  */
extern void clearerr_unlocked (FILE *__stream) __THROW;
#  endif
#endif

/* Print a message describing the meaning of the value of errno.
   This function is a cancellation point.  */
extern void perror (const char *__s);

/* Open a file and create a new stream for it.
   This function is a cancellation point.  */
extern FILE *fopen (const char *__restrict __filename,
		    const char *__restrict __mode);

/* Close the stream 'stream', ignoring any errors. Then 'filename'
   is opened with 'mode' with the same stream object 'stream'.
   Returns NULL on failure.  Usually used to connect to standard
   streams e.g. stdin, stdout or stderr.
   This function is a cancellation point.  */
extern FILE *freopen (const char *__restrict __filename,
		      const char *__restrict __mode,
		      FILE *__restrict __stream);

/* Cause 'stream' to be closed (or when 'stream' is NULL, all streams to
   be closed) and the connection to the corresponding file to be broken.
   All buffered output is written and buffered input is discarded.
   Returns 0 on success, EOF if an error was detected.
   This function is a cancellation point.  */
extern int fclose (FILE *__stream);

/* Cause any buffered output on 'stream' to be delivered to the file.
   If 'stream' is null, then fflush causes buffered output on all open
   output streams. Returns EOF if a write error occurs, zero otherwise.
   This function is a cancellation point.  */
extern int fflush (FILE *__stream);

/* Clears the buffer of a given stream. */
extern int fpurge (FILE *__stream);

/* Read up to 'count' objects of size 'size' into the array 'ptr',
   from the stream 'stream'. Return the number of objects actually
   read.
   This function is a cancellation point.  */
extern size_t fread (void *__restrict __ptr, size_t __size,
		     size_t __count, FILE *__restrict __stream);

/* Write up to 'count' objects of size 'size' from the array 'ptr',
   to the stream 'stream'. The return value is normally 'count' if the
   call succeeds.
   This function is a cancellation point.  */
extern size_t fwrite (const void *__restrict __ptr, size_t __size,
		      size_t __count, FILE *__restrict __stream);

/* If buf is null, make stream unbuffered.
   If not null, use buffer buf of size BUFSIZ.  */
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __THROW;

/* Specify that the stream 'stream' should have the buffering mode
   'mode', which can be either _IOFBF (full buffering), _IOLBF (line
   buffering) or _IONBF (unbuffered input/output).
   If 'buf' is NULL, then setvbuf allocates a buffer itself using
   malloc. This is freed when the stream is closed.
   Otherwise 'buf' is a character array of 'size' characters.  */
extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
		    int __mode, size_t __size) __THROW;

/* Pushes back the character 'c' onto the input stream 'stream'.
   The next input from 'stream' will read 'c' before anything else.
   If 'c' is EOF, ungetc does nothing and just returns EOF.
   This function is a cancellation point.  */
extern int ungetc (int __c, FILE *__stream);

/* Store the value of the file position indicator for the
   stream 'stream' in the fpos_t object pointed to by 'position'.
   fgetpos returns zero on success.
   This function is a cancellation point.  */
extern int fgetpos (FILE *__stream, fpos_t *__pos);

/* Set the file position indicator for the stream 'stream' to the
   position 'position', which must be set by a previous call to
   fgetpos.
   This function is a cancellation point.  */
extern int fsetpos (FILE *__stream, const fpos_t *__pos);

/* Change the file position of the stream 'stream'. 'whence'
   must be one of the constants SEEK_SET, SEEK_CUR, SEEK_END,
   to indicate the meaning of the relative 'offset'.
   This function is a cancellation point.  */
extern int fseek (FILE *__stream, long int __off, int __whence);

/* Return the current file position of the stream 'stream'.
   If a failure occurs, -1 is returned.
   This function is a cancellation point.  */
extern long ftell (FILE *__stream) __wur;

/* Positions the stream 'stream' at the beginning of the file.
   Equivalent to fseek (stream, 0, SEEK_SET).
   This function is a cancellation point.  */
extern void rewind (FILE *__stream);
__END_NAMESPACE_STD

#if defined __USE_LARGEFILE || defined __USE_XOPEN2K
/* Seek to a position in given stream.
   This function is a cancellation point.  */
extern int fseeko (FILE *__stream, __off_t __off, int __whence);

/* Get the current position of stream.
   This function is a cancellation point.  */
extern __off_t ftello (FILE *__stream) __nonnull ((1)) __wur;
#endif

__BEGIN_NAMESPACE_STD
/* This function is a cancellation point.  */
extern int getc (FILE *__stream) __nonnull ((1));

/* This function is a cancellation point.  */
extern int getchar (void);

/* Read the next character as an unsigned char from the stream
   'stream' and return its value, converted to an int.  EOF
   is returned on read error/end-of-file.
   This function is a cancellation point.   */
extern int fgetc (FILE *__stream) __nonnull ((1));
__END_NAMESPACE_STD


/* Read a character from stdin.  */
#define getchar() getc(stdin)

#if defined __USE_POSIX || defined __USE_MISC

#  ifndef __TARGET_SCL__
/* These functions are cancellation points.  */
extern int getc_unlocked (FILE *__stream) __nonnull ((1));
extern int getchar_unlocked (void);
#  endif

/* Read a character from stream.  */
#  ifdef __TARGET_SCL__
#    define getc(p) \
	(--((p)->__icnt) >= 0 ? *((p)->__ptr)++ : __filbuf(p))
#    define getc_unlocked(p) getc(p)
#  else
#    define getc_unlocked(f) \
	((--((f)->i_cnt) >= 0 ? *((f)->i_ptr)++ : __filbuf(f)))
#  endif
#  define getchar_unlocked() getc_unlocked(stdin)

#  ifdef __TARGET_SCL__
extern int fputc_unlocked (int __c, FILE *__stream) __nonnull ((2));

extern int putc_unlocked (int __c, FILE *__stream) __nonnull ((2));
#  endif
#endif

__BEGIN_NAMESPACE_STD

/* Convert the character 'c' to type unsigned char and writes it
   to stream 'stream'.  EOF is returned if an error occurs;
   otherwise the character 'c' is returned.
   This function is a cancellation point.  */
extern int putc (int __c, FILE *__stream) __nonnull ((2));
#ifdef __TARGET_SCL__
#  define putc(ch, p) \
	(--((p)->__ocnt) >= 0 ? (*((p)->__ptr)++ = (ch)) : __flsbuf(ch,p))
#endif

/* Write a character to stream.  This function is a cancellation point.  */
extern int fputc (int __c, FILE *__stream) __nonnull ((2));

/* Write a character to stdout.  This function is a cancellation point.  */
extern int putchar (int __c);

/* Read chars from the stream 'stream' up to and including a
   newline character and stores them in the string 's'. A NUL
   character is added to mark the end of the string.  The number
   of characters to read at most is 'count - 1'.
   This function is a cancellation point.  */
extern char *fgets (char *__s, int __n, FILE *__stream)
     __nonnull ((1, 3));

/* Get a newline-terminated string from stdin, removing the newline.  */
extern char *gets (char *__s);

/* Write the string 's' top the stream 'stream'. The terminating NUL
   character is not written, and a newline character is not added, either.  */
extern int fputs (const char *__restrict __s, FILE *__restrict __stream)
     __THROW __nonnull ((1, 2));

/* Write a string, followed by a newline, to stdout.  */
extern int puts (const char *__s) __THROW __nonnull ((1));

__END_NAMESPACE_STD

#ifdef __USE_GNU
#  ifdef __TARGET_SCL__
extern char *fgets_unlocked (char *__restrict __s, int __n,
			     FILE *__restrict __stream);
#  endif
#define fgets_unlocked fgets
#endif

/* Formatted I/O */

__BEGIN_NAMESPACE_STD

/* Write formatted output to s from argument list arg.  */
extern int vsprintf (char *__restrict __s,
		     const char *__restrict __format, __gnuc_va_list __arg)
     __THROW;

/* Write formatted output to stream from arg list arg.
   This function is a cancellation point.  Thread-safe version.  */
extern int vfprintf (FILE *__restrict __stream,
		     const char *__restrict __format, __gnuc_va_list __arg);

/* Write formatted output to stdio from arg list arg.
   This function is a cancellation point.  */
extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

#ifdef __TARGET_SCL__
extern int __gcc_vsprintf (char *__restrict __s,
			   const char *__restrict __format,
                           __gnuc_va_list *__arg);
#define vsprintf(__s,__fmt,__ap) (__gcc_vsprintf(__s, __fmt, &__ap))
extern int __gcc_vfprintf (FILE *__restrict __stream,
                           const char *__restrict __format,
                           __gnuc_va_list *__arg);
#define vfprintf(__s,__fmt,__ap) (__gcc_vfprintf(__s, __fmt, &__ap))
extern int __gcc_vprintf (const char *__restrict __format,
                          __gnuc_va_list *__arg);
#define vprintf(__fmt,__ap) (__gcc_vprintf(__fmt, &__ap))
#endif

/* Similar to printf except the output is stored in the array
   's'. A NUL terminating character is also written.  */
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
#ifdef __TARGET_SCL__
extern int __gcc_vsnprintf (char *__restrict __s, size_t __limit,
			    const char *__restrict __format,
			    __gnuc_va_list *__arg)
     __attribute__ ((__format__ (__printf__, 3, 0)));
#define vsnprintf(__s,__limit,__fmt,__ap) \
	(__gcc_vsnprintf(__s,__limit,__fmt, &__ap))
#endif
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
/* Print the optional arguments under control of the template
   string 'format' to the stream stdout. Returns the number of characters
   printed, or a negative value if there was an output error.
   This function is a cancellation point.  */
extern int printf (const char *__restrict __format, ...);

/* Similar to printf except the output is written to the stream
   'stream' instead of stdout.
   This function is a cancellation point.  */
extern int fprintf (FILE *__restrict __stream,
		    const char *__restrict __format, ...);

/* Read formatted input from the stream stdin under control
   of the template 'format'. Returns the number of successful
   assignments.
   This function is a cancellation point.  */
extern int scanf (const char *__restrict __format, ...);

/* Similar to scanf but reads from the array 'string'.  */
extern int sscanf (const char *__restrict __s,
		   const char *__restrict __format, ...) __THROW;

/* Similar to scanf but reads from the stream 'stream'.
   This function is a cancellation point.  */
extern int fscanf (FILE *__restrict __stream,
		   const char *__restrict __format, ...);

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

#ifdef __TARGET_SCL__
extern int __gcc_vscanf (const char *__restrict __format,
                         __gnuc_va_list *__ap)
     __attribute__ ((__format__ (__scanf__, 1, 0)));
#define vscanf(__fmt,__ap) (__gcc_vscanf(__fmt, &__ap))
extern int __gcc_vfscanf (FILE *__restrict __stream,
			  const char *__restrict __format,
                          __gnuc_va_list *__ap)
     __attribute__ ((__format__ (__scanf__, 2, 0)));
#define vfscanf(__s,__fmt,__ap) (__gcc_vfscanf(__s, __fmt, &__ap))
extern int __gcc_vsscanf (const char *__restrict __s,
                          const char *__restrict __format,
                          __gnuc_va_list *__ap)
     __attribute__ ((__format__ (__scanf__, 2, 0)));
#define vsscanf(__s,__fmt,__ap) (__gcc_vsscanf(__s, __fmt, &__ap))
#endif

__END_NAMESPACE_C99
#endif

/* How long an array of chars must be to be passed to tmpnam.  */
#ifdef __TARGET_SCL__
#  define L_tmpnam	20
#else
#  define L_tmpnam	255
#endif

/* The minimum number of unique filenames generated by tmpnam.  */
#ifdef __TARGET_SCL
#  define TMP_MAX	1000000000
#else
#  define TMP_MAX 	217672836
#endif

__BEGIN_NAMESPACE_STD
/* Remove file filename.  */
extern int remove (const char *__filename) __THROW;

/* Rename a file called 'oldname' to 'newname'. If rename fails
   it returns -1.  */
extern int rename (const char *__oldname, const char *__newname) __THROW;

/* Create a temporary binary file for updade mode, as if calling
   fopen with mode "wb+".  The file is deleted automatically when
   it is closed or when the program terminates.
   This function is a cancellation point.  */
extern FILE *tmpfile (void);

/* Construct and return a file name that is a valid and does not
   name any existing file. If 'result' is null, the return value
   points to an internal static string. Otherwise, 'result' points
   to an array of at least L_tmpnam chars.  */
extern char *tmpnam (char *__result) __THROW;

__END_NAMESPACE_STD

#ifndef __TARGET_SCL__
#ifdef __USE_MISC
/* Re-entrant version of tmpnam(). 's' must not be null.  */
extern char *tmpnam_r (char *__result) __THROW __nonnull ((1));
#endif

#if defined __USE_SVID || defined __USE_XOPEN
/* Return a pointer to a malloc'ed unique temporary filename, including
   pathname.  */
extern char *tempnam (const char *__dir, const char *__prefix)
     __THROW __attribute_malloc__;
#endif
#endif

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
#  ifndef __TARGET_SCL__
#    define fileno(f)	((f)->fd)

#    define L_ctermid 16

/* Return the name of the controlling terminal.  */
extern char *ctermid (char *__s) __THROW;
#  endif

#endif

#ifdef __USE_XOPEN
#  ifndef __TARGET_SCL__
/* Return the name of the current user.  */
extern char *cuserid (char *__s);
#  endif
#endif

/* POSIX 2 enhancements.  */

#if (defined __USE_POSIX2 || defined __USE_SVID  || defined __USE_BSD || \
     defined __USE_MISC)
#  ifndef __TARGET_SCL__
/* Create a new stream connected to a pipe running the given command.
   This function is a cancellation point.  */
extern FILE *popen (const char *__command, const char *__modes);

/* Close a stream opened by popen and return the status of its child.
   This function is a cancellation point.  */
extern int pclose (FILE *__stream);
#  endif
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
