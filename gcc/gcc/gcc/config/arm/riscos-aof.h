/* Operating system specific defines to be used when targetting GCC
   for RISC OS.
   Copyright (C) 1998, 1999, 2000 Free Software Foundation, Inc.
   Contributed by Nick Burrett (nick.burrett@btinternet.com)

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* This overrides all the paths that cpp attempts to use, as otherwise we
   need to specifiy the same path several times.  */

#ifndef CROSS_COMPILE
/* Structure format: directory name, for C++ only, C++ aware.  */

/* ***** These five defines should be in the Makefile ***** */

#ifndef DEFAULT_TARGET_MACHINE
#define DEFAULT_TARGET_MACHINE "arm-riscos-aof"
#endif

#ifndef DEFAULT_TARGET_VERSION
#define DEFAULT_TARGET_VERSION "2_95_2"
#endif

/* Default prefix to try when searching for startup files.  */
#ifndef STANDARD_STARTFILE_PREFIX
#define STANDARD_STARTFILE_PREFIX "startup/"
#endif

/* Default prefix to try when searching for compiler executable files.  */
#ifndef STANDARD_EXEC_PREFIX
#define STANDARD_EXEC_PREFIX "gccpkg:bin/"
#endif

#ifndef GCC_INCLUDE_DIR
#define GCC_INCLUDE_DIR "gccpkg:lib/gcc-lib/arm-riscos-aof/2_95_2/include"
#endif

#ifndef GPLUSPLUS_INCLUDE_DIR
#define GPLUSPLUS_INCLUDE_DIR "gccpkg:lib/gcc-lib/arm-riscos-aof/2_95_2/include/g++-3"
#endif

#ifndef LOCAL_INCLUDE_DIR
#define LOCAL_INCLUDE_DIR "gcclcl:include"
#endif

#ifndef SYSTEM_INCLUDE_DIR
#define SYSTEM_INCLUDE_DIR "gccsys:include"
#endif

#ifndef TARGET_NAME
#define TARGET_NAME "arm-riscos-aof"
#endif

#define INCLUDE_DEFAULTS			\
{						\
  { GPLUSPLUS_INCLUDE_DIR, "G++", 1, 1},	\
  { LOCAL_INCLUDE_DIR, 0, 0, 1}, 		\
  { GCC_INCLUDE_DIR, "GCC", 0, 1},		\
  { SYSTEM_INCLUDE_DIR, 0, 0, 1},		\
  { 0, 0, 0, 0 }				\
}

#define PATH_SEPARATOR ','

#endif /* !CROSS_COMPILE */

/* Descriptions for the assembler output format.  */

/* This is used in ASM_FILE_START.  */
#define ARM_OS_NAME "RISC OS"

/* Stack overflow checking functions.  */
#define ARM_STKOVF_SPLIT_SMALL "__rt_stkovf_split_small"
#define ARM_STKOVF_SPLIT_BIG "__rt_stkovf_split_big"



/* Run-time Target Specification.  */

#define TARGET_VERSION \
  fputs (" (ARM/RISC OS with AOF)", stderr)

/* Extra command line options supported by the RISC OS backend.  */

/* Non-zero if we allow the argument pointer (fp) to be used by
   the register allocator. Note this option is really only useful
   when not targetting the APCS.  */
#define ARM_USE_AP		(0x10000000)
#define TARGET_USE_AP		(target_flags & ARM_USE_AP)

/* Non-zero if we wish to format dependencies for Acorn's
   Make Utility.  */
#define ARM_AMU			(0x20000000)
#define TARGET_AMU		(target_flags & ARM_AMU)

/* Non-zero if we will allow a text editor to capture our errors.  */
#define ARM_THROWBACK		(0x40000000)
#define TARGET_THROWBACK	(target_flags & ARM_THROWBACK)

/* Options not used in cc1 will have bit 31 set.  */

#define SUBTARGET_SWITCHES					\
  {"use-arg-pointer",		 ARM_USE_AP,			\
   "Make fp (r11) available to the register allocator" },	\
  {"no-use-arg-pointer",	-ARM_USE_AP, "" },		\
  {"throwback",			 ARM_THROWBACK,			\
   "Send errors to a DDEUtils compatible text editor" },	\
  {"no-throwback",		-ARM_THROWBACK, "" },		\
  {"amu",			 ARM_AMU,	 		\
   "Format source dependencies for Acorn's Make Utility" },	\
  {"no-amu",			-ARM_AMU, "" },			\
  {"libscl",			 0x80000000,			\
   "Compile with the SharedCLibrary headers" },			\
  {"unixlib",			-0x80000000, 			\
   "Compile with the headers from UnixLib (default)" },

