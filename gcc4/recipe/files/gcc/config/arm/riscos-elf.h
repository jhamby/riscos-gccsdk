/* Definitions for ARM running RISC OS using ELF
   Copyright (C) 1993, 1994, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005 Free Software Foundation, Inc.
   Contributed by Nick Burrett <nick@sqrt.co.uk>

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* elfos.h should have already been included.  Now just override
   any conflicting definitions and add any extras.  */

/* Run-time Target Specification.  */
#undef  TARGET_VERSION
#define TARGET_VERSION fputs (" (ARM RISC OS with ELF)", stderr)

/* Do not assume anything about header files.  */
#define NO_IMPLICIT_EXTERN_C

#undef  TARGET_DEFAULT_FLOAT_ABI
#define TARGET_DEFAULT_FLOAT_ABI ARM_FLOAT_ABI_HARD

#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm6

/* RISC OS FPE always understands FPA instructions.  */
#undef  FPUTYPE_DEFAULT
#define FPUTYPE_DEFAULT FPUTYPE_FPA_EMU3

/* RISC OS uses the APCS-32 ABI.  */
#undef ARM_DEFAULT_ABI
#define ARM_DEFAULT_ABI ARM_ABI_APCS32

#define SUBTARGET_EXTRA_LINK_SPEC " -m armelf_riscos -p %{!static:%{!fpic:-fPIC}} -L/DSOLib:lib %{fpic:-fpic}"

#undef  MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "mhard-float" }

#undef SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC "%{mlibscl:-D__TARGET_SCL__}%{!mlibscl:-D__TARGET_UNIXLIB__}"

/* #define SUBTARGET_CPP_SPEC \
	"%{mlibscl:-D__TARGET_SCL__} \
	%{mlibscl:-icrossdirafter /libscl} \
	%{!mlibscl:-icrossdirafter /unixlib}" */

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

/* Now we define the strings used to build the spec file.  */
#undef  LIB_SPEC
#define LIB_SPEC \
  "%{!nostdlib:%{!mlibscl:-lunixlib}%{mlibscl:-lscl}}"

#define LIBGCC_SPEC "-lgcc"

/* Specify an item that is to appear at the very beginning of the command
   given to the linker.  We're declaring __EH_FRAME_BEGIN__ as early as
   possible to ensure it appears at the start of the '.eh_frame' section.
   Otherwise, we can end up missing chunks of potentially interesting
   frame data.  */
#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crti-riscos.o%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtn-riscos.o%s"

#undef  LINK_SPEC
#define LINK_SPEC "%{h*} %{version:-v} \
   %{b} %{Wl,*:%*} \
   %{static:-Bstatic} \
   %{shared:-shared} \
   %{symbolic:-Bsymbolic} \
   %{rdynamic:-export-dynamic} \
   -X \
   %{mbig-endian:-EB}" \
   SUBTARGET_EXTRA_LINK_SPEC


/* Some systems use __main in a way incompatible with its use in gcc, in
   these cases use the macros NAME__MAIN to give a quoted symbol and
   SYMBOL__MAIN to give the same symbol without quotes for an alternative
   entry point.  You must define both, or neither.  */
#define NAME__MAIN "__gccmain"
#define SYMBOL__MAIN __gccmain
#define INVOKE__main

/* We can't use these until the ELFloader properly sets the stack
   register before passing control to the user-land app.  */
#undef INIT_SECTION_ASM_OP
#undef FINI_SECTION_ASM_OP
#undef HAVE_INIT_SECTION

#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
	builtin_define ("__riscos");		\
	builtin_define ("__riscos__");		\
	if (flag_pic)				\
	  {					\
		builtin_define ("__PIC__");	\
		builtin_define ("__pic__");	\
	  }					\
    }						\
  while (0)

/* This is how we tell the assembler that two symbols have the same value.  */
#define ASM_OUTPUT_DEF(FILE, NAME1, NAME2) \
  do					   \
    {					   \
      assemble_name (FILE, NAME1); 	   \
      fputs (" = ", FILE);		   \
      assemble_name (FILE, NAME2);	   \
      fputc ('\n', FILE);		   \
    }					   \
  while (0)

