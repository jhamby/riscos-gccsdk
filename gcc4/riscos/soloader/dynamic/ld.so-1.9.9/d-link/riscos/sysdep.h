
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
#define GET_ARGV(ARGVP, ARGS)	\
	__asm__("\t" \
		"add	%0, r12, #4\n\t" \
		: "=r" (ARGVP) \
		: \
		: "ip")
/*
 * Get the address of the Global offset table.  This must be absolute, not
 * relative.
 */
/*
 * Ensure the following inlines use the same PIC register as the
 * C code.
 */
//#define GET_GOT(X)     __asm__("\tmovl %%ebx,%0\n\t" : "=a" (X))
#define GET_GOT(X)     __asm__("\tmov %0,r7\n\t" : "=r" (X))

#define	SET_GOT(GOT)	\
	asm volatile("\t" \
		"mov	r7,%0\n\t" \
		: \
		: "r" (GOT))

/*
 * Initialization sequence for a GOT.
 */
#define INIT_GOT(GOT_BASE,MODULE) \
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

/*	case R_386_32:		\
	  *REL += SYMBOL;		\
	  break;		\
	case R_386_PC32:		\
	  *REL += SYMBOL - (unsigned int) REL;		\
	  break;		\
	case R_386_GLOB_DAT:		\
	case R_386_JMP_SLOT:		\
	  *REL = SYMBOL;		\
	  break;		\*/

/*
 * Transfer control to the user's application, once the dynamic loader
 * is done. We pass _dl_malloc_addr to Unixlib so that it knows where
 * free memory starts and it doesn't overwrite any data belonging to the
 * dynamic loader.
 */

/*#define START()		\
	__asm__ volatile ("leave\n\t" \
		    "jmp *%%eax\n\t"	\
		    : "=a" (status) :	\
		    "d" (_dl_interpreter_exit), "a" (_dl_elf_main))*/
#define START() \
	__asm__ volatile ("\t" \
			"mov a1,%1\n" \
			"mov pc,%0\n" \
			: /* no outputs */ \
			: "r" (_dl_elf_main), "r" (_dl_malloc_addr) \
			: "a1")



/* Here we define the magic numbers that this dynamic loader should accept */

#define MAGIC1 EM_ARM
#undef MAGIC2
/* Used for error messages */
#define ELF_TARGET "ARM"

extern unsigned int _dl_riscos_resolver(int dummy, int i);

#define do_rem(res,top,bottom)	\
       __asm__ volatile ("\t"	\
		"movs	r12,%2\n\t"	\
		"beq	2f\n\t"	\
		"cmp	r12,%1,lsr#1\n"	\
		"0:\n\t"	\
		"movls	r12,r12,lsl#1\n\t"	\
		"cmp	r12,%1,lsr#1\n\t"	\
		"bls	0b\n"	\
		"1:\n\t"	\
		"cmp	%1,r12\n\t"	\
		"subcs	%1,%1,r12\n\t"	\
		"mov	r12,r12,lsr#1\n\t"	\
		"cmp	r12,%2\n\t"	\
		"bhs	1b\n\t"	\
		"mov	%0,%1\n"	\
		"b	3f\n"	\
		"2:\n\t"	\
		"swi	0x1\n\t" /* SWI "OS_Write0" */	\
		".asciz	\"Divide by zero\"\n\t"	\
		".align\n\t"	\
		"swi	0x10a\n\t"	\
		"swi	0x10d\n\t"	\
		"swi	0x11\n\t" /* SWI "OS_Exit" */	\
		"3:\n\t"	\
		: "=r" (res)	\
		: "r" (top), "r" (bottom)	\
		: "ip", "cc"); /* uses r12 as temporary register and alters flags */

