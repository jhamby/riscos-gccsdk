;----------------------------------------------------------------------------
;
; $Source$
; $Date$
; $Revision$
; $State$
; $Author$
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_getpeername|
	NAME	_getpeername
|_getpeername|
	[ COMPAT_INET4 = 1
	NetSWIsimple0	XSocket_Getpeername
	|
	NetSWIsimple0	XSocket_Getpeername_1
	]

	END
