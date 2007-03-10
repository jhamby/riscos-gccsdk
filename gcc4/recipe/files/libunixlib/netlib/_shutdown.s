@ _shutdown
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_shutdown
	NAME	_shutdown
_shutdown:
	NetSWIsimple0	XSocket_Shutdown

	.end
