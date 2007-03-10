@ _bind
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_bind
	NAME	_bind
_bind:
	NetSWIsimple0	XSocket_Bind

	.end