/* Default RISC OS options
   - APCS
   - explicit stack checks */
#define TARGET_DEFAULT (ARM_FLAG_APCS_FRAME + ARM_FLAG_APCS_STACK)

/* Prevent FRAME_POINTER_REGNUM being clobbered in functions that call
   alloca.  */
#define PREVENT_FP_CLOBBER (TARGET_APCS_STACK)

/* If we're targeting explicit APCS stack checks, then force calls to
   __builtin_return_address and __builtin_frame_address as library
   function calls.  */
#define HAVE_BUILTIN_RETURN_ADDR_FUNC (TARGET_APCS_STACK)
#define HAVE_BUILTIN_FRAME_ADDR_FUNC (TARGET_APCS_STACK)

#define SUBTARGET_OVERRIDE_OPTIONS					\
{									\
  if (write_symbols != NO_DEBUG)					\
    warning ("-g under RISC OS does not give useful debugging data");	\
  /* Floating point tuning. ARM 6, ARM 7 and StrongARM were */		\
  /* released for RISC OS 3.5 and above and this included an */		\
  /* architecture 3 floating point emulator.  */			\
  if (arm_is_strong || arm_is_6_or_7)					\
    arm_fpu_arch = FP_SOFT3;						\
}

#define SUBTARGET_CONDITIONAL_REGISTER_USAGE				\
{									\
  if (TARGET_USE_AP)							\
    {									\
      fixed_regs[11] = 0;						\
      call_used_regs[11] = 0;						\
    }									\
}

/* Add support of long doubles.  Undefine for better compatibility with
   Norcroft C.
   Keep undefined at the moment due to incompatibilities between
   the various floating point emulators and some other stuff.  */
#undef ARM_LONG_DOUBLE_SUPPORT


/* Unsigned chars produces much better code than signed.  */
#define DEFAULT_SIGNED_CHAR  0

/* Some systems use __main in a way incompatible with its use in gcc, in
   these cases use the macros NAME__MAIN to give a quoted symbol and
   SYMBOL__MAIN to give the same symbol without quotes for an alternative
   entry point.  You must define both, or neither.  */
#define NAME__MAIN "__gccmain"
#define SYMBOL__MAIN __gccmain

/* In RISC OS:
      size_t is "unsigned int"
      ptrdiff_t is "int"
      wchar_t is "int".  */
#define SIZE_TYPE "unsigned int"
#define PTRDIFF_TYPE "int"
#define WCHAR_TYPE "int"

/* Size in bits of the data type for wide characters.  */
#define WCHAR_TYPE_SIZE 32
#define MAX_WCHAR_TYPE_SIZE 32

/* Override the normal default CPU.  */
#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm2

/* For AOF style output.  */
#include "arm/aof.h"

/* Define this if the AOF assemble cannot generate relocations for
   PIC addresses directly.  */
/* #define AOF_ASSEMBLER_PIC_PROBS */

/* This code handle the constructor/destructor tables required for C++.
   It relies on a feature in the AOF linker that sorts areas in the object
   file into alphabetical order.  In the gcc library, we have two areas for
   both the constructors and destructors, which are named so the linker
   brackets the data tables with these areas, thus setting up the complete
   tables.  Easy !  */

#undef CTORS_SECTION_ASM_OP
#define CTORS_SECTION_ASM_OP "AREA\t|C$$gnu_ctorsvec2|, DATA, READONLY"
#undef DTORS_SECTION_ASM_OP
#define DTORS_SECTION_ASM_OP "AREA\t|C$$gnu_dtorsvec2|, DATA, READONLY"

#undef CTOR_LIST_BEGIN
#undef CTOR_LIST_END
#undef DO_GLOBAL_CTORS_BODY
#undef DTOR_LIST_BEGIN
#undef DTOR_LIST_END
#undef DO_GLOBAL_DTORS_BODY

/* Translation to find startup files.  In RISC OS, everything
   is in one library.  */
#undef STARTFILE_SPEC
#define STARTFILE_SPEC  ""

#undef ENDFILE_SPEC
#define ENDFILE_SPEC ""

#undef LIBGCC_SPEC
#undef LINK_LIBGCC_SPECIAL
#undef LINK_LIBGCC_SPECIAL_1

/* RISC OS has some fun default libraries.  */
#undef LIB_SPEC

/* A C string constant that tells the GNU CC driver program options to
   pass to CPP.  It can also specify how to translate options you give
   to GNU CC into options for GNU CC to pass to the CPP.  */
