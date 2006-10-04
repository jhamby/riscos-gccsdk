; _swrite
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_swrite|
	NAME	_swrite
|_swrite|
	NetSWIsimple	XSocket_Write

	END
