/* libgcc1 and libgcc2 routines for ARM cpu (AOF format)
 Copyright (C) 1998, 2000 Free Software Foundation, Inc.
 Contributed by Nick Burrett (nick.burrett@btinternet.com)
 and Peter Burwood (pjb@arcangel.dircon.co.uk)

 This file is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 In addition to the permissions in the GNU General Public License, the
 Free Software Foundation gives you unlimited permission to link the
 compiled version of this file with other programs, and to distribute
 those programs without any restriction coming from the use of this
 file.  (The General Public License restrictions do apply in other
 respects; for example, they cover modification of the file, and
 distribution when not linked into another program.)

 This file is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to
 the Free Software Foundation, 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.

 As a special exception, if you link this library with other files,
 some of which are compiled with GCC, to produce an executable,
 this library does not by itself cause the resulting executable
 to be covered by the GNU General Public License.
 This exception does not however invalidate any other reasons why
 the executable file might be covered by the GNU General Public License.  */

/* Run this through cpp */

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
r12 RN 12
r14 RN 14

#ifdef __APCS_26__
#define RETURN(dst,src) movs dst, src
#define RETURNc(x,dst,src) mov##x##s dst, src
#define RETCOND ^
#else
#define RETURN(dst,src) mov dst, src
#define RETURNc(x,dst,src) mov##x dst, src
#define RETCOND
#endif

	AREA	|C$$code|, CODE, READONLY

#ifdef L_ashldi3
	EXPORT  |__ashldi3|

/* DItype __ashldi3 (DItype u, word_type b)
   a1, a2 = u
   a3 = b  */
|__ashldi3|
	/* if shift = 0 then return */
	CMP	a3, #0
	RETURNc(eq, pc, lr)

	/* if shift >= 32 then low word will be 0 */
	RSBS	ip, a3, #32		/* bm = 32 - b */
	RSBLE	ip, ip, #0
	MOVLE	a2, a1, ASL ip		/* high = low << -bm */
	MOVLE	a1, #0			/* low = 0 */

	MOVGT	a2, a2, LSL a3		/* high = high << b */
	ORRGT	a2, a2, a1, LSR ip	/* high = high | low >> bm */
	MOVGT	a1, a1, ASL a3		/* low = low << b */

	RETURN(pc, lr)
#endif

#ifdef L_ashrdi3
	EXPORT	|__ashrdi3|
/* DItype __ashrdi3 (DItype u, word_type b)
   a1, a2 = u
   a3 = b  */
|__ashrdi3|
	/* if shift = 0 then return */
	CMP	a3, #0
	RETURNc(eq, pc, lr)

	/* if shift >= 32 then high word will be 1..1 or 0..0 */
	RSBS	ip, a3, #32		/* bm = 32 - b */
	RSBLE	ip, ip, #0
	MOVLE	a1, a2, ASR ip		/* low = high >> - bm */
	MOVLE	a2, a2, ASR #31		/* high = high >> 31 */

	MOVGT	a1, a1, LSR a3		/* low = low >> b */
	ORRGT	a1, a1, a2, ASL ip	/* low = low | high << bm */
	MOVGT	a2, a2, ASR a3		/* high = high >> b */

	RETURN(pc, lr)
#endif

#ifdef L_clear_icache
	EXPORT	|__clear_icache|
	/* clear instruction cache

	  On entry
	     R0 = low address of range (word aligned)
	     R1 = high address (word aligned, _inclusive_) */

X_Bit				EQU	&020000
XOS_SynchroniseCodeAreas	EQU	&00006E + X_Bit

|__clear_icache|
	MOV	a3, a2
	MOV	a2, a1
	MOV	a1, #1
	SWI	XOS_SynchroniseCodeAreas
	RETURN(pc, lr)
#endif

#ifdef L_cmpdi2
	EXPORT |__cmpdi2|
	/* compare two long long ints
	   enter with __cmpdi2 (DItype a, DItype b)
	   entry a1,a2 = u, a3,a4 = v
	   exit u > v = 2, u < v = 0, u = v = 1  */
|__cmpdi2|
	CMP    a2, a4
	CMPEQ  a1, a3
	MOVLT  a1, #0
	MOVGT  a1, #2
	MOVEQ  a1, #1
	RETURN(pc, lr)
#endif

#ifdef L_divdi3
	EXPORT |__divdi3|
	IMPORT |__div0|

	/* (a1, a2) / (a3, a4)
	   result in v2 and v3
	   remainder in v4 and ip  */
|__divdi3|
	/* fast exits
	   check the numerator for zero  */
	CMP	a1, #0	/* low */
	CMPEQ	a2, #0	/* high */
	RETURNc(eq, pc, lr)
	/* test for a divide by zero */
	CMP	a3, #0	/* low */
	CMPEQ	a4, #0	/* high */
	BEQ	|__div0|	/* divide by zero.  */
	/* No stack checking since we are a leaf function.
	   We don't need to store ip - procedure calling standard
	   lets it be corrupted.  */
	STMFD	sp!, {v2, v3, v4, v6, lr}
	/* v6 = sign bit */
	ANDS	v6, a4, #&80000000
	BPL	|__divdi3.L00|
	RSBS	a3, a3, #0 /* low */
	RSC	a4, a4, #0 /* high */
|__divdi3.L00|
	EORS	v6, v6, a2, ASR#32
	BCC	|__divdi3.L01|
	RSBS	a1, a1, #0 /* low */
	RSC	a2, a2, #0 /* high */
|__divdi3.L01|
	/* initialise the quotient to zero */
	MOV	v2, #0 /* low */
	MOV	v3, #0 /* high */
	/* remainder = numerator */
	MOV	v4, a1 /* low */
	MOV	ip, a2 /* high */

	/* lr is a count of the number of shifts needed */
        MOVS    lr, #0
	/* shift the denominator left until the next doubling
	  generates a number larger than the numerator.
	  Count the number of shifts needed */
|__divdi3.bit_count|
	/* double the denominator, we've finished when the top bit
	   is set. */
	ADDS	a3, a3, a3 /* low */
	ADCS	a4, a4, a4 /* high */
	/* too big so we've finished */
	BCS	|__divdi3.count_done|
	CMP	a4, ip /* high */
	CMPEQ	a3, v4 /* low */
	/* if denominator is lower or the same as the numerator
	   signify an extra shift and carry on. */
	ADDLS	lr, lr, #1
	BLS	|__divdi3.bit_count|

	/* clear the carry flag */
	ADDS	lr, lr, #0

|__divdi3.count_done|
	/* we've overflowed. shift the denominator back down
	   to bring in the overflowed bit */
	MOVS	a4, a4, RRX /* high */
	MOV	a3, a3, RRX /* low */

|__divdi3.division|
	/* numerator = remainder - denominator */
	SUBS	a1, v4, a3 /* low */
	SBCS	a2, ip, a4 /* high */
	/* if (remainder >= denominator) remainder = numerator */
	MOVCS	v4, a1 /* low */
	MOVCS	ip, a2 /* high */
	/* place next bit into the result */
	ADCS	v2, v2, v2 /* low */
	ADC	v3, v3, v3 /* high */
	/* shift the denominator right 1 bit */
	MOVS	a4, a4, LSR #1 /* high */
	MOV	a3, a3, RRX /* low */
	SUBS	lr, lr, #1
	BGE	|__divdi3.division|
	/* result */
	MOVS	v6, v6, LSL#2
	MOVCC	a1, v2
	MOVCC	a2, v3
	LDMCCFD	sp!, {v2, v3, v4, v6, pc}RETCOND
	RSBS	a1, v2, #0 /* low */
	RSC	a2, v3, #0 /* high */
	LDMFD	sp!, {v2, v3, v4, v6, pc}RETCOND
#endif

#ifdef L_divsi3
	EXPORT |__divsi3|
	EXPORT |__divmodsi4|
	IMPORT |__div0|

