;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_profile.s,v $
; $Date: 2001/09/11 13:05:55 $
; $Revision: 1.2.2.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_count|
|_count|
	; Increment the work pointed to by lr, then add 4 to
	; lr to get the true return address
	LDR	ip, [lr, #0]
	ADD	ip, ip, #1
	STR	ip, [lr], #4
	return	AL, pc, lr

	EXPORT	|_count1|
|_count1|
	; Increment the work pointed to by lr, then add 8 to
	; lr to get the true return address
	LDR	ip, [lr, #0]
	ADD	ip, ip, #1
	STR	ip, [lr], #8
	return	AL, pc, lr

	END
