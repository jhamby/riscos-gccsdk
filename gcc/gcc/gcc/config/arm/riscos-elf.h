/* Definitions for ARM running RISC OS using ELF.
   Copyright (C) 2000 Free Software Foundation, Inc.
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

#ifndef CROSS_COMPILE
/* Structure format: directory name, for C++ only, C++ aware.  */

/* ***** These five defines should be in the Makefile ***** */

#ifndef DEFAULT_TARGET_MACHINE
#define DEFAULT_TARGET_MACHINE "arm-riscos-elf"
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
#define GCC_INCLUDE_DIR "gccpkg:lib/gcc-lib/arm-riscos-elf/2_95_2/include"
#endif

#ifndef GPLUSPLUS_INCLUDE_DIR
#define GPLUSPLUS_INCLUDE_DIR "gccpkg:lib/gcc-lib/arm-riscos-elf/2_95_2/include/g++-3"
#endif

#ifndef LOCAL_INCLUDE_DIR
#define LOCAL_INCLUDE_DIR "gcclcl:include"
#endif

#ifndef SYSTEM_INCLUDE_DIR
#define SYSTEM_INCLUDE_DIR "gccsys:include"
#endif

#ifndef TARGET_NAME
#define TARGET_NAME "arm-riscos-elf"
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

/* Run-time Target Specification.  */
#define TARGET_VERSION fputs (" (ARM RISC OS with ELF)", stderr)

/* Handle #pragma weak and #pragma pack.  */
#define HANDLE_SYSV_PRAGMA

/* Descriptions for the assembler output format.  */

/* This is used in ASM_FILE_START.  */
#define ARM_OS_NAME "RISC OS"

