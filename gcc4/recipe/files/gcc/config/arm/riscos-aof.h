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

/* For AOF style output.  */
#define AOF_ASSEMBLER

/* This overrides all the paths that cpp attempts to use, as otherwise we
   need to specifiy the same path several times.  */

#ifndef CROSS_COMPILE
/* Structure format: directory name, for C++ only, C++ aware.  */

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

/* Default RISC OS options
   - APCS
   - explicit stack checks */
#define TARGET_DEFAULT (TARGET_APCS_FRAME + TARGET_APCS_STACK)

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
}

/* Unsigned chars produces much better code than signed.  */
#define DEFAULT_SIGNED_CHAR  0

/* Some systems use __main in a way incompatible with its use in gcc, in
   these cases use the macros NAME__MAIN to give a quoted symbol and
   SYMBOL__MAIN to give the same symbol without quotes for an alternative
   entry point.  You must define both, or neither.  */
#define NAME__MAIN "__gccmain"
#define SYMBOL__MAIN __gccmain
#define INVOKE__main

/* Override the normal default CPU.  */
#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm6

/* The minimum FPEmulator we now support is 4.00, which has
   full support for architecture 3.  */
#define FPUTYPE_DEFAULT FPUTYPE_FPA_EMU3

#undef  TARGET_DEFAULT_FLOAT_ABI
#define TARGET_DEFAULT_FLOAT_ABI ARM_FLOAT_ABI_HARD

/* RISC OS uses the APCS-32 ABI.  */
#undef ARM_DEFAULT_ABI
#define ARM_DEFAULT_ABI ARM_ABI_APCS32

/* Dividing the Output into Sections (Text, Data, ...) */
/* AOF Assembler syntax is a nightmare when it comes to areas, since once
   we change from one area to another, we can't go back again.  Instead,
   we must create a new area with the same attributes and add the new output
   to that.  Unfortunately, there is nothing we can do here to guarantee that
   two areas with the same attributes will be linked adjacently in the
   resulting executable, so we have to be careful not to do pc-relative 
   addressing across such boundaries.  */
#define TEXT_SECTION_ASM_OP aof_text_section ()
#define DATA_SECTION_ASM_OP aof_data_section ()
#define READONLY_DATA_SECTION_ASM_OP aof_rodata_section ()
#define BSS_SECTION_ASM_OP "\tAREA\t|C$$data|, DATA"

#define MAKE_DECL_ONE_ONLY(DECL) (DECL_WEAK (DECL) = 1)

/* Weak symbol testing.  */

/* This is how we tell the assembler that a symbol is weak.  */
#define SUPPORTS_WEAK 1
#define ASM_WEAKEN_LABEL(FILE, NAME)    \
  do                                    \
    {                                   \
      fputs ("\tIMPORT\t", (FILE));	\
      assemble_name ((FILE), (NAME));   \
      fputs (", WEAK\n", (FILE));	\
    }                                   \
  while (0)

/* This code handle the constructor/destructor tables required for C++.
   It relies on a feature in the AOF linker that sorts areas in the object
   file into alphabetical order.  In the gcc library, we have two areas for
   both the constructors and destructors, which are named so the linker
   brackets the data tables with these areas, thus setting up the complete
   tables.  Easy !  */
#define CTORS_SECTION_ASM_OP "\tAREA\t|C$$gnu_ctorsvec2|, DATA, READONLY"
#define DTORS_SECTION_ASM_OP "\tAREA\t|C$$gnu_dtorsvec2|, DATA, READONLY"

#undef  TARGET_ASM_NAMED_SECTION
#define TARGET_ASM_NAMED_SECTION  arm_aof_asm_named_section

#define EXTRA_SECTIONS in_zero_init, in_common

#define EXTRA_SECTION_FUNCTIONS	\
ZERO_INIT_SECTION		\
COMMON_SECTION

#define ZERO_INIT_SECTION					\
void								\
zero_init_section (void)						\
{								\
  static int zero_init_count = 1;				\
  if (in_section != in_zero_init)				\
    {								\
      fprintf (asm_out_file, "\tAREA\t|C$$zidata%d|, NOINIT\n",	\
	       zero_init_count++);				\
      in_section = in_zero_init;				\
    }								\
}

/* Used by ASM_OUTPUT_COMMON (below) to tell varasm.c that we've
   changed areas.  */
