#ifndef src_regs_h_
#define src_regs_h_

#include <stdint.h>

/* The order of these registers is determined by gdb.  */
/* FIXME: vfpv2, vfpv3, neon support */
typedef union
{
  struct
    {
#define N_CORE_CPU_REGS 16
      uint32_t r[N_CORE_CPU_REGS]; /**< Registers r0-r15 */
#define N_FPA_REGS 8
      uint32_t fpa_r[3*N_FPA_REGS]; /**< FPA registers f0-f7 */
      uint32_t fpa_status; /**< FPA status register */
      uint32_t cpsr; /**< CPSR register */
    } detail;
  uint32_t raw[N_CORE_CPU_REGS + 3*N_FPA_REGS + 1 + 1];
} cpu_registers;

#endif
