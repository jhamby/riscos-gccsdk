/* Generated automatically by the program `genemit'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tm_p.h"
#include "function.h"
#include "expr.h"
#include "optabs.h"
#include "real.h"
#include "flags.h"
#include "output.h"
#include "insn-config.h"
#include "hard-reg-set.h"
#include "recog.h"
#include "resource.h"
#include "reload.h"
#include "toplev.h"
#include "ggc.h"

#define FAIL return (end_sequence (), _val)
#define DONE return (_val = gen_sequence (), end_sequence (), _val)
rtx
gen_incscc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	gen_rtx (GET_CODE (operand2), SImode,
		operand3,
		const0_rtx),
	operand1));
}

rtx
gen_addsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SFmode,
	operand1,
	operand2));
}

rtx
gen_adddf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DFmode,
	operand1,
	operand2));
}

rtx
gen_addxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (XFmode,
	operand1,
	operand2));
}

rtx
gen_decscc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand1,
	gen_rtx (GET_CODE (operand2), SImode,
		operand3,
		const0_rtx)));
}

rtx
gen_subsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SFmode,
	operand1,
	operand2));
}

rtx
gen_subdf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DFmode,
	operand1,
	operand2));
}

rtx
gen_subxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (XFmode,
	operand1,
	operand2));
}

rtx
gen_mulsidi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)));
}

rtx
gen_umulsidi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)));
}

rtx
gen_smulsi3_highpart (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)),
	GEN_INT (32)))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

rtx
gen_umulsi3_highpart (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)),
	GEN_INT (32)))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

rtx
gen_mulhisi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SImode,
	gen_rtx_SIGN_EXTEND (SImode,
	operand1),
	gen_rtx_SIGN_EXTEND (SImode,
	operand2)));
}

rtx
gen_mulsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SFmode,
	operand1,
	operand2));
}

rtx
gen_muldf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DFmode,
	operand1,
	operand2));
}

rtx
gen_mulxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (XFmode,
	operand1,
	operand2));
}

rtx
gen_divsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (SFmode,
	operand1,
	operand2));
}

rtx
gen_divdf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (DFmode,
	operand1,
	operand2));
}

rtx
gen_divxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (XFmode,
	operand1,
	operand2));
}

rtx
gen_modsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MOD (SFmode,
	operand1,
	operand2));
}

rtx
gen_moddf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MOD (DFmode,
	operand1,
	operand2));
}

rtx
gen_modxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MOD (XFmode,
	operand1,
	operand2));
}

rtx
gen_anddi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (DImode,
	operand1,
	operand2));
}

rtx
gen_andsi_notsi_si (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	operand2),
	operand1));
}

rtx
gen_bicsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	operand1),
	operand2));
}

rtx
gen_andsi_not_shiftsi_si (operand0, operand1, operand2, operand3, operand4)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
     rtx operand4;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	gen_rtx (GET_CODE (operand4), SImode,
		operand2,
		operand3)),
	operand1));
}

rtx
gen_iordi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (DImode,
	operand1,
	operand2));
}

rtx
gen_xordi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_XOR (DImode,
	operand1,
	operand2));
}

rtx
gen_smaxsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SMAX (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_sminsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SMIN (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_umaxsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UMAX (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_uminsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UMIN (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_negsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (SFmode,
	operand1));
}

rtx
gen_negdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (DFmode,
	operand1));
}

rtx
gen_negxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (XFmode,
	operand1));
}

rtx
gen_abssi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (SImode,
	operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_abssf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (SFmode,
	operand1));
}

rtx
gen_absdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (DFmode,
	operand1));
}

rtx
gen_absxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (XFmode,
	operand1));
}

rtx
gen_sqrtsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (SFmode,
	operand1));
}

rtx
gen_sqrtdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (DFmode,
	operand1));
}

rtx
gen_sqrtxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (XFmode,
	operand1));
}

rtx
gen_one_cmpldi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (DImode,
	operand1));
}

rtx
gen_floatsisf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (SFmode,
	operand1));
}

rtx
gen_floatsidf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (DFmode,
	operand1));
}

rtx
gen_floatsixf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (XFmode,
	operand1));
}

rtx
gen_fix_truncsfsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

rtx
gen_fix_truncdfsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

rtx
gen_fix_truncxfsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

rtx
gen_truncdfsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_TRUNCATE (SFmode,
	operand1));
}

rtx
gen_truncxfsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_TRUNCATE (SFmode,
	operand1));
}

rtx
gen_truncxfdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_TRUNCATE (DFmode,
	operand1));
}

rtx
gen_zero_extendsidi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1));
}

rtx
gen_zero_extendqidi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1));
}

rtx
gen_extendsidi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1));
}

rtx
gen_extendsfdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_EXTEND (DFmode,
	operand1));
}

rtx
gen_extendsfxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_EXTEND (XFmode,
	operand1));
}

rtx
gen_extenddfxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_EXTEND (XFmode,
	operand1));
}

rtx
gen_pic_load_addr_arm (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	3));
}

rtx
gen_pic_load_addr_thumb (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	3));
}

rtx
gen_pic_add_dot_plus_four (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand0,
	gen_rtx_CONST (VOIDmode,
	gen_rtx_PLUS (SImode,
	pc_rtx,
	GEN_INT (4))))),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1))));
}

rtx
gen_pic_add_dot_plus_eight (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand0,
	gen_rtx_CONST (VOIDmode,
	gen_rtx_PLUS (SImode,
	pc_rtx,
	GEN_INT (8))))),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1))));
}

rtx
gen_rotated_loadsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATE (SImode,
	operand1,
	GEN_INT (16)));
}

rtx
gen_thumb_movhi_clobber (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_CLOBBER (VOIDmode,
	operand2)));
}

rtx
gen_movmem12b (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (8,
		gen_rtx_SET (VOIDmode,
	gen_rtx_MEM (SImode,
	operand2),
	gen_rtx_MEM (SImode,
	operand3)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	operand2,
	GEN_INT (4))),
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	operand3,
	GEN_INT (4)))),
		gen_rtx_SET (VOIDmode,
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	operand2,
	GEN_INT (8))),
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	operand3,
	GEN_INT (8)))),
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand2,
	GEN_INT (12))),
		gen_rtx_SET (VOIDmode,
	operand1,
	gen_rtx_PLUS (SImode,
	operand3,
	GEN_INT (12))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

rtx
gen_movmem8b (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (6,
		gen_rtx_SET (VOIDmode,
	gen_rtx_MEM (SImode,
	operand2),
	gen_rtx_MEM (SImode,
	operand3)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	operand2,
	GEN_INT (4))),
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	operand3,
	GEN_INT (4)))),
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand2,
	GEN_INT (8))),
		gen_rtx_SET (VOIDmode,
	operand1,
	gen_rtx_PLUS (SImode,
	operand3,
	GEN_INT (8))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

rtx
gen_cbranchsi4 (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx (GET_CODE (operand0), VOIDmode,
		operand1,
		operand2),
	gen_rtx_LABEL_REF (VOIDmode,
	operand3),
	pc_rtx));
}

rtx
gen_return ()
{
  return gen_rtx_RETURN (VOIDmode);
}

rtx
gen_blockage ()
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	0);
}

rtx
gen_casesi_internal (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LEU (VOIDmode,
	operand0,
	operand1),
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	gen_rtx_MULT (SImode,
	operand0,
	GEN_INT (4)),
	gen_rtx_LABEL_REF (VOIDmode,
	operand2))),
	gen_rtx_LABEL_REF (VOIDmode,
	operand3))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24)),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand2))));
}

rtx
gen_nop ()
{
  return const0_rtx;
}

rtx
gen_movcond (operand0, operand1, operand2, operand3, operand4, operand5)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
     rtx operand4;
     rtx operand5;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SImode,
	gen_rtx (GET_CODE (operand5), VOIDmode,
		operand3,
		operand4),
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_sibcall_epilogue ()
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	1);
}

rtx
gen_align_4 ()
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	2);
}

rtx
gen_consttable_end ()
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	3);
}

rtx
gen_consttable_1 (operand0)
     rtx operand0;
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	4);
}

rtx
gen_consttable_2 (operand0)
     rtx operand0;
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	5);
}

rtx
gen_consttable_4 (operand0)
     rtx operand0;
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	6);
}

rtx
gen_consttable_8 (operand0)
     rtx operand0;
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	7);
}

rtx
gen_tablejump (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1))));
}

rtx
gen_clz (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	5));
}

rtx
gen_prologue_use (operand0)
     rtx operand0;
{
  return gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand0),
	6);
}

rtx
gen_adddi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (TARGET_THUMB)
    {
      if (GET_CODE (operands[1]) != REG)
        operands[1] = force_reg (SImode, operands[1]);
      if (GET_CODE (operands[2]) != REG)
        operands[2] = force_reg (SImode, operands[2]);
     }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24)))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_360 PARAMS ((rtx *));
rtx
gen_split_360 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CC_Cmode,
	24),
	gen_rtx_COMPARE (CC_Cmode,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2),
	operand1)),
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_PLUS (SImode,
	gen_rtx_LTU (SImode,
	gen_rtx_REG (CC_Cmode,
	24),
	const0_rtx),
	gen_rtx_PLUS (SImode,
	operand4,
	operand5))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_361 PARAMS ((rtx *));
rtx
gen_split_361 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CC_Cmode,
	24),
	gen_rtx_COMPARE (CC_Cmode,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2),
	operand1)),
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_PLUS (SImode,
	gen_rtx_LTU (SImode,
	gen_rtx_REG (CC_Cmode,
	24),
	const0_rtx),
	gen_rtx_PLUS (SImode,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (31)),
	operand4))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_362 PARAMS ((rtx *));
rtx
gen_split_362 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CC_Cmode,
	24),
	gen_rtx_COMPARE (CC_Cmode,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2),
	operand1)),
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_PLUS (SImode,
	gen_rtx_LTU (SImode,
	gen_rtx_REG (CC_Cmode,
	24),
	const0_rtx),
	gen_rtx_PLUS (SImode,
	operand4,
	const0_rtx))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_addsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (TARGET_ARM && GET_CODE (operands[2]) == CONST_INT)
    {
      arm_split_constant (PLUS, SImode, INTVAL (operands[2]), operands[0],
			  operands[1],
			  (no_new_pseudos ? 0 : preserve_subexpressions_p ()));
      DONE;
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_peephole2_364 PARAMS ((rtx, rtx *));
rtx
gen_peephole2_364 (curr_insn, operands)
     rtx curr_insn ATTRIBUTE_UNUSED;
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  if ((operands[3] = peep2_find_free_register (0, 0, "r", SImode, &_regs_allocated)) == NULL_RTX)
    return NULL;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand2));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_365 PARAMS ((rtx *));
rtx
gen_split_365 (operands)
      rtx *operands ATTRIBUTE_UNUSED;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (PLUS, SImode, INTVAL (operands[2]), operands[0],
		      operands[1], 0);
  DONE;
  
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_peephole2_366 PARAMS ((rtx, rtx *));
rtx
gen_peephole2_366 (curr_insn, operands)
     rtx curr_insn ATTRIBUTE_UNUSED;
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand2,
	operand1)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_subdi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (TARGET_THUMB)
    {
      if (GET_CODE (operands[1]) != REG)
        operands[1] = force_reg (SImode, operands[1]);
      if (GET_CODE (operands[2]) != REG)
        operands[2] = force_reg (SImode, operands[2]);
     }	
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24)))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_subsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (GET_CODE (operands[1]) == CONST_INT)
    {
      if (TARGET_ARM)
        {
          arm_split_constant (MINUS, SImode, INTVAL (operands[1]), operands[0],
	  		      operands[2],
			      (no_new_pseudos ? 0
			       :  preserve_subexpressions_p ()));
          DONE;
	}
      else /* TARGET_THUMB */
        operands[1] = force_reg (SImode, operands[1]);
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_369 PARAMS ((rtx *));
rtx
gen_split_369 (operands)
      rtx *operands ATTRIBUTE_UNUSED;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (MINUS, SImode, INTVAL (operands[1]), operands[0],
		      operands[2], 0);
  DONE;
  
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_peephole2_370 PARAMS ((rtx, rtx *));
rtx
gen_peephole2_370 (curr_insn, operands)
     rtx curr_insn ATTRIBUTE_UNUSED;
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  if ((operands[3] = peep2_find_free_register (0, 0, "r", SImode, &_regs_allocated)) == NULL_RTX)
    return NULL;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand3,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_mulsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SImode,
	operand2,
	operand1));
}