#define COMMON_SECTION						\
void								\
common_section (void)						\
{								\
  if (in_section != in_common)					\
    {								\
      in_section = in_common;					\
    }								\
}

/* We really want to put Thumb tables in a read-only data section, but
   switching to another section during function output is not
   possible.  We could however do what the SPARC does and defer the
   whole table generation until the end of the function.  */
#define JUMP_TABLES_IN_TEXT_SECTION 1

#ifndef ARM_OS_NAME
#define ARM_OS_NAME "(generic)"
#endif

#define ASM_COMMENT_START ";"
#define TARGET_ASM_FILE_START_APP_OFF false
#define TARGET_ASM_FILE_START_FILE_DIRECTIVE false

#define ASM_OUTPUT_ASCII(STREAM,PTR,LEN)		\
{							\
  int i;						\
  const char *ptr = (PTR);				\
  fprintf ((STREAM), "\tDCB");				\
  for (i = 0; i < (long)(LEN); i++)			\
    fprintf ((STREAM), " &%02x%s", 			\
	     (unsigned ) *(ptr++),			\
	     (i + 1 < (long)(LEN)				\
	      ? ((i & 3) == 3 ? "\n\tDCB" : ",")	\
	      : "\n"));					\
}

#define IS_ASM_LOGICAL_LINE_SEPARATOR(C) ((C) == '\n')

/* Output of Uninitialized Variables */
#define ASM_OUTPUT_COMMON(STREAM,NAME,SIZE,ROUNDED)		\
  do								\
    {								\
     common_section ();						\
     fprintf ((STREAM), "\tAREA |%s|, DATA, COMMON\n", (NAME));	\
     fprintf ((STREAM), "\tEXPORT |%s|\n", (NAME));		\
     fprintf ((STREAM), "\t%% %d\t%s size=%d\n",		\
	      (ROUNDED), ASM_COMMENT_START, (SIZE));		\
     aof_delete_import ((NAME));				\
    }								\
  while (0)

/* Place uninitialised global data in the common section.  */ 
#define ASM_OUTPUT_BSS(STREAM,DECL,NAME,SIZE,ROUNDED)		\
  do								\
    {								\
     common_section ();						\
     fprintf ((STREAM), "\tAREA |%s|, DATA, COMMON\n", (NAME));	\
     fprintf ((STREAM), "\tEXPORT |%s|\n", (NAME));		\
     fprintf ((STREAM), "\t%% %d\t%s size=%d\n",		\
	      (ROUNDED), ASM_COMMENT_START, (SIZE));		\
     aof_delete_import ((NAME));				\
    }								\
  while (0)

#define ASM_OUTPUT_LOCAL(STREAM,NAME,SIZE,ROUNDED)	\
   (zero_init_section (),				\
    assemble_name ((STREAM), (NAME)),			\
    fprintf ((STREAM), "\n"),				\
    fprintf ((STREAM), "\t%% %d\t%s size=%d\n",		\
	     (ROUNDED), ASM_COMMENT_START, SIZE))

/* Output and Generation of Labels */

extern int arm_main_function;

/* Globalizing directive for a label.  */
#define GLOBAL_ASM_OP "\tEXPORT\t"

#define ASM_OUTPUT_LABEL(STREAM,NAME)	\
do {					\
  assemble_name (STREAM,NAME);		\
  fputs ("\n", STREAM);			\
} while (0)

/* Encode a function name before the prologue.  RISC OS can use this
   to provide a more informative stack-backtrace.  Prevent this for
   linkonce sections because the common areas must exactly match to be
   merged, which they won't if one is compiled with a function name
   and the other isn't.  */
#define ASM_DECLARE_FUNCTION_NAME(STREAM,NAME,DECL)	\
{							\
  if (TARGET_POKE_FUNCTION_NAME				\
      && in_text_section ())				\
    arm_poke_function_name ((STREAM), (NAME));		\
  ASM_OUTPUT_LABEL (STREAM, NAME);			\
  if (! TREE_PUBLIC (DECL))				\
    {							\
      fputs ("\tKEEP ", STREAM);			\
      ASM_OUTPUT_LABEL (STREAM, NAME);			\
    }							\
  aof_delete_import ((NAME));				\
}

