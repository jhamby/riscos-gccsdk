/* Generated automaticaly by the program `gengenrtl'
   from the RTL description file `rtl.def' */

extern rtx gen_rtx_fmt_ee PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtx arg1));
extern rtx gen_rtx_fmt_ue PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtx arg1));
extern rtx gen_rtx_fmt_iss PROTO((RTX_CODE, enum machine_mode mode, int arg0, char * arg1, char * arg2));
extern rtx gen_rtx_fmt_is PROTO((RTX_CODE, enum machine_mode mode, int arg0, char * arg1));
extern rtx gen_rtx_fmt_i PROTO((RTX_CODE, enum machine_mode mode, int arg0));
extern rtx gen_rtx_fmt_isE PROTO((RTX_CODE, enum machine_mode mode, int arg0, char * arg1, rtvec arg2));
extern rtx gen_rtx_fmt_iE PROTO((RTX_CODE, enum machine_mode mode, int arg0, rtvec arg1));
extern rtx gen_rtx_fmt_s PROTO((RTX_CODE, enum machine_mode mode, char * arg0));
extern rtx gen_rtx_fmt_Ess PROTO((RTX_CODE, enum machine_mode mode, rtvec arg0, char * arg1, char * arg2));
extern rtx gen_rtx_fmt_sEss PROTO((RTX_CODE, enum machine_mode mode, char * arg0, rtvec arg1, char * arg2, char * arg3));
extern rtx gen_rtx_fmt_eE PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtvec arg1));
extern rtx gen_rtx_fmt_E PROTO((RTX_CODE, enum machine_mode mode, rtvec arg0));
extern rtx gen_rtx_fmt_e PROTO((RTX_CODE, enum machine_mode mode, rtx arg0));
extern rtx gen_rtx_fmt_sse PROTO((RTX_CODE, enum machine_mode mode, char * arg0, char * arg1, rtx arg2));
extern rtx gen_rtx_fmt_ss PROTO((RTX_CODE, enum machine_mode mode, char * arg0, char * arg1));
extern rtx gen_rtx_fmt_sE PROTO((RTX_CODE, enum machine_mode mode, char * arg0, rtvec arg1));
extern rtx gen_rtx_fmt_iuueiee PROTO((RTX_CODE, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, rtx arg3, int arg4, rtx arg5, rtx arg6));
extern rtx gen_rtx_fmt_iuueiee0 PROTO((RTX_CODE, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, rtx arg3, int arg4, rtx arg5, rtx arg6));
extern rtx gen_rtx_fmt_iuueieee PROTO((RTX_CODE, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, rtx arg3, int arg4, rtx arg5, rtx arg6, rtx arg7));
extern rtx gen_rtx_fmt_iuu PROTO((RTX_CODE, enum machine_mode mode, int arg0, rtx arg1, rtx arg2));
extern rtx gen_rtx_fmt_iuuis00 PROTO((RTX_CODE, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, int arg3, char * arg4));
extern rtx gen_rtx_fmt_iuuuiiiiiieeiiEeEssE PROTO((RTX_CODE, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, rtx arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, rtx arg10, rtx arg11, int arg12, int arg13, rtvec arg14, rtx arg15, rtvec arg16, char * arg17, char * arg18, rtvec arg19));
extern rtx gen_rtx_fmt_ssiEEsi PROTO((RTX_CODE, enum machine_mode mode, char * arg0, char * arg1, int arg2, rtvec arg3, rtvec arg4, char * arg5, int arg6));
extern rtx gen_rtx_fmt_Ei PROTO((RTX_CODE, enum machine_mode mode, rtvec arg0, int arg1));
extern rtx gen_rtx_fmt_eEeei PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtvec arg1, rtx arg2, rtx arg3, int arg4));
extern rtx gen_rtx_fmt_ PROTO((RTX_CODE, enum machine_mode mode));
extern rtx gen_rtx_fmt_w PROTO((RTX_CODE, enum machine_mode mode, HOST_WIDE_INT arg0));
extern rtx gen_rtx_fmt_e0ww PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, HOST_WIDE_INT arg1, HOST_WIDE_INT arg2));
extern rtx gen_rtx_fmt_i0 PROTO((RTX_CODE, enum machine_mode mode, int arg0));
extern rtx gen_rtx_fmt_0 PROTO((RTX_CODE, enum machine_mode mode));
extern rtx gen_rtx_fmt_ei PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, int arg1));
extern rtx gen_rtx_fmt_e0 PROTO((RTX_CODE, enum machine_mode mode, rtx arg0));
extern rtx gen_rtx_fmt_u00 PROTO((RTX_CODE, enum machine_mode mode, rtx arg0));
extern rtx gen_rtx_fmt_ei0 PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, int arg1));
extern rtx gen_rtx_fmt_eeeee PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtx arg1, rtx arg2, rtx arg3, rtx arg4));
extern rtx gen_rtx_fmt_eee PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtx arg1, rtx arg2));
extern rtx gen_rtx_fmt_Ee PROTO((RTX_CODE, enum machine_mode mode, rtvec arg0, rtx arg1));
extern rtx gen_rtx_fmt_uuEiiiiiibbii PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtx arg1, rtvec arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, void * arg9, void * arg10, int arg11, int arg12));
extern rtx gen_rtx_fmt_iiiiiiiitt PROTO((RTX_CODE, enum machine_mode mode, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, void * arg8, void * arg9));
extern rtx gen_rtx_fmt_eti PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, void * arg1, int arg2));
extern rtx gen_rtx_fmt_bi PROTO((RTX_CODE, enum machine_mode mode, void * arg0, int arg1));
extern rtx gen_rtx_fmt_uuuu PROTO((RTX_CODE, enum machine_mode mode, rtx arg0, rtx arg1, rtx arg2, rtx arg3));

