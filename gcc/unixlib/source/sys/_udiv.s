;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_udiv.s,v $
; $Date: 2001/01/29 15:10:21 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

dividend RN 1
divisor RN 0

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_kernel_udiv|
	EXPORT	|x$udivide|
	EXPORT	|__rt_udiv|
	NAME	__rt_udiv
|_kernel_udiv|
|x$udivide|
|__rt_udiv|
        MOV     a3,dividend
        MOVS    ip,divisor
	; just return for divide by zero
	return	EQ, pc, lr

        CMP     ip,a3,LSR #16
        MOVLS   ip,ip,LSL #16
        CMP     ip,a3,LSR #8
        MOVLS   ip,ip,LSL #8
        CMP     ip,a3,LSR #4
        MOVLS   ip,ip,LSL #4
        CMP     ip,a3,LSR #2
        MOVLS   ip,ip,LSL #2
        CMP     ip,a3,LSR #1
        MOVLS   ip,ip,LSL #1

        MOV     a2,#0
divloop
	CMP     a3,ip
        SUBCS   a3,a3,ip
        ADC     a2,a2,a2
        MOV     ip,ip,LSR #1
        CMP     ip,a1
        BCS     divloop
	MOV	a1, a2
	MOV	a2, a3
	return	AL, pc, lr

	END
