;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/asm_dec.s,v $
; $Date: 2003/04/04 22:04:55 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: alex $
;
; Declare registers and SWIs we will be calling.
;
;----------------------------------------------------------------------------

; Bits that control which bits are compiled into UnixLib. Note, this must be
; kept in sync with <sys/syslib.h>, <signal.h> and <errno.h>.

; Is an alloca() failure fatal? Nick says yes, Linux says no.
ALLOCA_FATAL	EQU	1
; Do paranoid checks ?
PARANOID	EQU	0
; Use dynamic areas for heap on RISC OS 3.5+
DYNAMIC_AREA	EQU	1
; Align things to 4K boundaries for exec
|__4K_BOUNDARY|	EQU	0
; Emulate the SWP instruction for ARM2 compatibility
|__SWP_ARM2|	EQU	0

|__INTEGRITY_CHECK|	EQU	1
|__FEATURE_ITIMERS|	EQU	1
|__FEATURE_SOCKET|	EQU	1
|__FEATURE_PIPEDEV|	EQU	1
|__FEATURE_PTHREADS|	EQU	1
USEFILEPATH		EQU	0
; For Internet 4 compatibility
COMPAT_INET4	EQU	0

; The offset of various members of the __pthread_thread structure
; This should be kept in sync with pthread.h, lib1aof.s, and stubs.s
__PTHREAD_MAGIC_OFFSET	EQU	0
__PTHREAD_CONTEXT_OFFSET	EQU	4
__PTHREAD_ALLOCA_OFFSET	EQU	8
__PTHREAD_ERRNO_OFFSET	EQU	20
__PTHREAD_ERRBUF_OFFSET	EQU	24

; registers

r0	RN	0
r1	RN	1
r2	RN	2
r3	RN	3
r4	RN	4
r5	RN	5
r6	RN	6
r7	RN	7
r8	RN	8
r9	RN	9
r10	RN	10
r11	RN	11
r12	RN	12
r13	RN	13
r14	RN	14
r15	RN	15

a1	RN	0	; Argument 1
a2	RN	1	; Argument 2
a3	RN	2	; Argument 3
a4	RN	3	; Argument 4
v1	RN	4	; Variable 1
v2	RN	5	; Variable 2
v3	RN	6	; Variable 3
v4	RN	7	; Variable 4
v5	RN	8	; Variable 5
v6	RN	9	; Variable 6

sl	RN	10	; Stack limit / stack chunk handle
fp	RN	11	; frame pointer
ip	RN	12	; Temporary workspace, used in procedure entry
sp	RN	13	; Stack pointer
lr	RN	14	; Link register
pc	RN	15	; Program counter

f0	FN	0
f1	FN	1
f2	FN	2
f3	FN	3
f4	FN	4
f5	FN	5
f6	FN	6
f7	FN	7

USR_Mode	EQU	&0
FIQ_Mode	EQU	&1
IRQ_Mode	EQU	&2
SVC_Mode	EQU	&3
Mode_Bits	EQU	&3

