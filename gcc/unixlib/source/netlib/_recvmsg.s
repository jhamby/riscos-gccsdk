;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_recvmsg.s,v $
; $Date: 2002/07/19 13:12:30 $
; $Revision: 1.2.2.1 $
; $State: Exp $
; $Author: admin $
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
