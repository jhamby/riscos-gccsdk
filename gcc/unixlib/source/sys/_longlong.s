;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_longlong.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

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
	MOV	pc, lr

	; int64_t _ll_from_l(int32_t a)
	;  { return (int64_t)a; }
	EXPORT	|_ll_from_l|
	NAME	_ll_from_l
|_ll_from_l|
	MOV 	a2, a1, ASR#32
	MOV	pc, lr

	; int64_t _ll_to_l(int64_t a)
	;  { return (int64_t)a; }
	EXPORT	|_ll_to_l|
	NAME	_ll_to_l
|_ll_to_l|
	MOV	pc, lr

	; int64_t _ll_add(int64_t a, int64_t b)
	;  { return a + b; }
	EXPORT	|_ll_add|
	NAME	_ll_add
|_ll_add|
	ADDS	a1, a1, a3
	ADC	a2, a2, a4
	MOV	pc, lr

	; int64_t _ll_addlu(int64_t a, uint32_t b)
	;  { return a + (uint64_t)b; }
	EXPORT	|_ll_addlu|
	NAME	_ll_addlu
|_ll_addlu|
	ADDS	a1, a1, a3
	ADC	a2, a2, #0
	MOV	pc, lr

	; int64_t _ll_addls(int64_t a, int32_t b)
	;  { return a + (int64_t)b; }
	EXPORT	|_ll_addls|
	NAME	_ll_addls
|_ll_addls|
	ADDS	a1, a1, a3
	ADC	a2, a2, a3, ASR#32
	MOV	pc, lr

	; int64_t _ll_adduu(uint32_t a, uint32_t b)
        ;  { return (int64_t)a + (int64_t)b; }
	EXPORT	|_ll_adduu|
	NAME	_ll_adduu
|_ll_adduu|
	MOV	a3, #0
	ADDS	a1, a1, a2
	ADC	a2, a3, #0
	MOV	pc, lr

	; int64_t _ll_addss(int32_t a, int32_t b)
        ;  { return (int64_t)a + (int64_t)b; }
	EXPORT	|_ll_addss|
	NAME	_ll_addss
|_ll_addss|
	MOV	a3, a1, ASR#32
	ADDS	a1, a1, a2
	ADC	a2, a3, a2, ASR#32
	MOV	pc, lr

	; int64_t _ll_sub(int64_t a, int64_t b)
	;  { return a - b; }
	EXPORT	|_ll_sub|
	NAME	_ll_sub
|_ll_sub|
	SUBS	a1, a1, a2
	SBC	a2, a2, a3
	MOV	pc, lr

	; int64_t _ll_sublu(int64_t a, uint32_t b)
	;  { return a - (int64_t)b; }
	EXPORT	|_ll_sublu|
	NAME	_ll_sublu
|_ll_sublu|
	SUBS	a1, a1, a2
	SBC	a2, a2, #0
	MOV	pc, lr

	; int64_t _ll_subls(int64_t a, int32_t b)
	;  { return a - (int64_t)b; }
	EXPORT	|_ll_subls|
	NAME	_ll_subls
|_ll_subls|
	SUBS	a1, a1, a3
	SBC	a2, a2, a3, ASR#32
	MOV	pc, lr

	; int64_t _ll_subuu(uint32_t a, uint32_t b)
	;  { return (int64_t)a - (int64_t)b; }
	EXPORT	|_ll_subuu|
	NAME	_ll_subuu
|_ll_subuu|
	SUBS	a1, a1, a2
	SBC	a2, a2, a2
	MOV	pc, lr

	; int64_t _ll_subss(int32_t a, int32_t b)
	;  { return (int64_t)a - (int64_t)b; }
	EXPORT	|_ll_subss|
	NAME	_ll_subss
|_ll_subss|
	MOV	a3, a1, ASR#32
	SUBS	a1, a1, a2
	SBC	a2, a3, a2, ASR#32
	MOV	pc, lr

	; int64_t _ll_rsb(int64_t a, int64_t b)
	;  { return b - a; }
	EXPORT	|_ll_rsb|
	NAME	_ll_rsb
|_ll_rsb|
	RSBS	a1, a1, a3
	RSC	a2, a2, a4
	MOV	pc, lr

	; int64_t _ll_rsblu(int64_t a, uint32_t b)
	;  { return (int64_t)b - a; }
	EXPORT	|_ll_rsblu|
	NAME	_ll_rsblu
