/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/stdio.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* ANSI Standard 4.9: Input/Output <stdio.h>.  */

#ifndef __STDIO_H
#define __STDIO_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif
#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif
#ifndef __STDDEF_H
#include <stddef.h>
#endif

typedef char *__va_list;

#ifdef __cplusplus
extern "C" {
#endif

typedef __off_t fpos_t;


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

#ifndef __FILE_declared
#define __FILE_declared 1
typedef struct __iobuf FILE;
#endif

/* The mode of file input/output. */
typedef struct
{
  unsigned int __read:1; /* Open for reading.  */
  unsigned int __write:1; /* Open for writing.  */
  unsigned int __append:1; /* Open for appending.  */
  unsigned int __binary:1; /* Opened binary.  */
  unsigned int __create:1; /* Create the file.  */
  unsigned int __exclusive:1; /* Error if it already exists.  */
  unsigned int __truncate:1; /* Truncate the file on opening.  */
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
  FILE *next; /* next FILE in the linked list */
};

#ifdef __UNIXLIB_INTERNALS

/* Magic number to fill __magic.  */
#define _IOMAGIC 0xfedabeeb

#if __INTEGRITY_CHECK
/* Nonzero if stream is a valid stream.  */
#define __validfp(stream) (stream != NULL && stream->__magic == _IOMAGIC)
#else
#define __validfp(stream) (1)
#endif

/* Invalidate a stream.  */
extern void __invalidate (FILE *__stream);

/* Make a new stream.  */
extern FILE *__newstream (void);

/* Initialise a new stream.  */
extern FILE *__stream_init (int __fd, FILE *__stream);

/* Dissect the given mode string into an __io_mode.  */
extern __io_mode __getmode (const char *__mode);

extern void __stdioinit(void);	/* initialise stdin,stdout & stderr */
extern void __stdioexit(void);	/* close streams & delete tmpfile() */

/* Return the next character in the input buffer, keeping it in
   the input buffer.  If the buffer is empty, then fill it.  */
extern int __peek_char (FILE *__stream);

#define __STDIOLIB__ static void (*__stdiolib)(void) = __stdioinit;

/* All streams are in a linked list.
   This is the head of the list.  */
extern FILE *__iob_head;

#endif  /* __UNIXLIB_INTERNALS */

/* fill buffer */
extern int __filbuf (FILE *__stream);
/* flush buffer */
extern int __flsbuf (int __c, FILE *__stream);

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

/* Return the EOF indicator for stream.  */
extern int feof (FILE *__stream);
/* Return the error indicator for stream.  */
extern int ferror (FILE *__stream);
extern int fisatty (FILE *__stream);

extern void clearerr (FILE *__stream);

#define feof(stream) ((stream)->__eof != 0)
#define ferror(stream) ((stream)->__error != 0)

/* Print a message describing the meaning of the value of errno. */
extern void perror (const char *__s);

/* Open a file and create a new stream for it.  */
extern FILE *fopen (const char *__filename, const char *__mode);
/* Open a file, replacing an existing stream with it.  */
extern FILE *freopen (const char *__filename, const char *__mode,
       	     	      FILE *__stream);

/* Close stream, or all streams if stream is null.  */
extern int fclose (FILE *__stream);
/* Flush stream, or all streams if stream is null.  */
extern int fflush (FILE *__stream);

/* Read chunks of generic data from stream.  */
extern size_t fread (void *__ptr, size_t __size,
       	      	     size_t __n, FILE *__stream);

/* Write chunks of generic data to stream.  */
extern size_t fwrite (const void *__ptr, size_t __size,
       	      	      size_t __n, FILE *__stream);

/* If buf is null, make stream unbuffered.
   If not null, use buffer buf of size BUFSIZ.  */
extern void setbuf (FILE *__stream, char *__buf);
/* Make stream use buffering mode 'mode'.
   If buf is not NULL, use n bytes of it for buffering;
   else allocate an internal buffer n bytes long.  */
extern int setvbuf (FILE *__stream, char *__buf, int __modes, size_t __n);

/* Push a character back onto the input buffer of stream.  */
extern int ungetc (int __c, FILE *__stream);

/* Get stream's position.  */
extern int fgetpos (FILE *__stream, fpos_t *__pos);
/* Set stream's position.  */
extern int fsetpos (FILE *__stream, const fpos_t *__pos);

/* Seek to a certain position on stream.  */
extern int fseek (FILE *__stream, long int __off, int __whence);
/* Return the current position of stream.  */
extern long ftell (FILE *__stream);
/* Rewind to the beginning of stream.  */
extern void rewind (FILE *__stream);

extern int getc (FILE *__stream);
extern int getchar (void);

/* Read a character from stream.  */
#define getc(f) \
	((--((f)->i_cnt) >= 0 ? *((f)->i_ptr)++ : __filbuf(f)))

/* Read a character from stdin.  */
#define getchar() getc(stdin)

/* Read a character from stream.  */
extern int fgetc (FILE *__stream);

/* Write a character to stream. */
extern int putc (int __c, FILE *__stream);

#define putc(c,f) \
	(((f)->__linebuf && (c) == '\n') ? __flsbuf(c,f) : \
	((--((f)->o_cnt) > 0 ? (*((f)->o_ptr)++ = (c)) : __flsbuf(c,f))))

extern int fputc (int __c, FILE *__stream);

/* Write a character to stdout.  */
extern int putchar (int __c);

#define putchar(ch) putc(ch, stdout)

/* Get a newline-terminated string of finite length from stream.  */
extern char *fgets (char *__s, size_t __n, FILE *__stream);
/* Get a newline-terminated string from stdin, removing the newline.  */
extern char *gets (char *__s);

/* Write a string to stream.  */
extern int fputs (const char *__s, FILE *__stream);
/* Write a string, followed by a newline, to stdout.  */
extern int puts (const char *__s);

/* Formatted I/O */

/* I know I said I would not do this, but ... */

/* Write formatted output to s from argument list arg.  */
extern int vsprintf (char *__s, const char *__format, __va_list __arg);
/* Write formatted output to stream from arg list arg.  */
extern int vfprintf (FILE *__stream, const char *__format, __va_list __arg);
/* Write formatted output to stdio from arg list arg.  */
extern int vprintf (const char *__format, __va_list __arg);

#ifndef __GNUC__
#pragma -v1
#endif

/* Write formatted output to s.  */
extern int sprintf (char *__s, const char *__format, ...);
/* Write formatted output to stream.  */
extern int fprintf (FILE *__stream, const char *__format, ...);
/* Write formatted output to stdout.  */
extern int printf (const char *__format, ...);

#ifndef __GNUC__
#pragma -v2
#endif

/* Read formatted input from s.  */
extern int sscanf (const char *__s, const char *__format, ...);
/* Read formatted input from stream.  */
extern int fscanf (FILE *__stream, const char *__format, ...);
/* Read formatted input from stdin.  */
extern int scanf (const char *__format, ...);

#ifndef __GNUC__
#pragma -v0
#endif

/* Read formatted input from stdin into argument list arg.  */
extern int vscanf (const char *__format, __va_list __ap);
/* Read formatted input from stream into argument list arg.  */
extern int vfscanf (FILE *__stream, const char *__format, __va_list __ap);
/* Read formatted input from 's' into argument list arg.  */
extern int vsscanf (const char *__s, const char *__format, __va_list __ap);

/* How long an array of chars must be to be passed to tmpnam.  */
#define L_tmpnam	255
/* The minimum number of unique filenames generated by tmpnam.  */
#define TMP_MAX 	217672836

/* Remove file filename.  */
extern int remove (const char *__filename);

/* Rename file oldname to newname.  */
extern int rename(const char *__old, const char *__newname);

/* Create a temporary file and open it read/write.  */
extern FILE *tmpfile (void);
/* Generate a temporary filename.  */
extern char *tmpnam (char *__s);
/* Re-entrant version of tmpnam(). 's' must not be null.  */
extern char *tmpnam_r (char *__s);

/* Return a pointer to a malloc'ed unique temporary filename, including
   pathname.  */
extern char *tempnam (const char *__dir, const char *__prefix);

/* Generate a unique temporary file name for temp.  */
extern char *mktemp(char *__temp);
/* As for mktemp but returns an open file descriptor on the file.  */
extern int mkstemp(char *__temp);

/* System V enhancements.  */

/* Get a word (int) from stream.  */
extern int getw (FILE *__stream);
/* Write a word (int) to stream.  */
extern int putw (int __w, FILE *__stream);

/* Default path prefix for tempnam and tmpnam.  */
#define P_tmpdir "/tmp"


/* BSD enhancements.  */

/* If BUF is NULL, make STREAM unbuffered.
   Else make it use SIZE bytes of BUF for buffering.  */
extern void setbuffer (FILE *__stream, char *__buf, size_t __size);

/* Make STREAM line-buffered.  */
extern void setlinebuf (FILE *__stream);

extern int sys_nerr;
/* This is also defined in <errno.h> */
extern char *sys_errlist[];

/* POSIX enhancements.  */

/* Create a new stream that refers to an existing system file descriptor.  */
extern FILE *fdopen (int __fd, const char *__modes);

/* Return the system file descriptor for stream.  */
extern int fileno (FILE *__stream);
#define fileno(f)	((f)->fd)

#define L_ctermid 16

/* Return the name of the controlling terminal.  */
extern char *ctermid (char *__s);

/* Return the name of the current user.  */
extern char *cuserid (char *__s);

/* POSIX 2 enhancements.  */

/* Create a new stream connected to a pipe running the given command.  */
extern FILE *popen (const char *__command, const char *__modes);

/* Close a stream opened by popen and return the status of its child.  */
extern int pclose (FILE *__stream);


/* GNU extenstions.  */

/* Read an entire line from stream (upto newline), storing the text in
   a buffer and storing the buffer address in *lineptr.  */
extern __ssize_t getline (char **__lineptr, size_t *__n, FILE *__stream);

/* Similar to getline except that the line terminator doesn't
   have to be a newline.  */
extern __ssize_t getdelim (char **__lineptr, size_t *__n,
       		 	   int __delimiter, FILE *__stream);

#ifdef __cplusplus
	}
#endif

#endif