|__divsi3|
|__divmodsi4|
	MOVS	a4, a2
	BEQ	|__div0|	/* divide by zero.  */
	AND	a3, a1, #&80000000
	EOR	ip, a4, a1
	ORR	a3, a3, ip, LSR #1
	RSBMI	a4, a4, #0
	MOVS	a2, a1
	RSBMI	a2, a2, #0
	MOV	ip, a4

	CMP	ip, a2, LSR #16
	MOVLS	ip, ip, LSL #16
	CMP	ip, a2, LSR #8
	MOVLS	ip, ip, LSL #8
	CMP	ip, a2, LSR #4
	MOVLS	ip, ip, LSL #4
	CMP	ip, a2, LSR #2
	MOVLS	ip, ip, LSL #2
	CMP	ip, a2, LSR #1
	MOVLS	ip, ip, LSL #1

	MOV	a1, #0
|__divsi3.divloop|
	CMP	a2, ip
	SUBCS	a2, a2, ip
	ADC	a1, a1, a1
	MOV	ip, ip, LSR #1
	CMP	ip, a4
	BCS	|__divsi3.divloop|
	TST	a3,  #&40000000
	RSBNE	a1, a1, #0
	/* This line is for __divmodsi4 only */
	TST	a3,  #&80000000
	RSBNE	a2, a2, #0
	RETURN(pc, lr)
#endif

#ifdef L_ffs
	EXPORT	|ffs|

	/* return index to least significant 1-bit of a1 */