|_ll_rsblu|
	RSBS	a1, a1, a3
	RSC	a2, a2, #0
	MOV	pc, lr

	; int64_t _ll_rsbls(int64_t a, int32_t b)
	;  { return (int64_t)b - a; }
	EXPORT	|_ll_rsbls|
	NAME	_ll_rsbls
|_ll_rsbls|
	RSBS	a1, a1, a3
	RSC	a2, a2, a3, ASR#32
	MOV	pc, lr

	; int64_t _ll_rsbuu(uint32_t a, uint32_t b)
	;  { return (int64_t)b - (int64_t)a; }
	EXPORT	|_ll_rsbuu|
	NAME	_ll_rsbuu
|_ll_rsbuu|
	RSBS	a1, a1, a2
	RSC	a2, a2, a2
	MOV	pc, lr

	; int64_t _ll_rsbss(int32_t a, int32_t b)
	;  { return (int64_t)b - (int64_t)a; }
	EXPORT	|_ll_rsbss|
	NAME	_ll_rsbss
|_ll_rsbss|
	MOV	a3, a1, ASR#32
	RSBS	a1, a1, a2
	RSB	a2, a3, a2, ASR#32
	MOV	pc, lr

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
	[ __UNIXLIB_MULL_SUPPORTED > 0
	MUL	ip, a2, a3		;ip = a2 * a3
	MLA	ip, a4, a1, ip		;ip = a2 * a3 + a4 * a1
	SMULL	a1, a2, a3, a1		;a2,a1 = a1 * a3
	ADD	a2, a2, ip		;a2,a1 = a1 * a3 + (a2 * a3 + a4 * a1) << 32
	MOV	pc, lr
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
	LDMFD	sp!, {v1-v3, pc}
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
	[ __UNIXLIB_MULL_SUPPORTED > 0
	MOV	ip, a2
	UMULL	a1, a2, a3, a1
	MLA	a2, ip, a3, a2
	MOV	pc, lr
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
	LDMFD	sp!, {v1-v2, pc}
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
	[ __UNIXLIB_MULL_SUPPORTED > 0
	MOV	a3, a1
	UMULL	a1, a2, a3, a2
	MOV	pc, lr
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
	LDMFD	sp!, {v1, pc}
	]

	; int64_t _ll_mulss(int32_t a, int32_t b)
	;  { return (int64_t)a * (int64_t)b; }
	EXPORT	|_ll_mulss|
	NAME	_ll_mulss
|_ll_mulss|
	[ __UNIXLIB_MULL_SUPPORTED > 0
	MOV	a3, a1
	SMULL	a1, a2, a3, a2
	MOV	pc, lr
	|
	MOV	a4, a2, ASR#32
	MOV	a3, a2
	MOV	a2, a1, ASR#32
	B	|_ll_mul|
	]

	; uint64_t _ll_udiv(uint64_t a, uint64_t b)
	;  { return a / b; }
	; Result in a1,a2; remainder in a3,a4.
	EXPORT	|_ll_udiv|
	IMPORT	|__udivdi3|
	NAME	_ll_udiv
|_ll_udiv|
	B	|__udivdi3|

	; uint64_t _ll_urdv(uint64_t a, uint64_t b)
	;  { return b / a; }
	; Result in a1,a2; remainder in a3,a4.
	EXPORT	|_ll_urdv|
	IMPORT	|__udivdi3|
	NAME	_ll_urdv
|_ll_urdv|
	STMFD	sp!, {lr}
	EOR	a3, a1, a3
	EOR	a4, a2, a4
	EOR	a1, a1, a3
	EOR	a2, a2, a4
	EOR	a3, a1, a3
	EOR	a4, a2, a4
	BL	|__udivdi3|
	LDMFD	sp!, {pc}

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
	LDMMIFD	sp!, {pc}
	ADDS	a1, a1, #1		; Fix up quotient
	ADC	a2, a2, #0
	LDMFD	sp!, {pc}

	; int64_t _ll_sdiv(int64_t a, int64_t b)
	;  { return a / b; }
	; Result in a1,a2; remainder in a3,a4.
	EXPORT	|_ll_sdiv|
	IMPORT	|__divdi3|
	NAME	_ll_sdiv
|_ll_sdiv|
	B	|__divdi3|

	; int64_t _ll_srdv(int64_t a, int64_t b)
	;  { return b / a; }
	; Result in a1,a2; remainder in a3,a4.
	EXPORT	|_ll_srdv|
	IMPORT	|__divdi3|
	NAME	_ll_srdv
|_ll_srdv|
	STMFD	sp!, {lr}
	EOR	a3, a1, a3
	EOR	a4, a2, a4
	EOR	a1, a1, a3
	EOR	a2, a2, a4
	EOR	a3, a1, a3
	EOR	a4, a2, a4
	BL	|__divdi3|
	LDMFD	sp!, {pc}

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
	LDMPLFD	sp!, {v1, pc}
_ll_sdiv10_fixsign
	RSBS	a1, a1, #0		; Fix sign quotient
	RSC	a2, a2, #0
	RSBS	a3, a3, #0		; Fix sign remainder
	RSC	a4, a4, #0
	LDMFD	sp!, {v1, pc}

	; int64_t _ll_not(int64_t a)
	;  { return ~a; }
	EXPORT	|_ll_not|
	NAME	_ll_not
|_ll_not|
	MVN	a1, a1
	MVN	a2, a2
	MOV	pc, lr

	; int64_t _ll_neg(int64_t a)
	;  { return -a; }
	EXPORT	|_ll_neg|
	NAME	_ll_neg
|_ll_neg|
	RSBS	a1, a1, #0
	RSC	a2, a2, #0
	MOV	pc, lr

	; int64_t _ll_and(int64_t a, int64_t b)
	;  { return a & b; }
	EXPORT	|_ll_and|
	NAME	_ll_and
|_ll_and|
	AND	a1, a1, a3
	AND	a2, a2, a4
	MOV	pc, lr

	; int64_t _ll_or(int64_t a, int64_t b)
	;  { return a | b; }
	EXPORT	|_ll_or|
	NAME	_ll_or
|_ll_or|
	ORR	a1, a1, a3
	ORR	a2, a2, a4
	MOV	pc, lr

	; int64_t _ll_eor(int64_t a, int64_t b)
	;  { return a ^ b; }
	EXPORT	|_ll_eor|
	NAME	_ll_eor
|_ll_eor|
	EOR	a1, a1, a3
	EOR	a2, a2, a4
	MOV	pc, lr

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
	MOV	pc, lr
_ll_shift_l_more32bits
	MOV	a2, a1, LSL a4
	MOV	a1, #0
	MOV	pc, lr

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
	MOV	pc, lr
_ll_ushift_r_more32bits
	MOV	a1, a2, LSR a4
	MOV	a2, #0
	MOV	pc, lr

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
	MOV	pc, lr
_ll_sshift_r_more32bits
	MOV	a1, a2, ASR a4
	MOV	a2, a2, ASR #32
	MOV	pc, lr

	; uint64_t a, b
	; CMP a, b
	; Use CS,CC or HI,LS afterwards
	; Note that the V flag is not updated correctly.
	EXPORT	|_ll_cmpu|
	NAME	_ll_cmpu
|_ll_cmpu|
	CMP	a2, a4
	CMPEQ	a1, a3
	MOV	pc, lr

	; int64_t a, b
	; CMPGE a, b
	; Use GE,LT or GT,LE afterwards
	; Note that the Z flag is not updated correctly.
	EXPORT	|_ll_cmpge|
	NAME	_ll_cmpge
|_ll_cmpge|
	SUBS	a1, a1, a3
	SBCS	a2, a2, a4
	MOV	pc, lr

	; int64_t a, b
	; CMPLE a, b
	; Use GE,LT or GT,LE afterwards
	; Note that the Z flag is not updated correctly.
	EXPORT	|_ll_cmple|
	NAME	_ll_cmple
|_ll_cmple|
	SUBS	a1, a3, a1
	SBCS	a2, a4, a2
	MOV	pc, lr

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
	MOVEQ	pc, lr			;a1,a2 = 0, so quit with 0 result

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
	MOV	pc, lr

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
	MOVEQ	pc, lr			;a1,a2 = 0, so quit with 0 result

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
	MOV	pc, lr

	; uint64_t _ll_ufrom_d(double a)
	;  { return (uint64_t)a; }
	EXPORT	|_ll_ufrom_d|
	NAME	_ll_ufrom_d
|_ll_ufrom_d|
	MOV	r2, r0, lsr #20		; r2 = exponent
	BIC	r0, r0, r2, lsl #20	; r0 = fraction

	MOVS	r3, r2, lsl #20		; Check for sign=1 or exponent=0
	MOVLE	r0, #0			; ...in which case return 0
	MOVLE	r1, #0
	MOVLE	pc, lr

	SUB	r2, r2, #1024		; Adjust exponent (really this should be 1023, but that'd take two instructions)
	CMP	r2, #62			; Maximum exponent we can allow is 63, since 2^64 would overflow
	MVNGT	r0, #0
	MVNGT	r1, #0			; unsigned +infinity
	MOVGT	pc, lr

	; Else exponent is in range
	SUBS	r2, r2, #19+32		; Adjust again so that the 1 bit will lie in bit 0 of r0
	ORR	r0, r0, #&100000	; Add the 1 bit
	BGE	_ll_ufrom_d_left	; Need to shift left

	; Else shift right
	RSB	r2, r2, #0
	MOV	r1, r1, lsr r2		; Shift lower word
	; Now add overlapping bits from upper word
	RSBS	r2, r2, #32
	ORRGE	r1, r1, r0, lsl r2
	RSBLT	r2, r2, #0
	ORRLT	r1, r1, r0, lsr r2
	RSBLT	r2, r2, #0
	RSB	r2, r2, #32
	MOV	r2, r0, lsr r2		; And shift upper word

	; return (completing word order swap started above)
	MOV	r0, r1
	MOV	r1, r2
	MOV	pc, lr

_ll_ufrom_d_left
	MOV	r0, r0, lsl r2		; Shift upper word
	; now add overlapping bits from lower word
	RSB	r2, r2, #32
	ORR	r0, r0, r1, lsr r2
	RSB	r2, r2, #32
	MOV	r2, r1, lsl r2		; shift lower word

	; return (completing word order swap started above)
	MOV	r1, r0
	MOV	r0, r2
	MOV	pc, lr

	; int64_t _ll_sfrom_d(double a)
	;  { return (int64_t)a; }
	EXPORT	|_ll_sfrom_d|
	NAME	_ll_sfrom_d
|_ll_sfrom_d|
	MOV	r3, r0, lsr #31		; r3 = sign bit
	MOV	r2, r0, lsr #20		; r2 = exponent
	BIC	r0, r0, r2, lsl #20	; r0 = fraction
	BICS	r2, r2, #&800		; Clear sign bit from exponent and check for exponent of 0
	MOVEQ	r0, #0			; If the exponent was 0, return 0
	MOVEQ	r1, #0			; (Handles cases where fraction == 0 and so number=0, or fraction != 0 and so number=0.fraction*2^-1022 (i.e. very small indeed))
	MOVEQ	pc, lr

	SUB	r2, r2, #1024		; Adjust exponent (really this should be 1023, but that'd take two instructions)
	CMP	r2, #61			; Maximum exponent we can allow is 62, since 2^63 would overflow into the sign slot and result in -1
	BGT	_ll_sfrom_d_big

	; Else exponent is in range
	SUBS	r2, r2, #19+32		; Adjust again so that the 1 bit will lie in bit 0 of r0
	ORR	r0, r0, #&100000	; Add the 1 bit
	BGE	_ll_sfrom_d_left	; Need to shift left

	; Else shift right
	RSB	r2, r2, #0
	MOV	r1, r1, lsr r2		; Shift lower word
	; Now add overlapping bits from upper word
	RSBS	r2, r2, #32
	ORRGE	r1, r1, r0, lsl r2
	RSBLT	r2, r2, #0
	ORRLT	r1, r1, r0, lsr r2
	RSBLT	r2, r2, #0
	RSB	r2, r2, #32
	MOV	r2, r0, lsr r2		; And shift upper word (And start a word swap using r2 as temp, since r1 and r0 are currently the wrong way round)

	; return
	CMP	r3, #0			; Negative ?
	MOVEQ	r0, r1			; Finish word swap
	MOVEQ	r1, r2
	MOVEQ	pc, lr
	RSBS	r0, r1, #0
	RSC	r1, r2, #0
	MOV	pc, lr

_ll_sfrom_d_left
	MOV	r0, r0, lsl r2		; Shift upper word
	; now add overlapping bits from lower word
	RSB	r2, r2, #32
	ORR	r0, r0, r1, lsr r2
	RSB	r2, r2, #32
	MOV	r2, r1, lsl r2		; shift lower word

	; return
	CMP	r3, #0			; Negative ?
	MOVEQ	r1, r0			; complete word order swap started above
	MOVEQ	r0, r2
	MOVEQ	pc, lr
	RSBS	r2, r2, #0
	RSC	r1, r0, #0
	MOV	r0, r2
	MOV	pc, lr

	; For big numbers, including infinity
_ll_sfrom_d_big
	CMP	r3, #0
	MVNEQ	r0, #0
	MVNEQ	r1, #1<<31		; signed +infinity
	MOVNE	r0, #0
	MOVNE	r1, #1<<31		; signed -infinity
	MOV	pc, lr

	; uint64_t _ll_ufrom_f(float a)
	;  { return (uint64_t)a; }
	EXPORT	|_ll_ufrom_f|
	NAME	_ll_ufrom_f
|_ll_ufrom_f|
	MOV	r2, r0, lsr #23		; r2 = exponent
	BIC	r0, r0, r2, lsl #23	; r0 = fraction
	MOVS	r1, r2, lsl #23		; Check for sign=1 or exponent=0
	MOVLE	r0, #0			; ...in which case return 0
	MOVLE	r1, #0
	MOVLE	pc, lr

	CMP	r2, #190		; Maximum exponent we can allow is 63, since 2^64 would overflow. Tracing that back results in an r2 of 63+127 = 190
	MVNGT	r0, #0
	MVNGT	r1, #0			; unsigned +infinity
	MOVGT	pc, lr

	; Else exponent >0 and <191, so treat as 1.fraction*2^(exponent-127)
	ORR	r1, r0, #&800000	; Add the implicit 1 to the fraction
	SUBS	r2, r2, #127+23		; Adjust exponent to give us hiw far we need to shift

	; Calculate the low word
	MOVGE	r0, r1, lsl r2
	RSBLT	r2, r2, #0
	MOVLT	r0, r1, lsr r2
	RSBLT	r2, r2, #0

	; Calculate the high word
	SUBS	r2, r2, #32
	MOVGE	r1, r1, lsl r2
	RSBLT	r2, r2, #0
	MOVLT	r1, r1, lsr r2

	MOV	pc, lr

	; int64_t _ll_sfrom_f(float a)
	;  { return (int64_t)a; }
	EXPORT	|_ll_sfrom_f|
	NAME	_ll_sfrom_f
|_ll_sfrom_f|
	MOV	r3, r0, lsr #31		; r3 = sign bit
	MOV	r2, r0, lsr #23		; r2 = exponent
	BIC	r0, r0, r2, lsl #23	; r0 = fraction
	ANDS	r2, r2, #255		; Clear sign bit from exponent, and check for an exponent of 0
	MOVEQ	r0, #0			; If the exponent was 0, return 0
	MOVEQ	r1, #0			; (Handles cases where fraction == 0 and so number=0, or fraction != 0 and so number=0.fraction*2^-126 (i.e. very small indeed))
	MOVEQ	pc, lr

	CMP	r2, #189		; Maximum exponent we can allow is 62, since 2^63 would overflow into the sign slot and result in -1. Tracing that back results in an r2 of 62+127 = 189
	BGT	_ll_sfrom_f_big

	; Else exponent >0 and <190, so treat as 1.fraction*2^(exponent-127)
	ORR	r1, r0, #&800000	; Add the implicit 1 to the fraction
	SUBS	r2, r2, #127+23		; Adjust exponent to give us hiw far we need to shift

	; Calculate the low word
	MOVGE	r0, r1, lsl r2
	RSBLT	r2, r2, #0
	MOVLT	r0, r1, lsr r2
	RSBLT	r2, r2, #0

	; Calculate the high word
	SUBS	r2, r2, #32
	MOVGE	r1, r1, lsl r2
	RSBLT	r2, r2, #0
	MOVLT	r1, r1, lsr r2

	; Check sign
	CMP	r3, #0
	MOVEQ	pc, r14			; Return if positive
	RSBS	r0, r0, #0		; Or negate
	RSC	r1, r1, #0
	MOV	pc, lr

	; For big numbers, including infinity
_ll_sfrom_f_big
	CMP	r3, #0
	MVNEQ	r0, #0
	MVNEQ	r1, #1<<31		; signed +infinity
	MOVNE	r0, #0
	MOVNE	r1, #1<<31		; signed -infinity
	MOV	pc, lr

	END
