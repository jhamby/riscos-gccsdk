;----------------------------------------------------------------------------
;
; $Source: $
; $Date: $
; $Revision: $
; $State: $
; $Author: $
;
;----------------------------------------------------------------------------

	;  Some routines are based on code from Iyonix's HAL v0.10 (22 Jan 2003)
	;  with the following license :

	;  This program is free software; you can redistribute it and/or modify it
	;  under the terms of version 2 of the GNU General Public License as
	;  published by the Free Software Foundation;
	;
	;  This program is distributed in the hope that it will be useful, but WITHOUT
	;  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	;  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
	;  more details.
	;
	;  You should have received a copy of the GNU General Public License along with
	;  this program; if not, write to the Free Software Foundation, Inc., 59
	;  Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	GBLL	MULLSupported
;	[ {ARCHITECTURE} = "3M" :LOR: {ARCHITECTURE} = "4" :LOR: {ARCHITECTURE} = "4T" :LOR: {ARCHITECTURE} = "5" :LOR: {ARCHITECTURE} = "5T" :LOR: {ARCHITECTURE} = "5TExP" :LOR: {ARCHITECTURE} = "5TE"
MULLSupported	SETL	{TRUE}
;	|
;MULLSupported	SETL	{FALSE}
;	]

	;Naming conventions used :
	; 's' = int32_t
	; 'u' = uint32_t
	; 'l' = int64_t

	; int64_t _ll_from_u(uint32_t a)
	;  { return (int64_t)a; }
	EXPORT	|_ll_from_u|
	NAME	_ll_from_u
|_ll_from_u|
	MOV	a1, #0
	return	AL, pc, lr

	; int64_t _ll_from_l(int32_t a)
	;  { return (int64_t)a; }
	EXPORT	|_ll_from_l|
	NAME	_ll_from_l
|_ll_from_l|
	MOV 	a2, a1, ASR#32
	return	AL, pc, lr

	; int64_t _ll_to_l(int64_t a)
	;  { return (int64_t)a; }
	EXPORT	|_ll_to_l|
	NAME	_ll_to_l
|_ll_to_l|
	return	AL, pc, lr

	; int64_t _ll_add(int64_t a, int64_t b)
	;  { return a + b; }
	EXPORT	|_ll_add|
	NAME	_ll_add
|_ll_add|
	ADDS	a1, a1, a3
	ADC	a2, a2, a4
	return	AL, pc, lr

	; int64_t _ll_addlu(int64_t a, uint32_t b)
	;  { return a + (uint64_t)b; }
	EXPORT	|_ll_addlu|
	NAME	_ll_addlu
|_ll_addlu|
	ADDS	a1, a1, a3
	ADC	a2, a2, #0
	return	AL, pc, lr

	; int64_t _ll_addls(int64_t a, int32_t b)
	;  { return a + (int64_t)b; }
	EXPORT	|_ll_addls|
	NAME	_ll_addls
|_ll_addls|
	ADDS	a1, a1, a3
	ADC	a2, a2, a3, ASR#32
	return	AL, pc, lr

	; int64_t _ll_adduu(uint32_t a, uint32_t b)
        ;  { return (int64_t)a + (int64_t)b; }
	EXPORT	|_ll_adduu|
	NAME	_ll_adduu
|_ll_adduu|
	MOV	a3, #0
	ADDS	a1, a1, a2
	ADC	a2, a3, #0
	return	AL, pc, lr

	; int64_t _ll_addss(int32_t a, int32_t b)
        ;  { return (int64_t)a + (int64_t)b; }
	EXPORT	|_ll_addss|
	NAME	_ll_addss
|_ll_addss|
	MOV	a3, a1, ASR#32
	ADDS	a1, a1, a2
	ADC	a2, a3, a2, ASR#32
	return	AL, pc, lr

	; int64_t _ll_sub(int64_t a, int64_t b)
	;  { return a - b; }
	EXPORT	|_ll_sub|
	NAME	_ll_sub
|_ll_sub|
	SUBS	a1, a1, a2
	SBC	a2, a2, a3
	return	AL, pc, lr

	; int64_t _ll_sublu(int64_t a, uint32_t b)
	;  { return a - (int64_t)b; }
	EXPORT	|_ll_sublu|
	NAME	_ll_sublu
