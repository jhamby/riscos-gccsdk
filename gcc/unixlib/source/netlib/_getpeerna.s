;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_getpeerna.s,v $
; $Date: 2002/12/22 18:22:29 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_getpeername|
	NAME	_getpeername
|_getpeername|
	NetSWIsimple0	XSocket_Getpeername
	END
