@ _sclose
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_sclose
	NAME	_sclose
_sclose:
	NetSWIsimple0	XSocket_Close


	.end
