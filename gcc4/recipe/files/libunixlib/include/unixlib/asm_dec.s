@ Global definitions used by all assembler files.
@ Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.

@ Bits that control which bits are compiled into UnixLib. Note, this must be
@ kept in sync with <sys/syslib.h>, <signal.h> and <errno.h>.

	@ Include the UnixLib build options.
#include "unixlib/buildoptions.h"

@ The offset of various members of the __pthread_thread structure
@ This should be kept in sync with pthread.h, lib1aof.s, and stubs.s
.set	__PTHREAD_MAGIC_OFFSET, 0
.set	__PTHREAD_CONTEXT_OFFSET, 4
.set	__PTHREAD_ALLOCA_OFFSET, 8
.set	__PTHREAD_ERRNO_OFFSET, 20
.set	__PTHREAD_ERRBUF_OFFSET, 24

@ registers





.set	USR_Mode, 0x0
.set	FIQ_Mode, 0x1
.set	IRQ_Mode, 0x2
.set	SVC_Mode, 0x3
.set	Mode_Bits, 0x3

.set	NFlag, 0x80000000	@ Negative flag
.set	ZFlag, 0x40000000	@ Zero flag
.set	CFlag, 0x20000000	@ Carry flag
.set	VFlag, 0x10000000	@ Overflow flag
.set	IFlag, 0x08000000	@ IRQ disable
.set	FFlag, 0x04000000	@ FIRQ disable

.set	USR32_mode, 0b10000
.set	FIQ32_mode, 0b10001
.set	IRQ32_mode, 0b10010
.set	SVC32_Mode, 0b10011
.set	ABT32_Mode, 0b10111
.set	UND32_Mode, 0b11011
.set	SYS32_Mode, 0b11111

.set	IFlag32, 0x00000080

	@ Import the assembler helper macros.
	@ The script that translates AOF to ELF, will convert this line
	@ to refer to 'elf-macros.s' instead.
#include "internal/elf-macros.s"

.set	EXIT_SUCCESS, 0
.set	EXIT_FAILURE, 1

.set	TRUE, 1
.set	FALSE, 0

	@Keep the signal list in sync with <signal.h> contents.
.set	SIGHUP, 1	@   hangup
.set	SIGINT, 2	@   terminal interrupt
.set	SIGQUIT, 3	@ * ^\ from terminal
.set	SIGILL, 4	@ * illegal instruction
.set	SIGTRAP, 5	@ * trace trap - unused
.set	SIGIOT, 6	@   abort
.set	SIGABRT, 6	@   abort
.set	SIGEMT, 7	@ * EMT (h/ware error)
.set	SIGSTAK, SIGEMT
.set	SIGFPE, 8	@ * FPE trap
.set	SIGKILL, 9	@   kill signal
.set	SIGBUS, 10	@ * bus error
.set	SIGSEGV, 11	@ * segmentation violation
.set	SIGSYS, 12	@ * bad system call
.set	SIGPIPE, 13	@   broken pipe
.set	SIGALRM, 14	@   alarm call
.set	SIGTERM, 15	@   termination signal
.set	SIGURG, 16	@   urgent or out-of-band data
.set	SIGSTOP, 17	@   stop, unblockable
.set	SIGTSTP, 18	@   keyboard stop
.set	SIGCONT, 19	@   continue
.set	SIGCHLD, 20	@   child process has terminated/stopped
.set	SIGCLD, SIGCHLD
.set	SIGTTIN, 21	@   background read from tty
.set	SIGTTOU, 22	@   background write to tty
.set	SIGIO, 23	@   file descriptor is ready to perform I/O
.set	SIGPOLL, SIGIO
.set	SIGXCPU, 24	@   CPU time limit exceeded
.set	SIGXFSZ, 25	@   file size limit exceeded
.set	SIGVTALRM, 26	@   alarm call
.set	SIGPROF, 27	@   profiler alarm call
.set	SIGWINCH, 28	@   window size change
.set	SIGINFO, 29	@   information request
.set	SIGUSR1, 30	@   user signal 1
.set	SIGUSR2, 31	@   user signal 2
.set	SIGLOST, 32	@   resource lost
.set	SIGOSERROR, 33	@   RISC OS error

.set	EOPSYS, 88	@ RISC OS error