#undef CPP_SPEC

/* Options to pass to the linker.  */
#undef LINK_SPEC
#define LINK_SPEC ""

#undef ASM_FINAL_SPEC
#define ASM_FINAL_SPEC ""


#ifdef CROSS_COMPILE
/* Cross-compiler specific definitions.  */

/* Look for UnixLib unless mlibscl is set, then look for SharedCLib
   stubs.  */
#define LIB_SPEC "%{!nostdlib:%{!mlibscl:-lunixlib}%{mlibscl:-lscl}}"

#define CPP_SPEC "%(cpp_cpu_arch) %(cpp_apcs_pc) %(cpp_float) \
        -D__JMP_BUF_SIZE=24 %{mlibscl:-D__TARGET_SCL__} \
	%{mlibscl:-icrossdirafter /libscl} \
	%{!mlibscl:-icrossdirafter /unixlib}"

#else
/* Library specs for RISC OS.  We look for Unixlib in the path Unix:.
   SharedCLibrary headers will be looked for in C:.  */
#define LIB_SPEC \
	"%D %{!nostdlib:%{!mlibscl:-LUnix: -lunixlib}" \
	"%{mlibscl:-LC: -lscl}}"

#define LIBGCC_SPEC "%{!nostdlib:-lgcc}"

#define CPP_SPEC "%(cpp_cpu_arch) %(cpp_apcs_pc) %(cpp_float) \
	%{mamu:-MD !Depend} \
	%{!mlibscl:-idirafter Unix:} %{mlibscl:-idirafter C:} \
	-D__JMP_BUF_SIZE=24 %{mlibscl:-D__TARGET_SCL__}"

#endif /* CROSS_COMPILE */

#undef CPP_PREDEFINES
#define CPP_PREDEFINES  \
	"-Darm -Driscos -Asystem(riscos) -Acpu(arm) -Amachine(acorn)"

/* Options to pass through to the assembler.  */
#undef ASM_SPEC
#define ASM_SPEC "%{mcpu=strongarm:-t SA110 -apcsfpv3} \
	%{mcpu=arm6:-t ARM6 -apcsfpv3} \
	%{mcpu=arm7:-t ARM7 -apcsfpv3} \
	%{mfp=3:-apcsfpv3} %{mfpe=3:-apcsfpv3} %{mapcs-32:-apcs32}"

#undef ASM_OUTPUT_COMMON
#if 0
#define ASM_OUTPUT_COMMON(STREAM,NAME,SIZE,ROUNDED)		\
  (common_section (),						\
   fprintf ((STREAM), "\tEXPORT\t"),				\
   assemble_name ((STREAM), (NAME)),				\
   fprintf ((STREAM), "\n\tAREA "),				\
   assemble_name ((STREAM), (NAME)),				\
   fprintf ((STREAM), ", DATA, COMMON\n\t%% %d\t%s size=%d\n",	\
	    (ROUNDED), ASM_COMMENT_START, SIZE),		\
   aof_delete_import ((NAME)))
#else
#define ASM_OUTPUT_COMMON(STREAM,NAME,SIZE,ROUNDED)		\
  (common_section (),						\
   fprintf ((STREAM), "\tAREA |Common$$%s|, DATA, COMMON\n",	\
            (NAME)),						\
   assemble_name ((STREAM), (NAME)),				\
   fprintf ((STREAM), "\n\t%% %d\t%s size=%d\n",		\
	    (ROUNDED), ASM_COMMENT_START, (SIZE)),		\
   aof_delete_import ((NAME)),					\
   fprintf ((STREAM), "\tEXPORT\t"),				\
   assemble_name ((STREAM), (NAME)),				\
   fputc ('\n', (STREAM)))
#endif

/* This is how we tell the assembler that a symbol is weak.  */
#undef ASM_WEAKEN_LABEL

/* Define if the architecture is an Acorn RiscPC or Archimedes. The
   RiscPC cannot support the half-word load/store instructions introduced
   by the StrongARM.  */
#undef ARM_ARCH_RISCPC
#define ARM_ARCH_RISCPC 1

#undef DOLLARS_IN_IDENTIFIERS
#define DOLLARS_IN_IDENTIFIERS 0
  
#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "munixlib" }

/* The backend assembler used for RISC OS does not support debugging data
   - this might change in the future (but don't bet on it).  */
#ifdef DBX_DEBUGGING_INFO
#undef DBX_DEBUGGING_INFO
#endif

/* #define DWARF2_DEBUGGING_INFO
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG */
