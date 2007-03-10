@ _socket
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_socket
	NAME	_socket
_socket:
	NetSWIsimple	XSocket_Creat

	.end