NFlag	EQU	&80000000	; Negative flag
ZFlag	EQU	&40000000	; Zero flag
CFlag	EQU	&20000000	; Carry flag
VFlag	EQU	&10000000	; Overflow flag
IFlag	EQU	&08000000	; IRQ disable
FFlag	EQU	&04000000	; FIRQ disable


	; Macro for embedding function names in code, just before
	; the function prologue.
	MACRO
	NAME	$name
	DCB	"$name",0
	ALIGN
	DCD	&FF000000 + (:LEN: "$name"+3+1) :AND: &FFFFFFFC
	MEND

	MACRO
	return	$cond, $dstreg, $srcreg
	[ {CONFIG} = 26
	MOV$cond.S	$dstreg, $srcreg
	|
	MOV$cond	$dstreg, $srcreg
	]
	MEND

	MACRO
	stackreturn	$cond, $regs
	[ {CONFIG} = 26
	LDM$cond.FD	sp!, {$regs}^
	|
	LDM$cond.FD	sp!, {$regs}
	]
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
	IMPORT	|errno|
	LDR	$Rerrno,=|errno|
	STR	$val,[$Rerrno]
	MOV	$val,#-1
	MEND

	MACRO
	__get_errno	$val,$Rerrno
	ASSERT	$val <> $Rerrno
	IMPORT	|errno|
	LDR	$Rerrno,=|errno|
	LDR	$val,[$Rerrno]
	MEND

	; NetSWI macro to call a networking (tcp/ip) swi.
	; We ensure that we are in SVC mode before calling the SWI because,
	; according to Stewart Brodie, certain versions of Acorn's Internet
	; stack require SWIs to be called in SVC mode.
	;
	; If ModuleCode is "yes", then nothing needs to be done, since
	; it is assumed we are already in SVC mode (possibly not always true,
	; so beware).
	; Note, UnixLib does not define ModuleCode.
	;
	; ip is destroyed. lr must be saved if called in SVC mode. This
	; macro is *NOT* safe for calling from IRQ or FIQ mode because
	; SVC_r14 is corrupted.

	MACRO
	NetSWI	$swiname

	[	"$ModuleCode" <> "yes"
	MOV	ip, pc			; PC+PSR
	EOR	ip, ip, #SVC_Mode	; current mode EOR SVC_Mode
	SWI	XOS_EnterOS		; enter SVC mode
	]

	SWI	$swiname

	BLVS	|__net_error|		; Call net error still in SVC
	[	"$ModuleCode" <> "yes"
	TEQP	ip, pc			; restore mode, corrupting flags
	MOV	a1, a1			; no-op to prevent contention
	]

	MEND

	MACRO
	NetSWIsimple	$swiname

	MOV	ip, lr			; Save mode, flags, return address
	[	"$ModuleCode" <> "yes"
	SWI	XOS_EnterOS		; enter SVC mode
	]

	SWI	$swiname

	MOVVCS	pc, ip			; return, restore mode, flags
	B	|__net_error_simple_entry|
	; branch to error routine still in SVC mode, with return in ip

	MEND

	; Macros for Socket SWIs which corrupt R0
	; Ensure R0 is 0 for VC, net_error ensures 1 for VS
	MACRO
	NetSWI0	$swiname

	[	"$ModuleCode" <> "yes"
	MOV	ip, pc			; PC+PSR
	EOR	ip, ip, #SVC_Mode	; current mode EOR SVC_Mode
	SWI	XOS_EnterOS		; enter SVC mode
	]

	SWI	$swiname
	MOVVC	a1, #0

	BLVS	|__net_error|		; Call net error still in SVC
	[	"$ModuleCode" <> "yes"
	TEQP	ip, pc			; restore mode, corrupting flags
	MOV	a1, a1			; no-op to prevent contention
	]

	MEND

	; Macro for Socket SWIs which corrupt R0
	; Ensure R0 is 0 for VC, net_error ensures 1 for VS
	MACRO
	NetSWIsimple0	$swiname

	MOV	ip, lr			; Save mode, flags, return adrress
	[	"$ModuleCode" <> "yes"
	SWI	XOS_EnterOS		; enter SVC mode
	]

	SWI	$swiname
	MOVVC	a1, #0

	MOVVCS	pc, ip			; return, restore mode, flags
	B	|__net_error_simple_entry|
	; branch to error routine still in SVC mode, with return in ip

	MEND

	; Macro to implement SWP instruction
	; srcreg and dstreg can be the same register, provided scratch is a different register
	; if srcreg and dstreg are different registers then scratch can be the same as dstreg
	MACRO
	swp_arm2	$dstreg, $srcreg, $addr, $scratch
	[ __SWP_ARM2 = 0
	SWP	$dstreg, $srcreg, [$addr]
	|
	STMFD	sp!, {lr}	; Could be called in USR or SVC mode
	SWI	XOS_IntOff
	LDR	$scratch, [$addr]
	STR	$srcreg, [$addr]
	[ $dstreg <> $scratch
	MOV	$dstreg, $scratch
	]
	SWI	XOS_IntOn
	LDMFD	sp!, {lr}
	]

	MEND

