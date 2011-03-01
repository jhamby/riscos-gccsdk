#ifndef DEF_HEADER_INCLUDED
#define DEF_HEADER_INCLUDED

/* These are deliberately *not* defined.  */
extern void func_not_defined (void) __attribute__ ((weak));
extern int var_not_defined __attribute__ ((weak));
extern int const_var_not_defined __attribute__ ((weak));

/* These are defined in def.c.  */
extern void func_defined (void) __attribute__ ((weak));
extern int var_defined __attribute__ ((weak));
extern int const_var_defined __attribute__ ((weak));

#define VAR_DEFINED_VALUE (0x89ABCDEF)
#define CONST_VAR_DEFINED_VALUE (0x01234567)

#endif

