/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/errlist.c,v $
 * $Date: 2000/07/15 14:52:33 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: errlist.c,v 1.1.1.1 2000/07/15 14:52:33 nick Exp $";
#endif

#include <string.h>
#include <errno.h>

int sys_nerr = __SYS_NERR + 1;

char *sys_errlist[__SYS_NERR + 1] =
{
  "Error 0",
  "Operation not permitted",	/* EPERM */
  "No such file or directory",	/* ENOENT */
  "No such process",		/* ESRCH */
  "Interrupted system call",	/* EINTR */
  "Input/output error",		/* EIO */
  "No such device or address",	/* ENXIO */
  "Argument list too long",	/* E2BIG */
  "Exec format error",		/* ENOEXEC */
  "Bad file descriptor",	/* EBADF */
  "No child processes",		/* ECHILD */
  "Resource deadlock avoided",	/* EDEADLK */
  "Cannot allocate memory",	/* ENOMEM */
  "Permission denied",		/* EACCES */
  "Bad address",		/* EFAULT */
  "Block device required",	/* ENOTBLK */
  "Device busy",		/* EBUSY */
  "File exists",		/* EEXIST */
  "Invalid cross-device link",	/* EXDEX */
  "Operation not supported by device",	/* ENODEV */
  "Not a directory",		/* ENOTDIR */
  "Is a directory",		/* EISDIR */
  "Invalid argument",		/* EINVAL */
  "Too many open files in system",	/* ENFILE */
  "Too many open files",	/* EMFILE */
  "Inappropriate ioctl for device",	/* ENOTTY */
  "Text file busy",		/* ETXTBSY */
  "File too large",		/* EFBIG */
  "No space left on device",	/* ENOSPC */
  "Illegal seek",		/* ESPIPE */
  "Read-only file system",	/* EROFS */
  "Too many links",		/* EMLINK */
  "Broken pipe",		/* EPIPE */
  "Argument out of domain",	/* EDOM */
  "Range error",		/* ERANGE */
  "Operation would block",	/* EWOULDBLOCK */
  "Operation now in progress",	/* EINPROGRESS */
  "Operation already in progress",	/* EALREADY */
  "Socket operation on non-socket",	/* ENOTSOCK */
  "Destination address required",	/* EDESTADDRREQ */
  "Message too long",		/* EMSGSIZE */
  "Protocol wrong type for socket",	/* EPROTOTYPE */
  "Option not supported by protocol",	/* ENOPROTOOPT */
  "Protocol not supported",	/* EPROTONOSUPPORT */
  "Socket type not supported",	/* ESOCKTNOSUPPORT */
  "Operation not supported",	/* EOPNOTSUPP */
  "Protocol family not supported",	/* EPFNOSUPPORT */
  "Address family not supported by protocol family",	/* EAFNOSUPPORT */
  "Address already in use",	/* EADDRINUSE */
  "Can't assign requested address",	/* EADDRNOTAVAIL */
  "Network is down",		/* ENETDOWN */
  "Network unreachable",	/* ENETUNREACH */
  "Network dropped connection on reset",	/* ENETRESET */
  "Software caused connection abort",	/* ECONNABORTED */
  "Connection reset by peer",	/* ECONNRESET */
  "No buffer space available",	/* ENOBUFS */
  "Socket is already connected",	/* EISCONN */
  "Socket is not connected",	/* ENOTCONN */
  "Can't send after socket shutdown",	/* ESHUTDOWN */
  "Too many references: can't splice",	/* ETOOMANYREFS */
  "Connection timed out",	/* ETIMEDOUT */
  "Connection refused",		/* ECONNREFUSED */
  "Too many levels of symbolic links",	/* ELOOP */
  "File name too long",		/* ENAMETOOLONG */
  "Host is down",		/* EHOSTDOWN */
  "Host unreachable",		/* EHOSTUNREACH */
  "Directory not empty",	/* ENOTEMPTY */
  "Too many processes",		/* EPROCLIM */
  "Too many users",		/* EUSERS */
  "Disc quota exceeded",	/* EDQUOT */
  "Stale NFS file handle",	/* ESTALE */
  "Too many levels of remote in path",	/* EREMOTE */
  "RPC struct is bad",		/* EBADRPC */
  "RPC version wrong",		/* ERPCMISMATCH */
  "RPC program not available",	/* EPROGUNAVAIL */
  "RPC program version wrong",	/* EPROGMISMATCH */
  "RPC bad procedure for program",	/* EPROCUNAVAIL */
  "No locks available",		/* ENOLCK */
  "Function not implemented",	/* ENOSYS */
  "Inappropriate file type or format",	/* EFTYPE */
  "Authentication error",	/* EAUTH */
  "Need authenticator",		/* ENEEDAUTH */
  "Resource temporarily unavailable",	/* EAGAIN */
  "",				/* 83 */
  "",				/* 84 */
  "",				/* 85 */
  "",				/* 86 */
  "",				/* 87 */
  "",				/* 88 */
  "",				/* 89 */
  "",				/* 90 */
  "",				/* 91 */
  "",				/* 92 */
  "",				/* 93 */
  "",				/* 94 */
  "",				/* 95 */
  "",				/* 96 */
  "",				/* 97 */
  "",				/* 98 */
  "",				/* 99 */
  "Inappropriate operation for background process",	/* EBACKGROUND */
  "Translator died",		/* EDIED */
  "",				/* 102 */
  "A pretty bad error",		/* EGREGIOUS */
  "",				/* 104 */
  "Gratuitous error",		/* EGRATUITOUS */
  "RISC OS error",		/* EOPSYS */
  "Signal Received"		/* as flagged by SIG_ERR */
};

char *
strerror (register int e)
{
  if (e < 0 || e >= sys_nerr)
    return (0);

  return (sys_errlist[e]);
}
