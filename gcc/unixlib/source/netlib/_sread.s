;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_sread.s,v $
; $Date: 2000/07/15 14:52:27 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_sread|
|_sread|
	NetSWIsimple	XSocket_Read


	END
