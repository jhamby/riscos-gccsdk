/*
Global Run Time System definitions.

Copyright (C) 1991-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         J.J. van der Heijden <j.j.vanderheijden@student.utwente.nl>
         Frank Heckenbach <frank@pascal.gnu.de>

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

As a special exception, if you link this file with files compiled
with a GNU compiler to produce an executable, this does not cause
the resulting executable to be covered by the GNU General Public
License. This exception does not however invalidate any other
reasons why the executable file might be covered by the GNU General
Public License.
*/

/*
  Edit history:

  15 6 85: First version for Pax compiler at HUT.FI.

  Later (89?) I converted this for GNU Pascal compiler (GPC).

  The run time system is rewritten for GPC and it should support
  all features of the Extended Pascal Standard.

  July '96 : adapted for GNU autoconf <j.j.vanderheijde@student.utwente.nl>
*/

#define _GNU_SOURCE

/* Created by autoconf */
#ifndef HAVE_NO_RTS_CONFIG_H
#include "rts-config.h"
#endif

#ifdef STDC_HEADERS
#include <stdlib.h>
#include <string.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_BSD_SIGNAL_H
#include <bsd/signal.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

/* needed by e.g. Solaris to stat a file system */
#ifdef HAVE_SYS_STATVFS_H
#include <sys/statvfs.h>
#endif

/* needed by e.g. Linux and DJGPP to stat a file system */
#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif

/* needed by e.g. FreeBSD to stat a file system */
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif

/* needed by e.g. mingw for getpid */
#ifdef HAVE_PROCESS_H
#include <process.h>
#endif

#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if HAVE_DIRENT_H
#include <dirent.h>
#else
#define dirent direct
#if HAVE_SYS_NDIR_H
#include <sys/ndir.h>
#endif
#if HAVE_SYS_DIR_H
#include <sys/dir.h>
#endif
#if HAVE_NDIR_H
#include <ndir.h>
#endif
#endif

/* needed by e.g. IRIX and AIX for select() and the macros like FD_SET */
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

/* This include file can be added on any system to remove
   incompatibilities etc. */
#ifdef HAVE_OS_HACKS_H
#include <os-hacks.h>
#endif

#ifndef errno
extern int errno;
#endif

/* Workaround for a bug in e.g. libc-5.4.46 */
#undef FD_ZERO
#define FD_ZERO(p) (memset ((char *) (p), 0, sizeof (*(p))))

#ifdef CLOSEDIR_VOID
#define closedir(dir) (closedir (dir), 0)
#endif

#ifndef SSIZE_MAX
#define SSIZE_MAX 16384
#endif

#ifndef S_ISLNK
#define S_ISLNK(f) 0
#endif
#ifndef S_ISCHR
#define S_ISCHR(f) 0
#endif
#ifndef S_ISBLK
#define S_ISBLK(f) 0
#endif
#ifndef S_ISFIFO
#define S_ISFIFO(f) 0
#endif
#ifndef S_ISSOCK
#define S_ISSOCK(f) 0
#endif

#ifndef HAVE_LSTAT
#define lstat stat
#endif

#ifndef HAVE_SSIZE_T
typedef signed size_t ssize_t;
#endif

#ifdef HAVE_LLSEEK
#ifndef HAVE_LOFF_T
typedef offset_t loff_t;
#endif
#endif

#ifndef HAVE_STRSIGNAL
char *strsignal (int);
#endif

#ifdef DEBUG
#define D(level, x) do if (_p_debug >= level) x; while (0)
#else
#define D(level, x)
#endif

#ifdef MACH
void *malloc (size_t);
#endif

#include "types.h"

#define NEWPAGE '\f' /* `Page' writes this */
#define NEWLINE '\n' /* `Writeln' writes this */

#define TRUE  1
#define FALSE 0

/* For Write (Boolean) */
#ifndef FALSE_str
#define FALSE_str "False"
#endif
#ifndef TRUE_str
#define TRUE_str "True"
#endif

#define EOT '\004' /* File name queries abort if first char is EOT */

typedef long long int LongestInt;

typedef long long int FileSizeType;

typedef enum { NoRangeCheck, SignedRangeCheck, UnsignedRangeCheck } TRangeCheck;

/* Association list for internal and external file names set with an
   RTS command line option, see init.pas (`int_name' is the file
   name in the program, case is irrelevant). */
typedef struct FileAssociation
{
  struct FileAssociation *Next;
  const char *int_name, *ext_name;
} TFileAssociation;

typedef struct
{
  LongestInt f_block_size, f_blocks_total, f_blocks_free;
  int f_files_total, f_files_free;
} statfsbuf;

typedef struct
{
  int count;
  char **result;
  void *buf;
} globbuffer;

