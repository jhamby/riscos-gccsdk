@ _writev
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_writev
	NAME	_writev
_writev:
	NetSWIsimple	XSocket_Writev

	.end
