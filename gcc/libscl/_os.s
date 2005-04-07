; SWI call routine taken from UnixLib for compatibility with riscosify()

XOS_CallASWIR12                 EQU     &020071

	AREA	|C$$Code|, CODE, READONLY
	
	EXPORT	|__os_swi|
|__os_swi|
	STMFD	sp!, {a2, v1, v2, v3, v4, v5, v6, lr}
	ORR	ip, a1, #&20000		; X bit
	TEQ	a2, #0
	LDMNEIA a2, {a1, a2, a3, a4, v1, v2, v3, v4, v5, v6}
	SWI	XOS_CallASWIR12
	LDMFD	sp!, {lr}		; Retrieve passed in a2
	TEQ	lr, #0
	STMNEIA lr, {a1, a2, a3, a4, v1, v2, v3, v4, v5, v6}
	MOVVC	a1, #0
	LDMFD	sp!, {v1, v2, v3, v4, v5, v6, pc}

	END
