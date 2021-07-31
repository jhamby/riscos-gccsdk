#ifndef __REPORTER_H__
#define __REPORTER_H__

/* 32 bit hex to string
 * return pointer to terminating null in buffer
 */
static inline char *
hex_to_string(unsigned int v, char *buffer, int l)
{
char *term;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	MOV	r0, %1;\n"
		  "	MOV	r1, %2;\n"
		  "	MOV	r2, %3;\n"
		  "	SWI	%[os_converthex8];\n"
		  "	MOV	%0, r1;\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : "=r" (term)
		  : "r" (v), "r" (buffer), "rI" (l), [os_converthex8] "i" (0xD4)
		  : "r0", "r1", "r2", "r14", "cc");
  return term;
}

/* Signed 32 bit integer to string
 * return pointer to terminating null in buffer
 */
static inline char *
sint32_to_string(int v, char *buffer, int l)
{
char *term;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	MOV	r0, %1;\n"
		  "	MOV	r1, %2;\n"
		  "	MOV	r2, %3;\n"
		  "	SWI	%[os_convertint4];\n"
		  "	MOV	%0, r1;\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : "=r" (term)
		  : "r" (v), "r" (buffer), "rI" (l), [os_convertint4] "i" (0xDC)
		  : "r0", "r1", "r2", "r14", "cc");
  return term;
}

static inline void report_text(const char *s)
{
  register const char *__s __asm__("r0") = s;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	SWI	%[report_text0];\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : /* no outputs */
		  : "r" (__s), [report_text0] "i" (0x74C80)
		  : "r14", "cc");
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
