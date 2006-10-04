;
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_rd1chk|
	EXPORT	|_rd2chk|
	EXPORT	|_rd4chk|
	EXPORT	|__rt_rd1chk|
	EXPORT	|__rt_rd2chk|
	EXPORT	|__rt_rd4chk|
	EXPORT	|_wr1chk|
	EXPORT	|_wr2chk|
	EXPORT	|_wr4chk|
	EXPORT	|__rt_wr1chk|
	EXPORT	|__rt_wr2chk|
	EXPORT	|__rt_wr4chk|
	IMPORT	raise
	IMPORT	|__base|
	IMPORT	|__break|
	IMPORT	|__himem|
	IMPORT	|__stack|
	IMPORT	|__rwbase|

|__rt_rd1chk|
|_rd1chk|
	BIC	a1, a1, #1 	; Ensure address is half word aligned
|__rt_rd2chk|
|_rd2chk|
	TST	a1, #1
	BNE	|__rwNsegv|
	BIC	a1, a1, #2 ; Ensure address is full word aligned
|__rt_rd4chk|
|_rd4chk|
	TST	a1, #2
	BNE	|__rwNsegv|

	LDR	a2, =|__base|
	LDR	a2, [a2]
|__rwNchk|
	CMP	a1, a2
	BLO	|__rwNsegv|
	LDR	a2, =|__break|
	LDR	a2, [a2]
	CMP	a1, a2
	MOVLO	pc, lr
	LDR	a2, =|__stack|
	LDR	a2, [a2]
	CMP	a1, a2
	BLO	|__rwNsegv|
	LDR	a2, =|__himem|
	LDR	a2, [a2]
	CMP	a1, a2
	MOVLO	pc, lr
|__rwNsegv|
	MOV	a1, #SIGSEGV
	B	raise

|__rt_wr1chk|
|_wr1chk|
	BIC	a1, a1, #1	; Ensure address is half word aligned
|__rt_wr2chk|
|_wr2chk|
	TST	a1, #1
	BNE	|__rwNsegv|
	BIC	a1, a1, #2	; Ensure address is full word aligned

|__rt_wr4chk|
|_wr4chk|
	TST	a1,#2
	BNE	|__rwNsegv|

	LDR	a2,=|__rwbase|
	LDR	a2,[a2]
	B	|__rwNchk|

	END

