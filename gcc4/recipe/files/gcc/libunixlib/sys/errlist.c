/* strerror (), strerror_r ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <string.h>
#include <errno.h>
#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif

int sys_nerr = __SYS_NERR + 1;

const char *sys_errlist[__SYS_NERR + 1] =
{
  "Error 0",					/* (0) */
  "Numerical argument out of domain / Operation not permitted", /* EDOM / EPERM */
  "Result too large / No such file or directory", /* ERANGE / ENOENT */
  "Unrecognised signal / No such process",	/* ESIGNUM / ESRCH */
  "Interrupted system call",			/* EINTR */
  "Input/output error",				/* EIO */
  "No such device or address",			/* ENXIO */
  "Argument list too long",			/* E2BIG */
  "Exec format error",				/* ENOEXEC */
  "Bad file descriptor",			/* EBADF */
  "No child processes",				/* (10) ECHILD */
  "Resource temporarily unavailable",		/* EAGAIN */
  "Cannot allocate memory",			/* ENOMEM */
  "Permission denied",				/* EACCES */
  "Bad address",				/* EFAULT */
  "Block device required",			/* ENOTBLK */
  "Device busy",				/* EBUSY */
  "File exists",				/* EEXIST */
  "Invalid cross-device link",			/* EXDEX */
  "Operation not supported by device",		/* ENODEV */
  "Not a directory",				/* (20) ENOTDIR */
  "Is a directory",				/* EISDIR */
  "Invalid argument",				/* EINVAL */
  "Too many open files in system",		/* ENFILE */
  "Too many open files",			/* EMFILE */
  "Inappropriate ioctl for device",		/* ENOTTY */
  "Text file busy",				/* ETXTBSY */
  "File too large",				/* EFBIG */
  "No space left on device",			/* ENOSPC */
  "Illegal seek",				/* ESPIPE */
  "Read-only file system",			/* (30) EROFS */
  "Too many links",				/* EMLINK */
  "Broken pipe",				/* EPIPE */
  "Argument out of domain",			/* EDOM */
  "Range error",				/* ERANGE */
  "Operation would block",			/* EWOULDBLOCK */
  "Operation now in progress",			/* EINPROGRESS */
  "Operation already in progress",		/* EALREADY */
  "Socket operation on non-socket",		/* ENOTSOCK */
  "Destination address required",		/* EDESTADDRREQ */
  "Message too long",				/* (40) EMSGSIZE */
  "Protocol wrong type for socket",		/* EPROTOTYPE */
  "Option not supported by protocol",		/* ENOPROTOOPT */
  "Protocol not supported",			/* EPROTONOSUPPORT */
  "Socket type not supported",			/* ESOCKTNOSUPPORT */
  "Operation not supported",			/* EOPNOTSUPP */
  "Protocol family not supported",		/* EPFNOSUPPORT */
  "Address family not supported by protocol family",	/* EAFNOSUPPORT */
  "Address already in use",			/* EADDRINUSE */
  "Can't assign requested address",		/* EADDRNOTAVAIL */
  "Network is down",				/* (50) ENETDOWN */
  "Network unreachable",			/* ENETUNREACH */
  "Network dropped connection on reset",	/* ENETRESET */
  "Software caused connection abort",		/* ECONNABORTED */
  "Connection reset by peer",			/* ECONNRESET */
  "No buffer space available",			/* ENOBUFS */
  "Socket is already connected",		/* EISCONN */
  "Socket is not connected",			/* ENOTCONN */
  "Can't send after socket shutdown",		/* ESHUTDOWN */
  "Too many references: can't splice",		/* ETOOMANYREFS */
  "Connection timed out",			/* (60) ETIMEDOUT */
  "Connection refused",				/* ECONNREFUSED */
  "Too many levels of symbolic links",		/* ELOOP */
  "File name too long",				/* ENAMETOOLONG */
  "Host is down",				/* EHOSTDOWN */
  "Host unreachable",				/* EHOSTUNREACH */
  "Directory not empty",			/* ENOTEMPTY */
  "Too many processes",				/* EPROCLIM */
  "Too many users",				/* EUSERS */
  "Disc quota exceeded",			/* EDQUOT */
  "Stale NFS file handle",			/* (70) ESTALE */
  "Too many levels of remote in path",		/* EREMOTE */

#ifdef __TARGET_SCL__
  "RPC struct is bad",				/* EBADRPC */
  "RPC version wrong",				/* ERPCMISMATCH */
  "RPC prog. not avail",			/* EPROGUNAVAIL */
  "Program version wrong",			/* EPROGMISMATCH */
  "Bad procedure for program",			/* EPROCUNAVAIL */
  "No locks available",				/* ENOLCK */
  "Function not implemented",			/* ENOSYS */
  "Inappropriate file type or format",		/* EFTYPE */
  "Authentication error",			/* (80) EAUTH */
  "Need authenticator",				/* ENEEDAUTH */
  "RISC OS error",				/* EOPSYS */
  "Invalid multibyte sequence"			/* EILSEQ */
#else
  "Not a stream device",			/* ENOSTR */
  "Timer expired",				/* ETIME */
  "Out of stream resources",			/* ENOSR */
  "No message of desired type",			/* ENOMSG */
  "Not a data message",				/* EBADMSG */
  "Identifier removed",				/* EIDRM */
  "Deadlock situation detected/avoided",	/* EDEADLK */
  "No record locks available",			/* ENOLCK */
  "No suitable message on queue",		/* (80) */
  "Identified removed from system",		/* (81) */
  "Wrong version of shared library",		/* ELIBVER */
  "Permission denied (shared library)",		/* ELIBACC */
  "Shared libraries nested too deeply",		/* ELIBLIM */
  "Shared library file not found",		/* ELIBNOENT */
  "Shared library exec format error",		/* ELIBNOEXEC */
  "Function not implemented",			/* ENOSYS */
  "RISC OS error",				/* EOPSYS */
  "Signal Received",				/* ESIG, as flagged by SIG_ERR */
  "Invalid multibyte sequence"			/* (90) EILSEQ */
  "Value too large for defined data type"       /* EOVERFLOW */
#endif
};

char *
strerror (int errnum)
{
  if (errnum < 0 || errnum >= sys_nerr)
    {
      (void) __set_errno (EINVAL);
      return (char *) "Unknown Error";
    }
#ifndef __TARGET_SCL__
  if (errnum == EOPSYS)
    return __pthread_running_thread->errbuf.errmess;
#endif

  return (char *) sys_errlist[errnum];
}

int
strerror_r (int errnum, char *strerrbuf, size_t buflen)
{
  if (errnum < 0 || errnum >= sys_nerr)
    return EINVAL;

  const char *errmess = strerror (errnum);

  size_t len = strlen (errmess);
  if (len >= buflen)
    return ERANGE;

  memcpy (strerrbuf, errmess, len + 1);

  return 0;
}
