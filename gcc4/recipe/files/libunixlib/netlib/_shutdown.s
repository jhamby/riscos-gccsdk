@ _shutdown
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_shutdown
	NAME	_shutdown
_shutdown:
	NetSWIsimple0	XSocket_Shutdown

	.end
