r0	RN	0
r1	RN	1
r2	RN	2
r3	RN	3
r4	RN	4
r12	RN	12
ip	RN	12
sp	RN	13
r14	RN	14
lr	RN	14
pc	RN	15
f0	FN	0

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fixsfdi
	; Convert single precision float to 64-bit int
fixsfdi
	mov	r3, r0, lsr #31 ; r3=sign bit
	mov	r2, r0, lsr #23 ; r2=exponent
	bic	r0, r0, r2, lsl #23 ; r0=fraction
	ands	r2, r2, #255 ; Clear sign bit from exponent, and check for an exponent of 0

	moveq	r0, #0 ; If the exponent was 0, return 0
	moveq	r1, #0 ; (Handles cases where fraction == 0 and so number=0, or fraction != 0 and so number=0.fraction*2^-126 (i.e. very small indeed))
	moveq	pc, lr

	cmp	r2, #189 ; Maximum exponent we can allow is 62, since 2^63 would overflow into the sign slot and result in -1. Tracing that back results in an r2 of 62+127 = 189
	bgt	fixsfdi_big

	; Else exponent >0 and <190, so treat as 1.fraction*2^(exponent-127)
	orr	r1, r0, #&800000 ; Add the implicit 1 to the fraction
	subs	r2, r2, #127+23 ; Adjust exponent to give us hiw far we need to shift

	; Calculate the low word
	movge	r0, r1, lsl r2
	rsblt	r2, r2, #0
	movlt	r0, r1, lsr r2
	rsblt	r2, r2, #0

	; Calculate the high word
	subs	r2, r2, #32
	movge	r1, r1, lsl r2
	rsblt	r2, r2, #0
	movlt	r1, r1, lsr r2

	; Check sign
	cmp	r3, #0
	moveq	pc, lr ; Return if positive
	rsbs	r0, r0, #0 ; Or negate
	rsc	r1, r1, #0
	mov	pc, lr

fixsfdi_big ; For big numbers, including infinity
	cmp	r3, #0
	mvneq	r0, #0
	mvneq	r1, #&80000000 ; +infinity
	movne	r0, #0
	movne	r1, #&80000000 ; -infinity
	mov	pc, lr


	EXPORT	fixunssfdi
	; Convert single precision float to unsigned 64-bit int
fixunssfdi
	mov	r2, r0, lsr #23 ; r2=exponent
	bic	r0, r0, r2, lsl #23 ; r0=fraction

	movs	r1, r2, lsl #23 ; Check for sign=1 or exponent=0 
	movle	r0, #0 ; ...in which case return 0
	movle	r1, #0
	movle	pc, lr

	cmp	r2, #190 ; Maximum exponent we can allow is 63, since 2^64 would overflow. Tracing that back results in an r2 of 63+127 = 190
	mvngt	r0, #0
	mvngt	r1, #0 ; +infinity
	movgt	pc, lr

	; Else exponent >0 and <191, so treat as 1.fraction*2^(exponent-127)
	orr	r1, r0, #&800000 ; Add the implicit 1 to the fraction
	subs	r2, r2, #127+23 ; Adjust exponent to give us hiw far we need to shift

	; Calculate the low word
	movge	r0, r1, lsl r2
	rsblt	r2, r2, #0
	movlt	r0, r1, lsr r2
	rsblt	r2, r2, #0

	; Calculate the high word
	subs	r2, r2, #32
	movge	r1, r1, lsl r2
	rsblt	r2, r2, #0
	movlt	r1, r1, lsr r2

	mov	pc, lr

	EXPORT	fixdfdi
	; Convert double precision float to 64-bit int
