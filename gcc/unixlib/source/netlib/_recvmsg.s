;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_recvmsg.s,v $
; $Date: 2003/05/13 22:59:47 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_recvmsg|
	NAME	_recvmsg
|_recvmsg|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple	XSocket_Recvmsg
	|
	NetSWIsimple	XSocket_Recvmsg_1
	]

	END
