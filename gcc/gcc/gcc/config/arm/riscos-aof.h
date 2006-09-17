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

/* Extra command line options supported by the RISC OS backend.
   Currently bits 0 to 23 (incl) are in use by arm.h, we defined here
   a couple other onces.  */

/* Use library static data offset instead of client offset. */
#define ARM_LIBSTATICS		(0x04000000)
#define TARGET_LIBSTATICS	(target_flags & ARM_LIBSTATICS)

/* Create code suitable for a RISC OS module */
#define ARM_MODULE		(0x08000000)
#define TARGET_MODULE		(target_flags & ARM_MODULE)

/* Non-zero if we wish to format dependencies for Acorn's
   Make Utility.  */
#define ARM_AMU			(0x10000000)
#define TARGET_AMU		(target_flags & ARM_AMU)

/* Non-zero if we will allow a text editor to capture our errors.  */
#define ARM_THROWBACK		(0x20000000)
#define TARGET_THROWBACK	(target_flags & ARM_THROWBACK)

/* Non-zero if we are using the SharedCLibrary instead of UnixLib
   as Runtime Library for our compiled code.  */
#define ARM_SCL			(0x40000000)
#define TARGET_SCL		(target_flags & ARM_SCL)

/* Options not used in cc1 will have bit 31 set.  */

#define SUBTARGET_SWITCHES					\
  {"throwback",			 ARM_THROWBACK,			\
   "Send errors to a DDEUtils compatible text editor" },	\
  {"no-throwback",		-ARM_THROWBACK, "" },		\
  {"amu",			 ARM_AMU,	 		\
   "Format source dependencies for Acorn's Make Utility" },	\
  {"no-amu",			-ARM_AMU, "" },			\
  {"libscl",			 ARM_SCL,			\
   "Compile with the SharedCLibrary headers" },			\
  {"unixlib",			-ARM_SCL, 			\
   "Compile with the headers from UnixLib (default)" },		\
  {"module",			 ARM_MODULE,			\
   "Generate data references suitable for RISC OS modules" },	\
  {"no-module",			-ARM_MODULE, ""}, \
  {"library-static-data-offset", ARM_LIBSTATICS, \
   "Use library static data offset in RISC OS module code" }, \
  {"client-static-data-offset", -ARM_LIBSTATICS, \
   "Use client static data offset in RISC OS module code" },


/* Default RISC OS options
   - APCS
   - explicit stack checks */
#define TARGET_DEFAULT (ARM_FLAG_APCS_FRAME + ARM_FLAG_APCS_STACK + ARM_FLAG_APCS_32)

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

/* Add support of long doubles.  Undefine for better compatibility with
   Norcroft C.
   Keep undefined at the moment due to incompatibilities between
   the various floating point emulators and some other stuff.  */
#undef ARM_LONG_DOUBLE_SUPPORT

/* Unsigned chars produces much better code than signed.  */
#define DEFAULT_SIGNED_CHAR  0

/* Compare with the version found at gbl-ctors.h : this one uses a
   static variable to have the start of __CTOR_LIST__ which is the
   only way to have a correct value when RISC OS module code is
   produced (otherwise the __CTOR_LIST__ data array will be accessed
   indirectly via [sl, #-586] and contain garbage).
   The __DTOR_LIST__ handling is at libgcc/libgcc2.c and contains
   a similar way to make things work.  */
#define DO_GLOBAL_CTORS_BODY						\
do {									\
  static func_ptr *p = __CTOR_LIST__;					\
  unsigned nptrs, i;							\
  for (nptrs = 1; p[nptrs] != NULL; nptrs++);				\
  for (i = nptrs; i > 1; i--)						\
    (p[i-1])();								\
} while (0)

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
#ifdef TARGET_CPU
#define SUBTARGET_CPU_DEFAULT TARGET_CPU
#else
#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm6
#endif

/* Tune for XScale.  */
#define TARGET_TUNE_DEFAULT TARGET_CPU_xscale

/* The minimum FPEmulator we now support is 4.00, which has
   full support for architecture 3.  */
#define FPUTYPE_DEFAULT FPUTYPE_FPA_EMU3

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

/* For RMA code we force string constants into the data section.  This is
   bogus but works around a problem in legitimize_pic_address where we have
   to add a displacement to the referenced variable.  At that time, it is
   not possible to reliably determine whether a SYMBOL_REF points to a
   read-only code section or a data section.  */
