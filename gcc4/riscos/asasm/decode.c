/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2011 GCCSDK Developers
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
 * decode.c
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <stdio.h>
#include <string.h>

#include "area.h"
#include "asm.h"
#include "code.h"
#include "commands.h"
#include "decode.h"
#include "error.h"
#include "frame.h"
#include "input.h"
#include "local.h"
#include "macros.h"
#include "m_cop.h"
#include "m_copmem.h"
#include "m_cpu.h"
#include "m_cpuctrl.h"
#include "m_cpumem.h"
#include "m_fpe.h"
#include "option.h"
#include "storage.h"

typedef bool (*po_void)(void); /* For eCB_Void and eCB_NoLex.  */
typedef bool (*po_void_lc)(bool doLowerCase); /* For eCB_VoidPMatch and eCB_NoLexPMatch.  */
typedef bool (*po_lex)(const Lex *labelP); /* For eCB_Lex.  */
typedef bool (*po_lex_lc)(const Lex *labelP, bool doLowerCase); /* For eCB_LexPMatch.  */
typedef bool (*po_sym)(Symbol *symbolP); /* For eCB_Symbol.  */

/* Determine the kind of callback to do wrt the LexId @ column 0.  */
enum
{
  eCB_Void        = 0, /* If there is a valid LexId/LexLocalLabel @ column 0, define this as symbol.  This is then a label.  */
  eCB_VoidPMatch  = 1, /* Same as eCB_Void + further mnemonic decoding needs to be done by parse_opcode callback.  */
  eCB_NoLex       = 2, /* If there is a valid LexId/LexLocalLabel @ column 0, complain about it as it is not allowed.  */
  eCB_NoLexPMatch = 3, /* Same as eCB_NoLex + further mnemonic decoding needs to be done by parse_opcode callback.  */
  eCB_Lex         = 4, /* If there is a valid LexId @ column 0, pass it on as parameter.  If it is made a symbol afterwards by the callback, assign code size to it.  */
  eCB_LexPMatch   = 5, /* Save as eCB_Lex + further mnemonic decoding needs to be done by parse_opcode callback.  */
  eCB_Symbol      = 6  /* If there is a valid LexId @ column 0, turn this into a symbol and pass it on as parameter.  */
};
/* Determines the kind of output the mnemonic is going to do.  */
enum
{
  eRslt_ARM   = 0, /* ARM instruction.  */
  eRslt_Data  = 1, /* Data.  */
  eRslt_Thumb = 2, /* Thumb instruction.  */
  eRslt_None  = 3  /* No data nor instruction, or no change.  */
};
typedef struct
{
  const char *mnemonic; /* Ptr to mnemonic (or first part of mnemonic).  */
  unsigned int cb_type:3; /* See eCB_* values.  */
  unsigned int result:2; /* Kind of output (see eRslt_* enum).  */
  union
    {
      po_void vd; /* Callback for eCB_Void.  */
      po_void_lc vdpm; /* Callback for eCB_VoidPMatch.  */
      po_void nolex; /* Callback for eCB_NoLex.  */
      po_void_lc nolexpm; /* Callback for eCB_NoLexPMatch.  */
      po_lex lex; /* Callback for eCB_Lex.  */
      po_lex_lc lexpm; /* Callback for eCB_LexPMatch.  */
      po_sym sym; /* Callback for eCB_Symbol.  */
    } parse_opcode;
} decode_table_t;

