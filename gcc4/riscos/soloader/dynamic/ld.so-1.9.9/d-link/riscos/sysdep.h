
/*
 * Various assmbly language/system dependent  hacks that are required
 * so that we can minimize the amount of platform specific code.
 */

/*
 * Define this if the system uses RELOCA.
 */
#undef ELF_USES_RELOCA

/*
 * Get a pointer to the argv array.  On many platforms this can be just
 * the address of the first argument, on other platforms we need to
 * do something a little more subtle here.
 */

/* RISC OS: We get passed a pointer to the stack itself from _dl_start.  */
#define GET_ARGV(ARGVP, ARGS) ARGVP = ((unsigned int*)ARGS)

/*
 * Get the address of the Global offset table.  This must be absolute, not
 * relative.
 * The compiler is free to use any register for the PIC register, so we
 * can't simply use a MOV instruction. Instead use the standard sequence of
 * instructions to retrieve the GOT pointer.
 */
#ifdef __ARM_EABI__
#define GET_GOT(X) \
	__asm__("	mov	%0, #0x8000;\n"	\
		"	ldr %0, [%0, #0x38];\n"	\
		"	ldr %0, [%0, #__GOTT_INDEX__];\n"	\
		: "=r" (X))
#else
#define GET_GOT(X) \
	__asm__("\tldr %0, 0f;\n"	\
		"\tldr %0, [%0, #0];\n"	\
		"\tldr %0, [%0, #__GOTT_INDEX__];\n"	\
		"\tb 1f;\n"	\
		"0:\n"	\
		"\t.word __GOTT_BASE__;\n"	\
		"1:\n"	\
		: "=r" (X))
#endif

/*
 * Initialization sequence for a GOT.
 */
#define INIT_GOT(GOT_BASE,MODULE) \
{				\
  GOT_BASE[2] = (int) _dl_riscos_resolve; \
  GOT_BASE[1] = (int) MODULE; \
}

/* Specific to GCC 4.1  */
#define INIT_41_GOT(GOT_BASE,MODULE) \
{				\
  GOT_BASE[4] = (int) _dl_riscos_resolve; \
  GOT_BASE[3] = (int) MODULE; \
}

/*
 * Here is a macro to perform a relocation.  This is only used when
 * bootstrapping the dynamic loader.  RELP is the relocation that we
 * are performing, REL is the pointer to the address we are relocating.
 * SYMBOL is the symbol involved in the relocation, and LOAD is the
 * load address.
 */
#define PERFORM_BOOTSTRAP_RELOC(RELP,REL,SYMBOL,LOAD) \
	switch(ELF32_R_TYPE((RELP)->r_info)){		\
	case R_ARM_GLOB_DAT:	\
	case R_ARM_JMP_SLOT:	\
	  *REL = SYMBOL;	\
	  break;	\
	case R_ARM_RELATIVE:		\
	  *REL += (unsigned int) LOAD;		\
	  break;		\
	default:		\
  	  print_text("unknown relocation in PERFORM_BOOTSTRAP_RELOC()\r\n"); \
	  print_hex((int)ELF32_R_TYPE((RELP)->r_info)); \
	  print_text(" : "); \
	  print_hex((int)REL); \
	  print_text(" : "); \
	  print_hex((int)SYMBOL); \
	  print_text(" : "); \
	  print_hex((int)LOAD); \
	  print_text("\r\n"); \
	  _dl_exit(1);		\
	}

/*
 * Transfer control to the user's application, once the dynamic loader
 * is done. We pass _dl_malloc_addr to the runtime library so that it knows
 * where free memory starts and it doesn't overwrite any data belonging to the
 * dynamic loader. We also pass the address of a function that can be used
 * to call all of the static object constructors in the shared libraries.
 */
#define START() \
      __asm__ volatile ("	mov a1, %[free_mem];\n" \
			"	mov a2, %[ctors];\n" \
			"	mov a3, #0;\n" \
			"	mov pc, %[main];\n" \
			: /* no outputs */ \
			: [main] "r" (_dl_elf_main), \
			  [free_mem] "r" (_dl_malloc_addr), \
			  [ctors] "r" (_dl_call_ctors) \
			: "a1", "a2", "a3")



/* Here we define the magic numbers that this dynamic loader should accept */

#define MAGIC1 EM_ARM
#undef MAGIC2
/* Used for error messages */
#define ELF_TARGET "ARM"

extern unsigned int _dl_riscos_resolver(int dummy, int i);

static inline
unsigned do_rem(unsigned top, int bottom)
{
  unsigned result, scratch;
  unsigned temp;

  __asm__ volatile ("	mov	%[temp], %[top];\n"
		    "	movs	%[scratch], %[bottom];\n"
		    "	beq	2f;\n"
		    "	cmp	%[scratch], %[temp], lsr#1;\n"
		    "0:\n"
		    "	movls	%[scratch], %[scratch], lsl#1;\n"
		    "	cmp	%[scratch], %[temp], lsr#1;\n"
		    "	bls	0b;\n"
		    "1:\n"
		    "	cmp	%[temp], %[scratch];\n"
		    "	subcs	%[temp], %[temp], %[scratch];\n"
		    "	mov	%[scratch], %[scratch], lsr#1;\n"
		    "	cmp	%[scratch], %[bottom];\n"
		    "	bhs	1b;\n"
		    "	mov	%[result], %[temp];\n"
		    "	b	3f;\n"
		    "2:\n"
		    "	swi	0x74C81;\n" /* SWI "Report_TextS" */
		    "	.asciz	\"Divide by zero\"\n"
		    "	.align\n\t"
		    "	swi	0x11;\n" /* SWI "OS_Exit" */
		    "3:\n"
		    : [result] "=r" (result), [scratch] "=&r" (scratch), [temp] "=&r" (temp)
		    : [bottom] "r" (bottom),
		      [top] "r" (top)
		    : "cc");
  return result;
}

#define ATTRIBUTE_HIDDEN __attribute__ ((visibility ("hidden")))
