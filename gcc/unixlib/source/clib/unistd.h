/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unistd.h,v $
 * $Date: 2000/07/15 14:52:12 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX Standard 2.10: Symbolic Constants <unistd.h>.  */

#ifndef __UNISTD_H
#define __UNISTD_H

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __GNUC__
#undef  __attribute__
#define __attribute__(x) /* Ignore */
#endif

/* These may be used to determine what facilities are present at compile time.
   Their values can be obtained at run time from sysconf.  */

/* POSIX Standard approved as IEEE Std 1003.1 as of August, 1988.  */
#define	_POSIX_VERSION	199009L

#define	_POSIX2_C_VERSION	199912L	/* Invalid until 1003.2 is done.  */

/* If defined, the implementation supports the
   C Language Bindings Option.  */
#define	_POSIX2_C_BIND	1

/* If defined, the implementation supports the
   C Language Development Utilities Option.  */
#define	_POSIX2_C_DEV	1

/* If defined, the implementation supports the
   Software Development Utilities Option.  */
#define	_POSIX2_SW_DEV	1

/* File name components longer than NAME_MAX will generate an
   ENAMETOOLONG error.  */
#define _POSIX_NO_TRUNC 1

/* For terminal devices.  This allows special control characters
   to be disabled individually.  */
#define _POSIX_VDISABLE ((unsigned char) - 1)

/* POSIX.4 clocks and timers are supported.  */
#define _POSIX_TIMERS 1

/* Real-time queued signals are supported.  */
#define _POSIX_REALTIME_SIGNALS 1

/* Protocol-independent interfaces are supported.  */
#define _POSIX_PII 1

/* Socket protocol-independent interfaces are supported.  */
#define _POSIX_PII_SOCKET 1

/* Internet family of protocols is supported.  */
#define _POSIX_PII_INTERNET 1

/* Connection-mode Internet protocol is supported.  */
#define _POSIX_PII_INTERNET_STREAM 1

/* Connectionless Internet protocol is supported.  */
#define _POSIX_PII_INTERNET_DGRAM 1

/* Library supports the poll function.  */
#define _POSIX_POLL 1

/* Library supports select and pselect.  */
#define _POSIX_SELECT 1

/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */

#ifndef __STDDEF_H
#include <stddef.h>
#endif

/* Values for the second argument to access.  */

/* Test for existence. */
#ifndef F_OK
#define F_OK	0
#endif
/* Test for execute permission.  */
#ifndef X_OK
#define X_OK	1
#endif
/* Test for write permission.  */
#ifndef W_OK
#define W_OK	2
#endif
/* Test for read permission.  */
#ifndef R_OK
#define R_OK	4
#endif

/* Test for access to name.  */
extern int access (const char *__name, int __type);

/* Values for the WHENCE argument to lseek.  */
#ifndef	__STDIO_H		/* <stdio.h> has the same definitions.  */
#define	SEEK_SET	0	/* Seek from beginning of file.  */
#define	SEEK_CUR	1	/* Seek from current position.  */
#define	SEEK_END	2	/* Seek from end of file.  */
#endif

/* Move fd's file position to offset bytes from the:
   beginning of the file (if SEEK_SET),
   the current position (if SEEK_CUR),
   or the end of the file (SEEK_END).
   Return the new file position.  */
extern __off_t lseek (int __fd, __off_t __offset, int __whence);

/* Close the file descriptor fd.  */
extern int close (int __fd);

/* Read nbytes into buf from fd.  */
extern __ssize_t read (int __fd, void *__buf, size_t __nbytes);

/* Write n bytes of buf to fd.  */
extern __ssize_t write (int __fd, const void *__buf, size_t __nbytes);

/* Create a one way communication channel (pipe). */
extern int pipe (int __pipedes[2]);

/* Schedule an alarm.  */
extern unsigned int alarm (unsigned int __seconds);

/* Make the process sleep for 'seconds' seconds.  */
extern unsigned int sleep (unsigned int __seconds);

/* Suspend the process until a signal arrives.  */
extern int pause (void);

/* Change the owner and group of file.  */
extern int chown (const char *__file, __uid_t __owner, __gid_t __group);

/* Change the owner and group of the file associated with 'fd'  */
extern int fchown (int __fd, __uid_t __owner, __gid_t __group);

/* Change the process's working directory to path.  */
extern int chdir (const char *__path);

/* Get the pathname of the current working directory.  */
extern char *getcwd (char *__buf, size_t __size);

/* The BSD version of getcwd. This function calls getcwd but does
   not provide any buffer size checking.  */
extern char *getwd (char *__buf);

/* Duplicate FD, returning a new file descriptor on the same file.  */
extern int dup (int __fd);

/* Duplicate fd to fd2, closing fd2 and making it open on the same file.  */
extern int dup2 (int __fd, int __fd2);

/* NULL-terminated array of "NAME=VALUE" environment variables.  */
extern char **environ;

/* Replace the current process, executing path with args argv and
   environment envp.  */
extern int execve (const char *__path, char **__argv, char **__envp);

