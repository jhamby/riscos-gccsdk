@ _sclose
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_sclose
	NAME	_sclose
_sclose:
	NetSWIsimple0	XSocket_Close


	.end
