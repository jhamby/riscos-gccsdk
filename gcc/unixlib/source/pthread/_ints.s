;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/_ints.s,v $
; $Date: 2004/09/23 22:16:39 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

; Enable and disable the callevery interrupt from causing context switches
; disable_ints can be called multiple times, provided enable_ints is
; subsequently called an equal number of times

; Written by Martin Piper and Alex Waugh

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__pthread_fatal_error|
	IMPORT	|__pthread_callback_missed|
	IMPORT	|pthread_yield|
	IMPORT	|__pthread_callback_semaphore|

	EXPORT	|__pthread_disable_ints|
	EXPORT	|__pthread_enable_ints|
	EXPORT	|__pthread_protect_unsafe|

	EXPORT	|__pthread_worksemaphore|

;
; Disable context switches by incrementing the semaphore
; May be called from USR or SVC mode
	NAME	__pthread_disable_ints
|__pthread_disable_ints|
	LDR	a1, =|__pthread_worksemaphore|
	MOV	a3, #1
	swp_arm2	a2, a3, a1, a2	; From this point onwards we will not be interrupted by the callback
	ADD	a2, a2, #1
	STR	a2, [a1]
	MOV	pc, lr


;
; Decrement the semaphore, thus enabling context switches if it reaches 0
; May be called from USR or SVC mode
	NAME	__pthread_enable_ints
|__pthread_enable_ints|
	LDR	a2, =|__pthread_worksemaphore|
	LDR	a1, [a2]
	[ __UNIXLIB_PARANOID > 0
	CMP	a1, #0
	ADRLE	a1, |semazero|
	BLE	|__pthread_fatal_error|
	]
	SUB	a1, a1, #1
	STR	a1, [a2]
	MOV	pc, lr

	[ __UNIXLIB_PARANOID > 0
|semazero|
	DCB	"__pthread_enable_ints called with semaphore already 0", 0
	ALIGN
	]

; Similar to __pthread_disable_ints, but alters the caller's
; stack frame so __pthread_unprotect_unsafe is called
; when the caller returns
	NAME	__pthread_protect_unsafe
|__pthread_protect_unsafe|
	[ __UNIXLIB_PARANOID > 0
	CMP	fp, #0
	ADREQ	a1, |noframe|
	BEQ	|__pthread_fatal_error|	; We can't do much without a stack frame
	]
	LDR	a1, =|__pthread_worksemaphore|
	MOV	a2, #1
	swp_arm2	a3, a2, a1, a3	; From this point onwards we cannot be interrupted by the callback
	CMP	a3, #0
	STRNE	a3, [a1]	; Restore original value
	; Return, as if ints are disabled on entry to the
	; calling function then they should not be reenabled
	; until the calling function has returned
	MOVNE	pc, lr

	LDR	a2, =|__pthread_return_address|
	[ __UNIXLIB_PARANOID > 0
	LDR	a3, [a2]
	CMP	a3, #0
	ADRNE	a1, |return_notempty|
	BNE	|__pthread_fatal_error|
	]
	LDR	a1, [fp, #-4]	; Load calling function's return address
	STR	a1, [a2]	; Save it
	ADR	a2, |__pthread_unprotect_unsafe|
	TEQ	a1, a1
	TEQ	pc, pc
	ANDNE	a1, a1, #&fc000003	; Make sure the flags and mode bits of
	ORRNE	a2, a1, a2		; pc are preserved when in 26bit mode
	; Alter calling function's return address to point to __pthread_unprotect_unsafe
	STR	a2, [fp, #-4]

	MOV	pc, lr

	[ __UNIXLIB_PARANOID > 0
|noframe|
	DCB "__pthread_protect_unsafe called without an APCS stack frame" ,0
	ALIGN
|return_notempty|
	DCB	"Return address is not empty in __pthread_protect_unsafe", 0
	ALIGN
	]


; Similar to __pthread_enable_ints, but return to the saved __pthread_return_address
; Can corrupt a3-a4,ip,lr but NOT a1 or a2
	NAME	__pthread_unprotect_unsafe
|__pthread_unprotect_unsafe|
	LDR	a4, =|__pthread_return_address|
	LDR	lr, [a4]
	[ __UNIXLIB_PARANOID > 0
	CMP	lr, #0
	ADREQ	a1, |return_unset|
	BEQ	|__pthread_fatal_error|
	MOV	a3, #0
	STR	a3, [a4]
	]

	LDR	a4, =|__pthread_worksemaphore|
	[ __UNIXLIB_PARANOID > 0
	LDR	a3, [a4]
	CMP	a3, #1
	ADRNE	a1, |bad_semaphore|
	BNE	|__pthread_fatal_error|
	]
	MOV	a3, #0
	STR	a3, [a4]

	LDR	a4, =|__pthread_callback_missed|
	LDR	a4, [a4]
	CMP	a4, #0
	MOVEQ	pc, lr	;No callback occured while ints were disabled

	LDR	a4, =|__pthread_callback_semaphore|
	LDR	a4, [a4]
	CMP	a4, #0
	MOVNE	pc, lr	;Don't yield if we're in the middle of a context switch

	; An callback occured whilst ints were disabled,
	; so yield to avoid hogging the processor
	STMFD	sp!, {a1, a2, lr}
	BL	|pthread_yield|
	LDMFD	sp!, {a1, a2, pc}

	[ __UNIXLIB_PARANOID > 0
|bad_semaphore|
	DCB	"Semaphore not set correctly in __pthread_unprotect_unsafe", 0
	ALIGN
|return_unset|
	DCB	"Return address unset in __pthread_unprotect_unsafe", 0
	ALIGN
	]


	AREA	|C$$data|, DATA

|__pthread_return_address|
	DCD	0
|__pthread_worksemaphore|
	DCD	0

	END