EXIT_SUCCESS	EQU	0
EXIT_FAILURE	EQU	1

TRUE	EQU	1
FALSE	EQU	0

SIGHUP	EQU	1	;   hangup
SIGINT	EQU	2	;   terminal interrupt
SIGQUIT EQU	3	; * ^\ from terminal
SIGILL	EQU	4	; * illegal instruction
SIGTRAP EQU	5	; * trace trap - unused
SIGIOT	EQU	6	;   abort
SIGABRT EQU	6	;   abort
SIGEMT	EQU	7	; * EMT (h/ware error)
SIGFPE	EQU	8	; * FPE trap
SIGKILL EQU	9	;   kill signal
SIGBUS	EQU	10	; * bus error
SIGSEGV EQU	11	; * segmentation violation
SIGSYS	EQU	12	; * bad system call
SIGPIPE EQU	13	;   broken pipe
SIGALRM EQU	14	;   alarm call
SIGTERM EQU	15	;   termination signal
SIGURG	EQU	16	;   urgent or out-of-band data
SIGSTOP	EQU	17	;   stop, unblockable
SIGTSTP	EQU	18	;   keyboard stop
SIGCONT	EQU	19	;   continue
SIGCHLD	EQU	20	;   child process has terminated/stopped
SIGCLD	EQU	SIGCHLD
SIGTTIN	EQU	21	;   background read from tty
SIGTTOU	EQU	22	;   background write to tty
SIGIO	EQU	23	;   file descriptor is ready to perform I/O
SIGPOLL	EQU	SIGIO
SIGXCPU	EQU	24	;   CPU time limit exceeded
SIGXFSZ	EQU	25	;   file size limit exceeded
SIGVTALRM EQU	26	;   alarm call
SIGPROF EQU	27	;   profiler alarm call
SIGWINCH EQU	28	;   window size change
SIGINFO EQU	29	;   information request
SIGUSR1 EQU	30	;   user signal 1
SIGUSR2 EQU	31	;   user signal 2
SIGLOST EQU	32	;   resource lost
SIGERR	EQU	33	;   RISC OS error

EOPSYS	EQU	88	; RISC OS error


OS_WriteS	EQU	&000001
OS_NewLine	EQU	&000003
OS_Exit		EQU	&000011
OS_EnterOS	EQU	&000016
OS_GenerateError	EQU	&00002B

X_Bit	EQU	&20000

XOS_WriteC			EQU	&000000 + X_Bit
XOS_WriteS			EQU	&000001 + X_Bit
XOS_Write0			EQU	&000002 + X_Bit
XOS_NewLine			EQU	&000003 + X_Bit
XOS_ReadC			EQU	&000004 + X_Bit
XOS_CLI				EQU	&000005 + X_Bit
XOS_Byte			EQU	&000006 + X_Bit
XOS_Word			EQU	&000007 + X_Bit
XOS_File			EQU	&000008 + X_Bit
XOS_Args			EQU	&000009 + X_Bit
XOS_BGet			EQU	&00000A + X_Bit
XOS_BPut			EQU	&00000B + X_Bit
XOS_GBPB			EQU	&00000C + X_Bit
XOS_Find			EQU	&00000D + X_Bit
XOS_ReadLine			EQU	&00000E + X_Bit
XOS_Control			EQU	&00000F + X_Bit
XOS_GetEnv			EQU	&000010 + X_Bit
XOS_Exit			EQU	&000011 + X_Bit
XOS_SetEnv			EQU	&000012 + X_Bit
XOS_IntOn			EQU	&000013 + X_Bit
XOS_IntOff			EQU	&000014 + X_Bit
XOS_CallBack			EQU	&000015 + X_Bit
XOS_EnterOS			EQU	&000016 + X_Bit
XOS_BreakPt			EQU	&000017 + X_Bit
XOS_BrealCtrl			EQU	&000018 + X_Bit

