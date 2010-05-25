#ifndef src_socket_h_
#define src_socket_h_

#include <stdint.h>
#include <unistd.h>

int socket_open_client(const char *host, int port);
int socket_open_server(int port);
void socket_close(int h);

int socket_accept(int h);

size_t socket_send(int h, const uint8_t *data, size_t len);
ssize_t socket_recv(int h, uint8_t *buf, size_t len);

#endif

