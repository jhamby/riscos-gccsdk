; Provides an Object Format file for linking with the SharedCLibrary
; Copyright (c) 1997-2005 Nick Burrett
; All rights reserved.
 
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions
; are met:
; 1. Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
; 2. Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the distribution.
; 3. The name of the author may not be used to endorse or promote products
;    derived from this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
; IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
; OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
; IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
; INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
; NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
; DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
; THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
; THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

r0 RN 0
r1 RN 1
r2 RN 2
r3 RN 3
r4 RN 4
r5 RN 5
r6 RN 6
fp RN 11
ip RN 12
sp RN 13
lr RN 14
pc RN 15


OS_GenerateError	EQU &2B
OS_Exit			EQU &11
OS_GetEnv		EQU &10

SharedCLibrary_LibInitAPCS_R	EQU &80681
SharedCLibrary_LibInitAPCS_32	EQU &80683

; Keep these in sync with unixlib/asm_dec.s and features.h
__FEATURE_PTHREADS	EQU	1
__PTHREAD_ALLOCA_OFFSET	EQU	8
 
	IMPORT	|Image$$RO$$Base|
	IMPORT	|RTSK$$Data$$Base|
	IMPORT	|RTSK$$Data$$Limit|
	IMPORT	|Image$$RW$$Base|
	IMPORT	|Image$$RW$$Limit|
	IMPORT	|Image$$ZI$$Base|
	IMPORT	|__root_stack_size|, WEAK
	IMPORT	|C$$code$$Base|
	IMPORT	|C$$code$$Limit|
	IMPORT	main, WEAK

	EXPORT	|__main|
	EXPORT	|_kernel_init|
	EXPORT	|_kernel_exit|
	EXPORT	|_kernel_setreturncode|
	EXPORT	|_kernel_exittraphandler|
	EXPORT	|_kernel_unwind|
	EXPORT	|_kernel_procname|
	EXPORT	|_kernel_language|
	EXPORT	|_kernel_command_string|
	EXPORT	|_kernel_hostos|
	EXPORT	|_kernel_swi|
	EXPORT	|_kernel_osbyte|
	EXPORT	|_kernel_osrdch|
	EXPORT	|_kernel_oswrch|
	EXPORT	|_kernel_osbget|
	EXPORT	|_kernel_osbput|
	EXPORT	|_kernel_osgbpb|
	EXPORT	|_kernel_osword|
	EXPORT	|_kernel_osfind|
	EXPORT	|_kernel_osfile|
	EXPORT	|_kernel_osargs|
	EXPORT	|_kernel_oscli|
	EXPORT	|_kernel_last_oserror|
	EXPORT	|_kernel_system|
	EXPORT	|_kernel_getenv|
	EXPORT	|_kernel_setenv|
	EXPORT	|_kernel_register_allocs|
	EXPORT	|_kernel_alloc|
	EXPORT	|_kernel_stkovf_split_0frame|
	EXPORT	|_kernel_stkovf_split|
	EXPORT	|_kernel_stkovf_copyargs|
	EXPORT	|_kernel_stkovf_copy0args|
	EXPORT	|_kernel_udiv|
	EXPORT	|__rt_udiv|
	EXPORT	|_kernel_urem|
	EXPORT	|_kernel_udiv10|
	EXPORT	|__rt_udiv10|
	EXPORT	|_kernel_sdiv|
	EXPORT	|__rt_sdiv|
	EXPORT	|_kernel_srem|
	EXPORT	|_kernel_sdiv10|
	EXPORT	|__rt_sdiv10|
	EXPORT	|_kernel_fpavailable|
	EXPORT	|_kernel_moduleinit|
	EXPORT	|_kernel_irqs_on|
	EXPORT	|_kernel_irqs_off|
	EXPORT	|_kernel_irqs_disabled|
	EXPORT	|_kernel_entermodule|
	EXPORT	|_kernel_escape_seen|
	EXPORT	|_kernel_current_stack_chunk|
	EXPORT	|_kernel_swi_c|
	EXPORT	|_kernel_register_slotextend|
	EXPORT	|_kernel_raise_error|

	EXPORT	TrapHandler
	EXPORT	UncaughtTrapHandler
	EXPORT	EventHandler
	EXPORT	UnhandledEventHandler
	EXPORT	|x$stack_overflow|
	EXPORT	|__rt_stkovf_split_small|
	EXPORT	|x$stack_overflow_1|
	EXPORT	|__rt_stkovf_split_big|
	EXPORT	|x$udivide|
	EXPORT	|x$uremainder|
	EXPORT	|x$divide|
	EXPORT	|x$divtest|
	EXPORT	|__rt_divtest|
	EXPORT	|x$remainder|
	EXPORT	|x$multiply|
	EXPORT	|_rd1chk|
	EXPORT	|_rd2chk|
	EXPORT	|_rd4chk|
	EXPORT	|_wr1chk|
	EXPORT	|_wr2chk|
	EXPORT	|_wr4chk|
	EXPORT	|_main|
	EXPORT	|_exit|
	EXPORT	|_clib_initialise|
	EXPORT	|_backtrace|
	EXPORT	|_count|
	EXPORT	|_count1|
	EXPORT	|_stfp|
	EXPORT	|_ldfp|
	EXPORT	|_printf|
	EXPORT	|_fprintf|
	EXPORT	|_sprintf|
	EXPORT	clock
	EXPORT	difftime
	EXPORT	mktime
	EXPORT	time
	EXPORT	asctime
	EXPORT	ctime
	EXPORT	gmtime
	EXPORT	localtime
	EXPORT	strftime
	EXPORT	memcpy
	EXPORT	memmove
	EXPORT	strcpy
	EXPORT	strncpy
	EXPORT	strcat
	EXPORT	strncat
	EXPORT	memcmp
	EXPORT	strcmp
	EXPORT	strncmp
	EXPORT	memchr
	EXPORT	strchr
	EXPORT	strcspn
	EXPORT	strpbrk
	EXPORT	strrchr
	EXPORT	strspn
	EXPORT	strstr
	EXPORT	strtok
	EXPORT	memset
	EXPORT	strerror
	EXPORT	strlen
	EXPORT	atof
	EXPORT	atoi
	EXPORT	atol
	EXPORT	strtod
	EXPORT	strtol
	EXPORT	strtoul
	EXPORT	rand
	EXPORT	srand
	EXPORT	calloc
	EXPORT	free
	EXPORT	malloc
	EXPORT	realloc
	EXPORT	abort
	EXPORT	atexit
	EXPORT	exit
	EXPORT	getenv
	EXPORT	system
	EXPORT	bsearch
	EXPORT	qsort
	EXPORT	abs
	EXPORT	div
	EXPORT	labs
	EXPORT	ldiv
	EXPORT	remove
	EXPORT	rename
	EXPORT	tmpfile
	EXPORT	|__old_tmpnam|
	EXPORT	fclose
	EXPORT	fflush
	EXPORT	fopen
	EXPORT	freopen
	EXPORT	setbuf
	EXPORT	setvbuf
	EXPORT	printf
	EXPORT	fprintf
	EXPORT	sprintf
	EXPORT	scanf
	EXPORT	fscanf
	EXPORT	sscanf
	EXPORT	vprintf
	EXPORT	|__gcc_vprintf|
	EXPORT	vfprintf
	EXPORT	|__gcc_vfprintf|
	EXPORT	vsprintf
	EXPORT	|__gcc_vsprintf|
	EXPORT	|_vfprintf|
	EXPORT	fgetc
	EXPORT	fgets
	EXPORT	fputc
	EXPORT	fputs
	EXPORT	|__filbuf|
	EXPORT	getc
	EXPORT	getchar
	EXPORT	gets
	EXPORT	|__flsbuf|
	EXPORT	putc
	EXPORT	putchar
	EXPORT	puts
	EXPORT	ungetc
	EXPORT	fread
	EXPORT	fwrite
	EXPORT	fgetpos
	EXPORT	fseek
	EXPORT	fsetpos
	EXPORT	ftell
	EXPORT	rewind
	EXPORT	clearerr
	EXPORT	feof
	EXPORT	ferror
	EXPORT	perror
        EXPORT  |__SIG_IGN|
	EXPORT	|__ignore_signal_handler|
        EXPORT  |__SIG_ERR|
	EXPORT	|__default_signal_handler|
        EXPORT  |__SIG_DFL|
	EXPORT	|__error_signal_marker|
	EXPORT	signal
	EXPORT	raise
	EXPORT	setjmp
	EXPORT	longjmp
	EXPORT	acos
	EXPORT	asin
	EXPORT	atan
	EXPORT	atan2
	EXPORT	cos
	EXPORT	sin
	EXPORT	tan
	EXPORT	cosh
	EXPORT	sinh
	EXPORT	tanh
	EXPORT	exp
	EXPORT	frexp
	EXPORT	ldexp
	EXPORT	log
	EXPORT	log10
	EXPORT	modf
	EXPORT	pow
	EXPORT	sqrt
	EXPORT	ceil
	EXPORT	fabs
	EXPORT	floor
	EXPORT	fmod
	EXPORT	setlocale
	EXPORT	isalnum
	EXPORT	isalpha
	EXPORT	iscntrl
	EXPORT	isdigit
	EXPORT	isgraph
	EXPORT	islower
	EXPORT	isprint
	EXPORT	ispunct
	EXPORT	isspace
	EXPORT	isupper
	EXPORT	isxdigit
	EXPORT	tolower
	EXPORT	toupper
	EXPORT	|__assert|
	EXPORT	|_memcpy|
	EXPORT	|_memset|
	EXPORT	localeconv
	EXPORT	mblen
	EXPORT	mbtowc
	EXPORT	wctomb
	EXPORT	mbstowcs
	EXPORT	wcstombs
	EXPORT	strxfrm
	EXPORT	strcoll
	EXPORT	|_clib_finalisemodule|
	EXPORT	|_clib_version|
	EXPORT	|_clib_finalise|  ; RISC OS PRMs say 'Finalise' but that'll clash
	EXPORT	tmpnam
	EXPORT	|_swi|
	EXPORT	|_swix|
	EXPORT  |disable_stack_extension|
	EXPORT	|__errno|
	EXPORT	errno
	EXPORT	|__iob|
	EXPORT	|_interrupts_off|
	EXPORT	|__ctype|
	EXPORT	|__huge_val|

	AREA	|Stub$$code|, CODE, READONLY

	ENTRY

	SWI	OS_GetEnv
	MOV	r2, r1			; workspace limit
	ADR	r0, stubs		; list of stubs
	LDR	r1, workspace		; workspace start
	MOV	r3, #-1			; "-1"
	MOV	r4, #0			; "0"
	MOV	r5, #-1			; "-1"

	; check for __root_stack_size. If it doesn't exist, then
	; make our own up.
	LDR	r6, stack_size
	TEQ	r6, #0
	LDRNE	r6, [r6]		; use __root_stack_size
	MOVEQ	r6, #(4*1024)		; 4KB (single chunk) stack, then.
	MOV	r6, r6, asr #10 	; convert bytes to KB
	MOV	r6, r6, lsl #16 	; put in upper 16bits

	TEQ	r0, r0			; Set Z flag
	TEQ	pc, pc			; EQ if in 32-bit mode
	ORREQ	r6, r6, #1

	[ {config} = 26
	SWI	SharedCLibrary_LibInitAPCS_R
	MOV	r6, r6, lsl #16 	; safety catch for ancient Shared C Lib modules
	CMP	r6, #(5<<16)		; check library version number
	|
	SWI	SharedCLibrary_LibInitAPCS_32
	MOV	r6, r6, lsl #16 	; safety catch for ancient Shared C Lib modules
	CMP	r6, #(6<<16)		; check library version number (32bit)
	]

	MOVGE	r4, r0			; end of workspace
	ADRGE	r0, kernel_init_block
	MOVGE	r3, #0			; we're an application
	BGE	|_kernel_init|		; off we go!

	ADR	r0, error
	SWI	OS_GenerateError
