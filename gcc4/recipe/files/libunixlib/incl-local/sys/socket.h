/* Internal UnixLib sys/socket.h
 * Copyright (c) 2008 UnixLib Developers
 */

#ifndef __SYS_SOCKET_H
#include_next <sys/socket.h>
#endif

#if !defined(__INTERNAL_SYS_SOCKET_H) && defined(__SYS_SOCKET_H)
#define	__INTERNAL_SYS_SOCKET_H

__BEGIN_DECLS

/* Direct SWI veneers: */
extern int _socket (int __domain, int __type, int __protocol);
extern int _bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
extern int _listen (int __fd, int __n);
extern int _accept (int __fd, __SOCKADDR_ARG __addr,
		    socklen_t *__restrict __addr_len);
extern int _connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
extern ssize_t _recv (int __fd, void *__buf, size_t __n, int __flags);
extern ssize_t _recvfrom (int __fd, void *__restrict __buf, size_t __n, int __flags,
			  __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len);
extern ssize_t _recvmsg (int __fd, struct msghdr *__message, int __flags);
extern ssize_t _send (int __fd, __const void *__buf, size_t __n, int __flags);
extern ssize_t _sendto (int __fd, __const void *__buf, size_t __n,
		        int __flags, __CONST_SOCKADDR_ARG __addr,
		        socklen_t __addr_len);
extern ssize_t _sendmsg (int __fd, __const struct msghdr *__message, int __flags);
extern int _shutdown (int __fd, int __how);
extern int _setsockopt (int __fd, int __level, int __optname,
		        __const void *__optval, socklen_t __optlen);
extern int _getsockopt (int __fd, int __level, int __optname,
		        void *__restrict __optval,
		        socklen_t *__restrict __optlen);
extern int _getsockname (int __fd, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __len);
extern int _getpeername (int __fd, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __len);

/* Return a socket of any type.  The socket can be used in subsequent
   ioctl calls to talk to the kernel.  */
extern int __opensock (void) internal_function;

__END_DECLS

#endif
