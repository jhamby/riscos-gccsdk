;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/Attic/_swi.s,v $
; $Date: 2001/08/07 09:08:51 $
; $Revision: 1.1.2.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	; This code is modified from the Straylight sources.
	; Straylight is distributed under the GNU General Public License


	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

; --- _swi, _swix ---
;
; On entry:	r0 == SWI number
;		r1 == feature flags:
;			 0--9  == input registers
;			  11   == local block flag
;			12--15 == local block register
;			16--19 == return register (_swi only)
;			31--22 == output registers
;		r2, r3 and stack contain other arguments
;
; On exit:	r0 == return value, or error indicator
;
; Use:		Calls a SWI or assembler routine.

	EXPORT	|_swi|
	EXPORT	|_swix|

	; --- How this works ---
	;
	; The old version of this code used to build some neat code
	; on the stack and then execute it.  This new spiffy version
	; just saves data on the stack, because building code is a
	; major no-no on the StrongARM.
	;
	; The data stacked is as follows:
	;
	;	  pc   -- address to call
	;	  r14  -- return address
	;	  r12  -- saved r12 value
	;	  r11  -- maybe Sapphire application context
	;	  r10  -- maybe the SWI number
	;
	; Because I can't use dynamic code at all, I'm having to
	; use some really nasty speed hacks here.

	; --- SWI entries ---



|_swi|
	STMFD	sp!, {r2, r3}		;Stack all variable args
	STMFD	sp!, {r4 - r12, r14}	;Save other registers
	ADR	r12, |_swi_nonx|	;Point to non-X entry point
	MOV	r9, r0			;Fetch the SWI number
	LDR	r0, |_swihack|		;Point to SWI calling routine
	B	|_swi_main|		;Skip onwards to main code

|_swix|
	STMFD	sp!,{r2,r3}		;Stack all variable args
	STMFD	sp!,{r4-r12,r14}	;Save other registers
	BIC	r1,r1,#&000F0000	;Return r0 value no matter
	ADR	r12,|_swi_x|		;Point to X entry point
	ORR	r9,r0,#&20000		;Set the X bit on the SWI
	LDR	r0,|_swihack|		;Point to SWI calling routine
	B	|_swi_main|		;Skip onwards to main code


	; --- First job is to set up the call address ---

|_swi_main|
	MOV	r14,pc			;Get the current CPU flags
	AND	r14,r14,#&0C000003	;Leave interrupts and mode
	ORR	r12,r12,r14		;Set the return address
	ORR	r14,r0,r14		;And the call address
	SUB	sp,sp,#8		;Make a hole in the stack
	STMFD	sp!,{r9-r12,r14}	;Save r10-r12 and pc (fake)

	; --- Set up the input registers ---
	;
	; Unrolled loop to do two registers at a time.  There are
	; frequent exits while scanning the early registers to
	; speed up common cases, petering out towards the end.

	MOV	r10,r1			;Fetch the feature flags
	ADD	r12,sp,#68		;Point to arguments

	MOVS	r14,r10,LSL #31
	LDRMI	r0,[r12],#4
	LDRCS	r1,[r12],#4
	TST	r10,#&3FC
	BEQ	|done|
	MOVS	r14,r10,LSL #29
	LDRMI	r2,[r12],#4
	LDRCS	r3,[r12],#4
	TST	r10,#&3F0
	BEQ	|done|
	MOVS	r14,r10,LSL #27
	LDRMI	r4,[r12],#4
	LDRCS	r5,[r12],#4
	TST	r10,#&3C0
	BEQ	|done|
	MOVS	r14,r10,LSL #25
	LDRMI	r6,[r12],#4
	LDRCS	r7,[r12],#4
	MOVS	r14,r10,LSL #23
	LDRMI	r8,[r12],#4
	LDRCS	r9,[r12],#4
|done|
	; --- Now sort out block arguments ---

	ADD	r14,sp,#20		;Find the hole in the stack
	STMIA	r14,{r10,r12}		;Save important context
	TST	r10,#&800		;Do we have a block argument?
	BNE	|_swi_block|		;Yes -- sort out out-of-line
	LDMFD	sp!,{r10-r12,r14,pc}^	;And call the routine

		; --- X-type return ---

