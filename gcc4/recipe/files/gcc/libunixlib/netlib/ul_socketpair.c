/* socketpair ()
 * Written by Peter Naulls
 * Copyright (c) 2003-2010 UnixLib Developers
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int
socketpair (int af, int type, int protocol, int fd[2])
{
  /* The following is only valid if type == SOCK_STREAM */
  if (type != SOCK_STREAM)
    return -1;

  /* Create a temporary listen socket; temporary, so any port is good */
  int listen_socket = socket (af, type, protocol);
  if (listen_socket < 0)
    return -1;

  struct sockaddr_in s_in[2];
  s_in[0].sin_family = af;
  s_in[0].sin_port = 0; /* Use any port number */
  s_in[0].sin_addr = inet_makeaddr(INADDR_ANY, 0);
  int len = sizeof(s_in[0]);
  /* getsockname() : read the port number we got, so that our client can connect to it
     listen() : put the listen socket in listening mode
     socket() : create the client socket  */
  if (bind (listen_socket, (const struct sockaddr *)&s_in[0], sizeof(s_in[0])) < 0
      || getsockname (listen_socket, (struct sockaddr *)&s_in[0], (socklen_t *)&len) < 0
      || listen (listen_socket, 5) < 0
      || (fd[1] = socket (af, type, protocol)) < 0)
    {
      int save_errno = errno;
      close (listen_socket);
      return __set_errno (save_errno);
    }

  /* Put the client socket in non-blocking connecting mode.  */
  fcntl (fd[1], F_SETFL, fcntl(fd[1], F_GETFL, 0) | O_NDELAY);

  len = sizeof(s_in[1]);
  /* accept() : at the listen-side, accept the incoming connection we generated */
  if (connect (fd[1], (const struct sockaddr *)&s_in[0], sizeof(s_in[0])) < 0
      || (fd[0] = accept (listen_socket, (struct sockaddr *)&s_in[1], (socklen_t *)&len)) < 0)
    {
      int save_errno = errno;
      close (fd[1]);
      close (listen_socket);
      return __set_errno (save_errno);
    }

  /* Reset the client socket to blocking mode */
  fcntl (fd[1], F_SETFL, fcntl(fd[1], F_GETFL, 0) & ~O_NDELAY);

  close (listen_socket);

  return 0;
}
