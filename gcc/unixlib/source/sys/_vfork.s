;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_vfork.s,v $
; $Date: 2003/04/05 12:16:34 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: alex $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__vfork|
	IMPORT	|__vexit|
	IMPORT	|__dynamic_num|
	IMPORT	|__real_break|
	IMPORT	|__pthread_system_running|
	IMPORT	|__pthread_atfork_callprepare|
	IMPORT	|__pthread_atfork_callparentchild|
	IMPORT	|__pthread_disable_ints|
	IMPORT	|__pthread_enable_ints|
	IMPORT	|__pthread_start_ticker|
	IMPORT	|__pthread_stop_ticker|

	IMPORT	setjmp
	IMPORT	longjmp

	EXPORT	vfork
	NAME	vfork
vfork
	STMFD	sp!, {lr}
	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BEQ	pthread_skip1
	BL	|__pthread_atfork_callprepare|
	; Child process only contins a single thread
	BL	|__pthread_stop_ticker|
pthread_skip1
	]
	BL	|__vfork|
	; If zero was returned, we will return -1
	CMP	a1, #0
	BEQ	return_fail
	LDMFD	sp!, {lr}
	; save lr for use when setjmp returns either immediately or
	; via longjmp as spawned program exits and current program
	; starts running again.
	LDR	a2, =|__saved_lr|
	STR	lr, [a2]
	[ __FEATURE_PTHREADS = 1
	; save __pthread_system_running, as the child process
	; will always set it to 0 on exit
	LDR	a3, =|__pthread_system_running|
	LDR	a2, =|__saved_pthread_system_running|
	LDR	a3, [a3]
	STR	a3, [a2]
	]
	BL	setjmp
	[ __FEATURE_PTHREADS = 1
	LDR	a2, =|__saved_pthread_system_running|
	LDR	a3, =|__pthread_system_running|
	LDR	a2, [a2]
	STR	a2, [a3]
	]
	LDR	a2, =|__saved_lr|
	TEQ	a1, #0
	LDR	lr, [a2]
	BNE	return_parent
	; Return from the vfork in the child process
	STMFD	sp!, {lr}
	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BEQ	pthread_skip2
	MOV	a1, #0
	BL	|__pthread_atfork_callparentchild|
pthread_skip2
	]
	MOV	a1, #0
	stackreturn	AL, "pc"

return_parent	; Return from the vfork in the parent process
	; Restore dynamic area size to size indicated by __real_break
	; which was the size of the area before the forked program
	; ran and extended the dynamic area. We can't do this anywhere
	; before this point (e.g., in the called program) since some
	; pointers may need copying down from the spawned program's area.
	STMFD	sp!,{a1,a2,a3,a4,v1,v2,v3,v4,v5,lr}
	LDR	a2, =|__dynamic_num|
	LDR	a2, [a2]
	CMN	a2, #1
	BEQ	return_parent2
	MOV	a1, #2
	SWI	XOS_DynamicArea
	MOVVC	a1, a2
	LDRVC	a2, =|__real_break|
	ADDVC	a3, a3, a4
	LDRVC	a2, [a2]
	SUBVC	a2, a2, a3
	SWIVC	XOS_ChangeDynamicArea
return_parent2
	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BEQ	pthread_skip3
	BL	|__pthread_start_ticker|
	MOV	a1, #1
	BL	|__pthread_atfork_callparentchild|
pthread_skip3
	]
	stackreturn	AL, "a1,a2,a3,a4,v1,v2,v3,v4,v5,pc"

return_fail
	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BEQ	pthread_skip4
	BL	|__pthread_start_ticker|
	MOV	a1, #1
	BL	|__pthread_atfork_callparentchild|
pthread_skip4
	]
	MVN	a1, #0
	stackreturn	AL, "pc"


	EXPORT	|__vret|
|__vret|
	BL	|__vexit|
	; The location of the child pid in jmp_buf
	LDR	a2, [a1, #60]
	B	longjmp

	AREA	|C$$zidata|, DATA, NOINIT
|__saved_lr|
	%	4
	[ __FEATURE_PTHREADS = 1
|__saved_pthread_system_running|
	%	4
	]

	END