|_ll_sublu|
	SUBS	a1, a1, a2
	SBC	a2, a2, #0
	return	AL, pc, lr

	; int64_t _ll_subls(int64_t a, int32_t b)
	;  { return a - (int64_t)b; }
	EXPORT	|_ll_subls|
	NAME	_ll_subls
|_ll_subls|
	SUBS	a1, a1, a3
	SBC	a2, a2, a3, ASR#32
	return	AL, pc, lr

	; int64_t _ll_subuu(uint32_t a, uint32_t b)
	;  { return (int64_t)a - (int64_t)b; }
	EXPORT	|_ll_subuu|
	NAME	_ll_subuu
|_ll_subuu|
	SUBS	a1, a1, a2
	SBC	a2, a2, a2
	return	AL, pc, lr

	; int64_t _ll_subss(int32_t a, int32_t b)
	;  { return (int64_t)a - (int64_t)b; }
	EXPORT	|_ll_subss|
	NAME	_ll_subss
|_ll_subss|
	MOV	a3, a1, ASR#32
	SUBS	a1, a1, a2
	SBC	a2, a3, a2, ASR#32
	return	AL, pc, lr

	; int64_t _ll_rsb(int64_t a, int64_t b)
	;  { return b - a; }
	EXPORT	|_ll_rsb|
	NAME	_ll_rsb
|_ll_rsb|
	RSBS	a1, a1, a3
	RSC	a2, a2, a4
	return	AL, pc, lr

	; int64_t _ll_rsblu(int64_t a, uint32_t b)
	;  { return (int64_t)b - a; }
	EXPORT	|_ll_rsblu|
	NAME	_ll_rsblu
|_ll_rsblu|
	RSBS	a1, a1, a3
	RSC	a2, a2, #0
	return	AL, pc, lr

	; int64_t _ll_rsbls(int64_t a, int32_t b)
	;  { return (int64_t)b - a; }
	EXPORT	|_ll_rsbls|
	NAME	_ll_rsbls
|_ll_rsbls|
	RSBS	a1, a1, a3
	RSC	a2, a2, a3, ASR#32
	return	AL, pc, lr

	; int64_t _ll_rsbuu(uint32_t a, uint32_t b)
	;  { return (int64_t)b - (int64_t)a; }
	EXPORT	|_ll_rsbuu|
	NAME	_ll_rsbuu
|_ll_rsbuu|
	RSBS	a1, a1, a2
	RSC	a2, a2, a2
	return	AL, pc, lr

	; int64_t _ll_rsbss(int32_t a, int32_t b)
	;  { return (int64_t)b - (int64_t)a; }
	EXPORT	|_ll_rsbss|
	NAME	_ll_rsbss
|_ll_rsbss|
	MOV	a3, a1, ASR#32
	RSBS	a1, a1, a2
	RSB	a2, a3, a2, ASR#32
	return	AL, pc, lr

	; int64_t _ll_mul(int64_t a, int32_t b)
	;  { return a * (int64_t)b; }
	EXPORT	|_ll_mulls|
	NAME	_ll_mul
	NAME	_ll_mulls
|_ll_mulls|
	MOV	a4, a3, ASR#32
	; Fall through |_ll_mul| !!!

	; int64_t _ll_mul(int64_t a, int64_t b)
	;  { return a * b; }
	;                               a2         a1
	;  x                            a4         a3
	; -------------------------------------------
	;                                     a1 * a3
	;  +                (a2 * a3 + a4 * a1) << 32
	; -------------------------------------------
	;                                   a1l * a3l
	;  +            (a3h * a1l + a3l * a1h) << 16
	;  +                        (a1h * a3h) << 32
	;  +                (a2 * a3 + a4 * a1) << 32
	EXPORT	|_ll_mul|