XOS_UpdateMEMC			EQU	&00001A + X_Bit
XOS_SetCallBack			EQU	&00001B + X_Bit
XOS_Mouse			EQU	&00001C + X_Bit
XOS_Heap			EQU	&00001D + X_Bit
XOS_Module			EQU	&00001E + X_Bit
XOS_ReadUnsigned		EQU	&000021 + X_Bit
XOS_ReadVarVal			EQU	&000023 + X_Bit
XOS_SetVarVal			EQU	&000024 + X_Bit
XOS_BinaryToDecimal		EQU	&000028 + X_Bit
XOS_FSControl			EQU	&000029 + X_Bit
XOS_ChangeDynamicArea		EQU	&00002A + X_Bit
XOS_ValidateAddress		EQU	&00003A + X_Bit
XOS_CallAfter			EQU	&00003B + X_Bit
XOS_CallEvery			EQU	&00003C + X_Bit
XOS_RemoveTickerEvent		EQU	&00003D + X_Bit
XOS_ChangeEnvironment		EQU	&000040 + X_Bit
XOS_ReadMonotonicTime		EQU	&000042 + X_Bit
XOS_WriteN			EQU	&000046 + X_Bit
XOS_ReadMemMapInfo		EQU	&000051 + X_Bit
XOS_AddCallBack			EQU	&000054 + X_Bit
XOS_SerialOp			EQU	&000057 + X_Bit
XOS_DynamicArea			EQU	&000066 + X_Bit
XOS_PlatformFeatures		EQU	&00006D + X_Bit
XOS_SynchroniseCodeAreas	EQU	&00006E + X_Bit
XOS_CallASWI			EQU	&00006F + X_Bit
XOS_CallASWIR12			EQU	&000071 + X_Bit
XOS_ConvertHex8			EQU	&0000D4 + X_Bit
XOS_ConvertInteger1		EQU	&0000D9 + X_Bit
XOS_ConvertInteger2		EQU	&0000DA + X_Bit
XOS_ConvertInteger3		EQU	&0000DB + X_Bit
XOS_ConvertInteger4		EQU	&0000DC + X_Bit
XOS_WriteI			EQU	&000100 + X_Bit

XFPEmulator_Version		EQU	&040480 + X_Bit

Socket_Creat			EQU	&41200
Socket_Bind			EQU	&41201
Socket_Listen			EQU	&41202
Socket_Accept			EQU	&41203
Socket_Connect			EQU	&41204
Socket_Recv			EQU	&41205
Socket_Recvfrom			EQU	&41206
Socket_Recvmsg			EQU	&41207
Socket_Send			EQU	&41208
Socket_Sendto			EQU	&41209
Socket_Sendmsg			EQU	&4120A
Socket_Shutdown			EQU	&4120B
Socket_Setsockopt		EQU	&4120C
Socket_Getsockopt		EQU	&4120D
Socket_Getpeername		EQU	&4120E
Socket_Getsockname		EQU	&4120F
Socket_Close			EQU	&41210
Socket_Select			EQU	&41211
Socket_Ioctl			EQU	&41212
Socket_Read			EQU	&41213
Socket_Write			EQU	&41214
Socket_Stat			EQU	&41215
Socket_Readv			EQU	&41216
Socket_Writev			EQU	&41217
Socket_Gettsize			EQU	&41218
Socket_Sendtosm			EQU	&41219
Socket_Sysctl			EQU	&4121A
Socket_Accept_1			EQU	&4121B
Socket_Recvfrom_1		EQU	&4121C
Socket_Recvmsg_1		EQU	&4121D
Socket_Sendmsg_1		EQU	&4121E
Socket_Getpeername_1		EQU	&4121F
Socket_Getsockname_1		EQU	&41220
Socket_InternalLookup		EQU	&41221
Socket_Version			EQU	&41222

