; _send
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_send|
	NAME	_send
|_send|
	NetSWIsimple	XSocket_Send

	END
