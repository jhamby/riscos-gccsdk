/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/errno,v $
 * $Date: 2000/07/05 15:00:14 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* ANSI Standard 4.1.3: Errors <errno.h> */

#ifndef __ERRNO_H
#define __ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef errno
extern int errno;
#define __errno errno
#endif

extern char *sys_errlist[];
extern int sys_nerr;

/* Operation not permitted.  */
#define EPERM           1
/* No such file or directory.  */
#define ENOENT		2
/* No such process.  */
#define ESRCH		3
/* Interrupted function call. */
#define EINTR		4
/* Input/output error.  */
#define EIO		5
/* No such device or address (device not configured).  */
#define ENXIO		6
/* Argument list too long.  */
#define E2BIG		7
/* Invalid executable file format (exec format error). */
#define ENOEXEC 	8
/* Bad file descriptor.  */
#define EBADF		9
/* There are no child processes.  */
#define ECHILD		10
/* Resource deadlock avoided.  */
#define EDEADLK	     	11
/* Cannot allocate memory.  */
#define ENOMEM		12
/* Permission denied.
   File permissions do not allow the attempted operation.  */
#define EACCES		13
/* Bad address. An invalid pointer was detected.  */
#define EFAULT		14
/* Block device required.  */
#define ENOTBLK 	15
/* Device busy. Caused by a system resource that can't be shared
   is already in use.  */
#define EBUSY		16
/* File exists.  */
#define EEXIST		17
/* Invalid cross-device link.  */
#define EXDEV		18
/* Operation not supported by device.  */
#define ENODEV		19
/* Not a directory.  */
#define ENOTDIR 	20
/* Is a directory.  */
#define EISDIR		21
/* Invalid argument.  */
#define EINVAL		22
/* Too many open files in system.  */
#define ENFILE		23
/* Too many open files.  */
#define EMFILE		24
/* Inappropriate ioctl for device.  */
#define ENOTTY		25
/* Text file busy.  */
#define ETXTBSY 	26
/* File too large.  */
#define EFBIG		27
/* No space left on device.  */
#define ENOSPC		28
/* Illegal seek.  */
#define ESPIPE		29
/* Read-only filing system.  */
#define EROFS		30
/* Too many links.  */
#define EMLINK		31
/* Broken pipe.  */
#define EPIPE		32
/* Domain error.  */
#define EDOM		33
/* Range error.  */
#define ERANGE		34
/* Operation would block.  */
#define EWOULDBLOCK	35
/* Operation now in progress.  */
#define EINPROGRESS	36
/* Operation already in progress.  */
#define EALREADY	37
/* Socket operation on non-socket.  */
#define ENOTSOCK	38
/* Destination address required.  */
#define EDESTADDRREQ	39
/* Message too long.  */
#define EMSGSIZE	40
/* Protocol wrong type for socket.  */
#define EPROTOTYPE	41
/* Option not supported by protocol.  */
#define ENOPROTOOPT	42
/* Protocol not supported */
#define EPROTONOSUPPORT	43
/* Socket type not supported */
#define ESOCKTNOSUPPORT	44
/* Operation not supported */
#define EOPNOTSUPP	45
/* Protocol family not supported.  */
#define EPFNOSUPPORT	46
/* Address family not supported by protocol family. */
#define EAFNOSUPPORT	47
/* Address already in use */
#define EADDRINUSE	48
/* Can't assign requested address.  */
#define EADDRNOTAVAIL	49
/* Network is down.  */
#define ENETDOWN	50
/* Network unreachable */
#define ENETUNREACH	51
/* Network dropped connection on reset.  */
#define ENETRESET  	52
/* Software caused connection abort.  */
#define ECONNABORTED	53
/* Connection reset by peer */
#define ECONNRESET	54
/* No buffer space available */
#define ENOBUFS		55
/* Socket is already connected */
#define EISCONN		56
/* Socked is not connected */
#define ENOTCONN	57
/* Can't send after socket shutdown.  */
#define ESHUTDOWN	58
/* Too many references: can't splice.  */
#define ETOOMANYREFS	59
/* Connection timed out.  */
#define ETIMEDOUT	60
/* Connection refused */
#define ECONNREFUSED	61
/* Too many levels of symbolic links.  */
#define ELOOP		62
/* File name too long.  */
#define ENAMETOOLONG	63
/* Host is down.  */
#define EHOSTDOWN	64
/* Host unreachable.  */
#define EHOSTUNREACH	65
/* Directory not empty.  */
#define ENOTEMPTY	66
/* Too many processes.  */
#define EPROCLIM	67
/* Too many users.  */
#define EUSERS		68
/* Disc quota exceeded.  */
#define EDQUOT		69
/* Stale NFS file handle.  */
#define ESTALE		70
/* Too many levels of remote in path.  */
#define EREMOTE		71
/* RPC struct is bad.  */
#define EBADRPC		72
/* RPC version wrong.  */
#define ERPCMISMATCH	73
/* RPC program not available.  */
#define EPROGUNAVAIL	74
/* RPC program version wrong.  */
#define EPROGMISMATCH	75
/* RPC bad procedure for program.  */
#define EPROCUNAVAIL	76
/* No locks available.  */
#define ENOLCK		77
/* Function not implemented.  */
#define ENOSYS		78
/* Inappropriate file type or format.  */
#define EFTYPE		79
/* Authentication error.  */
#define EAUTH		80
/* Need authenticator.  */
#define ENEEDAUTH	81
/* Resource temporarily unavailable.  */
#define EAGAIN		82
/* Inappropriate operation for background process.  */
#define EBACKGROUND	100
/* Translator died.  */
#define EDIED		101
/* A pretty bad error.  */
#define EGREGIOUS	103
/* Gratuitous error.  */
#define EGRATUITOUS	105

/* EOPSYS was originally called EOS but this caused problems.  */
#define EOPSYS		106
#define ESIG		107

#define __SYS_NERR	107

/* This macro should be used whenever errno is to be set. This permits
   us to easily make setting the errno call a function if necessary if
   threads ever appear.  We also give a return value so we can use
   return __set_errno () which can allow function tail calling.  */
#ifndef __set_errno
#define __set_errno(val) (errno = (val), -1)
#endif

#ifdef __cplusplus
	}
#endif

#endif