/* Execute PATH with arguments ARGV and environment from `environ'.  */
extern int execv (const char *__path, char **__argv);
/* Execute PATH with all arguments after PATH until a NULL pointer,
   and the argument after that for environment.  */
extern int execle (const char *__path, const char *__arg, ...);
/* Execute PATH with all arguments after PATH until
   a NULL pointer and environment from `environ'.  */
extern int execl (const char *__path, const char *__arg, ...);
/* Execute FILE, searching in the `PATH' environment variable if it contains
   no slashes, with arguments ARGV and environment from `environ'.  */
extern int execvp (const char *__file, char **__argv);
/* Execute FILE, searching in the `PATH' environment variable if
   it contains no slashes, with all arguments after FILE until a
   NULL pointer and environment from `environ'.  */
extern int execlp (const char *__file, const char *__arg, ...);

/* Terminate program execution with the low-order 8 bits of status.  */
extern void _exit (int __status) __attribute__ ((__noreturn__));

/* Values for the NAME argument to `pathconf' and `fpathconf'.  */
enum
  {
    _PC_LINK_MAX,
    _PC_MAX_CANON,
    _PC_MAX_INPUT,
    _PC_NAME_MAX,
    _PC_PATH_MAX,
    _PC_PIPE_BUF,
    _PC_CHOWN_RESTRICTED,
    _PC_NO_TRUNC,
    _PC_VDISABLE
  };

/* Get file-specific configuration information about PATH.  */
extern long int pathconf (const char *__path, int __name);

/* Get file-specific configuration about descriptor FD.  */
extern long int fpathconf (int __fd, int __name);

/* Values for the argument to `sysconf'.  */
enum
  {
    _SC_ARG_MAX,
    _SC_CHILD_MAX,
    _SC_CLK_TCK,
    _SC_NGROUPS_MAX,
    _SC_OPEN_MAX,
    _SC_STREAM_MAX,
    _SC_TZNAME_MAX,
    _SC_JOB_CONTROL,
    _SC_SAVED_IDS,
    _SC_VERSION,
    _SC_PAGESIZE,

    /* Values for the argument to `sysconf' corresponding to
       _POSIX2_* symbols.  */
    _SC_BC_BASE_MAX,
    _SC_BC_DIM_MAX,
    _SC_BC_SCALE_MAX,
    _SC_BC_STRING_MAX,
    _SC_COLL_WEIGHTS_MAX,
    _SC_EQUIV_CLASS_MAX,
    _SC_EXPR_NEST_MAX,
    _SC_LINE_MAX,
    _SC_RE_DUP_MAX,

    _SC_2_VERSION,
    _SC_2_C_BIND,
    _SC_2_C_DEV,
    _SC_2_FORT_DEV,
    _SC_2_FORT_RUN,
    _SC_2_SW_DEV,
    _SC_2_LOCALEDEF
  };

/* Get the value of the system variable NAME.  */
extern long int sysconf (int __name);


/* Get the process id of the calling process.  */
extern __pid_t getpid (void);
/* Get the process id of the calling process's parent.  */
extern __pid_t getppid (void);
/* Get the process group id of the calling process.  */
extern __pid_t getpgrp (void);
/* Set the process group id of the process matching pid to pgid.  */
extern int setpgrp (__pid_t __pid, __pid_t __pgid);
extern int setpgid (__pid_t __pid, __pid_t __pgid);

#if 0
/* Create a new session with the calling process as its leader.  */
extern __pid_t setsid (void);
#endif

/* Get the real user id of the calling process.  */
extern __uid_t getuid (void);
/* Get the effective user id of the calling process.  */
extern __uid_t geteuid (void);
/* Get the real group id of the calling process.  */
extern __gid_t getgid (void);
/* Get the effective group id of the calling process.  */
extern __gid_t getegid (void);

#if 0
/* Return the number of supplementary groups the calling process is in.  */
extern int getgroups (int __size, __gid_t __list[]);
#endif

/* Set the user id of the calling process to uid.  */
extern int setuid (__uid_t __uid);
/* Set the effective user id of the calling process to uid (from BSD).  */
extern int seteuid (__uid_t __uid);
#if 0
/* Combination of setuid and seteuid (from BSD).  */
extern int setreuid (__uid_t __ruid, __uid_t __euid);
#endif

/* Set the group id of the calling process to uid.  */
extern int setgid (__gid_t __gid);
/* Set the effective group id of the calling process to uid (from BSD).  */
extern int setegid (__gid_t __gid);
#if 0
/* Combination of setgid and setegid (from BSD).  */
extern int setregid (__gid_t __rgid, __gid_t __egid);
#endif

/* Clone the calling process, creating an exact copy.  */
#define fork() vfork()	/* WARNING */
/* Clone the calling process, but without copying the whole address
   space (from BSD).  */
extern __pid_t vfork (void);

/* Return the pathname of the terminal fd is open on.  */
extern char *ttyname (int __fd);

/* Return 1 if fd is a valid descriptor associated with a terminal.  */
extern int isatty (int __fd);

/* Return 1 if fd is a valid descriptor associated with a pipe.  */
extern int ispipe (int __fd);

