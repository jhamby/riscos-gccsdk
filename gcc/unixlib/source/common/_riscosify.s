; __get_riscosify_control, __set_riscosify_control
; Copyright (c) 2002-2006 UnixLib Developers

        GET     clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__riscosify_control|, WEAK

	;int __get_riscosify_control (void)
	EXPORT	|__get_riscosify_control|
	NAME	__get_riscosify_control
|__get_riscosify_control|
 PICEQ "STMFD	sp!, {v4, lr}"
 PICEQ "BL	__rt_load_pic"
	LDR	a1, |.L0|+4	;=__riscosify_control
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	LDREQ	a1, |.L0|	;=__riscosify_control_internal
 PICEQ "LDREQ	a1, [v4, a1]"
	LDR	a1, [a1, #0]
 PICEQ "LDMFD	sp!, {v4, lr}"
	MOV	pc, lr
|.L0|
	WORD	|__riscosify_control_internal|
	WORD	|__riscosify_control|
	DECLARE_FUNCTION __get_riscosify_control

	;void __set_riscosify_control(int __riscosify_flags)
	EXPORT	|__set_riscosify_control|
	NAME	__set_riscosify_control
|__set_riscosify_control|
 PICEQ "STMFD	sp!, {v4, lr}"
 PICEQ "BL	__rt_load_pic"
	LDR	a2, |.L0|+4	;=__riscosify_control
 PICEQ "LDR	a2, [v4, a2]"
	TEQ	a2, #0
	LDREQ	a2, |.L0|	;=__riscosify_control_internal
 PICEQ "LDREQ	a2, [v4, a2]"
	STR	a1, [a2, #0]
 PICEQ "LDMFD	sp!, {v4, lr}"
	MOV	pc, lr
	DECLARE_FUNCTION __set_riscosify_control

	AREA	|C$$data|, DATA

|__riscosify_control_internal|
	DCD	0
	DECLARE_OBJECT __riscosify_control_internal

	END
