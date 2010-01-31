@ _recv
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_recv
	NAME	_recv
_recv:
	NetSWIsimple	XSocket_Recv

	.end
