/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
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
#include "debug_dwarf.h"
#include "decode.h"
#include "directive_data.h"
#include "directive_file.h"
#include "directive_misc.h"
#include "directive_storagemap.h"
#include "directive_symbol.h"
#include "error.h"
#include "filestack.h"
#include "frame.h"
#include "input.h"
#include "it.h"
#include "lit.h"
#include "local.h"
#include "macros.h"
#include "m_coprocessor.h"
#include "m_cpu.h"
#include "m_cpuctrl.h"
#include "m_cpumem.h"
#include "m_fpe.h"
#include "opt.h"
#include "main.h"
#include "reloc.h"
#include "state.h"
#include "whileif.h"

typedef bool (*po_void)(void); /* For eCB_Void.  */
typedef bool (*po_void_pm)(bool doLowerCase); /* For eCB_VoidPMatch.  As
  there is a partial match, the doLowerCase parameter is needed to take into
  account for further mnemonic matching.  */
typedef bool (*po_nolex)(void); /* eCB_NoLex and eCB_NoLexPMatch. The partial
  match NoLex directives are all in uppercase so no need for a doLowerCase
  parameter.  */
typedef bool (*po_lex)(const Lex *labelP); /* For eCB_Lex and eCB_LexPMatch.  */
typedef bool (*po_sym)(Symbol *symbolP); /* For eCB_Symbol.  */

/* Determine the kind of callback to do wrt the LexId @ column 0.  */
enum
{
  eCB_Void        = 0, /* If there is a valid LexId/LexLocalLabel @ column 0, define this as a symbol (label).  */
  eCB_VoidPMatch  = 1, /* Same as eCB_Void + further mnemonic decoding needs to be done by parse_opcode callback.  */
  eCB_NoLex       = 2, /* If there is a valid LexId/LexLocalLabel @ column 0, complain about it as it is not allowed.  */
  eCB_NoLexPMatch = 3, /* Same as eCB_NoLex + further mnemonic decoding needs to be done by parse_opcode callback.  */
  eCB_Lex         = 4, /* If there is a valid LexId @ column 0, pass it on as parameter.  If it is made a symbol afterwards by the callback, assign code size to it.  */
  eCB_LexPMatch   = 5, /* Save as eCB_Lex + further mnemonic decoding needs to be done by parse_opcode callback.  */
  eCB_Symbol      = 6  /* If there is a valid LexId @ column 0, turn this into a symbol and pass it on as parameter.  */
};
typedef struct
{
  const char *mnemonic; /* Ptr to mnemonic (or first part of mnemonic).  */
  unsigned int cb_type:3; /* See eCB_* values.  */
  unsigned int instr:1; /* Is an ARM/Thumb instruction, used to decide if
    lowercase mnemonic is allowed or not. Is 0 for directives (incl. DCI).  */
  unsigned int updateMap:1; /* Whether the mapping symbol needs to be
    emited.  */
  union
    {
      po_void vd; /* Callback for eCB_Void.  */
      po_void_pm vdpm; /* Callback for eCB_VoidPMatch.  */
      po_nolex nolex; /* Callback for eCB_NoLex and eCB_NoLexPMatch.  */
      po_lex lex; /* Callback for eCB_Lex and eCB_LexPMatch.  */
      po_sym sym; /* Callback for eCB_Symbol.  */
    } parse_opcode;
} decode_table_t;

/* This table is sorted alphabetically.
   Adjust kDecodeTableIndex_* values when necessary after updating
   oDecodeTable.  */
