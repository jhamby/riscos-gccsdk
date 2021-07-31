#include <stddef.h>
#include <string.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <swis.h>

#include "debug.h"
#include "socket.h"

int
socket_open_client (const char *host, int port)
{
  struct hostent *h;
  if ((h = gethostbyname (host)) == NULL)
    return -1;

  unsigned long a;
  memcpy (&a, h->h_addr, sizeof (a));

  int sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    return -1;

  struct sockaddr_in s;
  memset (&s, 0, sizeof (struct sockaddr_in));
  s.sin_family = AF_INET;
  s.sin_addr.s_addr = a;
  s.sin_port = htons (port);

  if (connect (sock, (struct sockaddr *) &s, sizeof (struct sockaddr_in)) < 0)
    {
      socketclose (sock);
      return -1;
    }

  int one = 1;
  ioctl (sock, FIOSLEEPTW, &one);
  ioctl (sock, FIOASYNC, &one);
  ioctl (sock, FIONBIO, &one);

  return sock;
}

int
socket_open_server (int port)
{
  int sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    return -1;

  int one = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

  struct sockaddr_in s;
  memset (&s, 0, sizeof (struct sockaddr_in));
  s.sin_family = AF_INET;
  s.sin_addr.s_addr = htonl (INADDR_ANY);
  s.sin_port = htons (port);

  if (bind (sock, (struct sockaddr *) &s, sizeof (struct sockaddr_in)) < 0)
    {
      shutdown (sock, SHUT_RDWR);
      socketclose (sock);
      return -1;
    }

  ioctl (sock, FIOSLEEPTW, &one);
  ioctl (sock, FIOASYNC, &one);
  ioctl (sock, FIONBIO, &one);

  if (listen (sock, 5) < 0)
    {
      shutdown (sock, 2);
      socketclose (sock);
      return -1;
    }

  return sock;
}

void
socket_close (int h)
{
  shutdown (h, SHUT_RDWR);
  socketclose (h);
}

int
socket_accept (int h)
{
  struct sockaddr_in caddr;
  socklen_t clen = sizeof (struct sockaddr_in);
  int client = accept (h, (struct sockaddr *) &caddr, &clen);
  if (client < 0)
    return -1;

  int one = 1;
  ioctl (client, FIOSLEEPTW, &one);
  ioctl (client, FIOASYNC, &one);
  ioctl (client, FIONBIO, &one);

  return client;
}

size_t
socket_send (int h, const uint8_t * data, size_t len)
{
  while (len)
    {
      int32_t sent;
      if ((sent = send (h, data, len, 0)) == -1)
	{
	  dprintf ("socket_send(): failed to send data.\n");
	  return 0;
	}

      len -= sent;
      data += sent;
    }

  return 0;
}

ssize_t
socket_recv (int h, uint8_t * buf, size_t len)
{
  return recv (h, buf, len, 0);
}