#define gen_rtx_EXPR_LIST(mode, arg0, arg1)   gen_rtx_fmt_ee(EXPR_LIST,(mode),(arg0),(arg1))
#define gen_rtx_INSN_LIST(mode, arg0, arg1)   gen_rtx_fmt_ue(INSN_LIST,(mode),(arg0),(arg1))
#define gen_rtx_MATCH_OPERAND(mode, arg0, arg1, arg2)   gen_rtx_fmt_iss(MATCH_OPERAND,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_MATCH_SCRATCH(mode, arg0, arg1)   gen_rtx_fmt_is(MATCH_SCRATCH,(mode),(arg0),(arg1))
#define gen_rtx_MATCH_DUP(mode, arg0)   gen_rtx_fmt_i(MATCH_DUP,(mode),(arg0))
#define gen_rtx_MATCH_OPERATOR(mode, arg0, arg1, arg2)   gen_rtx_fmt_isE(MATCH_OPERATOR,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_MATCH_PARALLEL(mode, arg0, arg1, arg2)   gen_rtx_fmt_isE(MATCH_PARALLEL,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_MATCH_OP_DUP(mode, arg0, arg1)   gen_rtx_fmt_iE(MATCH_OP_DUP,(mode),(arg0),(arg1))
#define gen_rtx_MATCH_PAR_DUP(mode, arg0, arg1)   gen_rtx_fmt_iE(MATCH_PAR_DUP,(mode),(arg0),(arg1))
#define gen_rtx_MATCH_INSN(mode, arg0)   gen_rtx_fmt_s(MATCH_INSN,(mode),(arg0))
#define gen_rtx_MATCH_INSN2(mode, arg0, arg1)   gen_rtx_fmt_is(MATCH_INSN2,(mode),(arg0),(arg1))
#define gen_rtx_DEFINE_COMBINE(mode, arg0, arg1, arg2)   gen_rtx_fmt_Ess(DEFINE_COMBINE,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_DEFINE_EXPAND(mode, arg0, arg1, arg2, arg3)   gen_rtx_fmt_sEss(DEFINE_EXPAND,(mode),(arg0),(arg1),(arg2),(arg3))
#define gen_rtx_DEFINE_DELAY(mode, arg0, arg1)   gen_rtx_fmt_eE(DEFINE_DELAY,(mode),(arg0),(arg1))
#define gen_rtx_SEQUENCE(mode, arg0)   gen_rtx_fmt_E(SEQUENCE,(mode),(arg0))
#define gen_rtx_ADDRESS(mode, arg0)   gen_rtx_fmt_e(ADDRESS,(mode),(arg0))
#define gen_rtx_DEFINE_ATTR(mode, arg0, arg1, arg2)   gen_rtx_fmt_sse(DEFINE_ATTR,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_ATTR(mode, arg0)   gen_rtx_fmt_s(ATTR,(mode),(arg0))
#define gen_rtx_SET_ATTR(mode, arg0, arg1)   gen_rtx_fmt_ss(SET_ATTR,(mode),(arg0),(arg1))
#define gen_rtx_SET_ATTR_ALTERNATIVE(mode, arg0, arg1)   gen_rtx_fmt_sE(SET_ATTR_ALTERNATIVE,(mode),(arg0),(arg1))
#define gen_rtx_EQ_ATTR(mode, arg0, arg1)   gen_rtx_fmt_ss(EQ_ATTR,(mode),(arg0),(arg1))
#define gen_rtx_ATTR_FLAG(mode, arg0)   gen_rtx_fmt_s(ATTR_FLAG,(mode),(arg0))
#define gen_rtx_INSN(mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   gen_rtx_fmt_iuueiee(INSN,(mode),(arg0),(arg1),(arg2),(arg3),(arg4),(arg5),(arg6))
#define gen_rtx_JUMP_INSN(mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   gen_rtx_fmt_iuueiee0(JUMP_INSN,(mode),(arg0),(arg1),(arg2),(arg3),(arg4),(arg5),(arg6))
#define gen_rtx_CALL_INSN(mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)   gen_rtx_fmt_iuueieee(CALL_INSN,(mode),(arg0),(arg1),(arg2),(arg3),(arg4),(arg5),(arg6),(arg7))
#define gen_rtx_BARRIER(mode, arg0, arg1, arg2)   gen_rtx_fmt_iuu(BARRIER,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_CODE_LABEL(mode, arg0, arg1, arg2, arg3, arg4)   gen_rtx_fmt_iuuis00(CODE_LABEL,(mode),(arg0),(arg1),(arg2),(arg3),(arg4))
#define gen_rtx_INLINE_HEADER(mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19)   gen_rtx_fmt_iuuuiiiiiieeiiEeEssE(INLINE_HEADER,(mode),(arg0),(arg1),(arg2),(arg3),(arg4),(arg5),(arg6),(arg7),(arg8),(arg9),(arg10),(arg11),(arg12),(arg13),(arg14),(arg15),(arg16),(arg17),(arg18),(arg19))
#define gen_rtx_PARALLEL(mode, arg0)   gen_rtx_fmt_E(PARALLEL,(mode),(arg0))
#define gen_rtx_ASM_INPUT(mode, arg0)   gen_rtx_fmt_s(ASM_INPUT,(mode),(arg0))
#define gen_rtx_ASM_OPERANDS(mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6)   gen_rtx_fmt_ssiEEsi(ASM_OPERANDS,(mode),(arg0),(arg1),(arg2),(arg3),(arg4),(arg5),(arg6))
#define gen_rtx_UNSPEC(mode, arg0, arg1)   gen_rtx_fmt_Ei(UNSPEC,(mode),(arg0),(arg1))
#define gen_rtx_UNSPEC_VOLATILE(mode, arg0, arg1)   gen_rtx_fmt_Ei(UNSPEC_VOLATILE,(mode),(arg0),(arg1))
#define gen_rtx_ADDR_VEC(mode, arg0)   gen_rtx_fmt_E(ADDR_VEC,(mode),(arg0))
#define gen_rtx_ADDR_DIFF_VEC(mode, arg0, arg1, arg2, arg3, arg4)   gen_rtx_fmt_eEeei(ADDR_DIFF_VEC,(mode),(arg0),(arg1),(arg2),(arg3),(arg4))
#define gen_rtx_SET(mode, arg0, arg1)   gen_rtx_fmt_ee(SET,(mode),(arg0),(arg1))
#define gen_rtx_USE(mode, arg0)   gen_rtx_fmt_e(USE,(mode),(arg0))
#define gen_rtx_CLOBBER(mode, arg0)   gen_rtx_fmt_e(CLOBBER,(mode),(arg0))
#define gen_rtx_CALL(mode, arg0, arg1)   gen_rtx_fmt_ee(CALL,(mode),(arg0),(arg1))
#define gen_rtx_RETURN(mode)   gen_rtx_fmt_(RETURN,(mode))
#define gen_rtx_TRAP_IF(mode, arg0, arg1)   gen_rtx_fmt_ee(TRAP_IF,(mode),(arg0),(arg1))
#define gen_rtx_raw_CONST_INT(mode, arg0)   gen_rtx_fmt_w(CONST_INT,(mode),(arg0))
#define gen_rtx_raw_CONST_DOUBLE(mode, arg0, arg1, arg2)   gen_rtx_fmt_e0ww(CONST_DOUBLE,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_CONST_STRING(mode, arg0)   gen_rtx_fmt_s(CONST_STRING,(mode),(arg0))
#define gen_rtx_CONST(mode, arg0)   gen_rtx_fmt_e(CONST,(mode),(arg0))
#define gen_rtx_PC(mode)   gen_rtx_fmt_(PC,(mode))
#define gen_rtx_raw_REG(mode, arg0)   gen_rtx_fmt_i0(REG,(mode),(arg0))
#define gen_rtx_SCRATCH(mode)   gen_rtx_fmt_0(SCRATCH,(mode))
#define gen_rtx_SUBREG(mode, arg0, arg1)   gen_rtx_fmt_ei(SUBREG,(mode),(arg0),(arg1))
#define gen_rtx_STRICT_LOW_PART(mode, arg0)   gen_rtx_fmt_e(STRICT_LOW_PART,(mode),(arg0))
#define gen_rtx_CONCAT(mode, arg0, arg1)   gen_rtx_fmt_ee(CONCAT,(mode),(arg0),(arg1))
#define gen_rtx_raw_MEM(mode, arg0)   gen_rtx_fmt_e0(MEM,(mode),(arg0))
#define gen_rtx_LABEL_REF(mode, arg0)   gen_rtx_fmt_u00(LABEL_REF,(mode),(arg0))
#define gen_rtx_SYMBOL_REF(mode, arg0)   gen_rtx_fmt_s(SYMBOL_REF,(mode),(arg0))
#define gen_rtx_CC0(mode)   gen_rtx_fmt_(CC0,(mode))
#define gen_rtx_ADDRESSOF(mode, arg0, arg1)   gen_rtx_fmt_ei0(ADDRESSOF,(mode),(arg0),(arg1))
#define gen_rtx_QUEUED(mode, arg0, arg1, arg2, arg3, arg4)   gen_rtx_fmt_eeeee(QUEUED,(mode),(arg0),(arg1),(arg2),(arg3),(arg4))
#define gen_rtx_IF_THEN_ELSE(mode, arg0, arg1, arg2)   gen_rtx_fmt_eee(IF_THEN_ELSE,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_COND(mode, arg0, arg1)   gen_rtx_fmt_Ee(COND,(mode),(arg0),(arg1))
#define gen_rtx_COMPARE(mode, arg0, arg1)   gen_rtx_fmt_ee(COMPARE,(mode),(arg0),(arg1))
#define gen_rtx_PLUS(mode, arg0, arg1)   gen_rtx_fmt_ee(PLUS,(mode),(arg0),(arg1))
#define gen_rtx_MINUS(mode, arg0, arg1)   gen_rtx_fmt_ee(MINUS,(mode),(arg0),(arg1))
#define gen_rtx_NEG(mode, arg0)   gen_rtx_fmt_e(NEG,(mode),(arg0))
#define gen_rtx_MULT(mode, arg0, arg1)   gen_rtx_fmt_ee(MULT,(mode),(arg0),(arg1))
#define gen_rtx_DIV(mode, arg0, arg1)   gen_rtx_fmt_ee(DIV,(mode),(arg0),(arg1))
#define gen_rtx_MOD(mode, arg0, arg1)   gen_rtx_fmt_ee(MOD,(mode),(arg0),(arg1))
#define gen_rtx_UDIV(mode, arg0, arg1)   gen_rtx_fmt_ee(UDIV,(mode),(arg0),(arg1))
#define gen_rtx_UMOD(mode, arg0, arg1)   gen_rtx_fmt_ee(UMOD,(mode),(arg0),(arg1))
#define gen_rtx_AND(mode, arg0, arg1)   gen_rtx_fmt_ee(AND,(mode),(arg0),(arg1))
#define gen_rtx_IOR(mode, arg0, arg1)   gen_rtx_fmt_ee(IOR,(mode),(arg0),(arg1))
#define gen_rtx_XOR(mode, arg0, arg1)   gen_rtx_fmt_ee(XOR,(mode),(arg0),(arg1))
#define gen_rtx_NOT(mode, arg0)   gen_rtx_fmt_e(NOT,(mode),(arg0))
#define gen_rtx_ASHIFT(mode, arg0, arg1)   gen_rtx_fmt_ee(ASHIFT,(mode),(arg0),(arg1))
#define gen_rtx_ROTATE(mode, arg0, arg1)   gen_rtx_fmt_ee(ROTATE,(mode),(arg0),(arg1))
#define gen_rtx_ASHIFTRT(mode, arg0, arg1)   gen_rtx_fmt_ee(ASHIFTRT,(mode),(arg0),(arg1))
#define gen_rtx_LSHIFTRT(mode, arg0, arg1)   gen_rtx_fmt_ee(LSHIFTRT,(mode),(arg0),(arg1))
#define gen_rtx_ROTATERT(mode, arg0, arg1)   gen_rtx_fmt_ee(ROTATERT,(mode),(arg0),(arg1))
#define gen_rtx_SMIN(mode, arg0, arg1)   gen_rtx_fmt_ee(SMIN,(mode),(arg0),(arg1))
#define gen_rtx_SMAX(mode, arg0, arg1)   gen_rtx_fmt_ee(SMAX,(mode),(arg0),(arg1))
#define gen_rtx_UMIN(mode, arg0, arg1)   gen_rtx_fmt_ee(UMIN,(mode),(arg0),(arg1))
#define gen_rtx_UMAX(mode, arg0, arg1)   gen_rtx_fmt_ee(UMAX,(mode),(arg0),(arg1))
#define gen_rtx_PRE_DEC(mode, arg0)   gen_rtx_fmt_e(PRE_DEC,(mode),(arg0))
#define gen_rtx_PRE_INC(mode, arg0)   gen_rtx_fmt_e(PRE_INC,(mode),(arg0))
#define gen_rtx_POST_DEC(mode, arg0)   gen_rtx_fmt_e(POST_DEC,(mode),(arg0))
#define gen_rtx_POST_INC(mode, arg0)   gen_rtx_fmt_e(POST_INC,(mode),(arg0))
#define gen_rtx_PRE_MODIFY(mode, arg0, arg1)   gen_rtx_fmt_ee(PRE_MODIFY,(mode),(arg0),(arg1))
#define gen_rtx_POST_MODIFY(mode, arg0, arg1)   gen_rtx_fmt_ee(POST_MODIFY,(mode),(arg0),(arg1))
#define gen_rtx_NE(mode, arg0, arg1)   gen_rtx_fmt_ee(NE,(mode),(arg0),(arg1))
#define gen_rtx_EQ(mode, arg0, arg1)   gen_rtx_fmt_ee(EQ,(mode),(arg0),(arg1))
#define gen_rtx_GE(mode, arg0, arg1)   gen_rtx_fmt_ee(GE,(mode),(arg0),(arg1))
#define gen_rtx_GT(mode, arg0, arg1)   gen_rtx_fmt_ee(GT,(mode),(arg0),(arg1))
#define gen_rtx_LE(mode, arg0, arg1)   gen_rtx_fmt_ee(LE,(mode),(arg0),(arg1))
#define gen_rtx_LT(mode, arg0, arg1)   gen_rtx_fmt_ee(LT,(mode),(arg0),(arg1))
#define gen_rtx_GEU(mode, arg0, arg1)   gen_rtx_fmt_ee(GEU,(mode),(arg0),(arg1))
#define gen_rtx_GTU(mode, arg0, arg1)   gen_rtx_fmt_ee(GTU,(mode),(arg0),(arg1))
#define gen_rtx_LEU(mode, arg0, arg1)   gen_rtx_fmt_ee(LEU,(mode),(arg0),(arg1))
#define gen_rtx_LTU(mode, arg0, arg1)   gen_rtx_fmt_ee(LTU,(mode),(arg0),(arg1))
#define gen_rtx_SIGN_EXTEND(mode, arg0)   gen_rtx_fmt_e(SIGN_EXTEND,(mode),(arg0))
#define gen_rtx_ZERO_EXTEND(mode, arg0)   gen_rtx_fmt_e(ZERO_EXTEND,(mode),(arg0))
#define gen_rtx_TRUNCATE(mode, arg0)   gen_rtx_fmt_e(TRUNCATE,(mode),(arg0))
#define gen_rtx_FLOAT_EXTEND(mode, arg0)   gen_rtx_fmt_e(FLOAT_EXTEND,(mode),(arg0))
#define gen_rtx_FLOAT_TRUNCATE(mode, arg0)   gen_rtx_fmt_e(FLOAT_TRUNCATE,(mode),(arg0))
#define gen_rtx_FLOAT(mode, arg0)   gen_rtx_fmt_e(FLOAT,(mode),(arg0))
#define gen_rtx_FIX(mode, arg0)   gen_rtx_fmt_e(FIX,(mode),(arg0))
#define gen_rtx_UNSIGNED_FLOAT(mode, arg0)   gen_rtx_fmt_e(UNSIGNED_FLOAT,(mode),(arg0))
#define gen_rtx_UNSIGNED_FIX(mode, arg0)   gen_rtx_fmt_e(UNSIGNED_FIX,(mode),(arg0))
#define gen_rtx_ABS(mode, arg0)   gen_rtx_fmt_e(ABS,(mode),(arg0))
#define gen_rtx_SQRT(mode, arg0)   gen_rtx_fmt_e(SQRT,(mode),(arg0))
#define gen_rtx_FFS(mode, arg0)   gen_rtx_fmt_e(FFS,(mode),(arg0))
#define gen_rtx_SIGN_EXTRACT(mode, arg0, arg1, arg2)   gen_rtx_fmt_eee(SIGN_EXTRACT,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_ZERO_EXTRACT(mode, arg0, arg1, arg2)   gen_rtx_fmt_eee(ZERO_EXTRACT,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_HIGH(mode, arg0)   gen_rtx_fmt_e(HIGH,(mode),(arg0))
#define gen_rtx_LO_SUM(mode, arg0, arg1)   gen_rtx_fmt_ee(LO_SUM,(mode),(arg0),(arg1))
#define gen_rtx_RANGE_INFO(mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12)   gen_rtx_fmt_uuEiiiiiibbii(RANGE_INFO,(mode),(arg0),(arg1),(arg2),(arg3),(arg4),(arg5),(arg6),(arg7),(arg8),(arg9),(arg10),(arg11),(arg12))
#define gen_rtx_RANGE_REG(mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)   gen_rtx_fmt_iiiiiiiitt(RANGE_REG,(mode),(arg0),(arg1),(arg2),(arg3),(arg4),(arg5),(arg6),(arg7),(arg8),(arg9))
#define gen_rtx_RANGE_VAR(mode, arg0, arg1, arg2)   gen_rtx_fmt_eti(RANGE_VAR,(mode),(arg0),(arg1),(arg2))
#define gen_rtx_RANGE_LIVE(mode, arg0, arg1)   gen_rtx_fmt_bi(RANGE_LIVE,(mode),(arg0),(arg1))
#define gen_rtx_CONSTANT_P_RTX(mode, arg0)   gen_rtx_fmt_e(CONSTANT_P_RTX,(mode),(arg0))
#define gen_rtx_CALL_PLACEHOLDER(mode, arg0, arg1, arg2, arg3)   gen_rtx_fmt_uuuu(CALL_PLACEHOLDER,(mode),(arg0),(arg1),(arg2),(arg3))