.set	SharedUnixLibrary_SWIChunk, 0x55c80	@Range 0x55c80 - &55cc0 (excl) is allocated.
.set	SharedUnixLibrary_ErrorChunk, 0x81a400	@Range 0x81a400 - &81a40 (excl) is allocated.
@ The beginning of the error range is used by the SUL module.  The end of
@ the error ranged (allocated from back to front) is used by the UnixLib
@ main code itself (to generate errors during startup when necessary).
.set	SharedUnixLibrary_Error_UnknownSWI, SharedUnixLibrary_ErrorChunk + 0x00
.set	SharedUnixLibrary_Error_UnknownKey, SharedUnixLibrary_ErrorChunk + 0x01
.set	SharedUnixLibrary_Error_StillActive, SharedUnixLibrary_ErrorChunk + 0x02
.set	SharedUnixLibrary_Error_TooOld, SharedUnixLibrary_ErrorChunk + 0x03
@ ...
.set	SharedUnixLibrary_Error_FatalError, SharedUnixLibrary_ErrorChunk + 0x3B
.set	SharedUnixLibrary_Error_NoFPE, SharedUnixLibrary_ErrorChunk + 0x3C
.set	SharedUnixLibrary_Error_NotRecentEnough, SharedUnixLibrary_ErrorChunk + 0x3D
.set	SharedUnixLibrary_Error_NotEnoughMem, SharedUnixLibrary_ErrorChunk + 0x3E
.set	SharedUnixLibrary_Error_NoCallASWI, SharedUnixLibrary_ErrorChunk + 0x3F

.set	OS_WriteS, 0x000001
.set	OS_NewLine, 0x000003
.set	OS_Exit, 0x000011
.set	OS_EnterOS, 0x000016
.set	OS_GenerateError, 0x00002B
.set	OS_CLI, 0x000005

.set	X_Bit, 0x20000

.set	XOS_WriteC, 0x000000 + X_Bit
.set	XOS_WriteS, 0x000001 + X_Bit
.set	XOS_Write0, 0x000002 + X_Bit
.set	XOS_NewLine, 0x000003 + X_Bit
.set	XOS_ReadC, 0x000004 + X_Bit
.set	XOS_CLI, 0x000005 + X_Bit
.set	XOS_Byte, 0x000006 + X_Bit
.set	XOS_Word, 0x000007 + X_Bit
.set	XOS_File, 0x000008 + X_Bit
.set	XOS_Args, 0x000009 + X_Bit
.set	XOS_BGet, 0x00000A + X_Bit
.set	XOS_BPut, 0x00000B + X_Bit
.set	XOS_GBPB, 0x00000C + X_Bit
.set	XOS_Find, 0x00000D + X_Bit
.set	XOS_ReadLine, 0x00000E + X_Bit
.set	XOS_Control, 0x00000F + X_Bit
.set	XOS_GetEnv, 0x000010 + X_Bit
.set	XOS_Exit, 0x000011 + X_Bit
.set	XOS_SetEnv, 0x000012 + X_Bit
.set	XOS_IntOn, 0x000013 + X_Bit
.set	XOS_IntOff, 0x000014 + X_Bit
.set	XOS_CallBack, 0x000015 + X_Bit
.set	XOS_EnterOS, 0x000016 + X_Bit
.set	XOS_BreakPt, 0x000017 + X_Bit
.set	XOS_BrealCtrl, 0x000018 + X_Bit

.set	XOS_UpdateMEMC, 0x00001A + X_Bit
.set	XOS_SetCallBack, 0x00001B + X_Bit
.set	XOS_Mouse, 0x00001C + X_Bit
.set	XOS_Heap, 0x00001D + X_Bit
.set	XOS_Module, 0x00001E + X_Bit
.set	XOS_ReadUnsigned, 0x000021 + X_Bit
.set	XOS_ReadVarVal, 0x000023 + X_Bit
.set	XOS_SetVarVal, 0x000024 + X_Bit
.set	XOS_BinaryToDecimal, 0x000028 + X_Bit
.set	XOS_FSControl, 0x000029 + X_Bit
.set	XOS_ChangeDynamicArea, 0x00002A + X_Bit
.set	XOS_ValidateAddress, 0x00003A + X_Bit
.set	XOS_CallAfter, 0x00003B + X_Bit
.set	XOS_CallEvery, 0x00003C + X_Bit
.set	XOS_RemoveTickerEvent, 0x00003D + X_Bit
.set	XOS_ChangeEnvironment, 0x000040 + X_Bit
.set	XOS_ReadMonotonicTime, 0x000042 + X_Bit
.set	XOS_WriteN, 0x000046 + X_Bit
.set	XOS_ReadMemMapInfo, 0x000051 + X_Bit
.set	XOS_AddCallBack, 0x000054 + X_Bit
.set	XOS_SerialOp, 0x000057 + X_Bit
.set	XOS_DynamicArea, 0x000066 + X_Bit
.set	XOS_PlatformFeatures, 0x00006D + X_Bit
.set	XOS_SynchroniseCodeAreas, 0x00006E + X_Bit
.set	XOS_CallASWI, 0x00006F + X_Bit
.set	XOS_AMBControl, 0x000070 + X_Bit
.set	XOS_CallASWIR12, 0x000071 + X_Bit
.set	XOS_ConvertHex8, 0x0000D4 + X_Bit
.set	XOS_ConvertInteger1, 0x0000D9 + X_Bit
.set	XOS_ConvertInteger2, 0x0000DA + X_Bit
.set	XOS_ConvertInteger3, 0x0000DB + X_Bit
.set	XOS_ConvertInteger4, 0x0000DC + X_Bit
.set	XOS_WriteI, 0x000100 + X_Bit

