/* File converted from config/arm/lib1funcs.asm.  */

a1 RN 0
a2 RN 1
a3 RN 2
a4 RN 3
v1 RN 4
v2 RN 5
v3 RN 6
v4 RN 7
v5 RN 8
v6 RN 9
sl RN 10
fp RN 11
ip RN 12
sp RN 13
lr RN 14
pc RN 15
f0 FN 0
f1 FN 1
f2 FN 2
f3 FN 3
f4 FN 4
f5 FN 5
f6 FN 6
f7 FN 7
r0 RN 0
r1 RN 1
r2 RN 2
r3 RN 3
r4 RN 4
r5 RN 5
r6 RN 6
r7 RN 7
r8 RN 8
r9 RN 9
r10 RN 10
r11 RN 11
r12 RN 12
r13 RN 13
r14 RN 14

	AREA |C$$code|, CODE, READONLY

	
/* ------------------------------------------------------------------------ */
/*		Bodies of the division and modulo routines.		    */
/* ------------------------------------------------------------------------ */	
	MACRO
	ARM_DIV_BODY	$dividend, $divisor, $result, $curbit

#if __ARM_ARCH__ >= 5

	clz	$curbit, $divisor
	clz	$result, $dividend
	sub	$result, $curbit, $result
	mov	$curbit, #1
	mov	$divisor, $divisor, lsl $result
	mov	$curbit, $curbit, lsl $result
	mov	$result, #0
	
#else

	; Initially shift the divisor left 3 bits if possible,
	; set curbit accordingly.  This allows for curbit to be located
	; at the left end of each 4 bit nibbles in the division loop
	; to save one loop in most cases.
	tst	$divisor, #0xe0000000
	moveq	$divisor, $divisor, lsl #3
	moveq	$curbit, #8
	movne	$curbit, #1

	; Unless the divisor is very big, shift it up in multiples of
	; four bits, since this is the amount of unwinding in the main
	; division loop.  Continue shifting until the divisor is 
	; larger than the dividend.
02	cmp	$divisor, #0x10000000
	cmplo	$divisor, $dividend
	movlo	$divisor, $divisor, lsl #4
	movlo	$curbit, $curbit, lsl #4
	blo	%B02

	; For very big divisors, we must shift it a bit at a time, or
	; we will be in danger of overflowing.
03	cmp	$divisor, #0x80000000
	cmplo	$divisor, $dividend
	movlo	$divisor, $divisor, lsl #1
	movlo	$curbit, $curbit, lsl #1
	blo	%B03

	mov	$result, #0

#endif

	; Division loop
01	cmp	$dividend, $divisor
	subhs	$dividend, $dividend, $divisor
	orrhs	$result,   $result,   $curbit
	cmp	$dividend, $divisor,  lsr #1
	subhs	$dividend, $dividend, $divisor, lsr #1
	orrhs	$result,   $result,   $curbit,  lsr #1
	cmp	$dividend, $divisor,  lsr #2
	subhs	$dividend, $dividend, $divisor, lsr #2
	orrhs	$result,   $result,   $curbit,  lsr #2
	cmp	$dividend, $divisor,  lsr #3
	subhs	$dividend, $dividend, $divisor, lsr #3
	orrhs	$result,   $result,   $curbit,  lsr #3
	cmp	$dividend, #0			; Early termination?
	movnes	$curbit,   $curbit,  lsr #4	; No, any more bits to do?
	movne	$divisor,  $divisor, lsr #4
	bne	%B01
	MEND

/* ------------------------------------------------------------------------ */	
	MACRO
	ARM_DIV2_ORDER	$divisor, $order

#if __ARM_ARCH__ >= 5

	clz	$order, $divisor
	rsb	$order, $order, #31

#else

	cmp	$divisor, #(1 << 16)
	movhs	$divisor, $divisor, lsr #16
	movhs	$order, #16
	movlo	$order, #0

	cmp	$divisor, #(1 << 8)
	movhs	$divisor, $divisor, lsr #8
	addhs	$order, $order, #8

	cmp	$divisor, #(1 << 4)
	movhs	$divisor, $divisor, lsr #4
	addhs	$order, $order, #4

	cmp	$divisor, #(1 << 2)
	addhi	$order, $order, #3
	addls	$order, $order, $divisor, lsr #1

#endif
	MEND

/* ------------------------------------------------------------------------ */
	MACRO
	ARM_MOD_BODY	$dividend, $divisor, $order, $spare

#if __ARM_ARCH__ >= 5

	clz	$order, $divisor
	clz	$spare, $dividend
	sub	$order, $order, $spare
	mov	$divisor, $divisor, lsl $order
	
#else

	mov	$order, #0

	; Unless the divisor is very big, shift it up in multiples of
	; four bits, since this is the amount of unwinding in the main
	; division loop.  Continue shifting until the divisor is 
	; larger than the dividend.
08	cmp	$divisor, #0x10000000
	cmplo	$divisor, $dividend
	movlo	$divisor, $divisor, lsl #4
	addlo	$order, $order, #4
	blo	%B08

	; For very big divisors, we must shift it a bit at a time, or
	; we will be in danger of overflowing.
