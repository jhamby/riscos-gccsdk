/*
 * mnemonics.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef mnemonics_header_included
#define mnemonics_header_included

#include "global.h"

void m_abs (WORD cc);
void m_acs (WORD cc);
void m_adc (WORD cc);
void m_add (WORD cc);
void m_adf (WORD cc);
void m_adr (WORD cc);
void m_and (WORD cc);
void m_asn (WORD cc);
void m_atn (WORD cc);
void m_bic (WORD cc);
void m_branch (WORD cc);
void m_cdp (WORD cc);
void m_cmn (WORD cc);
void m_cmf (WORD cc);
void m_cnf (WORD cc);
void m_cmp (WORD cc);
void m_cos (WORD cc);
void m_dvf (WORD cc);
void m_eor (WORD cc);
void m_exp (WORD cc);
void m_fdv (WORD cc);
void m_fix (WORD cc);
void m_flt (WORD cc);
void m_fml (WORD cc);
void m_frd (WORD cc);
void m_ldc (WORD cc);
void m_ldf (WORD cc);
void m_ldm (WORD cc);
void m_ldr (WORD cc);
void m_lfm (WORD cc);
void m_lgn (WORD cc);
void m_log (WORD cc);
void m_mcr (WORD cc);
void m_mla (WORD cc);
void m_mnf (WORD cc);
void m_mov (WORD cc);
void m_mrc (WORD cc);
void m_mrs (WORD cc);
void m_msr (WORD cc);
void m_muf (WORD cc);
void m_mul (WORD cc);
void m_mvf (WORD cc);
void m_mvn (WORD cc);
void m_orr (WORD cc);
void m_pld (void);
void m_pol (WORD cc);
void m_pow (WORD cc);
void m_rdf (WORD cc);
void m_rfs (WORD cc);
void m_rfc (WORD cc);
void m_rmf (WORD cc);
void m_rnd (WORD cc);
void m_rpw (WORD cc);
void m_rsb (WORD cc);
void m_rsc (WORD cc);
void m_rsf (WORD cc);
void m_sbc (WORD cc);
void m_sfm (WORD cc);
void m_sin (WORD cc);
void m_smull (WORD cc);
void m_smlal (WORD cc);
void m_sqt (WORD cc);
void m_stc (WORD cc);
void m_stf (WORD cc);
void m_stm (WORD cc);
void m_str (WORD cc);
void m_sub (WORD cc);
void m_suf (WORD cc);
void m_swi (WORD cc);
void m_swp (WORD cc);
void m_tan (WORD cc);
void m_teq (WORD cc);
void m_tst (WORD cc);
void m_umull (WORD cc);
void m_umlal (WORD cc);
void m_urd (WORD cc);
void m_nrm (WORD cc);
void m_wfs (WORD cc);
void m_wfc (WORD cc);

void m_nop (void);
void m_ret (WORD cc);
void m_stack (void);
void m_tail (WORD cc);

#endif
