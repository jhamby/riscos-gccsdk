/* Generated automatically by the program `genopinit'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "flags.h"
#include "insn-flags.h"
#include "insn-codes.h"
#include "insn-config.h"
#include "recog.h"
#include "expr.h"
#include "reload.h"

void
init_all_optabs ()
{
  add_optab->handlers[(int) DImode].insn_code = CODE_FOR_adddi3;
  add_optab->handlers[(int) SImode].insn_code = CODE_FOR_addsi3;
  if (HAVE_addsf3)
    add_optab->handlers[(int) SFmode].insn_code = CODE_FOR_addsf3;
  if (HAVE_adddf3)
    add_optab->handlers[(int) DFmode].insn_code = CODE_FOR_adddf3;
  if (HAVE_addxf3)
    add_optab->handlers[(int) XFmode].insn_code = CODE_FOR_addxf3;
  sub_optab->handlers[(int) DImode].insn_code = CODE_FOR_subdi3;
  sub_optab->handlers[(int) SImode].insn_code = CODE_FOR_subsi3;
  if (HAVE_subsf3)
    sub_optab->handlers[(int) SFmode].insn_code = CODE_FOR_subsf3;
  if (HAVE_subdf3)
    sub_optab->handlers[(int) DFmode].insn_code = CODE_FOR_subdf3;
  if (HAVE_subxf3)
    sub_optab->handlers[(int) XFmode].insn_code = CODE_FOR_subxf3;
  smul_optab->handlers[(int) SImode].insn_code = CODE_FOR_mulsi3;
  if (HAVE_mulsidi3)
    smul_widen_optab->handlers[(int) DImode].insn_code = CODE_FOR_mulsidi3;
  if (HAVE_umulsidi3)
    umul_widen_optab->handlers[(int) DImode].insn_code = CODE_FOR_umulsidi3;
  if (HAVE_smulsi3_highpart)
    smul_highpart_optab->handlers[(int) SImode].insn_code = CODE_FOR_smulsi3_highpart;
  if (HAVE_umulsi3_highpart)
    umul_highpart_optab->handlers[(int) SImode].insn_code = CODE_FOR_umulsi3_highpart;
  if (HAVE_mulsf3)
    smul_optab->handlers[(int) SFmode].insn_code = CODE_FOR_mulsf3;
  if (HAVE_muldf3)
    smul_optab->handlers[(int) DFmode].insn_code = CODE_FOR_muldf3;
  if (HAVE_mulxf3)
    smul_optab->handlers[(int) XFmode].insn_code = CODE_FOR_mulxf3;
  if (HAVE_divsf3)
    flodiv_optab->handlers[(int) SFmode].insn_code = CODE_FOR_divsf3;
  if (HAVE_divdf3)
    flodiv_optab->handlers[(int) DFmode].insn_code = CODE_FOR_divdf3;
  if (HAVE_divxf3)
    flodiv_optab->handlers[(int) XFmode].insn_code = CODE_FOR_divxf3;
  if (HAVE_modsf3)
    smod_optab->handlers[(int) SFmode].insn_code = CODE_FOR_modsf3;
  if (HAVE_moddf3)
    smod_optab->handlers[(int) DFmode].insn_code = CODE_FOR_moddf3;
  if (HAVE_modxf3)
    smod_optab->handlers[(int) XFmode].insn_code = CODE_FOR_modxf3;
  and_optab->handlers[(int) DImode].insn_code = CODE_FOR_anddi3;
  and_optab->handlers[(int) SImode].insn_code = CODE_FOR_andsi3;
  ior_optab->handlers[(int) DImode].insn_code = CODE_FOR_iordi3;
  ior_optab->handlers[(int) SImode].insn_code = CODE_FOR_iorsi3;
  xor_optab->handlers[(int) DImode].insn_code = CODE_FOR_xordi3;
  xor_optab->handlers[(int) SImode].insn_code = CODE_FOR_xorsi3;
  smax_optab->handlers[(int) SImode].insn_code = CODE_FOR_smaxsi3;
  smin_optab->handlers[(int) SImode].insn_code = CODE_FOR_sminsi3;
  umax_optab->handlers[(int) SImode].insn_code = CODE_FOR_umaxsi3;
  umin_optab->handlers[(int) SImode].insn_code = CODE_FOR_uminsi3;
  ashl_optab->handlers[(int) SImode].insn_code = CODE_FOR_ashlsi3;
  ashr_optab->handlers[(int) SImode].insn_code = CODE_FOR_ashrsi3;
  lshr_optab->handlers[(int) SImode].insn_code = CODE_FOR_lshrsi3;
  rotl_optab->handlers[(int) SImode].insn_code = CODE_FOR_rotlsi3;
  rotr_optab->handlers[(int) SImode].insn_code = CODE_FOR_rotrsi3;
  neg_optab->handlers[(int) DImode].insn_code = CODE_FOR_negdi2;
  neg_optab->handlers[(int) SImode].insn_code = CODE_FOR_negsi2;
  if (HAVE_negsf2)
    neg_optab->handlers[(int) SFmode].insn_code = CODE_FOR_negsf2;
  if (HAVE_negdf2)
    neg_optab->handlers[(int) DFmode].insn_code = CODE_FOR_negdf2;
  if (HAVE_negxf2)
    neg_optab->handlers[(int) XFmode].insn_code = CODE_FOR_negxf2;
  abs_optab->handlers[(int) SImode].insn_code = CODE_FOR_abssi2;
  if (HAVE_abssf2)
    abs_optab->handlers[(int) SFmode].insn_code = CODE_FOR_abssf2;
  if (HAVE_absdf2)
    abs_optab->handlers[(int) DFmode].insn_code = CODE_FOR_absdf2;
  if (HAVE_absxf2)
    abs_optab->handlers[(int) XFmode].insn_code = CODE_FOR_absxf2;
  if (HAVE_sqrtsf2)
    sqrt_optab->handlers[(int) SFmode].insn_code = CODE_FOR_sqrtsf2;
  if (HAVE_sqrtdf2)
    sqrt_optab->handlers[(int) DFmode].insn_code = CODE_FOR_sqrtdf2;
  if (HAVE_sqrtxf2)
    sqrt_optab->handlers[(int) XFmode].insn_code = CODE_FOR_sqrtxf2;
  one_cmpl_optab->handlers[(int) DImode].insn_code = CODE_FOR_one_cmpldi2;
  one_cmpl_optab->handlers[(int) SImode].insn_code = CODE_FOR_one_cmplsi2;
  if (HAVE_floatsisf2)
    floattab[(int) SFmode][(int) SImode][0] = CODE_FOR_floatsisf2;
  if (HAVE_floatsidf2)
    floattab[(int) DFmode][(int) SImode][0] = CODE_FOR_floatsidf2;
  if (HAVE_floatsixf2)
    floattab[(int) XFmode][(int) SImode][0] = CODE_FOR_floatsixf2;
  if (HAVE_fix_truncsfsi2)
    fixtrunctab[(int) SFmode][(int) SImode][0] = CODE_FOR_fix_truncsfsi2;
  if (HAVE_fix_truncdfsi2)
    fixtrunctab[(int) DFmode][(int) SImode][0] = CODE_FOR_fix_truncdfsi2;
  if (HAVE_fix_truncxfsi2)
    fixtrunctab[(int) XFmode][(int) SImode][0] = CODE_FOR_fix_truncxfsi2;
  extendtab[(int) DImode][(int) SImode][1] = CODE_FOR_zero_extendsidi2;
  extendtab[(int) DImode][(int) QImode][1] = CODE_FOR_zero_extendqidi2;
  extendtab[(int) DImode][(int) SImode][0] = CODE_FOR_extendsidi2;
  extendtab[(int) SImode][(int) HImode][1] = CODE_FOR_zero_extendhisi2;
  extendtab[(int) SImode][(int) QImode][1] = CODE_FOR_zero_extendqisi2;
  extendtab[(int) SImode][(int) HImode][0] = CODE_FOR_extendhisi2;
  extendtab[(int) HImode][(int) QImode][0] = CODE_FOR_extendqihi2;
  extendtab[(int) SImode][(int) QImode][0] = CODE_FOR_extendqisi2;
  if (HAVE_extendsfdf2)
    extendtab[(int) DFmode][(int) SFmode][0] = CODE_FOR_extendsfdf2;
  if (HAVE_extendsfxf2)
    extendtab[(int) XFmode][(int) SFmode][0] = CODE_FOR_extendsfxf2;
  if (HAVE_extenddfxf2)
    extendtab[(int) XFmode][(int) DFmode][0] = CODE_FOR_extenddfxf2;
  mov_optab->handlers[(int) DImode].insn_code = CODE_FOR_movdi;
  mov_optab->handlers[(int) SImode].insn_code = CODE_FOR_movsi;
  mov_optab->handlers[(int) HImode].insn_code = CODE_FOR_movhi;
  reload_out_optab[(int) HImode] = CODE_FOR_reload_outhi;
  if (HAVE_reload_inhi)
    reload_in_optab[(int) HImode] = CODE_FOR_reload_inhi;
  mov_optab->handlers[(int) QImode].insn_code = CODE_FOR_movqi;
  mov_optab->handlers[(int) SFmode].insn_code = CODE_FOR_movsf;
  mov_optab->handlers[(int) DFmode].insn_code = CODE_FOR_movdf;
  reload_out_optab[(int) DFmode] = CODE_FOR_reload_outdf;
  if (HAVE_movxf)
    mov_optab->handlers[(int) XFmode].insn_code = CODE_FOR_movxf;
  movstr_optab[(int) QImode] = CODE_FOR_movstrqi;
  cmp_optab->handlers[(int) SImode].insn_code = CODE_FOR_cmpsi;
  if (HAVE_cmpsf)
    cmp_optab->handlers[(int) SFmode].insn_code = CODE_FOR_cmpsf;
  if (HAVE_cmpdf)
    cmp_optab->handlers[(int) DFmode].insn_code = CODE_FOR_cmpdf;
  if (HAVE_cmpxf)
    cmp_optab->handlers[(int) XFmode].insn_code = CODE_FOR_cmpxf;
  bcc_gen_fctn[(int) EQ] = gen_beq;
  bcc_gen_fctn[(int) NE] = gen_bne;
  bcc_gen_fctn[(int) GT] = gen_bgt;
  bcc_gen_fctn[(int) LE] = gen_ble;
  bcc_gen_fctn[(int) GE] = gen_bge;
  bcc_gen_fctn[(int) LT] = gen_blt;
  bcc_gen_fctn[(int) GTU] = gen_bgtu;
  bcc_gen_fctn[(int) LEU] = gen_bleu;
  bcc_gen_fctn[(int) GEU] = gen_bgeu;
  bcc_gen_fctn[(int) LTU] = gen_bltu;
  setcc_gen_code[(int) EQ] = CODE_FOR_seq;
  setcc_gen_code[(int) NE] = CODE_FOR_sne;
  setcc_gen_code[(int) GT] = CODE_FOR_sgt;
  setcc_gen_code[(int) LE] = CODE_FOR_sle;
  setcc_gen_code[(int) GE] = CODE_FOR_sge;
  setcc_gen_code[(int) LT] = CODE_FOR_slt;
  setcc_gen_code[(int) GTU] = CODE_FOR_sgtu;
  setcc_gen_code[(int) LEU] = CODE_FOR_sleu;
  setcc_gen_code[(int) GEU] = CODE_FOR_sgeu;
  setcc_gen_code[(int) LTU] = CODE_FOR_sltu;
  movcc_gen_code[(int) SImode] = CODE_FOR_movsicc;
  movcc_gen_code[(int) SFmode] = CODE_FOR_movsfcc;
  if (HAVE_movdfcc)
    movcc_gen_code[(int) DFmode] = CODE_FOR_movdfcc;
}