/* Call the function profiler with a given profile label.  */
#undef  ARM_FUNCTION_PROFILER
#define ARM_FUNCTION_PROFILER(STREAM, LABELNO)  			\
{									\
  fprintf (STREAM, "\tbl\tmcount%s\n", NEED_PLT_RELOC ? "(PLT)" : "");	\
}

/* The linux profiler clobbers the link register.  Make sure the
   prologue knows to save it.  */
#define PROFILE_HOOK(X)						\
  emit_insn (gen_rtx_CLOBBER (VOIDmode, gen_rtx_REG (SImode, LR_REGNUM)))

#undef  CC1_SPEC
#define CC1_SPEC "%{profile:-p}"

#define LINK_GCC_C_SEQUENCE_SPEC \
  "%{static:--start-group} %G %L %{static:--end-group}%{!static:%G}"

/* Use --as-needed -lgcc_s for eh support.  */
#ifdef HAVE_LD_AS_NEEDED
#define USE_LD_AS_NEEDED 1
#endif

/* Descriptions for the assembler output format.  */

/* This is used in ASM_FILE_START.  */
#define ARM_OS_NAME "RISC OS"

/* Stack overflow checking functions.  */
#define ARM_STKOVF_SPLIT_SMALL "__rt_stkovf_split_small"
#define ARM_STKOVF_SPLIT_BIG "__rt_stkovf_split_big"

#define	ARM_LOAD_PIC "__rt_load_pic"


/* Default RISC OS options
   - APCS
   - explicit stack checks */
#undef  TARGET_DEFAULT
/* ARM_FLAG_APCS_STACK */
#define TARGET_DEFAULT (MASK_APCS_FRAME + MASK_POKE_FUNCTION_NAME + MASK_APCS_STACK)

/* Prevent FRAME_POINTER_REGNUM being clobbered in functions that call
   alloca.  */
#define PREVENT_FP_CLOBBER (TARGET_APCS_STACK)

#define TARGET_RISCOSELF
#define TARGET_RISCOSAOF

/* Override configure checks for mmap() compatibility.  Our C library
   partly provides these features, but they do not work in a way
   that the garbage collector expects.  */
#undef HAVE_MMAP_ANON
#undef HAVE_MMAP_DEV_ZERO

/* Clear the instruction cache from `beg' to `end'.  This makes an
   inline system call to XOS_SynchroniseCodeAreas.  */
#undef CLEAR_INSN_CACHE
#define CLEAR_INSN_CACHE(BEG, END)                                      \
{                                                                       \
  register unsigned long _beg __asm ("r1") = (unsigned long) (BEG);     \
  register unsigned long _end __asm ("r2") = (unsigned long) (END);     \
  register unsigned long _flg __asm ("r0") = 1;                         \
  __asm __volatile ("swi 0x2006e  @ XOS_SynchroniseCodeAreas"           \
                    : "=r" (_beg)                                       \
                    : "0" (_beg), "r" (_end), "r" (_flg));              \
}


#ifndef CROSS_COMPILE
/* This section defines all the specific features for GCC when running
   natively on RISC OS.  */

/* For the throwback of GCC errors to a text editor.  */
extern void arm_error_throwback (const char *file, int line,
                                 const char *prefix, const char *s);
extern void arm_throwback_finish (void);
extern void riscos_host_initialisation (void);
extern const char *riscos_convert_filename (void *obstack,
                        const char *name, int do_exe, int do_obj);

/* Defines for the implementation of throwbacks.  */
#define ERROR_THROWBACK(FILE,LINE,PREFIX,S)        \
{                                                  \
  if (TARGET_THROWBACK)                            \
    arm_error_throwback (FILE, LINE, PREFIX, S);  \
}

#define ERROR_THROWBACK_FINALISE arm_throwback_finish ()
#define GCC_DRIVER_HOST_INITIALIZATION riscos_host_initialisation ()
#define TARGET_CONVERT_FILENAME(a,b,c,d) return riscos_convert_filename (a,b,c,d)

/* Character constant used in separating components in paths.  */
#undef PATH_SEPARATOR
#define PATH_SEPARATOR ':'

/* Directory name separator.  */
#undef DIR_SEPARATOR
#define DIR_SEPARATOR '/'

/* Maths operation domain error number, EDOM.  For CLib it is 1.  */
#define TARGET_EDOM 33

#endif /* ! CROSS_COMPILE */
