/* Simple example of how to call inline assembler and link with object
   files from an assembler.  */

#include <stdlib.h>

extern void my_puts_gas (const char *str);
extern void my_puts_asasm (const char *str);

static __inline__ void
my_puts_inline (const char *str)
{
  register int _str __asm ("r0") = (int) str;
  __asm__ __volatile__ ("SWI	2	@ SWI OS_Write0\n\t"
  			:
  			: "r" (_str)
  			: "cc");
}

int
main (int argc, char *argv[])
{
  my_puts_inline ("This is printed using inline assembler\r\n");
  my_puts_gas ("This is printed using GAS assembler\r\n");
  my_puts_gas ("This is printed using ASASM assembler\r\n");

  return EXIT_SUCCESS;
}
