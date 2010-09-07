	@ Globals in this file are written back into the module itself.
	@ This will prevent it being put into ROM. If that is required, then
	@ this file (and this file alone) will need to be reworked to store
	@ its globals somewhere else (i.e. in zero page)

	@ Current session, or NULL if none active
current_session:
	.word	0

	@ Retrieve the current session
	.globl session_get_current
session_get_current:
	LDR	r0, current_session
	MOV	pc, lr

	@ Set the current session
	.globl session_set_current
session_set_current:
	STR	r0, current_session
	MOV	pc, lr

@ --------------------------------------------------------------------

	@ Previous SWI vector handler
prev_swi_vector:
	.word	0

	@ Get previous SWI vector handler
	.globl get_prev_swi_handler
get_prev_swi_handler:
	LDR	r0, prev_swi_vector
	MOV	pc, lr

	@ Set previous SWI vector handler
	.globl set_prev_swi_handler
set_prev_swi_handler:
	STR	r0, prev_swi_vector
	MOV	pc, lr

	@ SWI vector handler
	.globl swi_handler
swi_handler:
	STMFD	r13!, {r0-r12, r14}
	MRS	r1, SPSR
	STR	r1, [r13, #-4]!		@ Save SPSR

	ANDS	r1, r1, #0xf		@ not called from USR => bail
	MOVNE	r0, #0
	LDREQ	r0, current_session
	TEQ	r0, #0			@ current_session == NULL => bail
	BEQ	1f

	SUB	r13, r13, #8
	STMIA	r13, {r13, r14}^	@ Save USR r13 + r14
	NOP

	STR	r14, [r0]		@ save return address into session

	LDR	r1, [r14, #-4]		@ read SWI instruction
	BIC	r1, r1, #0xff000000	@ clear uninteresting bits
	BIC	r1, r1, #0x00020000	@ clear X
	MOV	r2, #0x40000
	ORR	r2, r2, #0xc0
	TEQ	r1, r2			@ (X)Wimp_Initialise?
	@ r0 -> current_session
	BLEQ	session_wimp_initialise

	LDMIA	r13, {r13, r14}^	@ Restore USR r13 + r14
	ADD	r13, r13, #8

1:
	LDR	r1, [r13], #4
	MSR	SPSR, r1		@ Restore SPSR
	LDMIA	r13!, {r0-r12, r14}
	LDR	pc, prev_swi_vector	@ pass on to previous claimant

@ --------------------------------------------------------------------

	@ Previous IRQ vector handler
prev_irq_vector:
	.word	0

	@ Get previous IRQ vector handler
	.globl get_prev_irq_handler
get_prev_irq_handler:
	LDR	r0, prev_irq_vector
	MOV	pc, lr

	@ Set previous IRQ vector handler
	.globl set_prev_irq_handler
set_prev_irq_handler:
	STR	r0, prev_irq_vector
	MOV	pc, lr

	@ IRQ vector handler
	.globl irq_handler
irq_handler:
	STMFD	r13!, {r0, r1}
	MRS	r1, SPSR
	ANDS	r1, r1, #0xf		@ not from USR => bail
	MOVNE	r0, #0
	LDREQ	r0, current_session
	TEQ	r0, #0			@ current_session == NULL => bail
	SUBNE	r1, r14, #4
	STRNE	r1, [r0]		@ save return address into session
	LDMIA	r13!, {r0, r1}
	LDR	pc, prev_irq_vector	@ pass on to previous claimant

@ --------------------------------------------------------------------

	@ Previous data abort handler
prev_dabort_vector:
	.word 0

	@ Get previous data abort vector handler
	.globl get_prev_dabort_handler
get_prev_dabort_handler:
	LDR	r0, prev_dabort_vector
	MOV	pc, lr

	@ Set previous data abort vector handler
	.globl set_prev_dabort_handler
set_prev_dabort_handler:
	STR	r0, prev_dabort_vector
	MOV	pc, lr

	@ Data abort vector handler
	.globl dabort_handler
dabort_handler:
	STMFD	r13!, {r0, r1, r14}
	LDR	r0, current_session
	TEQ	r0, #0
	LDMEQIA	r13!, {r0, r1, r14}
	LDREQ	pc, prev_dabort_vector	@ current_session == NULL => bail

	STMIB	r0, {r0-r14}^		@ Save USR mode's r0-r14
	NOP
	LDR	r1, [r13]
	STR	r1, [r0, #4]		@ Fix up saved r0

	SUB	r1, r14, #8		@ Retrieve aborted PC
	STR	r1, [r0, #16*4]		@ Save it

	MRS	r1, SPSR
	STR	r1, [r0, #17*4]		@ Save SPSR

	@ If the abort was from a privileged mode, replace the saved 
	@ USR mode r13/r14 with the ones from the appropriate mode.
	ANDS	r14, r1, #0xf		@ Extract mode bits from SPSR
	ADDNE	r0, r0, #14*4		@ r0 -> save_area[r13]
	MRSNE	r1, CPSR
	BICNE	r1, r1, #0xf
	ORRNE	r14, r1, r14
	MSRNE	CPSR_c, r14		@ Switch to privileged mode
	NOPNE
	STMNEIA	r0, {r13, r14}		@ Save r13 + r14
	ORRNE	r1, r1, #0x7
	MSRNE	CPSR_c, r1		@ Back to ABT mode
	SUBNE	r0, r0, #14*4
	
	@ TODO: Save FP regs?

	@ Enter SVC + I
	MRS	r1, CPSR
	BIC	r14, r1, #0x8f		@ Clear bottom 4 mode bits + I
	ORR	r14, r14, #0x3		@ SVC
	MSR	CPSR_c, r14

	@ Call break handler -- APCS calling conventions
	STMFD	r13!, {r0-r3, ip, r14}
	@ r0 -> current_session
	MOV	r1, #0			@ Data abort
	BL	session_handle_break
	LDMIA	r13!, {r0-r3, ip, r14}

	@ Return to ABT + ~I
	MSR	CPSR_c, r1

	@ Restore aborted mode registers from save block
	LDR	r0, current_session
	TEQ	r0, #0
	BEQ	.			@ Should never happen

	@ TODO: Load FP regs?

	LDR	r1, [r0, #17*4]		@ Load SPSR
	MSR	SPSR, r1

	LDR	r14, [r0, #16*4]	@ Load PC to resume at
	STR	r14, [r13, #2*4]	@ Save it for later

	ADD	r0, r0, #14*4		@ r0 -> save_area[r13]
	ANDS	r14, r1, #0xf
	LDMEQIA	r0, {r13, r14}^		@ Load USR r13-r14
	MRSNE	r1, CPSR
	BICNE	r1, r1, #0xf
	ORRNE	r14, r1, r14
	MSRNE	CPSR_c, r14		@ Switch to privileged mode
	NOPNE
	LDMNEIA	r0, {r13, r14}		@ Load r13 + r14
	ORRNE	r1, r1, #0x7
	MSRNE	CPSR_c, r1		@ Back to ABT mode
	SUB	r0, r0, #14*4		@ r0 -> session_ctx
	LDMIB	r0, {r0-r12}		@ Load general registers

	ADD	r13, r13, #2*4		@ Balance stack
	LDR	r14, [r13], #4		@ Retrieve return address

	MOVS	pc, r14			@ Return to sender

@ --------------------------------------------------------------------

	@ Previous prefetch abort handler
prev_prefetch_vector:
	.word 0

	@ Get previous prefetch abort vector handler
	.globl get_prev_prefetch_handler
get_prev_prefetch_handler:
	LDR	r0, prev_prefetch_vector
	MOV	pc, lr

	@ Set previous prefetch abort vector handler
	.globl set_prev_prefetch_handler
set_prev_prefetch_handler:
	STR	r0, prev_prefetch_vector
	MOV	pc, lr

	@ Prefetch abort vector handler
	.globl prefetch_handler
prefetch_handler:
	STMFD	r13!, {r0, r1, r14}
	LDR	r0, current_session
	TEQ	r0, #0
	LDMEQIA	r13!, {r0, r1, r14}
	LDREQ	pc, prev_prefetch_vector@ current_session == NULL => bail

	STMIB	r0, {r0-r14}^		@ Save USR mode's r0-r14
	NOP
	LDR	r1, [r13]
	STR	r1, [r0, #4]		@ Fix up saved r0

	SUB	r1, r14, #4		@ Retrieve aborted PC
	STR	r1, [r0, #16*4]		@ Save it

	MRS	r1, SPSR
	STR	r1, [r0, #17*4]		@ Save SPSR

	@ If the abort was from a privileged mode, replace the saved 
	@ USR mode r13/r14 with the ones from the appropriate mode.
	ANDS	r14, r1, #0xf		@ Extract mode bits from SPSR
	ADDNE	r0, r0, #14*4		@ r0 -> save_area[r13]
	MRSNE	r1, CPSR
	BICNE	r1, r1, #0xf
	ORRNE	r14, r1, r14
	MSRNE	CPSR_c, r14		@ Switch to privileged mode
	NOPNE
	STMNEIA	r0, {r13, r14}		@ Save r13 + r14
	ORRNE	r1, r1, #0x7
	MSRNE	CPSR_c, r1		@ Back to ABT mode
	SUBNE	r0, r0, #14*4
	
	@ TODO: Save FP regs?

	@ Enter SVC + I
	MRS	r1, CPSR
	BIC	r14, r1, #0x8f		@ Clear bottom 4 mode bits + I
	ORR	r14, r14, #0x3		@ SVC
	MSR	CPSR_c, r14

	@ Call break handler -- APCS calling conventions
	STMFD	r13!, {r0-r3, ip, r14}
	@ r0 -> current_session
	MOV	r1, #1			@ Prefetch abort
	BL	session_handle_break
	LDMIA	r13!, {r0-r3, ip, r14}

	@ Return to ABT + ~I
	MSR	CPSR_c, r1

	@ Restore aborted mode registers from save block
	LDR	r0, current_session
	TEQ	r0, #0
	BEQ	.			@ Should never happen

	@ TODO: Load FP regs?

	LDR	r1, [r0, #17*4]		@ Load SPSR
	MSR	SPSR, r1

	LDR	r14, [r0, #16*4]	@ Load PC to resume at
	STR	r14, [r13, #2*4]	@ Save it for later

	ADD	r0, r0, #14*4		@ r0 -> save_area[r13]
	ANDS	r14, r1, #0xf
	LDMEQIA	r0, {r13, r14}^		@ Load USR r13-r14
	MRSNE	r1, CPSR
	BICNE	r1, r1, #0xf
	ORRNE	r14, r1, r14
	MSRNE	CPSR_c, r14		@ Switch to privileged mode
	NOPNE
	LDMNEIA	r0, {r13, r14}		@ Load r13 + r14
	ORRNE	r1, r1, #0x7
	MSRNE	CPSR_c, r1		@ Back to ABT mode
	SUB	r0, r0, #14*4		@ r0 -> session_ctx
	LDMIB	r0, {r0-r12}		@ Load general registers

	ADD	r13, r13, #2*4		@ Balance stack
	LDR	r14, [r13], #4		@ Retrieve return address

	MOVS	pc, r14			@ Return to sender

@ --------------------------------------------------------------------

	@ Previous undefined instruction abort handler
prev_undef_vector:
	.word 0

	@ Get previous undefined instruction abort vector handler
	.globl get_prev_undef_handler
get_prev_undef_handler:
	LDR	r0, prev_undef_vector
	MOV	pc, lr

	@ Set previous undefined instruction abort vector handler
	.globl set_prev_undef_handler
set_prev_undef_handler:
	STR	r0, prev_undef_vector
	MOV	pc, lr

	@ Undefined instruction abort vector handler
	.globl undef_handler
undef_handler:
	STMFD	r13!, {r0, r1, r14}
	LDR	r0, current_session
	TEQ	r0, #0
	LDMEQIA	r13!, {r0, r1, r14}
	LDREQ	pc, prev_undef_vector	@ current_session == NULL => bail

	LDR	r0, [r14, #-4]		@ Get aborted instruction
	AND	r1, r0, #0x0f000000
	TEQ	r1, #0x0f000000		@ Clears C
	CMPNE	r1, #0x0c000000		@ Sets C if >= 0x0c000000
	BCC	1f			@ Not coprocessor, handle it
	AND	r0, r0, #0x00000f00
	TEQ	r0, #0x00000100
	TEQNE	r0, #0x00000200		@ Coprocessor 1/2 => FPA
	LDMEQIA	r13!, {r0, r1, r14}
	LDREQ	pc, prev_undef_vector	@ Pass it on to FPEmulator
	
1:
	LDR	r1, [r13, #4]		@ Restore aborted r1

	STMIB	r0, {r0-r14}^		@ Save USR mode's r0-r14
	NOP
	LDR	r1, [r13]
	STR	r1, [r0, #4]		@ Fix up saved r0

	SUB	r1, r14, #4		@ Retrieve aborted PC
	STR	r1, [r0, #16*4]		@ Save it

	MRS	r1, SPSR
	STR	r1, [r0, #17*4]		@ Save SPSR

	@ If the abort was from a privileged mode, replace the saved 
	@ USR mode r13/r14 with the ones from the appropriate mode.
	ANDS	r14, r1, #0xf		@ Extract mode bits from SPSR
	ADDNE	r0, r0, #14*4		@ r0 -> save_area[r13]
	MRSNE	r1, CPSR
	BICNE	r1, r1, #0xf
	ORRNE	r14, r1, r14
	MSRNE	CPSR_c, r14		@ Switch to privileged mode
	NOPNE
	STMNEIA	r0, {r13, r14}		@ Save r13 + r14
	ORRNE	r1, r1, #0xb
	MSRNE	CPSR_c, r1		@ Back to UND mode
	SUBNE	r0, r0, #14*4
	
	@ TODO: Save FP regs?

	@ Enter SVC + I
	MRS	r1, CPSR
	BIC	r14, r1, #0x8f		@ Clear bottom 4 mode bits + I
	ORR	r14, r14, #0x3		@ SVC
	MSR	CPSR_c, r14

	@ Call break handler -- APCS calling conventions
	STMFD	r13!, {r0-r3, ip, r14}
	@ r0 -> current_session
	MOV	r1, #2			@ Undefined instruction abort
	BL	session_handle_break
	LDMIA	r13!, {r0-r3, ip, r14}

	@ Return to UND + ~I
	MSR	CPSR_c, r1

	@ Restore aborted mode registers from save block
	LDR	r0, current_session
	TEQ	r0, #0
	BEQ	.			@ Should never happen

	@ TODO: Load FP regs?

	LDR	r1, [r0, #17*4]		@ Load SPSR
	MSR	SPSR, r1

	LDR	r14, [r0, #16*4]	@ Load PC to resume at
	STR	r14, [r13, #2*4]	@ Save it for later

	ADD	r0, r0, #14*4		@ r0 -> save_area[r13]
	ANDS	r14, r1, #0xf
	LDMEQIA	r0, {r13, r14}^		@ Load USR r13-r14
	MRSNE	r1, CPSR
	BICNE	r1, r1, #0xf
	ORRNE	r14, r1, r14
	MSRNE	CPSR_c, r14		@ Switch to privileged mode
	NOPNE
	LDMNEIA	r0, {r13, r14}		@ Load r13 + r14
	ORRNE	r1, r1, #0xb
	MSRNE	CPSR_c, r1		@ Back to UND mode
	SUB	r0, r0, #14*4		@ r0 -> session_ctx
	LDMIB	r0, {r0-r12}		@ Load general registers

	ADD	r13, r13, #2*4		@ Balance stack
	LDR	r14, [r13], #4		@ Retrieve return address

	MOVS	pc, r14			@ Return to sender

@ --------------------------------------------------------------------

.set XOS_EnterOS, 0x20016
.set OS_GenerateError, 0x2b
.set OS_Exit, 0x11

	@ Error handler
	.globl error_veneer
error_veneer:
	STR	r12, [r13, #-4]!
	LDR	r12, [r0, #18*4]	@ Retrieve module private word
	SWI	XOS_EnterOS		@ Up into SVC mode
	BL	apperror		@ Call handler (r0->ctx, r12->pw)
	MRS	r12, CPSR
	BIC	r12, r12, #0xf
	MSR	CPSR_c, r12		@ Back into USR mode
	NOP
	LDR	r12, [r13], #4
	SWI	OS_GenerateError	@ Exit, generating error

@ --------------------------------------------------------------------

	@ Exit handler
	.globl exit_veneer
exit_veneer:
	STMFD	r13!, {r0, r12}
	MOV	r0, r12
	LDR	r12, [r12, #18*4]	@ Retrieve module private word
	SWI	XOS_EnterOS		@ Up into SVC mode
	BL	appexit			@ Call handler (r0->ctx, r12->pw)
	MRS	r12, CPSR
	BIC	r12, r12, #0xf
	MSR	CPSR_c, r12		@ Back into USR mode
	LDMIA	r13!, {r0, r12}
	SWI	OS_Exit			@ Exit

@ --------------------------------------------------------------------

	@ Upcall handler
	.globl upcall_veneer
upcall_veneer:
	TEQ	r0, #0x100		@ Only interested in New Application
	MOVNE	pc, lr

	STMFD	r13!, {r0, r12, r14}
	MOV	r0, r12
	LDR	r12, [r12, #18*4]	@ Retrieve module private word
	BL	appupcall		@ Call handler (r0->ctx, r12->pw)
	LDMIA	r13!, {r0, r12, r14}
	MOV	pc, lr			@ Return to caller