Internet_GetHostByName		EQU	&46000
Internet_GetHostByAddr		EQU	&46001

XSocket_Creat			EQU	Socket_Creat + X_Bit
XSocket_Bind			EQU	Socket_Bind + X_Bit
XSocket_Listen			EQU	Socket_Listen + X_Bit
XSocket_Accept			EQU	Socket_Accept + X_Bit
XSocket_Connect			EQU	Socket_Connect + X_Bit
XSocket_Recv			EQU	Socket_Recv + X_Bit
XSocket_Recvfrom		EQU	Socket_Recvfrom + X_Bit
XSocket_Recvmsg			EQU	Socket_Recvmsg + X_Bit
XSocket_Send			EQU	Socket_Send + X_Bit
XSocket_Sendto			EQU	Socket_Sendto + X_Bit
XSocket_Sendmsg			EQU	Socket_Sendmsg + X_Bit
XSocket_Shutdown		EQU	Socket_Shutdown + X_Bit
XSocket_Setsockopt		EQU	Socket_Setsockopt + X_Bit
XSocket_Getsockopt		EQU	Socket_Getsockopt + X_Bit
XSocket_Getpeername		EQU	Socket_Getpeername + X_Bit
XSocket_Getsockname		EQU	Socket_Getsockname + X_Bit
XSocket_Close			EQU	Socket_Close + X_Bit
XSocket_Select			EQU	Socket_Select + X_Bit
XSocket_Ioctl			EQU	Socket_Ioctl + X_Bit
XSocket_Read			EQU	Socket_Read + X_Bit
XSocket_Write			EQU	Socket_Write + X_Bit
XSocket_Stat			EQU	Socket_Stat + X_Bit
XSocket_Readv			EQU	Socket_Readv + X_Bit
XSocket_Writev			EQU	Socket_Writev + X_Bit
XSocket_Gettsize		EQU	Socket_Gettsize + X_Bit
XSocket_Sendtosm		EQU	Socket_Sendtosm + X_Bit
XSocket_Sysctl			EQU	Socket_Sysctl + X_Bit
XSocket_Accept_1		EQU	Socket_Accept_1 + X_Bit
XSocket_Recvfrom_1		EQU	Socket_Recvfrom_1 + X_Bit
XSocket_Recvmsg_1		EQU	Socket_Recvmsg_1 + X_Bit
XSocket_Sendmsg_1		EQU	Socket_Sendmsg_1 + X_Bit
XSocket_Getpeername_1		EQU	Socket_Getpeername_1 + X_Bit
XSocket_Getsockname_1		EQU	Socket_Getsockname_1 + X_Bit
XSocket_InternalLookup		EQU	Socket_InternalLookup + X_Bit
XSocket_Version			EQU	Socket_Version + X_Bit

XInternet_GetHostByName		EQU	Internet_GetHostByName + X_Bit
XInternet_GetHostByAddr		EQU	Internet_GetHostByAddr + X_Bit

XSharedCLibrary_LibInitAPCS_R	EQU	&080681 + X_Bit
XSharedCLibrary_LibInitModule	EQU	&080682 + X_Bit

XMessageTrans_ErrorLookup	EQU	&041506 + X_Bit

XTaskWindow_TaskInfo		EQU	&043380 + X_Bit
XWimp_ReadSysInfo		EQU	&0400F2 + X_Bit
XDDEUtils_SetCLSize		EQU	&042581 + X_Bit

XSharedUnixLibrary_RegisterUpCall	EQU	&55c80 + X_Bit
XSharedUnixLibrary_DeRegisterUpCall	EQU	&55c81 + X_Bit

	END
