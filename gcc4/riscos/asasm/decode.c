/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
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
#include "input.h"
#include "local.h"
#include "macros.h"
#include "m_cop.h"
#include "m_copmem.h"
#include "m_cpu.h"
#include "m_cpuctrl.h"
#include "m_cpumem.h"
#include "m_fpu.h"
#include "m_fpumem.h"
#include "option.h"
#include "storage.h"

typedef bool (*po_void)(void);
typedef bool (*po_lbl)(const Lex *labelP);
typedef bool (*po_sym)(Symbol *symbolP);

#define DTABLE_CALLBACK_VOID    0
#define DTABLE_CALLBACK_LEX     1
#define DTABLE_CALLBACK_SYMBOL  2
#define DTABLE_CALLBACK_MASK    3 /* This is a mask.  */
#define DTABLE_PART_MNEMONIC	(1<<2) /* Mnemonic is not full parsed yet.  */
typedef struct
{
  const char *mnemonic;
  unsigned int flags;
  union
    {
      po_void vd; /* Callback with void parameter.  */
      po_lbl lbl; /* Callback with Lex (representing label) parameter.  */
      po_sym sym; /* Callback with Symbol (representing label) parameter.  */
    } parse_opcode;
} decode_table_t;

/* This table is alphabetically ordered.  */
static const decode_table_t oDecodeTable[] =
{
  { "!", DTABLE_CALLBACK_VOID, { .vd = c_info } }, /* INFO shorthand */
  { "#", DTABLE_CALLBACK_SYMBOL, { .sym = c_alloc } }, /* # / FIELD : reserve space in the current record.  */
  { "%", DTABLE_CALLBACK_VOID, { .vd = c_reserve } }, /* % / SPACE : reserve space.  */
  { "&", DTABLE_CALLBACK_VOID, { .vd = c_dcd } }, /* & / DCD / DCDU */
  { "*", DTABLE_CALLBACK_SYMBOL, { .sym = c_equ } }, /* * / EQU */
  { "=", DTABLE_CALLBACK_VOID, { .vd = c_dcb } }, /* DCB */
  { "ABS", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_abs } }, /* ABS CC P R */
  { "ACS", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_acs } }, /* ACS CC P R */
  { "ADC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_adc } }, /* ADC CC S */
  { "ADD", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_add } }, /* ADD CC S */
  { "ADF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_adf } }, /* ADF CC P R */
  { "ADR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_adr } }, /* ADR CC */
  { "ALIGN", DTABLE_CALLBACK_VOID, { .vd = c_align } }, /* ALIGN */
  { "AND", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_and } }, /* AND CC S */
  { "AREA", DTABLE_CALLBACK_VOID, { .vd = c_area } }, /* AREA */
  { "ARM", DTABLE_CALLBACK_VOID, { .vd = c_code32 } }, /* ARM/CODE32 */
  { "ASN", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_asn } }, /* ASN CC P R */
  { "ASSERT", DTABLE_CALLBACK_VOID, { .vd = c_assert } }, /* ASSERT */
  { "ATN", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_atn } }, /* ATN CC P R */
  { "B", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_branch } }, /* B [L] CC */
  { "BIC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_bic } }, /* BIC CC S */
  { "BKPT", DTABLE_CALLBACK_VOID, { .vd = m_bkpt } }, /* BKPT */
  { "BLX", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_blx } }, /* BLX CC */
  { "BX", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_bx } }, /* BX CC */
  { "CDP", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_cdp } }, /* CDP CC */
  { "CDP2", DTABLE_CALLBACK_VOID, { .vd = m_cdp2 } }, /* CDP2 */
  { "CLZ", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_clz } }, /* CLZ CC */
  { "CMF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_cmf } }, /* CMF CC or CMFE CC */
  { "CMN", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_cmn } }, /* CMN CC SP */
  { "CMP", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_cmp } }, /* CMP CC SP */
  { "CN", DTABLE_CALLBACK_SYMBOL, { .sym = c_cn } }, /* CN */
  { "CNF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_cnf } }, /* CNF CC or CNFE CC */
  { "CODE16", DTABLE_CALLBACK_VOID, { .vd = c_code16 } }, /* CODE16 */
  { "CODE32", DTABLE_CALLBACK_VOID, { .vd = c_code32 } }, /* ARM/CODE32 */
  { "COS", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_cos } }, /* COS CC P R */
  { "CP", DTABLE_CALLBACK_SYMBOL, { .sym = c_cp } }, /* CP */
  { "DCB", DTABLE_CALLBACK_VOID, { .vd = c_dcb } }, /* DCB */
  { "DCD", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = c_dcd } }, /* & / DCD / DCDU */
  { "DCFD", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = c_dcfd } }, /* DCFD / DCFDU */
  { "DCFS", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = c_dcfs } }, /* DCFS / DCFSU */
  { "DCI", DTABLE_CALLBACK_VOID, { .vd = c_dci } }, /* DCI */
  { "DCW", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = c_dcw } }, /* DCW / DCWU */
  { "DVF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_dvf } }, /* DVF CC P R */
  { "ELIF", DTABLE_CALLBACK_LEX, { .lbl = c_elif } }, /* ELIF */
  { "ELSE", DTABLE_CALLBACK_LEX, { .lbl = c_else } }, /* | ELSE */
  { "END", DTABLE_CALLBACK_VOID, { .vd = c_end } }, /* END */
  { "ENDIF", DTABLE_CALLBACK_LEX, { .lbl = c_endif } }, /* ] ENDIF */
  { "ENTRY", DTABLE_CALLBACK_VOID, { .vd = c_entry } }, /* ENTRY */
  { "EOR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_eor } }, /* EOR CC S */
  { "EQU", DTABLE_CALLBACK_SYMBOL, { .sym = c_equ } }, /* * / EQU */
  { "EXP", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_exp } }, /* EXP CC P R */
  { "EXPORT", DTABLE_CALLBACK_VOID, { .vd = c_export } }, /* EXPORT / GLOBAL */
  { "EXTERN", DTABLE_CALLBACK_VOID, { .vd = c_import } }, /* IMPORT / EXTERN */
  { "FDV", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_fdv } }, /* FDV CC P R */
  { "FIELD", DTABLE_CALLBACK_SYMBOL, { .sym = c_alloc } }, /* # / FIELD : reserve space in the current record.  */
  { "FIX", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_fix } }, /* FIX CC [P] R */
  { "FLT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_flt } }, /* FLT CC P R */
  { "FML", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_fml } }, /* FML CC P R */
  { "FN", DTABLE_CALLBACK_SYMBOL, { .sym = c_fn } }, /* FN */
  { "FRD", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_frd } }, /* FRD CC P R */
  { "GBL", DTABLE_CALLBACK_LEX | DTABLE_PART_MNEMONIC, { .lbl = c_gbl } }, /* GBLA, GBLL, GBLS */
  { "GET", DTABLE_CALLBACK_VOID, { .vd = c_get } }, /* GET */
  { "GLOBAL", DTABLE_CALLBACK_VOID, { .vd = c_export } }, /* EXPORT / GLOBAL */
  { "HEAD", DTABLE_CALLBACK_VOID, { .vd = c_head } }, /* HEAD */
  { "IDFN", DTABLE_CALLBACK_VOID, { .vd = c_idfn } }, /* IDFN */
  { "IF", DTABLE_CALLBACK_LEX, { .lbl = c_if } }, /* [ IF */
  { "IMPORT", DTABLE_CALLBACK_VOID, { .vd = c_import } }, /* IMPORT / EXTERN */
  { "INCBIN", DTABLE_CALLBACK_VOID, { .vd = c_incbin } }, /* INCBIN */
  { "INCLUDE", DTABLE_CALLBACK_VOID, { .vd = c_get } }, /* GET / INCLUDE */
  { "INFO", DTABLE_CALLBACK_VOID, { .vd = c_info } }, /* INFO */
  { "KEEP", DTABLE_CALLBACK_VOID, { .vd = c_keep } }, /* KEEP */
  { "LCL", DTABLE_CALLBACK_LEX | DTABLE_PART_MNEMONIC, { .lbl = c_lcl } }, /* LCLA, LCLL, LCLS */
  { "LDC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_ldc } }, /* LDC CC L */
  { "LDC2", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_ldc2 } }, /* LDC2 L */
  { "LDF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_ldf } }, /* LDF CC P */
  { "LDM", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_ldm } }, /* LDM CC TYPE */
  { "LDR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_ldr } }, /* LDR CC BYTE */
  { "LFM", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_lfm } }, /* LFM CC (TYPE) */
  { "LGN", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_lgn } }, /* LGN CC P R */
  { "LNK", DTABLE_CALLBACK_VOID, { .vd = c_lnk } }, /* LNK */
  { "LOG", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_log } }, /* LOG CC P R */
  { "LTORG", DTABLE_CALLBACK_VOID, { .vd = c_ltorg } }, /* LTORG */
  { "MACRO", DTABLE_CALLBACK_LEX, { .lbl = c_macro } }, /* MACRO */
  { "MAP", DTABLE_CALLBACK_VOID, { .vd = c_record } }, /* ^ / MAP : start of new record layout.  */
  { "MCR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mcr } }, /* MCR CC */
  { "MCR2", DTABLE_CALLBACK_VOID, { .vd = m_mcr2 } }, /* MCR2 */
  { "MCRR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mcrr } }, /* MCRR CC */
  { "MEXIT", DTABLE_CALLBACK_LEX, { .lbl = c_mexit } }, /* MEXIT */
  { "MLA", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mla } }, /* MLA CC S */
  { "MNF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mnf } }, /* MNF CC P R */
  { "MOV", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mov } }, /* MOV CC s */
  { "MRC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mrc } }, /* MRC CC */
  { "MRC2", DTABLE_CALLBACK_VOID, { .vd = m_mrc2 } }, /* MRC2 */
  { "MRRC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mrrc } }, /* MRRC CC */
  { "MRS", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mrs } }, /* MRS CC */
  { "MSR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_msr } }, /* MSR CC */
  { "MUF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_muf } }, /* MUF CC P R */
  { "MUL", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mul } }, /* MUL CC S */
  { "MVF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mvf } }, /* MVF CC P R */
  { "MVN", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_mvn } }, /* MVN CC S */
  { "NOP", DTABLE_CALLBACK_VOID, { .vd = m_nop } }, /* NOP */
  { "NRM", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_nrm } }, /* NRM CC P R */
  { "OPT", DTABLE_CALLBACK_VOID, { .vd = c_opt } }, /* OPT */
  { "ORG", DTABLE_CALLBACK_VOID, { .vd = c_org } }, /* ORG */
  { "ORR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_orr } }, /* ORR CC S */
  { "PLD", DTABLE_CALLBACK_VOID, { .vd = m_pld } }, /* PLD */
  { "POL", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_pol } }, /* POL CC P R */
  { "POP", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_pop } }, /* POP CC */
  { "POW", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_pow } }, /* POW CC P R */
  { "PRESERVE8", DTABLE_CALLBACK_VOID, { .vd = c_preserve8 } }, /* PRESERVE8 {TRUE}/{FALSE} */
  { "PUSH", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_push } }, /* PUSH CC */
  { "QADD", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_qadd } }, /* QADD CC */
  { "QDADD", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_qdadd } }, /* QDADD CC */
  { "QDSUB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_qdsub } }, /* QDSUB CC */
  { "QSUB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_qsub } }, /* QSUB CC */
  { "RDF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rdf } }, /* RDF CC P R */
  { "REQUIRE8", DTABLE_CALLBACK_VOID, { .vd = c_require8 } }, /* REQUIRE8 {TRUE}/{FALSE} */
  { "RET", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_ret } }, /* RET CC */
  { "RFC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rfc } }, /* RFC CC */
  { "RFS", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rfs } }, /* RFS CC */
  { "RMF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rmf } }, /* RMF CC P R */
  { "RN", DTABLE_CALLBACK_SYMBOL, { .sym = c_rn } }, /* RN */
  { "RND", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rnd } }, /* RND CC P R */
  { "ROUT", DTABLE_CALLBACK_LEX, { .lbl = c_rout } }, /* ROUT */
  { "RPW", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rpw } }, /* RPW CC P R */
  { "RSB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rsb } }, /* RSB CC S */
  { "RSC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rsc } }, /* RSC CC S */
  { "RSF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_rsf } }, /* RSF CC P R */
  { "SBC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_sbc } }, /* SBC CC S */
  { "SET", DTABLE_CALLBACK_LEX | DTABLE_PART_MNEMONIC, { .lbl = c_set } }, /* SETA, SETL, SETS */
  { "SFM", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_sfm } }, /* SFM CC (TYPE) */
  { "SIN", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_sin } }, /* SIN CC P R */
  { "SMLABB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlabb } }, /* SMLABB CC */
  { "SMLABT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlabt } }, /* SMLABT CC */
  { "SMLAL", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlal } }, /* SMLAL CC S */
  { "SMLALBB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlalbb } }, /* SMLALBB CC */
  { "SMLALBT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlalbt } }, /* SMLALBT CC */
  { "SMLALTB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlaltb } }, /* SMLALTB CC */
  { "SMLALTT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlaltt } }, /* SMLALTT CC */
  { "SMLATB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlatb } }, /* SMLATB CC */
  { "SMLATT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlatt } }, /* SMLATT CC */
  { "SMLAWB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlawb } }, /* SMLAWB CC */
  { "SMLAWT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smlawt } }, /* SMLAWT CC */
  { "SMULBB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smulbb } }, /* SMULBB CC */
  { "SMULBT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smulbt } }, /* SMULBT CC */
  { "SMULL", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smull } }, /* SMULL CC */
  { "SMULTB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smultb } }, /* SMULTB CC */
  { "SMULTT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smultt } }, /* SMULTT CC */
  { "SMULWB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smulwb } }, /* SMULWB CC */
  { "SMULWT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_smulwt } }, /* SMULWT CC */
  { "SPACE", DTABLE_CALLBACK_VOID, { .vd = c_reserve } }, /* % / SPACE : reserve space.  */
  { "SQT", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_sqt } }, /* SQT CC P R */
  { "STACK", DTABLE_CALLBACK_VOID, { .vd = m_stack } }, /* STACK */
  { "STC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_stc } }, /* STC CC l */
  { "STC2", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_stc2 } }, /* STC2 CC l */
  { "STF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_stf } }, /* STF CC P */
  { "STM", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_stm } }, /* STM CC TYPE */
  { "STR", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_str } }, /* STR CC BYTE */
  { "STRONG", DTABLE_CALLBACK_VOID, { .vd = c_strong } }, /* STRONG */
  { "SUB", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_sub } }, /* SUB CC S */
  { "SUBT", DTABLE_CALLBACK_VOID, { .vd = c_title } }, /* SUBT */
  { "SUF", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_suf } }, /* SUF CC P R */
  { "SWI", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_swi } }, /* SWI CC */
  { "SWP", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_swp } }, /* SWP CC B */
  { "TAIL", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_tail } }, /* TAIL CC */
  { "TAN", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_tan } }, /* TAN CC P R */
  { "TEQ", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_teq } }, /* TEQ CC */
  { "THUMB", DTABLE_CALLBACK_VOID, { .vd = c_thumb } }, /* THUMB */
  { "THUMBX", DTABLE_CALLBACK_VOID, { .vd = c_thumbx } }, /* THUMBX */
  { "TST", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_tst } }, /* TST CC */
  { "TTL", DTABLE_CALLBACK_VOID, { .vd = c_title } }, /* TTL */
  { "UMLAL", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_umlal } }, /* UMLAL CC */
  { "UMULL", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_umull } }, /* UMULL CC */
  { "URD", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_urd } }, /* URD CC P R */
  { "WEND", DTABLE_CALLBACK_LEX, { .lbl = c_wend } }, /* WEND */
  { "WFC", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_wfc } }, /* WFC CC */
  { "WFS", DTABLE_CALLBACK_VOID | DTABLE_PART_MNEMONIC, { .vd = m_wfs } }, /* WFS CC */
  { "WHILE", DTABLE_CALLBACK_LEX, { .lbl = c_while } }, /* WHILE */
  { "[", DTABLE_CALLBACK_LEX, { .lbl = c_if } }, /* [ IF */
  { "]", DTABLE_CALLBACK_LEX, { .lbl = c_endif } }, /* ] ENDIF */
  { "^", DTABLE_CALLBACK_VOID, { .vd = c_record } }, /* ^ / MAP : start of new record layout.  */
  { "|", DTABLE_CALLBACK_LEX, { .lbl = c_else } }, /* | ELSE */
};
#define DECODE_ENTRIES (sizeof (oDecodeTable) / sizeof (oDecodeTable[0]))

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

  /* Deal with empty line quickly.  */
  if (Input_IsEolOrCommentStart ())
    {
      ASM_DefineLabel (label, areaCurrentSymbol->value.Data.Int.i);
      return;
    }

  const char * const inputMark = Input_GetMark ();

  /* Locate mnemonic entry in decode table.  */
  size_t low = 0;
  size_t high = DECODE_ENTRIES - 1;
  size_t charsMatched = 0;
  size_t indexFound = SIZE_MAX;
  while (1)
    {
      unsigned char c = (unsigned char)inputGet ();
      assert (c != 0);
      while (low <= high)
	{
	  size_t mid = low + (high - low) / 2;
	  unsigned char m = (unsigned char)oDecodeTable[mid].mnemonic[charsMatched];
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
	             && (unsigned char)oDecodeTable[lowNew - 1].mnemonic[charsMatched] == c)
		--lowNew;
	      low = lowNew;
	      size_t highNew = mid;
	      while (highNew < high
	             && (unsigned char)oDecodeTable[highNew + 1].mnemonic[charsMatched] == c)
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
          && (oDecodeTable[low].flags & DTABLE_PART_MNEMONIC))
	{
	  /* E.g. input "SUB" matching "SUB*" and "SUBT".  First try "SUBT"
	     and then fall back on the 'wildcard'.  */
	  while (memcmp (Input_GetMark (), &oDecodeTable[high].mnemonic[charsMatched + 1], strlen (&oDecodeTable[high].mnemonic[charsMatched + 1])))
	    --high;
	  inputSkipN (strlen (&oDecodeTable[high].mnemonic[charsMatched + 1]));
	  indexFound = high;
	}
      else
	{
          bool moreMatchingIsNeeded = oDecodeTable[high].mnemonic[charsMatched + 1] != '\0';
          bool moreMatchingIsPossible = !Input_IsEolOrCommentStart ()
					  && !isspace ((unsigned char)inputLook ());
          if (moreMatchingIsNeeded && moreMatchingIsPossible)
	    {
	      ++charsMatched;
	      continue;
	    }
	  if (moreMatchingIsNeeded && !moreMatchingIsPossible && low < high)
	    indexFound = low;
          if (!moreMatchingIsNeeded && !moreMatchingIsPossible)
	    {
	      /* We have a full match (it must be unique).  */
	      assert (low == high);
	      indexFound = high;
	    }
	}
      break;
    }

  bool tryAsMacro;
  if (indexFound != SIZE_MAX)
    {
      assert (Input_IsEolOrCommentStart ()
              || isspace ((unsigned char)inputLook ())
              || (oDecodeTable[indexFound].flags & DTABLE_PART_MNEMONIC));
      if (!(oDecodeTable[indexFound].flags & DTABLE_PART_MNEMONIC))
        skipblanks ();

      const int startOffset = areaCurrentSymbol ? areaCurrentSymbol->value.Data.Int.i : 0;
      Value startStorage =
	{
	  .Tag = ValueIllegal
	};
      Value_Assign (&startStorage, storageValue ());

      Symbol *labelSymbol;
      switch (oDecodeTable[indexFound].flags & DTABLE_CALLBACK_MASK)
	{
	  case DTABLE_CALLBACK_VOID:
	    {
	      int offset = areaCurrentSymbol->value.Data.Int.i;
	      tryAsMacro = oDecodeTable[indexFound].parse_opcode.vd ();
	      /* Define the label *after* the mnemonic implementation but
	         with the currnet offset *before* processing the mnemonic.  */
	      labelSymbol = ASM_DefineLabel (label, offset);
	    }
	    break;

	  case DTABLE_CALLBACK_LEX:
	    /* Any valid label here will *not* get any size assigned.  */
	    labelSymbol = NULL;
	    tryAsMacro = oDecodeTable[indexFound].parse_opcode.lbl (label);
	    break;

	  case DTABLE_CALLBACK_SYMBOL:
	    labelSymbol = ASM_DefineLabel (label, areaCurrentSymbol->value.Data.Int.i);
	    tryAsMacro = oDecodeTable[indexFound].parse_opcode.sym (labelSymbol);
	    break;

	  default:
	    tryAsMacro = false;
	    break;
	}
      if (tryAsMacro && labelSymbol)
	{
	  symbolRemove (label);
	  labelSymbol = NULL;
	}
      
      /* Determine the code size associated with the label on this line (if any).  */
      if (labelSymbol != NULL)
        {
          assert (labelSymbol->codeSize == 0);
          /* Either we have an increase in code/data in our current area, either
             we have an increase in storage map, either non of the previous (like
	     with "<lbl> * <value>" input).  */
	  if (areaCurrentSymbol->value.Data.Int.i - startOffset != 0)
	    labelSymbol->codeSize = areaCurrentSymbol->value.Data.Int.i - startOffset;
	  else
	    {
	      codeInit ();
	      codeValue (storageValue (), false);
	      codeValue (&startStorage, false);
	      codeOperator (Op_sub);
	      const Value *value = codeEval (ValueInt, NULL);
	      if (value->Tag == ValueInt)
		labelSymbol->codeSize = value->Data.Int.i;
	      else
		error (ErrorError, "Failed to determine label size");
	    }
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
    errorAbort ("Skipping extra characters '%s'", inputRest ());
}
