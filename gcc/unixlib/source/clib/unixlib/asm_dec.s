; Global definitions used by all assembler files.
; Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.

; Bits that control which bits are compiled into UnixLib. Note, this must be
; kept in sync with <sys/syslib.h>, <signal.h> and <errno.h>.

	; Include the UnixLib build options.
	GET	clib/unixlib/buildoptions.s

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

USR32_mode	EQU	2_10000
FIQ32_mode	EQU	2_10001
IRQ32_mode	EQU	2_10010
SVC32_Mode	EQU	2_10011
ABT32_Mode	EQU	2_10111
UND32_Mode	EQU	2_11011
SYS32_Mode	EQU	2_11111

IFlag32		EQU	&00000080

	; Import the assembler helper macros.
	; The script that translates AOF to ELF, will convert this line
	; to refer to 'elf-macros.s' instead.
	GET	incl-local/internal/aof-macros.s

EXIT_SUCCESS	EQU	0
EXIT_FAILURE	EQU	1

TRUE	EQU	1
FALSE	EQU	0

	;Keep the signal list in sync with <signal.h> contents.
SIGHUP	EQU	1	;   hangup
SIGINT	EQU	2	;   terminal interrupt
SIGQUIT EQU	3	; * ^\ from terminal
SIGILL	EQU	4	; * illegal instruction
SIGTRAP EQU	5	; * trace trap - unused
SIGIOT	EQU	6	;   abort
SIGABRT EQU	6	;   abort
SIGEMT	EQU	7	; * EMT (h/ware error)
SIGSTAK	EQU	SIGEMT
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
SIGOSERROR EQU	33	;   RISC OS error

EOPSYS	EQU	88	; RISC OS error

SharedUnixLibrary_SWIChunk	EQU	&55c80	;Range &55c80 - &55cc0 (excl) is allocated.
SharedUnixLibrary_ErrorChunk	EQU	&81a400	;Range &81a400 - &81a40 (excl) is allocated.
; The beginning of the error range is used by the SUL module.  The end of
; the error ranged (allocated from back to front) is used by the UnixLib
; main code itself (to generate errors during startup when necessary).
SharedUnixLibrary_Error_UnknownSWI	EQU SharedUnixLibrary_ErrorChunk + &00
SharedUnixLibrary_Error_UnknownKey	EQU SharedUnixLibrary_ErrorChunk + &01
SharedUnixLibrary_Error_StillActive	EQU SharedUnixLibrary_ErrorChunk + &02
SharedUnixLibrary_Error_TooOld		EQU SharedUnixLibrary_ErrorChunk + &03
; ...
SharedUnixLibrary_Error_FatalError	EQU SharedUnixLibrary_ErrorChunk + &3B
SharedUnixLibrary_Error_NoFPE		EQU SharedUnixLibrary_ErrorChunk + &3C
SharedUnixLibrary_Error_NotRecentEnough	EQU SharedUnixLibrary_ErrorChunk + &3D
SharedUnixLibrary_Error_NotEnoughMem	EQU SharedUnixLibrary_ErrorChunk + &3E
SharedUnixLibrary_Error_NoCallASWI	EQU SharedUnixLibrary_ErrorChunk + &3F

OS_WriteS			EQU	&000001
OS_NewLine			EQU	&000003
OS_Exit				EQU	&000011
OS_EnterOS			EQU	&000016
OS_GenerateError		EQU	&00002B
OS_CLI				EQU	&000005

X_Bit				EQU	&20000

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
XOS_AMBControl			EQU	&000070 + X_Bit
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
XWimp_SlotSize			EQU	&0400EC + X_Bit
XDDEUtils_SetCLSize		EQU	&042581 + X_Bit

Wimp_SlotSize			EQU	&0400EC

XFilter_RegisterPreFilter	EQU	&042640 + X_Bit
XFilter_RegisterPostFilter	EQU	&042641 + X_Bit
XFilter_DeRegisterPreFilter	EQU	&042642 + X_Bit
XFilter_DeRegisterPostFilter	EQU	&042643 + X_Bit

XSharedUnixLibrary_RegisterUpCall	EQU	&55c80 + X_Bit
XSharedUnixLibrary_DeRegisterUpCall	EQU	&55c81 + X_Bit
XSharedUnixLibrary_SetValue		EQU	&55c82 + X_Bit
XSharedUnixLibrary_Count		EQU	&55c83 + X_Bit
XSharedUnixLibrary_Initialise		EQU	&55c84 + X_Bit

XSOM_GOTFromAddr			EQU	&5858a + X_Bit

	; Entries into the __ul_global structure.  Must be kept in sync with
	; sys/_syslib.s and incl-local/internal/{aof,elf}-macros.s.
GBL_UNIXLIB_CLI				EQU	0

GBL_TIME_LOW				EQU	4
GBL_TIME_HIGH				EQU	8

GBL_NOTUSED1				EQU	12
GBL_TASKWINDOW				EQU	16
GBL_TASKHANDLE				EQU	20
GBL_DYNAMIC_NUM				EQU	24
GBL_OLD_U				EQU	28
GBL_32BIT				EQU	32
GBL_PANIC_MODE				EQU	36
GBL_PROC				EQU	40
GBL_UL_PAGESIZE				EQU	44
GBL_UPCALL_HANDLER_ADDR			EQU	48
GBL_UPCALL_HANDLER_R12			EQU	52

GBL_PTH_RETURN_ADDRESS			EQU	56
GBL_PTH_WORKSEMAPHORE			EQU	60
GBL_PTH_CALLBACK_SEMAPHORE		EQU	64
GBL_PTH_SYSTEM_RUNNING			EQU	68
GBL_PTH_CALLBACK_MISSED			EQU	72
GBL_PTH_NUM_RUNNING_THREADS		EQU	76

GBL_EXECUTING_SIGNALHANDLER		EQU	80
GBL_SIGNALHANDLER_SL			EQU	84
GBL_SIGNALHANDLER_SP			EQU	88

GBL_MUTEX				EQU	92
GBL_MALLOC_GBL				EQU	96

	; Entries in the __ul_memory table.  Must be kept in sync with
	; sys/_syslib.s and incl-local/internal/{aof,elf}-macros.s
MEM_APPSPACE_HIMEM			EQU	4
MEM_UNIXLIB_STACK			EQU	8

MEM_ROBASE				EQU	12
MEM_RWLOMEM				EQU	16
MEM_RWBASE				EQU	20
MEM_RWBREAK				EQU	24

MEM_UNIXLIB_STACK_LIMIT			EQU	28

MEM_DALOMEM				EQU	32
MEM_DABREAK				EQU	36
MEM_DALIMIT				EQU	40

MEM_APPSPACE_LIMIT			EQU	44
MEM_OLD_HIMEM				EQU	48

	; Entries in the struct __stack_chunk.  Must be kept in sync with
	; unix.h definition.
	; First 20 bytes equals SCL's _kernel_stack_chunk structure :
CHUNK_MAGIC				EQU	0	; Magic number to help detect if someone overwrites the stack
CHUNK_NEXT				EQU	4	; Ptr to next chunk
CHUNK_PREV				EQU	8	; Ptr to previous chunk
CHUNK_SIZE				EQU	12	; Size of chunk, including header
CHUNK_DEALLOC				EQU	16	; Function to call to free the chunk
CHUNK_RETURN				EQU	20	; Return address after freeing this chunk

CHUNK_OVERHEAD				EQU	24	; Size of chunk header

	END
