;----------------------------------------------------------------------------
;
; $Source$
; $Date$
; $Revision$
; $State$
; $Author$
;
; _swi and _swix implementation
;
;----------------------------------------------------------------------------

	AREA |C$$code|, CODE, READONLY

	EXPORT _swi
	EXPORT _swix

	; _kernel_oserror *_swix(int swi_no, unsigned int, ...)
_swix
        ORR	a1, a1, #(1<<17)	; add X bit


	; int _swi(int swi_no, unsigned int, ...)
_swi
	STMFD	sp!, {a3, a4}		; push remaining arguments onto stack

	; order of variadic arguments on stack:
	;
	; input regs (r0->r9) as specified by bits 0->9 of the mask
	; output regs (r0->r9) as specified by bits 31->22 of the mask
	; PC output reg, if mask bit 21 is set
	; block arguments, if mask bit 11 is set


	STMFD	sp!, {a2-a4, v1-v6, r10-r11, lr} ; save APCS regs & scratch
	ADD	lr, sp, #48		; lr points to arguments

	; scratch space on stack:
	;
	; sp+0	mask
	; sp+4	pointer to remaining arguments (lr)
	; sp+8	SWI number

	STR	a1, [sp, #8]		; save SWI num on stack

	MOV	a3, #&300
	ORR	a3, a3, #&FF
	TST	a2, a3			; fast-track no arguments
	BEQ	swi_block

	MOV	r11, a2			; stick mask in r11

	; load registers off stack, two at a time
	MOVS	r10, r11, LSL #31
	LDRMI	a1, [lr], #4
	LDRCS	a2, [lr], #4
	MOVS	r10, r11, LSL #29
	LDRMI	a3, [lr], #4
	LDRCS	a4, [lr], #4
	; done 4, can we stop here?
	TST	r11, #&3F0
	BEQ	swi_block

	MOVS	r10, r11, LSL #27
	LDRMI	v1, [lr], #4
	LDRCS	v2, [lr], #4
	MOVS	r10, r11, LSL #25
	LDRMI	v3, [lr], #4
	LDRCS	v4, [lr], #4
	MOVS	r10, r11, LSL #23
	LDRMI	v5, [lr], #4
	LDRCS	v6, [lr], #4

	; now handle parameter block (if specified)
swi_block
	STR	lr, [sp, #4]		; save the argument ptr

	TST	r11, #(1<<11)
	BEQ	swi_call

	; count number of output regs, so we can point
	; the block register somewhere sensible
	MOV	r12, #0
	MOVS	r10, r11, LSL #1
	ADDMI	r12, r12, #1		; 1
	ADDCS	r12, r12, #1		; 0
	MOVS	r10, r11, LSL #3
	ADDMI	r12, r12, #1		; 3
	ADDCS	r12, r12, #1		; 2
	MOVS	r10, r11, LSL #5
	ADDMI	r12, r12, #1		; 5
	ADDCS	r12, r12, #1		; 4
	MOVS	r10, r11, LSL #7
	ADDMI	r12, r12, #1		; 7
	ADDCS	r12, r12, #1		; 6
	MOVS	r10, r11, LSL #9
	ADDMI	r12, r12, #1		; 9
	ADDCS	r12, r12, #1		; 8
	; return PC too?
	TST	r11, #(1<<21)
	ADDNE	r12, r12, #1

	; block register number is in bits 12-15 of the mask
	MOVS	r10, r11, LSL #17
	BCC	%FA10
	; 8, 9
	TST	r11, #(1<<12)
	ADDNE	v6, lr, r12, LSL #2	; 9
	ADDEQ	v5, lr, r12, LSL #2	; 8
	B	swi_call
10	; 0->7
	BPL	%FA20
	; 4->7
	TST	r11, #(3<<12)
	ADDEQ	v1, lr, r12, LSL #2	; 4
	BEQ	swi_call

	TSTNE	r11, #(1<<12)
	ADDEQ	v3, lr, r12, LSL #2	; 6
	BEQ	swi_call

	TST	r11, #(1<<13)
	ADDNE	v4, lr, r12, LSL #2	; 7
	ADDEQ	v2, lr, r12, LSL #2	; 5
	B	swi_call
20	; 0->3
	TST	r11, #(3<<12)
	ADDEQ	a1, lr, r12, LSL #2	; 0
	BEQ	swi_call

	TSTNE	r11, #(1<<12)
	ADDEQ	a3, lr, r12, LSL #2	; 2
	BEQ	swi_call

	TST	r11, #(1<<13)
	ADDNE	a4, lr, r12, LSL #2	; 3
	ADDEQ	a2, lr, r12, LSL #2	; 1

	; now call the SWI
swi_call
	LDR	r12, [sp, #8]		; grab the SWI num off the stack
	STR	r11, [sp, #0]		; save the mask, for use on return
	SWI	&71			; OS_CallASWIR12

	; now handle the SWI return
	LDR	lr, [sp, #4]		; restore argument pointer
	LDR	r11, [sp, #0]		; restore mask

	; TODO: should we return the entire PC, rather than just the PSR
	;       (even though only the top 4 bits would have any meaning)?
	MRS	r12, CPSR		; save flags (won't work on <ARMv3)
	STR	r12, [sp, #0]		; on stack

	BVC	fill_return		; V clear, continue as normal
					; TODO: any way to avoid this branch?

	LDR	r12, [sp, #8]		; get the SWI num
	TST	r12, #(1<<17)		; X bit set?
	BNE	swi_end			; yes => exit

fill_return
	; fill in return values
	MOVS	r10, r11, LSL #1
	LDRMI	r12, [lr], #4
	STRMI	a2, [r12, #0]
	LDRCS	r12, [lr], #4
	STRCS	a1, [r12, #0]
	MOVS	r10, r11, LSL #3
	LDRMI	r12, [lr], #4
	STRMI	a4, [r12, #0]
	LDRCS	r12, [lr], #4
	STRCS	a3, [r12, #0]
	; done 4, can we stop here?
	TST	r11, #&FC0
	BEQ	swi_flags

	MOVS	r10, r11, LSL #5
	LDRMI	r12, [lr], #4
	STRMI	v2, [r12, #0]
	LDRCS	r12, [lr], #4
	STRCS	v1, [r12, #0]
	MOVS	r10, r11, LSL #7
	LDRMI	r12, [lr], #4
	STRMI	v4, [r12, #0]
	LDRCS	r12, [lr], #4
	STRCS	v3, [r12, #0]
	MOVS	r10, r11, LSL #9
	LDRMI	r12, [lr], #4
	STRMI	v6, [r12, #0]
	LDRCS	r12, [lr], #4
	STRCS	v5, [r12, #0]

	; do we need to put the processor flags anywhere?
swi_flags
	TST	r11, #(1<<21)
	LDRNE	lr, [lr, #0]
	LDRNE	r12, [sp, #0]
	STRNE	r12, [lr]

swi_return
	LDR	r12, [sp, #8]		; this is not applicable for swix
	TST	r12, #(1<<17)
	MOVNE	a1, #0			; ensure no error
	BNE	swi_end

	; return register number is in bits 16-19
	; fast-track common case exit (r0)
	TST	r11, #(15<<16)
	BEQ	swi_end

	; TODO: it's feasible to be asked to return r10->r14, too. should we?
	MOVS	r10, r11, LSL #13
	BCC	%FA30
	; 8, 9, 15
	TST	r11, #(1<<16)
	MOVEQ	a1, v5			; 8
	BEQ	swi_end
	TSTNE	r11, #(1<<17)
	MOVEQ	a1, v6			; 9
	MOVNE	a1, r12			; 15
	B	swi_end
30	; 0->7
	BPL	%FA40
	; 4->7
	TST	r11, #(3<<16)
	MOVEQ	a1, v1			; 4
	BEQ	swi_end

	TSTNE	r11, #(1<<16)
	MOVEQ	a1, v3			; 6
	BEQ	swi_end

	TST	r11, #(1<<17)
	MOVNE	a1, v4			; 7
	MOVEQ	a1, v2			; 5
	B	swi_end
40	; 0->3
	TST	r11, #(3<<16)
	MOVEQ	a1, a1			; 0
	BEQ	swi_end

	TSTNE	r11, #(1<<16)
	MOVEQ	a1, a3			; 2
	BEQ	swi_end

	TST	r11, #(1<<17)
	MOVNE	a1, a4			; 3
	MOVEQ	a1, a2			; 1

swi_end
	LDMFD	sp!, {a2-a4, v1-v6, r10-r11, lr}
	LDMFD	sp!, {a2, a3}
	MOV	pc, lr

	END