#define ASM_DECLARE_OBJECT_NAME(STREAM,NAME,DECL) \
{						\
  ASM_OUTPUT_LABEL (STREAM, NAME);		\
  if (! TREE_PUBLIC (DECL))			\
    {						\
      fputs ("\tKEEP ", STREAM);		\
      ASM_OUTPUT_LABEL (STREAM, NAME);		\
    }						\
  aof_delete_import ((NAME));			\
}

#define ASM_OUTPUT_EXTERNAL(STREAM,DECL,NAME)	\
 aof_add_import ((NAME))

#define ASM_OUTPUT_EXTERNAL_LIBCALL(STREAM,SYMREF)	\
 (fprintf ((STREAM), "\tIMPORT\t"),			\
  assemble_name ((STREAM), XSTR ((SYMREF), 0)),		\
  fputc ('\n', (STREAM)))

#define ASM_OUTPUT_LABELREF(STREAM,NAME)	\
  fprintf ((STREAM), "|%s|", NAME)

#define ASM_GENERATE_INTERNAL_LABEL(STRING,PREFIX,NUM)	\
  sprintf ((STRING), "*|%s..%ld|", (PREFIX), (long)(NUM))

#define ASM_FORMAT_PRIVATE_NAME(OUTVAR,NAME,NUMBER)	\
 ((OUTVAR) = (char *) alloca (strlen ((NAME)) + 12),	\
  sprintf ((OUTVAR), "|%s.%d|", (NAME), (NUMBER)))


/* Output of Assembler Instructions */
#define REGISTER_NAMES                          \
{                                               \
  "a1", "a2", "a3", "a4",                       \
  "v1", "v2", "v3", "v4",                       \
  "v5", "v6", "sl", "fp",                       \
  "ip", "sp", "lr", "pc",                       \
  "f0", "f1", "f2", "f3",                       \
  "f4", "f5", "f6", "f7",                       \
  "cc", "sfp", "afp",                           \
  "mv0",   "mv1",   "mv2",   "mv3",             \
  "mv4",   "mv5",   "mv6",   "mv7",             \
  "mv8",   "mv9",   "mv10",  "mv11",            \
  "mv12",  "mv13",  "mv14",  "mv15",            \
  "wcgr0", "wcgr1", "wcgr2", "wcgr3",           \
  "wr0",   "wr1",   "wr2",   "wr3",             \
  "wr4",   "wr5",   "wr6",   "wr7",             \
  "wr8",   "wr9",   "wr10",  "wr11",            \
  "wr12",  "wr13",  "wr14",  "wr15"             \
}

#define ADDITIONAL_REGISTER_NAMES		\
{						\
  {"r0", 0}, {"a1", 0},				\
  {"r1", 1}, {"a2", 1},				\
  {"r2", 2}, {"a3", 2},				\
  {"r3", 3}, {"a4", 3},		      		\
  {"r4", 4}, {"v1", 4},				\
  {"r5", 5}, {"v2", 5},				\
  {"r6", 6}, {"v3", 6},				\
  {"r7", 7}, {"wr", 7},				\
  {"r8", 8}, {"v5", 8},				\
  {"r9", 9}, {"v6", 9},				\
  {"r10", 10}, {"sl", 10}, {"v7", 10},		\
  {"r11", 11}, {"fp", 11},			\
  {"r12", 12}, {"ip", 12}, 			\
  {"r13", 13}, {"sp", 13}, 			\
  {"r14", 14}, {"lr", 14},			\
  {"r15", 15}, {"pc", 15}			\
}

#define REGISTER_PREFIX ""
#define USER_LABEL_PREFIX ""
#define LOCAL_LABEL_PREFIX ""

/* AOF does not prefix user function names with an underscore.  */
#define ARM_MCOUNT_NAME "_mcount"
#define ASM_APP_ON ""
#define ASM_APP_OFF ""

/* Output of Dispatch Tables */

#define ASM_OUTPUT_ADDR_DIFF_ELT(STREAM,BODY,VALUE,REL)			\
  do {									\
    if (TARGET_ARM)							\
      fprintf ((STREAM), "\tb\t|L..%d|\n", (VALUE));			\
    else								\
      fprintf ((STREAM), "\tDCD\t|L..%d| - |L..%d|\n", (VALUE), (REL));	\
  } while (0)

#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM,VALUE)	\
  fprintf ((STREAM), "\tDCD\t|L..%d|\n", (VALUE))