typedef struct
{
  FDR File;
  Boolean WantedReadOrEOF,
          WantedRead,
          WantedEOF,
          WantedWrite,
          WantedException,
          WantedAlways,
          OccurredReadOrEOF,
          OccurredRead,
          OccurredEOF,
          OccurredWrite,
          OccurredException;
} InternalIOSelectType;

typedef struct {
  char *UserName, *RealName, *Password, *HomeDirectory, *Shell;
  int UID, GID;
} TCPasswordEntry;

#define GCC_CONST __attribute__((const))
#define NORETURN __attribute__((noreturn))

/* read.c */
int _p_internal_getc (FDR);
signed long long _p_readi_check (FDR, TRangeCheck, signed long long, signed long long);
signed long long _p_readi (FDR);
long double _p_read_longreal (FDR);
float _p_read_shortreal (FDR);
double _p_read_real (FDR);
char _p_read_char (FDR);
Boolean _p_read_boolean (FDR);
int _p_read_enum (FDR, char **, int);
int _p_read_string (FDR, char *, int);
void _p_readln (FDR);
void _p_read_init (FDR, int);
void _p_readstr_init (FDR, char *, int, int);
void _p_val_init (FDR, char *, int, int);
int _p_get_val_return_value (FDR);

/* write.c */
void _p_write_flush (FDR);
void _p_write_integer (FDR, signed int, int);
void _p_write_longint (FDR, signed long long, int);
void _p_write_cardinal (FDR, unsigned int, int);
void _p_write_longcard (FDR, unsigned long long, int);
void _p_write_real (FDR, long double, int, int);
void _p_write_one_real (FDR, long double, int, int);
void _p_writestr_one_real (char *, int *, int, long double, int, int);
void _p_write_char (FDR, char, int);
void _p_write_boolean (FDR, int, int);
void _p_write_enum (FDR, char **, int, int, int);
void _p_write_string (FDR, char *, int, int);
void _p_writeln (FDR);
void _p_write_init (FDR, int);
void _p_writestr_init (FDR, char *, int, int);
int _p_writestr_getlength (FDR);

/* numtodec.pas */
extern int _p_low_integer;
char *_p_card_to_decimal (unsigned int, char *);
char *_p_longcard_to_decimal (unsigned long long, char *);
char *_p_longreal_to_decimal (long double, int, int, int, int, int, int *);

/* file.c */
void _p_initialize_std_files (void);
void _p_inittfdd (FDR);
void _p_initfdr (FDR, const char *, int, int);
void _p_checkfiletype (FDR, int);
void _p_open (FDR, TOpenMode);
void _p_clearbuffer (FDR);
void _p_flushbuffer (FDR);
void _p_flush (FDR);
void _p_fflush (Boolean);
void _p_done_files (void);
int _p_filehandle (FDR);
void _p_rewrite (FDR, char *, int);
void _p_extend (FDR, char *, int);
void _p_reset (FDR, char *, int);
int _p_ok_READ (FDR);
int _p_ok_EOF (FDR);
int _p_ok_WRITE (FDR);
void _p_internal_write (const char *, size_t, FDR);
void _p_blockwrite (FDR, const char *, unsigned int, unsigned int *);
void _p_read_buffer (FDR);
void _p_blockread (FDR, char *, unsigned int, unsigned int *);
void _p_getbyte (FDR);
void _p_get (FDR);
void _p_got (FDR);
void _p_lazytryget (FDR);
void _p_lazyget (FDR);
void _p_lazyunget (FDR);
Boolean _p_canread (FDR);
int _p_eof (FDR);
int _p_eoln (FDR);
void _p_page (FDR);
void _p_put (FDR);
const char *_p_filename (const FDR);
const char *_p_get_file_name (const FDR);
void _p_erase (FDR);
void _p_rename (FDR, char *);
void _p_chdir (char *);
void _p_mkdir (char *);
void _p_rmdir (char *);
DIR *_p_opendir_c (char *);
char *_p_readdir_c (DIR *);
void _p_closedir (DIR *);
int _p_Mode2SysMode (int);
int _p_SysMode2Mode (int);
void _p_chmod (FDR, int);
char *_p_realpath (char *);
const char *_p_c_executable_path (char *);
void _p_set_file_time (FDR, UnixTimeType);
void _p_statfs (char *, statfsbuf *);
int _p_fnmatch (const char *, const char *);
void _p_globon (globbuffer *gbuf, char *pattern);
void _p_globfree (globbuffer *gbuf);
void _p_close (FDR);
void _p_donefdr (FDR);
int _p_select (InternalIOSelectType *, int, int, MicroSecondTimeType);
Boolean _p_write_error_message (char *, Boolean);

