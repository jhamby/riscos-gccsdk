@ _swi, _swix
@ Copyright (c) 2002-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_swi
	.global	_swix

	@ _kernel_oserror *_swix(int swi_no, unsigned int, ...)
_swix:
        ORR	a1, a1, #(1<<17)	@ add X bit


	@ int _swi(int swi_no, unsigned int, ...)
_swi:
	STMFD	sp!, {a3, a4}		@ push remaining arguments onto stack

	@ order of variadic arguments on stack:
	@
	@ input regs (r0->r9) as specified by bits 0->9 of the mask
	@ output regs (r0->r9) as specified by bits 31->22 of the mask
	@ PC output reg, if mask bit 21 is set
	@ block arguments, if mask bit 11 is set


	STMFD	sp!, {a1-a4, v1-v6, r10-r11, lr} @ save APCS regs & scratch
	ADD	lr, sp, #52		@ lr points to arguments

	@ scratch space on stack:
	@
	@ sp+0	mask
	@ sp+4	pointer to remaining arguments (lr)
	@ sp+8	SWI number
	@ sp+12	PC value to return

	STR	a1, [sp, #8]		@ save SWI num on stack
	MOV	r11, a2			@ stick mask in r11

	MOV	a3, #0x300
	ORR	a3, a3, #0xFF
	TST	a2, a3			@ fast-track no arguments
	BEQ	swi_block

	@ load registers off stack, two at a time
	MOVS	r10, r11, LSL #31
	LDRMI	a1, [lr], #4
	LDRCS	a2, [lr], #4
	MOVS	r10, r11, LSL #29
	LDRMI	a3, [lr], #4
	LDRCS	a4, [lr], #4
	@ done 4, can we stop here?
	TST	r11, #0x3F0
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

	@ now handle parameter block (if specified)
swi_block:
	STR	lr, [sp, #4]		@ save the argument ptr

	TST	r11, #(1<<11)
	BEQ	swi_call

	@ count number of output regs, so we can point
	@ the block register somewhere sensible
	MOV	r12, #0
	MOVS	r10, r11, LSL #1
	ADDMI	r12, r12, #1		@ 1
	ADDCS	r12, r12, #1		@ 0
	MOVS	r10, r11, LSL #3
	ADDMI	r12, r12, #1		@ 3
	ADDCS	r12, r12, #1		@ 2
	MOVS	r10, r11, LSL #5
	ADDMI	r12, r12, #1		@ 5
	ADDCS	r12, r12, #1		@ 4
	MOVS	r10, r11, LSL #7
	ADDMI	r12, r12, #1		@ 7
	ADDCS	r12, r12, #1		@ 6
	MOVS	r10, r11, LSL #9
	ADDMI	r12, r12, #1		@ 9
	ADDCS	r12, r12, #1		@ 8
	@ return PC too?
	TST	r11, #(1<<21)
	ADDNE	r12, r12, #1

	@ block register number is in bits 12-15 of the mask
	ANDS	r10, r11, #(15<<12)
	ADDEQ	a1, lr, r12, LSL #2
	BEQ	swi_call

	TEQ	r10, #(1<<12)
	ADDEQ	a2, lr, r12, LSL #2
	TEQ	r10, #(2<<12)
	ADDEQ	a3, lr, r12, LSL #2
	TEQ	r10, #(3<<12)
	ADDEQ	a4, lr, r12, LSL #2
	TEQ	r10, #(4<<12)
	ADDEQ	v1, lr, r12, LSL #2
	TEQ	r10, #(5<<12)
	ADDEQ	v2, lr, r12, LSL #2
	TEQ	r10, #(6<<12)
	ADDEQ	v3, lr, r12, LSL #2
	TEQ	r10, #(7<<12)
	ADDEQ	v4, lr, r12, LSL #2
	TEQ	r10, #(8<<12)
	ADDEQ	v5, lr, r12, LSL #2
	TEQ	r10, #(9<<12)
	ADDEQ	v6, lr, r12, LSL #2

	@ now call the SWI
swi_call:
	LDR	r12, [sp, #8]		@ grab the SWI num off the stack
	STR	r11, [sp, #0]		@ save the mask, for use on return
	SWI	0x71			@ OS_CallASWIR12

	STR	pc, [sp, #12]		@ save PC on return

	@ now handle the SWI return

	MOV	r12, #0			@ put known value in r12
	MRS	r12, CPSR		@ save flags (NOP on <ARMv3)
	TEQ	r12, #0			@ <ARMv3?
	@ If < ARMv3, PC already contains flags
	@    >=ARMv3, need to ORR flags into PC value
	LDRNE	lr, [sp, #12]		@ get PC off stack
	BICNE	lr, lr, #(15<<28)	@ clear top 4 bits of PC
	ANDNE	r12, r12, #(15<<28)	@ only interested in the flags
	ORRNE	r12, lr, r12		@ put flags into top 4 bits of PC
	STRNE	r12, [sp, #12]		@ save on stack

	LDR	r11, [sp], #4		@ restore mask

	@ the stack pointer has now been moved (to avoid corrupting a1 on
	@ exit). Therefore from now on, the scratch space looks like:
	@
	@ sp+0	pointer to remaining arguments (lr)
	@ sp+4	SWI number
	@ sp+8	PC value to return

	LDR	lr, [sp, #0]		@ restore argument pointer

	BVC	fill_return		@ V clear, continue as normal
					@ TODO: any way to avoid this branch?

	LDR	r12, [sp, #4]		@ get the SWI num
	TST	r12, #(1<<17)		@ X bit set?
	BNE	swi_end			@ yes => exit

fill_return:
	@ fill in return values
	MOVS	r10, r11, LSL #1
	LDRCS	r12, [lr], #4
	STRCS	a1, [r12, #0]
	LDRMI	r12, [lr], #4
	STRMI	a2, [r12, #0]
	MOVS	r10, r11, LSL #3
	LDRCS	r12, [lr], #4
	STRCS	a3, [r12, #0]
	LDRMI	r12, [lr], #4
	STRMI	a4, [r12, #0]
	@ done 4, can we stop here?
	TST	r11, #(0xFC<<20)
	BEQ	swi_flags

	MOVS	r10, r11, LSL #5
	LDRCS	r12, [lr], #4
	STRCS	v1, [r12, #0]
	LDRMI	r12, [lr], #4
	STRMI	v2, [r12, #0]
	MOVS	r10, r11, LSL #7
	LDRCS	r12, [lr], #4
	STRCS	v3, [r12, #0]
	LDRMI	r12, [lr], #4
	STRMI	v4, [r12, #0]
	MOVS	r10, r11, LSL #9
	LDRCS	r12, [lr], #4
	STRCS	v5, [r12, #0]
	LDRMI	r12, [lr], #4
	STRMI	v6, [r12, #0]

	@ do we need to return PC?
swi_flags:
	TST	r11, #(1<<21)
	LDRNE	lr, [lr, #0]
	LDRNE	r12, [sp, #8]
	STRNE	r12, [lr]

swi_return:
	LDR	r12, [sp, #4]		@ this is not applicable for _swix
	TST	r12, #(1<<17)
	MOVNE	a1, #0			@ ensure no error
	BNE	swi_end

	ANDS	r11, r11, #(15<<16)
	BEQ	swi_end

	@ return register number is in bits 16-19 of the mask
	TEQ	r11, #(1<<16)
	MOVEQ	a1, a2
	TEQ	r11, #(2<<16)
	MOVEQ	a1, a3
	TEQ	r11, #(3<<16)
	MOVEQ	a1, a4
	TEQ	r11, #(4<<16)
	MOVEQ	a1, v1
	TEQ	r11, #(5<<16)
	MOVEQ	a1, v2
	TEQ	r11, #(6<<16)
	MOVEQ	a1, v3
	TEQ	r11, #(7<<16)
	MOVEQ	a1, v4
	TEQ	r11, #(8<<16)
	MOVEQ	a1, v5
	TEQ	r11, #(9<<16)
	MOVEQ	a1, v6
	@ PC
	TEQ	r11, #(15<<16)
	LDREQ	a1, [sp, #8]

swi_end:
	LDMFD	sp!, {a2-a4, v1-v6, r10-r11, lr}
	LDMFD	sp!, {a2, a3}
	MOV	pc, lr
	DECLARE_FUNCTION _swix
	DECLARE_FUNCTION _swi

	.end
