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
#define TARGET_DEFAULT_FLOAT_ABI ARM_FLOAT_ABI_SOFT

#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm6

/* RISC OS FPE always understands FPA instructions.  */
#undef  FPUTYPE_DEFAULT
#define FPUTYPE_DEFAULT FPUTYPE_FPA_EMU3

/* RISC OS uses the APCS-32 ABI.  */
#undef ARM_DEFAULT_ABI
#define ARM_DEFAULT_ABI ARM_ABI_APCS32

/* For PIC code we need to explicitly specify (PLT) and (GOT) relocs. (PLT) is
   not needed for module code.  */
#define NEED_PLT_RELOC	(!TARGET_MODULE && flag_pic)
#define NEED_GOT_RELOC	flag_pic

/* We explicitly specify an -mfpu value when there isn't one specified as
   otherwise selecting an -march makes the assembler choose an 'appropriate'
   default fpu value which either clashes with -mfloat-abi option (resulting
   in e.g. "hard-float conflicts with specified fpu" assembler errors), either
   result in linker errors when VFP fpu is taken as default fpu not matching
   -mfloat-abi=soft.  */
#undef SUBTARGET_EXTRA_ASM_SPEC
#define SUBTARGET_EXTRA_ASM_SPEC "%{!mfpu:%{mhard-float|mfloat-abi=hard:-mfpu=fpa; :-mfpu=softfpa}}"

#undef SUBTARGET_EXTRA_LINK_SPEC
#ifdef CROSS_COMPILE
#  define SUBTARGET_EXTRA_LINK_SPEC \
     "-m armelf_riscos -p %{!static:%{!fpic:-fPIC}} " \
     "%{fpic:-fpic} %{mmodule:--ro-module-reloc} "
#else
/* When building the native RISC OS compiler, we add an extra library path
   DSOLib:lib  */
#  define SUBTARGET_EXTRA_LINK_SPEC \
     "-m armelf_riscos -p %{!static:%{!fpic:-fPIC}} " \
     "-L/DSOLib:lib " \
     "%{fpic:-fpic} %{mmodule:--ro-module-reloc} "
#endif

/* libscl means hard-float only.  Module support means libscl and
   hard-float.  */
#define SUBTARGET_OPTION_TRANSLATE_TABLE            \
  { "-mlibscl", "-mlibscl -mhard-float" },          \
  { "-mmodule", "-mmodule -mlibscl -mhard-float" }

/* Default multilib is UnixLib, hard-float (and no module support) */
#undef  MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "" }

#undef SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC "\
%{mmodule:-D__TARGET_SCL__ -D__TARGET_MODULE__ -icrossdirafter /libscl; \
  mlibscl:-D__TARGET_SCL__ -icrossdirafter /libscl; \
  :-D__TARGET_UNIXLIB__ -icrossdirafter /libunixlib} \
"

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

/* Now we define the strings used to build the spec file.  */
#undef  LIB_SPEC
#define LIB_SPEC \
  "%{!nostdlib:%{mlibscl:-lscl; :-lunixlib }}"

#define LIBGCC_SPEC "-lgcc"

/* Provide a STARTFILE_SPEC appropriate for GNU/Linux.  Here we add
   the GNU/Linux magical crtbegin.o file (see crtstuff.c) which
   provides part of the support for getting C++ file-scope static
   object constructed before entering `main'.  */
#undef STARTFILE_SPEC
#define STARTFILE_SPEC  "%{!mlibscl:crti.o%s} \
			 %{!shared:crt1-riscos.o%s %{!mlibscl:crtbegin.o%s}} \
			 %{!mlibscl:%{shared:crtbeginS.o%s}}"

/* Provide a ENDFILE_SPEC appropriate for GNU/Linux.  Here we tack on
   the GNU/Linux magical crtend.o file (see crtstuff.c) which
   provides part of the support for getting C++ file-scope static
   object constructed before entering `main', followed by a normal
   GNU/Linux "finalizer" file, `crtn.o'.  */
#undef ENDFILE_SPEC
#define ENDFILE_SPEC	"%{!mlibscl:%{!shared:crtend.o%s}} \
			 %{!mlibscl:%{shared:crtendS.o%s} crtn.o%s}"

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
#ifdef __TARGET_SCL__
#define INVOKE__main
#else
#undef INVOKE__main
#endif