.set	XFPEmulator_Version, 0x040480 + X_Bit

.set	Socket_Creat, 0x41200
.set	Socket_Bind, 0x41201
.set	Socket_Listen, 0x41202
.set	Socket_Accept, 0x41203
.set	Socket_Connect, 0x41204
.set	Socket_Recv, 0x41205
.set	Socket_Recvfrom, 0x41206
.set	Socket_Recvmsg, 0x41207
.set	Socket_Send, 0x41208
.set	Socket_Sendto, 0x41209
.set	Socket_Sendmsg, 0x4120A
.set	Socket_Shutdown, 0x4120B
.set	Socket_Setsockopt, 0x4120C
.set	Socket_Getsockopt, 0x4120D
.set	Socket_Getpeername, 0x4120E
.set	Socket_Getsockname, 0x4120F
.set	Socket_Close, 0x41210
.set	Socket_Select, 0x41211
.set	Socket_Ioctl, 0x41212
.set	Socket_Read, 0x41213
.set	Socket_Write, 0x41214
.set	Socket_Stat, 0x41215
.set	Socket_Readv, 0x41216
.set	Socket_Writev, 0x41217
.set	Socket_Gettsize, 0x41218
.set	Socket_Sendtosm, 0x41219
.set	Socket_Sysctl, 0x4121A
.set	Socket_Accept_1, 0x4121B
.set	Socket_Recvfrom_1, 0x4121C
.set	Socket_Recvmsg_1, 0x4121D
.set	Socket_Sendmsg_1, 0x4121E
.set	Socket_Getpeername_1, 0x4121F
.set	Socket_Getsockname_1, 0x41220
.set	Socket_InternalLookup, 0x41221
.set	Socket_Version, 0x41222

.set	Internet_GetHostByName, 0x46000
.set	Internet_GetHostByAddr, 0x46001

.set	XSocket_Creat, Socket_Creat + X_Bit
.set	XSocket_Bind, Socket_Bind + X_Bit
.set	XSocket_Listen, Socket_Listen + X_Bit
.set	XSocket_Accept, Socket_Accept + X_Bit
.set	XSocket_Connect, Socket_Connect + X_Bit
.set	XSocket_Recv, Socket_Recv + X_Bit
.set	XSocket_Recvfrom, Socket_Recvfrom + X_Bit
.set	XSocket_Recvmsg, Socket_Recvmsg + X_Bit
.set	XSocket_Send, Socket_Send + X_Bit
.set	XSocket_Sendto, Socket_Sendto + X_Bit
.set	XSocket_Sendmsg, Socket_Sendmsg + X_Bit
.set	XSocket_Shutdown, Socket_Shutdown + X_Bit
.set	XSocket_Setsockopt, Socket_Setsockopt + X_Bit
.set	XSocket_Getsockopt, Socket_Getsockopt + X_Bit
.set	XSocket_Getpeername, Socket_Getpeername + X_Bit
.set	XSocket_Getsockname, Socket_Getsockname + X_Bit
.set	XSocket_Close, Socket_Close + X_Bit
.set	XSocket_Select, Socket_Select + X_Bit
.set	XSocket_Ioctl, Socket_Ioctl + X_Bit
.set	XSocket_Read, Socket_Read + X_Bit
.set	XSocket_Write, Socket_Write + X_Bit
.set	XSocket_Stat, Socket_Stat + X_Bit
.set	XSocket_Readv, Socket_Readv + X_Bit
.set	XSocket_Writev, Socket_Writev + X_Bit
.set	XSocket_Gettsize, Socket_Gettsize + X_Bit
.set	XSocket_Sendtosm, Socket_Sendtosm + X_Bit
.set	XSocket_Sysctl, Socket_Sysctl + X_Bit
.set	XSocket_Accept_1, Socket_Accept_1 + X_Bit
.set	XSocket_Recvfrom_1, Socket_Recvfrom_1 + X_Bit
.set	XSocket_Recvmsg_1, Socket_Recvmsg_1 + X_Bit
.set	XSocket_Sendmsg_1, Socket_Sendmsg_1 + X_Bit
.set	XSocket_Getpeername_1, Socket_Getpeername_1 + X_Bit
.set	XSocket_Getsockname_1, Socket_Getsockname_1 + X_Bit
.set	XSocket_InternalLookup, Socket_InternalLookup + X_Bit
.set	XSocket_Version, Socket_Version + X_Bit