|_ll_mul|
	[ MULLSupported
	MUL	ip, a2, a3		;ip = a2 * a3
	MLA	ip, a4, a1, ip		;ip = a2 * a3 + a4 * a1
	SMULL	a1, a2, a3, a1		;a2,a1 = a1 * a3
	ADD	a2, a2, ip		;a2,a1 = a1 * a3 + (a2 * a3 + a4 * a1) << 32
	return	AL, pc, lr
	|
	STMFD	sp!, {v1-v3, lr}
	MOV	lr, a1
	MOV	v2, a1, LSR #16		;v2 = a1h
	BIC	v1, lr, v2, LSL #16	;v1 = a1l
	MOV	ip, a3, LSR #16		;ip = a3h
	BIC	v3, a3, ip, LSL #16	;v3 = a3l
	MUL	a1, v1, v3		;a1 = a1l * a3l
	MUL	v1, ip, v1		;v1 = a3h * a1l
	MUL	ip, v2, ip		;ip = a1h * a3h
	MUL	v2, v3, v2		;v2 = a3l * a1h
	ADDS	a1, a1, v1, LSL #16	;ip,a1 = a1l * a3l + (a3h * a1l)<<16 + (a1h * a3h)<<32
	ADC	ip, ip, v1, LSR #16
	ADDS	a1, a1, v2, LSL #16	;ip,a1 = a1l * a3l + (a3h * a1l + a3l * a1h)<<16 + (a1h * a3h)<<32
	ADC	ip, ip, v2, LSR #16
	MLA	a2, a3, a2, ip		;a2,a1 = a1l * a3l + (a3h * a1l + a3l * a1h)<<16 + (a1h * a3h + a2 * a3)<<32
	MLA	a2, a4, lr, a2		;a2,a1 = a1l * a3l + (a3h * a1l + a3l * a1h)<<16 + (a1h * a3h + a2 * a3 + a4 * a1)<<32
	stackreturn	AL, "v1-v3, pc"
	]

	; int64_t _ll_mul(int64_t a, uint32_t b)
	;  { return a * (int64_t)b; }
	;                               a2         a1
	;  x                                       a3
	; -------------------------------------------
	;                                     a1 * a3
	;  +                          (a2 * a3) << 32
	; -------------------------------------------
	;                                   a1l * a3l
	;  +            (a3h * a1l + a3l * a1h) << 16
	;  +                        (a1h * a3h) << 32
	;  +                          (a2 * a3) << 32
	EXPORT	|_ll_mullu|
	NAME	_ll_mullu
|_ll_mullu|
	[ MULLSupported
	MOV	ip, a2
	UMULL	a1, a2, a3, a1
	MLA	a2, ip, a3, a2
	return	AL, pc, lr
	|
	STMFD	sp!, {v1-v2, lr}
	MOV	lr, a1
	MOV	v2, a1, LSR #16		;v2 = a1h
	BIC	v1, lr, v2, LSL #16	;v1 = a1l
	MOV	ip, a3, LSR #16		;ip = a3h
	BIC	a4, a3, ip, LSL #16	;a4 = a3l
	MUL	a1, v1, a4		;a1 = a1l * a3l
	MUL	v1, ip, v1		;v1 = a3h * a1l
	MUL	ip, v2, ip		;ip = a1h * a3h
	MUL	v2, a4, v2		;v2 = a3l * a1h
	ADDS	a1, a1, v1, LSL #16	;ip,a1 = a1l * a3l + (a3h * a1l)<<16 + (a1h * a3h)<<32
	ADC	ip, ip, v1, LSR #16
	ADDS	a1, a1, v2, LSL #16	;ip,a1 = a1l * a3l + (a3h * a1l + a3l * a1h)<<16 + (a1h * a3h)<<32
	ADC	ip, ip, v2, LSR #16
	MLA	a2, a3, a2, ip		;a2,a1 = a1l * a3l + (a3h * a1l + a3l * a1h)<<16 + (a1h * a3h + a2 * a3)<<32
	stackreturn	AL, "v1-v2, pc"
	]

	; int64_t _ll_muluu(uint32_t a, uint32_t b)
	;  { return (int64_t)a * (int64_t)b; }
	;                                          a1
	;  x                                       a2
	; -------------------------------------------
	;                                     a1 * a2
	; -------------------------------------------
	;                                   a1l * a2l
	;  +            (a2h * a1l + a2l * a1h) << 16
	;  +                        (a1h * a2h) << 32
	EXPORT	|_ll_muluu|
	NAME	_ll_muluu
