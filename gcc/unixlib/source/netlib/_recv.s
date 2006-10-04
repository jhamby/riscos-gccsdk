; _recv
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_recv|
	NAME	_recv
|_recv|
	NetSWIsimple	XSocket_Recv

	END
