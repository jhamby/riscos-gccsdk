; ARM mpn_addmul_1 -- Multiply a limb vector with a limb and store the result
; in a second limb vector.
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
sl RN 10
fp RN 11
ip RN 12
sp RN 13
lr RN 14
pc RN 15

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|__gmpn_mul_1|
|__gmpn_mul_1|
	stmfd	sp!, { r8, r9, lr }
	ands	ip, n, #1
	beq	skip1
	ldr	lr, [a], #4
	umull	r9, ip, w, lr
	str	r9, [p], #4
skip1
	tst	n, #2
	beq	skip2
	mov	r8, ip
	ldmia	a!, { ip, lr }
	mov	r9, #0
	umlal	r8, r9, w, ip
	mov	ip, #0
	umlal	r9, ip, w, lr
	stmia	p!, { r8, r9 }
skip2
	bics	n, n, #3
	beq	return
	stmfd	sp!, { r6, r7 }
mul_1_loop
	mov	r6, ip
	ldmia	a!, { r8, r9, ip, lr }
	ldr	r7, [p] /* Bring stuff into cache. */
	mov	r7, #0
	umlal	r6, r7, w, r8
	mov	r8, #0
	umlal	r7, r8, w, r9
	mov	r9, #0
	umlal	r8, r9, w, ip
	mov	ip, #0
	umlal	r9, ip, w, lr
	subs	n, n, #4
	stmia	p!, { r6, r7, r8, r9 }
	bne	mul_1_loop
	ldmfd	sp!, { r6, r7 }
return
	mov	r0, ip
	ldmfd	sp!, { r8, r9, pc }
