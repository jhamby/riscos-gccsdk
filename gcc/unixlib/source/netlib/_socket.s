; _socket
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_socket|
	NAME	_socket
|_socket|
	NetSWIsimple	XSocket_Creat

	END
