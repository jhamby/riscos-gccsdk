#ifndef src_regs_h_
#define src_regs_h_

#include <stdint.h>

typedef struct cpu_registers
{
#define N_REGS 16
  uint32_t r[N_REGS];
  uint32_t cpsr;
} cpu_registers;

#endif