#define READONLY_DATA_SECTION_ASM_OP \
  ((TARGET_MODULE) ? aof_data_section () : aof_rodata_section ())

#define BSS_SECTION_ASM_OP "\tAREA\t|C$$data|, DATA"


/* NAB test */
#define SUPPORTS_ONE_ONLY 1

#define EH_FRAME_IN_DATA_SECTION 1
#define EH_FRAME_SECTION_NAME ".eh_frame"

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

#undef  TARGET_ASM_SELECT_SECTION
#define TARGET_ASM_SELECT_SECTION arm_aof_select_section

#define EXTRA_SECTIONS in_zero_init, in_common, in_comdef

#define EXTRA_SECTION_FUNCTIONS	\
ZERO_INIT_SECTION		\
COMMON_SECTION			\
COMDEF_SECTION

#define ZERO_INIT_SECTION					\
void								\
zero_init_section ()						\
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
common_section ()						\
{								\
  if (in_section != in_common)					\
    {								\
      in_section = in_common;					\
    }								\
}

/* Used by arm_aof_select_section() which filters out the COMDEF
   area and is using comdef_section() to tell varasm.c that we're
   in a COMDEF area/section now.  */
#define COMDEF_SECTION						\
void								\
comdef_section (void)						\
{								\
  if (in_section != in_comdef)					\
    {								\
      in_section = in_comdef;					\
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


/* Some systems use __main in a way incompatible with its use in gcc, in these
   cases use the macros NAME__MAIN to give a quoted symbol and SYMBOL__MAIN to
   give the same symbol without quotes for an alternative entry point.  You
   must define both, or neither. */
#define NAME__MAIN "__gccmain"
#define SYMBOL__MAIN __gccmain

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
	     (i + 1 < (long)(LEN)			\
	      ? ((i & 3) == 3 ? "\n\tDCB" : ",")	\
	      : "\n"));					\
}

#define IS_ASM_LOGICAL_LINE_SEPARATOR(C) ((C) == '\n')

/* Output of Uninitialized Variables */
#define ASM_OUTPUT_COMMON(STREAM,NAME,SIZE,ROUNDED)		\
  do								\
    {								\
     common_section ();						\
     fprintf ((STREAM), "\tAREA |%s|, DATA, NOINIT, COMMON\n", (NAME));	\
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
     fprintf ((STREAM), "\tAREA |%s|, DATA, NOINIT, COMMON\n", (NAME));	\
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
  fputc ('\n', STREAM);			\
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

/* This is how we tell the assembler that two symbols have the same value.  */
#define ASM_OUTPUT_DEF(FILE, NAME1, NAME2) \
  do					   \
    {					   \
      assemble_name (FILE, NAME1); 	   \
      fputs (" EQU ", FILE);		   \
      assemble_name (FILE, NAME2);	   \
      fputc ('\n', FILE);		   \
    }					   \
  while (0)

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
 ((OUTVAR) = (char *) alloca (strlen ((NAME)) + 10),	\
  sprintf ((OUTVAR), "%s.%d", (NAME), (NUMBER)))


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
    fprintf ((STREAM), "\tALIGN\n");			\
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
#define LINK_SPEC "%{mmodule:-module}"
#define ASM_FINAL_SPEC ""

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

#ifdef CROSS_COMPILE
/* Cross-compiler specific definitions.  */

/* Look for UnixLib unless mlibscl is set, then look for SharedCLib
   stubs.  */
#define LIB_SPEC "%{!nostdlib:%{!mlibscl:-lunixlib}%{mlibscl:-lscl}}"

#define SUBTARGET_CPP_SPEC "\
	%{mlibscl:-icrossdirafter /libscl} \
	%{!mlibscl:-icrossdirafter /unixlib} \
	-D__JMP_BUF_SIZE=26 \
	%{posix:-D_POSIX_SOURCE}"

#else

/* Look for UnixLib unless mlibscl is set, then look for SharedCLib
   stubs.  */
#define LIB_SPEC "%{!nostdlib:%{!mlibscl:-lunixlib}%{mlibscl:-lscl}}"

#define LIBGCC_SPEC "%{!nostdlib:-lgcc}"

#define SUBTARGET_CPP_SPEC "%{mamu:-MD !Depend} \
	%{mlibscl:-icrossdirafter /libscl} \
	%{!mlibscl:-icrossdirafter /unixlib} \
	-D__JMP_BUF_SIZE=26 \
	%{posix:-D_POSIX_SOURCE}"

#endif /* CROSS_COMPILE */

/* Options to pass through to the assembler.  */
#undef ASM_SPEC
#define ASM_SPEC "%{!mcpu=*:%{march=armv3:-t ARM6 -apcsfpv3; \
	march=armv3m:-t ARM7M -apcsfpv3; march=armv4:-t SA110 -apcsfpv3; \
	march=armv5te:-t XSCALE -apcsfpv3; :-t ARM6 -apcsfpv3}} \
	%{mcpu=arm6:-t ARM6 -apcsfpv3} \
	%{mcpu=arm7:-t ARM7 -apcsfpv3} \
	%{mcpu=strongarm:-t SA110 -apcsfpv3} \
	%{mcpu=xscale:-t XSCALE -apcsfpv3} \
	%{mfp=3:-apcsfpv3} %{mfpe=3:-apcsfpv3} \
	%{msoft-float:-soft-float} \
	%{mthrowback:-throwback} \
	%{mmodule:-module} \
	%{mcpu=arm2:-apcs26; mcpu=arm250:-apcs26; mcpu=arm3:-apcs26; \
	march=armv2:-apcs26; march=armv2a:-apcs26; mapcs-*:-apcs%*; :-apcs32} \
	%{mapcs-*:-apcs%*} \
	%{I*}"

