;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_umod.s,v $
; $Date: 2005/01/03 22:55:13 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

divisor RN 0
dividend RN 1

	AREA	|C$$code|, CODE, READONLY

	IMPORT	raise

	EXPORT	|__rt_urem|
	EXPORT	|x$uremainder|
	EXPORT	|_kernel_urem|
	NAME	__rt_urem
|_kernel_urem|
|x$uremainder|
|__rt_urem|
        MOVS    a3, divisor
	; Raise an abort when there is a division by zero.
	BEQ	divbyzero

        CMP     a3, dividend, LSR #16
        MOVLS   a3, a3, LSL #16
        CMP     a3, dividend,LSR #8
        MOVLS   a3, a3, LSL #8
        CMP     a3, dividend,LSR #4
        MOVLS   a3, a3, LSL #4
        CMP     a3, dividend,LSR #2
        MOVLS   a3, a3, LSL #2
        CMP     a3, dividend,LSR #1
        MOVLS   a3, a3, LSL #1

divloop
	CMP     dividend, a3
        SUBCS   dividend, dividend, a3
        MOV     a3, a3, LSR #1
        CMP     a3, divisor
        BCS     divloop
	MOV	a1, dividend
	MOV	pc, lr

divbyzero
	MOV	a1, #SIGFPE
	B	raise

	END
