; _shutdown
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_shutdown|
	NAME	_shutdown
|_shutdown|
	NetSWIsimple0	XSocket_Shutdown

	END
