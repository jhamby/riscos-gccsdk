/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2010 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * mnemonics.h
 */

#ifndef mnemonics_header_included
#define mnemonics_header_included

#include "global.h"

void m_abs (ARMWord cc);
void m_acs (ARMWord cc);
void m_adc (ARMWord cc);
void m_add (ARMWord cc);
void m_adf (ARMWord cc);
void m_adr (ARMWord cc);
void m_and (ARMWord cc);
void m_asn (ARMWord cc);
void m_atn (ARMWord cc);
void m_bic (ARMWord cc);
void m_bkpt (void);
void m_blx (ARMWord cc);
void m_branch (ARMWord cc);
void m_bx  (ARMWord cc);
void m_cdp (ARMWord cc);
void m_cdp2 (void);
void m_clz (ARMWord cc);
void m_cmn (ARMWord cc);
void m_cmf (ARMWord cc);
void m_cnf (ARMWord cc);
void m_cmp (ARMWord cc);
void m_cos (ARMWord cc);
void m_dvf (ARMWord cc);
void m_eor (ARMWord cc);
void m_exp (ARMWord cc);
void m_fdv (ARMWord cc);
void m_fix (ARMWord cc);
void m_flt (ARMWord cc);
void m_fml (ARMWord cc);
void m_frd (ARMWord cc);
void m_ldc (ARMWord cc);
void m_ldc2 (ARMWord cc);
void m_ldf (ARMWord cc);
void m_ldm (ARMWord cc);
void m_ldr (ARMWord cc);
void m_lfm (ARMWord cc);
void m_lgn (ARMWord cc);
void m_log (ARMWord cc);
void m_mcr (ARMWord cc);
void m_mcr2 (void);
void m_mcrr (ARMWord cc);
void m_mla (ARMWord cc);
void m_mnf (ARMWord cc);
void m_mov (ARMWord cc);
void m_mrc (ARMWord cc);
void m_mrc2 (void);
void m_mrrc (ARMWord cc);
void m_mrs (ARMWord cc);
void m_msr (ARMWord cc);
void m_muf (ARMWord cc);
void m_mul (ARMWord cc);
void m_mvf (ARMWord cc);
void m_mvn (ARMWord cc);
void m_orr (ARMWord cc);
void m_pld (void);
void m_pol (ARMWord cc);
void m_pow (ARMWord cc);
void m_qadd (ARMWord cc);
void m_qdadd (ARMWord cc);
void m_qdsub (ARMWord cc);
void m_qsub (ARMWord cc);
void m_rdf (ARMWord cc);
void m_rfs (ARMWord cc);
void m_rfc (ARMWord cc);
void m_rmf (ARMWord cc);
void m_rnd (ARMWord cc);
void m_rpw (ARMWord cc);
void m_rsb (ARMWord cc);
void m_rsc (ARMWord cc);
void m_rsf (ARMWord cc);
void m_sbc (ARMWord cc);
void m_sfm (ARMWord cc);
void m_sin (ARMWord cc);
void m_smlabb (ARMWord cc);
void m_smlabt (ARMWord cc);
void m_smlatb (ARMWord cc);
void m_smlatt (ARMWord cc);
void m_smlalbb (ARMWord cc);
void m_smlalbt (ARMWord cc);
void m_smlaltb (ARMWord cc);
void m_smlaltt (ARMWord cc);
void m_smlawb (ARMWord cc);
void m_smlawt (ARMWord cc);
void m_smulbb (ARMWord cc);
void m_smulbt (ARMWord cc);
void m_smultb (ARMWord cc);
void m_smultt (ARMWord cc);
void m_smulwb (ARMWord cc);
void m_smulwt (ARMWord cc);
void m_smull (ARMWord cc);
void m_smlal (ARMWord cc);
void m_sqt (ARMWord cc);
void m_stc (ARMWord cc);
void m_stc2 (ARMWord cc);
void m_stf (ARMWord cc);
void m_stm (ARMWord cc);
void m_str (ARMWord cc);
void m_sub (ARMWord cc);
void m_suf (ARMWord cc);
void m_swi (ARMWord cc);
void m_swp (ARMWord cc);
void m_tan (ARMWord cc);
void m_teq (ARMWord cc);
void m_tst (ARMWord cc);
void m_umull (ARMWord cc);
void m_umlal (ARMWord cc);
void m_urd (ARMWord cc);
void m_nrm (ARMWord cc);
void m_wfs (ARMWord cc);
void m_wfc (ARMWord cc);

void m_nop (void);
void m_ret (ARMWord cc);
void m_stack (void);
void m_tail (ARMWord cc);

#endif