fixdfdi
	mov	r3, r0, lsr #31 ; r3=sign bit
	mov	r2, r0, lsr #20 ; r2=exponent
	bic	r0, r0, r2, lsl #20 ; r0=fraction
	bics	r2, r2, #&800 ; Clear sign bit from exponent and check for exponent of 0

	moveq	r0, #0 ; If the exponent was 0, return 0
	moveq	r1, #0 ; (Handles cases where fraction == 0 and so number=0, or fraction != 0 and so number=0.fraction*2^-1022 (i.e. very small indeed))
	moveq	pc, lr

	sub	r2, r2, #1024 ; Adjust exponent (really this should be 1023, but that'd take two instructions)
	cmp	r2, #61 ; Maximum exponent we can allow is 62, since 2^63 would overflow into the sign slot and result in -1
	bgt	fixdfdi_big

	; Else exponent is in range
	subs	r2, r2, #19+32 ; Adjust again so that the 1 bit will lie in bit 0 of r0
	orr	r0, r0, #&100000 ; Add the 1 bit
	bge	fixdfdi_left ; Need to shift left

	; Else shift right
	rsb	r2, r2, #0
	mov	r1, r1, lsr r2 ; Shift lower word
	; Now add overlapping bits from upper word
	rsbs	r2, r2, #32
	orrge	r1, r1, r0, lsl r2
	rsblt	r2, r2, #0
	orrlt	r1, r1, r0, lsr r2
	rsblt	r2, r2, #0
	rsb	r2, r2, #32
	mov	r2, r0, lsr r2 ; And shift upper word (And start a word swap using r2 as temp, since r1 and r0 are currently the wrong way round)

	; return
	cmp	r3, #0 ; Negative?
	moveq	r0, r1 ; Finish word swap
	moveq	r1, r2
	moveq	pc, lr
	rsbs	r0, r1, #0
	rsc	r1, r2, #0
	mov	pc, lr

fixdfdi_left
	mov	r0, r0, lsl r2 ; Shift upper word
	; now add overlapping bits from lower word
	rsb	r2, r2, #32
	orr	r0, r0, r1, lsr r2
	rsb	r2, r2, #32
	mov	r2, r1, lsl r2 ; shift lower word

	; return
	cmp	r3, #0 ; Negative?
	moveq	r1, r0 ; complete word order swap started above
	moveq	r0, r2
	moveq	pc, lr
	rsbs	r2, r2, #0
	rsc	r1, r0, #0
	mov	r0, r2
	mov	pc, lr

fixdfdi_big ; For big numbers, including infinity
	cmp	r3, #0
	mvneq	r0, #0
	mvneq	r1, #&80000000 ; +infinity
	movne	r0, #0
	movne	r1, #&80000000 ; -infinity
	mov	pc, lr



	EXPORT	fixunsdfdi
	; Convert double precision float to unsigned 64-bit int
fixunsdfdi
	mov	r2, r0, lsr #20 ; r2=exponent
	bic	r0, r0, r2, lsl #20 ; r0=fraction

	movs	r3, r2, lsl #20 ; Check for sign=1 or exponent=0
	movle	r0, #0 ; ...in which case return 0
	movle	r1, #0
	movle	pc, lr

	sub	r2, r2, #1024 ; Adjust exponent (really this should be 1023, but that'd take two instructions)
	cmp	r2, #62 ; Maximum exponent we can allow is 63, since 2^64 would overflow
	mvngt	r0, #0
	mvngt	r1, #0 ; +infinity
	movgt	pc, lr

	; Else exponent is in range
	subs	r2, r2, #19+32 ; Adjust again so that the 1 bit will lie in bit 0 of r0
	orr	r0, r0, #&100000 ; Add the 1 bit
	bge	fixunsdfdi_left ; Need to shift left

	; Else shift right
	rsb	r2, r2, #0
	mov	r1, r1, lsr r2 ; Shift lower word
	; Now add overlapping bits from upper word
	rsbs	r2, r2, #32
	orrge	r1, r1, r0, lsl r2
	rsblt	r2, r2, #0
	orrlt	r1, r1, r0, lsr r2
	rsblt	r2, r2, #0
	rsb	r2, r2, #32
	mov	r2, r0, lsr r2 ; And shift upper word

	; return (completing word order swap started above)
	mov	r0, r1
	mov	r1, r2
	mov	pc, lr