/* bind.c */
void _p_bind (FDR, const GPC_BINDING *);
void _p_clearbinding (GPC_BINDING *);
void _p_binding (const FDR, GPC_BINDING *);
void _p_unbind (FDR);
void _p_assign_tfdd (FDR, TOpenProc, TSelectFunc, TSelectProc, TReadFunc, TWriteFunc, TFlushProc, TCloseProc, TDoneProc, void *);
void _p_set_tfdd (FDR, TOpenProc, TSelectFunc, TSelectProc, TReadFunc, TWriteFunc, TFlushProc, TCloseProc, TDoneProc, void *);
void _p_get_tfdd (FDR, TOpenProc *, TSelectFunc *, TSelectProc *, TReadFunc *, TWriteFunc *, TFlushProc *, TCloseProc *, TDoneProc *, void **);

/* files.pas */
void _p_internal_assign (FDR, const char *, int);

/* randfile.c */
void _p_seekall (FDR, FileSizeType);
void _p_seekwrite (FDR, FileSizeType);
void _p_seekread (FDR, FileSizeType);
void _p_seekupdate (FDR, FileSizeType);
void _p_update (FDR);
void _p_definesize (FDR, FileSizeType);
void _p_truncate (FDR);
int _p_empty (const FDR);
FileSizeType _p_position (FDR);
FileSizeType _p_lastposition (FDR);
FileSizeType _p_getsize (FDR);
FileSizeType _p_seek (FDR, FileSizeType, int);

/* heap.pas */
void _p_dispose (const void *);
void *_p_malloc (size_t);
void _p_free (void *);
void _p_gpc_reallocmem (void **, size_t);

/* init.pas */
extern Boolean _p_eoln_reset_hack, _p_force_direct_files;
extern FDR _p_current_stdin;
extern TFileAssociation *_p_FileAssociation;

/* filename.pas */
extern char _p_dirseparatorvar;
Boolean _p_isdirseparator (char);
char *_p_get_temp_file_name_cstring (void);
char *_p_osdirseparator2slash_cstring (char *);

/* string.pas */
extern const char **_p_argv;
size_t _p_strlen (const char *);
char *_p_strdup (const char *);
int _p_strcasecmp (const char *, const char *);
int _p_strcmp (const char *, const char *);
char *_p_strlcpy (char *, const char *, size_t);
char *_p_strscan (const char *, char);
char *_p_strrscan (const char *, char);
char *_p_strlower (char *);
Boolean _p_str_eq (char *, int, char *, int);
Boolean _p_str_lt (char *, int, char *, int);
Boolean _p_eq (char *, int, char *, int);
Boolean _p_lt (char *, int, char *, int);
int _p_index (char *, int, char *, int);
int _p_trim (char *, int, char *);
int _p_substr (char *, int, int, int, char *, Boolean);

/* move.pas */
void _p_move (const void *, void *, size_t);

/* error.pas */
extern int _p_inoutres, _p_error_fd, _p_debug;
extern char *_p_inoutres_str, *_p_error_file_name;
void _p_SetReturnAddress (void *);
void _p_RestoreReturnAddress (void);
char *_p_errmsg (int);
void _p_error (int) NORETURN;
void _p_error_integer (int, long int) NORETURN;
void _p_error_string (int, const char *) NORETURN;
void _p_io_error (int);
void _p_io_error_string (int, const char *);
void _p_io_error_file (int, FDR);
void _p_warning (const char *);
void _p_warning_integer (const char *, long int);
void _p_warning_string (const char *, const char *);
void _p_start_temp_io_error (void);
int _p_end_temp_io_error (void);
void _p_internal_error (int) NORETURN;
void _p_internal_error_integer (int, long int) NORETURN;

#define SAVE_RETURN_ADDRESS _p_SetReturnAddress (__builtin_return_address (0))
#define RESTORE_RETURN_ADDRESS _p_RestoreReturnAddress ()
#define DO_RETURN_ADDRESS(stmt) \
do \
  { \
    SAVE_RETURN_ADDRESS; \
    stmt; \
    RESTORE_RETURN_ADDRESS; \
  } \
while (0)

#define _p_error(n)                     DO_RETURN_ADDRESS (_p_error (n))
#define _p_error_integer(n, i)          DO_RETURN_ADDRESS (_p_error_integer (n, i))
#define _p_error_string(n, s)           DO_RETURN_ADDRESS (_p_error_string (n, s))
#define _p_internal_error(n)            DO_RETURN_ADDRESS (_p_internal_error (n))
#define _p_internal_error_integer(n, i) DO_RETURN_ADDRESS (_p_internal_error_integer (n, i))

#define IOERROR(err, retval) do { _p_io_error (err); return retval; } while (0)
#define IOERROR_STRING(err, str, retval) do { _p_io_error_string (err, str); return retval; } while (0)
#define IOERROR_FILE(err, file, retval) do { _p_io_error_file (err, file); return retval; } while (0)
