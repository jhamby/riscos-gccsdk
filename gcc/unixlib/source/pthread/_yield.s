;----------------------------------------------------------------------------
;
; $Source: $
; $Date: $
; $Revision: $
; $State: $
; $Author: $
;
;----------------------------------------------------------------------------

; Written by Martin Piper and Alex Waugh

	GET	clib/unixlib/asm_dec.s

	AREA |C$$code|, CODE, READONLY

	IMPORT |__pthread_fatal_error|
	IMPORT |__pthread_callback_semaphore|
	IMPORT |__pthread_system_running|
	IMPORT |__pthread_worksemaphore|
	IMPORT |__pthread_callback_pending|

	EXPORT |pthread_yield|

;
; pthread_yield
; Called in USR mode by a thread wishing to give up the processor
	NAME pthread_yield
|pthread_yield|
	; Setup an APCS stack frame so this will appear in a backtrace
	MOV	ip, sp
	STMFD	sp!, {fp, ip, lr, pc}
	SUB	fp, ip, #4

	; If the thread system isn't running then yielding is pointless
	LDR	a1, =|__pthread_system_running|
	LDR	a2, [a1]
	CMP	a2, #0
	[ {CONFIG} = 26
	LDMEQDB	fp, {fp, sp, pc}^
	|
	LDMEQDB	fp, {fp, sp, pc}
	]

	; Check that a context switch can take place
	LDR	a1, =|__pthread_worksemaphore|
	LDR	a2, [a1]
	CMP	a2, #0
	ADRNE	a1, failmessage
	BLNE	|__pthread_fatal_error|

	; Set the semaphore to prevent anyone else setting a callback untill this one has completed
	LDR	a1, =|__pthread_callback_semaphore|
	MOV	a2, #0
	swp_arm2	a3, a2, a1, a3
	CMP	a3, #1
	BNE	skip_callback	; Don't set the callback if someone else already has (this shouldn't ever occur)

	; Record that we are expecting a callback
	MOV	a1, #1
	LDR	a2, =|__pthread_callback_pending|
	STR	a1, [a2]
	SWI	XOS_SetCallBack	; Set the OS callback flag. Not much we can do if this fails

skip_callback
	MOV	a1, #0
	MOV	a2, #1
	SWI	XOS_Byte	; Force a callback
	[ {CONFIG} = 26
	LDMDB	fp, {fp, sp, pc}^
	|
	LDMDB	fp, {fp, sp, pc}
	]

failmessage
	DCB	"pthread_yield called with context switching disabled", 0
	ALIGN


 END
