; riscosify assembler weak handling
; Taken from UnixLib.

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__riscosify_control|, WEAK

	;int __get_riscosify_control (void)
	EXPORT	|__get_riscosify_control|
|__get_riscosify_control|
	LDR	a1, ctrl
	TEQ	a1, #0
	LDREQ	a1, ctrl_int
	[ {MODULE}={TRUE}
	LDREQ	a2, [sl, #-536]
	ADDEQ	a1, a1, a2
	]
	LDR	a1, [a1, #0]
	MOV	pc, lr

	;void __set_riscosify_control(int __riscosify_flags)
	EXPORT	|__set_riscosify_control|
|__set_riscosify_control|
	LDR	a2, ctrl
	TEQ	a2, #0
	LDREQ	a2, ctrl_int

	[ {MODULE}={TRUE}
	LDREQ	a3, [sl, #-536]
	ADDEQ	a2, a2, a3
	]
	STR	a1, [a2, #0]
	MOV	pc, lr

ctrl
	DCD	|__riscosify_control|
ctrl_int
	DCD	|__riscosify_control_internal|

	AREA	|C$$data|, DATA

|__riscosify_control_internal|
	DCD	0

	END
