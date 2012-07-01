#ifdef ASM

/* I'd like to use PIC as a pseudo instruction, but GCC defines it to
   indicate -fPIC, undefine it and use __PIC__ instead (which is
   also already defined for us).  */
#ifdef PIC
#undef PIC
#endif


	.macro	NAME	name
0:
	.asciz	"\name"
	.align
1:
	.word	0xff000000 + (1b - 0b)
	.endm

	/* Macro to define a word of data either with a GOT relocation for
	   the shared library or with no relocation for the static library.  */
	.macro	WORD name
#ifdef __PIC__
	.word	\name(GOT)
#else
	.word	\name
#endif
	.endm

	@ Macro to conditionally assemble instruction for use in the shared library.
	.macro PICEQ instr
#ifdef __PIC__
	\instr
#endif
	.endm

	@ Macro to conditionally assemble instruction for use in the static library.
	.macro PICNE instr
#ifndef __PIC__
	\instr
#endif
	.endm

	@ Macro to load the GOT pointer into the specified register
	.macro PIC reg
#ifdef __PIC__
	LDR	\reg, =__GOTT_BASE__
	LDR	\reg, [\reg, #0]
	LDR	\reg, [\reg, #__GOTT_INDEX__]
#endif
	.endm

	@ Macro for declaring the type and size of a function defined in ARM code.
	@ Place at end of function definition for size to be calculated correctly.
	.macro	DECLARE_FUNCTION name
	.type	\name, %function
	.size	\name, . - \name
	.endm

	@ Macro for declaring the type and size of an object defined in ARM code.
	@ Place at end of object definition for size to be calculated correctly.
	.macro	DECLARE_OBJECT name
	.type	\name, %object
	.size	\name, . - \name
	.endm

	.macro	REPORT_HEX int
	STMFD	sp!, {r0-r3}

	MOV	r0,\int
	B	89f
88:
	.word	0,0,0
89:
	ADR	r1,88b
	MOV	r2,#12
	SWI	0xd4
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x74c80

	LDMFD	sp!, {r0-r3}
	.endm

	.macro REPORT_TEXT string

	SWI	0x74c81
	.asciz	"\string"
	.align

	.endm

	.macro REPORT_TEXT0 reg
	STMFD	sp!, {r0-r3}

	MOV	r0, \reg
	SWI	0x74c80

	LDMFD	sp!, {r0-r3}

	.endm

	.macro	PRINT_HEX int
	STMFD	sp!, {r0-r3}
	MOV	r0,\int
	B	89f
88:
	.word	0,0,0
89:
	ADR	r1,88b
	MOV	r2,#12
	SWI	0xd4
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2

	LDMFD	sp!, {r0-r3}
	.endm

	.macro	PRINT_DEC int
	STMFD	sp!, {r0-r3}
	MOV	r0,\int
	B	89f
88:
	.word	0,0,0
89:
	ADR	r1,88b
	MOV	r2,#12
	SWI	0xdc
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2

	LDMFD	sp!, {r0-r3}
	.endm

	.macro PRINT_NL

	SWI	0x10D
	SWI	0x10A

	.endm

	.macro PRINT_TEXT string

	SWI	0x1
	.asciz	"\string"
	.align

	.endm

#else

/* 32 bit hex to string
 * return pointer to terminating null in buffer
 */
static inline char *
hex_to_string(unsigned int v, char *buffer, int l)
{
char *term;

  __asm__ volatile ("	MOV	r0, %1;\n"
		"	MOV	r1, %2;\n"
		"	MOV	r2, %3;\n"
		"	SWI	%[os_converthex8];\n"
		"	MOV	%0, r1;\n"
		: "=r" (term)
		: "r" (v), "r" (buffer), "rI" (l), [os_converthex8] "i" (0xD4)
		: "a1", "a2", "a3", "cc");
  return term;
}

/* Signed 32 bit integer to string
 * return pointer to terminating null in buffer
 */
static inline char *
sint32_to_string(int v, char *buffer, int l)
{
char *term;

  __asm__ volatile ("	MOV	r0, %1;\n"
		"	MOV	r1, %2;\n"
		"	MOV	r2, %3;\n"
		"	SWI	%[os_convertint4];\n"
		"	MOV	%0, r1;\n"
		: "=r" (term)
		: "r" (v), "r" (buffer), "rI" (l), [os_convertint4] "i" (0xDC)
		: "a1", "a2", "a3", "cc");
  return term;
}

static inline void report_text(const char *s)
{
  __asm__ volatile ("	MOV	r0, %0;\n"
		"	SWI	%[report_text0];\n"
		: /* no outputs */
		: "r" (s), [report_text0] "i" (0x54C80)
		: "a1", "cc");
}

static inline void report_hex(unsigned int v)
{
char buf[12];

  hex_to_string(v, buf, 10);
  report_text(buf);
}

static inline void report_dec(int v)
{
char buf[20];

  sint32_to_string(v, buf, 20);
  report_text(buf);
}

#endif