#define kDecodeTableIndex_RELOC (143)
static const decode_table_t oDecodeTable[] =
{
  { "!", eCB_NoLex, 0, 0, { .nolex = c_info } }, /* INFO shorthand */
  { "#", eCB_Lex, 0, 0, { .lex = c_alloc } }, /* # / FIELD : reserve space in the current record.  */
  { "%", eCB_Void, 0, 1, { .vd = c_reserve } }, /* % / SPACE : reserve space.  */
  { "&", eCB_Void, 0, 1, { .vd = c_ampersand } }, /* & */
  { "*", eCB_Symbol, 0, 0, { .sym = c_equ } }, /* * / EQU */
  { "=", eCB_Void, 0, 1, { .vd = c_dcb } }, /* = / DCB */
  { "ABS", eCB_VoidPMatch, 1, 1, { .vdpm = m_abs } }, /* ABS CC P R */
  { "ACS", eCB_VoidPMatch, 1, 1, { .vdpm = m_acs } }, /* ACS CC P R */
  { "ADC", eCB_VoidPMatch, 1, 1, { .vdpm = m_adc } }, /* ADC CC S */
  { "ADD", eCB_VoidPMatch, 1, 1, { .vdpm = m_add } }, /* ADD CC S */
  { "ADF", eCB_VoidPMatch, 1, 1, { .vdpm = m_adf } }, /* ADF CC P R */
  { "ADR", eCB_VoidPMatch, 1, 1, { .vdpm = m_adr } }, /* ADR CC */
  { "ALIGN", eCB_Void, 0, 0, { .vd = c_align } }, /* ALIGN */
  { "AND", eCB_VoidPMatch, 1, 1, { .vdpm = m_and } }, /* AND CC S */
  { "AOF", eCB_NoLex, 0, 0, { .nolex = c_aof } }, /* AOF */
  { "AOUT", eCB_NoLex, 0, 0, { .nolex = c_aout } }, /* AOUT */
  { "AREA", eCB_NoLex, 0, 0, { .nolex = c_area } }, /* AREA */
  { "ARM", eCB_NoLex, 0, 0, { .nolex = c_code32 } }, /* ARM/CODE32 */
  { "ASN", eCB_VoidPMatch, 1, 1, { .vdpm = m_asn } }, /* ASN CC P R */
  { "ASR", eCB_VoidPMatch, 1, 1, { .vdpm = m_asr } }, /* ASR S CC */
  { "ASSERT", eCB_NoLex, 0, 0, { .nolex = c_assert } }, /* ASSERT */
  { "ATN", eCB_VoidPMatch, 1, 1, { .vdpm = m_atn } }, /* ATN CC P R */
  { "B", eCB_VoidPMatch, 1, 1, { .vdpm = m_branch } }, /* B [L] CC */
  { "BFC", eCB_VoidPMatch, 1, 1, { .vdpm = m_bfc } }, /* BFC */
  { "BFI", eCB_VoidPMatch, 1, 1, { .vdpm = m_bfi } }, /* BFC */
  { "BIC", eCB_VoidPMatch, 1, 1, { .vdpm = m_bic } }, /* BIC CC S */
  { "BIN", eCB_NoLex, 0, 0, { .nolex = c_incbin } }, /* BIN / INCBIN */
  { "BKPT", eCB_Void, 1, 1, { .vd = m_bkpt } }, /* BKPT */
  { "BLX", eCB_VoidPMatch, 1, 1, { .vdpm = m_blx } }, /* BLX CC */
  { "BX", eCB_VoidPMatch, 1, 1, { .vdpm = m_bx } }, /* BX CC */
  { "BXJ", eCB_VoidPMatch, 1, 1, { .vdpm = m_bxj } }, /* BXJ CC */
  { "CDP", eCB_VoidPMatch, 1, 1, { .vdpm = m_cdp } }, /* CDP CC / CDP2 */
  { "CLREX", eCB_Void, 1, 1, { .vd = m_clrex } }, /* CLREX */
  { "CLZ", eCB_VoidPMatch, 1, 1, { .vdpm = m_clz } }, /* CLZ CC */
  { "CMF", eCB_VoidPMatch, 1, 1, { .vdpm = m_cmf } }, /* CMF CC or CMFE CC */
  { "CMN", eCB_VoidPMatch, 1, 1, { .vdpm = m_cmn } }, /* CMN CC SP */
  { "CMP", eCB_VoidPMatch, 1, 1, { .vdpm = m_cmp } }, /* CMP CC SP */
  { "CN", eCB_Symbol, 0, 0, { .sym = c_cn } }, /* CN */
  { "CNF", eCB_VoidPMatch, 1, 1, { .vdpm = m_cnf } }, /* CNF CC or CNFE CC */
  { "CODE16", eCB_NoLex, 0, 0, { .nolex = c_code16 } }, /* CODE16 */
  { "CODE32", eCB_NoLex, 0, 0, { .nolex = c_code32 } }, /* ARM/CODE32 */
  { "COMMON", eCB_NoLex, 0, 0, { .nolex = c_common } }, /* COMMON */
  { "COS", eCB_VoidPMatch, 1, 1, { .vdpm = m_cos } }, /* COS CC P R */
  { "CP", eCB_Symbol, 0, 0, { .sym = c_cp } }, /* CP */
  { "CPS", eCB_VoidPMatch, 1, 1, { .vdpm = m_cps } }, /* CPS */
  { "DATA", eCB_Void, 0, 0, { .vd = c_data } }, /* DATA */
  { "DBG", eCB_VoidPMatch, 1, 1, { .vdpm = m_dbg } }, /* DBG */
  { "DCB", eCB_Void, 0, 1, { .vd = c_dcb } }, /* = / DCB */
  { "DCD", eCB_VoidPMatch, 0, 1, { .vdpm = c_dcd } }, /* DCD / DCDU */
  /* FIXME: DCDO */
  { "DCFD", eCB_VoidPMatch, 0, 1, { .vdpm = c_dcfd } }, /* DCFD / DCFDU */
  { "DCFH", eCB_VoidPMatch, 0, 1, { .vdpm = c_dcfh } }, /* DCFH / DCFHU */
  { "DCFS", eCB_VoidPMatch, 0, 1, { .vdpm = c_dcfs } }, /* DCFS / DCFSU */
  { "DCI", eCB_VoidPMatch, 0, 1, { .vdpm = c_dci } }, /* DCI */
  { "DCQ", eCB_VoidPMatch, 0, 1, { .vdpm = c_dcq } }, /* DCQ / DCQU */
  { "DCW", eCB_VoidPMatch, 0, 1, { .vdpm = c_dcw } }, /* DCW / DCWU */
  /* FIXME: DN */
  { "DMB", eCB_Void, 1, 1, { .vd = m_dmb } }, /* DMB */
  { "DSB", eCB_Void, 1, 1, { .vd = m_dsb } }, /* DSB */
  { "DVF", eCB_VoidPMatch, 1, 1, { .vdpm = m_dvf } }, /* DVF CC P R */
  { "ELIF", eCB_NoLex, 0, 0, { .nolex = c_elif } }, /* ELIF */
  { "ELSE", eCB_NoLex, 0, 0, { .nolex = c_else } }, /* | ELSE */
  { "END", eCB_NoLex, 0, 0, { .nolex = c_end } }, /* END */
  { "ENDFUNC", eCB_NoLex, 0, 0, { .nolex = c_endfunc } }, /* ENDFUNC / ENDP */
  { "ENDIF", eCB_NoLex, 0, 0, { .nolex = c_endif } }, /* ] ENDIF */
  { "ENDP", eCB_NoLex, 0, 0, { .nolex = c_endfunc } }, /* ENDFUNC / ENDP */
  { "ENTRY", eCB_NoLex, 0, 0, { .nolex = c_entry } }, /* ENTRY */
  { "EOR", eCB_VoidPMatch, 1, 1, { .vdpm = m_eor } }, /* EOR CC S */
  { "EQU", eCB_Symbol, 0, 0, { .sym = c_equ } }, /* * / EQU */
  { "EXP", eCB_VoidPMatch, 1, 1, { .vdpm = m_exp } }, /* EXP CC P R */
  { "EXPORT", eCB_NoLex, 0, 0, { .nolex = c_export } }, /* EXPORT / GLOBAL */
  { "EXTERN", eCB_NoLex, 0, 0, { .nolex = c_import } }, /* IMPORT / EXTERN */
  { "FDV", eCB_VoidPMatch, 1, 1, { .vdpm = m_fdv } }, /* FDV CC P R */
  { "FIELD", eCB_Lex, 0, 0, { .lex = c_alloc } }, /* # / FIELD : reserve space in the current record.  */
  { "FILL", eCB_Void, 0, 1, { .vd = c_fill } }, /* FILL : reserve space.  */
  { "FIX", eCB_VoidPMatch, 1, 1, { .vdpm = m_fix } }, /* FIX CC [P] R */
  { "FLT", eCB_VoidPMatch, 1, 1, { .vdpm = m_flt } }, /* FLT CC P R */
  { "FML", eCB_VoidPMatch, 1, 1, { .vdpm = m_fml } }, /* FML CC P R */
  { "FN", eCB_Symbol, 0, 0, { .sym = c_fn } }, /* FN */
  { "FRAME", eCB_NoLex, 0, 0, { .nolex = c_frame } }, /* FRAME */
  { "FRD", eCB_VoidPMatch, 1, 1, { .vdpm = m_frd } }, /* FRD CC P R */
  { "FUNCTION", eCB_NoLex, 0, 0, { .nolex = c_function } }, /* FUNCTION / PROC */
  { "GBL", eCB_NoLexPMatch, 0, 0, { .nolex = c_gbl } }, /* GBLA, GBLL, GBLS */
  { "GET", eCB_NoLex, 0, 0, { .nolex = c_get } }, /* GET */
  { "GLOBAL", eCB_NoLex, 0, 0, { .nolex = c_export } }, /* EXPORT / GLOBAL */
  { "IDFN", eCB_NoLex, 0, 0, { .nolex = c_idfn } }, /* IDFN */
  { "IF", eCB_NoLex, 0, 0, { .nolex = c_if } }, /* [ IF */
  { "IMPORT", eCB_NoLex, 0, 0, { .nolex = c_import } }, /* IMPORT / EXTERN */
  { "INCBIN", eCB_NoLex, 0, 0, { .nolex = c_incbin } }, /* BIN / INCBIN */
  { "INCLUDE", eCB_NoLex, 0, 0, { .nolex = c_get } }, /* GET / INCLUDE */
  { "INFO", eCB_NoLex, 0, 0, { .nolex = c_info } }, /* INFO */
  { "ISB", eCB_Void, 1, 1, { .vd = m_isb } }, /* ISB */
  { "IT", eCB_VoidPMatch, 1, 1, { .vdpm = m_it } }, /* IT */
  { "KEEP", eCB_NoLex, 0, 0, { .nolex = c_keep } }, /* KEEP */
  { "LCL", eCB_NoLexPMatch, 0, 0, { .nolex = c_lcl } }, /* LCLA, LCLL, LCLS */
  { "LDC", eCB_VoidPMatch, 1, 1, { .vdpm = m_ldc } }, /* LDC CC L / LDC2 */
  { "LDF", eCB_VoidPMatch, 1, 1, { .vdpm = m_ldf } }, /* LDF CC P */
  { "LDM", eCB_VoidPMatch, 1, 1, { .vdpm = m_ldm } }, /* LDM CC TYPE */
  { "LDR", eCB_VoidPMatch, 1, 1, { .vdpm = m_ldr } }, /* LDR CC BYTE */
  { "LDREX", eCB_VoidPMatch, 1, 1, { .vdpm = m_ldrex } }, /* LDREX / LDREXB / LDREXD / LDREXH */
  { "LEAF", eCB_NoLex, 0, 0, { .nolex = c_leaf } }, /* LEAF */
  { "LFM", eCB_VoidPMatch, 1, 1, { .vdpm = m_lfm } }, /* LFM CC (TYPE) */
  { "LGN", eCB_VoidPMatch, 1, 1, { .vdpm = m_lgn } }, /* LGN CC P R */
  { "LNK", eCB_NoLex, 0, 0, { .nolex = c_lnk } }, /* LNK */
  { "LOG", eCB_VoidPMatch, 1, 1, { .vdpm = m_log } }, /* LOG CC P R */
  { "LSL", eCB_VoidPMatch, 1, 1, { .vdpm = m_lsl } }, /* LSL S CC */
  { "LSR", eCB_VoidPMatch, 1, 1, { .vdpm = m_lsr } }, /* LSR S CC */
  { "LTORG", eCB_NoLex, 0, 0, { .nolex = c_ltorg } }, /* LTORG */
  { "MACRO", eCB_NoLex, 0, 0, { .nolex = c_macro } }, /* MACRO */
  { "MAP", eCB_NoLex, 0, 0, { .nolex = c_record } }, /* ^ / MAP : start of new record layout.  */
  { "MCR", eCB_VoidPMatch, 1, 1, { .vdpm = m_mcr } }, /* MCR CC / MCR2 */
  { "MCRR", eCB_VoidPMatch, 1, 1, { .vdpm = m_mcrr } }, /* MCRR CC */
  { "MEND", eCB_NoLex, 0, 0, { .nolex = c_mend } }, /* MEND */
  { "MEXIT", eCB_NoLex, 0, 0, { .nolex = c_mexit } }, /* MEXIT */
  { "MLA", eCB_VoidPMatch, 1, 1, { .vdpm = m_mla } }, /* MLA CC S */
  { "MLS", eCB_VoidPMatch, 1, 1, { .vdpm = m_mls } }, /* MLS CC */
  { "MNF", eCB_VoidPMatch, 1, 1, { .vdpm = m_mnf } }, /* MNF CC P R */
  { "MOV", eCB_VoidPMatch, 1, 1, { .vdpm = m_mov } }, /* MOV / MOVW / MOVT / MOV32 */
  { "MRC", eCB_VoidPMatch, 1, 1, { .vdpm = m_mrc } }, /* MRC CC / MRC2 */
  { "MRRC", eCB_VoidPMatch, 1, 1, { .vdpm = m_mrrc } }, /* MRRC CC */
  { "MRS", eCB_VoidPMatch, 1, 1, { .vdpm = m_mrs } }, /* MRS CC */
  { "MSR", eCB_VoidPMatch, 1, 1, { .vdpm = m_msr } }, /* MSR CC */
  { "MUF", eCB_VoidPMatch, 1, 1, { .vdpm = m_muf } }, /* MUF CC P R */
  { "MUL", eCB_VoidPMatch, 1, 1, { .vdpm = m_mul } }, /* MUL CC S */
  { "MVF", eCB_VoidPMatch, 1, 1, { .vdpm = m_mvf } }, /* MVF CC P R */
  { "MVN", eCB_VoidPMatch, 1, 1, { .vdpm = m_mvn } }, /* MVN CC S */
  { "NOFP", eCB_NoLex, 0, 0, { .nolex = c_nofp } }, /* NOFP */
  { "NOP", eCB_VoidPMatch, 1, 1, { .vdpm = m_nop } }, /* NOP [CC] */
  { "NRM", eCB_VoidPMatch, 1, 1, { .vdpm = m_nrm } }, /* NRM CC P R */
  { "OPT", eCB_NoLex, 0, 0, { .nolex = c_opt } }, /* OPT */
  { "ORG", eCB_NoLex, 0, 0, { .nolex = c_org } }, /* ORG */
  { "ORR", eCB_VoidPMatch, 1, 1, { .vdpm = m_orr } }, /* ORR CC S */
  { "PKH", eCB_VoidPMatch, 1, 1, { .vdpm = m_pkh } }, /* PKHBT / PKHTB */
  { "PL", eCB_VoidPMatch, 1, 1, { .vdpm = m_pl } }, /* PLD, PLDW, PLI */
  { "POL", eCB_VoidPMatch, 1, 1, { .vdpm = m_pol } }, /* POL CC P R */
  { "POP", eCB_VoidPMatch, 1, 1, { .vdpm = m_pop } }, /* POP CC */
  { "POW", eCB_VoidPMatch, 1, 1, { .vdpm = m_pow } }, /* POW CC P R */
  { "PRESERVE8", eCB_NoLex, 0, 0, { .nolex = c_preserve8 } }, /* PRESERVE8 {TRUE}/{FALSE} */
  { "PROC", eCB_NoLex, 0, 0, { .nolex = c_function } }, /* FUNCTION / PROC */
  { "PUSH", eCB_VoidPMatch, 1, 1, { .vdpm = m_push } }, /* PUSH CC */
  { "QADD", eCB_VoidPMatch, 1, 1, { .vdpm = m_qadd } }, /* QADD CC */
  /* FIXME: QASX */
  { "QDADD", eCB_VoidPMatch, 1, 1, { .vdpm = m_qdadd } }, /* QDADD CC */
  { "QDSUB", eCB_VoidPMatch, 1, 1, { .vdpm = m_qdsub } }, /* QDSUB CC */
  /* FIXME: QN */
  /* FIXME: QSAX */
  { "QSUB", eCB_VoidPMatch, 1, 1, { .vdpm = m_qsub } }, /* QSUB CC */
  /* FIXME: RBIT */
  /* FIXME: REV, REV16, REVSH */
  { "RDF", eCB_VoidPMatch, 1, 1, { .vdpm = m_rdf } }, /* RDF CC P R */
  { "RELOC", eCB_NoLex, 0, 0, { .nolex = c_reloc } }, /* RELOC */
  { "REQUIRE8", eCB_NoLex, 0, 0, { .nolex = c_require8 } }, /* REQUIRE8 {TRUE}/{FALSE} */
  { "RFC", eCB_VoidPMatch, 1, 1, { .vdpm = m_rfc } }, /* RFC CC */
  { "RFE", eCB_VoidPMatch, 1, 1, { .vdpm = m_rfe } }, /* RFE MODE */
  { "RFS", eCB_VoidPMatch, 1, 1, { .vdpm = m_rfs } }, /* RFS CC */
  { "RLIST", eCB_Symbol, 0, 0, { .sym = c_rlist } }, /* RLIST */
  { "RMF", eCB_VoidPMatch, 1, 1, { .vdpm = m_rmf } }, /* RMF CC P R */
  { "RN", eCB_Symbol, 0, 0, { .sym = c_rn } }, /* RN */
  { "RND", eCB_VoidPMatch, 1, 1, { .vdpm = m_rnd } }, /* RND CC P R */
  { "ROR", eCB_VoidPMatch, 1, 1, { .vdpm = m_ror } }, /* ROR S CC */
  { "ROUT", eCB_Lex, 0, 0, { .lex = c_rout } }, /* ROUT */
  { "RPW", eCB_VoidPMatch, 1, 1, { .vdpm = m_rpw } }, /* RPW CC P R */
  { "RRX", eCB_VoidPMatch, 1, 1, { .vdpm = m_rrx } }, /* RRX S CC */
  { "RSB", eCB_VoidPMatch, 1, 1, { .vdpm = m_rsb } }, /* RSB CC S */
  { "RSC", eCB_VoidPMatch, 1, 1, { .vdpm = m_rsc } }, /* RSC CC S */
  { "RSF", eCB_VoidPMatch, 1, 1, { .vdpm = m_rsf } }, /* RSF CC P R */
  /* FIXME: SADD16/SADD8 */
  /* FIXME: SASX */
  { "SBC", eCB_VoidPMatch, 1, 1, { .vdpm = m_sbc } }, /* SBC CC S */
  { "SBFX", eCB_VoidPMatch, 1, 1, { .vdpm = m_sbfx } }, /* SBFX */
  /* FIXME: SEL */
  { "SET", eCB_LexPMatch, 0, 0, { .lex = c_set } }, /* SETA, SETL, SETS */
  /* FIXME: SETEND, note: deprecated for ARMv8.  */
  { "SEV", eCB_VoidPMatch, 1, 1, { .vdpm = m_sev } }, /* SEV CC */
  { "SFM", eCB_VoidPMatch, 1, 1, { .vdpm = m_sfm } }, /* SFM CC (TYPE) */
  /* FIXME: SHADD16/SHADD8 */
  /* FIXME: SHASX/SHSAX */
  /* FIXME: SHSUB16/SHSUB8 */
  { "SIN", eCB_VoidPMatch, 1, 1, { .vdpm = m_sin } }, /* SIN CC P R */
  { "SMC", eCB_VoidPMatch, 1, 1, { .vdpm = m_smc } }, /* SMC CC */
  { "SMI", eCB_VoidPMatch, 1, 1, { .vdpm = m_smc } }, /* SMI CC (pre-UAL) */
  { "SMLABB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlabb } }, /* SMLABB CC */
  { "SMLABT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlabt } }, /* SMLABT CC */
  /* FIXME: SMLAD */
  { "SMLAL", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlal } }, /* SMLAL CC S */
  { "SMLALBB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlalbb } }, /* SMLALBB CC */
  { "SMLALBT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlalbt } }, /* SMLALBT CC */
  { "SMLALTB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlaltb } }, /* SMLALTB CC */
  { "SMLALTT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlaltt } }, /* SMLALTT CC */
  /* FIXME: SMLAD */
  { "SMLATB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlatb } }, /* SMLATB CC */
  { "SMLATT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlatt } }, /* SMLATT CC */
  { "SMLAWB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlawb } }, /* SMLAWB CC */
  { "SMLAWT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smlawt } }, /* SMLAWT CC */
  /* FIXME: SMLSD, SMLSLD */
  /* FIXME: SMMLA, SMMLS, SMMUL, SMUAD */
  { "SMULBB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smulbb } }, /* SMULBB CC */
  { "SMULBT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smulbt } }, /* SMULBT CC */
  { "SMULL", eCB_VoidPMatch, 1, 1, { .vdpm = m_smull } }, /* SMULL CC */
  { "SMULTB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smultb } }, /* SMULTB CC */
  { "SMULTT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smultt } }, /* SMULTT CC */
  { "SMULWB", eCB_VoidPMatch, 1, 1, { .vdpm = m_smulwb } }, /* SMULWB CC */
  { "SMULWT", eCB_VoidPMatch, 1, 1, { .vdpm = m_smulwt } }, /* SMULWT CC */
  /* FIXME: SMUSD */
  /* FIXME: SN */
  { "SPACE", eCB_Void, 0, 1, { .vd = c_reserve } }, /* % / SPACE : reserve space.  */
  { "SQT", eCB_VoidPMatch, 1, 1, { .vdpm = m_sqt } }, /* SQT CC P R */
  { "SRS", eCB_VoidPMatch, 1, 1, { .vdpm = m_srs } }, /* SRS MODE */
  /* FIXME: SSAT, SSAT16 */
  /* FIXME: SSAX */
  /* FIXME: SSUB16, SSUB8 */
  { "STC", eCB_VoidPMatch, 1, 1, { .vdpm = m_stc } }, /* STC CC l / STC2 */
  { "STF", eCB_VoidPMatch, 1, 1, { .vdpm = m_stf } }, /* STF CC P */
  /* FIXME: STLEX : ARMv8 only */
  { "STM", eCB_VoidPMatch, 1, 1, { .vdpm = m_stm } }, /* STM CC TYPE */
  { "STR", eCB_VoidPMatch, 1, 1, { .vdpm = m_str } }, /* STR CC BYTE */
  { "STREX", eCB_VoidPMatch, 1, 1, { .vdpm = m_strex } }, /* STREX / STREXB / STREXD / STREXH */
  { "STRONG", eCB_NoLex, 0, 0, { .nolex = c_strong } }, /* STRONG */
  { "SUB", eCB_VoidPMatch, 1, 1, { .vdpm = m_sub } }, /* SUB CC S */
  { "SUBT", eCB_NoLex, 0, 0, { .nolex = c_title } }, /* SUBT */
  { "SUF", eCB_VoidPMatch, 1, 1, { .vdpm = m_suf } }, /* SUF CC P R */
  { "SVC", eCB_VoidPMatch, 1, 1, { .vdpm = m_swi } }, /* SVC CC */
  { "SWI", eCB_VoidPMatch, 1, 1, { .vdpm = m_swi } }, /* SWI CC */
  { "SWP", eCB_VoidPMatch, 1, 1, { .vdpm = m_swp } }, /* SWP CC B */
  { "SXT", eCB_VoidPMatch, 1, 1, { .vdpm = m_sxt } }, /* SXT */
  { "TAN", eCB_VoidPMatch, 1, 1, { .vdpm = m_tan } }, /* TAN CC P R */
  { "TEQ", eCB_VoidPMatch, 1, 1, { .vdpm = m_teq } }, /* TEQ CC */
  { "THUMB", eCB_NoLex, 0, 0, { .nolex = c_thumb } }, /* THUMB */
  { "THUMBX", eCB_NoLex, 0, 0, { .nolex = c_thumbx } }, /* THUMBX */
  { "TST", eCB_VoidPMatch, 1, 1, { .vdpm = m_tst } }, /* TST CC */
  { "TTL", eCB_NoLex, 0, 0, { .nolex = c_title } }, /* TTL */
  /* FIXME: UADD16, UADD8 */
  /* FIXME: UASX */
  { "UBFX", eCB_VoidPMatch, 1, 1, { .vdpm = m_ubfx } }, /* UBFX */
  /* FIXME: UHADD16, UHADD8 */
  /* FIXME: UHASX, UHSA */
  /* FIXME: UHSUB16, UHSUB8 */
  /* FIXME: UMAAL */
  { "UMLAL", eCB_VoidPMatch, 1, 1, { .vdpm = m_umlal } }, /* UMLAL CC */
  { "UMULL", eCB_VoidPMatch, 1, 1, { .vdpm = m_umull } }, /* UMULL CC */
  { "UND", eCB_VoidPMatch, 1, 1, { .vdpm = m_und } }, /* UND */
  /* FIXME: UQADD16, UQADD8 */
  /* FIXME: UQASX, UQSAX */
  /* FIXME: UQSUB16, UQSUB8 */
  /* FIXME: USAD8, USADA8 */
  /* FIXME: USAT, USAT16 */
  /* FIXME: USAX */
  /* FIXME: USUB16, USUB8 */
  { "URD", eCB_VoidPMatch, 1, 1, { .vdpm = m_urd } }, /* URD CC P R */
  { "UXT", eCB_VoidPMatch, 1, 1, { .vdpm = m_uxt } }, /* UXT */
  /* FIXME: V* */
  { "WEND", eCB_NoLex, 0, 0, { .nolex = c_wend } }, /* WEND */
  { "WFC", eCB_VoidPMatch, 1, 1, { .vdpm = m_wfc } }, /* WFC CC */
  { "WFE", eCB_VoidPMatch, 1, 1, { .vdpm = m_wfe } }, /* WFE CC */
  { "WFI", eCB_VoidPMatch, 1, 1, { .vdpm = m_wfi } }, /* WFI CC */
  { "WFS", eCB_VoidPMatch, 1, 1, { .vdpm = m_wfs } }, /* WFS CC */
  { "WHILE", eCB_NoLex, 0, 0, { .nolex = c_while } }, /* WHILE */
  { "YIELD", eCB_VoidPMatch, 1, 1, { .vdpm = m_yield } }, /* YIELD CC */
  { "[", eCB_NoLex, 0, 0, { .nolex = c_if } }, /* [ IF */
  { "]", eCB_NoLex, 0, 0, { .nolex = c_endif } }, /* ] ENDIF */
  { "^", eCB_NoLex, 0, 0, { .nolex = c_record } }, /* ^ / MAP : start of new record layout.  */
  { "|", eCB_NoLex, 0, 0, { .nolex = c_else } }, /* | ELSE */
};
#define DECODE_ENTRIES (sizeof (oDecodeTable) / sizeof (oDecodeTable[0]))

/**
 * Only ARM and Thumb instructions can be written with lower case mnemonics.
 */
static inline bool
LowerCaseIsOK (const decode_table_t *entry)
{
  return entry->instr != 0;
}

/**
 * \return true when we've selected an ARM or Thumb (including ThumbEE)
 * instruction (including the DCI directive).
 */
static inline bool
IsARMOrThumbInstr (const decode_table_t *entry)
{
  return entry->instr != 0
	   || (entry->mnemonic[0] == 'D' && entry->mnemonic[1] == 'C' && entry->mnemonic[2] == 'I');
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
Decode (const Lex *label)
{
#if 0
  /* Check that all entries in oDecodeTable are sorted.  */
  for (size_t i = 1; i != DECODE_ENTRIES; ++i)
    assert (strcmp (oDecodeTable[i - 1].mnemonic, oDecodeTable[i].mnemonic) < 0);
  assert (!strcmp (oDecodeTable[kDecodeTableIndex_RELOC].mnemonic, "RELOC"));
#endif
  assert (label->tag == LexNone || label->tag == LexId || label->tag == LexLocalLabel);

  /* Deal with empty line quickly.  */
  if (Input_IsEolOrCommentStart ())
    {
      (void) ASM_DefineLabel (label, areaCurrentSymbol->area->curIdx, false);
      return;
    }
  
  const char * const inputMark = Input_GetMark ();

  /* Determines whether the mnemonic is all lowercase or all uppercase.  */
  const bool doLowerCase = !option_uppercase && Input_Look () >= 'a' && Input_Look () <= 'z'; 
  
  /* Locate mnemonic entry in decode table.  */
  size_t low = 0;
  size_t high = DECODE_ENTRIES - 1;
  size_t indexFound = SIZE_MAX;
  for (size_t charsMatched = 0; /* */; ++charsMatched)
    {
      unsigned char c = (unsigned char)Input_GetC ();
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
	  Input_SkipN (strlen (&oDecodeTable[high].mnemonic[charsMatched + 1]));
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
        Input_SkipWS ();

      /* Take snapshots of current area, its current offset and current file
         and its current line number.  Executing the next instruction,
         mnemonic and directive can change any of those 'current' value.  */
      uint32_t startOffset = areaCurrentSymbol->area->curIdx;
      Symbol * const startAreaSymbol = areaCurrentSymbol;
      const char * const startFileNameP = FS_GetCurFileName ();
      const unsigned startLineNumber = FS_GetCurLineNumber ();

      Value startStorage = Value_Copy (StorageMap_Value ());

      /* Unless it's RELOC, disable the use of RELOC until further notice.  */
      if (indexFound != kDecodeTableIndex_RELOC)
	Reloc_DisableExplicitReloc (&areaCurrentSymbol->area->reloc);
      
      Symbol *labelSymbol;
      switch (oDecodeTable[indexFound].cb_type)
	{
	  case eCB_Void:
	  case eCB_VoidPMatch:
	    {
	      if (label->tag == LexLocalLabel)
		labelSymbol = ASM_DefineLabel (label, startOffset, false);
	      if (oDecodeTable[indexFound].cb_type == eCB_Void)
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.vd ();
	      else
		tryAsMacro = oDecodeTable[indexFound].parse_opcode.vdpm (doLowerCase);
	      /* Define the label *after* the mnemonic implementation but
	         with the current offset *before* processing the mnemonic
	         (and only when the mnemonic is a valid one).  */
	      break;
	    }

	  case eCB_Lex:
	  case eCB_LexPMatch:
	    {
	      assert (!doLowerCase);
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
	      tryAsMacro = oDecodeTable[indexFound].parse_opcode.lex (labelLexP);
	      if (!tryAsMacro)
		{
		  labelSymbol = Symbol_Find (labelLexP);
		  if (lclLabelWarn)
		    Error (ErrorWarning, "Local label not allowed here - ignoring");
		}
	      break;
	    }

	  case eCB_NoLex:
	  case eCB_NoLexPMatch:
	    {
	      assert (!doLowerCase);
	      if (label->tag != LexNone)
		Error (ErrorWarning, "Label not allowed here - ignoring");
	      tryAsMacro = oDecodeTable[indexFound].parse_opcode.nolex ();
	      labelSymbol = NULL;
	      break;
	    }

	  case eCB_Symbol:
	    {
	      assert (!doLowerCase);
	      Symbol *symbol = label->tag == LexId ? Symbol_Get (label) : NULL;
	      bool lclLabelWarn = label->tag == LexLocalLabel;
	      tryAsMacro = oDecodeTable[indexFound].parse_opcode.sym (symbol);
	      if (!tryAsMacro)
		{
		  /* We don't want to define a label based on this symbol.  */
		  labelSymbol = NULL;
		  if (lclLabelWarn)
		    Error (ErrorWarning, "Local label not allowed here - ignoring");
		}
	      break;
	    }

	  default:
	    tryAsMacro = false;
	    assert (0);
	    break;
	}

      if (!tryAsMacro)
	{
	  if (IsARMOrThumbInstr (&oDecodeTable[indexFound]))
	    {
	      /* Do the same area offset adjustment as done for any ARM/Thumb
		 instruction in Put_InsWithOffset().  */
	      unsigned alignValue = State_GetInstrType () == eInstrType_ARM ? 4 : 2;
	      startOffset = Area_AlignOffset (startAreaSymbol, startOffset, alignValue, NULL);

	      /* Mark all ARM/Thumb instructions applicable for an explicit
	         RELOC.  */
	      Reloc_EnableExplicitReloc (&startAreaSymbol->area->reloc, startOffset);
	    }

	  if (oDecodeTable[indexFound].updateMap)
	    {
	      /* Define mapping symbols.  */
	      Area_eEntryType entryType;
	      if (IsARMOrThumbInstr (&oDecodeTable[indexFound]))
		{
		  switch (State_GetInstrType ())
		    {
		      case eInstrType_ARM:
			entryType = eARM;
			break;
		      case eInstrType_Thumb:
			entryType = eThumb;
			break;
		      case eInstrType_ThumbEE:
			entryType = eThumbEE;
			break;
		    }
		}
	      else
		entryType = eData;
	      Area_MarkStartAs (startAreaSymbol, startOffset, entryType);
	      if (entryType != eData)
		DWARF_MarkAs (startAreaSymbol, startOffset,
			      startFileNameP, startLineNumber);

	      /* Give warning when ARM/Thumb instructions are being used in
	         DATA areas.  */
	      if ((entryType == eARM || entryType == eThumb)
	          && !(areaCurrentSymbol->area->type & AREA_CODE))
		Error (ErrorWarning, "Code generated in data area");

	      /* FIXME: test on currently unsupported Thumb/ThumbEE state.  */
	      static bool thumbWarnOnce;
	      if (gPhase == ePassTwo && !thumbWarnOnce && entryType == eThumb)
		{
		  thumbWarnOnce = true;
		  Error (ErrorWarning, "Thumb/ThumbEE is not fully supported");
		  return;
		}
	    }

	  /* Define label after the Area_MarkStartAs call as otherwise
	     SYMBOL_DATUM will not be correct.  */
	  /* FIXME: is it still necessary for LexLocalLabels to have them
	     defined before the instruction is parsed ? */
	  if ((oDecodeTable[indexFound].cb_type == eCB_Void || oDecodeTable[indexFound].cb_type == eCB_VoidPMatch)
	      && label->tag != LexLocalLabel)
	    labelSymbol = ASM_DefineLabel (label, startOffset, false);

	  /* When areaCurrentSymbol changed, this can only be using "AREA" and
	     that means no increase of curIdx.  */
	  if (startAreaSymbol != areaCurrentSymbol)
	    {
	      assert (!strcmp (oDecodeTable[indexFound].mnemonic, "AREA"));
	      startOffset = areaCurrentSymbol->area->curIdx;
	    }

	  /* Determine the code size associated with the label on this line
	     (if any).  */
	  if (labelSymbol != NULL)
	    {
	      uint32_t codeSize;
	      /* Either we have an increase in code/data in our current area,
	         either we have an increase in storage map, either non of the
	         previous (like with "<lbl> * <value>" input).  */
	      if (areaCurrentSymbol->area->curIdx - startOffset != 0)
		codeSize = areaCurrentSymbol->area->curIdx - startOffset;
	      else
		{
		  Code_Init ();
		  Code_Value (StorageMap_Value (), false);
		  Code_Value (&startStorage, false);
		  Code_Operator (eOp_Sub);
		  const Value *value = Code_Eval (ValueInt);
		  if (value->Tag == ValueInt)
		    codeSize = value->Data.Int.i;
		  else
		    Error (ErrorError, "Failed to determine label size");
		}
	      assert ((gPhase == ePassOne && labelSymbol->codeSize == 0) || (gPhase == ePassTwo && labelSymbol->codeSize == codeSize));
	      labelSymbol->codeSize = codeSize;
	    }
	}

      Value_Free (&startStorage);
    }
  else
    tryAsMacro = true;

  if (tryAsMacro)
    {
      /* Mnemonic/directive is not recognized, maybe it is a macro.  */
      Input_RollBackToMark (inputMark);
      size_t macroNameLen;
      const char *macroName = Input_Symbol (&macroNameLen);
      /* In case we have a column, we don't read any symbol.  */
      if (macroNameLen == 0)
	{
	  Error (ErrorError, "Mnemonic, directive or macro missing");
	  return;
	}
      else if (Macro_Call (macroName, macroNameLen, label))
	{
	  Error (ErrorError, "'%.*s' is not a recognized mnemonic, directive nor known macro",
		 (int)macroNameLen, macroName);
	  return;
        }
    }

  /* Sanity check we have consumed the complete line.  */
  Decode_FinalCheck ();
}


/**
 * Checks if the remaining part of the line are only blanks or comment part.
 */
void
Decode_FinalCheck (void)
{
  Input_SkipWS ();
  if (!Input_IsEolOrCommentStart ())
    {
      Error (ErrorError, "Unexpected characters");
      Input_Rest ();
    }
}