/* Stack overflow checking functions.  */
#define ARM_STKOVF_SPLIT_SMALL "__rt_stkovf_split_small"
#define ARM_STKOVF_SPLIT_BIG "__rt_stkovf_split_big"

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
  {"stubs",			 0x80000000,			\
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
#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm6


/* Allow #sccs in preprocessor.  */
#define SCCS_DIRECTIVE

/* Attach a special .ident directive to the end of the file to identify
   the version of GCC which compiled this code.  */
#define IDENT_ASM_OP 	".ident"

/* Output #ident as a .ident.  */
#define ASM_OUTPUT_IDENT(FILE, NAME) \
  fprintf (FILE, "\t%s\t\"%s\"\n", IDENT_ASM_OP, NAME);

#define ASM_IDENTIFY_GCC(FILE) /* nothing */
#define ASM_IDENTIFY_LANGUAGE(FILE)			\
 fprintf (FILE, "\t%s \"GCC (%s) %s\"\n", IDENT_ASM_OP,	\
	 lang_identify (), version_string)

/* Support const sections and the ctors and dtors sections for g++.
   Note that there appears to be two different ways to support const
   sections at the moment.  You can either #define the symbol
   READONLY_DATA_SECTION (giving it some code which switches to the
   readonly data section) or else you can #define the symbols
   EXTRA_SECTIONS, EXTRA_SECTION_FUNCTIONS, SELECT_SECTION, and
   SELECT_RTX_SECTION.  We do both here just to be on the safe side.  */
#define USE_CONST_SECTION	1

/* Support for Constructors and Destructors.  */
#define READONLY_DATA_SECTION() const_section ()

/* A default list of other sections which we might be "in" at any given
   time.  For targets that use additional sections (e.g. .tdesc) you
   should override this definition in the target-specific file which
   includes this file.  */
#define SUBTARGET_EXTRA_SECTIONS in_const,

/* A default list of extra section function definitions.  For targets
   that use additional sections (e.g. .tdesc) you should override this
   definition in the target-specific file which includes this file.  */
#define SUBTARGET_EXTRA_SECTION_FUNCTIONS	CONST_SECTION_FUNCTION

extern void text_section ();

#define CONST_SECTION_ASM_OP	".section\t.rodata"

#define CONST_SECTION_FUNCTION						\
void									\
const_section ()							\
{									\
  if (!USE_CONST_SECTION)						\
    text_section ();							\
  else if (in_section != in_const)					\
    {									\
      fprintf (asm_out_file, "%s\n", CONST_SECTION_ASM_OP);		\
      in_section = in_const;						\
    }									\
}

/* Switch into a generic section.
   This is currently only used to support section attributes.

   We make the section read-only and executable for a function decl,
   read-only for a const data decl, and writable for a non-const data decl.  */
#define ASM_OUTPUT_SECTION_NAME(FILE, DECL, NAME, RELOC) \
  fprintf (FILE, ".section\t%s,\"%s\",%%progbits\n", NAME, \
	   (DECL) && TREE_CODE (DECL) == FUNCTION_DECL ? "ax" : \
	   (DECL) && DECL_READONLY_SECTION (DECL, RELOC) ? "a" : "aw")

/* A C statement or statements to switch to the appropriate
   section for output of DECL.  DECL is either a `VAR_DECL' node
   or a constant of some sort.  RELOC indicates whether forming
   the initial value of DECL requires link-time relocations.  */
#define SELECT_SECTION(DECL,RELOC)					\
{									\
  if (TREE_CODE (DECL) == STRING_CST)					\
    {									\
      if (! flag_writable_strings)					\
	const_section ();						\
      else								\
	data_section ();						\
    }									\
  else if (TREE_CODE (DECL) == VAR_DECL)				\
    {									\
      if ((flag_pic && RELOC)						\
	  || !TREE_READONLY (DECL) || TREE_SIDE_EFFECTS (DECL)		\
	  || !DECL_INITIAL (DECL)					\
	  || (DECL_INITIAL (DECL) != error_mark_node			\
	      && !TREE_CONSTANT (DECL_INITIAL (DECL))))			\
	data_section ();						\
      else								\
	const_section ();						\
    }									\
  else									\
    const_section ();							\
}

/* A C statement or statements to switch to the appropriate
   section for output of RTX in mode MODE.  RTX is some kind
   of constant in RTL.  The argument MODE is redundant except
   in the case of a `const_int' rtx.  Currently, these always
   go into the const section.  */
#define SELECT_RTX_SECTION(MODE,RTX) const_section ()

/* On svr4, we *do* have support for the .init and .fini sections, and we
   can put stuff in there to be executed before and after `main'.  We let
   crtstuff.c and other files know this by defining the following symbols.
   The definitions say how to change sections to the .init and .fini
   sections.  This is the same for all known svr4 assemblers.  */
#define INIT_SECTION_ASM_OP	".section\t.init"
#define FINI_SECTION_ASM_OP	".section\t.fini"

/* This is how we tell the assembler that two symbols have the same
   value.  */
#define ASM_OUTPUT_DEF(FILE,NAME1,NAME2) \
  do { assemble_name (FILE, NAME1); 	 \
       fputs (" = ", FILE);		 \
       assemble_name (FILE, NAME2);	 \
       fputc ('\n', FILE); } while (0)

/* Make DWARF2 an option, but keep DBX as the default for now.
   Use -gdwarf-2 to turn on DWARF2.  */
#define DWARF2_DEBUGGING_INFO
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* Get the standard ELF stabs definitions.  */
#include "arm/elf.h"

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

#ifdef CROSS_COMPILE
/* Look for UnixLib unless mstubs is set, then look for stubs.  */
#define LIB_SPEC "%{!nostdlib:%{!mstubs:-lUnixLib}%{mstubs:-lstubs}}"
#else
/* Library specs for RISC OS.  We look for Unixlib in the path Unix:.
   SharedCLibrary headers will be looked for in C:.  */

#define LIB_SPEC \
	"%D %{!nostdlib:%{!mstubs:-LUnix: -lUnixLib}" \
	"%{mstubs:-LC: -lstubs}}"


#if 0
#define LIB_SPEC  \
	"%{!nostdlib:%{!mstubs:-LGCC: %{.cc:-LGPP:} -LUnix: -lUnixLib}" \
	"%{mstubs:-LGCC: -LC: -lstubs}}"
#endif

#define LIBGCC_SPEC "%{!nostdlib:%{!mstubs:-lgcc} %{mstubs:-lgcc_sh}}"

#endif /* CROSS_COMPILE */

#undef CPP_PREDEFINES
#define CPP_PREDEFINES  \
	"-Darm -Driscos -Asystem(riscos) -Acpu(arm) -Amachine(acorn) -D__ELF__ -Darm_elf"

/* A C string constant that tells the GNU CC driver program options to
   pass to CPP.  It can also specify how to translate options you give
   to GNU CC into options for GNU CC to pass to the CPP.  */

#undef CPP_SPEC

#ifdef CROSS_COMPILE
#define CPP_SPEC "%(cpp_cpu_arch) %(cpp_apcs_pc) %(cpp_float) \
        -D__JMP_BUF_SIZE=26"
#else
#define CPP_SPEC "%(cpp_cpu_arch) %(cpp_apcs_pc) %(cpp_float) \
	%{mamu:-MD !Depend} \
	%{!mstubs:-idirafter Unix:} %{mstubs:-idirafter C:} \
	-D__JMP_BUF_SIZE=26 %{mstubs:-D__TARGET_SCL__}"
#endif

/* Options to pass to the linker.  */
#undef LINK_SPEC
#define LINK_SPEC ""

#undef ASM_FINAL_SPEC
#define ASM_FINAL_SPEC ""

/* Options to pass through to the assembler.  */
#undef ASM_SPEC
#define ASM_SPEC "%{mcpu=strongarm:-t SA110} %{mcpu=arm6:-t ARM6} " \
	"%{mcpu=arm7:-t ARM7}"
