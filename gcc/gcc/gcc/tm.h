/* tm.h */

/* The target machine.  */

#define TARGET_CPU_DEFAULT (TARGET_CPU_generic)
#ifdef IN_GCC
#include "gansidecl.h"
#ifdef TARGET_RISCOSELF
#include "arm/riscos-elf.h"
#else
#include "arm/riscos-aof.h"
#endif
#endif
