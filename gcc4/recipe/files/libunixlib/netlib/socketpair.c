/* socketpair ()
 * Written by Peter Naulls
 * Copyright (c) 2003-2006 UnixLib Developers
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int
socketpair (int af, int type, int protocol, int fd[2])
{
  int listen_socket;
  struct sockaddr_in s_in[2];
  int len;

  /* The following is only valid if type == SOCK_STREAM */
  if (type != SOCK_STREAM)
    return -1;

  /* Create a temporary listen socket; temporary, so any port is good */
  listen_socket = socket(af, type, protocol);
  if (listen_socket < 0)
    {
      perror("creating listen_socket");
      return -1;
    }
  s_in[0].sin_family = af;
  s_in[0].sin_port = 0; /* Use any port number */
  s_in[0].sin_addr = inet_makeaddr(INADDR_ANY, 0);
  if (bind(listen_socket, (const struct sockaddr *)&s_in[0], sizeof(s_in[0])) < 0)
    {
      perror("bind");
      return -1;
    }
  len = sizeof(s_in[0]);

  /* Read the port number we got, so that our client can connect to it */
  if (getsockname(listen_socket, (struct sockaddr *)&s_in[0], (socklen_t *)&len) < 0)
    {
      perror("getsockname");
      return -1;
    }

  /* Put the listen socket in listening mode */
  if (listen(listen_socket, 5) < 0)
    {
      perror("listen");
      return -1;
    }

  /* Create the client socket */
  fd[1] = socket(af, type, protocol);
  if (fd[1] < 0)
    {
      perror("creating client_socket");
      return -1;
    }

  /* Put the client socket in non-blocking connecting mode */
  fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL, 0) | O_NDELAY);
  if (connect(fd[1], (const struct sockaddr *)&s_in[0], sizeof(s_in[0])) < 0)
    {
      perror("connect");
      return -1;
    }

  /* At the listen-side, accept the incoming connection we generated */
  len = sizeof(s_in[1]);
  if ((fd[0] = accept(listen_socket, (struct sockaddr *)&s_in[1], (socklen_t *)&len)) < 0)
    {
      perror("accept");
      return -1;
    }

  /* Reset the client socket to blocking mode */
  fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL, 0) & ~O_NDELAY);

  close(listen_socket);

  return 0;
}
