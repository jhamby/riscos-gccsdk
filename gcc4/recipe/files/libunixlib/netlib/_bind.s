@ _bind
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_bind
	NAME	_bind
_bind:
	NetSWIsimple0	XSocket_Bind

	.end
