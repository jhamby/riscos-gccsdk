/* Internal UnixLib sys/socket.h
 * Copyright (c) 2008-2010 UnixLib Developers
 */

#ifndef __SYS_SOCKET_H
#include_next <sys/socket.h>
#endif

#if !defined(__INTERNAL_SYS_SOCKET_H) && defined(__SYS_SOCKET_H)
#define	__INTERNAL_SYS_SOCKET_H

#include <swis.h>

__BEGIN_DECLS

extern int __net_error (int rtrn, const _kernel_oserror *err);

static inline int
_accept (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addrlen)
{
  register int fd __asm ("r0") = __fd;
  register __SOCKADDR_ARG addr __asm ("r1") = __addr;
  register socklen_t *addrlen __asm ("r2") = __addrlen;
  register int rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Accept_1]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (fd), "r" (addr), "r" (addrlen),
                      [SWI_XSocket_Accept_1] "i" (Socket_Accept_1 | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline int
_bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __addrlen)
{
  register int fd __asm ("r0") = __fd;
  register __CONST_SOCKADDR_ARG addr __asm ("r1") = __addr;
  register socklen_t addrlen __asm ("r2") = __addrlen;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Bind]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
		    : "r" (fd), "r" (addr), "r" (addrlen),
                      [SWI_XSocket_Bind] "i" (Socket_Bind | (1<<17))
		    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __addrlen)
{
  register int fd __asm ("r0") = __fd;
  register __CONST_SOCKADDR_ARG addr __asm ("r1") = __addr;
  register socklen_t addrlen __asm ("r2") = __addrlen;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Connect]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
		    : "r" (fd), "r" (addr), "r" (addrlen),
                      [SWI_XSocket_Connect] "i" (Socket_Connect | (1<<17))
		    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_getpeername (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addrlen)
{
  register int fd __asm ("r0") = __fd;
  register __SOCKADDR_ARG addr __asm ("r1") = __addr;
  register socklen_t *addrlen __asm ("r2") = __addrlen;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Getpeername_1]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (fd), "r" (addr), "r" (addrlen),
                      [SWI_XSocket_Getpeername_1] "i" (Socket_Getpeername_1 | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_getsockname (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addrlen)
{
  register int fd __asm ("r0") = __fd;
  register __SOCKADDR_ARG addr __asm ("r1") = __addr;
  register socklen_t *addrlen __asm ("r2") = __addrlen;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Getsockname_1]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (fd), "r" (addr), "r" (addrlen),
                      [SWI_XSocket_Getsockname_1] "i" (Socket_Getsockname_1 | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_getsockopt (int __fd, int __level, int __optname, void *__restrict __optval,
	     socklen_t *__restrict __optlen)
{
  register int fd __asm ("r0") = __fd;
  register int level __asm ("r1") = __level;
  register int optname __asm ("r2") = __optname;
  register void *optval __asm ("r3") =  __optval;
  register socklen_t *optlen __asm ("r4") = __optlen;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Getsockopt]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (fd), "r" (level), "r" (optname), "r" (optval), "r" (optlen),
                      [SWI_XSocket_Getsockopt] "i" (Socket_Getsockopt | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_listen (int __fd, int __backlog)
{
  register int fd __asm ("r0") = __fd;
  register int backlog __asm ("r1") = __backlog;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Listen]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (fd), "r" (backlog),
                      [SWI_XSocket_Listen] "i" (Socket_Listen | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline ssize_t
_recv (int __fd, void *__buf, size_t __len, int __flags)
{
  register int fd __asm ("r0") = __fd;
  register void *buf __asm ("r1") = __buf;
  register size_t len __asm ("r2") = __len;
  register int flags __asm ("r3") =  __flags;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Recv]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (fd), "r" (buf), "r" (len), "r" (flags),
                      [SWI_XSocket_Recv] "i" (Socket_Recv | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_recvfrom (int __fd, void *__restrict __buf, size_t __len, int __flags,
	   __SOCKADDR_ARG __srcaddr, socklen_t *__restrict __srcaddrlen)
{
  register int fd __asm ("r0") = __fd;
  register void *buf __asm ("r1") = __buf;
  register size_t len __asm ("r2") = __len;
  register int flags __asm ("r3") = __flags;
  register __SOCKADDR_ARG srcaddr __asm ("r4") = __srcaddr;
  register socklen_t *srcaddrlen __asm ("r5") = __srcaddrlen;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Recvfrom_1]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (fd), "r" (buf), "r" (len), "r" (flags),
		      "r" (srcaddr), "r" (srcaddrlen),
                      [SWI_XSocket_Recvfrom_1] "i" (Socket_Recvfrom_1 | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_recvmsg (int __fd, struct msghdr *__msg, int __flags)
{
  register int fd __asm ("r0") = __fd;
  register struct msghdr *msg __asm ("r1") = __msg;
  register int flags __asm ("r2") = __flags;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Recvmsg_1]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (fd), "r" (msg), "r" (flags),
                      [SWI_XSocket_Recvmsg_1] "i" (Socket_Recvmsg_1 | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline int
_sclose (int __sd)
{
  register int sd __asm ("r0") = __sd;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Close]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (sd),
                      [SWI_XSocket_Close] "i" (Socket_Close | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

/* Only to be called when we're not interested in a possible error
   happening, i.e. to be called in any error handling code.  */
static inline void
_sclose_no_error (int __sd)
{
  register int sd __asm ("r0") = __sd;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Close]\n\t"
		    :
                    : "r" (sd),
                      [SWI_XSocket_Close] "i" (Socket_Close | (1<<17))
                    : "r14", "cc");
}

static inline int
_sselect (int __nfds, fd_set *__readfds, fd_set *__writefds,
	  fd_set *__exceptfds, struct timeval *__timeout)
{
  register int nfds __asm ("r0") = __nfds;
  register fd_set *readfds __asm ("r1") = __readfds;
  register fd_set *writefds __asm ("r2") = __writefds;
  register fd_set *exceptfds __asm ("r3") = __exceptfds;
  register struct timeval *timeout __asm ("r4") = __timeout;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Select]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (nfds), "r" (readfds), "r" (writefds),
		      "r" (exceptfds), "r" (timeout),
                      [SWI_XSocket_Select] "i" (Socket_Select | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_send (int __fd, __const void *__buf, size_t __len, int __flags)
{
  register int fd __asm ("r0") = __fd;
  register __const void *buf __asm ("r1") = __buf;
  register size_t len __asm ("r2") = __len;
  register int flags __asm ("r3") = __flags;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Send]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (fd), "r" (buf), "r" (len), "r" (flags),
                      [SWI_XSocket_Send] "i" (Socket_Send | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_sendmsg (int __fd, __const struct msghdr *__msg, int __flags)
{
  register int fd __asm ("r0") = __fd;
  register __const struct msghdr *msg __asm ("r1") = __msg;
  register int flags __asm ("r2") = __flags;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Sendmsg_1]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (fd), "r" (msg), "r" (flags),
                      [SWI_XSocket_Sendmsg_1] "i" (Socket_Sendmsg_1 | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_sendto (int __fd, __const void *__buf, size_t __len, int __flags,
	 __CONST_SOCKADDR_ARG __addr, socklen_t __addrlen)
{
  register int fd __asm ("r0") = __fd;
  register __const void *buf __asm ("r1") = __buf;
  register size_t len __asm ("r2") = __len;
  register int flags __asm ("r3") = __flags;
  register __CONST_SOCKADDR_ARG addr __asm ("r4") = __addr;
  register socklen_t addrlen __asm ("r5") = __addrlen;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Sendto]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : [err] "=r" (err)
                    : "r" (fd), "r" (buf), "r" (len), "r" (flags),
		      "r" (addr), "r" (addrlen),
                      [SWI_XSocket_Sendto] "i" (Socket_Sendto | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_setsockopt (int __fd, int __level, int __optname, const void *__optval,
	     socklen_t __optlen)
{
  register int fd __asm ("r0") = __fd;
  register int level __asm ("r1") = __level;
  register int optname __asm ("r2") = __optname;
  register const void *optval __asm ("r3") = __optval;
  register socklen_t optlen __asm ("r4") = __optlen;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Setsockopt]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (fd), "r" (level), "r" (optname), "r" (optval),
		      "r" (optlen),
                      [SWI_XSocket_Setsockopt] "i" (Socket_Setsockopt | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_shutdown (int __fd, int __how)
{
  register int fd __asm ("r0") = __fd;
  register int how __asm ("r1") = __how;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Shutdown]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (fd), "r" (how),
                      [SWI_XSocket_Shutdown] "i" (Socket_Shutdown | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_sioctl (int __sd, unsigned long __request, void *__arg)
{
  register int sd __asm ("r0") = __sd;
  register unsigned long request __asm ("r1") = __request;
  register void *arg __asm ("r2") = __arg;
  register const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Ioctl]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err)
                    : "r" (sd), "r" (request), "r" (arg),
                      [SWI_XSocket_Ioctl] "i" (Socket_Ioctl | (1<<17))
                    : "r14", "cc");
  return __net_error (0, err);
}

static inline int
_socket (int __domain, int __type, int __protocol)
{
  register int domain __asm ("r0") = __domain;
  register int type __asm ("r1") = __type;
  register int protocol __asm ("r2") = __protocol;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Creat]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (domain), "r" (type), "r" (protocol),
                      [SWI_XSocket_Creat] "i" (Socket_Creat | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_sread (int __sd, void *__buf, size_t __count)
{
  register int sd __asm ("r0") = __sd;
  register void *buf __asm ("r1") = __buf;
  register size_t count __asm ("r2") = __count;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Read]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (sd), "r" (buf), "r" (count),
                      [SWI_XSocket_Read] "i" (Socket_Read | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_swrite (int __sd, const void *__buf, size_t __count)
{
  register int sd __asm ("r0") = __sd;
  register const void *buf __asm ("r1") = __buf;
  register size_t count __asm ("r2") = __count;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Write]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
                    : "r" (sd), "r" (buf), "r" (count),
                      [SWI_XSocket_Write] "i" (Socket_Write | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_sreadv (int __sd, const struct iovec *__vector, int __count)
{
  register int sd __asm ("r0") = __sd;
  register const struct iovec *vector __asm ("r1") = __vector;
  register int count __asm ("r2") = __count;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Readv]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
		    : "r" (sd), "r" (vector), "r" (count),
                      [SWI_XSocket_Readv] "i" (Socket_Readv | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

static inline ssize_t
_swritev (int __sd, const struct iovec *__vector, int __count)
{
  register int sd __asm ("r0") = __sd;
  register const struct iovec *vector __asm ("r1") = __vector;
  register int count __asm ("r2") = __count;
  register ssize_t rtrn __asm ("r0");
  const _kernel_oserror *err;
  __asm__ volatile ("SWI\t%[SWI_XSocket_Writev]\n\t"
		    "MOVVC\t%[err], #0\n\t"
		    "MOVVS\t%[err], r0\n\t"
		    : [err] "=r" (err), "=r" (rtrn)
		    : "r" (sd), "r" (vector), "r" (count),
                      [SWI_XSocket_Writev] "i" (Socket_Writev | (1<<17))
                    : "r14", "cc");
  return __net_error (rtrn, err);
}

#ifndef __TARGET_SCL__
/* Return a socket of any type.  The socket can be used in subsequent
   ioctl calls to talk to the kernel.  */
extern int __opensock (void) internal_function;
#endif

__END_DECLS

#endif