extern rtx gen_split_372 PARAMS ((rtx *));
rtx
gen_split_372 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx (GET_CODE (operand6), SImode,
		operand1,
		operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx (GET_CODE (operand6), SImode,
		operand4,
		operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_373 PARAMS ((rtx *));
rtx
gen_split_373 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx (GET_CODE (operand6), SImode,
		operand1,
		operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx (GET_CODE (operand6), SImode,
		gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (31)),
		operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_374 PARAMS ((rtx *));
rtx
gen_split_374 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  {
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand4));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_375 PARAMS ((rtx *));
rtx
gen_split_375 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  {
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_XOR (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand4));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_376 PARAMS ((rtx *));
rtx
gen_split_376 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_andsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (TARGET_ARM)
    {
      if (GET_CODE (operands[2]) == CONST_INT)
        {
          arm_split_constant (AND, SImode, INTVAL (operands[2]), operands[0],
			      operands[1],
			      (no_new_pseudos
			       ? 0 : preserve_subexpressions_p ()));
          DONE;
        }
    }
  else /* TARGET_THUMB */
    {
      if (GET_CODE (operands[2]) != CONST_INT)
        operands[2] = force_reg (SImode, operands[2]);
      else
        {
          int i;
	  
          if (((unsigned HOST_WIDE_INT) ~INTVAL (operands[2])) < 256)
  	    {
	      operands[2] = force_reg (SImode,
				       GEN_INT (~INTVAL (operands[2])));
	      
	      emit_insn (gen_bicsi3 (operands[0], operands[2], operands[1]));
	      
	      DONE;
	    }

          for (i = 9; i <= 31; i++)
	    {
	      if ((((HOST_WIDE_INT) 1) << i) - 1 == INTVAL (operands[2]))
	        {
	          emit_insn (gen_extzv (operands[0], operands[1], GEN_INT (i),
			 	        const0_rtx));
	          DONE;
	        }
	      else if ((((HOST_WIDE_INT) 1) << i) - 1
		       == ~INTVAL (operands[2]))
	        {
	          rtx shift = GEN_INT (i);
	          rtx reg = gen_reg_rtx (SImode);
		
	          emit_insn (gen_lshrsi3 (reg, operands[1], shift));
	          emit_insn (gen_ashlsi3 (operands[0], reg, shift));
		  
	          DONE;
	        }
	    }

          operands[2] = force_reg (SImode, operands[2]);
        }
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_378 PARAMS ((rtx *));
rtx
gen_split_378 (operands)
      rtx *operands ATTRIBUTE_UNUSED;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant  (AND, SImode, INTVAL (operands[2]), operands[0],
		       operands[1], 0);
  DONE;
  
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_insv (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  {
    int start_bit = INTVAL (operands[2]);
    int width = INTVAL (operands[1]);
    HOST_WIDE_INT mask = (((HOST_WIDE_INT)1) << width) - 1;
    rtx target, subtarget;

    target = operands[0];
    /* Avoid using a subreg as a subtarget, and avoid writing a paradoxical 
       subreg as the final target.  */
    if (GET_CODE (target) == SUBREG)
      {
	subtarget = gen_reg_rtx (SImode);
	if (GET_MODE_SIZE (GET_MODE (SUBREG_REG (target)))
	    < GET_MODE_SIZE (SImode))
	  target = SUBREG_REG (target);
      }
    else
      subtarget = target;    

    if (GET_CODE (operands[3]) == CONST_INT)
      {
	/* Since we are inserting a known constant, we may be able to
	   reduce the number of bits that we have to clear so that
	   the mask becomes simple.  */
	/* ??? This code does not check to see if the new mask is actually
	   simpler.  It may not be.  */
	rtx op1 = gen_reg_rtx (SImode);
	/* ??? Truncate operand3 to fit in the bitfield.  See comment before
	   start of this pattern.  */
	HOST_WIDE_INT op3_value = mask & INTVAL (operands[3]);
	HOST_WIDE_INT mask2 = ((mask & ~op3_value) << start_bit);

	emit_insn (gen_andsi3 (op1, operands[0], GEN_INT (~mask2)));
	emit_insn (gen_iorsi3 (subtarget, op1,
			       GEN_INT (op3_value << start_bit)));
      }
    else if (start_bit == 0
	     && !(const_ok_for_arm (mask)
		  || const_ok_for_arm (~mask)))
      {
	/* A Trick, since we are setting the bottom bits in the word,
	   we can shift operand[3] up, operand[0] down, OR them together
	   and rotate the result back again.  This takes 3 insns, and
	   the third might be mergable into another op.  */
	/* The shift up copes with the possibility that operand[3] is
           wider than the bitfield.  */
	rtx op0 = gen_reg_rtx (SImode);
	rtx op1 = gen_reg_rtx (SImode);

	emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
	emit_insn (gen_lshrsi3 (op1, operands[0], operands[1]));
	emit_insn (gen_iorsi3  (op1, op1, op0));
	emit_insn (gen_rotlsi3 (subtarget, op1, operands[1]));
      }
    else if ((width + start_bit == 32)
	     && !(const_ok_for_arm (mask)
		  || const_ok_for_arm (~mask)))
      {
	/* Similar trick, but slightly less efficient.  */

	rtx op0 = gen_reg_rtx (SImode);
	rtx op1 = gen_reg_rtx (SImode);

	emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
	emit_insn (gen_ashlsi3 (op1, operands[0], operands[1]));
	emit_insn (gen_lshrsi3 (op1, op1, operands[1]));
	emit_insn (gen_iorsi3 (subtarget, op1, op0));
      }
    else
      {
	rtx op0 = GEN_INT (mask);
	rtx op1 = gen_reg_rtx (SImode);
	rtx op2 = gen_reg_rtx (SImode);

	if (!(const_ok_for_arm (mask) || const_ok_for_arm (~mask)))
	  {
	    rtx tmp = gen_reg_rtx (SImode);

	    emit_insn (gen_movsi (tmp, op0));
	    op0 = tmp;
	  }

	/* Mask out any bits in operand[3] that are not needed.  */
	   emit_insn (gen_andsi3 (op1, operands[3], op0));

	if (GET_CODE (op0) == CONST_INT
	    && (const_ok_for_arm (mask << start_bit)
		|| const_ok_for_arm (~(mask << start_bit))))
	  {
	    op0 = GEN_INT (~(mask << start_bit));
	    emit_insn (gen_andsi3 (op2, operands[0], op0));
	  }
	else
	  {
	    if (GET_CODE (op0) == CONST_INT)
	      {
		rtx tmp = gen_reg_rtx (SImode);

		emit_insn (gen_movsi (tmp, op0));
		op0 = tmp;
	      }

	    if (start_bit != 0)
	      emit_insn (gen_ashlsi3 (op0, op0, operands[2]));
	    
	    emit_insn (gen_andsi_notsi_si (op2, operands[0], op0));
	  }

	if (start_bit != 0)
          emit_insn (gen_ashlsi3 (op1, op1, operands[2]));

	emit_insn (gen_iorsi3 (subtarget, op1, op2));
      }

    if (subtarget != target)
      {
	/* If TARGET is still a SUBREG, then it must be wider than a word,
	   so we must be careful only to set the subword we were asked to.  */
	if (GET_CODE (target) == SUBREG)
	  emit_move_insn (target, subtarget);
	else
	  emit_move_insn (target, gen_lowpart (GET_MODE (target), subtarget));
      }

    DONE;
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	gen_rtx_ZERO_EXTRACT (SImode,
	operand0,
	operand1,
	operand2),
	operand3));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_380 PARAMS ((rtx *));
rtx
gen_split_380 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	operand1),
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	operand4),
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_381 PARAMS ((rtx *));
rtx
gen_split_381 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	operand1),
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand4));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_382 PARAMS ((rtx *));
rtx
gen_split_382 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	operand1),
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (31))),
	operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_iorsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT)
    {
      if (TARGET_ARM)
        {
          arm_split_constant (IOR, SImode, INTVAL (operands[2]), operands[0],
		 	      operands[1],
			      (no_new_pseudos
			      ? 0 : preserve_subexpressions_p ()));
          DONE;
	}
      else /* TARGET_THUMB */
	operands [2] = force_reg (SImode, operands [2]);
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_384 PARAMS ((rtx *));
rtx
gen_split_384 (operands)
      rtx *operands ATTRIBUTE_UNUSED;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (IOR, SImode, INTVAL (operands[2]), operands[0],
		      operands[1], 0);
  DONE;
  
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_peephole2_385 PARAMS ((rtx, rtx *));
rtx
gen_peephole2_385 (curr_insn, operands)
     rtx curr_insn ATTRIBUTE_UNUSED;
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  if ((operands[3] = peep2_find_free_register (0, 0, "r", SImode, &_regs_allocated)) == NULL_RTX)
    return NULL;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand2));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	operand1,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_xorsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