#define TARGET_OS_CPP_BUILTINS()		\
    do {					\
	builtin_define ("__riscos");		\
	builtin_define ("__arm");		\
	builtin_define ("__riscos__");		\
	builtin_define ("__arm__");		\
	builtin_define ("__aof__");		\
    } while (0)

/* This is how we tell the assembler that a symbol is weak.  Though
   we can specify a symbol as being weak using the method defined below,
   it doesn't quite work.

   The problem might be that the IMPORT, WEAK appears at the end of the
   assembler file, rather than at the start.  So the assembler has already
   encoded the symbol and therefore ignores the IMPORT attribute.

   Something to investigate on a rainy day perhaps.  */
#if 0
#undef ASM_WEAKEN_LABEL
#define ASM_WEAKEN_LABEL(FILE, NAME)    \
  do                                    \
    {                                   \
      fputs ("\tIMPORT\t", (FILE));     \
      assemble_name ((FILE), (NAME));   \
      fputs (", WEAK\n", (FILE));       \
      aof_delete_import ((NAME));	\
    }                                   \
  while (0)

#define MAKE_DECL_ONE_ONLY(DECL) (DECL_WEAK (DECL) = 1)
#endif

#undef DOLLARS_IN_IDENTIFIERS
#define DOLLARS_IN_IDENTIFIERS 0

#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "munixlib", "mapcs-32", "!mmodule" }

/* The backend assembler used for RISC OS does not support debugging data
   - this might change in the future (but don't bet on it).  */
#ifdef DBX_DEBUGGING_INFO
#undef DBX_DEBUGGING_INFO
#endif

#ifdef DWARF2_DEBUGGING_INFO
#undef DWARF2_DEBUGGING_INFO
#endif

#define DBX_REGISTER_NUMBER(REGNO) (REGNO)

#undef INITIALIZE_TRAMPOLINE
#undef CLEAR_INSN_CACHE

/* Emit RTL insns to initialize the variable parts of a trampoline.
   FNADDR is an RTX for the address of the function's pure code.
   CXT is an RTX for the static chain value for the function.  */
#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT)		      \
{								      \
  rtx end_addr;                                                       \
                                                                      \
  emit_move_insn (gen_rtx_MEM (SImode, plus_constant ((TRAMP), 8)),   \
                (CXT));                                               \
  emit_move_insn (gen_rtx_MEM (SImode, plus_constant ((TRAMP), 12)),  \
                (FNADDR));                                            \
  /* Flush the instruction cache.  */                                 \
  end_addr = memory_address (SImode, plus_constant ((TRAMP), 16));    \
  emit_library_call (gen_rtx (SYMBOL_REF, Pmode, "__clear_icache"),   \
                   0, VOIDmode, 2,                                    \
                   memory_address (SImode, (TRAMP)), Pmode,           \
                   end_addr, Pmode);                                  \
}

/* A C expression used to clear the instruction cache from address
   BEG to address END.  */
#define CLEAR_INSN_CACHE(BEG, END) __clear_icache (BEG, END)