09	cmp	$divisor, #0x80000000
	cmplo	$divisor, $dividend
	movlo	$divisor, $divisor, lsl #1
	addlo	$order, $order, #1
	blo	%B09

#endif

	; Perform all needed substractions to keep only the reminder.
	; Do comparisons in batch of 4 first.
	subs	$order, $order, #3		; yes, 3 is intended here
	blt	%F02

01	cmp	$dividend, $divisor
	subhs	$dividend, $dividend, $divisor
	cmp	$dividend, $divisor,  lsr #1
	subhs	$dividend, $dividend, $divisor, lsr #1
	cmp	$dividend, $divisor,  lsr #2
	subhs	$dividend, $dividend, $divisor, lsr #2
	cmp	$dividend, $divisor,  lsr #3
	subhs	$dividend, $dividend, $divisor, lsr #3
	cmp	$dividend, #1
	mov	$divisor, $divisor, lsr #4
	subges	$order, $order, #4
	bge	%B01

	tst	$order, #3
	teqne	$dividend, #0
	beq	%F05

	; Either 1, 2 or 3 comparison/substractions are left.
02	cmn	$order, #2
	blt	%F04
	beq	%F03
	cmp	$dividend, $divisor
	subhs	$dividend, $dividend, $divisor
	mov	$divisor,  $divisor,  lsr #1
03	cmp	$dividend, $divisor
	subhs	$dividend, $dividend, $divisor
	mov	$divisor,  $divisor,  lsr #1
04	cmp	$dividend, $divisor
	subhs	$dividend, $dividend, $divisor
05
	MEND


/* ------------------------------------------------------------------------ */
/*		Start of the Real Functions				    */
/* ------------------------------------------------------------------------ */
#ifdef L_udivsi3

	EXPORT	|__udivsi3|
	IMPORT	|__div0|
|__udivsi3|	ROUT

	subs	r2, r1, #1
	moveq	pc, lr
	bcc	|__div0|
	cmp	r0, r1
	bls	%F11
	tst	r1, r2
	beq	%F12
	
	ARM_DIV_BODY r0, r1, r2, r3
	
	mov	r0, r2
	mov	pc, lr

11	moveq	r0, #1
	movne	r0, #0
	mov	pc, lr

12
	ARM_DIV2_ORDER r1, r2

	mov	r0, r0, lsr r2
	mov	pc, lr

#endif /* L_udivsi3 */
/* ------------------------------------------------------------------------ */
#ifdef L_umodsi3

	EXPORT	|__umodsi3|
	IMPORT	|__div0|
|__umodsi3|	ROUT
	subs	r2, r1, #1			; compare divisor with 1
	bcc	|__div0|
	cmpne	r0, r1				; compare dividend with divisor
	moveq   r0, #0
	tsthi	r1, r2				; see if divisor is power of 2
	andeq	r0, r0, r2
	movls	pc, lr

	ARM_MOD_BODY r0, r1, r2, r3
	
	mov	pc, lr

#endif /* L_umodsi3 */
/* ------------------------------------------------------------------------ */
#ifdef L_divsi3

	EXPORT	|__divsi3|
	IMPORT	|__div0|
|__divsi3|	ROUT
	
	cmp	r1, #0
	eor	ip, r0, r1			; save the sign of the result.
	beq	|__div0|
	rsbmi	r1, r1, #0			; loops below use unsigned.
	subs	r2, r1, #1			; division by 1 or -1 ?
	beq	%F10
	movs	r3, r0
	rsbmi	r3, r0, #0			; positive dividend value
	cmp	r3, r1
	bls	%F11
	tst	r1, r2				; divisor is power of 2 ?
	beq	%F12

	ARM_DIV_BODY r3, r1, r0, r2
	
	cmp	ip, #0
	rsbmi	r0, r0, #0
	mov	pc, lr

10	teq	ip, r0				; same sign ?
	rsbmi	r0, r0, #0
	mov	pc, lr

11	movlo	r0, #0
	moveq	r0, ip, asr #31
	orreq	r0, r0, #1
	mov	pc, lr

12
	ARM_DIV2_ORDER r1, r2

	cmp	ip, #0
	mov	r0, r3, lsr r2
	rsbmi	r0, r0, #0
	mov	pc, lr

#endif /* L_divsi3 */
/* ------------------------------------------------------------------------ */
#ifdef L_modsi3

	EXPORT	|__modsi3|
	IMPORT	|__div0|
|__modsi3|	ROUT
	cmp	r1, #0
	beq	|__div0|
	rsbmi	r1, r1, #0			; loops below use unsigned.
	movs	ip, r0				; preserve sign of dividend
	rsbmi	r0, r0, #0			; if negative make positive
	subs	r2, r1, #1			; compare divisor with 1
	cmpne	r0, r1				; compare dividend with divisor
	moveq	r0, #0
	tsthi	r1, r2				; see if divisor is power of 2
	andeq	r0, r0, r2
	bls	%F10

	ARM_MOD_BODY r0, r1, r2, r3

10	cmp	ip, #0
	rsbmi	r0, r0, #0
	mov	pc, lr

#endif /* L_modsi3 */

	END
	
