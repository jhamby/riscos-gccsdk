/* Generated automatically by the program `genopinit'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"
#include "reload.h"

void
init_all_optabs ()
{
  if (HAVE_addsf3)
    addv_optab->handlers[(int) (int) SFmode].insn_code =
    add_optab->handlers[(int) (int) SFmode].insn_code = CODE_FOR_addsf3;
  if (HAVE_adddf3)
    addv_optab->handlers[(int) (int) DFmode].insn_code =
    add_optab->handlers[(int) (int) DFmode].insn_code = CODE_FOR_adddf3;
  if (HAVE_addxf3)
    addv_optab->handlers[(int) (int) XFmode].insn_code =
    add_optab->handlers[(int) (int) XFmode].insn_code = CODE_FOR_addxf3;
  if (HAVE_subsf3)
    subv_optab->handlers[(int) (int) SFmode].insn_code =
    sub_optab->handlers[(int) (int) SFmode].insn_code = CODE_FOR_subsf3;
  if (HAVE_subdf3)
    subv_optab->handlers[(int) (int) DFmode].insn_code =
    sub_optab->handlers[(int) (int) DFmode].insn_code = CODE_FOR_subdf3;
  if (HAVE_subxf3)
    subv_optab->handlers[(int) (int) XFmode].insn_code =
    sub_optab->handlers[(int) (int) XFmode].insn_code = CODE_FOR_subxf3;
  if (HAVE_mulsidi3)
    smul_widen_optab->handlers[(int) DImode].insn_code = CODE_FOR_mulsidi3;
  if (HAVE_umulsidi3)
    umul_widen_optab->handlers[(int) DImode].insn_code = CODE_FOR_umulsidi3;
  if (HAVE_smulsi3_highpart)
    smul_highpart_optab->handlers[(int) SImode].insn_code = CODE_FOR_smulsi3_highpart;
  if (HAVE_umulsi3_highpart)
    umul_highpart_optab->handlers[(int) SImode].insn_code = CODE_FOR_umulsi3_highpart;
  if (HAVE_mulhisi3)
    smul_widen_optab->handlers[(int) SImode].insn_code = CODE_FOR_mulhisi3;
  if (HAVE_mulsf3)
    smulv_optab->handlers[(int) (int) SFmode].insn_code =
    smul_optab->handlers[(int) (int) SFmode].insn_code = CODE_FOR_mulsf3;
  if (HAVE_muldf3)
    smulv_optab->handlers[(int) (int) DFmode].insn_code =
    smul_optab->handlers[(int) (int) DFmode].insn_code = CODE_FOR_muldf3;
  if (HAVE_mulxf3)
    smulv_optab->handlers[(int) (int) XFmode].insn_code =
    smul_optab->handlers[(int) (int) XFmode].insn_code = CODE_FOR_mulxf3;
  if (HAVE_divsf3)
    sdiv_optab->handlers[(int) SFmode].insn_code = CODE_FOR_divsf3;
  if (HAVE_divdf3)
    sdiv_optab->handlers[(int) DFmode].insn_code = CODE_FOR_divdf3;
  if (HAVE_divxf3)
    sdiv_optab->handlers[(int) XFmode].insn_code = CODE_FOR_divxf3;
  if (HAVE_modsf3)
    smod_optab->handlers[(int) SFmode].insn_code = CODE_FOR_modsf3;
  if (HAVE_moddf3)
    smod_optab->handlers[(int) DFmode].insn_code = CODE_FOR_moddf3;
  if (HAVE_modxf3)
    smod_optab->handlers[(int) XFmode].insn_code = CODE_FOR_modxf3;
  if (HAVE_anddi3)
    and_optab->handlers[(int) DImode].insn_code = CODE_FOR_anddi3;
  if (HAVE_iordi3)
    ior_optab->handlers[(int) DImode].insn_code = CODE_FOR_iordi3;
  if (HAVE_xordi3)
    xor_optab->handlers[(int) DImode].insn_code = CODE_FOR_xordi3;
  if (HAVE_smaxsi3)
    smax_optab->handlers[(int) SImode].insn_code = CODE_FOR_smaxsi3;
  if (HAVE_sminsi3)
    smin_optab->handlers[(int) SImode].insn_code = CODE_FOR_sminsi3;
  if (HAVE_umaxsi3)
    umax_optab->handlers[(int) SImode].insn_code = CODE_FOR_umaxsi3;
  if (HAVE_uminsi3)
    umin_optab->handlers[(int) SImode].insn_code = CODE_FOR_uminsi3;
  if (HAVE_negsf2)
    negv_optab->handlers[(int) (int) SFmode].insn_code =
    neg_optab->handlers[(int) (int) SFmode].insn_code = CODE_FOR_negsf2;
  if (HAVE_negdf2)
    negv_optab->handlers[(int) (int) DFmode].insn_code =
    neg_optab->handlers[(int) (int) DFmode].insn_code = CODE_FOR_negdf2;
  if (HAVE_negxf2)
    negv_optab->handlers[(int) (int) XFmode].insn_code =
    neg_optab->handlers[(int) (int) XFmode].insn_code = CODE_FOR_negxf2;
  if (HAVE_abssi2)
    abs_optab->handlers[(int) SImode].insn_code = CODE_FOR_abssi2;
  if (HAVE_abssf2)
    absv_optab->handlers[(int) (int) SFmode].insn_code =
    abs_optab->handlers[(int) (int) SFmode].insn_code = CODE_FOR_abssf2;
  if (HAVE_absdf2)
    absv_optab->handlers[(int) (int) DFmode].insn_code =
    abs_optab->handlers[(int) (int) DFmode].insn_code = CODE_FOR_absdf2;
  if (HAVE_absxf2)
    absv_optab->handlers[(int) (int) XFmode].insn_code =
    abs_optab->handlers[(int) (int) XFmode].insn_code = CODE_FOR_absxf2;
  if (HAVE_sqrtsf2)
    sqrt_optab->handlers[(int) SFmode].insn_code = CODE_FOR_sqrtsf2;
  if (HAVE_sqrtdf2)
    sqrt_optab->handlers[(int) DFmode].insn_code = CODE_FOR_sqrtdf2;
  if (HAVE_sqrtxf2)
    sqrt_optab->handlers[(int) XFmode].insn_code = CODE_FOR_sqrtxf2;
  if (HAVE_one_cmpldi2)
    one_cmpl_optab->handlers[(int) DImode].insn_code = CODE_FOR_one_cmpldi2;
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
  if (HAVE_zero_extendsidi2)
    extendtab[(int) DImode][(int) SImode][1] = CODE_FOR_zero_extendsidi2;
  if (HAVE_zero_extendqidi2)
    extendtab[(int) DImode][(int) QImode][1] = CODE_FOR_zero_extendqidi2;
  if (HAVE_extendsidi2)
    extendtab[(int) DImode][(int) SImode][0] = CODE_FOR_extendsidi2;
  if (HAVE_extendsfdf2)
    extendtab[(int) DFmode][(int) SFmode][0] = CODE_FOR_extendsfdf2;
  if (HAVE_extendsfxf2)
    extendtab[(int) XFmode][(int) SFmode][0] = CODE_FOR_extendsfxf2;
  if (HAVE_extenddfxf2)
    extendtab[(int) XFmode][(int) DFmode][0] = CODE_FOR_extenddfxf2;
  if (HAVE_cbranchsi4)
    cbranch_optab->handlers[(int) SImode].insn_code = CODE_FOR_cbranchsi4;
  if (HAVE_adddi3)
    add_optab->handlers[(int) DImode].insn_code = CODE_FOR_adddi3;
  if (HAVE_addsi3)
    add_optab->handlers[(int) SImode].insn_code = CODE_FOR_addsi3;
  if (HAVE_subdi3)
    sub_optab->handlers[(int) DImode].insn_code = CODE_FOR_subdi3;
  if (HAVE_subsi3)
    sub_optab->handlers[(int) SImode].insn_code = CODE_FOR_subsi3;
  if (HAVE_mulsi3)
    smul_optab->handlers[(int) SImode].insn_code = CODE_FOR_mulsi3;
  if (HAVE_andsi3)
    and_optab->handlers[(int) SImode].insn_code = CODE_FOR_andsi3;
  if (HAVE_iorsi3)
    ior_optab->handlers[(int) SImode].insn_code = CODE_FOR_iorsi3;
  if (HAVE_xorsi3)
    xor_optab->handlers[(int) SImode].insn_code = CODE_FOR_xorsi3;
  if (HAVE_ashlsi3)
    ashl_optab->handlers[(int) SImode].insn_code = CODE_FOR_ashlsi3;
  if (HAVE_ashrsi3)
    ashr_optab->handlers[(int) SImode].insn_code = CODE_FOR_ashrsi3;
  if (HAVE_lshrsi3)
    lshr_optab->handlers[(int) SImode].insn_code = CODE_FOR_lshrsi3;
  if (HAVE_rotlsi3)
    rotl_optab->handlers[(int) SImode].insn_code = CODE_FOR_rotlsi3;
  if (HAVE_rotrsi3)
    rotr_optab->handlers[(int) SImode].insn_code = CODE_FOR_rotrsi3;
  if (HAVE_negdi2)
    neg_optab->handlers[(int) DImode].insn_code = CODE_FOR_negdi2;
  if (HAVE_negsi2)
    neg_optab->handlers[(int) SImode].insn_code = CODE_FOR_negsi2;
  if (HAVE_one_cmplsi2)
    one_cmpl_optab->handlers[(int) SImode].insn_code = CODE_FOR_one_cmplsi2;
  if (HAVE_zero_extendhisi2)
    extendtab[(int) SImode][(int) HImode][1] = CODE_FOR_zero_extendhisi2;
  if (HAVE_zero_extendqisi2)
    extendtab[(int) SImode][(int) QImode][1] = CODE_FOR_zero_extendqisi2;
  if (HAVE_extendhisi2)
    extendtab[(int) SImode][(int) HImode][0] = CODE_FOR_extendhisi2;
  if (HAVE_extendqihi2)
    extendtab[(int) HImode][(int) QImode][0] = CODE_FOR_extendqihi2;
  if (HAVE_extendqisi2)
    extendtab[(int) SImode][(int) QImode][0] = CODE_FOR_extendqisi2;
  if (HAVE_movdi)
    mov_optab->handlers[(int) DImode].insn_code = CODE_FOR_movdi;
  if (HAVE_movsi)
    mov_optab->handlers[(int) SImode].insn_code = CODE_FOR_movsi;
  if (HAVE_movhi)
    mov_optab->handlers[(int) HImode].insn_code = CODE_FOR_movhi;
  if (HAVE_reload_outhi)
    reload_out_optab[(int) HImode] = CODE_FOR_reload_outhi;
  if (HAVE_reload_inhi)
    reload_in_optab[(int) HImode] = CODE_FOR_reload_inhi;
  if (HAVE_movqi)
    mov_optab->handlers[(int) QImode].insn_code = CODE_FOR_movqi;
  if (HAVE_movsf)
    mov_optab->handlers[(int) SFmode].insn_code = CODE_FOR_movsf;
  if (HAVE_movdf)
    mov_optab->handlers[(int) DFmode].insn_code = CODE_FOR_movdf;
  if (HAVE_reload_outdf)
    reload_out_optab[(int) DFmode] = CODE_FOR_reload_outdf;
  if (HAVE_movxf)
    mov_optab->handlers[(int) XFmode].insn_code = CODE_FOR_movxf;
  if (HAVE_movstrqi)
    movstr_optab[(int) QImode] = CODE_FOR_movstrqi;
  if (HAVE_cmpsi)
    cmp_optab->handlers[(int) SImode].insn_code = CODE_FOR_cmpsi;
  if (HAVE_cmpsf)
    cmp_optab->handlers[(int) SFmode].insn_code = CODE_FOR_cmpsf;
  if (HAVE_cmpdf)
    cmp_optab->handlers[(int) DFmode].insn_code = CODE_FOR_cmpdf;
  if (HAVE_cmpxf)
    cmp_optab->handlers[(int) XFmode].insn_code = CODE_FOR_cmpxf;
  if (HAVE_beq)
    bcc_gen_fctn[(int) EQ] = gen_beq;
  if (HAVE_bne)
    bcc_gen_fctn[(int) NE] = gen_bne;
  if (HAVE_bgt)
    bcc_gen_fctn[(int) GT] = gen_bgt;
  if (HAVE_ble)
    bcc_gen_fctn[(int) LE] = gen_ble;
  if (HAVE_bge)
    bcc_gen_fctn[(int) GE] = gen_bge;
  if (HAVE_blt)
    bcc_gen_fctn[(int) LT] = gen_blt;
  if (HAVE_bgtu)
    bcc_gen_fctn[(int) GTU] = gen_bgtu;
  if (HAVE_bleu)
    bcc_gen_fctn[(int) LEU] = gen_bleu;
  if (HAVE_bgeu)
    bcc_gen_fctn[(int) GEU] = gen_bgeu;
  if (HAVE_bltu)
    bcc_gen_fctn[(int) LTU] = gen_bltu;
  if (HAVE_bunordered)
    bcc_gen_fctn[(int) UNORDERED] = gen_bunordered;
  if (HAVE_bordered)
    bcc_gen_fctn[(int) ORDERED] = gen_bordered;
  if (HAVE_bungt)
    bcc_gen_fctn[(int) UNGT] = gen_bungt;
  if (HAVE_bunlt)
    bcc_gen_fctn[(int) UNLT] = gen_bunlt;
  if (HAVE_bunge)
    bcc_gen_fctn[(int) UNGE] = gen_bunge;
  if (HAVE_bunle)
    bcc_gen_fctn[(int) UNLE] = gen_bunle;
  if (HAVE_buneq)
    bcc_gen_fctn[(int) UNEQ] = gen_buneq;
  if (HAVE_bltgt)
    bcc_gen_fctn[(int) LTGT] = gen_bltgt;
  if (HAVE_seq)
    setcc_gen_code[(int) EQ] = CODE_FOR_seq;
  if (HAVE_sne)
    setcc_gen_code[(int) NE] = CODE_FOR_sne;
  if (HAVE_sgt)
    setcc_gen_code[(int) GT] = CODE_FOR_sgt;
  if (HAVE_sle)
    setcc_gen_code[(int) LE] = CODE_FOR_sle;
  if (HAVE_sge)
    setcc_gen_code[(int) GE] = CODE_FOR_sge;
  if (HAVE_slt)
    setcc_gen_code[(int) LT] = CODE_FOR_slt;
  if (HAVE_sgtu)
    setcc_gen_code[(int) GTU] = CODE_FOR_sgtu;
  if (HAVE_sleu)
    setcc_gen_code[(int) LEU] = CODE_FOR_sleu;
  if (HAVE_sgeu)
    setcc_gen_code[(int) GEU] = CODE_FOR_sgeu;
  if (HAVE_sltu)
    setcc_gen_code[(int) LTU] = CODE_FOR_sltu;
  if (HAVE_sunordered)
    setcc_gen_code[(int) UNORDERED] = CODE_FOR_sunordered;
  if (HAVE_sordered)
    setcc_gen_code[(int) ORDERED] = CODE_FOR_sordered;
  if (HAVE_sungt)
    setcc_gen_code[(int) UNGT] = CODE_FOR_sungt;
  if (HAVE_sunge)
    setcc_gen_code[(int) UNGE] = CODE_FOR_sunge;
  if (HAVE_sunlt)
    setcc_gen_code[(int) UNLT] = CODE_FOR_sunlt;
  if (HAVE_sunle)
    setcc_gen_code[(int) UNLE] = CODE_FOR_sunle;
  if (HAVE_movsicc)
    movcc_gen_code[(int) SImode] = CODE_FOR_movsicc;
  if (HAVE_movsfcc)
    movcc_gen_code[(int) SFmode] = CODE_FOR_movsfcc;
  if (HAVE_movdfcc)
    movcc_gen_code[(int) DFmode] = CODE_FOR_movdfcc;
  if (HAVE_ffssi2)
    ffs_optab->handlers[(int) SImode].insn_code = CODE_FOR_ffssi2;
}
