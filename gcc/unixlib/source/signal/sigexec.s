;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigexec.s,v $
; $Date: 2002/09/24 21:02:38 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------
; signal.s.sigexec: Written by Nick Burrett, 1 September 1996.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; Low level functions used to setup stack frames and call the
	; signal handler. These are called by __unixlib_setup_sighandler.

	; Setup a signal handler to execute on the current stack.
	EXPORT	|__unixlib_exec_sig|
	NAME	__unixlib_exec_sig
|__unixlib_exec_sig|
	MOV	ip, sp
	STMFD	sp!, {v1, v2, v3, v4, v5, v6, fp, ip, lr, pc}
	SUB	fp, ip, #4
	; Call the signal handler
	MOV	a3, a1
	MOV	a1, a2
	MOV	lr, pc
	MOV	pc, a3
	LDMEA	fp, {v1, v2, v3, v4, v5, v6, fp, sp, pc}


        ; Execute a signal handler on an alternative stack
	; (sigaltstack for POSIX).
	EXPORT	|__unixlib_exec_sigstack|
	NAME	__unixlib_exec_sigstack
|__unixlib_exec_sigstack|
	STMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, lr}
	; Preserve sp
	MOV	v1, sp
	; Switch to the signal stack, setting it up for a proper
	; APCS stack frame.
	MOV	fp, #0
	ADD	sp, a1, a2
	; Pad the stack chunk limit in case we run out of room.
	ADD	sl, a1, #512
	; Call the signal handler
	MOV	a1, a4
	MOV	lr, pc
	MOV	pc, a3
	MOV	sp, v1
	LDMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, pc}

	; Execute a signal handler on an alternative stack
	; (sigstack for BSD).
	EXPORT	|__unixlib_exec_sigstack_bsd|
	NAME	__unixlib_exec_sigstack_bsd
|__unixlib_exec_sigstack_bsd|
	STMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, lr}
	; Preserve sp
	MOV	v1, sp
	; Switch to the signal stack, setting it up for a proper
	; APCS stack frame.
	MOV	fp, #0
	MOV	sp, a1
	; Under BSD, we don't know the size of the signal stack. Let's
	; make up a value and hope for the best.
	SUB	sl, sp, #16384
	; Call the signal handler
	MOV	a1, a3
	MOV	lr, pc
	MOV	pc, a2
	MOV	sp, v1
	LDMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, pc}

	END
