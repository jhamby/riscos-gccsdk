; ARM mpn_mul_1 -- Multiply a limb vector with a limb and add the result to a
; second limb vector.
; Contributed by Robert Harley.

; Copyright (C) 1998, 2000 Free Software Foundation, Inc.

; This file is part of the GNU MP Library.

; The GNU MP Library is free software; you can redistribute it and/or modify
; it under the terms of the GNU Lesser General Public License as published by
; the Free Software Foundation; either version 2.1 of the License, or (at your
; option) any later version.

; The GNU MP Library is distributed in the hope that it will be useful, but
; WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
; License for more details.

; You should have received a copy of the GNU Lesser General Public License
; along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
; the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
; MA 02111-1307, USA.

p RN 0
a RN 1
n RN 2
w RN 3
z RN 11
ip RN 12
sp RN 13
lr RN 14
pc RN 15

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|__gmpn_addmul_1|
|__gmpn_addmul_1|
	stmfd	sp!, { r8-r11, lr }
	mov	z, #0
	mov	ip, #0
	movs	n, n, lsr #1
	bcc	skip1
	ldr	lr, [a], #4
	ldr	r9, [p]
	umlal	r9, ip, w, lr
	str	r9, [p], #4
skip1
	movs	n, n, lsr #1
	bcc	skip2
	ldmia	p, { r9, r10 }
	adds	r8, ip, r9
	adc	r9, z, #0
	ldmia	a!, { ip, lr }
	umlal	r8, r9, w, ip
	adds	r9, r9, r10
	adc	ip, z, #0
	umlal	r9, ip, w, lr
	stmia	p!, { r8, r9 }
skip2
	teq	n, #0
	beq	return
	stmfd	sp!, { r4-r7 }
addmul_loop2
	ldmia	p, { r5, r6, r7, r8 }
	adds	r4, ip, r5
	adc	r5, z, #0
	ldmia	a!, { r9, r10, ip, lr }
	umlal	r4, r5, w, r9
	adds	r5, r5, r6
	adc	r6, z, #0
	umlal	r5, r6, w, r10
	adds	r6, r6, r7
	adc	r7, z, #0
	umlal	r6, r7, w, ip
	adds	r7, r7, r8
	adc	ip, z, #0
	umlal	r7, ip, w, lr
	subs	n, n, #1
	stmia	p!, { r4, r5, r6, r7 }
	bne	addmul_loop
	ldmfd	sp!, { r4-r7 }
return
	mov	r0, ip
	ldmfd	sp!, { r8-r11, pc }