|_ll_muluu|
	[ MULLSupported
	MOV	a3, a1
	UMULL	a1, a2, a3, a2
	return	AL, pc, lr
	|
	STMFD	sp!, {v1, lr}
	MOV	lr, a1
	MOV	a4, a1, LSR #16		;a4 = a1h
	BIC	v1, a1, a4, LSL #16	;v1 = a1l
	MOV	ip, a2, LSR #16		;ip = a2h
	BIC	a3, a2, ip, LSL #16	;a3 = a2l
	MUL	a1, v1, a3		;a1 = a1l * a2l
	MUL	v1, ip, v1		;v1 = a2h * a1l
	MUL	ip, a4, ip		;ip = a1h * a2h
	MUL	a4, a3, a4		;a4 = a2l * a1h
	ADDS	a1, a1, v1, LSL #16	;ip,a1 = a1l * a2l + (a2h * a1l)<<16 + (a1h * a2h)<<32
	ADC	ip, ip, v1, LSR #16
	ADDS	a1, a1, a4, LSL #16	;a2,a1 = a1l * a2l + (a2h * a1l + a2l * a1h)<<16 + (a1h * a2h)<<32
	ADC	a2, ip, a4, LSR #16
	stackreturn	AL, "v1, pc"
	]

	; int64_t _ll_mulss(int32_t a, int32_t b)
	;  { return (int64_t)a * (int64_t)b; }
	EXPORT	|_ll_mulss|
	NAME	_ll_mulss
|_ll_mulss|
	[ MULLSupported
	MOV	a3, a1
	SMULL	a1, a2, a3, a2
	return	AL, pc, lr
	|
	MOV	a4, a2, ASR#32
	MOV	a3, a2
	MOV	a2, a1, ASR#32
	B	|_ll_mul|
	]

; *** FIXME ***
;	; uint64_t _ll_udiv(uint64_t a, uint64_t b)
;	;  { return a / b; }
;	; Remainder in a2,a3
;	EXPORT	|_ll_udiv|
;	NAME	_ll_udiv
;|_ll_udiv|

; *** FIXME ***
;	; uint64_t _ll_urdv(uint64_t a, uint64_t b)
;	;  { return a % b; }
;	EXPORT	|_ll_urdv|
;	NAME	_ll_urdv
;|_ll_urdv|

	; uint64_t _ll_udiv10(uint64_t a)
	;  { return a / 10ULL; }
	; a2,a3 contains uint64_t remainder
	; Algorithm is the same as in _kernel_udiv10 and __rt_udiv10.
	EXPORT	|_ll_udiv10|
	NAME	_ll_udiv10
|_ll_udiv10|
	STMFD	sp!, {lr}
	SUBS	a3, a1, #10
	SBCS	a4, a2, #0		; a3,a4 = a1,a2 - 10

	MOV	lr, a1, LSR #2
	ORR	lr, lr, a2, LSL #32 - 2
	SUBS	a1, a1, lr
	SBC	a2, a2, a2, LSR #2	; a1,a2 = a * %0.1100

	MOV	lr, a1, LSR #4
	ORR	lr, lr, a2, LSL #32 - 4
	ADDS	a1, a1, lr
	ADC	a2, a2, a2, LSR #4	; a1,a2 = a * %0.11001100

	MOV	lr, a1, LSR #8
	ORR	lr, lr, a2, LSL #32 - 8
	ADDS	a1, a1, lr
	ADC	a2, a2, a2, LSR #8	; a1,a2 = a * %0.1100110011001100

	MOV	lr, a1, LSR #16
	ORR	lr, lr, a2, LSL #32 - 16
	ADDS	a1, a1, lr
	ADC	a2, a2, a2, LSR #16	; a1,a2 = a * %0.11001100110011001100110011001100

	ADDS	a1, a1, a2		; a1,a2 = a * %0.1100110011001100110011001100110011001100110011001100110011001100
	ADC	a2, a2, #0

	MOV	a1, a1, LSR #3
	ORR	a1, a1, a2, LSL #32 - 3
	MOV	a2, a2, LSR #3		; a1,a2 = a * %0.0001100110011001100110011001100110011001100110011001100110011001100

	MOV	ip, a2, LSL #2
	ORR	ip, ip, a1, LSR #32 - 2
	ADDS	lr, a1, a1, LSL #2
	ADC	ip, ip, a2
	ADDS	lr, lr, lr
	ADC	ip, ip, ip		; ip,lr = 10 * (a1,a2 / 10)

	SUBS	a3, a3, lr
	SBCS	a4, a4, ip		; a2,a3 = a1,a2 - 10 - 10 * (a1,a2 / 10)
	MOV	a4, #0
	ADDMI	a3, a3, #10		; Fix up remainder
	stackreturn	MI, "pc"
	ADDS	a1, a1, #1		; Fix up quotient
	ADC	a2, a2, #0
	stackreturn	AL, "pc"