/* A label marking the start of a jump table is a data label. */
#define ASM_OUTPUT_CASE_LABEL(STREAM,PREFIX,NUM,TABLE)	\
  fprintf ((STREAM), "\tALIGN\n|%s..%d|\n", (PREFIX), (NUM))

/* Assembler Commands for Alignment */

#define ASM_OUTPUT_SKIP(STREAM,NBYTES)		\
 fprintf ((STREAM), "\t%%\t%d\n", (NBYTES))

#define ASM_OUTPUT_ALIGN(STREAM,POWER)			\
do {							\
  register int amount = 1 << (POWER);			\
  if (amount == 2)					\
    fprintf ((STREAM), "\tALIGN 2\n");			\
  else if (amount == 4)					\
    fprintf ((STREAM), "\tALIGN\n");			\
  else							\
    fprintf ((STREAM), "\tALIGN %d\n", amount);		\
} while (0)

/* Define this if the AOF assemble cannot generate relocations for
   PIC addresses directly.  */
/* #define AOF_ASSEMBLER_PIC_PROBS */

/* Translation to find startup files.  In RISC OS, everything
   is in one library.  */
#define STARTFILE_SPEC  ""

#define ENDFILE_SPEC ""

/* Options to pass to the linker.  */
#define LINK_SPEC ""
#define ASM_FINAL_SPEC ""

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

#ifdef CROSS_COMPILE
/* Cross-compiler specific definitions.  */

/* Look for UnixLib unless mlibscl is set, then look for SharedCLib
   stubs.  */
#define LIB_SPEC "%{!nostdlib:%{!mlibscl:-lunixlib}%{mlibscl:-lscl}}"

#define SUBTARGET_CPP_SPEC "-D__JMP_BUF_SIZE=27 %{mlibscl:-D__TARGET_SCL__} \
	%{mlibscl:-icrossdirafter /libscl} \
	%{!mlibscl:-icrossdirafter /unixlib} \
	%{posix:-D_POSIX_SOURCE}"

#else

/* Look for UnixLib unless mlibscl is set, then look for SharedCLib
   stubs.  */
#define LIB_SPEC "%{!nostdlib:%{!mlibscl:-lunixlib}%{mlibscl:-lscl}}"

#define LIBGCC_SPEC "%{!nostdlib:-lgcc}"

#define SUBTARGET_CPP_SPEC "%{mamu:-MD !Depend} \
	%{mlibscl:-icrossdirafter /libscl} \
	%{!mlibscl:-icrossdirafter /unixlib} \
	-D__JMP_BUF_SIZE=27 %{mlibscl:-D__TARGET_SCL__} \
	%{posix:-D_POSIX_SOURCE}"

#endif /* CROSS_COMPILE */

/* Options to pass through to the assembler.  */
#undef ASM_SPEC
#define ASM_SPEC "%{mcpu=strongarm:-t SA110 -apcsfpv3} \
	%{mcpu=arm6:-t ARM6 -apcsfpv3} \
	%{mcpu=arm7:-t ARM7 -apcsfpv3} \
	%{mfp=3:-apcsfpv3} %{mfpe=3:-apcsfpv3} %{mapcs-32:-apcs32}"

#define TARGET_OS_CPP_BUILTINS()		\
    do {					\
	builtin_define_std ("riscos");		\
	builtin_define ("__AOF__");		\
    } while (0);

/* This is how we tell the assembler that a symbol is weak.  */
#undef ASM_WEAKEN_LABEL

#undef DOLLARS_IN_IDENTIFIERS
#define DOLLARS_IN_IDENTIFIERS 0

#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "munixlib", "archv3" }

/* The backend assembler used for RISC OS does not support debugging data
   - this might change in the future (but don't bet on it).  */
#ifdef DBX_DEBUGGING_INFO
#undef DBX_DEBUGGING_INFO
#endif

#define DWARF2_DEBUGGING_INFO 1
#ifndef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG
#endif

#define DBX_REGISTER_NUMBER(REGNO) (REGNO)

#undef INITIALIZE_TRAMPOLINE
#undef CLEAR_INSN_CACHE

  
/* A C expression used to clear the instruction cache from address
   BEG to address END.  */
#define CLEAR_INSN_CACHE(BEG, END) __clear_icache (BEG, END)

#define TARGET_RISCOSAOF

#undef HAVE_MMAP_ANON
#undef HAVE_MMAP_DEV_ZERO
