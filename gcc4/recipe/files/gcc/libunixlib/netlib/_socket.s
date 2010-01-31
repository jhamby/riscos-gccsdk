@ _socket
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_socket
	NAME	_socket
_socket:
	NetSWIsimple	XSocket_Creat

	.end