; *** FIXME ***
;	; int64_t _ll_sdiv(int64_t a, int64_t b)
;	;  { return a / b; }
;	; Remainder in a2,a3
;	EXPORT	|_ll_sdiv|
;	NAME	_ll_sdiv
;|_ll_sdiv|

; *** FIXME ***
;	; int64_t _ll_srdv(int64_t a, int64_t b)
;	;  { return a % b; }
;	EXPORT	|_ll_srdv|
;	NAME	_ll_srdv
;|_ll_srdv|

	; int64_t _ll_sdiv10(int64_t a)
	;  { return a / 10LL; }
	; a2,a3 contains int64_t remainder
	; Algorithm is the same as in _kernel_sdiv10 and __rt_sdiv10.
	EXPORT	|_ll_sdiv10|
	NAME	_ll_sdiv10
|_ll_sdiv10|
	STMFD	sp!, {v1, lr}
	MOVS	v1, a2			; Make a positive (copy sign in v1)
	BPL	_ll_sdiv10_pos
	RSBS	a1, a1, #0
	RSC	a2, a2, #0

_ll_sdiv10_pos
	SUBS	a3, a1, #10
	SBCS	a4, a2, #0		; a3,a4 = a1,a2 - 10

	MOV	lr, a1, LSR #2
	ORR	lr, lr, a2, LSL #32 - 2
	SUBS	a1, a1, lr
	SBC	a2, a2, a2, LSR #2	; a1,a2 = a * %0.1100

	MOV	lr, a1, LSR #4
	ORR	lr, lr, a2, LSL #32 - 4
	ADDS	a1, a1, lr
	ADC	a2, a2, a2, LSR #4	; a1,a2 = a * %0.11001100

	MOV	lr, a1, LSR #8
	ORR	lr, lr, a2, LSL #32 - 8
	ADDS	a1, a1, lr
	ADC	a2, a2, a2, LSR #8	; a1,a2 = a * %0.1100110011001100

	MOV	lr, a1, LSR #16
	ORR	lr, lr, a2, LSL #32 - 16
	ADDS	a1, a1, lr
	ADC	a2, a2, a2, LSR #16	; a1,a2 = a * %0.11001100110011001100110011001100

	ADDS	a1, a1, a2		; a1,a2 = a * %0.1100110011001100110011001100110011001100110011001100110011001100
	ADC	a2, a2, #0

	MOV	a1, a1, LSR #3
	ORR	a1, a1, a2, LSL #32 - 3
	MOV	a2, a2, LSR #3		; a1,a2 = a * %0.0001100110011001100110011001100110011001100110011001100110011001100

	MOV	ip, a2, LSL #2
	ORR	ip, ip, a1, LSR #32 - 2
	ADDS	lr, a1, a1, LSL #2
	ADC	ip, ip, a2
	ADDS	lr, lr, lr
	ADC	ip, ip, ip		; ip,lr = 10 * (a1,a2 / 10)

	SUBS	a3, a3, lr
	SBCS	a4, a4, ip		; a2,a3 = a1,a2 - 10 - 10 * (a1,a2 / 10)
	MOV	a4, #0
	ADDMI	a3, a3, #10		; Fix up remainder
	BMI	_ll_sdiv10_fixsign
	ADDS	a1, a1, #1		; Fix up quotient
	ADC	a2, a2, #0
	TST	v1, v1			; Need to fix sign ?
	stackreturn	PL, "v1, pc"
_ll_sdiv10_fixsign
	RSBS	a1, a1, #0		; Fix sign quotient
	RSC	a2, a2, #0
	RSBS	a3, a3, #0		; Fix sign remainder
	RSC	a4, a4, #0
	stackreturn	AL, "v1, pc"

	; int64_t _ll_not(int64_t a)
	;  { return ~a; }
	EXPORT	|_ll_not|
	NAME	_ll_not