|ffs|
	/* input and output in a1, workspace a2 */
	TEQ	a1, #0			/* test equality with 0 */
	SUBNE	a2, a1, #1		/* subtract if not equal */
	EORNE	a1, a1, a2		/* exclusive or if not equal */
	RSBNE	a1, a1, a1, LSL #3	/* reverse subtract if not equal */
	RSBNE	a1, a1, a1, LSL #8
	RSBNE	a1, a1, a1, LSL #8
	RSBNE	a1, a1, a1, LSL #8
	/* load register, if not equal, with byte */
	LDRNEB	a1, [pc, a1, LSR #26]
	RETURN(pc, lr)

|ffs.table|
	/* table must occur 8 bytes after LDRxxB instruction above */
	DCB	0,1,0,16,0,2,29,0,17,0,0,0,3,22,30,0
	DCB	0,0,20,18,11,0,13,0,0,4,0,7,0,23,31,0
	DCB	15,0,28,0,0,0,21,0,19,10,12,0,6,0,0,14
	DCB	27,0,0,9,0,5,0,26,0,8,25,0,24,0,32,0

	EXPORT	|__ffsdi2|
|__ffsdi2|
	CMP	a1, #0			/* if passed 0 */
	CMPEQ	a2, #0			/* then exit */
	RETURNc(eq, pc, lr)
	/* input and output in a1, workspace a2 */
	TEQ	a1, #0			/* test equality with 0 */
	MOVEQ	a1, a2			/* use the high word */
	MOVEQ	a3, #32			/* need to add extra 32 bits */
	MOVNE	a3, #0			/* use the low word */
	SUB	a2, a1, #1		/* subtract if not equal */
	EOR	a1, a1, a2		/* exclusive or if not equal */
	RSB	a1, a1, a1, LSL #3	/* reverse subtract if not equal */
	RSB	a1, a1, a1, LSL #8
	RSB	a1, a1, a1, LSL #8
	RSB	a1, a1, a1, LSL #8
	/* load register, if not equal, with byte */
	ADD	a4, pc, #|ffs.table| - . - 8
	LDRB	a1, [a4, a1, LSR #26]
	ADD	a1, a1, a3
	RETURN(pc, lr)
#endif

#ifdef L_fixsfdi
	EXPORT	|__fixsfdi|
	/* Convert single precision float to 64-bit int */
|__fixsfdi|
	cmp	r0, #0
	moveq	r1, #0
	RETURNc(eq, pc, lr)

	mov	r2, r0, lsr #23 /* Extract the exponent */
	ands	r2, r2, #&ff
	bic	r0, r0, r2, lsl #23 /* Clear exponent from mantissa */
	orrne	r0, r0, #1<<23 /* add implicit bit */

	tst	r2, #1<<8  /* Is sign bit set */
	rsbne	r0, r0, #0 /* yes; negate mantissa */

	bic	r2, r2, #1<<8 /* clear sign bit, leaving exponent */

	subs	r2, r2, #150 /* remove bias from exponent */
	movlt	r0, r0, asr r2
	movlt	r1, #0
	RETURNc(lt, pc, lr)

	rsb	r2, r2, #0

	/* perform 64-bit shift */
        rsbs	ip, r2, #32
	rsble	ip, ip, #0 /* shift >= 32 bits */
	movle	r1, r0, asl ip
	movle	r0, #0
	movgt	r1, r0, lsr ip
	movgt	r0, r0, asl r2
	RETURN(pc, lr)


	/* number is represented as 1.mantissa x 2^exponent-127 */
	subs	r2, r2, #127 /* remove bias from exponent */
	movmi	r0, #0 /* if exponent negative then not representable */
	movmi	r1, #0
	RETURNc(mi, pc, lr) /* as an integer */

	/* the mantissa is 23 bits wide. assume we are representing the */
	/* number as mantissa x 2^23, then we may need to shift left */
	/* or right. */
	rsbs	r2, r2, #23
	movpl	r0, r0, asr r2
	movpl	r1, #0
	RETURNc(pl, pc, lr)

	rsb	r2, r2, #0 /* exponent was larger than 23 */

	/* perform 64-bit shift */
        rsbs	ip, r2, #32
	rsble	ip, ip, #0 /* shift >= 32 bits */
	movle	r1, r0, asl ip
	movle	r0, #0
	movgt	r1, r0, lsr ip
	movgt	r0, r0, asl r2
	RETURN(pc, lr)
#endif

#ifdef L_fixunssfdi
	EXPORT	|__fixunssfdi|
	/* Convert single precision float to 64-bit int */
|__fixunssfdi|
	cmp	r0, #0
	moveq	r1, #0
	RETURNc(eq, pc, lr)

	mov	r2, r0, lsr #23 /* Extract the exponent */
	bic	r0, r0, r2, lsl #23 /* Clear exponent from mantissa */
	orr	r0, r0, #1<<23 /* add implicit bit */
	bic	r2, r2, #1<<8 /* clear sign bit, leaving exponent */

	/* number is represented as 1.mantissa x 2^exponent-127 */
	subs	r2, r2, #127 /* remove bias from exponent */
	movmi	r0, #0 /* if exponent negative then not representable */
	movmi	r1, #0
	RETURNc(mi, pc, lr) /* as an integer */

	/* the mantissa is 23 bits wide. assume we are representing the */
	/* number as mantissa x 2^23, then we may need to shift left */
	/* or right. */
	rsbs	r2, r2, #23
	movpl	r0, r0, lsr r2
	movpl	r1, #0
	RETURNc(pl, pc, lr)

	rsb	r2, r2, #0 /* exponent was larger than 23 */

	/* perform 64-bit shift */
        rsbs	ip, r2, #32
	rsble	ip, ip, #0 /* shift >= 32 bits */
	movle	r1, r0, asl ip
	movle	r0, #0
	movgt	r1, r0, lsr ip
	movgt	r0, r0, asl r2
	RETURN(pc, lr)
#endif

#ifdef L_fixunsdfdi
	EXPORT |__fixdfdi|
	EXPORT |__fixunsdfdi|

|__fixdfdi|
	stmfd	sp!, {r14}
	mov	r2, r0, lsr #31
	tst	r2, #1
	bl	|__fixunsdfdi|
	ldmeqfd	sp!, {pc}RETCOND
	rsbs	r0, r0, #0
	rsc	r1, r1, #0
	ldmfd	sp!, {pc}RETCOND

	/* Convert double precision float to 64-bit int */
|__fixunsdfdi|
	/* Check fast case, incoming integer == 0 */
	cmp	r0, #0
	cmpeq	r1, #0
	RETURNc(eq, pc, lr)

	/* r0 = first word */
	/* r1 = second word */

	mov	r3, r0, lsr #20 /* Extract the exponent */
	bic	r2, r0, r3, lsl #20 /* clear exponent from fraction */
	orr	r2, r2, #1<<20 /* add implicit bit */
	bic	r12, r3, #1<<11 /* clear sign bit, leaving exponent */

	/* number is represented as 1.mantissa x 2^exponent-1023 */
	sub	r12, r12, #1024 /* remove bias from exponent */
	adds	r12, r12, #1
	movmi	r0, #0 /* if exponent negative then not representable */
	movmi	r1, #0 /* as an integer */
	RETURNc(mi, pc, lr)

	/* at this point: r0 = exponent */
	/* r1 = second word (bottom 32-bits of fraction) */
	/* r2 = first word (top 20-bits of fraction) */

	rsbs	r12, r12, #52
	bmi	fixunsdfdi_shl

	rsbs	r3, r12, #32  /* r3 = 32 - shift */
	rsble	r3, r3, #0
	movle	r0, r2, lsr r3 /* low = high >> -bm */
	movle	r1, #0	 	/* high = 0 */
	movgt	r0, r1, lsr r12  /* low >>= exponent */
	orrgt	r0, r0, r2, lsl r3 /* low |= high << r3 */
	movgt	r1, r2, lsr r12
	RETURN(pc, lr)

fixunsdfdi_shl
	rsb	r12, r12, #0 /* exponent was larger than 52 */

	/* perform 64-bit shift */
        rsbs	r3, r12, #32
	rsble	r3, r3, #0 /* shift >= 32 bits */
	movle	r1, r1, asl r3
	movle	r0, #0
	movgt	r2, r2, lsl r12 /* shift <= 31 bits */
	movgt	r0, r1, asl r12
	orrgt	r1, r2, r1, lsr r3
	RETURN(pc, lr)
#endif

#ifdef L_fixunsxfdi
	EXPORT |__fixxfdi|
	EXPORT |__fixunsxfdi|

|c..2|
	DCD	0x4F000000 /* 2147483648.0 */

|__fixxfdi|
	STMFD	sp!, {a1, a2, a3}
	LDFE	f2, [sp], #12
	CMFE	f2, #0.0
	BGE	|__fixuns|
	MOV	a4, lr
	MNFE	f2, f2
	BL	|__fixuns|
	RSBS	a1, a1, #0
	RSC	a2, a2, #0
	RETURN(pc, a4)

	/* On entry:
	     sp = pointer to the floating point value
	   On exit:
	     r0 = lowest significant word of double integer
	     r1 = highest significant word of double integer */
|__fixunsxfdi|
	STMFD	sp!, {a1, a2, a3}
	LDFE	f2, [sp], #12

|__fixuns|
	CMFE	f2, #0.0
	MOVLE	a1, #0
	MOVLE	a2, #0
	RETURNc(le, pc, lr)
	LDFS	f1, [pc, #|c..1| - . - 8]
	FMLS	f0, f1, #2.0
	DVFE	f0, f2, f0
	CMFE	f0, f1
	FIXLTZ	a2, f0
	SUFGEE	f0, f0, f1
	FIXGEZ	a2, f0
	EORGE	a2, a2, #&80000000
	MOV	a1, #0
	CMP	a2, #0
	RSBLT	a3, a2, #0
	FLTLTE	f0, a3
	MNFLTE	f0, f0
	FLTGEE	f0, a2
	MUFE	f0, f0, f1
	MUFE	f0, f0, #2.0
	SUFE	f2, f2, f0
	CMFE	f2, #0.0
	MNFLTE	f2, f2
	MOVLT	ip, #0
	MOVGE	ip, #1
	CMFE	f2, f1
	FIXLTZ	a3, f2
	SUFGEE	f2, f2, f1
	FIXGEZ	a3, f2
	EORGE	a3, a3, #&80000000
	CMP	ip, #0
	BNE	|do_add.2|
	SUBS	a1, a1, a3
	SBC	a2, a2, #0
	RETURN(pc, lr)
|do_add.2|
	ADDS	a1, a1, a3
	ADC	a2, a2, #0
	RETURN(pc, lr)
#endif

#ifdef L_floatdidf
	/* On entry:
	    r0 = lowest significant word of double integer
	    r1 = highest significant word of double integer
	  On exit:
	    f0 = A floating point representation of the integer */

	/* Convert 64-bit integer to 96-bit float */
	EXPORT  |__floatdixf|
	/* Convert 64-bit integer to 64-bit float */
	EXPORT  |__floatdidf|
	/* Convert 64-bit integer to 32-bit float */
	EXPORT  |__floatdisf|
|__floatdixf|
|__floatdidf|
|__floatdisf|
	/* Check fast case, incoming integer == 0 */
	cmp	r0, #0
	cmpeq	r1, #0
	mvfeqd	f0, #0.0
	RETURNc(eq, pc, lr)

	tst	r1, #1<<31
	moveq	r2, r0	/* bottom word */
	moveq	r3, r1	/* top word */
	moveq	r0, #0  /* first word of IEEE result */
	rsbne	r2, r0, #0 /* negate fraction */
	rsbne	r3, r1, #0
	movne	r0, #1<<31  /* first word of IEEE result (with sign bit) */

	cmp	r3, #0 /* if top word if 0 then we'll use the bottom word */
	movne	r12, #64 /* exponent */
	moveq	r12, #32
	moveq	r3, r2
	moveq	r2, #0

	/* Do a little loop unrolling */
	cmp	r3, #1<<8
	movcc	r3, r3, lsl #23
	orrcc	r3, r3, r2, lsr #32-23
	movcc	r2, r2, lsl #23
	subcc	r12, r12, #23
	cmp	r3, #1<<16
	movcc	r3, r3, lsl #15
	orrcc	r3, r3, r2, lsr #32-15
	movcc	r2, r2, lsl #15
	subcc	r12, r12, #15
	cmp	r3, #1<<24
	movcc	r3, r3, lsl #7
	orrcc	r3, r3, r2, lsr #32-7
	movcc	r2, r2, lsl #7
	subcc	r12, r12, #7
fltdidf_norm
	tst	r3, #1<<31
	bne	fltdidf_norm_fin
	mov	r3, r3, lsl #1
	movs	r2, r2, lsl #1
	orrcs	r3, r3, #1
	sub	r12, r12, #1
	b	fltdidf_norm

fltdidf_norm_fin
	add	r12, r12, #1020 /* add bias to exponent */
	add	r12, r12, #2

	/* pack into IEEE double precision format */
	orr	r0, r0, r12, lsl #20  /* exponent */
	bic	r3, r3, #1<<31
	orr	r0, r0, r3, lsr #11
	mov	r1, r2, lsr #11
	orr	r1, r1, r3, lsl #21
	str	r0, [sp, #-8]
	str	r1, [sp, #-4]
	ldfd	f0, [sp, #-8]
	RETURN(pc, lr)
#endif

#ifdef L_lshrdi3
	EXPORT  |__lshrdi3|

	/* DItype __lshrdi3 (DItype u, word_type b)
   	   a1, a2 = u
   	   a3 = b  */
|__lshrdi3|
	/* if shift = 0 then return */
	CMP	a3, #0
	RETURNc(eq, pc, lr)

	/* if shift >= 32 then high word will be 0 */
	RSBS	ip, a3, #32		/* bm = 32 - b */
	RSBLE	ip, ip, #0
	MOVLE	a1, a2, LSR ip		/* low = high >> - bm */
	MOVLE	a2, #0			/* high = 0 */

	MOVGT	a1, a1, LSR a3		/* low = low >> b */
	ORRGT	a1, a1, a2, ASL ip	/* low = low | high << bm */
	MOVGT	a2, a2, LSR a3		/* high = high >> b */
	RETURN(pc, lr)
#endif

#ifdef L_moddi3
	EXPORT |__moddi3|
	IMPORT |__div0|

	/* (a1, a2) / (a3, a4)
	   result in v2 and v3
	   remainder in v4 and ip  */
|__moddi3|
	/* fast exits
	   check the numerator for zero */
	CMP	a1, #0	/* low */
	CMPEQ	a2, #0	/* high */
	RETURNc(eq, pc, lr)
	/* test for a divide by zero */
	CMP	a3, #0	/* low */
	CMPEQ	a4, #0	/* high */
	BEQ	|__div0|	/* divide by zero.  */
	/* No stack checking since we are a leaf function.
	   We don't need to store ip - procedure calling standard
	   lets it be corrupted. */
	STMFD	sp!, {v2, v3, v4, v6, lr}
	/* v6 = sign bit */
	ANDS	v6, a4, #&80000000
	BPL	|__moddi3.L00|
	RSBS	a3, a3, #0 /* low */
	RSC	a4, a4, #0 /* high */
|__moddi3.L00|
	EORS	v6, v6, a2, ASR#32
	BCC	|__moddi3.L01|
	RSBS	a1, a1, #0 /* low */
	RSC	a2, a2, #0 /* high */
|__moddi3.L01|
	/* initialise the quotient to zero */
	MOV	v2, #0 /* low */
	MOV	v3, #0 /* high */
	/* remainder = numerator */
	MOV	v4, a1 /* low */
	MOV	ip, a2 /* high */

	/* lr is a count of the number of shifts needed */
        MOVS    lr, #0
	/* shift the denominator left until the next doubling
	   generates a number larger than the numerator.
	   Count the number of shifts needed */
|__moddi3.bit_count|
	/* double the denominator, we've finished when the top bit
	   is set. */
	ADDS	a3, a3, a3 /* low */
	ADCS	a4, a4, a4 /* high */
	/* too big so we've finished */
	BCS	|__moddi3.count_done|
	CMP	a4, ip /* high */
	CMPEQ	a3, v4 /* low */
	/* if denominator is lower or the same as the numerator
	/* signify an extra shift and carry on. */
	ADDLS	lr, lr, #1
	BLS	|__moddi3.bit_count|

	/* clear the carry flag */
	ADDS	lr, lr, #0

|__moddi3.count_done|
	/* we've overflowed. shift the denominator back down
	   to bring in the overflowed bit  */
	MOVS	a4, a4, RRX /* high */
	MOV	a3, a3, RRX /* low */

|__moddi3.division|
	/* numerator = remainder - denominator */
	SUBS	a1, v4, a3 /* low */
	SBCS	a2, ip, a4 /* high */
	/* if (remainder >= denominator) remainder = numerator */
	MOVCS	v4, a1 /* low */
	MOVCS	ip, a2 /* high */
	/* shift the denominator right 1 bit */
	MOVS	a4, a4, LSR #1 /* high */
	MOV	a3, a3, RRX /* low */
	SUBS	lr, lr, #1
	BGE	|__moddi3.division|
	/* result */
	MOVS	v6, v6, LSL#1
	MOVCC	a1, v4 /* low */
	MOVCC	a2, ip /* high */
	LDMCCFD	sp!, {v2, v3, v4, v6, pc}RETCOND
	RSBS	a1, v4, #0 /* low */
	RSC	a2, ip, #0 /* high */
	LDMFD	sp!, {v2, v3, v4, v6, pc}RETCOND
#endif

#ifdef L_modsi3
	EXPORT |__modsi3|
	IMPORT |__div0|

	/* a1 = (a1 / a2) remainder a2
   	   returns remainder in a1 */

|__modsi3|
	ANDS	ip, a1, #&80000000
	RSBMI	a1, a1, #0
	MOVS	a2, a2
	BEQ	|__div0|	/* divide by zero.  */
	RSBMI	a2, a2, #0
	MOV	a3, a2

	CMP	a3, a1, LSR #16
	MOVLS	a3, a3, LSL #16
	CMP	a3, a1, LSR #8
	MOVLS	a3, a3, LSL #8
	CMP	a3, a1, LSR #4
	MOVLS	a3, a3, LSL #4
	CMP	a3, a1, LSR #2
	MOVLS	a3, a3, LSL #2
	CMP	a3, a1, LSR #1
	MOVLS	a3, a3, LSL #1

|__modsi3.divloop|
	CMP	a1, a3
	SUBCS	a1, a1, a3
	MOV	a3, a3, LSR #1
	CMP	a3, a2
	BCS	|__modsi3.divloop|
	TST	ip, #&80000000
	RSBNE	a1, a1, #0
	RETURN(pc, lr)
#endif

#ifdef L_muldi3
	EXPORT |__muldi3|

	/* (a1, a2) * (a3, a4) */
|__muldi3|
#ifdef __ARM_ARCH_4__
	SMULL	a1, a2, a3, a4
	RETURN(pc, lr)
#else
	STMFD	sp!, {v1, v2, v3, lr}
	MOV	v2, a1, LSR #16
	BIC	ip, a1, v2, LSL #16
	MOV	v1, a3, LSR #16
	BIC	lr, a3, v1, LSL #16
	MUL	v3, v2, v1
	MUL	v2, lr, v2
	MUL	v1, ip, v1
	MUL	ip, lr, ip
	ADDS	lr, v2, v1
	ADDCS	v3, v3, #&10000
	ADDS	v1, ip, lr, LSL #16
	ADC	v2, v3, lr, LSR #16
	MLA	v2, a1, a4, v2
	MLA	a2, a3, a2, v2
	MOV	a1, v1
	LDMFD	sp!, {v1, v2, v3, pc}RETCOND
#endif /* __ARM_ARCH_4__ */
#endif /* L_muldi3 */

#ifdef L_ucmpdi2
	EXPORT |__ucmpdi2|
	/* compare two unsigned long long ints
	   enter with __ucmpdi2 (DItype a, DItype b)
	   entry a1,a2 = u, a3,a4 = v
	   exit u > v = 2, u < v = 0, u = v = 1  */
|__ucmpdi2|
	CMP    a2, a4
	CMPEQ  a1, a3
	MOVCC  a1, #0
	MOVHI  a1, #2
	MOVEQ  a1, #1
	RETURN(pc, lr)
#endif

#ifdef L_udivdi3
	EXPORT |__udivdi3|
	IMPORT |__div0|

/* (a1, a2) / (a3, a4)
   result in v2 and v3
   remainder in v4 and ip */
|__udivdi3|
	CMP	a1, #0
	CMPEQ	a2, #0
	RETURNc(eq, pc, lr)
	CMP	a3, #0
	CMPEQ	a4, #0
	BEQ	|__div0|  /* divide by zero.  */
	STMFD	sp!, {v2, v3, v4, lr}
	MOV	v2, #0
	MOV	v3, #0
	MOV	v4, a1
	MOV	ip, a2
        MOVS    lr, #0
|__udivdi3.bit_count|
	ADDS	a3, a3, a3
	ADCS	a4, a4, a4
	BCS	|__udivdi3.count_done|
	CMP	a4, ip
	CMPEQ	a3, v4
	ADDLS	lr, lr, #1
	BLS	|__udivdi3.bit_count|
	ADDS	lr, lr, #0
|__udivdi3.count_done|
	MOVS	a4, a4, RRX
	MOV	a3, a3, RRX
|__udivdi3.division|
	SUBS	a1, v4, a3
	SBCS	a2, ip, a4
	MOVCS	v4, a1
	MOVCS	ip, a2
	ADCS	v2, v2, v2
	ADC	v3, v3, v3
	MOVS	a4, a4, LSR #1
	MOV	a3, a3, RRX
	SUBS	lr, lr, #1
	BGE	|__udivdi3.division|
	MOV	a1, v2
	MOV	a2, v3
	LDMFD	sp!, {v2, v3, v4, pc}RETCOND
#endif

#ifdef L_udivsi3
	EXPORT |__udivsi3|
	EXPORT |__udivmodsi4|
	IMPORT |__div0|

/* a1 = (a1 / a2) remainder a2
   returns quotient in a1
   returns remainder in a2

   a1 = numerator
   a2 = denominator
   a1 = quotient
   a2 = remainder
   ip = temporary store  */

|__udivsi3|
|__udivmodsi4|
        MOV     a3, a1
        MOVS    ip, a2
	BEQ	|__div0|   /* divide by zero.  */

        CMP     ip, a3, LSR #16
        MOVLS   ip, ip, LSL #16
        CMP     ip, a3, LSR #8
        MOVLS   ip, ip, LSL #8
        CMP     ip, a3, LSR #4
        MOVLS   ip, ip, LSL #4
        CMP     ip, a3, LSR #2
        MOVLS   ip, ip, LSL #2
        CMP     ip, a3, LSR #1
        MOVLS   ip, ip, LSL #1

        MOV     a1, #0
|__udivsi3.divloop|
	CMP     a3, ip
        SUBCS   a3, a3, ip
        ADC     a1, a1, a1
        MOV     ip, ip, LSR #1
        CMP     ip, a2
        BCS     |__udivsi3.divloop|
	MOV	a2, a3
	RETURN(pc, lr)
#endif

#ifdef L_umoddi3
	EXPORT |__umoddi3|
	IMPORT |__div0|

	/* (a1, a2) / (a3, a4)
	   result in v2 and v3
	   remainder in v4 and ip */
|__umoddi3|
	CMP	a1, #0
	CMPEQ	a2, #0
	RETURNc(eq, pc, lr)
	CMP	a3, #0
	CMPEQ	a4, #0
	BEQ	|__div0|   /* divide by zero.  */
	STMFD	sp!, {v2, v3, v4, lr}
	MOV	v2, #0
	MOV	v3, #0
	MOV	v4, a1
	MOV	ip, a2
        MOVS    lr, #0
|__umoddi3.bit_count|
	ADDS	a3, a3, a3
	ADCS	a4, a4, a4
	BCS	|__umoddi3.count_done|
	CMP	a4, ip
	CMPEQ	a3, v4
	ADDLS	lr, lr, #1
	BLS	|__umoddi3.bit_count|
	ADDS	lr, lr, #0
|__umoddi3.count_done|
	MOVS	a4, a4, RRX
	MOV	a3, a3, RRX
|__umoddi3.division|
	SUBS	a1, v4, a3
	SBCS	a2, ip, a4
	MOVCS	v4, a1
	MOVCS	ip, a2
	MOVS	a4, a4, LSR #1
	MOV	a3, a3, RRX
	SUBS	lr, lr, #1
	BGE	|__umoddi3.division|
	MOV	a1, v4
	MOV	a2, ip
	LDMFD	sp!, {v2, v3, v4, pc}RETCOND
#endif

#ifdef L_umodsi3
	EXPORT |__umodsi3|
	IMPORT |__div0|

/* a1 = (a1 / a2) remainder a2
   returns remainder in a1

   a1 = numerator
   a2 = denominator
   a1 = remainder
   a3 = temporary store */


|__umodsi3|
        MOVS    a3, a2
	BEQ	|__div0|   /* divide by zero  */

        CMP     a3, a1, LSR #16
        MOVLS   a3, a3, LSL #16
        CMP     a3, a1, LSR #8
        MOVLS   a3, a3, LSL #8
        CMP     a3, a1, LSR #4
        MOVLS   a3, a3, LSL #4
        CMP     a3, a1, LSR #2
        MOVLS   a3, a3, LSL #2
        CMP     a3, a1, LSR #1
        MOVLS   a3, a3, LSL #1

|__umodsi3.divloop|
	CMP     a1, a3
        SUBCS   a1, a1, a3
        MOV     a3, a3, LSR #1
        CMP     a3, a2
        BCS     |__umodsi3.divloop|
	MOV	a1, a1
	RETURN(pc, lr)
#endif

#ifdef L_div0
	/* Division by zero handler for RISC OS.  */
	EXPORT	|__div0|
	IMPORT	raise
|__div0|
        stmfd 	sp!, {a1, a2, lr}
#ifdef TARGET_SCL
	mov	a1, #2	/* SIGFPE under the SharedCLibrary.  */
#else
	mov	a1, #8	/* SIGFPE under UnixLib.  */
#endif
	bl	raise
	ldmfd	sp!, {a1, a2, pc}RETCOND
#endif

#ifdef L_ctors
/* This is a linker file used to help in the calling of constructors
   and destructors.

   As the linker sorts areas alphabetically, and GCC uses a '2' postfix
   on the constructor/destructor areas, we end up with:

	; constructor code
	___CTOR__LIST__
	dcd -1
	dcd constructor list *
	dcd 0

	; destructor code
	___DTOR_LIST__
	dcd -1
	dcd destructor list *
	dcd 0

   Note that as we use '0', '2' and '4' as postfixes, it is possible
   to sneak in areas with postfixes of '1' and '3', if you want to
   get clever.  */


	AREA	|C$$gnu_ctorsvec0|, DATA, READONLY
	EXPORT  |__CTOR_LIST__|
|__CTOR_LIST__|
	DCD	-1

	AREA	|C$$gnu_ctorsvec4|, DATA, READONLY
	DCD	0

	AREA	|C$$gnu_dtorsvec0|, DATA, READONLY
	EXPORT  |__DTOR_LIST__|
|__DTOR_LIST__|
	DCD	-1

	AREA	|C$$gnu_dtorsvec4|, DATA, READONLY
	DCD	0
#endif

#ifdef L_builtin_next_arg
	/* Yep, mate it's a dummy function alright.  */
	AREA	|C$$code|, CODE, READONLY

	EXPORT	|__builtin_next_arg|
|__builtin_next_arg|
	RETURN(pc, lr)
#endif


#ifdef L_arm_alloca

/* This file contains support routines for the use with the ARM Procedure
Call Standard and explicit stack check targets.

Functions provided are:
  __builtin_alloca
  setjmp, longjmp
  __builtin_return_address
  __builtin_frame_address
  __unwind_function


History:
  Originally the alloca function should allocate space directly from the
  run-time stack.

  Using a method like this would mean creating a new stack frame for
  each dynamic alloca.  It was deemed unsuitable because access to local
  variables was very difficult, stack extension code was very slow and
  the prospect of actually freeing the memory was further compilcated
  with the need to directly access the stack frame in order to create
  a suitable return address. In short, an extra 16 inlined instructions
  were needed for each dynamic alloca call.  The idea of nested functions,
  C++ exception handling and various other minor details basically
  destroyed any hope of this idea.

Implementation:
  The code below implements dynamic alloca and C++ exception handling
  using five functions which are called in the necessary place by the
  GNU compiler.

  For the following description, assume that when we refer to alloca
  we mean an inlined dynamic allocation off the stack

  The emulation of alloca starts with a call to __arm_alloca_alloc.
  This function uses malloc to claim the necessary memory and also
  modifies the stack frame to call the function __arm_alloca_free_all
  on exit from the function using alloca.  Multiple calls to
  __arm_alloca_alloc do not keep modifying the stack frame return address.

  To cope with different scopes, we assign a level number to each
  alloca chunk. As the chunks leave scope, those chunks are removed.
  To cope with goto's to different scopes, higher level chunks are also
  removed.  On exit from the function, __arm_alloca_free_all will be called
  to remove any chunks not previously removed by calls to
  __arm_alloca_block_free and also any chunks defined at the lowest scope
  level of the function.

  For example:

  int foo (int x)
  {
    char *aaaa[x];      ; level 0
    char *aaab[x];
    {
      char *baaa[x];    ; level 1
      char *baab[x];
      {
        char *caaa[x];  ; level 2
        return 0;  	; Will call _block_free with level = 2 and then
                        ; exit the function which will call _free_all, thus
                        ; removing the allocas at level 0 and 1.
      }  ; _block_free called with level = 2
    }    ; _block_free called with level = 1
  }      ; _block_free called with level = 0


  As a direct consequence of this technique, setjmp and longjmp must
  be intercepted to avoid memory leakages from the dynamic allocation
  routines.

  When setjmp is called, the current pointer to the list of dynamic
  allocations is saved at the end of the jmp_buf.  This is safe, because
  gcc defines a macro to set the size of the jmp_buf (__JMP_BUF_SIZE).
  Any header file defining the jmp_buf size needs to use this macro
  otherwise the use of setjmp/longjmp will undoubtedly result in the
  program crashing.

  When longjmp is called, all the dynamic allocations made since the
  previous setjmp with the same buffer are freed.  If the value stored
  in the jmp_buf is not found during the freeing of the dynamic
  allocations then the longjmp wrapper routine will abort.

  A previous implementation used a separately allocated area of memory
  to save the linkage between the jmp_buf and the state of dynamic
  allocation.  This method had extremely bad run-time performance and
  had a memory leak with this saved linkage information persisting
  not being deallocated (not possible without more hacks to the main
  compiler). */

/* Version 2.13 (06 Nov 1999)  */

XOS_Write0	*	&20002

	AREA	|C$$data|, DATA
	ALIGN	4
	EXPORT	|__arm_alloca_st|
	/* If you change this then make sure you change the references
	   to it in arm.md (save_block_stack, save_stack_nonlocal).  */
|__arm_alloca_st|
	DCD	0	/* unsigned int level */
	DCD	0	/* chunk *list */

|arm_unwinding|
	DCD	0

|arm_register_buffer|
	%	32	/* reserve space for 8 registers */

/* Structures used within this source:

typedef struct __chunk {
   void *frame_pointer
   unsigned int level
   struct __chunk *prev_chunk
   void *return_address
} chunk

struct __arm_alloca_st {
   unsigned int level
   chunk *list
}  */


	/* offsets for fields of chunk struct, change at your peril
	   because some uses of ldm,stm rely on this ordering  */
chunks_fp_off		*	0
chunks_level_off	*	4
chunks_prev_off		*	8
chunks_retaddr_off	*	12

chunk_size		*	16

	/* offsets for fields of alloca_list struct, change at your peril
	   because some uses of ldm,stm rely on this ordering  */
level_off		*	0
list_off		*	4

	/* UnixLib's jmp_buf is 23 words, Norcroft's is 22 words
	   so gcc's is 24 (one more word than the biggest)
	   setjmp_save is the offset to the 24th word.  */
setjmp_save		*	(23*4)


	AREA	|C$$code|, CODE, READONLY

	ALIGN	4
|arm_alloca_ptr|
	DCD	|__arm_alloca_st|

	/* please ensure all routines are preceded by their own
	   register usage.  While ObjASM will complain if the same
	   name is used for different register numbers, doing this
	   helps prevent double use of a register by mistake.
	   Each routine should be able to fed independently through
	   ObjASM to verify this.  */

	/* -----------------------------------------------------------------
	   ___arm_alloca_free_all

	   register usage  */
arm_alloca_v		RN	5
alloca_level		RN	6
old_chunks		RN	7
return_addr		RN	8
chunks			RN	9

	/* scratch register usage  */
chunks_fp_a		RN	2
chunks_level_a		RN	3
chunks_retaddr_a	RN	12

	/* check offsets to fields tie up with ldm/stm register usage */
#if 0
	[ chunks_fp_off < chunks_level_off
	ASSERT	chunks_fp_a < chunks_level_a
	|
	ASSERT	chunks_fp_a > chunks_level_a
	]
	[ chunks_fp_off < chunks_prev_off
	ASSERT	chunks_fp_a < chunks
	|
	ASSERT	chunks_fp_a > chunks
	]
	[ chunks_fp_off < chunks_retaddr_off
	ASSERT	chunks_fp_a < chunks_retaddr_a
	|
	ASSERT	chunks_fp_a > chunks_retaddr_a
	]
	[ chunks_level_off < chunks_prev_off
	ASSERT	chunks_level_a < chunks
	|
	ASSERT	chunks_level_a > chunks
	]
	[ chunks_level_off < chunks_retaddr_off
	ASSERT	chunks_level_a < chunks_retaddr_a
	|
	ASSERT	chunks_level_a > chunks_retaddr_a
	]
	[ chunks_prev_off < chunks_retaddr_off
	ASSERT	chunks < chunks_retaddr_a
	|
	ASSERT	chunks > chunks_retaddr_a
	]

	[ list_off > level_off
	ASSERT	alloca_level < chunks
	|
	ASSERT	alloca_level > chunks
	]
#endif

	IMPORT	|free|
	IMPORT	|abort|

	ALIGN	4
|___arm_alloca_free_all|
	STMFD	sp!, {a1, a2, a3, a4, arm_alloca_v, chunks, old_chunks, return_addr, alloca_level, fp, ip}
	LDR	arm_alloca_v, [pc, #|arm_alloca_ptr| - . -8]
	MOV	return_addr, #0

	/* alloca_level = arm_alloca.level; chunks = arm_allocal.list; */
	LDMIA	arm_alloca_v, {alloca_level,chunks}
	MOV	old_chunks, #0
|L..17|					/* while (chunks != NULL) { */
	MOVS	a1, chunks		/*    temp = chunks; */
	BEQ	|L..3|
	LDMIA	chunks, {chunks_fp_a,chunks_level_a,chunks,chunks_retaddr_a}
	CMP	chunks_fp_a, fp	 	/* if (chunks->frame_pointer == frame_p) { */
	BNE	|L..5|

	/* Free this chunk since the frame pointers match up. */
	CMP	chunks_level_a, alloca_level
	MOVCC	alloca_level, chunks_level_a

	/* If the return address is set then record it prior to freeing this chunk */
	CMP	chunks_retaddr_a, #0
	MOVNE	return_addr, chunks_retaddr_a
	CMP	old_chunks, #0
	STRNE	chunks, [old_chunks, #chunks_prev_off]
	BL	|free|

	/* If the return address has been found, then break out of the loop now. */
	CMP	return_addr, #0
	BNE	|L..3|
	B	|L..17|
|L..5|
	CMP	old_chunks, #0
	STREQ	a1, [arm_alloca_v, #list_off]
	MOV	old_chunks, a1
	B	|L..17|
|L..3|
	CMP	old_chunks, #0
	/* if (old_chunks == NULL) arm_alloca.list = chunks */
	STREQ	chunks, [arm_alloca_v, #list_off]
	LDRNE	chunks, [arm_alloca_v, #list_off]

	/* If the list is now empty, then the level can be reset to zero */
	CMP	chunks, #0
	STREQ	chunks, [arm_alloca_v, #level_off]
	STRNE	alloca_level, [arm_alloca_v, #level_off]

	/* We are in serious trouble if there is no return address, so abort. */
	CMP	return_addr, #0
	ADREQ	a1, |dynamic_alloca_rtn_error|
	SWIEQ	XOS_Write0
	BLEQ	|abort|
	MOV	lr, return_addr
	LDMFD	sp!, {a1, a2, a3, a4, arm_alloca_v, chunks, old_chunks, return_addr, alloca_level, fp, ip}
	RETURN(pc, lr)

|dynamic_alloca_rtn_error|
	DCB	"Dynamic alloca return point corrupted", 13, 10
	DCB	0
	ALIGN


	/* -----------------------------------------------------------------
	   ___arm_alloca_alloc

	  register usage  */
buffer		RN	0	/* do not change this register */
arm_alloca_a	RN	1
gbaa_t1		RN	2	/* temp registers */
gbaa_t2		RN	3
gbaa_t3		RN	12

	/* check scratch registers are consistent for ldm/stm usage */
#if 0
	ASSERT	gbaa_t1 <> gbaa_t2
	ASSERT	gbaa_t1 <> gbaa_t3
	ASSERT	gbaa_t2 <> gbaa_t3
	ASSERT	gbaa_t1 = chunks_fp_a
	ASSERT	gbaa_t2 = chunks_level_a

	[ list_off > level_off
	ASSERT	gbaa_t3 > gbaa_t2
	|
	ASSERT	gbaa_t3 < gbaa_t2
	]
#endif

	IMPORT	|malloc|
	IMPORT	|abort|

	ALIGN	4
	EXPORT	|___arm_alloca_alloc|
|___arm_alloca_alloc|
	/* Allocating nothing ? Then just return. */
	CMP	a1, #0
	RETURNc(eq, pc, lr)
	STMFD	sp!, {lr}
	ADD	a1, a1, #chunk_size
	BL	|malloc|

	/* If we could not malloc any space then print an error message
	   and force an abort.  */
	CMP	buffer, #0
	ADREQ   a1, |dynamic_alloca_error|
	SWIEQ	XOS_Write0
	BLEQ	|abort|			/* abort never returns */
	LDR	arm_alloca_a, [pc, #|arm_alloca_ptr|-.-8]
	LDMIA	arm_alloca_a, {gbaa_t2,gbaa_t3}
	LDR	gbaa_t1, [fp, #-12]
	STMIA	buffer, {gbaa_t1,gbaa_t2,gbaa_t3}	/* set fp, level, prev */
	LDR	gbaa_t2, [fp, #-4]
	BIC	gbaa_t1, gbaa_t2, #&FC000003
	ADR	gbaa_t3, |___arm_alloca_free_all|
	CMP	gbaa_t1, gbaa_t3
	MOVEQ	gbaa_t2, #0
	STR	gbaa_t2, [buffer, #12]
	STR	buffer, [arm_alloca_a, #list_off]
	STRNE	gbaa_t3, [fp, #-4]
	ADD	buffer, buffer, #16
	LDMFD	sp!, {pc}RETCOND

|dynamic_alloca_error|
	DCB	"Not enough free memory for dynamic alloca", 13, 10
	DCB	0
	ALIGN

	/* -----------------------------------------------------------------
	   ___arm_alloca_block_free

	  register usage  */
lev			RN	4
all_allocations_freed	RN	6

/*
;arm_alloca_v		RN	v2
;old_chunks		RN	v4
;return_addr		RN	v5
;chunks			RN	v6

	; scratch register usage
;chunks_fp_a		RN	a3
;chunks_level_a		RN	a4
;chunks_retaddr_a	RN	ip
*/
	IMPORT	|free|

	ALIGN
	EXPORT	|___arm_alloca_block_free|
|___arm_alloca_block_free|
	STMFD	sp!, {lev, chunks, arm_alloca_v, old_chunks, return_addr, all_allocations_freed, fp, ip, lr}
	MOV	lev, a1
	MOV	return_addr, #0
	LDR	arm_alloca_v, [pc, #|arm_alloca_ptr|-.-8]
	MOV	all_allocations_freed, #1
	LDR	chunks, [arm_alloca_v, #list_off]
	MOV	old_chunks, #0
|L..42|
	/* while (chunks != NULL) */
	MOVS	a1, chunks
	BEQ	|L..27|
	LDMIA	chunks, {chunks_fp_a,chunks_level_a,chunks,chunks_retaddr_a}

	/* if (chunks->level >= lev)  */
	CMP	chunks_level_a, lev
	BCC	|L..29|

	/* If the return address is set then record it prior to freeing this chunk */
	CMP	chunks_retaddr_a, #0
	MOVNE	return_addr, chunks_retaddr_a

	/* Remove this entry from the list */
	CMP	old_chunks, #0
	STRNE	chunks, [old_chunks, #chunks_prev_off]
	BL	|free|

	/* If the return address has been found, then break out of the loop now. */
	CMP	return_addr, #0
	BNE	|L..27|
	B	|L..42|
|L..29|
	/* Record that we haven't freed all the alloc space for this function.
	   Note, this check is specific to block_free. */
	CMP	chunks_fp_a, fp
	MOVEQ	all_allocations_freed, #0

	/* No chunk was freed so point old_chuks to that chunk */
	CMP	old_chunks, #0
	STREQ	a1, [arm_alloca_v, #list_off]
	MOV	old_chunks, a1
	B	|L..42|
|L..27|
	/* if (old_chunks == NULL) */
	CMP	old_chunks, #0
	STREQ	chunks, [arm_alloca_v, #list_off]

	/* following optimisation using a1,a2 is valid providing asserts hold  */
#if 0
	ASSERT	level_off = 0
	ASSERT	list_off = 4
#endif
	/* if the list is now empty, then the level can be reset to zero. */
	LDMIA	arm_alloca_v, {a1, a2}  /* level, list */
	CMP	a2, #0
	MOVEQ	a1, #0
	BEQ	|L..39|
	/* else if (arm_alloca.level >= lev) */
	CMP	a1, lev
	SUBCS	a1, lev, #1
|L..39|
	STR	a1, [arm_alloca_v, #level_off]
	/* Unpatch return address on the stack. */
	CMP	return_addr, #0
	CMPNE	all_allocations_freed, #0
	STRNE	return_addr, [fp, #-4]
	LDMFD	sp!, {lev, chunks, arm_alloca_v, old_chunks, return_addr, all_allocations_freed, fp, ip, pc}RETCOND

	/* -----------------------------------------------------------------
	   ___arm_alloca_block_init

	   register usage  */
/* ;arm_alloca_a		RN	a2 */

	ALIGN	4
	EXPORT	|___arm_alloca_block_init|
|___arm_alloca_block_init|
	LDR	arm_alloca_a, [pc, #|arm_alloca_ptr|-.-8]
	LDR	a1, [arm_alloca_a, #level_off]
	ADD	a1, a1, #1
	STR	a1, [arm_alloca_a, #level_off]
	RETURN(pc, lr)

	/* -----------------------------------------------------------------
	   ___arm_alloca_setjmp

	  register usage  */
/* ;arm_alloca_a		RN	a2 */

	IMPORT	|setjmp|

	ALIGN	4
	EXPORT	|___arm_alloca_setjmp|
|___arm_alloca_setjmp|
	/* Store within the setjmp buffer a pointer to the linked
	   list of alloca chunks. longjmp will use this to free any
	   memory allocated with `alloca' between the setjmp and longjmp */
	LDR	arm_alloca_a, [pc, #|arm_alloca_ptr|-.-8]
	LDR	a4, [arm_alloca_a, #list_off]
	STR	a4, [a1, #setjmp_save]
	B	|setjmp|

	/* -----------------------------------------------------------------
	   ___arm_alloca_longjmp

	  register usage */
num_save	RN	4
search_value	RN	7
jmpbuf		RN	8
/* ;arm_alloca_v	RN	v2
;chunks		RN	v6 */

	IMPORT	|abort|
	IMPORT	|free|
	IMPORT	|longjmp|

	ALIGN	4
	EXPORT	|___arm_alloca_longjmp|
|___arm_alloca_longjmp|
	MOV	jmpbuf, a1
	MOV	num_save, a2
	LDR	arm_alloca_v, [pc, #|arm_alloca_ptr|-.-8]
	LDR	chunks, [arm_alloca_v, #list_off]
	LDR	search_value, [jmpbuf, #setjmp_save]
	B	|L..48|
|L..49|
	MOV	a1, chunks
	LDR	chunks, [chunks, #chunks_prev_off]
	STR	chunks, [arm_alloca_v, #list_off]
	STR	chunks, [jmpbuf, #setjmp_save]
	BL	|free|
|L..48|
	CMP	chunks, search_value
	CMPNE	chunks, #0
	BNE	|L..49|
	/* Something must be wrong if we have hit the end of the list
	   and not found the list pointer stored in the jmp_buf.
	   Probably calling longjmp with an undefined value  */
	/* CMP	chunks, search_value
	ADRNE	a1, |arm_alloca_longjmp_error|
	SWINE	XOS_Write0
	BNE	|abort| */
|L..51|
	/* Call longjmp with the proper values.  */
	MOV	a1, jmpbuf
	MOV	a2, num_save
	B	|longjmp|

|arm_alloca_longjmp_error|
	DCB	"longjmp error: longjmp probably called with an undefined value", 13, 10
	DCB	0
	ALIGN

	/* -----------------------------------------------------------------
	   ___arm_alloca_nonlocal_save

	  register usage  */
/* ;arm_alloca_a		RN	a2 */

	ALIGN	4
	EXPORT	|___arm_alloca_nonlocal_save|
|___arm_alloca_nonlocal_save|
	LDR	arm_alloca_a, [pc, #|arm_alloca_ptr|-.-8]
	LDR	a1, [arm_alloca_a, #list_off]
	RETURN(pc, lr)

	/* -----------------------------------------------------------------
	   ___arm_alloca_nonlocal_restore

	  register usage */
/* ;search_value	RN	7 */
/* ;arm_alloca_v	RN	v2
;chunks		RN	v6 */

	IMPORT	|abort|
	IMPORT	|free|

	ALIGN	4
	EXPORT	|___arm_alloca_nonlocal_restore|
|___arm_alloca_nonlocal_restore|
	STMFD	sp!, {chunks, arm_alloca_v, search_value, fp, ip, lr}
	MOV	search_value, a1
	LDR	arm_alloca_v, [pc, #|arm_alloca_ptr|-.-8]
	LDR	chunks, [arm_alloca_v, #list_off]
	B	|L..48.1|
|L..49.1|
	MOV	a1, chunks
	LDR	chunks, [chunks, #chunks_prev_off]
	STR	chunks, [arm_alloca_v, #list_off]
	BL	|free|
|L..48.1|
	CMP	chunks, search_value
	CMPNE	chunks, #0
	BNE	|L..49.1|
	/* Something must be wrong if we have hit the end of the list
	   and not found the list pointer stored in the jmp_buf.
	   Probably calling nonlocal_restore with an undefined value  */
	CMP	chunks, search_value
	LDMEQFD	sp!, {chunks, arm_alloca_v, search_value, fp, ip, pc}RETCOND
	ADR	a1, |arm_alloca_nonlocal_restore_error|
	SWI	XOS_Write0
	B	|abort|

|arm_alloca_nonlocal_restore_error|
	DCB	"error: nonlocal_restore probably called with an undefined value", 13, 10
	DCB	0
	ALIGN

	/* -----------------------------------------------------------------
	   __builtin_return_address

	   return the return address from 'frames' functions ago.  */
	EXPORT	|__builtin_return_address|
|__builtin_return_address|
	STMFD	sp!, {lr}
	BL	|__builtin_frame_address|
	LDR	a1, [a1, #-4]
	LDMFD	sp!, {pc}RETCOND

	/* -----------------------------------------------------------------
	   __builtin_frame_address

	   return the address of the frame 'count' functions ago  */

/*
typedef struct __exception_stack_chunk
{
  unsigned long sc_mark;   == 0xf60690ff
  struct __exception_stack_chunk *sc_next, *sc_prev;
  unsigned long sc_size;
  int (*sc_deallocate)(void *);
} _exception_stack_chunk;  */

reg_fp	RN	0
sc	RN	1
x	RN	2
frames	RN	3
bra_t1	RN	4
bra_marker	RN	5
next_fp	RN	12

	EXPORT	|__builtin_frame_address|
|__builtin_frame_address|
	MOV	ip, sp
	STMFD	sp!, {bra_t1, bra_marker, fp, ip, lr, pc}
	SUB	fp, ip, #4

	MOV	frames, a1
	/* Get previous fp value. If zero, we have no enclosing frame,
	   so no return address.  */
	LDR	a1, [fp, #-12]
	CMP	a1, #0
	/* No enclosing frame, so no return address.  */
	LDMEQEA	fp, {bra_t1, bra_marker, fp, sp, pc}RETCOND
	MOV	reg_fp, fp
	/* Make 'sc' point to the current stack chunk. */
	SUB	sc, sl, #560
	/* We must cope with a variety of UnixLib and SharedCLibrary stacks
	   occuring within a program.  */
	MOV	x, #0
	LDR	bra_marker, =&F60690FF
|__builtin_frame_address.loop|

	LDR	next_fp, [reg_fp, #-12]
	LDR	bra_t1, [sc, #0] /* sc->sc_mark  */
	CMP	bra_marker, bra_t1
	MOVNE	reg_fp, next_fp
	BNE	|__builtin_frame_address.loop.end|

	LDR	bra_t1, [sc, #12]  /* sc->sc_size  */
	ADD	bra_t1, bra_t1, sc   /* sc_limit = sc+sc->sc_size */
	CMP	next_fp, bra_t1  /* next_fp >= sc_limit */
	BGE	|__builtin_frame_address.shared.c.library|

	CMP	next_fp, sc
	MOVGE	reg_fp, next_fp
	BGE	|__builtin_frame_address.loop.end|

	/* SharedCLibrary style: Current frame is outside current stack
	   chunk, so current frame's stack is the first in this chunk
	   and was created via stack extension.

	   This means that x$stack_overflow has copied the current stack
	   frame somewhere else and then created a new stack frame (in it's
	   old place) which points to the 'current' frame.  */
|__builtin_frame_address.shared.c.library|
	LDR	sc, [sc, #8] /* sc->sc_prev */
	CMP	sc, #0 /* No previous stack chunk */
	MOVEQ	a1, #0
	LDMEQEA	fp, {bra_t1, bra_marker, fp, sp, pc}RETCOND
	LDR	reg_fp, [next_fp, #-12]

|__builtin_frame_address.loop.end|
	ADD	x, x, #1
	CMP	x, frames
	BLE	|__builtin_frame_address.loop|


	LDR	bra_t1, [sc, #0]
	CMP	bra_t1, bra_marker	/* sc->sc_mark == 0xf60690ff */
	/*; MOVNE	a1, reg_fp. If reg_fp != a1 then we need this instruction */
	LDMNEEA	fp, {bra_t1, bra_marker, fp, sp, pc}RETCOND

	LDR	bra_t1, [sc, #12]
	ADD	bra_t1, bra_t1, sc ; sc+sc->sc_size
	CMP	reg_fp, bra_t1
	LDRGE	a1, [reg_fp, #-12]
	LDMGEEA	fp, {bra_t1, bra_marker, fp, sp, pc}RETCOND

	CMP	reg_fp, sc
	LDRLT	reg_fp, [reg_fp, #-12]
	/* MOV	a1, reg_fp. If reg_fp != a1 then we need this instruction */

	LDMEA	fp, {bra_t1, bra_marker, fp, sp, pc}RETCOND

	/* -----------------------------------------------------------------
	   __unwind_function

	   unwinds the stack by one level, effectively making the stack of
	   the parent function available.  */

uf_t1	RN	0
temp_lr	RN	2
/* ;sc	RN	1
;next_fp	RN	12 */

	EXPORT	|__unwind_function|
|__unwind_function|
	MOV	temp_lr, lr
	LDR	a1, [fp, #-12]
	CMP	a1, #0
	BLEQ	|abort|	/* no enclosing frame, so no previous stack */
	/* MOV	reg_fp, fp  */
	SUB	sc, sl, #560
	LDR	uf_t1, =&F60690FF
	LDR	next_fp, [sc, #0] /* use next_fp as a temp. reg here */
	CMP	next_fp, uf_t1	/* sc->sc_mark == 0xf60690ff */
	LDMNEEA fp, {fp, sp, lr} /* unwind stack one level and exit */
	RETURNc(ne, pc, temp_lr)

	LDR	next_fp, [fp, #-12]
	LDR	uf_t1, [sc, #12] /* sc->sc_size */
	ADD	uf_t1, uf_t1, sc
	CMP	next_fp, uf_t1
	BGE	|__unwind_function.shared.c.library|

	CMP	next_fp, sc
	LDMGEEA fp, {fp, sp, lr} /* unwind stack one level and exit */
	RETURNc(ge, pc, temp_lr)
|__unwind_function.shared.c.library|
	; Shared C Library style stack chunk
	LDR	uf_t1, [sc, #8] /* sc->sc_prev */
	CMP	uf_t1, #0
	BLEQ	|abort|		/* no previous stack chunk */
	/* Move to previous stack chunk, free the current one,
	   and remove the pointer to the next stack chunk in the
	   previous stack chunk.  */
	MOV	a1, #0
	STR	a1, [sl, #4]	/* sc->sc_next = 0 */
	MOV	a1, sc
	LDR	sc, [sc, #8]
	STMFD	sp!, {sc, temp_lr, next_fp}
	MOV	lr, pc
	LDR	pc, [a1, #16]	/* sc_deallocate (sc) */
	LDMFD	sp!, {sc, temp_lr, next_fp}
	LDMEA	next_fp, {fp, sp, lr}
	ADD	sl, sc, #560
	RETURN(pc, temp_lr)
#endif

        END

