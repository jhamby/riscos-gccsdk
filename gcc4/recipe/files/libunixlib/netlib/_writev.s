@ _writev
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_writev
	NAME	_writev
_writev:
	NetSWIsimple	XSocket_Writev

	.end