|_ll_not|
	MVN	a1, a1
	MVN	a2, a2
	return	AL, pc, lr

	; int64_t _ll_neg(int64_t a)
	;  { return -a; }
	EXPORT	|_ll_neg|
	NAME	_ll_neg
|_ll_neg|
	RSBS	a1, a1, #0
	RSC	a2, a2, #0
	return	AL, pc, lr

	; int64_t _ll_and(int64_t a, int64_t b)
	;  { return a & b; }
	EXPORT	|_ll_and|
	NAME	_ll_and
|_ll_and|
	AND	a1, a1, a3
	AND	a2, a2, a4
	return	AL, pc, lr

	; int64_t _ll_or(int64_t a, int64_t b)
	;  { return a | b; }
	EXPORT	|_ll_or|
	NAME	_ll_or
|_ll_or|
	ORR	a1, a1, a3
	ORR	a2, a2, a4
	return	AL, pc, lr

	; int64_t _ll_eor(int64_t a, int64_t b)
	;  { return a ^ b; }
	EXPORT	|_ll_eor|
	NAME	_ll_eor
|_ll_eor|
	EOR	a1, a1, a3
	EOR	a2, a2, a4
	return	AL, pc, lr

	; int64_t _ll_shift_l(int64_t a, uint32_t b)
	;  { return a << b; }
	EXPORT	|_ll_shift_l|
	NAME	_ll_shift_l
|_ll_shift_l|
	SUBS	a4, a3, #32
	BPL	_ll_shift_l_more32bits
	RSB	a4, a3, #32
	MOV	a2, a2, LSL a3
	ORR	a2, a2, a1, LSR a4
	MOV	a1, a1, LSL a3
	return	AL, pc, lr
_ll_shift_l_more32bits
	MOV	a2, a1, LSL a4
	MOV	a1, #0
	return	AL, pc, lr

	; int64_t _ll_ushift_r(uint64_t a, uint32_t b)
	;  { return a >> b; }
	EXPORT	|_ll_ushift_r|
	NAME	_ll_ushift_r
|_ll_ushift_r|
	SUBS	a4, a3, #32
	BPL	_ll_ushift_r_more32bits
	RSB	a4, a3, #32
	MOV	a1, a1, LSR a3
	ORR	a1, a1, a2, LSL a4
	MOV	a2, a2, LSR a3
	return	AL, pc, lr
_ll_ushift_r_more32bits
	MOV	a1, a2, LSR a4
	MOV	a2, #0
	return	AL, pc, lr

	; int64_t _ll_sshift_r(int64_t a, uint32_t b)
	;  { return a >> b; }
	EXPORT	|_ll_sshift_r|
	NAME	_ll_sshift_r
|_ll_sshift_r|
	SUBS	a4, a3, #32
	BPL	_ll_sshift_r_more32bits
	RSB	a4, a3, #32
	MOV	a1, a1, LSR a3
	ORR	a1, a1, a2, LSL a4
	MOV	a2, a2, ASR a3
	return	AL, pc, lr
_ll_sshift_r_more32bits
	MOV	a1, a2, ASR a4
	MOV	a2, a2, ASR #32
	return	AL, pc, lr

	; uint64_t a, b
	; CMP a, b
	; Use CS,CC or HI,LS afterwards
	; Note that the V flag is not updated correctly.
	EXPORT	|_ll_cmpu|
	NAME	_ll_cmpu
|_ll_cmpu|
	CMP	a2, a4
	CMPEQ	a1, a3
	MOV	pc, lr	;Do *not* replace this with 'return' macro !

	; int64_t a, b
	; CMPGE a, b
	; Use GE,LT or GT,LE afterwards
	; Note that the Z flag is not updated correctly.
	EXPORT	|_ll_cmpge|
	NAME	_ll_cmpge
|_ll_cmpge|
	SUBS	a1, a1, a3
	SBCS	a2, a2, a4
	MOV	pc, lr	;Do *not* replace this with 'return' macro !

	; int64_t a, b
	; CMPLE a, b
	; Use GE,LT or GT,LE afterwards
	; Note that the Z flag is not updated correctly.
	EXPORT	|_ll_cmple|
	NAME	_ll_cmple
