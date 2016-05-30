/*
 * Copyright (c) 2015, 2016 GCCSDK Developers.
 * Written by Lee Noar.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

	.macro REPORT_TEXT string

	STR	lr, [sp, #-4]!

	SWI	0x74c81
	.asciz	"\string"
	.align

	LDR	lr, [sp], #4

	.endm

	.macro	NAME	name
#ifndef NO_EMBED_FUNCTION_NAME
0:
	.asciz	"\name"
	.align
1:
	.word	0xff000000 + (1b - 0b)
#endif
	.endm

	@ For functions of upto 4 arguments
	.macro	DEFINE_FUNC	name, swi, reason
	.global	\name
	NAME \name
\name:
@REPORT_TEXT \name
	MOV	r8, #(\reason & 0xff)
	.if \reason > 255
	ORR	r8, r8, #(\reason & 0xff00)
	.endif
	SWI	\swi
	MOV	pc, lr
	.type	\name, %function
	.size	\name, . - \name
	.endm

	@ For functions with 5 arguments
	.macro	DEFINE_FUNC_5	name, swi, reason
	.global	\name
	NAME \name
\name:
@REPORT_TEXT \name
	STR	r4, [sp, #-4]!
	LDR	r4, [sp, #4]
	MOV	r8, #(\reason & 0xff)
	.if \reason > 255
	ORR	r8, r8, #(\reason & 0xff00)
	.endif
	SWI	\swi
	LDR	r4, [sp], #4
	MOV	pc, lr
	.type	\name, %function
	.size	\name, . - \name
	.endm

	@ For functions with 6 arguments
	.macro	DEFINE_FUNC_6	name, swi, reason
	.global	\name
	NAME \name
\name:
@REPORT_TEXT \name
	STMFD	sp!, {r4, r5}
	ADD	r4, sp, #8
	LDMIA	r4, {r4, r5}
	MOV	r8, #(\reason & 0xff)
	.if \reason > 255
	ORR	r8, r8, #(\reason & 0xff00)
	.endif
	SWI	\swi
	LDMIA	sp!, {r4, r5}
	MOV	pc, lr
	.type	\name, %function
	.size	\name, . - \name
	.endm

	@ For functions with 7 arguments
	.macro	DEFINE_FUNC_7	name, swi, reason
	.global	\name
	NAME \name
\name:
@REPORT_TEXT \name
	STMFD	sp!, {r4, r5, r6}
	ADD	r4, sp, #12
	LDMIA	r4, {r4, r5, r6}
	MOV	r8, #(\reason & 0xff)
	.if \reason > 255
	ORR	r8, r8, #(\reason & 0xff00)
	.endif
	SWI	\swi
	LDMIA	sp!, {r4, r5, r6}
	MOV	pc, lr
	.type	\name, %function
	.size	\name, . - \name
	.endm

	# This assumes a simple stub function that immediately returns and
	# should never be called.
	.macro WEAK_STUB_DECL	name
	.weak	\name
	.type	\name, %function
	.size	\name, 4
\name:
	.endm
