	.set XOS_IntOn, 0x20013
	.set XOS_EnterOS, 0x20016
	.set XOS_MMUControl, 0x2006b

	.globl trigger_callbacks
trigger_callbacks:
	MOV	r12, lr
	MRS	r0, CPSR
	BIC	r0, r0, #0xf
	MSR	CPSR_c, r0	@ Switch to USR mode
	SWI	XOS_IntOn	@ (callbacks trigger on exit)
	SWI	XOS_EnterOS	@ return to SVC
	MOV	pc, r12		@ and back whence we came

	.globl irqs_off
irqs_off:
	MRS	r0, CPSR
	ORR	r1, r0, #0x80	@ Set I
	TEQ	r0, r1
	MSRNE	CPSR_c, r1	@ Disable IRQs if they were enabled
	AND	r0, r0, #0x80	@ Return I bit as our state
	MOV	pc, lr

	.globl irqs_on
irqs_on:
	MRS	r0, CPSR
	BIC	r1, r0, #0x80	@ Clear I
	TEQ	r0, r1
	MSRNE	CPSR_c, r1	@ Enable IRQs if they were disabled
	AND	r0, r0, #0x80	@ Return I bit as our state
	MOV	pc, lr

	.globl irqs_restore
irqs_restore:
	MRS	r1, CPSR
	BIC	r2, r1, #0x80	@ Clear I
	ORR	r0, r2, r0	@ Restore our saved I
	TEQ	r0, r1
	MSRNE	CPSR_c, r0	@ Update PSR if IRQ state needs changing
	MOV	pc, lr

	.globl flush_icache
flush_icache:
	MOV	r0, #0
	MCR	p15, 0, r0, c7, c5, 0	@ Invalidate instruction cache
	NOP
	MOV	pc, lr

	.globl flush_caches
flush_caches:
	MOV	r12, lr
	MOV	r0, #0x80000001		@ Flush caches
	SWI	XOS_MMUControl
	MOV	pc, r12

	.globl cpuconfig
cpuconfig:
	TEQ	r0, r0
	TEQ	pc, pc
	MOVEQ	r0, #32
	MOVNE	r0, #26
	MOV	pc, lr

