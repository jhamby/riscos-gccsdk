/* Generated automatically by the program `genattr'
   from the machine description file `md'.  */

#ifndef GCC_INSN_ATTR_H
#define GCC_INSN_ATTR_H

#define HAVE_ATTR_alternative
#define get_attr_alternative(insn) which_alternative
#define HAVE_ATTR_is_thumb
enum attr_is_thumb {IS_THUMB_NO, IS_THUMB_YES};
extern enum attr_is_thumb get_attr_is_thumb PARAMS ((void));

#define HAVE_ATTR_prog_mode
enum attr_prog_mode {PROG_MODE_PROG26, PROG_MODE_PROG32};
extern enum attr_prog_mode get_attr_prog_mode PARAMS ((void));

#define HAVE_ATTR_is_strongarm
enum attr_is_strongarm {IS_STRONGARM_NO, IS_STRONGARM_YES};
extern enum attr_is_strongarm get_attr_is_strongarm PARAMS ((void));

#define HAVE_ATTR_is_xscale
enum attr_is_xscale {IS_XSCALE_NO, IS_XSCALE_YES};
extern enum attr_is_xscale get_attr_is_xscale PARAMS ((void));

#define HAVE_ATTR_shift
extern int get_attr_shift PARAMS ((rtx));
#define HAVE_ATTR_fpu
enum attr_fpu {FPU_FPA, FPU_FPE2, FPU_FPE3};
extern enum attr_fpu get_attr_fpu PARAMS ((void));

#define HAVE_ATTR_length
extern int get_attr_length PARAMS ((rtx));
extern void shorten_branches PARAMS ((rtx));
extern int insn_default_length PARAMS ((rtx));
extern int insn_variable_length_p PARAMS ((rtx));
extern int insn_current_length PARAMS ((rtx));

#include "insn-addr.h"

#define HAVE_ATTR_pool_range
extern int get_attr_pool_range PARAMS ((rtx));
#define HAVE_ATTR_neg_pool_range
extern int get_attr_neg_pool_range PARAMS ((rtx));
#define HAVE_ATTR_type
enum attr_type {TYPE_NORMAL, TYPE_MULT, TYPE_BLOCK, TYPE_FLOAT, TYPE_FDIVX, TYPE_FDIVD, TYPE_FDIVS, TYPE_FMUL, TYPE_FFMUL, TYPE_FARITH, TYPE_FFARITH, TYPE_FLOAT_EM, TYPE_F_LOAD, TYPE_F_STORE, TYPE_F_MEM_R, TYPE_R_MEM_F, TYPE_F_2_R, TYPE_R_2_F, TYPE_CALL, TYPE_LOAD, TYPE_STORE1, TYPE_STORE2, TYPE_STORE3, TYPE_STORE4};
extern enum attr_type get_attr_type PARAMS ((rtx));

#define HAVE_ATTR_ldsched
enum attr_ldsched {LDSCHED_NO, LDSCHED_YES};
extern enum attr_ldsched get_attr_ldsched PARAMS ((void));

#define HAVE_ATTR_conds
enum attr_conds {CONDS_USE, CONDS_SET, CONDS_CLOB, CONDS_JUMP_CLOB, CONDS_NOCOND};
extern enum attr_conds get_attr_conds PARAMS ((rtx));

#define HAVE_ATTR_predicable
enum attr_predicable {PREDICABLE_NO, PREDICABLE_YES};
extern enum attr_predicable get_attr_predicable PARAMS ((rtx));

#define HAVE_ATTR_model_wbuf
enum attr_model_wbuf {MODEL_WBUF_NO, MODEL_WBUF_YES};
extern enum attr_model_wbuf get_attr_model_wbuf PARAMS ((void));

#define HAVE_ATTR_write_conflict
enum attr_write_conflict {WRITE_CONFLICT_NO, WRITE_CONFLICT_YES};
extern enum attr_write_conflict get_attr_write_conflict PARAMS ((rtx));

#define HAVE_ATTR_core_cycles
enum attr_core_cycles {CORE_CYCLES_SINGLE, CORE_CYCLES_MULTI};
extern enum attr_core_cycles get_attr_core_cycles PARAMS ((rtx));

#define HAVE_ATTR_far_jump
enum attr_far_jump {FAR_JUMP_YES, FAR_JUMP_NO};
extern enum attr_far_jump get_attr_far_jump PARAMS ((rtx));

#define INSN_SCHEDULING

extern int result_ready_cost PARAMS ((rtx));
extern int function_units_used PARAMS ((rtx));

extern const struct function_unit_desc
{
  const char *const name;
  const int bitmask;
  const int multiplicity;
  const int simultaneity;
  const int default_cost;
  const int max_issue_delay;
  int (*const ready_cost_function) PARAMS ((rtx));
  int (*const conflict_cost_function) PARAMS ((rtx, rtx));
  const int max_blockage;
  unsigned int (*const blockage_range_function) PARAMS ((rtx));
  int (*const blockage_function) PARAMS ((rtx, rtx));
} function_units[];

#define FUNCTION_UNITS_SIZE 5
#define MIN_MULTIPLICITY 1
#define MAX_MULTIPLICITY 1
#define MIN_SIMULTANEITY 0
#define MAX_SIMULTANEITY 2
#define MIN_READY_COST 1
#define MAX_READY_COST 71
#define MIN_ISSUE_DELAY 1
#define MAX_ISSUE_DELAY 69
#define MIN_BLOCKAGE 1
#define MAX_BLOCKAGE 71
#define BLOCKAGE_BITS 8
#define INSN_QUEUE_SIZE 128

#define ATTR_FLAG_forward	0x1
#define ATTR_FLAG_backward	0x2
#define ATTR_FLAG_likely	0x4
#define ATTR_FLAG_very_likely	0x8
#define ATTR_FLAG_unlikely	0x10
#define ATTR_FLAG_very_unlikely	0x20

#endif /* GCC_INSN_ATTR_H */