if (TARGET_THUMB)
     if (GET_CODE (operands[2]) == CONST_INT)
       operands[2] = force_reg (SImode, operands[2]);
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_XOR (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_387 PARAMS ((rtx *));
rtx
gen_split_387 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_AND (SImode,
	gen_rtx_IOR (SImode,
	operand1,
	operand2),
	gen_rtx_NOT (SImode,
	operand3))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (SImode,
	operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_ashlsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_ashrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    operands[2] = GEN_INT (31);
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_lshrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_rotlsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT ((32 - INTVAL (operands[2])) % 32);
  else
    {
      rtx reg = gen_reg_rtx (SImode);
      emit_insn (gen_subsi3 (reg, GEN_INT (32), operands[2]));
      operands[2] = reg;
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATERT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_rotrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (TARGET_ARM)
    {
      if (GET_CODE (operands[2]) == CONST_INT
          && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
        operands[2] = GEN_INT (INTVAL (operands[2]) % 32);
    }
  else /* TARGET_THUMB */
    {
      if (GET_CODE (operands [2]) == CONST_INT)
        operands [2] = force_reg (SImode, operands[2]);
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATERT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extzv (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx operand4;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[5];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  {
    HOST_WIDE_INT lshift = 32 - INTVAL (operands[2]) - INTVAL (operands[3]);
    HOST_WIDE_INT rshift = 32 - INTVAL (operands[2]);
    
    operands[3] = GEN_INT (rshift);
    
    if (lshift == 0)
      {
        emit_insn (gen_lshrsi3 (operands[0], operands[1], operands[3]));
        DONE;
      }
      
    operands[2] = GEN_INT (lshift);
    operands[4] = gen_reg_rtx (SImode);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_ASHIFT (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand4,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_negdi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (TARGET_THUMB)
    {
      if (GET_CODE (operands[1]) != REG)
        operands[1] = force_reg (SImode, operands[1]);
     }
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (DImode,
	operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24)))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_negsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (SImode,
	operand1));
}

extern rtx gen_split_396 PARAMS ((rtx *));
rtx
gen_split_396 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();

  {
    operands[2] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[3] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (SImode,
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_NOT (SImode,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_one_cmplsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (SImode,
	operand1));
}

rtx
gen_zero_extendhisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    if (TARGET_ARM)
      {
        if (arm_arch4 && GET_CODE (operands[1]) == MEM)
          {
           /* Note: We do not have to worry about TARGET_MMU_TRAPS
	      here because the insn below will generate an LDRH instruction
	      rather than an LDR instruction, so we cannot get an unaligned
	      word access.  */
            emit_insn (gen_rtx_SET (VOIDmode, operands[0],
			            gen_rtx_ZERO_EXTEND (SImode,
							 operands[1])));
            DONE;
          }
        if (TARGET_MMU_TRAPS && GET_CODE (operands[1]) == MEM)
          {
            emit_insn (gen_movhi_bytes (operands[0], operands[1]));
            DONE;
          }
        if (!s_register_operand (operands[1], HImode))
          operands[1] = copy_to_mode_reg (HImode, operands[1]);
        operands[1] = gen_lowpart (SImode, operands[1]);
        operands[2] = gen_reg_rtx (SImode);
      }
    else /* TARGET_THUMB */
      {
        if (GET_CODE (operands[1]) == MEM)
	  {
	    rtx tmp;

	    tmp = gen_rtx_ZERO_EXTEND (SImode, operands[1]);
	    tmp = gen_rtx_SET (VOIDmode, operands[0], tmp);
	    emit_insn (tmp);
	  }
	else
	  {
	    rtx ops[3];
	    
	    if (!s_register_operand (operands[1], HImode))
	      operands[1] = copy_to_mode_reg (HImode, operands[1]);
	    operands[1] = gen_lowpart (SImode, operands[1]);
	    operands[2] = gen_reg_rtx (SImode);
	    
	    ops[0] = operands[2];
	    ops[1] = operands[1];
	    ops[2] = GEN_INT (16);
	    
	    emit_insn (gen_rtx_SET (VOIDmode, ops[0],
				    gen_rtx_ASHIFT (SImode, ops[1], ops[2])));

	    ops[0] = operands[0];
	    ops[1] = operands[2];
	    ops[2] = GEN_INT (16);

	    emit_insn (gen_rtx_SET (VOIDmode, ops[0],
				    gen_rtx_LSHIFTRT (SImode, ops[1],
						      ops[2])));
	  }
	DONE; 
      }
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_399 PARAMS ((rtx *));
rtx
gen_split_399 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  if ((operands[1] = arm_gen_rotated_half_load (operands[1])) == NULL)
    FAIL;
  
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_400 PARAMS ((rtx *));
rtx
gen_split_400 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  if ((operands[1] = arm_gen_rotated_half_load (operands[1])) == NULL)
    FAIL;
  
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx (GET_CODE (operand3), GET_MODE (operand3),
		gen_rtx_LSHIFTRT (SImode,
	operand2,
	GEN_INT (16)),
		operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_zero_extendqisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (GET_CODE (operands[1]) != MEM)
    {
      if (TARGET_ARM)
        {
          emit_insn (gen_andsi3 (operands[0],
				 gen_lowpart (SImode, operands[1]),
			         GEN_INT (255)));
        }
      else /* TARGET_THUMB */
        {
          rtx temp = gen_reg_rtx (SImode);
	  rtx ops[3];
	  
          operands[1] = copy_to_mode_reg (QImode, operands[1]);
          operands[1] = gen_lowpart (SImode, operands[1]);

	  ops[0] = temp;
	  ops[1] = operands[1];
	  ops[2] = GEN_INT (24);

	  emit_insn (gen_rtx_SET (VOIDmode, ops[0],
				  gen_rtx_ASHIFT (SImode, ops[1], ops[2])));
	  
          ops[0] = operands[0];
	  ops[1] = temp;
	  ops[2] = GEN_INT (24);

	  emit_insn (gen_rtx_SET (VOIDmode, ops[0],
				  gen_rtx_LSHIFTRT (SImode, ops[1], ops[2])));
	}
      DONE;
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_402 PARAMS ((rtx *));
rtx
gen_split_402 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	operand2,
	GEN_INT (255))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendhisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    if (TARGET_ARM && arm_arch4 && GET_CODE (operands[1]) == MEM)
      {
       /* Note: We do not have to worry about TARGET_MMU_TRAPS
	  here because the insn below will generate an LDRH instruction
	  rather than an LDR instruction, so we cannot get an unaligned
	  word access.  */
        emit_insn (gen_rtx_SET (VOIDmode, operands[0],
		   gen_rtx_SIGN_EXTEND (SImode, operands[1])));
        DONE;
      }

    if (TARGET_ARM && TARGET_MMU_TRAPS && GET_CODE (operands[1]) == MEM)
      {
        emit_insn (gen_extendhisi2_mem (operands[0], operands[1]));
        DONE;
      }
    if (!s_register_operand (operands[1], HImode))
      operands[1] = copy_to_mode_reg (HImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_reg_rtx (SImode);

    if (TARGET_THUMB)
      {
	rtx ops[3];
	
	ops[0] = operands[2];
	ops[1] = operands[1];
	ops[2] = GEN_INT (16);
	
        emit_insn (gen_rtx_SET (VOIDmode, ops[0],
				gen_rtx_ASHIFT (SImode, ops[1], ops[2])));
	    
	ops[0] = operands[0];
	ops[1] = operands[2];
	ops[2] = GEN_INT (16);
	
        emit_insn (gen_rtx_SET (VOIDmode, ops[0],
				gen_rtx_ASHIFTRT (SImode, ops[1], ops[2])));
	
	DONE;
      }
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendhisi2_mem (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[8];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    rtx mem1, mem2;
    rtx addr = copy_to_mode_reg (SImode, XEXP (operands[1], 0));

    mem1 = gen_rtx_MEM (QImode, addr);
    MEM_COPY_ATTRIBUTES (mem1, operands[1]);
    mem2 = gen_rtx_MEM (QImode, plus_constant (addr, 1));
    MEM_COPY_ATTRIBUTES (mem2, operands[1]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = mem1;
    operands[2] = gen_reg_rtx (SImode);
    operands[3] = gen_reg_rtx (SImode);
    operands[6] = gen_reg_rtx (SImode);
    operands[7] = mem2;

    if (BYTES_BIG_ENDIAN)
      {
	operands[4] = operands[2];
	operands[5] = operands[3];
      }
    else
      {
	operands[4] = operands[3];
	operands[5] = operands[2];
      }
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
    operand6 = operands[6];
    operand7 = operands[7];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_ZERO_EXTEND (SImode,
	operand7)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	gen_rtx_ASHIFT (SImode,
	operand4,
	GEN_INT (24))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	gen_rtx_ASHIFTRT (SImode,
	operand6,
	GEN_INT (16)),
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_405 PARAMS ((rtx *));
rtx
gen_split_405 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  if ((operands[1] = arm_gen_rotated_half_load (operands[1])) == NULL)
    FAIL;
  
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_406 PARAMS ((rtx *));
rtx
gen_split_406 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();
if ((operands[1] = arm_gen_rotated_half_load (operands[1])) == NULL)
     FAIL;
  
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx (GET_CODE (operand3), GET_MODE (operand3),
		gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16)),
		operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendqihi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    if (arm_arch4 && GET_CODE (operands[1]) == MEM)
      {
	emit_insn (gen_rtx_SET (VOIDmode,
				operands[0],
				gen_rtx_SIGN_EXTEND (HImode, operands[1])));
	DONE;
      }
    if (!s_register_operand (operands[1], QImode))
      operands[1] = copy_to_mode_reg (QImode, operands[1]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_reg_rtx (SImode);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (24))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (24))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_408 PARAMS ((rtx *));
rtx
gen_split_408 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();

  {
    HOST_WIDE_INT offset;

    operands[3] = gen_rtx_REG (SImode, REGNO (operands[0]));
    operands[2] = gen_rtx_MEM (QImode, operands[3]);
    MEM_COPY_ATTRIBUTES (operands[2], operands[1]);
    operands[1] = XEXP (operands[1], 0);
    if (GET_CODE (operands[1]) == PLUS
	&& GET_CODE (XEXP (operands[1], 1)) == CONST_INT
	&& !(const_ok_for_arm (offset = INTVAL (XEXP (operands[1], 1)))
	     || const_ok_for_arm (-offset)))
      {
	HOST_WIDE_INT low = (offset > 0
			     ? (offset & 0xff) : -((-offset) & 0xff));
	XEXP (operands[2], 0) = plus_constant (operands[3], low);
	operands[1] = plus_constant (XEXP (operands[1], 0), offset - low);
      }
    /* Ensure the sum is in correct canonical form */
    else if (GET_CODE (operands[1]) == PLUS
	     && GET_CODE (XEXP (operands[1], 1)) != CONST_INT
	     && !s_register_operand (XEXP (operands[1], 1), VOIDmode))
      operands[1] = gen_rtx_PLUS (GET_MODE (operands[1]),
					   XEXP (operands[1], 1),
					   XEXP (operands[1], 0));
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (HImode,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendqisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    if (TARGET_ARM && arm_arch4 && GET_CODE (operands[1]) == MEM)
      {
        emit_insn (gen_rtx_SET (VOIDmode,
			        operands[0],
			        gen_rtx_SIGN_EXTEND (SImode, operands[1])));
        DONE;
      }
    if (!s_register_operand (operands[1], QImode))
      operands[1] = copy_to_mode_reg (QImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_reg_rtx (SImode);
    
    if (TARGET_THUMB)
      {
	rtx ops[3];
	
	ops[0] = operands[2];
	ops[1] = operands[1];
	ops[2] = GEN_INT (24);
	
        emit_insn (gen_rtx_SET (VOIDmode, ops[0],
		   gen_rtx_ASHIFT (SImode, ops[1], ops[2])));

	ops[0] = operands[0];
	ops[1] = operands[2];
	ops[2] = GEN_INT (24);
	
        emit_insn (gen_rtx_SET (VOIDmode, ops[0],
		   gen_rtx_ASHIFTRT (SImode, ops[1], ops[2])));
	
	DONE;
      }
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (24))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (24))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_410 PARAMS ((rtx *));
rtx
gen_split_410 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  {
    HOST_WIDE_INT offset;

    operands[2] = gen_rtx_MEM (QImode, operands[0]);
    MEM_COPY_ATTRIBUTES (operands[2], operands[1]);
    operands[1] = XEXP (operands[1], 0);
    if (GET_CODE (operands[1]) == PLUS
	&& GET_CODE (XEXP (operands[1], 1)) == CONST_INT
	&& !(const_ok_for_arm (offset = INTVAL (XEXP (operands[1], 1)))
	     || const_ok_for_arm (-offset)))
      {
	HOST_WIDE_INT low = (offset > 0
			     ? (offset & 0xff) : -((-offset) & 0xff));
	XEXP (operands[2], 0) = plus_constant (operands[0], low);
	operands[1] = plus_constant (XEXP (operands[1], 0), offset - low);
      }
    /* Ensure the sum is in correct canonical form */
    else if (GET_CODE (operands[1]) == PLUS
	     && GET_CODE (XEXP (operands[1], 1)) != CONST_INT
	     && !s_register_operand (XEXP (operands[1], 1), VOIDmode))
      operands[1] = gen_rtx_PLUS (GET_MODE (operands[1]),
					   XEXP (operands[1], 1),
					   XEXP (operands[1], 0));
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (SImode,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movdi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (TARGET_THUMB)
    {
      if (!no_new_pseudos)
        {
          if (GET_CODE (operands[0]) != REG)
	    operands[1] = force_reg (DImode, operands[1]);
        }
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (TARGET_ARM)
    {
      /* Everything except mem = const or mem = mem can be done easily */
      if (GET_CODE (operands[0]) == MEM)
        operands[1] = force_reg (SImode, operands[1]);
      if (GET_CODE (operands[1]) == CONST_INT
          && !(const_ok_for_arm (INTVAL (operands[1]))
               || const_ok_for_arm (~INTVAL (operands[1]))))
        {
           arm_split_constant (SET, SImode, INTVAL (operands[1]), operands[0],
		    	      NULL_RTX,
			      (no_new_pseudos ? 0
			       : preserve_subexpressions_p ()));
          DONE;
        }
    }
  else /* TARGET_THUMB.... */
    {
      if (!no_new_pseudos)
        {
          if (GET_CODE (operands[0]) != REG)
	    operands[1] = force_reg (SImode, operands[1]);
        }
    }
    
  if (flag_pic
      && (CONSTANT_P (operands[1])
	 || symbol_mentioned_p (operands[1])
	 || label_mentioned_p (operands[1])))
    operands[1] = legitimize_pic_address (operands[1], SImode,
					  (no_new_pseudos ? operands[0] : 0));
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_413 PARAMS ((rtx *));
rtx
gen_split_413 (operands)
      rtx *operands ATTRIBUTE_UNUSED;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (SET, SImode, INTVAL (operands[1]), operands[0],
		      NULL_RTX, 0);
  DONE;
  
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_414 PARAMS ((rtx *));
rtx
gen_split_414 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
operands[1] = GEN_INT (- INTVAL (operands[1]));
  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (SImode,
	operand0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_415 PARAMS ((rtx *));
rtx
gen_split_415 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  {
    unsigned HOST_WIDE_INT val = INTVAL (operands[1]);
    unsigned HOST_WIDE_INT mask = 0xff;
    int i;
    
    for (i = 0; i < 25; i++)
      if ((val & (mask << i)) == val)
        break;

    if (i == 0)
      FAIL;

    operands[1] = GEN_INT (val >> i);
    operands[2] = GEN_INT (i);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand0,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movaddr (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

rtx
gen_pic_load_addr_based (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
operands[2] = pic_offset_table_rtx;
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_builtin_setjmp_receiver (operand0)
     rtx operand0;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;

{
  arm_finalize_pic (0);
  DONE;
}
    operand0 = operands[0];
  }
  emit_insn (gen_rtx_LABEL_REF (VOIDmode,
	operand0));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_storehi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[5];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    rtx op1 = operands[1];
    rtx addr = XEXP (op1, 0);
    enum rtx_code code = GET_CODE (addr);

    if ((code == PLUS && GET_CODE (XEXP (addr, 1)) != CONST_INT)
	|| code == MINUS)
      op1 = replace_equiv_address (operands[1], force_reg (SImode, addr));

    operands[4] = adjust_address (op1, QImode, 1);
    operands[1] = adjust_address (operands[1], QImode, 0);
    operands[3] = gen_lowpart (QImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[2] = gen_reg_rtx (SImode); 
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand1,
	operand3));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFTRT (SImode,
	operand0,
	GEN_INT (8))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_SUBREG (QImode,
	operand2,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_storehi_bigend (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[5];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    rtx op1 = operands[1];
    rtx addr = XEXP (op1, 0);
    enum rtx_code code = GET_CODE (addr);

    if ((code == PLUS && GET_CODE (XEXP (addr, 1)) != CONST_INT)
	|| code == MINUS)
      op1 = replace_equiv_address (op1, force_reg (SImode, addr));

    operands[4] = adjust_address (op1, QImode, 1);
    operands[1] = adjust_address (operands[1], QImode, 0);
    operands[3] = gen_lowpart (QImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[2] = gen_reg_rtx (SImode);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	operand3));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFTRT (SImode,
	operand0,
	GEN_INT (8))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand1,
	gen_rtx_SUBREG (QImode,
	operand2,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_storeinthi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    HOST_WIDE_INT value = INTVAL (operands[1]);
    rtx addr = XEXP (operands[0], 0);
    rtx op0 = operands[0];
    enum rtx_code code = GET_CODE (addr);

    if ((code == PLUS && GET_CODE (XEXP (addr, 1)) != CONST_INT)
	|| code == MINUS)
      op0 = replace_equiv_address (op0, force_reg (SImode, addr));

    operands[1] = gen_reg_rtx (SImode);
    if (BYTES_BIG_ENDIAN)
      {
	emit_insn (gen_movsi (operands[1], GEN_INT ((value >> 8) & 255)));
	if ((value & 255) == ((value >> 8) & 255))
	  operands[2] = operands[1];
	else
	  {
	    operands[2] = gen_reg_rtx (SImode);
	    emit_insn (gen_movsi (operands[2], GEN_INT (value & 255)));
	  }
      }
    else
      {
	emit_insn (gen_movsi (operands[1], GEN_INT (value & 255)));
	if ((value & 255) == ((value >> 8) & 255))
	  operands[2] = operands[1];
	else
	  {
	    operands[2] = gen_reg_rtx (SImode);
	    emit_insn (gen_movsi (operands[2], GEN_INT ((value >> 8) & 255)));
	  }
      }

    operands[3] = adjust_address (op0, QImode, 1);
    operands[0] = adjust_address (operands[0], QImode, 0);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SUBREG (QImode,
	operand1,
	0)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_SUBREG (QImode,
	operand2,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_storehi_single_op (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (!s_register_operand (operands[1], HImode))
    operands[1] = copy_to_mode_reg (HImode, operands[1]);
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movhi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (TARGET_ARM)
    {
      if (!no_new_pseudos)
        {
          if (GET_CODE (operands[0]) == MEM)
	    {
	      if (arm_arch4)
	        {
	          emit_insn (gen_storehi_single_op (operands[0], operands[1]));
	          DONE;
	        }
	      if (GET_CODE (operands[1]) == CONST_INT)
	        emit_insn (gen_storeinthi (operands[0], operands[1]));
	      else
	        {
	          if (GET_CODE (operands[1]) == MEM)
		    operands[1] = force_reg (HImode, operands[1]);
	          if (BYTES_BIG_ENDIAN)
		    emit_insn (gen_storehi_bigend (operands[1], operands[0]));
	          else
		   emit_insn (gen_storehi (operands[1], operands[0]));
	        }
	      DONE;
	    }
          /* Sign extend a constant, and keep it in an SImode reg.  */
          else if (GET_CODE (operands[1]) == CONST_INT)
	    {
	      rtx reg = gen_reg_rtx (SImode);
	      HOST_WIDE_INT val = INTVAL (operands[1]) & 0xffff;

	      /* If the constant is already valid, leave it alone.  */
	      if (!const_ok_for_arm (val))
	        {
	          /* If setting all the top bits will make the constant 
		     loadable in a single instruction, then set them.  
		     Otherwise, sign extend the number.  */

	          if (const_ok_for_arm (~(val | ~0xffff)))
		    val |= ~0xffff;
	          else if (val & 0x8000)
		    val |= ~0xffff;
	        }

	      emit_insn (gen_movsi (reg, GEN_INT (val)));
	      operands[1] = gen_rtx_SUBREG (HImode, reg, 0);
	    }
          else if (!arm_arch4)
	    {
	     /* Note: We do not have to worry about TARGET_MMU_TRAPS
	        for v4 and up architectures because LDRH instructions will
	        be used to access the HI values, and these cannot generate
	        unaligned word access faults in the MMU.  */
	      if (GET_CODE (operands[1]) == MEM)
	        {
	          if (TARGET_MMU_TRAPS)
		    {
		      rtx base;
		      rtx offset = const0_rtx;
		      rtx reg = gen_reg_rtx (SImode);

		      if ((GET_CODE (base = XEXP (operands[1], 0)) == REG
		           || (GET_CODE (base) == PLUS
			       && (GET_CODE (offset = XEXP (base, 1))
				   == CONST_INT)
                               && ((INTVAL(offset) & 1) != 1)
			       && GET_CODE (base = XEXP (base, 0)) == REG))
		          && REGNO_POINTER_ALIGN (REGNO (base)) >= 32)
		        {
		          HOST_WIDE_INT new_offset = INTVAL (offset) & ~3;
		          rtx new;

		          new = gen_rtx_MEM (SImode,
				   	     plus_constant (base, new_offset));
	                  MEM_COPY_ATTRIBUTES (new, operands[1]);
		          emit_insn (gen_movsi (reg, new));
		          if (((INTVAL (offset) & 2) != 0)
			      ^ (BYTES_BIG_ENDIAN ? 1 : 0))
			    {
			      rtx reg2 = gen_reg_rtx (SImode);

			      emit_insn (gen_lshrsi3 (reg2, reg,
					 GEN_INT (16)));
			      reg = reg2;
			    }
		        }
		      else
		        emit_insn (gen_movhi_bytes (reg, operands[1]));

		      operands[1] = gen_lowpart (HImode, reg);
		    }
	          else if (BYTES_BIG_ENDIAN)
		    {
		      rtx base;
		      rtx offset = const0_rtx;

		      if ((GET_CODE (base = XEXP (operands[1], 0)) == REG
		           || (GET_CODE (base) == PLUS
			      && (GET_CODE (offset = XEXP (base, 1))
				  == CONST_INT)
			      && GET_CODE (base = XEXP (base, 0)) == REG))
		          && REGNO_POINTER_ALIGN (REGNO (base)) >= 32)
		        {
		          rtx reg = gen_reg_rtx (SImode);
		          rtx new;

		          if ((INTVAL (offset) & 2) == 2)
			    {
			      HOST_WIDE_INT new_offset = INTVAL (offset) ^ 2;
			      new = gen_rtx_MEM (SImode,
				  	         plus_constant (base,
								new_offset));
                              MEM_COPY_ATTRIBUTES (new, operands[1]);
			      emit_insn (gen_movsi (reg, new));
			    }
		          else
			    {
			      new = gen_rtx_MEM (SImode,
						 XEXP (operands[1], 0));
	                      MEM_COPY_ATTRIBUTES (new, operands[1]);
			      emit_insn (gen_rotated_loadsi (reg, new));
			    }

		          operands[1] = gen_lowpart (HImode, reg);
		        }
		      else
		        {
		          emit_insn (gen_movhi_bigend (operands[0],
						       operands[1]));
		          DONE;
		        }
		    }
	       }
	   }
        }
      /* Handle loading a large integer during reload */
      else if (GET_CODE (operands[1]) == CONST_INT
	       && !const_ok_for_arm (INTVAL (operands[1]))
	       && !const_ok_for_arm (~INTVAL (operands[1])))
        {
          /* Writing a constant to memory needs a scratch, which should
	     be handled with SECONDARY_RELOADs.  */
          if (GET_CODE (operands[0]) != REG)
	    abort ();

          operands[0] = gen_rtx_SUBREG (SImode, operands[0], 0);
          emit_insn (gen_movsi (operands[0], operands[1]));
          DONE;
       }
    }
  else /* TARGET_THUMB */
    {
      if (!no_new_pseudos)
        {
          if (GET_CODE (operands[0]) != REG)
	    operands[1] = force_reg (HImode, operands[1]);

          /* ??? We shouldn't really get invalid addresses here, but this can
	     happen if we are passed a SP (never OK for HImode/QImode) or 
	     virtual register (rejected by GO_IF_LEGITIMATE_ADDRESS for 
	     HImode/QImode) relative address.  */
          /* ??? This should perhaps be fixed elsewhere, for instance, in
	     fixup_stack_1, by checking for other kinds of invalid addresses,
	     e.g. a bare reference to a virtual register.  This may confuse the
	     alpha though, which must handle this case differently.  */
          if (GET_CODE (operands[0]) == MEM
	      && !memory_address_p (GET_MODE (operands[0]),
				    XEXP (operands[0], 0)))
	    operands[0]
	      = replace_equiv_address (operands[0],
				       copy_to_reg (XEXP (operands[0], 0)));
   
          if (GET_CODE (operands[1]) == MEM
	      && !memory_address_p (GET_MODE (operands[1]),
				    XEXP (operands[1], 0)))
	    operands[1]
	      = replace_equiv_address (operands[1],
				       copy_to_reg (XEXP (operands[1], 0)));
        }
      /* Handle loading a large integer during reload */
      else if (GET_CODE (operands[1]) == CONST_INT
	        && !CONST_OK_FOR_THUMB_LETTER (INTVAL (operands[1]), 'I'))
        {
          /* Writing a constant to memory needs a scratch, which should
	     be handled with SECONDARY_RELOADs.  */
          if (GET_CODE (operands[0]) != REG)
	    abort ();

          operands[0] = gen_rtx (SUBREG, SImode, operands[0], 0);
          emit_insn (gen_movsi (operands[0], operands[1]));
          DONE;
        }
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movhi_bytes (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[7];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    rtx mem1, mem2;
    rtx addr = copy_to_mode_reg (SImode, XEXP (operands[1], 0));

    mem1 = gen_rtx_MEM (QImode, addr);
    MEM_COPY_ATTRIBUTES (mem1, operands[1]);
    mem2 = gen_rtx_MEM (QImode, plus_constant (addr, 1));
    MEM_COPY_ATTRIBUTES (mem2, operands[1]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = mem1;
    operands[2] = gen_reg_rtx (SImode);
    operands[3] = gen_reg_rtx (SImode);
    operands[6] = mem2;

    if (BYTES_BIG_ENDIAN)
      {
	operands[4] = operands[2];
	operands[5] = operands[3];
      }
    else
      {
	operands[4] = operands[3];
	operands[5] = operands[2];
      }
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
    operand6 = operands[6];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_ZERO_EXTEND (SImode,
	operand6)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	gen_rtx_ASHIFT (SImode,
	operand4,
	GEN_INT (8)),
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movhi_bigend (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;

  operands[2] = gen_reg_rtx (SImode);
  operands[3] = gen_reg_rtx (SImode);
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ROTATE (SImode,
	gen_rtx_SUBREG (SImode,
	operand1,
	0),
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SUBREG (HImode,
	operand3,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_reload_outhi (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
if (TARGET_ARM)
     arm_reload_out_hi (operands);
   else
     thumb_reload_out_hi (operands);
  DONE;
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		operand0,
		operand1,
		operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_reload_inhi (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  if (TARGET_ARM)
    arm_reload_in_hi (operands);
  else
    thumb_reload_out_hi (operands);
  DONE;

    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		operand0,
		operand1,
		operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movqi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (TARGET_ARM)
    {
      /* Everything except mem = const or mem = mem can be done easily */

      if (!no_new_pseudos)
        {
          if (GET_CODE (operands[1]) == CONST_INT)
	    {
	      rtx reg = gen_reg_rtx (SImode);

	      emit_insn (gen_movsi (reg, operands[1]));
	      operands[1] = gen_rtx_SUBREG (QImode, reg, 0);
	    }
         if (GET_CODE (operands[0]) == MEM)
	   operands[1] = force_reg (QImode, operands[1]);
       }
    }
  else /* TARGET_THUMB */
    {
      if (!no_new_pseudos)
        {
          if (GET_CODE (operands[0]) != REG)
	    operands[1] = force_reg (QImode, operands[1]);

          /* ??? We shouldn't really get invalid addresses here, but this can
	     happen if we are passed a SP (never OK for HImode/QImode) or
	     virtual register (rejected by GO_IF_LEGITIMATE_ADDRESS for
	     HImode/QImode) relative address.  */
          /* ??? This should perhaps be fixed elsewhere, for instance, in
	     fixup_stack_1, by checking for other kinds of invalid addresses,
	     e.g. a bare reference to a virtual register.  This may confuse the
	     alpha though, which must handle this case differently.  */
          if (GET_CODE (operands[0]) == MEM
	      && !memory_address_p (GET_MODE (operands[0]),
		  		     XEXP (operands[0], 0)))
	    operands[0]
	      = replace_equiv_address (operands[0],
				       copy_to_reg (XEXP (operands[0], 0)));
          if (GET_CODE (operands[1]) == MEM
	      && !memory_address_p (GET_MODE (operands[1]),
				    XEXP (operands[1], 0)))
	     operands[1]
	       = replace_equiv_address (operands[1],
					copy_to_reg (XEXP (operands[1], 0)));
        }
      /* Handle loading a large integer during reload */
      else if (GET_CODE (operands[1]) == CONST_INT
	       && !CONST_OK_FOR_LETTER_P (INTVAL (operands[1]), 'I'))
        {
          /* Writing a constant to memory needs a scratch, which should
	     be handled with SECONDARY_RELOADs.  */
          if (GET_CODE (operands[0]) != REG)
	    abort ();

          operands[0] = gen_rtx (SUBREG, SImode, operands[0], 0);
          emit_insn (gen_movsi (operands[0], operands[1]));
          DONE;
       }
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movsf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (TARGET_ARM)
    {
      if (GET_CODE (operands[0]) == MEM)
        operands[1] = force_reg (SFmode, operands[1]);
    }
  else /* TARGET_THUMB */
    {
      if (!no_new_pseudos)
        {
           if (GET_CODE (operands[0]) != REG)
	     operands[1] = force_reg (SFmode, operands[1]);
        }
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_430 PARAMS ((rtx *));
rtx
gen_split_430 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();

  operands[2] = gen_lowpart (SImode, operands[0]);
  operands[3] = gen_lowpart (SImode, operands[1]);
  if (operands[2] == 0 || operands[3] == 0)
    FAIL;
  
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand3));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movdf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  if (TARGET_ARM)
    {
      if (GET_CODE (operands[0]) == MEM)
        operands[1] = force_reg (DFmode, operands[1]);
    }
  else /* TARGET_THUMB */
    {
      if (!no_new_pseudos)
        {
          if (GET_CODE (operands[0]) != REG)
	    operands[1] = force_reg (DFmode, operands[1]);
        }
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_reload_outdf (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  {
    enum rtx_code code = GET_CODE (XEXP (operands[0], 0));

    if (code == REG)
      operands[2] = XEXP (operands[0], 0);
    else if (code == POST_INC || code == PRE_DEC)
      {
	operands[0] = gen_rtx_SUBREG (DImode, operands[0], 0);
	operands[1] = gen_rtx_SUBREG (DImode, operands[1], 0);
	emit_insn (gen_movdi (operands[0], operands[1]));
	DONE;
      }
    else if (code == PRE_INC)
      {
	rtx reg = XEXP (XEXP (operands[0], 0), 0);

	emit_insn (gen_addsi3 (reg, reg, GEN_INT (8)));
	operands[2] = reg;
      }
    else if (code == POST_DEC)
      operands[2] = XEXP (XEXP (operands[0], 0), 0);
    else
      emit_insn (gen_addsi3 (operands[2], XEXP (XEXP (operands[0], 0), 0),
			     XEXP (XEXP (operands[0], 0), 1)));

    emit_insn (gen_rtx_SET (VOIDmode, gen_rtx_MEM (DFmode, operands[2]),
			    operands[1]));

    if (code == POST_DEC)
      emit_insn (gen_addsi3 (operands[2], operands[2], GEN_INT (-8)));

    DONE;
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (operand0);
  emit (operand1);
  emit (operand2);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movxf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

rtx
gen_load_multiple (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  /* Support only fixed point registers.  */
  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) > 14
      || INTVAL (operands[2]) < 2
      || GET_CODE (operands[1]) != MEM
      || GET_CODE (operands[0]) != REG
      || REGNO (operands[0]) > (LAST_ARM_REGNUM - 1)
      || REGNO (operands[0]) + INTVAL (operands[2]) > LAST_ARM_REGNUM)
    FAIL;

  operands[3]
    = arm_gen_load_multiple (REGNO (operands[0]), INTVAL (operands[2]),
			     force_reg (SImode, XEXP (operands[1], 0)),
			     TRUE, FALSE, RTX_UNCHANGING_P(operands[1]),
			     MEM_IN_STRUCT_P(operands[1]),
	                     MEM_SCALAR_P (operands[1]));
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit (operand3);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_store_multiple (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  /* Support only fixed point registers */
  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) > 14
      || INTVAL (operands[2]) < 2
      || GET_CODE (operands[1]) != REG
      || GET_CODE (operands[0]) != MEM
      || REGNO (operands[1]) > (LAST_ARM_REGNUM - 1)
      || REGNO (operands[1]) + INTVAL (operands[2]) > LAST_ARM_REGNUM)
    FAIL;

  operands[3]
    = arm_gen_store_multiple (REGNO (operands[1]), INTVAL (operands[2]),
			      force_reg (SImode, XEXP (operands[0], 0)),
			      TRUE, FALSE, RTX_UNCHANGING_P (operands[0]),
			      MEM_IN_STRUCT_P(operands[0]), 
	                      MEM_SCALAR_P (operands[0]));
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit (operand3);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movstrqi (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  if (TARGET_ARM)
    {
      if (arm_gen_movstrqi (operands))
        DONE;
      FAIL;
    }
  else /* TARGET_THUMB */
    {
      if (   INTVAL (operands[3]) != 4
          || INTVAL (operands[2]) > 48)
        FAIL;

      thumb_expand_movstrqi (operands);
      DONE;
    }
  
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit (operand0);
  emit (operand1);
  emit (operand2);
  emit (operand3);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
{
    arm_compare_op0 = operands[0];
    arm_compare_op1 = operands[1];
    DONE;
  }
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpsf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  arm_compare_op0 = operands[0];
  arm_compare_op1 = operands[1];
  DONE;
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpdf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  arm_compare_op0 = operands[0];
  arm_compare_op1 = operands[1];
  DONE;
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpxf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  arm_compare_op0 = operands[0];
  arm_compare_op1 = operands[1];
  DONE;
  
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_beq (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (EQ, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_EQ (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bne (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (NE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_NE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bgt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GT, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GT (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_ble (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bge (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_blt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LT, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LT (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bgtu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GTU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GTU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bleu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LEU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LEU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bgeu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GEU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GEU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bltu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LTU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LTU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bunordered (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNORDERED, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNORDERED (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bordered (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (ORDERED, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_ORDERED (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bungt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNGT, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNGT (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bunlt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNLT, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNLT (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bunge (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNGE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNGE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bunle (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNLE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNLE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_buneq (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNEQ, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNEQ (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bltgt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LTGT, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LTGT (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_seq (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (EQ, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_EQ (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sne (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (NE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sgt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GT, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GT (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sle (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sge (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GE, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_slt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LT, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LT (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sgtu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GTU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GTU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sleu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LEU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LEU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sgeu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (GEU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GEU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sltu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (LTU, arm_compare_op0, arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LTU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sunordered (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNORDERED, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNORDERED (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sordered (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (ORDERED, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ORDERED (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sungt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNGT, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNGT (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sunge (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNGE, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNGE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sunlt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNLT, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNLT (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sunle (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
operands[1] = arm_gen_compare_reg (UNLE, arm_compare_op0,
				      arm_compare_op1);
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNLE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movsicc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  {
    enum rtx_code code = GET_CODE (operands[1]);
    rtx ccreg = arm_gen_compare_reg (code, arm_compare_op0, arm_compare_op1);

    operands[1] = gen_rtx (code, VOIDmode, ccreg, const0_rtx);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SImode,
	operand1,
	operand2,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movsfcc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  {
    enum rtx_code code = GET_CODE (operands[1]);
    rtx ccreg;

    /* When compiling for SOFT_FLOAT, ensure both arms are in registers. 
       Otherwise, ensure it is a valid FP add operand */
    if ((!TARGET_HARD_FLOAT)
        || (!fpu_add_operand (operands[3], SFmode)))
      operands[3] = force_reg (SFmode, operands[3]);

    ccreg = arm_gen_compare_reg (code, arm_compare_op0, arm_compare_op1);
    operands[1] = gen_rtx (code, VOIDmode, ccreg, const0_rtx);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SFmode,
	operand1,
	operand2,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movdfcc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  {
    enum rtx_code code = GET_CODE (operands[1]);
    rtx ccreg = arm_gen_compare_reg (code, arm_compare_op0, arm_compare_op1);

    operands[1] = gen_rtx (code, VOIDmode, ccreg, const0_rtx);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (DFmode,
	operand1,
	operand2,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_jump (operand0)
     rtx operand0;
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_LABEL_REF (VOIDmode,
	operand0));
}

rtx
gen_call (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  {
    rtx callee;
    
    /* In an untyped call, we can get NULL for operand 2.  */
    if (operands[2] == NULL_RTX)
      operands[2] = const0_rtx;
      
    /* This is to decide if we should generate indirect calls by loading the
       32 bit address of the callee into a register before performing the
       branch and link.  operand[2] encodes the long_call/short_call
       attribute of the function being called.  This attribute is set whenever
       __attribute__((long_call/short_call)) or #pragma long_call/no_long_call
       is used, and the short_call attribute can also be set if function is
       declared as static or if it has already been defined in the current
       compilation unit.  See arm.c and arm.h for info about this.  The third
       parameter to arm_is_longcall_p is used to tell it which pattern
       invoked it.  */
    callee  = XEXP (operands[0], 0);
    
    if (GET_CODE (callee) != REG
       && arm_is_longcall_p (operands[0], INTVAL (operands[2]), 0))
      XEXP (operands[0], 0) = force_reg (Pmode, callee);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	operand0,
	operand1),
		gen_rtx_USE (VOIDmode,
	operand2),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	14)))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_call_value (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  {
    rtx callee = XEXP (operands[1], 0);
    
    /* In an untyped call, we can get NULL for operand 2.  */
    if (operands[3] == 0)
      operands[3] = const0_rtx;
      
    /* See the comment in define_expand "call".  */
    if (GET_CODE (callee) != REG
	&& arm_is_longcall_p (operands[1], INTVAL (operands[3]), 0))
      XEXP (operands[1], 0) = force_reg (Pmode, callee);
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	operand1,
	operand2)),
		gen_rtx_USE (VOIDmode,
	operand3),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	14)))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sibcall (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  {
    if (operands[2] == NULL_RTX)
      operands[2] = const0_rtx;
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	operand0,
	operand1),
		gen_rtx_USE (VOIDmode,
	operand2),
		gen_rtx_USE (VOIDmode,
	gen_rtx_REG (SImode,
	14)))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sibcall_value (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;

  {
    if (operands[3] == NULL_RTX)
      operands[3] = const0_rtx;
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	operand1,
	operand2)),
		gen_rtx_USE (VOIDmode,
	operand3),
		gen_rtx_USE (VOIDmode,
	gen_rtx_REG (SImode,
	14)))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_untyped_call (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  {
    int i;

    emit_call_insn (GEN_CALL (operands[0], const0_rtx, NULL, const0_rtx));

    for (i = 0; i < XVECLEN (operands[2], 0); i++)
      {
	rtx set = XVECEXP (operands[2], 0, i);

	emit_move_insn (SET_DEST (set), SET_SRC (set));
      }

    /* The optimizer does not know that the call sets the function value
       registers we stored in the result block.  We avoid problems by
       claiming that all hard registers are used and clobbered at this
       point.  */
    emit_insn (gen_blockage ());

    DONE;
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	operand0,
	const0_rtx),
		operand1,
		operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_casesi (operand0, operand1, operand2, operand3, operand4)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
     rtx operand4;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[5];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
    operands[4] = operand4;

  {
    rtx reg;
    if (operands[1] != const0_rtx)
      {
	reg = gen_reg_rtx (SImode);

	emit_insn (gen_addsi3 (reg, operands[0],
			       GEN_INT (-INTVAL (operands[1]))));
	operands[0] = reg;
      }

    if (!const_ok_for_arm (INTVAL (operands[2])))
      operands[2] = force_reg (SImode, operands[2]);

    emit_jump_insn (gen_casesi_internal (operands[0], operands[2], operands[3],
					 operands[4]));
    DONE;
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
  }
  emit (operand0);
  emit (operand1);
  emit (operand2);
  emit (operand3);
  emit (operand4);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_indirect_jump (operand0)
     rtx operand0;
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0);
}

extern rtx gen_peephole2_493 PARAMS ((rtx, rtx *));
rtx
gen_peephole2_493 (curr_insn, operands)
     rtx curr_insn ATTRIBUTE_UNUSED;
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCmode,
	24),
	gen_rtx_COMPARE (CCmode,
	operand1,
	const0_rtx)),
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_500 PARAMS ((rtx *));
rtx
gen_split_500 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand5,
	gen_rtx_NOT (SImode,
	gen_rtx_ASHIFTRT (SImode,
	operand1,
	GEN_INT (31)))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx (GET_CODE (operand2), GET_MODE (operand2),
		operand3,
		operand4),
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_501 PARAMS ((rtx *));
rtx
gen_split_501 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  operands[1] = GEN_INT (((unsigned long) INTVAL (operands[1])) >> 24);
  
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ZERO_EXTEND (SImode,
	operand0)));
  emit_insn (gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCmode,
	24),
	gen_rtx_COMPARE (CCmode,
	operand2,
	operand1)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_prologue ()
{
  rtx _val = 0;
  start_sequence ();
  {
if (TARGET_ARM)
     arm_expand_prologue ();
   else
     thumb_expand_prologue ();
  DONE;
  
  }
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_epilogue ()
{
  rtx _val = 0;
  start_sequence ();
  {

  if (TARGET_THUMB)
    thumb_expand_epilogue ();
  else if (USE_RETURN_INSN (FALSE))
    {
      emit_jump_insn (gen_return ());
      DONE;
    }
  emit_jump_insn (gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		gen_rtx_RETURN (VOIDmode)),
	VUNSPEC_EPILOGUE));
  DONE;
  
  }
  emit_insn (gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		gen_rtx_RETURN (VOIDmode)),
	1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_eh_epilogue (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;

  {
    cfun->machine->eh_epilogue_sp_ofs = operands[1];
    if (GET_CODE (operands[2]) != REG || REGNO (operands[2]) != 2)
      {
	rtx ra = gen_rtx_REG (Pmode, 2);

	emit_move_insn (ra, operands[2]);
	operands[2] = ra;
      }
    /* This is a hack -- we may have crystalized the function type too
       early.  */
    cfun->machine->func_type = 0;
  }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_USE (VOIDmode,
	operand0));
  emit_insn (gen_rtx_USE (VOIDmode,
	operand1));
  emit_insn (gen_rtx_USE (VOIDmode,
	operand2));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_505 PARAMS ((rtx *));
rtx
gen_split_505 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx _val = 0;
  start_sequence ();

  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);
    enum rtx_code rc = GET_CODE (operands[1]);

    operands[5] = gen_rtx_REG (mode, CC_REGNUM);
    operands[6] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
    if (mode == CCFPmode || mode == CCFPEmode)
      rc = reverse_condition_maybe_unordered (rc);
    else
      rc = reverse_condition (rc);

    operands[7] = gen_rtx_fmt_ee (rc, VOIDmode, operands[5], const0_rtx);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand5,
	operand6));
  emit_insn (gen_rtx_COND_EXEC (VOIDmode,
	operand7,
	gen_rtx_SET (VOIDmode,
	operand0,
	operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_506 PARAMS ((rtx *));
rtx
gen_split_506 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();

  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);

    operands[5] = gen_rtx_REG (mode, CC_REGNUM);
    operands[6] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand5,
	operand6));
  emit_insn (gen_rtx_COND_EXEC (VOIDmode,
	gen_rtx (GET_CODE (operand1), GET_MODE (operand1),
		operand5,
		const0_rtx),
	gen_rtx_SET (VOIDmode,
	operand0,
	operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_507 PARAMS ((rtx *));
rtx
gen_split_507 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx operand8;
  rtx _val = 0;
  start_sequence ();

  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);
    enum rtx_code rc = GET_CODE (operands[1]);

    operands[6] = gen_rtx_REG (mode, CC_REGNUM);
    operands[7] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
    if (mode == CCFPmode || mode == CCFPEmode)
      rc = reverse_condition_maybe_unordered (rc);
    else
      rc = reverse_condition (rc);

    operands[8] = gen_rtx_fmt_ee (rc, VOIDmode, operands[6], const0_rtx);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  operand8 = operands[8];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	operand7));
  emit_insn (gen_rtx_COND_EXEC (VOIDmode,
	gen_rtx (GET_CODE (operand1), GET_MODE (operand1),
		operand6,
		const0_rtx),
	gen_rtx_SET (VOIDmode,
	operand0,
	operand4)));
  emit_insn (gen_rtx_COND_EXEC (VOIDmode,
	operand8,
	gen_rtx_SET (VOIDmode,
	operand0,
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

extern rtx gen_split_508 PARAMS ((rtx *));
rtx
gen_split_508 (operands)
      rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx operand8;
  rtx _val = 0;
  start_sequence ();

  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);
    enum rtx_code rc = GET_CODE (operands[1]);

    operands[6] = gen_rtx_REG (mode, CC_REGNUM);
    operands[7] = gen_rtx (COMPARE, mode, operands[2], operands[3]);
    if (mode == CCFPmode || mode == CCFPEmode)
      rc = reverse_condition_maybe_unordered (rc);
    else
      rc = reverse_condition (rc);

    operands[8] = gen_rtx_fmt_ee (rc, VOIDmode, operands[6], const0_rtx);
  }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  operand8 = operands[8];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	operand7));
  emit_insn (gen_rtx_COND_EXEC (VOIDmode,
	gen_rtx (GET_CODE (operand1), GET_MODE (operand1),
		operand6,
		const0_rtx),
	gen_rtx_SET (VOIDmode,
	operand0,
	operand4)));
  emit_insn (gen_rtx_COND_EXEC (VOIDmode,
	operand8,
	gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (SImode,
	operand5))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_ffssi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;

  {
    rtx t1, t2, t3;

    t1 = gen_reg_rtx (SImode);
    t2 = gen_reg_rtx (SImode);
    t3 = gen_reg_rtx (SImode);

    emit_insn (gen_negsi2 (t1, operands[1]));
    emit_insn (gen_andsi3 (t2, operands[1], t1));
    emit_insn (gen_clz (t3, t2));
    emit_insn (gen_subsi3 (operands[0], GEN_INT (32), t3));
    DONE;
  }
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FFS (SImode,
	operand1)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}



void
add_clobbers (pattern, insn_code_number)
     rtx pattern ATTRIBUTE_UNUSED;
     int insn_code_number;
{
  switch (insn_code_number)
    {
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
      XVECEXP (pattern, 0, 2) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	14));
      break;

    case 214:
      XVECEXP (pattern, 0, 4) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      XVECEXP (pattern, 0, 5) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    case 213:
      XVECEXP (pattern, 0, 5) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      XVECEXP (pattern, 0, 6) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      XVECEXP (pattern, 0, 7) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    case 320:
    case 279:
    case 276:
    case 273:
    case 270:
    case 165:
    case 147:
    case 124:
    case 121:
    case 100:
    case 93:
    case 83:
    case 55:
    case 54:
    case 49:
    case 46:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    case 318:
    case 316:
    case 314:
    case 312:
    case 310:
    case 308:
    case 306:
    case 304:
    case 302:
    case 300:
    case 298:
    case 296:
    case 294:
    case 292:
    case 291:
    case 290:
    case 285:
    case 284:
    case 282:
    case 134:
    case 133:
    case 126:
    case 125:
    case 114:
    case 113:
    case 112:
    case 111:
    case 110:
    case 109:
    case 32:
    case 31:
    case 30:
    case 29:
    case 28:
    case 27:
    case 26:
    case 3:
    case 2:
    case 1:
    case 0:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24));
      break;

    default:
      abort ();
    }
}


int
added_clobbers_hard_reg_p (insn_code_number)
     int insn_code_number;
{
  switch (insn_code_number)
    {
    case 214:
    case 213:
    case 320:
    case 279:
    case 276:
    case 273:
    case 270:
    case 165:
    case 147:
    case 124:
    case 121:
    case 100:
    case 93:
    case 83:
    case 55:
    case 54:
    case 49:
    case 46:
      return 0;

    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 318:
    case 316:
    case 314:
    case 312:
    case 310:
    case 308:
    case 306:
    case 304:
    case 302:
    case 300:
    case 298:
    case 296:
    case 294:
    case 292:
    case 291:
    case 290:
    case 285:
    case 284:
    case 282:
    case 134:
    case 133:
    case 126:
    case 125:
    case 114:
    case 113:
    case 112:
    case 111:
    case 110:
    case 109:
    case 32:
    case 31:
    case 30:
    case 29:
    case 28:
    case 27:
    case 26:
    case 3:
    case 2:
    case 1:
    case 0:
      return 1;

    default:
      abort ();
    }
}
