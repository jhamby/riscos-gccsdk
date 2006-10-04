; _sendtosm
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sendtosm|
	NAME	_sendtosm
|_sendtosm|
	NetSWIsimple	XSocket_Sendtosm

	END