|_ll_cmple|
	SUBS	a1, a3, a1
	SBCS	a2, a4, a2
	MOV	pc, lr	;Do *not* replace this with 'return' macro !

	; double _ll_uto_d(uint64_t a)
	;  { return (double)a; }
	EXPORT	|_ll_uto_d|
	NAME	_ll_uto_d
|_ll_uto_d|
	MOV	a3, #(1023 + 33)<<20
	B	_ll_sto_d_preload_exponent

	; double _ll_sto_d(int64_t a)
	;  { return (double)a; }
	EXPORT	|_ll_sto_d|
	NAME	_ll_sto_d
|_ll_sto_d|
	ANDS	a3, a2, #1<<31		;Negative or positive ?
	BPL	_ll_sto_d_pos
	RSBS	a1, a1, #0		; - negative, make positive
	RSC	a2, a2, #0
_ll_sto_d_pos				;a3 = sign<<31
	ORR	a3, a3, #(1023 + 33)<<20
_ll_sto_d_preload_exponent
	SUB	a3, a3, #3<<20		;a3 = sign<<31 + (1023 + 30)<<23

	; Move a1,a2 to left until it has highest bit set and record how many
	; positions we moved to left.  E.g. :
	;  31                        0       31                      0
	; o-----------------------------o  o---------------------------o
	; | 0 0 ... 0 1 f51 f50 f49 f48 |  | f47 f46  ...  f18 f17 f16 |
	; o-----------------------------o  o---------------------------o
	;     a2 (high word)                      a1 (low word)
	MOVS	a4, a2
	MOVNE	a4, #32			;a4 = 32 - leading zero bits found so far (= 32)
	MOVEQS	a2, a1			;                                         (= 0)
	return	EQ, pc, lr		;a1,a2 = 0, so quit with 0 result

	MOVS	ip, a2, LSR #16		;Highest 16 bits set ?
	SUBEQ	a4, a4, #16		; - no, we have 16 more zero leading bits
	MOVEQ	a2, a2, LSL #16		;   and move a2 16 bits to the left

	TST	a2, #&FF<<24		;Highest 8 bits set ?
	SUBEQ	a4, a4, #8		; - no, we have 8 more zero leading bits
	MOVEQ	a2, a2, LSL #8		;   and move a2 8 bits to the left

	TST	a2, #&F<<28		;Highest 4 bits set ?
	SUBEQ	a4, a4, #4		; - no, we have 4 more zero leading bits
	MOVEQ	a2, a2, LSL #4		;   and move a2 4 bits to the left

	TST	a2, #3<<30		;Highest 2 bits set ?
	SUBEQ	a4, a4, #2		; - no, we have 2 more zero leading bits,
	MOVEQS	a2, a2, LSL #2		;   move a2 2 bits to the left and updates N as a2 & (1<<31)
					; - yes, note that N is also a2 & (1<<31)
					;Highest bit set (= look at N) ?
	MOVPL	a2, a2, LSL #1		; - no, we have 1 more zero leading bit
	SUBPL	a4, a4, #1		;   and move a2 1 bit to the left
					;Bit 31 of a2 is garanteed set
	ORR	ip, a2, a1, LSR a4	;ip = 32 most significant bits of a1,a2 on entry (bit 31 is garanteed set)

	ADD	a3, a3, a4, LSL #20	;a3 = sign<<31 + (1023 + # meaningful bits before dot - 2)<<20

	RSB	a4, a4, #32
	MOV	a4, a1, LSL a4		;a4 = 32 least significant bits of a1,a2 on entry

	MOV	a2, a4, LSR #11		;a2 = lowest 21 bits of the fraction
	ADCS	a2, a2, ip, LSL #21	;a2 = fraction_low
	ADC	a1, a3, ip, LSR #11	;a1 = sign<<31 + (1023 + # meaningful bits before dot - 1)<<20 + fraction_high
	return	AL, pc, lr

	; float _ll_uto_f(uint64_t a)
	;  { return (float)a; }
	EXPORT	|_ll_uto_f|
	NAME	_ll_uto_f
|_ll_uto_f|
	MOV	a3, #&7F<<23
	B	_ll_sto_f_preload_exponent

	; float _ll_sto_f(int64_t a)
	;  { return (float)a; }
	EXPORT	|_ll_sto_f|
	NAME	_ll_sto_f
