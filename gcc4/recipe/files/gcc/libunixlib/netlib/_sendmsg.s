@ _sendmsg
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_sendmsg
	NAME	_sendmsg
_sendmsg:
	NetSWIsimple	XSocket_Sendmsg_1

	.end