|_swi_x|
	LDMFD	sp!,{r10,r12}		;Reload important context
	MOVVC	r14,#0			;If no error, return zero
	MOVVS	r14,r0			;Otherwise point to the error
	STR	r14,[sp,#-4]!		;Store as return value
	B	|_swi_output|		;And skip on a little

	; --- Non-X-type return ---
	;
	; Pick out the correct register with a branch table.  Also
	; invert the table to pick out common case of return r0.

|_swi_nonx|
	LDMFD	sp!,{r10,r12}		;Reload important context
	MOV	r14,#&F			;A nice bitmask
	AND	r14,r14,r10,LSR #16	;So mask the return register
	RSB	r14,r14,#&F		;Invert range hackily
	ADD	pc,pc,r14,LSL #3	;And dispatch nicely
	DCB	"hack"

	STR	pc,[sp,#-4]!
	B	|_swi_output|
	STR	r14,[sp,#-4]!
	B	|_swi_output|
	STR	sp,[sp,#-4]!
	B	|_swi_output|
	STR	r12,[sp,#-4]!
	B	|_swi_output|
	STR	r11,[sp,#-4]!
	B	|_swi_output|
	STR	r10,[sp,#-4]!
	B	|_swi_output|
	STR	r9,[sp,#-4]!
	B	|_swi_output|
	STR	r8,[sp,#-4]!
	B	|_swi_output|
	STR	r7,[sp,#-4]!
	B	|_swi_output|
	STR	r6,[sp,#-4]!
	B	|_swi_output|
	STR	r5,[sp,#-4]!
	B	|_swi_output|
	STR	r4,[sp,#-4]!
	B	|_swi_output|
	STR	r3,[sp,#-4]!
	B	|_swi_output|
	STR	r2,[sp,#-4]!
	B	|_swi_output|
	STR	r1,[sp,#-4]!
	B	|_swi_output|
	STR	r0,[sp,#-4]!

	; --- Now handle output parameters ---
	;
	; Same style as the input parameters, with early exits
	; placed conveniently.
|_swi_output|
	MOV	r11,pc			;Get the CPU flags

	TST	r10,#&FF000000		;Are there any output args?
	TSTEQ	r10,#&00E00000
	BEQ	swiout10		;No -- skip onwards then

	MOVS	r14,r10,LSL #1
	LDRCS	r14,[r12],#4
	STRCS	r0,[r14,#0]
	LDRMI	r14,[r12],#4
	STRMI	r1,[r14,#0]
	MOVS	r14,r10,LSL #3
	LDRCS	r14,[r12],#4
	STRCS	r2,[r14,#0]
	LDRMI	r14,[r12],#4
	STRMI	r3,[r14,#0]
	TST	r10,#&0FC00000
	BEQ	swiout00
	MOVS	r14,r10,LSL #5
	LDRCS	r14,[r12],#4
	STRCS	r4,[r14,#0]
	LDRMI	r14,[r12],#4
	STRMI	r5,[r14,#0]
	TST	r10,#&03C00000
	BEQ	swiout00
	MOVS	r14,r10,LSL #7
	LDRCS	r14,[r12],#4
	STRCS	r6,[r14,#0]
	LDRMI	r14,[r12],#4
	STRMI	r7,[r14,#0]
	MOVS	r14,r10,LSL #9
	LDRCS	r14,[r12],#4
	STRCS	r8,[r14,#0]
	LDRMI	r14,[r12],#4
	STRMI	r9,[r14,#0]
swiout00
	; --- Handle returning flags ---

	TST	r10,#&00200000
	LDRNE	r14,[r12],#4
	STRNE	r11,[r14,#0]
swiout10
	LDMFD	sp!,{r0,r4-r12,r14}
	ADD	sp,sp,#8
	MOVS	pc,r14

	; --- Handle block arguments ---
	;
	; Shift output registers to the right to find the block.
	; Then dispatch through a branch table to store in the right
	; register.  All the registers from r10 upwards are on the
	; stack so they can be restored easily.

|_swi_block|
	MOV	r11,r10,LSR #22		;Mask off output registers
	MOV	r11,r11,LSL #21		;Shift down one place
	MOV	r14,r12			;Preserve r12 here
swibl00	MOVS	r11,r11,LSL #2		;Shift into C and N flags
	ADDCS	r14,r14,#4		;If C set, bump counter
	ADDMI	r14,r14,#4		;If N set, bump counter
	BNE	swibl00			;And loop back until done
	AND	r11,r10,#&0000F000	;Fetch the right argument
	ADD	pc,pc,r11,LSR #9	;Dispatch through branch tbl
	DCB	"hack"

	; --- Main dispatch table ---
	;
	; This is now just a branch off the main routine, so I
	; can just call the SWI/routine appropriately rather than
	; returning to the caller.  This gives me an extra register
	; to play with above, which helps.

	MOV	r0,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r1,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r2,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r3,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r4,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r5,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r6,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r7,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r8,r14
	LDMFD	sp!,{r10-r12,r14,pc}^
	MOV	r9,r14
	LDMFD	sp!,{r10-r12,r14,pc}^

	; --- For safety, handle daft values of the parameter ---

	LDMFD	sp!,{r10-r12,r14,pc}^
	DCB	"daft"
	LDMFD	sp!,{r10-r12,r14,pc}^
	DCB	"daft"
	LDMFD	sp!,{r10-r12,r14,pc}^
	DCB	"daft"
	LDMFD	sp!,{r10-r12,r14,pc}^
	DCB	"daft"
	LDMFD	sp!,{r10-r12,r14,pc}^
	DCB	"daft"
	LDMFD	sp!,{r10-r12,r14,pc}^
	DCB	"daft"

|_swihack|
	DCD	|__swihack|

	; --- Initial sorting out and dispatching routine ---

|__swihack|
	STMFD	r13!,{r0,r1,r14}	;Store some registers away
	MOV	r0,#0			;Read feature flags
	SWI	XOS_PlatformFeatures	;Read the features then
	ADRVC	r0,|_swihack_call|	;If SWI there, call directly
	ADRVS	r0,|_swihack_build|	;Otherwise build the call
	STR	r0,|_swihack|		;Store the value away
	LDMFD	r13!,{r0,r1,r14}	;Restore the registers
	TEQP	r14,#0			;Reset flags from r14
	LDR	pc,|_swihack|		;And snap the pointer

	; --- Dispatch to OS_CallASWI ---

|_swihack_call|
	MOV	r12,r14			;Save the return address
	SWI	&6F			;OS_CallASWI
|_ret_r12|
	MOV	pc,r12			;And return to caller

	; --- Dispatch by building code dynamically ---

|_swihack_build|
	STMFD	r13!,{r14}		;Save the return address
	BIC	r10,r10,#&FF000000	;Clear the opcode byte
	ORR	r10,r10,#&EF000000	;And make it SWIAL
	LDR	r14,|_ret_r12|		;And load the return instr
	STMFD	r13!,{r10,r14}		;Save code on the stack
	MOV	r12,pc			;Set up return address
	MOV	pc,r13			;And call the code
	ADD	r13,r13,#8		;Reclaim stack space
	LDMFD	r13!,{pc}		;And return with SWI's flags

	END