fixunsdfdi_left
	mov	r0, r0, lsl r2 ; Shift upper word
	; now add overlapping bits from lower word
	rsb	r2, r2, #32
	orr	r0, r0, r1, lsr r2
	rsb	r2, r2, #32
	mov	r2, r1, lsl r2 ; shift lower word

	; return (completing word order swap started above)
	mov	r1, r0
	mov	r0, r2
	mov	pc, lr

	; Convert 32-bit integer to 32-bit float
	EXPORT	_fltsisf
_fltsisf
	cmp	r0, #0
	mvfeqs	f0, #0.0
	moveqs	pc, lr
	movgt	r3, r0
	movgt	r0, #0
	rsblt	r3, r0, #0 ; negate fraction
	movlt	r0, #1<<31

	mov	r2, #32 ; exponent

	; Do a little loop unrolling
	cmp	r3, #1<<8
	movle	r3, r3, lsl #23
	suble	r2, r2, #23
	cmp	r3, #1<<16
	movle	r3, r3, lsl #15
	suble	r2, r2, #15
	cmp	r3, #1<<24
	movle	r3, r3, lsl #7
	suble	r2, r2, #7
fltsisf_norm
	tst	r3, #1<<31
	moveq	r3, r3, lsl #1
	subeq	r2, r2, #1
	beq	fltsisf_norm

	add	r2, r2, #126 ; add bias to exponent

	; pack into IEEE single precision format
	orr	r0, r0, r2, lsl #23  ; exponent
	bic	r3, r3, #1<<31
	orr	r0, r0, r3, lsr #8
	str	r0, [sp, #-4]
	ldfs	f0, [sp, #-4]
	mov	pc, lr

	; Convert 64-bit integer to 64-bit float
	EXPORT	_fltdidf
	; Convert 64-bit integer to 32-bit float
	EXPORT	_fltdisf
_fltdidf
_fltdisf
	; Check fast case, incoming integer == 0
	cmp	r0, #0
	cmpeq	r1, #0
	mvfeqd	f0, #0.0
	moveq	pc, lr

	movs	r3, r1		; check sign and copy top word
	movge	r2, r0		; bottom word
	movge	r0, #0		; first word of IEEE result
	bge	fltdidf_pos
	rsbs	r2, r0, #0	; make positive if negative
	rsc	r3, r1, #0
	mov	r0, #1<<31	; first word of IEEE result (with sign bit)
fltdidf_pos

	cmp	r3, #0 ; if top word if 0 then we'll use the bottom word
	movne	r12, #64 ; exponent
	moveq	r12, #32
	moveq	r3, r2
	moveq	r2, #0

	; Do a little loop unrolling
	cmp	r3, #1<<8
	movle	r3, r3, lsl #23
	orrle	r3, r3, r2, lsr #32-23
	movle	r2, r2, lsl #23
	suble	r12, r12, #23
	cmp	r3, #1<<16
	movle	r3, r3, lsl #15
	orrle	r3, r3, r2, lsr #32-15
	movle	r2, r2, lsl #15
	suble	r12, r12, #15
	cmp	r3, #1<<24
	movle	r3, r3, lsl #7
	orrle	r3, r3, r2, lsr #32-7
	movle	r2, r2, lsl #7
	suble	r12, r12, #7
fltdidf_norm
	tst	r3, #1<<31
	bne	fltdidf_norm_fin
	mov	r3, r3, lsl #1
	movs	r2, r2, lsl #1
	orrcs	r3, r3, #1
	sub	r12, r12, #1
	b	fltdidf_norm

fltdidf_norm_fin
	add	r12, r12, #1020 ; add bias to exponent
	add	r12, r12, #2

	; pack into IEEE double precision format
	orr	r0, r0, r12, lsl #20  ; exponent
	bic	r3, r3, #1<<31
	orr	r0, r0, r3, lsr #11
	mov	r1, r2, lsr #11
	orr	r1, r1, r3, lsl #21
	str	r0, [sp, #-8]
	str	r1, [sp, #-4]
	ldfd	f0, [sp, #-8]
	mov	pc, lr
