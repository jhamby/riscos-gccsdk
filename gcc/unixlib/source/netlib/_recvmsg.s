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

	EXPORT	|_recvmsg|
	NAME	_recvmsg
|_recvmsg|
	[ COMPAT_INET4 = 1
	NetSWIsimple	XSocket_Recvmsg
	|
	NetSWIsimple	XSocket_Recvmsg_1
	]

	END