error
	DCD	0
	DCB	"Shared C Library not loaded or out of date", 0
	ALIGN

stubs
	DCD	1			; lib chunk id : kernel
	DCD	kernel_vectors
	DCD	kernel_vectors_end
	DCD	kernel_statics
	DCD	kernel_statics_end
	DCD	2			; lib chunk id : CLib
	DCD	clib_vectors
	DCD	clib_vectors_end
	DCD	clib_statics
	DCD	clib_statics_end
	DCD	-1			; end of list

kernel_init_block
	DCD	|Image$$RO$$Base|	; image base
	DCD	|RTSK$$Data$$Base|	; start of lang blocks
	DCD	|RTSK$$Data$$Limit|	; end of lang blocks
workspace
	DCD	|Image$$RW$$Limit|
stack_size
	DCD	|__root_stack_size|
	DCD	|Image$$RW$$Base|
	DCD	|Image$$ZI$$Base|



	AREA	|RTSK$$Data|, DATA, READONLY
	DCD	40 ; |RTSK$$Data$$Limit| - |RTSK$$Data$$Base|
	DCD	|Image$$RO$$Base| ; |C$$code$$Base|	; gcc is not nicely compatible with SCL
	DCD	|Image$$RO$$Limit| ; |C$$code$$Limit|	; because it makes C$$CodeX areas ;-(
	DCD	language_name		; must be "C"
	DCD	|__main|		; our PROC-returning InitProc
	DCD	0			; finaliser
	; these are the "optionals"...
	DCD	|TrapHandler|		; SharedCLib's own trap handler...
	DCD	|UncaughtTrapHandler|	; ...and uncaught trap proc
	DCD	|EventHandler|		; ...and event proc
	DCD	|UnhandledEventHandler|	; ...and unhandled event proc!
	; no fast event proc
	; no unwind proc
	; no name proc

; Provide storage for __arm_alloca
	[ __FEATURE_PTHREADS = 1
	AREA	|C$$Data|, DATA
	EXPORT	|__pthread_running_thread|
|__pthread_running_thread|
	DCD	|__pthread_running_thread_data|-__PTHREAD_ALLOCA_OFFSET
|__pthread_running_thread_data|
	%	12
	]

	AREA	|C$$Code|, CODE, READONLY
language_name
	DCB	"C", 0
	ALIGN

|__main|
	MOV	r0, #1
	LDR	r1, stub_data
	STRB	r0, [r1, #_stub_kallocExtendsWS - kernel_statics]

	MOV	r0, sp			; (historical??)
	STMFD	sp!, {lr}
	LDR	r1, rtsk_data
	LDMIB	r1, {r1, r2}		; fetch code start/end (historical??)
	BL	|_clib_initialise|
	LDR	r0, c_run		; get our main()
	CMP	r0, #0			; was there one?
	ADRNE	r0, c_next		; yup, so point to hook to it

	; For 26-bit SharedCLibrary (in which case, we're using a 26-bit OS)
	; we still need to restore flags on exit.  For 32-bit SCL on 26-bit
	; we restore them anyway, as this makes no difference.

	TEQ     r0, r0                  ; Set Z flag
	TEQ     pc, pc                  ; EQ if in 32-bit mode

	LDMEQFD sp!, {pc}
	LDMFD   sp!, {pc}^

	DCB     "___init",0
	ALIGN
	DCD	0xff000008
; this is called when all RTL blocks have been initialised via their InitProc
c_next
	MOV	ip, sp
	STMFD	sp!, {fp, ip, lr, pc}
	SUB	fp, ip, #4
	BL	|_kernel_command_string|; get r0=command string
	LDR	r1, c_run		; get ptr to main() function
	BL	|_main|			; call clib to enter it.  SHOULD never return from here.
	LDMFD   fp, {fp, sp, pc}

stub_data
	DCD	|Stub$$Data|

rtsk_data
	DCD	|RTSK$$Data|

c_run
	DCD	main

	GBLA	Loop

	AREA	|Stub$$Entries|, CODE, READONLY

kernel_vectors

|_kernel_init|			MOV	pc,#0
|_kernel_exit|			MOV	pc,#0
|_kernel_setreturncode|		MOV	pc,#0
|_kernel_exittraphandler|	MOV	pc,#0
|_kernel_unwind|		MOV	pc,#0
|_kernel_procname|		MOV	pc,#0
|_kernel_language|		MOV	pc,#0
|_kernel_command_string|	MOV	pc,#0
|_kernel_hostos|		MOV	pc,#0
|_kernel_swi|			MOV	pc,#0
|_kernel_osbyte|		MOV	pc,#0
|_kernel_osrdch|		MOV	pc,#0
|_kernel_oswrch|		MOV	pc,#0
|_kernel_osbget|		MOV	pc,#0
|_kernel_osbput|		MOV	pc,#0
|_kernel_osgbpb|		MOV	pc,#0
|_kernel_osword|		MOV	pc,#0
|_kernel_osfind|		MOV	pc,#0
|_kernel_osfile|		MOV	pc,#0
|_kernel_osargs|		MOV	pc,#0
|_kernel_oscli|			MOV	pc,#0
|_kernel_last_oserror|		MOV	pc,#0
|_kernel_system|		MOV	pc,#0
|_kernel_getenv|		MOV	pc,#0
|_kernel_setenv|		MOV	pc,#0
|_kernel_register_allocs|	MOV	pc,#0
|_kernel_alloc|			MOV	pc,#0
|_kernel_stkovf_split_0frame|	MOV	pc,#0
|_kernel_stkovf_split|		MOV	pc,#0
|_kernel_stkovf_copyargs|	MOV	pc,#0
|_kernel_stkovf_copy0args|	MOV	pc,#0
|__rt_udiv|
|_kernel_udiv|			MOV	pc,#0
|_kernel_urem|			MOV	pc,#0
|__rt_udiv10|
|_kernel_udiv10|		MOV	pc,#0
|__rt_sdiv|
|_kernel_sdiv|			MOV	pc,#0
|_kernel_srem|			MOV	pc,#0
|__rt_sdiv10|
|_kernel_sdiv10|		MOV	pc,#0
|_kernel_fpavailable|		MOV	pc,#0
|_kernel_moduleinit|		MOV	pc,#0
|_kernel_irqs_on|               MOV	pc,#0
|_kernel_irqs_off|		MOV	pc,#0
|_kernel_irqs_disabled| 	MOV	pc,#0
|_kernel_entermodule|		MOV	pc,#0
|_kernel_escape_seen|		MOV	pc,#0
|_kernel_current_stack_chunk|	MOV	pc,#0
|_kernel_swi_c| 		MOV	pc,#0
|_kernel_register_slotextend|   MOV     pc,#0
|_kernel_raise_error|           MOV     pc,#0

kernel_vectors_end

kernel_vectors_space	% kernel_vectors_end - kernel_vectors

clib_vectors

|TrapHandler|			MOV	pc,#0
|UncaughtTrapHandler|		MOV	pc,#0
|EventHandler|			MOV	pc,#0
|UnhandledEventHandler|		MOV	pc,#0
|__rt_stkovf_split_small|
|x$stack_overflow|		MOV	pc,#0
|__rt_stkovf_split_big|
|x$stack_overflow_1|		MOV	pc,#0
|x$udivide|			MOV	pc,#0
|x$uremainder|			MOV	pc,#0
|x$divide|			MOV	pc,#0
|__rt_divtest|
|x$divtest|			MOV	pc,#0
|x$remainder|			MOV	pc,#0
|x$multiply|			MOV	pc,#0
|_rd1chk|			MOV	pc,#0
|_rd2chk|			MOV	pc,#0
|_rd4chk|			MOV	pc,#0
|_wr1chk|			MOV	pc,#0
|_wr2chk|			MOV     pc,#0
|_wr4chk|			MOV     pc,#0
|_main|                         MOV	pc,#0
|_exit| 			MOV	pc,#0
|_clib_initialise|		MOV	pc,#0
|_backtrace|			MOV	pc,#0
|_count|			MOV	pc,#0
|_count1|			MOV	pc,#0
|_stfp| 			MOV	pc,#0
|_ldfp| 			MOV	pc,#0
|_printf|			MOV	pc,#0
|_fprintf|			MOV	pc,#0
|_sprintf|			MOV	pc,#0
|clock| 			MOV	pc,#0
|difftime|			MOV	pc,#0
|mktime|			MOV	pc,#0
|time|				MOV	pc,#0
|asctime|			MOV	pc,#0
|ctime| 			MOV	pc,#0
|gmtime|			MOV	pc,#0
|localtime|			MOV	pc,#0
|strftime|			MOV     pc,#0
|memcpy|			MOV     pc,#0
|memmove|                       MOV	pc,#0
|strcpy|			MOV	pc,#0
|strncpy|			MOV	pc,#0
|strcat|			MOV	pc,#0
|strncat|			MOV	pc,#0
|memcmp|			MOV	pc,#0
|strcmp|			MOV	pc,#0
|strncmp|			MOV	pc,#0
|memchr|			MOV	pc,#0
|strchr|			MOV	pc,#0
|strcspn|			MOV	pc,#0
|strpbrk|			MOV	pc,#0
|strrchr|			MOV	pc,#0
|strspn|			MOV	pc,#0
|strstr|			MOV	pc,#0
|strtok|			MOV	pc,#0
|memset|			MOV	pc,#0
|strerror|			MOV	pc,#0
|strlen|			MOV	pc,#0
|atof|				MOV     pc,#0
|atoi|				MOV     pc,#0
|atol|                          MOV	pc,#0
|strtod|			MOV	pc,#0
|strtol|			MOV	pc,#0
|strtoul|			MOV	pc,#0
|rand|				MOV	pc,#0
|srand| 			MOV	pc,#0
|calloc|			MOV	pc,#0
|free|				MOV	pc,#0
|malloc|			MOV	pc,#0
|realloc|			MOV	pc,#0
|abort| 			MOV	pc,#0
|atexit|			MOV	pc,#0
|exit|				MOV	pc,#0
|getenv|			MOV	pc,#0
|system|			MOV	pc,#0
|bsearch|			MOV	pc,#0
|qsort| 			MOV	pc,#0
|abs|				MOV	pc,#0
|div|				MOV	pc,#0
|labs|				MOV     pc,#0
|ldiv|				MOV     pc,#0
|remove|                        MOV	pc,#0
|rename|			MOV	pc,#0
|tmpfile|			MOV	pc,#0
|__old_tmpnam|			MOV	pc,#0
|fclose|			MOV	pc,#0
|fflush|			MOV	pc,#0
|fopen| 			MOV	pc,#0
|freopen|			MOV	pc,#0
|setbuf|			MOV	pc,#0
|setvbuf|			MOV	pc,#0
|printf|			MOV	pc,#0
|fprintf|			MOV	pc,#0
|sprintf|			MOV	pc,#0
|scanf| 			MOV	pc,#0
|fscanf|			MOV	pc,#0
|sscanf|			MOV	pc,#0
|__gcc_vprintf|
|vprintf|	 		MOV	pc,#0
|__gcc_vfprintf|
|vfprintf|			MOV     pc,#0
|__gcc_vsprintf|
|vsprintf|	                MOV	pc,#0
|_vfprintf|			MOV	pc,#0
|fgetc| 			MOV	pc,#0
|fgets| 			MOV	pc,#0
|fputc| 			MOV	pc,#0
|fputs| 			MOV	pc,#0
|__filbuf|			MOV	pc,#0
|getc|				MOV	pc,#0
|getchar|			MOV	pc,#0
|gets|				MOV	pc,#0
|__flsbuf|			MOV	pc,#0
|putc|				MOV	pc,#0
|putchar|			MOV	pc,#0
|puts|				MOV	pc,#0
|ungetc|			MOV	pc,#0
|fread| 			MOV	pc,#0
|fwrite|			MOV	pc,#0
|fgetpos|			MOV	pc,#0
|fseek| 			MOV	pc,#0
|fsetpos|			MOV     pc,#0
|ftell| 			MOV     pc,#0
|rewind|                        MOV	pc,#0
|clearerr|			MOV	pc,#0
|feof|				MOV	pc,#0
|ferror|			MOV	pc,#0
|perror|			MOV	pc,#0
|__SIG_IGN|
|__ignore_signal_handler|	MOV	pc,#0
|__SIG_ERR|
|__error_signal_marker| 	MOV	pc,#0
|__SIG_DFL|
|__default_signal_handler|	MOV	pc,#0
|signal|			MOV	pc,#0
|raise| 			MOV	pc,#0
|setjmp|			MOV	pc,#0
|longjmp|			MOV	pc,#0
|acos|				MOV	pc,#0
|asin|				MOV	pc,#0
|atan|				MOV	pc,#0
|atan2| 			MOV	pc,#0
|cos|				MOV	pc,#0
|sin|				MOV	pc,#0
|tan|				MOV	pc,#0
|cosh|				MOV     pc,#0
|sinh|				MOV     pc,#0
|tanh|                          MOV	pc,#0
|exp|				MOV	pc,#0
|frexp| 			MOV	pc,#0
|ldexp| 			MOV	pc,#0
|log|				MOV	pc,#0
|log10| 			MOV	pc,#0
|modf|				MOV	pc,#0
|pow|				MOV	pc,#0
|sqrt|				MOV	pc,#0
|ceil|				MOV	pc,#0
|fabs|				MOV	pc,#0
|floor| 			MOV	pc,#0
|fmod|				MOV	pc,#0
|setlocale|			MOV	pc,#0
|isalnum|			MOV	pc,#0
|isalpha|			MOV	pc,#0
|iscntrl|			MOV	pc,#0
|isdigit|			MOV	pc,#0
|isgraph|			MOV	pc,#0
|islower|			MOV     pc,#0
|isprint|			MOV     pc,#0
|ispunct|                       MOV	pc,#0
|isspace|			MOV	pc,#0
|isupper|			MOV	pc,#0
|isxdigit|			MOV	pc,#0
|tolower|			MOV	pc,#0
|toupper|			MOV	pc,#0
|__assert|			MOV	pc,#0
|_memcpy|			MOV	pc,#0
|_memset|			MOV	pc,#0
|localeconv|			MOV	pc,#0
|mblen| 			MOV	pc,#0
|mbtowc|			MOV	pc,#0
|wctomb|			MOV	pc,#0
|mbstowcs|			MOV	pc,#0
|wcstombs|			MOV	pc,#0
|strxfrm|			MOV	pc,#0
|strcoll|			MOV	pc,#0
|_clib_finalisemodule|		MOV	pc,#0
|_clib_version| 		MOV	pc,#0
|_clib_finalise|		MOV     pc,#0
|tmpnam|			MOV     pc,#0
|_swi|                          MOV     pc,#0
|_swix|                         MOV     pc,#0

clib_vectors_end
clib_vectors_space	% clib_vectors_end - clib_vectors

	AREA	|Stub$$Data|, DATA
kernel_statics	% &31c
kernel_statics_end

|_stub_imageBase|		EQU	kernel_statics+&000
|_stub_rtsDataBase|		EQU	kernel_statics+&004
|_stub_rtsDataLimit|		EQU	kernel_statics+&008
|_stub_errorR12|		EQU	kernel_statics+&00c
|_stub_errorBuffer|		EQU	kernel_statics+&010
|_stub_errorNumber|		EQU	kernel_statics+&014
|_stub_errorString|		EQU	kernel_statics+&018
|_stub_registerDump|		EQU	kernel_statics+&07c
|_stub_oldAbortHandlers| 	EQU	kernel_statics+&0bc
|_stub_oldExitHandler|		EQU	kernel_statics+&0cc
|_stub_oldMemoryLimit|		EQU	kernel_statics+&0d4
|_stub_oldErrorHandler|  	EQU	kernel_statics+&0d8
|_stub_oldErrorR0|		EQU	kernel_statics+&0dc
|_stub_oldErrorBuffer|		EQU	kernel_statics+&0e0
|_stub_oldCallBackHandler|	EQU	kernel_statics+&0e4
|_stub_oldEscapeHandler|	EQU	kernel_statics+&0f0
|_stub_oldEventHandler|  	EQU	kernel_statics+&0f8
|_stub_oldUpCallHandler| 	EQU	kernel_statics+&100
|_stub_languageEnvSave|  	EQU	kernel_statics+&108
|_stub_hadEscape|		EQU	kernel_statics+&114
|_stub_kallocExtendsWS|  	EQU	kernel_statics+&115
|_stub_inTrapHandler|		EQU	kernel_statics+&116
|_stub_beingDebugged|		EQU	kernel_statics+&117
|_stub_fpPresent|		EQU	kernel_statics+&118
|_stub_initialised|		EQU	kernel_statics+&119
|_stub_callbackInactive| 	EQU	kernel_statics+&11a
|_stub_unused_byte_2|		EQU	kernel_statics+&11b
|_stub_IIHandlerInData|  	EQU	kernel_statics+&11c
|_stub_PAHandlerInData|  	EQU	kernel_statics+&128
|_stub_DAHandlerInData|  	EQU	kernel_statics+&134
|_stub_AEHandlerInData|  	EQU	kernel_statics+&140
|_stub_eventCode|		EQU	kernel_statics+&14c
|_stub_eventUserR13|		EQU	kernel_statics+&150
|_stub_eventRegisters|		EQU	kernel_statics+&154
|_stub_fastEventStack|		EQU	kernel_statics+&184
|_stub_fastEventStackEnd|	EQU	kernel_statics+&284
|_stub_heapTop| 		EQU	kernel_statics+&284
|_stub_heapLimit|		EQU	kernel_statics+&288
|_stub_allocProc|		EQU	kernel_statics+&28c
|_stub_freeProc| 		EQU	kernel_statics+&290
|_stub_returnCode|		EQU	kernel_statics+&294
|_stub_moduleDataWord|		EQU	kernel_statics+&298
|_stub_APCS_A_Client|		EQU	kernel_statics+&29c
|_stub_escapeSeen|		EQU	kernel_statics+&29d
|_stub_unwinding|		EQU	kernel_statics+&29e
|_stub_underDesktop|		EQU	kernel_statics+&29f
|_stub_heapBase|		EQU	kernel_statics+&2a0
|_stub_ArgString|		EQU	kernel_statics+&2a4
|_stub_heapExtender|		EQU	kernel_statics+&2a8
|_stub_knownSlotSize|		EQU	kernel_statics+&2ac
|_stub_initSlotSize|		EQU	kernel_statics+&2b0
|_stub_lk_RestoreOSHandlers|	EQU	kernel_statics+&2b4
|_stub_extendChunk|		EQU	kernel_statics+&2b8
|_stub_rootStackChunk|		EQU	kernel_statics+&2bc
|_stub_pc_hex_buff|		EQU	kernel_statics+&2c0
|_stub_returnCodeLimit|		EQU	kernel_statics+&2c0
|_stub_reg_hex_buff|		EQU	kernel_statics+&2cc
|disable_stack_extension|	EQU	kernel_statics+&2d8
|_stub_unused|			EQU	kernel_statics+&2dc

clib_statics	% &b48
clib_statics_end

errno				EQU	clib_statics+0
|__errno|			EQU	clib_statics+0
|__iob|				EQU	clib_statics+4
|_interrupts_off|		EQU	clib_statics+&284
|_stub__saved_interrupt|	EQU	clib_statics+&288
|_stub_ctype_eof|		EQU	clib_statics+&28C
|__ctype|			EQU	clib_statics+&290
|__huge_val|			EQU	clib_statics+&390

|_stub_app_space_end|		EQU	clib_statics+&6b4
|_stub_stack_o_flag|		EQU	clib_statics+&6b8
|_stub_inSignalHandler|		EQU	clib_statics+&6b9
|_stub_dummybyte|		EQU	clib_statics+&6ba
|_stub_|			EQU	clib_statics+&6bb
|_stub_ClibSpace|		EQU	clib_statics+&6bc

	END
