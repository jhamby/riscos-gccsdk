; UnixLib assembler helper macros (AOF version)
; Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
;
; This file and elf-macros.s must be kept in sync with each other.

	; Macro for embedding function names in code, just before
	; the function prologue.
	MACRO
	NAME	$name
	DCB	"$name",0
	ALIGN
	DCD	&FF000000 + (:LEN: "$name"+3+1) :AND: &FFFFFFFC
	MEND

	; Assembler equivalent of __set_errno in errno.h.
	; #define __set_errno(val) (errno = (val), -1)
	; Entry condition
	;   val = new error code
	; Exit condition
	;   val = -1
	;   Rerrno destroyed

	MACRO
	__set_errno	$val,$Rerrno
	ASSERT	$val <> $Rerrno
	[ __UNIXLIB_ERRNO_THREADED > 0
	IMPORT	|__pthread_running_thread|
	LDR	$Rerrno, =|__pthread_running_thread|
	LDR	$Rerrno, [$Rerrno]
	STR	$val, [$Rerrno, #__PTHREAD_ERRNO_OFFSET]
	|
	IMPORT	|errno|
	LDR	$Rerrno, =|errno|
	STR	$val, [$Rerrno]
	]
	MOV	$val, #-1
	MEND

	MACRO
	__get_errno	$val,$Rerrno
	[ __UNIXLIB_ERRNO_THREADED > 0
	IMPORT	|__pthread_running_thread|
	LDR	$Rerrno, =|__pthread_running_thread|
	LDR	$Rerrno, [$Rerrno]
	LDR	$val, [$Rerrno, #__PTHREAD_ERRNO_OFFSET]
	|
	IMPORT	|errno|
	LDR	$Rerrno, =|errno|
	LDR	$val, [$Rerrno]
	]
	MEND

	; NetSWI, NetSWIsimple, NetSWI0 and NetSWIsimple0 are macros to call
	; one of the networking (TCP/IP) SWIs.

	; Calls networking (TCP/IP) SWI.
	MACRO
	NetSWI	$swiname
	IMPORT	|__net_error|
	SWI	$swiname
	BLVS	|__net_error|		; Call net error
	MEND

	; Calls networking (TCP/IP) SWI and returns to lr.
	MACRO
	NetSWIsimple	$swiname
	IMPORT	|__net_error|
	SWI	$swiname
	MOVVC	pc, lr
	B	|__net_error|
	MEND

	; Calls networking (TCP/IP) SWI.
	; Returns 0 in R0 for success, a non zero errno in R0 when
	; error happened.
	MACRO
	NetSWI0	$swiname
	IMPORT	|__net_error|
	SWI	$swiname
	MOVVC	a1, #0
	BLVS	|__net_error|		; Call net error
	MEND

	; Calls networking (TCP/IP) SWI and returns to lr.
	; Returns 0 in R0 for success, a non zero errno in R0 when
	; error happened.
	MACRO
	NetSWIsimple0	$swiname
	IMPORT	|__net_error|
	SWI	$swiname
	MOVVC	a1, #0
	MOVVC	pc, lr
	B	|__net_error|
	MEND

	; Macro to change processor modes and interrupt flags
	; Works in 26bit or 32bit modes, on all architectures
	; Use e.g. CHGMODE a1, SVC_Mode+IFlag
	MACRO
	CHGMODE	$scratch, $mode
	TEQ	a1, a1	; Set Z
	TEQ	pc, pc	; EQ if 32-bit mode
	TEQNEP	pc, #$mode
	MRSEQ	$scratch, CPSR	; Acts a NOP for TEQP
	BICEQ	$scratch, $scratch, #&cf	; Preserve 32bit mode bit
	[ $mode <> 0
	ORREQ	$scratch, $scratch, #(($mode) :AND: &f) + (($mode) :SHR: 20)
	]
	MSREQ	CPSR_c, $scratch
	MOV	a1, a1	; Avoid StrongARM MSR bug

	MEND

	; Macros to mirror ELF versions.
	MACRO
	WORD $name
	DCD	$name
	MEND

	; The rest are NOPs.
	MACRO
	DECLARE_FUNCTION $name
	MEND

	MACRO
	DECLARE_OBJECT $name
	MEND

	MACRO
	PICEQ $instr
	MEND

	MACRO
	PICNE $instr
	MEND

	END