/* On svr4, we *do* have support for the .init and .fini sections, and we
   can put stuff in there to be executed before and after `main'.  We let
   crtstuff.c and other files know this by defining the following symbols.
   The definitions say how to change sections to the .init and .fini
   sections.  This is the same for all known svr4 assemblers.  */
#undef INIT_SECTION_ASM_OP
#define INIT_SECTION_ASM_OP	".section\t.init"
#undef FINI_SECTION_ASM_OP
#define FINI_SECTION_ASM_OP	".section\t.fini"
#define HAVE_INIT_SECTION

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

#define LINK_COMMAND_SPEC "\
%{!fsyntax-only:%{!c:%{!M:%{!MM:%{!E:%{!S:\
    %(linker) %l " LINK_PIE_SPEC "%X %{o*} %{A} %{d} %{e*} %{m} %{N} %{n} %{r}\
    %{s} %{t} %{u*} %{x} %{z} %{Z} %{!A:%{!nostdlib:%{!nostartfiles:%S}}}\
    %{static:} %{L*} %(mfwrap) %(link_libgcc) --start-group %o --end-group %(mflib)\
    %{fprofile-arcs|fprofile-generate|coverage:-lgcov}\
    %{!nostdlib:%{!nodefaultlibs:%(link_ssp) %(link_gcc_c_sequence)}}\
    %{!A:%{!nostdlib:%{!nostartfiles:%E}}} %{T*} }}}}}}"

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

/* Default RISC OS options
   - APCS
   - function name poking
   - explicit stack checks
   - UnixLib as runtime library */
#undef  TARGET_DEFAULT
#define TARGET_DEFAULT (MASK_APCS_FRAME | MASK_POKE_FUNCTION_NAME | MASK_APCS_STACK | MASK_UNIXLIB)

/* Prevent FRAME_POINTER_REGNUM being clobbered in functions that call
   alloca.  */
#define PREVENT_FP_CLOBBER (TARGET_APCS_STACK)

/* If we're targeting explicit APCS stack checks, then force calls to
   __builtin_return_address and __builtin_frame_address as library
   function calls.  */
#define HAVE_BUILTIN_RETURN_ADDR_FUNC (TARGET_APCS_STACK)
#define HAVE_BUILTIN_FRAME_ADDR_FUNC (TARGET_APCS_STACK)

#define TARGET_RISCOSELF

/* Override configure checks for mmap() compatibility.  Our C library
   partly provides these features, but they do not work in a way
   that the garbage collector expects.  */
#undef HAVE_MMAP_ANON
#undef HAVE_MMAP_DEV_ZERO

/* Clear the instruction cache from `beg' to `end'.  This makes an
   inline system call to XOS_SynchroniseCodeAreas.  */
#undef CLEAR_INSN_CACHE
#define CLEAR_INSN_CACHE(BEG, END)                                             \
{                                                                              \
  register unsigned long _beg __asm ("r1") = ((unsigned long) (BEG) + 0) & -4; \
  register unsigned long _end __asm ("r2") = ((unsigned long) (END) - 1) & -4; \
  register unsigned long _flg __asm ("r0") = 1;                                \
  __asm __volatile ("swi 0x2006e  @ XOS_SynchroniseCodeAreas"                  \
                    : "=r" (_beg)                                              \
                    : "0" (_beg), "r" (_end), "r" (_flg));                     \
}

/* When UnixLib is used, we claim to support all C99 functions.  */
#define TARGET_C99_FUNCTIONS TARGET_UNIXLIB

#define PIC_PLT_SCRATCH_REGNUM	8

#undef SUBTARGET_CONDITIONAL_REGISTER_USAGE
#define SUBTARGET_CONDITIONAL_REGISTER_USAGE	\
 if (flag_pic == 2)				\
   {						\
     call_used_regs[PIC_PLT_SCRATCH_REGNUM] = 1;\
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
#define PATH_SEPARATOR ','

/* Directory name separator.  */
#undef DIR_SEPARATOR
#define DIR_SEPARATOR '/'

#endif /* ! CROSS_COMPILE */

/* Maths operation domain error number, EDOM.  For CLib it is 1, for UnixLib
   is is 33.  */
#define TARGET_EDOM ((TARGET_UNIXLIB) ? 33 : 1)
