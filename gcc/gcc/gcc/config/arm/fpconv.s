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
	cmp	r0, #0
	moveq	r1, #0
	moveq	pc, r14

	mov	r2, r0, lsr #23 ; Extract the exponent
	bic	r0, r0, r2, lsl #23 ; Clear exponent from mantissa
	orr	r0, r0, #1<<23 ; add implicit bit

	tst	r2, #1<<8  ; Is sign bit set
	rsbne	r0, r0, #0 ; yes; negate mantissa

	bic	r2, r2, #1<<8 ; clear sign bit, leaving exponent

	; number is represented as 1.mantissa x 2^exponent-127
	subs	r2, r2, #127 ; remove bias from exponent
	movmi	r0, #0 ; if exponent negative then not representable
	movmi	r1, #0
	movmi	pc, r14 ; as an integer

	; the mantissa is 23 bits wide. assume we are representing the
	; number as mantissa x 2^23, then we may need to shift left
	; or right.
	rsbs	r2, r2, #23
	movpl	r0, r0, asr r2
	movpl	r1, #0
	movpl	pc, r14

	rsb	r2, r2, #0 ; exponent was larger than 23

	; perform 64-bit shift
        rsbs	ip, r2, #32
	rsble	ip, ip, #0 ; shift >= 32 bits
	movle	r1, r0, asl ip
	movle	r0, #0
	movgt	r1, r0, lsr ip
	movgt	r0, r0, asl r2
	mov	pc, r14


	EXPORT	fixunssfdi
	; Convert single precision float to 64-bit int
fixunssfdi
	cmp	r0, #0
	moveq	r1, #0
	moveq	pc, r14

	mov	r2, r0, lsr #23 ; Extract the exponent
	bic	r0, r0, r2, lsl #23 ; Clear exponent from mantissa
	orr	r0, r0, #1<<23 ; add implicit bit
	bic	r2, r2, #1<<8 ; clear sign bit, leaving exponent

	; number is represented as 1.mantissa x 2^exponent-127
	subs	r2, r2, #127 ; remove bias from exponent
	movmi	r0, #0 ; if exponent negative then not representable
	movmi	r1, #0
	movmi	pc, r14 ; as an integer

	; the mantissa is 23 bits wide. assume we are representing the
	; number as mantissa x 2^23, then we may need to shift left
	; or right.
	rsbs	r2, r2, #23
	movpl	r0, r0, lsr r2
	movpl	r1, #0
	movpl	pc, r14

	rsb	r2, r2, #0 ; exponent was larger than 23

	; perform 64-bit shift
        rsbs	ip, r2, #32
	rsble	ip, ip, #0 ; shift >= 32 bits
	movle	r1, r0, asl ip
	movle	r0, #0
	movgt	r1, r0, lsr ip
	movgt	r0, r0, asl r2
	mov	pc, r14

	EXPORT	fixdfdi
fixdfdi
	stmfd	sp!, {r14}
	mov	r2, r0, lsr #31
	tst	r2, #1
	bl	fixunsdfdi
	ldmeqfd	sp!, {pc}
	rsbs	r0, r0, #0
	rsc	r1, r1, #0
	ldmfd	sp!, {pc}

	EXPORT	fixunsdfdi
	; Convert double precision float to 64-bit int
fixunsdfdi
	; Check fast case, incoming integer == 0
	cmp	r0, #0
	cmpeq	r1, #0
	moveq	pc, r14

	; r0 = first word
	; r1 = second word

	mov	r3, r0, lsr #20 ; Extract the exponent
	bic	r2, r0, r3, lsl #20 ; clear exponent from fraction
	orr	r2, r2, #1<<20 ; add implicit bit
	bic	r12, r3, #1<<11 ; clear sign bit, leaving exponent

	; number is represented as 1.mantissa x 2^exponent-1023
	sub	r12, r12, #1024 ; remove bias from exponent
	adds	r12, r12, #1
	movmi	r0, #0 ; if exponent negative then not representable
	movmi	r1, #0 ; as an integer
	movmi	pc, lr

	; at this point: r0 = exponent
	; r1 = second word (bottom 32-bits of fraction)
	; r2 = first word (top 20-bits of fraction)

	rsbs	r12, r12, #52
	bmi	fixunsdfdi_shl

	rsbs	r3, r12, #32  ; r3 = 32 - shift
	rsble	r3, r3, #0
	movle	r0, r2, lsr r3 ; low = high >> -bm
	movle	r1, #0	 	; high = 0
	movgt	r0, r1, lsr r12  ; low >>= exponent
	orrgt	r0, r0, r2, lsl r3 ; low |= high << r3
	movgt	r1, r2, lsr r12
	mov	pc, lr

fixunsdfdi_shl
	rsb	r12, r12, #0 ; exponent was larger than 52

	; perform 64-bit shift
        rsbs	r3, r12, #32
	rsble	r3, r3, #0 ; shift >= 32 bits
	movle	r1, r1, asl r3
	movle	r0, #0
	movgt	r2, r2, lsl r12 ; shift <= 31 bits
	movgt	r0, r1, asl r12
	orrgt	r1, r2, r1, lsr r3
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

	tst	r1, #1<<31
	moveq	r2, r0	; bottom word
	moveq	r3, r1	; top word
	moveq	r0, #0  ; first word of IEEE result
	rsbne	r2, r0, #0 ; negate fraction
	rsbne	r3, r1, #0
	movne	r0, #1<<31  ; first word of IEEE result (with sign bit)

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