.set	XInternet_GetHostByName, Internet_GetHostByName + X_Bit
.set	XInternet_GetHostByAddr, Internet_GetHostByAddr + X_Bit

.set	XSharedCLibrary_LibInitAPCS_R, 0x080681 + X_Bit
.set	XSharedCLibrary_LibInitModule, 0x080682 + X_Bit

.set	XMessageTrans_ErrorLookup, 0x041506 + X_Bit

.set	XTaskWindow_TaskInfo, 0x043380 + X_Bit
.set	XWimp_ReadSysInfo, 0x0400F2 + X_Bit
.set	XWimp_SlotSize, 0x0400EC + X_Bit
.set	XDDEUtils_SetCLSize, 0x042581 + X_Bit

.set	Wimp_SlotSize, 0x0400EC

.set	XFilter_RegisterPreFilter, 0x042640 + X_Bit
.set	XFilter_RegisterPostFilter, 0x042641 + X_Bit
.set	XFilter_DeRegisterPreFilter, 0x042642 + X_Bit
.set	XFilter_DeRegisterPostFilter, 0x042643 + X_Bit

.set	XSharedUnixLibrary_RegisterUpCall, 0x55c80 + X_Bit
.set	XSharedUnixLibrary_DeRegisterUpCall, 0x55c81 + X_Bit
.set	XSharedUnixLibrary_SetValue, 0x55c82 + X_Bit
.set	XSharedUnixLibrary_Count, 0x55c83 + X_Bit
.set	XSharedUnixLibrary_Initialise, 0x55c84 + X_Bit

.set	XSOM_GOTFromAddr, 0x5858a + X_Bit

	@ Entries into the __ul_global structure.  Must be kept in sync with
	@ sys/_syslib.s and incl-local/internal/{aof,elf}-macros.s.
.set	GBL_UNIXLIB_CLI, 0

.set	GBL_TIME_LOW, 4
.set	GBL_TIME_HIGH, 8

.set	GBL_NOTUSED1, 12
.set	GBL_TASKWINDOW, 16
.set	GBL_TASKHANDLE, 20
.set	GBL_DYNAMIC_NUM, 24
.set	GBL_OLD_U, 28
.set	GBL_32BIT, 32
.set	GBL_PANIC_MODE, 36
.set	GBL_PROC, 40
.set	GBL_UL_PAGESIZE, 44
.set	GBL_UPCALL_HANDLER_ADDR, 48
.set	GBL_UPCALL_HANDLER_R12, 52

.set	GBL_PTH_RETURN_ADDRESS, 56
.set	GBL_PTH_WORKSEMAPHORE, 60
.set	GBL_PTH_CALLBACK_SEMAPHORE, 64
.set	GBL_PTH_SYSTEM_RUNNING, 68
.set	GBL_PTH_CALLBACK_MISSED, 72
.set	GBL_PTH_NUM_RUNNING_THREADS, 76

.set	GBL_EXECUTING_SIGNALHANDLER, 80
.set	GBL_SIGNALHANDLER_SL, 84
.set	GBL_SIGNALHANDLER_SP, 88

.set	GBL_MUTEX, 92
.set	GBL_MALLOC_GBL, 96
.set	GBL_MAIN, 100

	@ Entries in the __ul_memory table.  Must be kept in sync with
	@ sys/_syslib.s and incl-local/internal/{aof,elf}-macros.s
.set	MEM_APPSPACE_HIMEM, 4
.set	MEM_UNIXLIB_STACK, 8

.set	MEM_ROBASE, 12
.set	MEM_RWLOMEM, 16
.set	MEM_RWBASE, 20
.set	MEM_RWBREAK, 24

.set	MEM_UNIXLIB_STACK_LIMIT, 28

.set	MEM_DALOMEM, 32
.set	MEM_DABREAK, 36
.set	MEM_DALIMIT, 40

.set	MEM_APPSPACE_LIMIT, 44
.set	MEM_OLD_HIMEM, 48

	@ Entries in the struct __stack_chunk.  Must be kept in sync with
	@ unix.h definition.
	@ First 20 bytes equals SCL's _kernel_stack_chunk structure :
.set	CHUNK_MAGIC, 0	@ Magic number to help detect if someone overwrites the stack
.set	CHUNK_NEXT, 4	@ Ptr to next chunk
.set	CHUNK_PREV, 8	@ Ptr to previous chunk
.set	CHUNK_SIZE, 12	@ Size of chunk, including header
.set	CHUNK_DEALLOC, 16	@ Function to call to free the chunk
.set	CHUNK_RETURN, 20	@ Return address after freeing this chunk

.set	CHUNK_OVERHEAD, 24	@ Size of chunk header

