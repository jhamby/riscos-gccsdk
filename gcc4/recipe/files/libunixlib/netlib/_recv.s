@ _recv
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_recv
	NAME	_recv
_recv:
	NetSWIsimple	XSocket_Recv

	.end
