@ __unixlib_exec_sig, __unixlib_exec_sigstack, __unixlib_exec_sigstack_bsd
@ Written by Nick Burrett, 1 September 1996.
@ Copyright (c) 1996-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ Low level functions used to setup stack frames and call the
	@ signal handler. These are called by sigsetup (signal/post.c).

	@ Setup a signal handler to execute on the current stack.
	@ void __unixlib_exec_sig (__sighandler_t, int, siginfo_t *, void *)
	.global	__unixlib_exec_sig
	NAME	__unixlib_exec_sig
__unixlib_exec_sig:
	MOV	ip, sp
	STMFD	sp!, {v1, v2, v3, v4, v5, v6, fp, ip, lr, pc}
	SUB	fp, ip, #4
	@ Call the signal handler
	MOV	ip, a1
	MOV	a3, a4
	MOV	a2, a3
	MOV	a1, a2
	MOV	lr, pc
	MOV	pc, ip
	LDMEA	fp, {v1, v2, v3, v4, v5, v6, fp, sp, pc}


        @ Execute a signal handler on an alternative stack
	@ (sigaltstack for POSIX).
	.global	__unixlib_exec_sigstack
	NAME	__unixlib_exec_sigstack
__unixlib_exec_sigstack:
	STMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, lr}
	@ Preserve sp
	MOV	v1, sp
	@ Switch to the signal stack, setting it up for a proper
	@ APCS-32 stack frame.
	MOV	fp, #0
	ADD	sp, a1, a2
	@ Pad the stack chunk limit in case we run out of room.
	ADD	sl, a1, #512
	@ Call the signal handler
	MOV	a1, a4
	MOV	lr, pc
	MOV	pc, a3
	MOV	sp, v1
	LDMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, pc}

	@ Execute a signal handler on an alternative stack
	@ (sigstack for BSD).
	.global	__unixlib_exec_sigstack_bsd
	NAME	__unixlib_exec_sigstack_bsd
__unixlib_exec_sigstack_bsd:
	STMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, lr}
	@ Preserve sp
	MOV	v1, sp
	@ Switch to the signal stack, setting it up for a proper
	@ APCS-32 stack frame.
	MOV	fp, #0
	MOV	sp, a1
	@ Under BSD, we don't know the size of the signal stack. Let's
	@ make up a value and hope for the best.
	SUB	sl, sp, #16384
	@ Call the signal handler
	MOV	a1, a3
	MOV	lr, pc
	MOV	pc, a2
	MOV	sp, v1
	LDMFD	sp!, {a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, pc}

	.end
