;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_memset.s,v $
; $Date: 2005/01/03 22:55:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

; This is based upon code from the GNU C library and carries the following
; notice:

;  Copyright (C) 1998 Free Software Foundation, Inc.
;  This file is part of the GNU C Library.
;  Contributed by Philip Blundell <philb@gnu.org>
;
;  The GNU C Library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU Lesser General Public
;  License as published by the Free Software Foundation; either
;  version 2.1 of the License, or (at your option) any later version.
;
;  The GNU C Library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;  Lesser General Public License for more details.
;
;  You should have received a copy of the GNU Lesser General Public
;  License along with the GNU C Library; if not, write to the Free
;  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
;  02111-1307 USA.


; void *memset (dstpp, c, len)

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|memset|
	EXPORT  |bzero|
|bzero|
        mov     a3, a2
        mov     a2, #0 

|memset|
	mov	a4, a1
	cmp	a3, #8		; at least 8 bytes to do?
	blt	%fa2
	orr	a2, a2, a2, lsl #8
	orr	a2, a2, a2, lsl #16
1
	tst	a4, #3		; aligned yet?
	strneb	a2, [a4], #1
	subne	a3, a3, #1
	bne	%ba1
	mov	ip, a2
1
	cmp	a3, #8		; 8 bytes still to do?
	blt	%fa2
	stmia	a4!, {a2, ip}
	sub	a3, a3, #8
	cmp	a3, #8		; 8 bytes still to do?
	blt	%fa2
	stmia	a4!, {a2, ip}
	sub	a3, a3, #8
	cmp	a3, #8		; 8 bytes still to do?
	blt	%fa2
	stmia	a4!, {a2, ip}
	sub	a3, a3, #8
	cmp	a3, #8		; 8 bytes still to do?
	stmgeia	a4!, {a2, ip}
	subge	a3, a3, #8
	bge	%ba1
2
	movs	a3, a3		; anything left?
        moveq   pc, lr          ; nope

	rsb	a3, a3, #7
	add	pc, pc, a3, lsl #2
	mov	r0, r0
	strb	a2, [a4], #1
	strb	a2, [a4], #1
	strb	a2, [a4], #1
	strb	a2, [a4], #1
	strb	a2, [a4], #1
	strb	a2, [a4], #1
	strb	a2, [a4], #1
        mov     pc, lr

        END