/* Make a link to from named to.  */
extern int link (const char *__from, const char *__to);

/* Remove the line name.  */
extern int unlink (const char *__name);

/* Remove the directory path.  */
extern int rmdir (const char *__path);

#define L_cuserid 16

/* Return the login name of the user.  */
extern char *getlogin (void);

#if 0
/* Set the foreground process group ID of FD set PGRP_ID.  */
extern int tcsetpgrp (int __fd, __pid_t __pgrp_id);


/* Set the login name returned by `getlogin'.  */
extern int setlogin (const char *__name);

/* Invoke system call number 'sysno', passing it the remaining args.  */
extern int syscall (int __sysno, ...);
#endif

#ifndef F_LOCK
/* `lockf' is a simpler interface to the locking facilities of `fcntl'.
   LEN is always relative to the current file position.
   The CMD argument is one of the following.  */

#define F_ULOCK 0	/* Unlock a previously locked region.  */
#define F_LOCK  1	/* Lock a region for exclusive use.  */
#define F_TLOCK 2	/* Test and lock a region for exclusive use.  */
#define F_TEST  3	/* Test a region for other processes locks.  */

extern int lockf (int __fd, int __cmd, __off_t __len);
#endif /* Use misc and F_LOCK not already defined.  */

/* BSD extensions.  */

extern int brk (void *__addr);
extern void *sbrk (int __incr);

/* Put the name of the current host in no more than len bytes of name.  */
extern int gethostname (char *__name, size_t __len);

/* Put the domainname of the current host in no more than len bytes of name.  */
extern int getdomainname (char *__name, size_t __len);

/* Make all changes done to all files actually appear on disk.  */
extern int sync (void);

/* Make all changes done to FD actually appear on disk.  */
extern int fsync (int __fd);

/* Truncate FILE to LENGTH bytes.  */
extern int truncate (const char *__file, __off_t __length);

/* Truncate the file FD is open on to LENGTH bytes.  */
extern int ftruncate (int __fd, __off_t __length);

/* Set the name of the current host to name, which is len bytes long.  */
extern int sethostname (const char *__name, size_t __len);

/* Set the domainname of the current host to name, which is len bytes long.  */
extern int setdomainname (const char *__name, size_t __len);

/* Return the number of bytes in a page.  This is the system's page size,
   which is not necessarily the same as the hardware page size.  */
extern size_t getpagesize (void);

/* Return the maximum number of file descriptors
   the current process could possibly have.  */
extern int getdtablesize (void);

#if 0

/* Return the current machine's Internet number.  */
extern long int gethostid (void);

/* Set the current machine's Internet number to ID.
   This call is restricted to the super-user.  */
extern int sethostid (long int __id);


/* Revoke access permissions to all processes currently communicating
   with the control terminal, and then send a SIGHUP signal to the process
   group of the control terminal.  */
extern int vhangup (void);


/* Turn accounting on if NAME is an existing file.  The system will then write
   a record for each process as it terminates, to this file.  If NAME is NULL,
   turn accounting off.  This call is restricted to the super-user.  */
extern int acct (const char *__name);

/* Make PATH be the root directory (the starting point for absolute paths).
   This call is restricted to the super-user.  */
extern int chroot (const char *__path);

/* Make the block special device PATH available to the system for swapping.
   This call is restricted to the super-user.  */
extern int swapon (const char *__path);

/* Reboot or halt the system.  */
extern int reboot (int __howto);


/* Successive calls return the shells listed in `/etc/shells'.  */
extern char *getusershell (void);
extern void endusershell (void); /* Discard cached info.  */
extern void setusershell (void); /* Rewind and re-read the file.  */


/* Prompt with PROMPT and read a string from the terminal without echoing.
   Uses /dev/tty if possible; otherwise stderr and stdin.  */
extern char *getpass (const char *__prompt);
#endif

/* POSIX 2 extensions.  */


/* Values for the NAME argument to `confstr'.  */
enum
  {
    _CS_PATH			/* The default search path.  */
  };

/* Get the value of the string-valued system variable NAME.  */
extern size_t confstr (int __name, char *__buf, size_t __len);

/* Process the arguments in ARGV (ARGC of them, minus
   the program name) for options given in OPTS.

   If `opterr' is zero, no messages are generated
   for invalid options; it defaults to 1.
   `optind' is the current index into ARGV.
   `optarg' is the argument corresponding to the current option.
   Return the option character from OPTS just read.
   Return -1 when there are no more options.
   For unrecognized options, or options missing arguments,
   `optopt' is set to the option letter, and '?' is returned.

   The OPTS string is a list of characters which are recognized option
   letters, optionally followed by colons, specifying that that letter
   takes an argument, to be placed in `optarg'.

   The argument `--' causes premature termination of argument scanning,
   explicitly telling `getopt' that there are no more options. */
extern int getopt (int __argc, char *const *__argv, const char *__opts);
extern int opterr, optind, optopt, optreset;
extern char *optarg;

#ifdef __cplusplus
	}
#endif

#endif