|_ll_sto_f|
	ANDS	a3, a2, #1<<31		;Negative or positive ?
	BPL	_ll_sto_f_pos
	RSBS	a1, a1, #0		; - negative, make positive
	RSC	a2, a2, #0
_ll_sto_f_pos				;a3 = sign<<31
	;Note : exponent for 1.fraction = &7F + (# meaningful bits of a1,a2 before their dot - 1)
	ORR	a3, a3, #&7F<<23	;a3 = sign<<31 + &7F<<23
_ll_sto_f_preload_exponent
	ADD	a3, a3, #30<<23		;a3 = sign<<31 + (&7F + 30)<<23

	; Move a1,a2 to left until it has highest bit set and record how many
	; positions we moved to left.  E.g. :
	;  31                        0       31                      0
	; o-----------------------------o  o---------------------------o
	; | 0 0 ... 0 1 f22 f21 f20 f19 |  | f18 f17 ... f1 f0 x ... x |
	; o-----------------------------o  o---------------------------o
	;     a2 (high word)                      a1 (low word)
	MOVS	a4, a2
	MOVNE	a4, #32			;a4 = 32 - leading zero bits found so far (= 32)
	MOVEQS	a2, a1			;                                         (= 0)
	return	EQ, pc, lr		;a1,a2 = 0, so quit with 0 result

	MOVS	ip, a2, LSR#16		;Highest 16 bits set ?
	SUBEQ	a4, a4, #16		; - no, we have 16 more zero leading bits
	MOVEQ	a2, a2, LSL#16		;   and move a2 16 bits to the left

	TST	a2, #&FF<<24		;Highest 8 bits set ?
	SUBEQ	a4, a4, #8		; - no, we have 8 more zero leading bits
	MOVEQ	a2, a2, LSL#8		;   and move a2 8 bits to the left

	TST	a2, #&F<<28		;Highest 4 bits set ?
	SUBEQ	a4, a4, #4		; - no, we have 4 more zero leading bits
	MOVEQ	a2, a2, LSL#4		;   and move a2 4 bits to the left

	TST	a2, #3<<30		;Highest 2 bits set ?
	SUBEQ	a4, a4, #2		; - no, we have 2 more zero leading bits,
	MOVEQS  a2, a2, LSL #2		;   move a2 2 bits to the left and updates N as a2 & (1<<31)
					; - yes, note that N is also a2 & (1<<31)
					;Highest bit set (= look at N) ?
	MOVPL	a2, a2, LSL #1		; - no, we have 1 more zero leading bit
	SUBPL	a4, a4, #1		;   and move a2 1 bit to the left
					;Bit 31 of a2 is garanteed set
	ORR	a2, a2, a1, LSR a4	;a2 = 32 most significant bits of a1,a2 on entry (bit 31 is garanteed set)
	;  31                                 8 7     0
	; o---------------------------------------------o
	; | 1 f22 f21 f20 f19 f18 f17 ... f1 f0 x ... x |
	; o---------------------------------------------o
	;                     a2
	ADD	a3, a3, a4, LSL#23	;a3 = sign<<31 + (&7F + # meaningful bits before dot - 2)<<23
	ADD	a1, a3, a2, LSR#8	;a1 = sign<<31 + (&7F + # meaningful bits before dot - 1)<<23 + fraction
	return	AL, pc, lr

; *** FIXME ***
;	; uint64_t _ll_ufrom_d(double a)
;	;  { return (uint64_t)a; }
;	EXPORT	|_ll_ufrom_d|
;	NAME	_ll_ufrom_d
;|_ll_ufrom_d|

; *** FIXME ***
;	; int64_t _ll_sfrom_d(double a)
;	;  { return (int64_t)a; }
;	EXPORT	|_ll_sfrom_d|
;	NAME	_ll_sfrom_d
;|_ll_sfrom_d|

; *** FIXME ***
;	; uint64_t _ll_ufrom_f(float a)
;	;  { return (uint64_t)a; }
;	EXPORT	|_ll_ufrom_f|
;	NAME	_ll_ufrom_f
;|_ll_ufrom_f|

; *** FIXME ***
;	; int64_t _ll_sfrom_f(float a)
;	;  { return (int64_t)a; }
;	EXPORT	|_ll_sfrom_f|
;	NAME	_ll_sfrom_f
;|_ll_sfrom_f|

	END

