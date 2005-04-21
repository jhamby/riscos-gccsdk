; This file contains the UnixLib build options and 
; gets included by C and ASM files so its syntax is 
; quite restricted. 

; Set to 1 if alloca() should call abort() if no memory can be allocated.  
; Otherwise set it to 0.  

__UNIXLIB_ALLOCA_FATAL EQU 		1

; Set to 1 if runtime paranoid checks need to be performed. Otherwise 
; set it to 0.  
__UNIXLIB_PARANOID EQU 		0

; Set to 1 if we should check that the entire stack chunk chain is 
; valid each time the stack is extended/shrunk. Otherwise set it to 0.  
__UNIXLIB_EXTREMELY_PARANOID EQU 	0

; Set to 1 if we should check that the magic number is valid each 
; time the stack is extended/shrunk. Otherwise set it to 0.  
__UNIXLIB_STACK_CHECK_MAGIC EQU 	1

; 
__UNIXLIB_USEFILEPATH EQU 		0

; Set to 1 if we want Internet 4 compatibility. 
__UNIXLIB_COMPAT_INET4 EQU 		0

; Set to 1 if DDEUtils' Prefix needs to be set at the beginning of 
; each process task. Note this hits bugs in some versions of RISC OS. 
__UNIXLIB_SET_DDEPREFIX EQU 	0

; Some ARM instructions are very handy and result in 
; increased performance but they are not supported on 
; all current ARM architectures. 

; Set to 1 if the SWP instruction is supported (not on ARM2). 
__UNIXLIB_SWP_SUPPORTED EQU 	1

; Set to 1 if the (U)MULL instructions can be used. 
__UNIXLIB_MULL_SUPPORTED EQU 	1

; Set to 1 if the CLZ instruction can be used. 
__UNIXLIB_CLZ_SUPPORTED EQU 	0

; Set to 1 if we want to support POSIX interval timers.  
__UNIXLIB_FEATURE_ITIMERS EQU 	1

; Set to 1 if we want support for pthreads. 
__UNIXLIB_FEATURE_PTHREADS EQU 	1

; Set to 1 if we want 'errno' to be per-thread rather than a global 
; variable.  It is currently inadvisable to set this option as many 
; applications rely on 'errno' being a real variable rather than a 
; macro.  GCC's builtin math functions also rely on this.  
__UNIXLIB_ERRNO_THREADED EQU 	0

; The APCS-32 ABI in GCC and Norcroft are not 100% 
; the same. 

; Set to 1 if we want to use the Norcroft ABI 
__UNIXLIB_NORCROFT_BUILD EQU 	0

; Here mainly as a placeholder to reduce the porting requirements 
; between AOF and ELF variants of the library.  Throughout 
; the C sources, we stick to __ELF__, but we can't explicitly 
; define that here, because that would class with GCC's  
; definition.  

; Set to 1 if we are building for ELF.  
__UNIXLIB_ELF EQU 	0
   END
