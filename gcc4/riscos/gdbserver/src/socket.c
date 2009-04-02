#include <stddef.h>
#include <string.h>

#include <swis.h>

#include "debug.h"
#include "socket.h"
#include "utils.h"

/******************************************************************************
 * Very rudimentary socket library                                            *
 ******************************************************************************/

struct hostent {
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
#define h_addr h_addr_list[0]
};

#define INADDR_ANY (uint32_t)0x00000000
struct in_addr {
	uint32_t s_addr;
};

struct sockaddr {
	uint16_t sa_family;
	uint8_t sa_data[14];
};

struct sockaddr_in {
	uint16_t sin_family;
	uint16_t sin_port;
	struct in_addr sin_addr;
	uint8_t sin_zero[8];
};

#define PF_INET 2
#define AF_INET 2
#define SOCK_STREAM 1

#define FIONBIO 0x8004667e
#define FIOASYNC 0x8004667d
#define FIOSLEEPTW 0x80046679

static int accept(int sockfd, struct sockaddr *addr, size_t *addrlen)
{
	int child;
	_kernel_oserror *e;

	e = _swix(Socket_Accept, _INR(0,2) | _OUT(0),
			sockfd, addr, addrlen,
			&child);
	if (e != NULL) {
		debug("Socket_Accept: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return child;
}

static int bind(int sockfd, const struct sockaddr *addr, size_t addrlen)
{
	int error;
	_kernel_oserror *e;

	e = _swix(Socket_Bind, _INR(0,2) | _OUT(0),
			sockfd, addr, addrlen,
			&error);
	if (e != NULL) {
		debug("Socket_Bind: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return error;
}

static int connect(int sockfd, const struct sockaddr *serv_addr, size_t addrlen)
{
	int error;
	_kernel_oserror *e;

	e = _swix(Socket_Connect, _INR(0,2) | _OUT(0),
			sockfd, serv_addr, addrlen,
			&error);
	if (e != NULL) {
		debug("Socket_Connect: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return error;
}

static struct hostent *gethostbyname(const char *name)
{
	int error;
	struct hostent *h;
	_kernel_oserror *e;

	e = _swix(Internet_GetHostByName, 
			_INR(0,1) | _OUTR(0,1),
			0, name,
			&error, &h);

	if (e != NULL || error != 0)
		return NULL;

	return h;
}

#define htonl(x) ((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) | \
		 (((x) & 0xff0000) >> 8) | (((x) & 0xff000000) >> 24))
#define htons(x) ((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))

static int ioctl(int s, int request, void *data)
{
	int error;
	_kernel_oserror *e;

	e = _swix(Socket_Ioctl, _INR(0,2) | _OUT(0),
			s, request, data,
			&error);
	if (e != NULL) {
		debug("Socket_Ioctl: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return error;
}

static int listen(int sockfd, int backlog)
{
	int error;
	_kernel_oserror *e;

	e = _swix(Socket_Listen, _INR(0,1) | _OUT(0),
			sockfd, backlog,
			&error);
	if (e != NULL) {
		debug("Socket_Listen: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return error;
}

static ssize_t recv(int s, void *buf, size_t len, int flags)
{
	ssize_t read;
	_kernel_oserror *e;

	e = _swix(Socket_Read, _INR(0,3) | _OUT(0),
			s, buf, len, flags,
			&read);
	if (e != NULL) {
		debug("Socket_Read: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return read;
}

static ssize_t send(int s, const void *buf, size_t len, int flags)
{
	ssize_t sent;
	_kernel_oserror *e;

	e = _swix(Socket_Send, _INR(0,3) | _OUT(0),
			s, buf, len, flags,
			&sent);
	if (e != NULL) {
		debug("Socket_Send: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return sent;
}

static int shutdown(int s, int how)
{
	int error;
	_kernel_oserror *e;

	e = _swix(Socket_Shutdown, _INR(0,1) | _OUT(0),
			s, how,
			&error);
	if (e != NULL) {
		debug("Socket_Shutdown: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return error;
}

static int socket(int domain, int type, int protocol)
{
	int sock;
	_kernel_oserror *e;

	e = _swix(Socket_Creat, _INR(0,2) | _OUT(0),
			domain, type, protocol,
			&sock);
	if (e != NULL) {
		debug("Socket_Creat: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return sock;
}

static int socketclose(int sock)
{
	int error;
	_kernel_oserror *e;

	e = _swix(Socket_Close, _IN(0) | _OUT(0),
			sock,
			&error);
	if (e != NULL) {
		debug("Socket_Close: 0x%x %s\n", e->errnum, e->errmess);
		return -1;
	}

	return error;
}

/******************************************************************************
 * Public API                                                                 *
 ******************************************************************************/

int socket_open_client(const char *host, int port)
{
	int one = 1;
	struct hostent *h;
	unsigned long a;
	int sock;
	struct sockaddr_in s;

	if ((h = gethostbyname(host)) == NULL)
		return -1;

	memcpy(&a, h->h_addr, sizeof(a));

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return -1;

	memset(&s, 0, sizeof(struct sockaddr_in));
	s.sin_family = AF_INET;
	s.sin_addr.s_addr = a;
	s.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *) &s, 
			sizeof(struct sockaddr_in)) < 0) {
		socketclose(sock);
		return -1;
	}

	ioctl(sock, FIOSLEEPTW, &one);
	ioctl(sock, FIOASYNC, &one);
	ioctl(sock, FIONBIO, &one);

	return sock;
}

int socket_open_server(int port)
{
	int one = 1;
	int sock;
	struct sockaddr_in s;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return -1;

	memset(&s, 0, sizeof(struct sockaddr_in));
	s.sin_family = AF_INET;
	s.sin_addr.s_addr = htonl(INADDR_ANY);
	s.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *) &s, 
			sizeof(struct sockaddr_in)) < 0) {
		shutdown(sock, 2 /* SHUT_RDWR */);
		socketclose(sock);
		return -1;
	}

	ioctl(sock, FIOSLEEPTW, &one);
	ioctl(sock, FIOASYNC, &one);
	ioctl(sock, FIONBIO, &one);

	if (listen(sock, 5) < 0) {
		shutdown(sock, 2);
		socketclose(sock);
		return -1;
	}

	return sock;
}

void socket_close(int h)
{
	shutdown(h, 2 /* SHUT_RDWR */);
	socketclose(h);
}

int socket_accept(int h)
{
	int one = 1;
	int client;
	struct sockaddr_in caddr;
	size_t clen = sizeof(struct sockaddr_in);

	client = accept(h, (struct sockaddr *) &caddr, &clen);
	if (client < 0) {
		return -1;
	}

	ioctl(client, FIOSLEEPTW, &one);
	ioctl(client, FIOASYNC, &one);
	ioctl(client, FIONBIO, &one);

	return client;
}

size_t socket_send(int h, const uint8_t *data, size_t len)
{
	int32_t sent;

	for (sent = 0; len > 0; ) {
		if ((sent = send(h, data, len, 0)) == -1) {
			return len;
		}

		len -= sent;
		data += sent;
	}

	return 0;
}

ssize_t socket_recv(int h, uint8_t *buf, size_t len)
{
	return recv(h, buf, len, 0);
}

