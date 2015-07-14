/*
 * Copyright (c) 2015, GCCSDK Developers.
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

	.text

@ void asm_khrn_callback(void *callback_param,VCHI_CALLBACK_REASON_T reason,void *handle)
	.global asm_khrn_callback
asm_khrn_callback:
	STMFD	sp!, {r0-r11,lr}

	MOV	r10, r13, LSR#20
	MOV	r10, r10, LSL#20
	LDR	r12, [r12]
	ADD	r10, r10, #20
	LDMIA	r10, {r4, r5}
	LDMIB	r12, {r11, r12}
	STMIA	r10, {r11, r12}
	ADD	r10, r10, #540
	BL	khrn_callback
	SUB	r10, r10, #540
	STMIA	r10, {r4, r5}

	LDMFD	sp!, {r0-r11,lr}
	MOV	pc, lr

@ void asm_khhn_callback(void *callback_param,VCHI_CALLBACK_REASON_T reason,void *handle)
	.global asm_khhn_callback
asm_khhn_callback:
	STMFD	sp!, {r0-r11,lr}

	MOV	r10, r13, LSR#20
	MOV	r10, r10, LSL#20
	LDR	r12, [r12]
	ADD	r10, r10, #20
	LDMIA	r10, {r4, r5}
	LDMIB	r12, {r11, r12}
	STMIA	r10, {r11, r12}
	ADD	r10, r10, #540
	BL	khhn_callback
	SUB	r10, r10, #540
	STMIA	r10, {r4, r5}

	LDMFD	sp!, {r0-r11,lr}
	MOV	pc, lr

@ void asm_khan_callback(void *callback_param,VCHI_CALLBACK_REASON_T reason,void *handle)
	.global asm_khan_callback
asm_khan_callback:
	STMFD	sp!, {r0-r11,lr}

	MOV	r10, r13, LSR#20
	MOV	r10, r10, LSL#20
	LDR	r12, [r12]
	ADD	r10, r10, #20
	LDMIA	r10, {r4, r5}
	LDMIB	r12, {r11, r12}
	STMIA	r10, {r11, r12}
	ADD	r10, r10, #540
	BL	khan_callback
	SUB	r10, r10, #540
	STMIA	r10, {r4, r5}

	LDMFD	sp!, {r0-r11,lr}
	MOV	pc, lr
