/* Generated automatically by the program `genflags'
   from the machine description file `md'.  */

#ifndef GCC_INSN_FLAGS_H
#define GCC_INSN_FLAGS_H

#define HAVE_incscc (TARGET_ARM)
#define HAVE_addsf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_adddf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_addxf3 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_decscc (TARGET_ARM)
#define HAVE_subsf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_subdf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_subxf3 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_mulsidi3 (TARGET_ARM && arm_fast_multiply)
#define HAVE_umulsidi3 (TARGET_ARM && arm_fast_multiply)
#define HAVE_smulsi3_highpart (TARGET_ARM && arm_fast_multiply)
#define HAVE_umulsi3_highpart (TARGET_ARM && arm_fast_multiply)
#define HAVE_mulhisi3 (TARGET_ARM && arm_is_xscale)
#define HAVE_mulsf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_muldf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_mulxf3 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_divsf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_divdf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_divxf3 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_modsf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_moddf3 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_modxf3 (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_anddi3 (TARGET_ARM)
#define HAVE_andsi_notsi_si (TARGET_ARM)
#define HAVE_bicsi3 (TARGET_THUMB)
#define HAVE_andsi_not_shiftsi_si (TARGET_ARM)
#define HAVE_iordi3 (TARGET_ARM)
#define HAVE_xordi3 (TARGET_ARM)
#define HAVE_smaxsi3 (TARGET_ARM)
#define HAVE_sminsi3 (TARGET_ARM)
#define HAVE_umaxsi3 (TARGET_ARM)
#define HAVE_uminsi3 (TARGET_ARM)
#define HAVE_negsf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_negdf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_negxf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_abssi2 (TARGET_ARM)
#define HAVE_abssf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_absdf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_absxf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_sqrtsf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_sqrtdf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_sqrtxf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_one_cmpldi2 (TARGET_ARM)
#define HAVE_floatsisf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_floatsidf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_floatsixf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_fix_truncsfsi2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_fix_truncdfsi2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_fix_truncxfsi2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_truncdfsf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_truncxfsf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_truncxfdf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_zero_extendsidi2 (TARGET_ARM)
#define HAVE_zero_extendqidi2 (TARGET_ARM)
#define HAVE_extendsidi2 (TARGET_ARM)
#define HAVE_extendsfdf2 (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_extendsfxf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_extenddfxf2 (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_pic_load_addr_arm (TARGET_ARM && flag_pic)
#define HAVE_pic_load_addr_thumb (TARGET_THUMB && flag_pic)
#define HAVE_pic_add_dot_plus_four (TARGET_THUMB && flag_pic)
#define HAVE_pic_add_dot_plus_eight (TARGET_ARM && flag_pic)
#define HAVE_rotated_loadsi (TARGET_ARM && (!TARGET_MMU_TRAPS))
#define HAVE_thumb_movhi_clobber (TARGET_THUMB)
#define HAVE_movmem12b (TARGET_THUMB)
#define HAVE_movmem8b (TARGET_THUMB)
#define HAVE_cbranchsi4 (TARGET_THUMB)
#define HAVE_return (TARGET_ARM && USE_RETURN_INSN (FALSE))
#define HAVE_blockage (TARGET_EITHER)
#define HAVE_casesi_internal (TARGET_ARM)
#define HAVE_nop (TARGET_EITHER)
#define HAVE_movcond (TARGET_ARM)
#define HAVE_sibcall_epilogue (TARGET_ARM)
#define HAVE_align_4 (TARGET_EITHER)
#define HAVE_consttable_end (TARGET_EITHER)
#define HAVE_consttable_1 (TARGET_THUMB)
#define HAVE_consttable_2 (TARGET_THUMB)
#define HAVE_consttable_4 (TARGET_EITHER)
#define HAVE_consttable_8 (TARGET_EITHER)
#define HAVE_tablejump (TARGET_THUMB)
#define HAVE_clz (TARGET_ARM && arm_arch5)
#define HAVE_prologue_use 1
#define HAVE_adddi3 (TARGET_EITHER)
#define HAVE_addsi3 (TARGET_EITHER)
#define HAVE_subdi3 (TARGET_EITHER)
#define HAVE_subsi3 (TARGET_EITHER)
#define HAVE_mulsi3 (TARGET_EITHER)
#define HAVE_andsi3 (TARGET_EITHER)
#define HAVE_insv (TARGET_ARM)
#define HAVE_iorsi3 (TARGET_EITHER)
#define HAVE_xorsi3 (TARGET_EITHER)
#define HAVE_ashlsi3 (TARGET_EITHER)
#define HAVE_ashrsi3 (TARGET_EITHER)
#define HAVE_lshrsi3 (TARGET_EITHER)
#define HAVE_rotlsi3 (TARGET_ARM)
#define HAVE_rotrsi3 (TARGET_EITHER)
#define HAVE_extzv (TARGET_THUMB)
#define HAVE_negdi2 (TARGET_EITHER)
#define HAVE_negsi2 (TARGET_EITHER)
#define HAVE_one_cmplsi2 (TARGET_EITHER)
#define HAVE_zero_extendhisi2 (TARGET_EITHER)
#define HAVE_zero_extendqisi2 (TARGET_EITHER)
#define HAVE_extendhisi2 (TARGET_EITHER)
#define HAVE_extendhisi2_mem (TARGET_ARM)
#define HAVE_extendqihi2 (TARGET_ARM)
#define HAVE_extendqisi2 (TARGET_EITHER)
#define HAVE_movdi (TARGET_EITHER)
#define HAVE_movsi (TARGET_EITHER)
#define HAVE_movaddr (TARGET_ARM)
#define HAVE_pic_load_addr_based (TARGET_ARM && flag_pic)
#define HAVE_builtin_setjmp_receiver (flag_pic)
#define HAVE_storehi (TARGET_ARM)
#define HAVE_storehi_bigend (TARGET_ARM)
#define HAVE_storeinthi (TARGET_ARM)
#define HAVE_storehi_single_op (TARGET_ARM && arm_arch4)
#define HAVE_movhi (TARGET_EITHER)
#define HAVE_movhi_bytes (TARGET_ARM)
#define HAVE_movhi_bigend (TARGET_ARM)
#define HAVE_reload_outhi (TARGET_EITHER)
#define HAVE_reload_inhi (TARGET_THUMB || (TARGET_ARM && TARGET_MMU_TRAPS))
#define HAVE_movqi (TARGET_EITHER)
#define HAVE_movsf (TARGET_EITHER)
#define HAVE_movdf (TARGET_EITHER)
#define HAVE_reload_outdf (TARGET_ARM)
#define HAVE_movxf (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_load_multiple (TARGET_ARM)
#define HAVE_store_multiple (TARGET_ARM)
#define HAVE_movstrqi (TARGET_EITHER)
#define HAVE_cmpsi (TARGET_ARM)
#define HAVE_cmpsf (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_cmpdf (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_cmpxf (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
#define HAVE_beq (TARGET_ARM)
#define HAVE_bne (TARGET_ARM)
#define HAVE_bgt (TARGET_ARM)
#define HAVE_ble (TARGET_ARM)
#define HAVE_bge (TARGET_ARM)
#define HAVE_blt (TARGET_ARM)
#define HAVE_bgtu (TARGET_ARM)
#define HAVE_bleu (TARGET_ARM)
#define HAVE_bgeu (TARGET_ARM)
#define HAVE_bltu (TARGET_ARM)
#define HAVE_bunordered (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_bordered (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_bungt (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_bunlt (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_bunge (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_bunle (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_buneq (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_bltgt (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_seq (TARGET_ARM)
#define HAVE_sne (TARGET_ARM)
#define HAVE_sgt (TARGET_ARM)
#define HAVE_sle (TARGET_ARM)
#define HAVE_sge (TARGET_ARM)
#define HAVE_slt (TARGET_ARM)
#define HAVE_sgtu (TARGET_ARM)
#define HAVE_sleu (TARGET_ARM)
#define HAVE_sgeu (TARGET_ARM)
#define HAVE_sltu (TARGET_ARM)
#define HAVE_sunordered (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_sordered (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_sungt (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_sunge (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_sunlt (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_sunle (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_movsicc (TARGET_ARM)
#define HAVE_movsfcc (TARGET_ARM)
#define HAVE_movdfcc (TARGET_ARM && TARGET_HARD_FLOAT)
#define HAVE_jump (TARGET_EITHER)
#define HAVE_call (TARGET_EITHER)
#define HAVE_call_value (TARGET_EITHER)
#define HAVE_sibcall (TARGET_ARM)
#define HAVE_sibcall_value (TARGET_ARM)
#define HAVE_untyped_call (TARGET_ARM)
#define HAVE_casesi (TARGET_ARM)
#define HAVE_indirect_jump (TARGET_EITHER)
#define HAVE_prologue (TARGET_EITHER)
#define HAVE_epilogue (TARGET_EITHER)
#define HAVE_eh_epilogue (TARGET_EITHER)
#define HAVE_ffssi2 (TARGET_ARM && arm_arch5)
struct rtx_def;
extern struct rtx_def *gen_incscc                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_addsf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_adddf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_addxf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_decscc                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_subsf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_subdf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_subxf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_mulsidi3                PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_umulsidi3               PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_smulsi3_highpart        PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_umulsi3_highpart        PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_mulhisi3                PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_mulsf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_muldf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_mulxf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_divsf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_divdf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_divxf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_modsf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_moddf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_modxf3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_anddi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_andsi_notsi_si          PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_bicsi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_andsi_not_shiftsi_si    PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_iordi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_xordi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_smaxsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_sminsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_umaxsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_uminsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_negsf2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_negdf2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_negxf2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_abssi2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_abssf2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_absdf2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_absxf2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_sqrtsf2                 PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_sqrtdf2                 PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_sqrtxf2                 PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_one_cmpldi2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_floatsisf2              PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_floatsidf2              PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_floatsixf2              PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_fix_truncsfsi2          PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_fix_truncdfsi2          PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_fix_truncxfsi2          PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_truncdfsf2              PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_truncxfsf2              PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_truncxfdf2              PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_zero_extendsidi2        PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_zero_extendqidi2        PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extendsidi2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extendsfdf2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extendsfxf2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extenddfxf2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_pic_load_addr_arm       PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_pic_load_addr_thumb     PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_pic_add_dot_plus_four   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_pic_add_dot_plus_eight  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_rotated_loadsi          PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_thumb_movhi_clobber     PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movmem12b               PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movmem8b                PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_cbranchsi4              PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_return                  PARAMS ((void));
extern struct rtx_def *gen_blockage                PARAMS ((void));
extern struct rtx_def *gen_casesi_internal         PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_nop                     PARAMS ((void));
extern struct rtx_def *gen_movcond                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_sibcall_epilogue        PARAMS ((void));
extern struct rtx_def *gen_align_4                 PARAMS ((void));
extern struct rtx_def *gen_consttable_end          PARAMS ((void));
extern struct rtx_def *gen_consttable_1            PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_consttable_2            PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_consttable_4            PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_consttable_8            PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_tablejump               PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_clz                     PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_prologue_use            PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_adddi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_addsi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_subdi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_subsi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_mulsi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_andsi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_insv                    PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_iorsi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_xorsi3                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_ashlsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_ashrsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_lshrsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_rotlsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_rotrsi3                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extzv                   PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_negdi2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_negsi2                  PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_one_cmplsi2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_zero_extendhisi2        PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_zero_extendqisi2        PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extendhisi2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extendhisi2_mem         PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extendqihi2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_extendqisi2             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movdi                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movsi                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movaddr                 PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_pic_load_addr_based     PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_builtin_setjmp_receiver PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_storehi                 PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_storehi_bigend          PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_storeinthi              PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_storehi_single_op       PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movhi                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movhi_bytes             PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movhi_bigend            PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_reload_outhi            PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_reload_inhi             PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movqi                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movsf                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movdf                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_reload_outdf            PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movxf                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_load_multiple           PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_store_multiple          PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movstrqi                PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_cmpsi                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_cmpsf                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_cmpdf                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_cmpxf                   PARAMS ((struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_beq                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bne                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bgt                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_ble                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bge                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_blt                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bgtu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bleu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bgeu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bltu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bunordered              PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bordered                PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bungt                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bunlt                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bunge                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bunle                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_buneq                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_bltgt                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_seq                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sne                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sgt                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sle                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sge                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_slt                     PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sgtu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sleu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sgeu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sltu                    PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sunordered              PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sordered                PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sungt                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sunge                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sunlt                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_sunle                   PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_movsicc                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movsfcc                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_movdfcc                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_jump                    PARAMS ((struct rtx_def *));
#define GEN_CALL(A, B, C, D) gen_call ((A), (B), (C))
extern struct rtx_def *gen_call                    PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
#define GEN_CALL_VALUE(A, B, C, D, E) gen_call_value ((A), (B), (C), (D))
extern struct rtx_def *gen_call_value              PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
#define GEN_SIBCALL(A, B, C, D) gen_sibcall ((A), (B), (C))
extern struct rtx_def *gen_sibcall                 PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
#define GEN_SIBCALL_VALUE(A, B, C, D, E) gen_sibcall_value ((A), (B), (C), (D))
extern struct rtx_def *gen_sibcall_value           PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_untyped_call            PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_casesi                  PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_indirect_jump           PARAMS ((struct rtx_def *));
extern struct rtx_def *gen_prologue                PARAMS ((void));
extern struct rtx_def *gen_epilogue                PARAMS ((void));
extern struct rtx_def *gen_eh_epilogue             PARAMS ((struct rtx_def *, struct rtx_def *, struct rtx_def *));
extern struct rtx_def *gen_ffssi2                  PARAMS ((struct rtx_def *, struct rtx_def *));

#endif /* GCC_INSN_FLAGS_H */