/* This table is alphabetically ordered.  */
static const decode_table_t oDecodeTable[] =
{
  { "!", eCB_Void, eRslt_None, { .vd = c_info } }, /* INFO shorthand */
  { "#", eCB_Lex, eRslt_None, { .lex = c_alloc } }, /* # / FIELD : reserve space in the current record.  */
  { "%", eCB_Void, eRslt_Data, { .vd = c_reserve } }, /* % / SPACE : reserve space.  */
  { "&", eCB_Void, eRslt_Data, { .vd = c_ampersand } }, /* & */
  { "*", eCB_Symbol, eRslt_None, { .sym = c_equ } }, /* * / EQU */
  { "=", eCB_Void, eRslt_Data, { .vd = c_dcb } }, /* = / DCB */
  { "ABS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_abs } }, /* ABS CC P R */
  { "ACS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_acs } }, /* ACS CC P R */
  { "ADC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_adc } }, /* ADC CC S */
  { "ADD", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_add } }, /* ADD CC S */
  { "ADF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_adf } }, /* ADF CC P R */
  { "ADR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_adr } }, /* ADR CC */
  { "ALIGN", eCB_Void, eRslt_Data, { .vd = c_align } }, /* ALIGN */
  { "AND", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_and } }, /* AND CC S */
  { "AREA", eCB_Void, eRslt_None, { .vd = c_area } }, /* AREA */
  { "ARM", eCB_Void, eRslt_None, { .vd = c_code32 } }, /* ARM/CODE32 */
  { "ASN", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_asn } }, /* ASN CC P R */
  { "ASR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_asr } }, /* ASR S CC */
  { "ASSERT", eCB_Void, eRslt_None, { .vd = c_assert } }, /* ASSERT */
  { "ATN", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_atn } }, /* ATN CC P R */
  { "B", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_branch } }, /* B [L] CC */
  /* FIXME: BFC */
  /* FIXME: BFI */
  { "BIC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_bic } }, /* BIC CC S */
  { "BIN", eCB_Void, eRslt_None, { .vd = c_incbin } }, /* BIN / INCBIN */
  { "BKPT", eCB_Void, eRslt_ARM, { .vd = m_bkpt } }, /* BKPT */
  { "BLX", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_blx } }, /* BLX CC */
  { "BX", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_bx } }, /* BX CC */
  { "BXJ", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_bxj } }, /* BXJ CC */
  { "CDP", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_cdp } }, /* CDP CC */
  { "CDP2", eCB_Void, eRslt_ARM, { .vd = m_cdp2 } }, /* CDP2 */
  { "CLREX", eCB_Void, eRslt_ARM, { .vd = m_clrex } }, /* CLREX */
  { "CLZ", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_clz } }, /* CLZ CC */
  { "CMF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_cmf } }, /* CMF CC or CMFE CC */
  { "CMN", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_cmn } }, /* CMN CC SP */
  { "CMP", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_cmp } }, /* CMP CC SP */
  { "CN", eCB_Symbol, eRslt_None, { .sym = c_cn } }, /* CN */
  { "CNF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_cnf } }, /* CNF CC or CNFE CC */
  { "CODE16", eCB_Void, eRslt_None, { .vd = c_code16 } }, /* CODE16 */
  { "CODE32", eCB_Void, eRslt_None, { .vd = c_code32 } }, /* ARM/CODE32 */
  { "COS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_cos } }, /* COS CC P R */
  { "CP", eCB_Symbol, eRslt_None, { .sym = c_cp } }, /* CP */
  { "CPS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_cps } }, /* CPS */
  { "DBG", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_dbg } }, /* DBG */
  { "DCB", eCB_Void, eRslt_Data, { .vd = c_dcb } }, /* = / DCB */
  { "DCD", eCB_VoidPMatch, eRslt_Data, { .vdpm = c_dcd } }, /* DCD / DCDU */
  { "DCFD", eCB_VoidPMatch, eRslt_Data, { .vdpm = c_dcfd } }, /* DCFD / DCFDU */
  { "DCFS", eCB_VoidPMatch, eRslt_Data, { .vdpm = c_dcfs } }, /* DCFS / DCFSU */
  { "DCI", eCB_Void, eRslt_ARM, { .vd = c_dci } }, /* DCI */
  { "DCW", eCB_VoidPMatch, eRslt_Data, { .vdpm = c_dcw } }, /* DCW / DCWU */
  { "DMB", eCB_Void, eRslt_ARM, { .vd = m_dmb } }, /* DMB */
  { "DSB", eCB_Void, eRslt_ARM, { .vd = m_dsb } }, /* DSB */
  { "DVF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_dvf } }, /* DVF CC P R */
  { "ELIF", eCB_NoLex, eRslt_None, { .nolex = c_elif } }, /* ELIF */
  { "ELSE", eCB_NoLex, eRslt_None, { .nolex = c_else } }, /* | ELSE */
  { "END", eCB_NoLex, eRslt_None, { .vd = c_end } }, /* END */
  { "ENDFUNC", eCB_Void, eRslt_None, { .vd = c_endfunc } }, /* ENDFUNC / ENDP */
  { "ENDIF", eCB_NoLex, eRslt_None, { .nolex = c_endif } }, /* ] ENDIF */
  { "ENDP", eCB_Void, eRslt_None, { .vd = c_endfunc } }, /* ENDFUNC / ENDP */
  { "ENTRY", eCB_Void, eRslt_None, { .vd = c_entry } }, /* ENTRY */
  { "EOR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_eor } }, /* EOR CC S */
  { "EQU", eCB_Symbol, eRslt_None, { .sym = c_equ } }, /* * / EQU */
  { "EXP", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_exp } }, /* EXP CC P R */
  { "EXPORT", eCB_Void, eRslt_None, { .vd = c_export } }, /* EXPORT / GLOBAL */
  { "EXTERN", eCB_Void, eRslt_None, { .vd = c_import } }, /* IMPORT / EXTERN */
  { "FDV", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_fdv } }, /* FDV CC P R */
  { "FIELD", eCB_Lex, eRslt_ARM, { .lex = c_alloc } }, /* # / FIELD : reserve space in the current record.  */
  { "FIX", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_fix } }, /* FIX CC [P] R */
  { "FLT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_flt } }, /* FLT CC P R */
  { "FML", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_fml } }, /* FML CC P R */
  { "FN", eCB_Symbol, eRslt_None, { .sym = c_fn } }, /* FN */
  { "FRAME", eCB_Void, eRslt_None, { .vd = c_frame } }, /* FRAME */
  { "FRD", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_frd } }, /* FRD CC P R */
  { "FUNCTION", eCB_Void, eRslt_None, { .vd = c_function } }, /* FUNCTION / PROC */
  { "GBL", eCB_NoLexPMatch, eRslt_None, { .nolexpm = c_gbl } }, /* GBLA, GBLL, GBLS */
  { "GET", eCB_Void, eRslt_None, { .vd = c_get } }, /* GET */
  { "GLOBAL", eCB_Void, eRslt_None, { .vd = c_export } }, /* EXPORT / GLOBAL */
  { "HEAD", eCB_Void, eRslt_Data, { .vd = c_head } }, /* HEAD */
  { "IDFN", eCB_Void, eRslt_None, { .vd = c_idfn } }, /* IDFN */
  { "IF", eCB_NoLex, eRslt_None, { .nolex = c_if } }, /* [ IF */
  { "IMPORT", eCB_Void, eRslt_None, { .vd = c_import } }, /* IMPORT / EXTERN */
  { "INCBIN", eCB_Void, eRslt_None, { .vd = c_incbin } }, /* BIN / INCBIN */
  { "INCLUDE", eCB_Void, eRslt_None, { .vd = c_get } }, /* GET / INCLUDE */
  { "INFO", eCB_Void, eRslt_None, { .vd = c_info } }, /* INFO */
  { "ISB", eCB_Void, eRslt_ARM, { .vd = m_isb } }, /* ISB */
  { "KEEP", eCB_Void, eRslt_None, { .vd = c_keep } }, /* KEEP */
  { "LCL", eCB_NoLexPMatch, eRslt_None, { .nolexpm = c_lcl } }, /* LCLA, LCLL, LCLS */
  { "LDC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_ldc } }, /* LDC CC L */
  { "LDC2", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_ldc2 } }, /* LDC2 L */
  { "LDF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_ldf } }, /* LDF CC P */
  { "LDM", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_ldm } }, /* LDM CC TYPE */
  { "LDR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_ldr } }, /* LDR CC BYTE */
  { "LDREX", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_ldrex } }, /* LDREX / LDREXB / LDREXD / LDREXH */
  { "LFM", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_lfm } }, /* LFM CC (TYPE) */
  { "LGN", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_lgn } }, /* LGN CC P R */
  { "LNK", eCB_Void, eRslt_None, { .vd = c_lnk } }, /* LNK */
  { "LOG", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_log } }, /* LOG CC P R */
  { "LSL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_lsl } }, /* LSL S CC */
  { "LSR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_lsr } }, /* LSR S CC */
  { "LTORG", eCB_Void, eRslt_None, { .vd = c_ltorg } }, /* LTORG */
  { "MACRO", eCB_NoLex, eRslt_None, { .nolex = c_macro } }, /* MACRO */
  { "MAP", eCB_NoLex, eRslt_None, { .nolex = c_record } }, /* ^ / MAP : start of new record layout.  */
  { "MCR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mcr } }, /* MCR CC */
  { "MCR2", eCB_Void, eRslt_ARM, { .vd = m_mcr2 } }, /* MCR2 */
  { "MCRR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mcrr } }, /* MCRR CC */
  { "MEND", eCB_NoLex, eRslt_None, { .nolex = c_mend } }, /* MEND */
  { "MEXIT", eCB_NoLex, eRslt_None, { .nolex = c_mexit } }, /* MEXIT */
  { "MLA", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mla } }, /* MLA CC S */
  { "MLS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mls } }, /* MLS CC */
  { "MNF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mnf } }, /* MNF CC P R */
  { "MOV", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mov } }, /* MOV CC s */
  { "MRC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mrc } }, /* MRC CC */
  { "MRC2", eCB_Void, eRslt_ARM, { .vd = m_mrc2 } }, /* MRC2 */
  { "MRRC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mrrc } }, /* MRRC CC */
  { "MRS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mrs } }, /* MRS CC */
  { "MSR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_msr } }, /* MSR CC */
  { "MUF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_muf } }, /* MUF CC P R */
  { "MUL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mul } }, /* MUL CC S */
  { "MVF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mvf } }, /* MVF CC P R */
  { "MVN", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_mvn } }, /* MVN CC S */
  { "NOFP", eCB_Void, eRslt_ARM, { .vd = c_nofp } }, /* NOFP */
  { "NOP", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_nop } }, /* NOP [CC] */
  { "NRM", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_nrm } }, /* NRM CC P R */
  { "OPT", eCB_Void, eRslt_None, { .vd = c_opt } }, /* OPT */
  { "ORG", eCB_Void, eRslt_None, { .vd = c_org } }, /* ORG */
  { "ORR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_orr } }, /* ORR CC S */
  /* FIXME: PKH */
  { "PL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_pl } }, /* PLD, PLDW, PLI */
  { "POL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_pol } }, /* POL CC P R */
  { "POP", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_pop } }, /* POP CC */
  { "POW", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_pow } }, /* POW CC P R */
  { "PRESERVE8", eCB_Void, eRslt_None, { .vd = c_preserve8 } }, /* PRESERVE8 {TRUE}/{FALSE} */
  { "PROC", eCB_Void, eRslt_None, { .vd = c_function } }, /* FUNCTION / PROC */
  { "PUSH", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_push } }, /* PUSH CC */
  { "QADD", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_qadd } }, /* QADD CC */
  /* FIXME: QASX */
  { "QDADD", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_qdadd } }, /* QDADD CC */
  { "QDSUB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_qdsub } }, /* QDSUB CC */
  /* FIXME: QSAX */
  { "QSUB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_qsub } }, /* QSUB CC */
  /* FIXME: RBIT */
  /* FIXME: REV, REV16, REVSH */
  { "RDF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rdf } }, /* RDF CC P R */
  { "REQUIRE8", eCB_Void, eRslt_ARM, { .vd = c_require8 } }, /* REQUIRE8 {TRUE}/{FALSE} */
  { "RET", eCB_VoidPMatch, eRslt_None, { .vdpm = m_ret } }, /* RET CC */
  { "RFC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rfc } }, /* RFC CC */
  { "RFE", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rfe } }, /* RFE MODE */
  { "RFS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rfs } }, /* RFS CC */
  /* FIXME: RLIST */
  { "RMF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rmf } }, /* RMF CC P R */
  { "RN", eCB_Symbol, eRslt_None, { .sym = c_rn } }, /* RN */
  { "RND", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rnd } }, /* RND CC P R */
  { "ROR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_ror } }, /* ROR S CC */
  { "ROUT", eCB_Lex, eRslt_None, { .lex = c_rout } }, /* ROUT */
  { "RPW", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rpw } }, /* RPW CC P R */
  { "RRX", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rrx } }, /* RRX S CC */
  { "RSB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rsb } }, /* RSB CC S */
  { "RSC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rsc } }, /* RSC CC S */
  { "RSF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_rsf } }, /* RSF CC P R */
  /* FIXME: SADD16/SADD8 */
  /* FIXME: SASX */
  { "SBC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_sbc } }, /* SBC CC S */
  /* FIXME: SBFX */
  /* FIXME: SEL */
  { "SET", eCB_LexPMatch, eRslt_None, { .lexpm = c_set } }, /* SETA, SETL, SETS */
  /* FIXME: SETEND */
  { "SEV", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_sev } }, /* SEV CC */
  { "SFM", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_sfm } }, /* SFM CC (TYPE) */
  /* FIXME: SHADD16/SHADD8 */
  /* FIXME: SHASX/SHSAX */
  /* FIXME: SHSUB16/SHSUB8 */
  { "SIN", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_sin } }, /* SIN CC P R */
  { "SMC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smc } }, /* SMC CC */
  { "SMI", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smc } }, /* SMI CC (pre-UAL) */
  { "SMLABB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlabb } }, /* SMLABB CC */
  { "SMLABT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlabt } }, /* SMLABT CC */
  /* FIXME: SMLAD */
  { "SMLAL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlal } }, /* SMLAL CC S */
  { "SMLALBB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlalbb } }, /* SMLALBB CC */
  { "SMLALBT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlalbt } }, /* SMLALBT CC */
  { "SMLALTB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlaltb } }, /* SMLALTB CC */
  { "SMLALTT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlaltt } }, /* SMLALTT CC */
  /* FIXME: SMLAD */
  { "SMLATB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlatb } }, /* SMLATB CC */
  { "SMLATT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlatt } }, /* SMLATT CC */
  { "SMLAWB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlawb } }, /* SMLAWB CC */
  { "SMLAWT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smlawt } }, /* SMLAWT CC */
  /* FIXME: SMLSD, SMLSLD */
  /* FIXME: SMMLA, SMMLS, SMMUL, SMUAD */
  { "SMULBB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smulbb } }, /* SMULBB CC */
  { "SMULBT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smulbt } }, /* SMULBT CC */
  { "SMULL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smull } }, /* SMULL CC */
  { "SMULTB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smultb } }, /* SMULTB CC */
  { "SMULTT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smultt } }, /* SMULTT CC */
  { "SMULWB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smulwb } }, /* SMULWB CC */
  { "SMULWT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_smulwt } }, /* SMULWT CC */
  /* FIXME: SMUSD */
  { "SPACE", eCB_Void, eRslt_None, { .vd = c_reserve } }, /* % / SPACE : reserve space.  */
  { "SQT", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_sqt } }, /* SQT CC P R */
  { "SRS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_srs } }, /* SRS MODE */
  /* FIXME: SSAT, SSAT16 */
  /* FIXME: SSAX */
  /* FIXME: SSUB16, SSUB8 */
  { "STACK", eCB_Void, eRslt_ARM, { .vd = m_stack } }, /* STACK */
  { "STC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_stc } }, /* STC CC l */
  { "STC2", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_stc2 } }, /* STC2 CC l */
  { "STF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_stf } }, /* STF CC P */
  { "STM", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_stm } }, /* STM CC TYPE */
  { "STR", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_str } }, /* STR CC BYTE */
  { "STREX", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_strex } }, /* STREX / STREXB / STREXD / STREXH */
  { "STRONG", eCB_Void, eRslt_None, { .vd = c_strong } }, /* STRONG */
  { "SUB", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_sub } }, /* SUB CC S */
  { "SUBT", eCB_Void, eRslt_None, { .vd = c_title } }, /* SUBT */
  { "SUF", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_suf } }, /* SUF CC P R */
  { "SVC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_swi } }, /* SVC CC */
  { "SWI", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_swi } }, /* SWI CC */
  { "SWP", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_swp } }, /* SWP CC B */
  /* FIXME: SXTAB, SXTAB16, SXTAH, SXTB, SXTB16, SXTH */
  { "TAIL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_tail } }, /* TAIL CC */
  { "TAN", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_tan } }, /* TAN CC P R */
  { "TEQ", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_teq } }, /* TEQ CC */
  { "THUMB", eCB_Void, eRslt_None, { .vd = c_thumb } }, /* THUMB */
  { "THUMBX", eCB_Void, eRslt_None, { .vd = c_thumbx } }, /* THUMBX */
  { "TST", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_tst } }, /* TST CC */
  { "TTL", eCB_Void, eRslt_None, { .vd = c_title } }, /* TTL */
  /* FIXME: UADD16, UADD8 */
  /* FIXME: UASX */
  /* FIXME: UBFX */
  /* FIXME: UHADD16, UHADD8 */
  /* FIXME: UHASX, UHSA */
  /* FIXME: UHSUB16, UHSUB8 */
  /* FIXME: UMAAL */
  { "UMLAL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_umlal } }, /* UMLAL CC */
  { "UMULL", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_umull } }, /* UMULL CC */
  /* FIXME: UQADD16, UQADD8 */
  /* FIXME: UQASX, UQSAX */
  /* FIXME: UQSUB16, UQSUB8 */
  /* FIXME: USAD8, USADA8 */
  /* FIXME: USAT, USAT16 */
  /* FIXME: USAX */
  /* FIXME: USUB16, USUB8 */
  /* FIXME: UXTAB, UXTAB16, UXTAH, UXTAB, UXTB16, UXTH */
  { "URD", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_urd } }, /* URD CC P R */
  /* FIXME: V* */
  { "WEND", eCB_NoLex, eRslt_None, { .nolex = c_wend } }, /* WEND */
  { "WFC", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_wfc } }, /* WFC CC */
  { "WFE", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_wfe } }, /* WFE CC */
  { "WFI", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_wfi } }, /* WFI CC */
  { "WFS", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_wfs } }, /* WFS CC */
  { "WHILE", eCB_NoLex, eRslt_None, { .nolex = c_while } }, /* WHILE */
  { "YIELD", eCB_VoidPMatch, eRslt_ARM, { .vdpm = m_yield } }, /* YIELD CC */
  { "[", eCB_NoLex, eRslt_None, { .nolex = c_if } }, /* [ IF */
  { "]", eCB_NoLex, eRslt_None, { .nolex = c_endif } }, /* ] ENDIF */
  { "^", eCB_NoLex, eRslt_None, { .nolex = c_record } }, /* ^ / MAP : start of new record layout.  */
  { "|", eCB_NoLex, eRslt_None, { .nolex = c_else } }, /* | ELSE */
};
#define DECODE_ENTRIES (sizeof (oDecodeTable) / sizeof (oDecodeTable[0]))

/**
 * Only ARM and Thumb instructions can be written with lower case mnemonics.
 */
static inline bool
LowerCaseIsOK (const decode_table_t *entry)
{
  return entry->result == eRslt_ARM || entry->result == eRslt_Thumb;
}

static inline unsigned char
ResolveLowerCase (unsigned char c, bool doLowerCase)
{
  return doLowerCase && c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c;
}

static bool
MatchRestOfKeyword (const char *l, const char *r, size_t size, bool doLowerCase)
{
  while (size--)
    {
      if ((unsigned char)*l++ != ResolveLowerCase ((unsigned char)*r++, doLowerCase))
	return false;
    }
  return true;
}

static inline bool
IsPartiallyMatched (const decode_table_t *decP)
{
  return decP->cb_type == eCB_VoidPMatch || decP->cb_type == eCB_LexPMatch || decP->cb_type == eCB_NoLexPMatch;
}

/**
 * Decode one assembler line.
 * \param label A non-NULL ptr to the starting label.  Its tag is LexId when
 * there was one found, it's LexNone when there wasn't one.
 */
void
decode (const Lex *label)
{
#if 0
  /* Check that all entries in oDecodeTable are sorted.  */
  for (size_t i = 1; i < DECODE_ENTRIES; ++i)
    assert (strcmp (oDecodeTable[i - 1].mnemonic, oDecodeTable[i].mnemonic) < 0);
#endif
  assert (label->tag == LexNone || label->tag == LexId || label->tag == LexLocalLabel);

  /* Deal with empty line quickly.  */
  if (Input_IsEolOrCommentStart ())
    {
      (void) ASM_DefineLabel (label, areaCurrentSymbol->area.info->curIdx);
      return;
    }

  const char * const inputMark = Input_GetMark ();

  const bool doLowerCase = inputLook () >= 'a' && inputLook () <= 'z'; 
  
  /* Locate mnemonic entry in decode table.  */
  size_t low = 0;
  size_t high = DECODE_ENTRIES - 1;
  size_t indexFound = SIZE_MAX;
  for (size_t charsMatched = 0; /* */; ++charsMatched)
    {
      unsigned char c = (unsigned char)inputGet ();
      assert (c != 0);
      while (low <= high)
	{
	  size_t mid = low + (high - low) / 2;
	  const unsigned char m = ResolveLowerCase ((unsigned char)oDecodeTable[mid].mnemonic[charsMatched],
	                                            doLowerCase);
	  if (m < c)
	    {
	      /* At 'mid' position, the first charsMatches + 1 chars are less
	         then what we're looking for.  */
	      low = mid + 1;
	    }
	  else if (m > c)
	    {
	      /* At 'mid' position, the first charsMatches + 1 chars are bigger
	         then what we're looking for.  */
	      high = mid - 1;
	    }
	  else
	    {
	      /* At 'mid' position, the first charsMatches + 1 chars are
		 equal.  Determine the range around 'mid' position where this
	         is true.  */
	      size_t lowNew = mid;
	      while (lowNew > low
	             && ResolveLowerCase ((unsigned char)oDecodeTable[lowNew - 1].mnemonic[charsMatched],
	                                  doLowerCase) == c)
		--lowNew;
	      low = lowNew;
	      size_t highNew = mid;
	      while (highNew < high
	             && ResolveLowerCase ((unsigned char)oDecodeTable[highNew + 1].mnemonic[charsMatched],
	                                  doLowerCase) == c)
		++highNew;
	      high = highNew;
	      break;
	    }
	}
      if (low > high)
	break; /* No match for sure.  */
      /* From index low to high (incl), we have up to charsMatched + 1 matching
	 characters but that's possibly not a full match.  Make up our mind
	 whether we should continue to search in our table, whether we don't
	 have a solution for sure, whether we have a match for sure.  */
      if (oDecodeTable[low].mnemonic[charsMatched + 1] == '\0'
          && IsPartiallyMatched (&oDecodeTable[low]))
	{
	  /* For sure we have a match, but try to match more mnemonic
	     characters.  E.g. input "SUBT" with charMatched 2 is matching
	     "SUB*" and "SUBT".  First try "SUBT" and then fall back on the
	     'wildcard' (IsPartiallyMatched() being true).  */
	  while (!MatchRestOfKeyword (Input_GetMark (),
				      &oDecodeTable[high].mnemonic[charsMatched + 1],
				      strlen (&oDecodeTable[high].mnemonic[charsMatched + 1]),
	                              doLowerCase))
	    --high;
	  assert (low <= high);
	  /* charsMatched += strlen (&oDecodeTable[high].mnemonic[charsMatched + 1]); */
	  inputSkipN (strlen (&oDecodeTable[high].mnemonic[charsMatched + 1]));
	  /* We only have a full match when the full mnemonic is read (and
	     should have been fully read).  */
	  if (IsPartiallyMatched (&oDecodeTable[high]) || Input_IsEndOfKeyword ())
	    indexFound = high;
	}
      else
	{
	  if (Input_IsEndOfKeyword ())
	    {
	      if (oDecodeTable[low].mnemonic[charsMatched + 1] == '\0')
		{
		  /* We have a full match.  E.g. we have "END" for low -> "END"
		     and high -> "ENDIF".  */
		  indexFound = low;
		}
	      else
		{
		  /* No match.  E.g. we have "INC" for low -> "INCBIN" and
		     high -> "INCLUDE".  */
		}
	    }
	  else if (oDecodeTable[high].mnemonic[charsMatched + 1] != '\0')
	    {
	      continue;
	    }
	  else
	    {
	      /* No match.  E.g. we have processed "FIX" of "FIXIT" and low
	         and high -> "FIX".  */
	      assert (low == high);
	    }
	}
      break;
    }

  /* Check if a lowercase mnemonic is ok.  */
  if (indexFound != SIZE_MAX
      && doLowerCase && !LowerCaseIsOK (&oDecodeTable[indexFound]))
    indexFound = SIZE_MAX;

  bool tryAsMacro;
  if (indexFound != SIZE_MAX)
    {
      assert (Input_IsEndOfKeyword () || IsPartiallyMatched (&oDecodeTable[indexFound]));
      if (!IsPartiallyMatched (&oDecodeTable[indexFound]))
        skipblanks ();

      /* Mark that this mnemonic will result in data, ARM or Thumb output.  */
      if (oDecodeTable[indexFound].result != eRslt_None)
	Area_MarkStartAs (oDecodeTable[indexFound].result == eRslt_ARM ? eARM
			  : oDecodeTable[indexFound].result == eRslt_Data ? eData
			  : eThumb);

      int startOffset = Area_IsImplicit (areaCurrentSymbol) ? 0 : areaCurrentSymbol->area.info->curIdx;
      const Symbol * const startAreaSymbol = areaCurrentSymbol;
      Value startStorage =
	{
	  .Tag = ValueIllegal
	};
      Value_Assign (&startStorage, storageValue ());

      Symbol *labelSymbol;
      switch (oDecodeTable[indexFound].cb_type)
	{
	  case eCB_Void:
	  case eCB_VoidPMatch:
	    {
	      if (label->tag == LexLocalLabel)
		labelSymbol = ASM_DefineLabel (label, startOffset);
	      if (oDecodeTable[indexFound].cb_type == eCB_Void)
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.vd ();
	      else
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.vdpm (doLowerCase);
	      /* Define the label *after* the mnemonic implementation but
	         with the current offset *before* processing the mnemonic
	         (and only when the mnemonic is a valid one).  */
	      if (label->tag != LexLocalLabel)
		labelSymbol = tryAsMacro ? NULL : ASM_DefineLabel (label, startOffset);
	      break;
	    }

	  case eCB_Lex:
	  case eCB_LexPMatch:
	    {
	      const Lex noLex = { .tag = LexNone };
	      const Lex *labelLexP;
	      bool lclLabelWarn;
	      if (label->tag == LexLocalLabel)
		{
		  lclLabelWarn = true;
		  labelLexP = &noLex;
		}
	      else
		{
		  lclLabelWarn = false;
		  labelLexP = label;
		}
	      /* Any valid label here will *not* get any size assigned, unless
		 the callback turns the lex into a symbol.  */
	      if (oDecodeTable[indexFound].cb_type == eCB_Lex)
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.lex (labelLexP);
	      else
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.lexpm (labelLexP, doLowerCase);
	      if (tryAsMacro)
		labelSymbol = NULL;
	      else
		{
		  labelSymbol = symbolFind (labelLexP);
		  if (lclLabelWarn)
		    error (ErrorWarning, "Local label not allowed here - ignoring");
		}
	      break;
	    }

	  case eCB_NoLex:
	  case eCB_NoLexPMatch:
	    {
	      if (label->tag != LexNone)
		error (ErrorWarning, "Label not allowed here - ignoring");
	      if (oDecodeTable[indexFound].cb_type == eCB_NoLex)
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.nolex ();
	      else
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.nolexpm (doLowerCase);
	      labelSymbol = NULL;
	      break;
	    }

	  case eCB_Symbol:
	    {
	      Symbol *symbol = label->tag == LexId ? symbolGet (label) : NULL;
	      bool lclLabelWarn = label->tag == LexLocalLabel;
	      tryAsMacro = oDecodeTable[indexFound].parse_opcode.sym (symbol);
	      if (!tryAsMacro && lclLabelWarn)
		error (ErrorWarning, "Local label not allowed here - ignoring");
	      /* We don't want to define a label based on this symbol.  */
	      labelSymbol = NULL;
	      break;
	    }

	  default:
	    tryAsMacro = false;
	    break;
	}

      /* When areaCurrentSymbol changed, this can only be using "AREA" and that means
	 no increase of curIdx.  */
      if (startAreaSymbol != areaCurrentSymbol)
	{
	  assert (!strcmp (oDecodeTable[indexFound].mnemonic, "AREA"));
	  startOffset = areaCurrentSymbol->area.info->curIdx;
	}
      // Too strong: assert ((oDecodeTable[indexFound].result != eRslt_None || areaCurrentSymbol->area.info->curIdx - startOffset == 0) && "Area size increase but no data, ARM nor Thumb mnemonic parsed ?");

      /* Give warning when ARM/Thumb instructions are being used in DATA
         areas.  */
      if (!tryAsMacro
          && (oDecodeTable[indexFound].result == eRslt_ARM
	      || oDecodeTable[indexFound].result == eRslt_Thumb)
          && !(areaCurrentSymbol->area.info->type & AREA_CODE))
	error (ErrorWarning, "Code generated in data area");

      /* Determine the code size associated with the label on this line (if any).  */
      if (labelSymbol != NULL)
        {
	  size_t codeSize;
          /* Either we have an increase in code/data in our current area, either
             we have an increase in storage map, either non of the previous (like
	     with "<lbl> * <value>" input).  */
	  if (areaCurrentSymbol->area.info->curIdx - startOffset != 0)
	    codeSize = areaCurrentSymbol->area.info->curIdx - startOffset;
	  else
	    {
	      codeInit ();
	      codeValue (storageValue (), false);
	      codeValue (&startStorage, false);
	      codeOperator (Op_sub);
	      const Value *value = codeEval (ValueInt, NULL);
	      if (value->Tag == ValueInt)
		codeSize = value->Data.Int.i;
	      else
		error (ErrorError, "Failed to determine label size");
	    }
	  assert ((gASM_Phase == ePassOne && labelSymbol->codeSize == 0) || (gASM_Phase == ePassTwo && labelSymbol->codeSize == codeSize));
	  labelSymbol->codeSize = codeSize;
	}

      valueFree (&startStorage);
    }
  else
    tryAsMacro = true;
  if (tryAsMacro)
    {
      /* Mnemonic is not recognized, maybe it is a macro.  */
      Input_RollBackToMark (inputMark);
      size_t macroNameLen;
      const char *macroName = Input_Symbol (&macroNameLen);
      const Macro *m = Macro_Find (macroName, macroNameLen);
      if (m)
        Macro_Call (m, label);
      else
	{
	  error (ErrorError, "'%.*s' is not a recognized mnemonic nor known macro",
		 (int)macroNameLen, macroName);
	  return;
        }
    }

  /* Sanity check we have consumed the complete line.  */
  decode_finalcheck ();
}


/**
 * Checks if the remaining part of the line are only blanks or comment part.
 */
void
decode_finalcheck (void)
{
  skipblanks ();
  if (!Input_IsEolOrCommentStart ())
    error (ErrorError, "Skipping extra characters '%s'", Input_Rest ());
}
