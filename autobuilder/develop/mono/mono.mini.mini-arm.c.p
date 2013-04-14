--- mono/mini/mini-arm.c.orig	2013-03-08 04:31:49.000000000 +0000
+++ mono/mini/mini-arm.c	2013-04-14 14:47:26.000000000 +0100
@@ -65,7 +65,7 @@
 #define mono_mini_arch_unlock() LeaveCriticalSection (&mini_arch_mutex)
 static CRITICAL_SECTION mini_arch_mutex;
 
-static int v5_supported = 0;
+int v5_supported = 0;
 static int v6_supported = 0;
 static int v7_supported = 0;
 static int thumb_supported = 0;
@@ -805,7 +805,7 @@
 mono_arch_cpu_optimizations (guint32 *exclude_mask)
 {
 	guint32 opts = 0;
-	const char *cpu_arch = getenv ("MONO_CPU_ARCH");
+	const char *cpu_arch = getenv ("Mono$CPUArch");
 	if (cpu_arch != NULL) {
 		thumb_supported = strstr (cpu_arch, "thumb") != NULL;
 		if (strncmp (cpu_arch, "armv", 4) == 0) {
@@ -951,6 +951,15 @@
 	regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V1));
 	regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V2));
 	regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V3));
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	if (!(cfg->compile_aot || cfg->uses_rgctx_reg || COMPILE_LLVM (cfg)))
+		/* V4 is reserved for passing the vtable/rgctx/IMT method */
+		regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V4));
+	if ((cfg->flags & MONO_CFG_HAS_CALLS) == 0)
+		/* V5 is corrupted by calls into shared libraries */
+		/* FIXME: Perhaps v5 could be added to MONO_ARCH_CALLEE_REGS? */
+		regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V5));
+#else
 	if (darwin)
 		/* V4=R7 is used as a frame pointer, but V7=R10 is preserved */
 		regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V7));
@@ -959,6 +968,7 @@
 	if (!(cfg->compile_aot || cfg->uses_rgctx_reg || COMPILE_LLVM (cfg)))
 		/* V5 is reserved for passing the vtable/rgctx/IMT method */
 		regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V5));
+#endif
 	/*regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V6));*/
 	/*regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V7));*/
 
@@ -1405,6 +1415,11 @@
 	cfg->arch.omit_fp = TRUE;
 	cfg->arch.omit_fp_computed = TRUE;
 
+#ifdef __riscos__
+	if (cfg->method->save_lmf)
+		cfg->arch.omit_fp = FALSE;
+#endif
+
 	if (cfg->disable_omit_fp)
 		cfg->arch.omit_fp = FALSE;
 	if (!debug_omit_fp ())
@@ -1488,9 +1503,13 @@
 		cfg->used_int_regs |= 1 << cfg->frame_reg;
 
 	if (cfg->compile_aot || cfg->uses_rgctx_reg || COMPILE_LLVM (cfg))
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		/* V4 is reserved for passing the vtable/rgctx/IMT method */
+		cfg->used_int_regs |= (1 << ARMREG_V4);
+#else
 		/* V5 is reserved for passing the vtable/rgctx/IMT method */
 		cfg->used_int_regs |= (1 << ARMREG_V5);
-
+#endif
 	offset = 0;
 	curinst = 0;
 	if (!MONO_TYPE_ISSTRUCT (sig->ret) && !cinfo->vtype_retaddr) {
@@ -3706,7 +3725,17 @@
 		case OP_STOREI2_MEMBASE_IMM:
 			code = mono_arm_emit_load_imm (code, ARMREG_LR, ins->inst_imm & 0xFFFF);
 			g_assert (arm_is_imm8 (ins->inst_offset));
+			if (v5_supported) {
 			ARM_STRH_IMM (code, ARMREG_LR, ins->inst_destbasereg, ins->inst_offset);
+			} else {
+				// TODO: I haven't yet found the right C# source code that will generate
+				// this code sequence and allow it to be validated.
+				ARM_ADD_REG_IMM8 (code, ARMREG_IP, ins->inst_destbasereg, ins->inst_offset);
+				ARM_STRB_IMM (code, ARMREG_LR, ARMREG_IP, 0);
+				// I don't think it's necessary to preserve LR here.
+				ARM_SHR_IMM (code, ARMREG_LR, ARMREG_LR, 8);
+				ARM_STRB_IMM (code, ARMREG_LR, ARMREG_IP, 1);
+			}
 			break;
 		case OP_STORE_MEMBASE_IMM:
 		case OP_STOREI4_MEMBASE_IMM:
@@ -3720,7 +3749,13 @@
 			break;
 		case OP_STOREI2_MEMBASE_REG:
 			g_assert (arm_is_imm8 (ins->inst_offset));
+			if (v5_supported) {
 			ARM_STRH_IMM (code, ins->sreg1, ins->inst_destbasereg, ins->inst_offset);
+			} else {
+				ARM_STRB_IMM (code, ins->sreg1, ins->inst_destbasereg, ins->inst_offset);
+				ARM_SHR_IMM (code, ARMREG_LR, ins->sreg1, 8);
+				ARM_STRB_IMM (code, ARMREG_LR, ins->inst_destbasereg, ins->inst_offset + 1);
+			}
 			break;
 		case OP_STORE_MEMBASE_REG:
 		case OP_STOREI4_MEMBASE_REG:
@@ -3736,7 +3771,14 @@
 			ARM_STRB_REG_REG (code, ins->sreg1, ins->inst_destbasereg, ins->sreg2);
 			break;
 		case OP_STOREI2_MEMINDEX:
+			if (v5_supported) {
 			ARM_STRH_REG_REG (code, ins->sreg1, ins->inst_destbasereg, ins->sreg2);
+			} else {
+				ARM_ADD_REG_REG (code, ARMREG_LR, ins->inst_destbasereg, ins->sreg2);
+				ARM_STRB_IMM (code, ins->sreg1, ARMREG_LR, 0);
+				ARM_SHR_IMM (code, ARMREG_IP, ins->sreg1, 8);
+				ARM_STRB_IMM (code, ARMREG_IP, ARMREG_LR, 1);
+			}
 			break;
 		case OP_STORE_MEMINDEX:
 		case OP_STOREI4_MEMINDEX:
@@ -3751,16 +3793,39 @@
 			ARM_LDR_REG_REG (code, ins->dreg, ins->inst_basereg, ins->sreg2);
 			break;
 		case OP_LOADI1_MEMINDEX:
+			if (v5_supported) {
 			ARM_LDRSB_REG_REG (code, ins->dreg, ins->inst_basereg, ins->sreg2);
+			} else {
+				ARM_LDRB_REG_REG (code, ins->dreg, ins->inst_basereg, ins->sreg2);
+				ARM_SHL_IMM (code, ins->dreg, ins->dreg, 24);
+				ARM_SAR_IMM (code, ins->dreg, ins->dreg, 24);
+			}
 			break;
 		case OP_LOADU1_MEMINDEX:
 			ARM_LDRB_REG_REG (code, ins->dreg, ins->inst_basereg, ins->sreg2);
 			break;
 		case OP_LOADI2_MEMINDEX:
+			if (v5_supported) {
 			ARM_LDRSH_REG_REG (code, ins->dreg, ins->inst_basereg, ins->sreg2);
+			} else {
+				ARM_ADD_REG_REG (code, ARMREG_LR, ins->inst_basereg, ins->sreg2);
+				ARM_LDRB_IMM (code, ins->dreg, ARMREG_LR, 0);
+				ARM_LDRB_IMM (code, ARMREG_LR, ARMREG_LR, 1);
+				ARM_ORR_REG_IMMSHIFT (code, ins->dreg, ins->dreg, ARMREG_LR, ARMSHIFT_LSL, 8);
+				/* Sign extend */
+				ARM_SHL_IMM (code, ins->dreg, ins->dreg, 16);
+				ARM_SAR_IMM (code, ins->dreg, ins->dreg, 16);
+			}
 			break;
 		case OP_LOADU2_MEMINDEX:
+			if (v5_supported) {
 			ARM_LDRH_REG_REG (code, ins->dreg, ins->inst_basereg, ins->sreg2);
+			} else {
+				ARM_ADD_REG_REG (code, ARMREG_LR, ins->inst_basereg, ins->sreg2);
+				ARM_LDRB_IMM (code, ins->dreg, ARMREG_LR, 0);
+				ARM_LDRB_IMM (code, ARMREG_LR, ARMREG_LR, 1);
+				ARM_ORR_REG_IMMSHIFT (code, ins->dreg, ins->dreg, ARMREG_LR, ARMSHIFT_LSL, 8);
+			}
 			break;
 		case OP_LOAD_MEMBASE:
 		case OP_LOADI4_MEMBASE:
@@ -3775,7 +3840,13 @@
 			break;
 		case OP_LOADI1_MEMBASE:
 			g_assert (arm_is_imm8 (ins->inst_offset));
+			if (v5_supported) {
 			ARM_LDRSB_IMM (code, ins->dreg, ins->inst_basereg, ins->inst_offset);
+			} else {
+				ARM_LDRB_IMM (code, ins->dreg, ins->inst_basereg, ins->inst_offset);
+				ARM_SHL_IMM (code, ins->dreg, ins->dreg, 24);
+				ARM_SAR_IMM (code, ins->dreg, ins->dreg, 24);
+			}
 			break;
 		case OP_LOADU1_MEMBASE:
 			g_assert (arm_is_imm12 (ins->inst_offset));
@@ -3783,11 +3854,26 @@
 			break;
 		case OP_LOADU2_MEMBASE:
 			g_assert (arm_is_imm8 (ins->inst_offset));
+			if (v5_supported) {
 			ARM_LDRH_IMM (code, ins->dreg, ins->inst_basereg, ins->inst_offset);
+			} else {
+				ARM_LDRB_IMM (code, ARMREG_LR, ins->inst_basereg, ins->inst_offset);
+				ARM_LDRB_IMM (code, ins->dreg, ins->inst_basereg, ins->inst_offset + 1);
+				ARM_ORR_REG_IMMSHIFT (code, ins->dreg, ARMREG_LR, ins->dreg, ARMSHIFT_LSL, 8);
+			}
 			break;
 		case OP_LOADI2_MEMBASE:
 			g_assert (arm_is_imm8 (ins->inst_offset));
+			if (v5_supported) {
 			ARM_LDRSH_IMM (code, ins->dreg, ins->inst_basereg, ins->inst_offset);
+			} else {
+				ARM_LDRB_IMM (code, ARMREG_LR, ins->inst_basereg, ins->inst_offset);
+				ARM_LDRB_IMM (code, ins->dreg, ins->inst_basereg, ins->inst_offset + 1);
+				ARM_ORR_REG_IMMSHIFT (code, ins->dreg, ARMREG_LR, ins->dreg, ARMSHIFT_LSL, 8);
+				/* Sign extend */
+				ARM_SHL_IMM (code, ins->dreg, ins->dreg, 16);
+				ARM_SAR_IMM (code, ins->dreg, ins->dreg, 16);
+			}
 			break;
 		case OP_ICONV_TO_I1:
 			ARM_SHL_IMM (code, ins->dreg, ins->sreg1, 24);
@@ -5066,6 +5152,10 @@
 	mono_register_jit_icall (__aeabi_read_tp, "__aeabi_read_tp", mono_create_icall_signature ("void"), TRUE);
 #endif
 #endif
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	mono_register_jit_icall (__rt_stkovf_split_small, "__rt_stkovf_split_small", NULL, FALSE);
+	mono_register_jit_icall (__rt_stkovf_split_big, "__rt_stkovf_split_big", NULL, FALSE);
+#endif
 }
 
 #define patch_lis_ori(ip,val) do {\
@@ -5113,6 +5203,11 @@
 		}
 
 		switch (patch_info->type) {
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		case MONO_PATCH_INFO_RISCOS_STKOVF:
+			*((guint32 *)ip) |= ((((target - ip) - 8) >> 2) & 0xffffff);
+			continue;
+#endif
 		case MONO_PATCH_INFO_IP:
 			g_assert_not_reached ();
 			patch_lis_ori (ip, ip);
@@ -5158,6 +5253,114 @@
 
 #ifndef DISABLE_JIT
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+
+#include <dlfcn.h>
+
+static gboolean
+mono_stack_check_needed (gboolean leaf_function, int frame_size)
+{
+	if (leaf_function && frame_size <= 256)
+		return FALSE;
+
+	return TRUE;
+}
+
+static gboolean
+mono_stack_frame_needed (gboolean leaf_function, int frame_size)
+{
+	if (!leaf_function || mono_stack_check_needed (leaf_function, frame_size))
+		return TRUE;
+
+	return FALSE;
+}
+
+static void
+mono_arch_emit_stack_check (MonoCompile *cfg, guint8 **code_ptr, int alloc_size)
+{
+	static gpointer stkovf_split_big_func = NULL;
+	static gpointer stkovf_split_small_func = NULL;
+	guint8 *code = *code_ptr;
+	gpointer stkovf;
+
+	if (!stkovf_split_small_func) {
+		void *unixlib = dlopen ("libunixlib.so", RTLD_LOCAL);
+		if (!unixlib) {
+			fprintf (stderr,"Failed to open libunixlib.so\n");
+			exit(1);
+		}
+
+		stkovf_split_small_func = dlsym (unixlib, "__rt_stkovf_split_small");
+		stkovf_split_big_func = dlsym (unixlib, "__rt_stkovf_split_big");
+
+		dlclose (unixlib);
+
+		if (!stkovf_split_small_func || !stkovf_split_big_func) {
+			fprintf (stderr, "Failed to find stack overflow routines in libunixlib\n");
+			exit (1);
+		}
+	}
+
+	if (alloc_size >= 256) {
+		int imm8, rot_amount;
+
+		stkovf = stkovf_split_big_func;
+
+		if ((imm8 = mono_arm_is_rotated_imm8 (alloc_size, &rot_amount)) >= 0) {
+			ARM_SUB_REG_IMM (code, ARMREG_IP, ARMREG_SP, imm8, rot_amount);
+		} else {
+			gboolean first_inst = TRUE;
+			int i;
+
+			for (i = 0; i < 4; i++) {
+				if ((alloc_size >> (i * 8)) & 0xff) {
+					ARM_SUB_REG_IMM (code,
+							 ARMREG_IP,
+							 first_inst ? ARMREG_SP : ARMREG_IP,
+							 (alloc_size >> (i * 8)) & 0xff,
+							 32 - (i * 8));
+					first_inst = FALSE;
+				}
+			}
+		}
+
+		ARM_CMP_REG_REG (code, ARMREG_IP, ARMREG_R10);
+	} else {
+		stkovf = stkovf_split_small_func;
+		ARM_CMP_REG_REG (code, ARMREG_SP, ARMREG_R10);
+	}
+
+	/* The BL offset can only address +/-32MB. We may wish to generate code in a dynamic area
+	 * or the JIT may generate so much code that the BL offset overflows. Unfortunately, at this
+	 * point we don't know the final address of the code, so we can't tell if the offset will overflow.
+	 * There are a couple of alternative code sequences that can be used that do not limit the
+	 * range of the jump, although not as a efficient as a simple BL.
+	 * For now, I've settled on loading the stack overflow routine address from a local constant pool
+	 * and jumping to it. The slight loss of efficiency can be offset by jumping straight to the routine
+	 * instead of via the PLT.
+	 * 
+	 *	bge	0f
+	 *	adr	lr, 0f
+	 *	ldr	pc, 1f
+	 * 1:	.word	stkovf
+	 * 0:
+	 */
+	ARM_B_COND (code, ARMCOND_GE, 2);
+	ARM_ADD_REG_IMM8 (code, ARMREG_LR, ARMREG_PC, 4);
+	ARM_LDR_IMM (code, ARMREG_PC, ARMREG_PC, -4);
+	*(gpointer *)code = stkovf;
+	code += 4;
+#if 0
+	mono_add_patch_info (cfg, code - cfg->native_code, MONO_PATCH_INFO_RISCOS_STKOVF,
+			     stkovf);
+	ARM_BL_COND (code, ARMCOND_LT, 0);
+#endif
+
+	*code_ptr = code;
+}
+
+#endif
+
 /*
  * Stack frame layout:
  * 
@@ -5187,6 +5390,10 @@
 	int lmf_offset = 0;
 	int prev_sp_offset, reg_offset;
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	gboolean stack_check_needed = FALSE;
+#endif
+
 	if (mono_jit_trace_calls != NULL && mono_trace_eval (method))
 		tracing = 1;
 
@@ -5200,6 +5407,60 @@
 	pos = 0;
 	prev_sp_offset = 0;
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	stack_check_needed = mono_stack_check_needed (!(cfg->flags & MONO_CFG_HAS_CALLS), alloc_size);
+	cfg->arch.stack_frame_needed = mono_stack_frame_needed (!(cfg->flags & MONO_CFG_HAS_CALLS), alloc_size);
+
+	if (!method->save_lmf) {
+		if (cfg->arch.stack_frame_needed) {
+			ARM_MOV_REG_REG (code, ARMREG_IP, ARMREG_SP);
+			ARM_PUSH (code, cfg->used_int_regs | (1 << ARMREG_R11) | (1 << ARMREG_IP) | (1 << ARMREG_LR) | (1 << ARMREG_PC));
+			ARM_SUB_REG_IMM8 (code, ARMREG_R11, ARMREG_IP, 4);
+			prev_sp_offset += 16; // r11,ip,lr,pc
+			mono_emit_unwind_op_def_cfa_offset (cfg, code, prev_sp_offset);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_PC, -4);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_LR, -8);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_SP, -12);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_R11, -16);
+			mini_gc_set_slot_type_from_cfa (cfg, -8, SLOT_NOREF);
+		} else {
+			ARM_PUSH (code, cfg->used_int_regs | (1 << ARMREG_LR));
+			prev_sp_offset += 4;
+			mono_emit_unwind_op_def_cfa_offset (cfg, code, prev_sp_offset);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_LR, -4);
+			mini_gc_set_slot_type_from_cfa (cfg, -4, SLOT_NOREF);
+		}
+
+		for (i = 0; i < 16; ++i) {
+			if (cfg->used_int_regs & (1 << i))
+				prev_sp_offset += 4;
+		}
+		mono_emit_unwind_op_def_cfa_offset (cfg, code, prev_sp_offset);
+		reg_offset = 0;
+		for (i = 0; i < 16; ++i) {
+			if ((cfg->used_int_regs & (1 << i))) {
+				mono_emit_unwind_op_offset (cfg, code, i, (- prev_sp_offset) + reg_offset);
+				mini_gc_set_slot_type_from_cfa (cfg, (- prev_sp_offset) + reg_offset, SLOT_NOREF);
+				reg_offset += 4;
+			}
+		}
+	} else {
+		ARM_MOV_REG_REG (code, ARMREG_IP, ARMREG_SP);
+		ARM_PUSH (code, 0xDBF0); /* r4-r9,fp,ip,lr,pc */
+		ARM_SUB_REG_IMM8 (code, ARMREG_R11, ARMREG_IP, 4);
+		prev_sp_offset += 4 * 10; /* all but r0-r3, r10 and sp */
+		mono_emit_unwind_op_def_cfa_offset (cfg, code, prev_sp_offset);
+		reg_offset = 0;
+		for (i = 0; i < 16; ++i) {
+			if ((i > ARMREG_R3) && (i != ARMREG_R10) && (i != ARMREG_SP)) {
+				mono_emit_unwind_op_offset (cfg, code, i, (- prev_sp_offset) + reg_offset);
+				reg_offset += 4;
+			}
+		}
+		pos += sizeof (MonoLMF) - prev_sp_offset;
+		lmf_offset = pos;
+	}
+#else
 	if (!method->save_lmf) {
 		if (iphone_abi) {
 			/* 
@@ -5261,6 +5522,7 @@
 		pos += sizeof (MonoLMF) - prev_sp_offset;
 		lmf_offset = pos;
 	}
+#endif
 	alloc_size += pos;
 	orig_alloc_size = alloc_size;
 	// align to MONO_ARCH_FRAME_ALIGNMENT bytes
@@ -5273,6 +5535,11 @@
 	if (prev_sp_offset & 4)
 		alloc_size += 4;
 	cfg->stack_usage = alloc_size;
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	if (stack_check_needed) {
+		mono_arch_emit_stack_check (cfg, &code, alloc_size);
+	}
+#endif
 	if (alloc_size) {
 		if ((i = mono_arm_is_rotated_imm8 (alloc_size, &rot_amount)) >= 0) {
 			ARM_SUB_REG_IMM (code, ARMREG_SP, ARMREG_SP, i, rot_amount);
@@ -5358,15 +5625,22 @@
 			else if (ainfo->storage == RegTypeFP) {
 				g_assert_not_reached ();
 			} else if (ainfo->storage == RegTypeBase) {
+#ifdef ENABLE_RISCOS_STACKFRAMES
+				if (cfg->arch.stack_frame_needed) {
+					ARM_LDR_IMM (code, inst->dreg, ARMREG_R11, (ainfo->offset + 4));
+				} else {
+#endif
 				if (arm_is_imm12 (prev_sp_offset + ainfo->offset)) {
 					ARM_LDR_IMM (code, inst->dreg, ARMREG_SP, (prev_sp_offset + ainfo->offset));
 				} else {
 					code = mono_arm_emit_load_imm (code, ARMREG_IP, inst->inst_offset);
 					ARM_LDR_REG_REG (code, inst->dreg, ARMREG_SP, ARMREG_IP);
 				}
+#ifdef ENABLE_RISCOS_STACKFRAMES
+				}
+#endif
 			} else
 				g_assert_not_reached ();
-
 			if (cfg->verbose_level > 2)
 				g_print ("Argument %d assigned to register %s\n", pos, mono_arch_regname (inst->dreg));
 		} else {
@@ -5382,12 +5656,26 @@
 					}
 					break;
 				case 2:
+					if (v5_supported) {
 					if (arm_is_imm8 (inst->inst_offset)) {
 						ARM_STRH_IMM (code, ainfo->reg, inst->inst_basereg, inst->inst_offset);
 					} else {
 						code = mono_arm_emit_load_imm (code, ARMREG_IP, inst->inst_offset);
 						ARM_STRH_REG_REG (code, ainfo->reg, inst->inst_basereg, ARMREG_IP);
 					}
+					} else {
+						if (arm_is_imm8 (inst->inst_offset) && arm_is_imm8 (inst->inst_offset + 1)) {
+							ARM_STRB_IMM (code, ainfo->reg, inst->inst_basereg, inst->inst_offset);
+							ARM_SHR_IMM (code, ARMREG_IP, ainfo->reg, 8);
+							ARM_STRB_IMM (code, ARMREG_IP, inst->inst_basereg, inst->inst_offset + 1);
+						} else {
+							code = mono_arm_emit_load_imm (code, ARMREG_IP, inst->inst_offset);
+							ARM_ADD_REG_REG (code, ARMREG_IP, inst->inst_basereg, ARMREG_IP);
+							ARM_STRB_IMM (code, ainfo->reg, ARMREG_IP, 0);
+							ARM_SHR_IMM (code, ARMREG_LR, ainfo->reg, 8);
+							ARM_STRB_IMM (code, ARMREG_LR, ARMREG_IP, 1);
+						}
+					}
 					break;
 				case 8:
 					if (arm_is_imm12 (inst->inst_offset)) {
@@ -5415,16 +5703,32 @@
 			} else if (ainfo->storage == RegTypeBaseGen) {
 				g_assert (arm_is_imm12 (prev_sp_offset + ainfo->offset));
 				g_assert (arm_is_imm12 (inst->inst_offset));
+#ifdef ENABLE_RISCOS_STACKFRAMES
+				if (cfg->arch.stack_frame_needed) {
+					ARM_LDR_IMM (code, ARMREG_LR, ARMREG_R11, (ainfo->offset + 4));
+				} else {
+#endif
 				ARM_LDR_IMM (code, ARMREG_LR, ARMREG_SP, (prev_sp_offset + ainfo->offset));
+#ifdef ENABLE_RISCOS_STACKFRAMES
+				}
+#endif
 				ARM_STR_IMM (code, ARMREG_LR, inst->inst_basereg, inst->inst_offset + 4);
 				ARM_STR_IMM (code, ARMREG_R3, inst->inst_basereg, inst->inst_offset);
 			} else if (ainfo->storage == RegTypeBase) {
+#ifdef ENABLE_RISCOS_STACKFRAMES
+				if (cfg->arch.stack_frame_needed) {
+					ARM_LDR_IMM (code, ARMREG_LR, ARMREG_R11, (ainfo->offset + 4));
+				} else {
+#endif
 				if (arm_is_imm12 (prev_sp_offset + ainfo->offset)) {
 					ARM_LDR_IMM (code, ARMREG_LR, ARMREG_SP, (prev_sp_offset + ainfo->offset));
 				} else {
 					code = mono_arm_emit_load_imm (code, ARMREG_IP, prev_sp_offset + ainfo->offset);
 					ARM_LDR_REG_REG (code, ARMREG_LR, ARMREG_SP, ARMREG_IP);
 				}
+#ifdef ENABLE_RISCOS_STACKFRAMES
+				}
+#endif
 
 				switch (ainfo->size) {
 				case 1:
@@ -5436,12 +5740,30 @@
 					}
 					break;
 				case 2:
+					if (v5_supported) {
 					if (arm_is_imm8 (inst->inst_offset)) {
 						ARM_STRH_IMM (code, ARMREG_LR, inst->inst_basereg, inst->inst_offset);
 					} else {
 						code = mono_arm_emit_load_imm (code, ARMREG_IP, inst->inst_offset);
 						ARM_STRH_REG_REG (code, ARMREG_LR, inst->inst_basereg, ARMREG_IP);
 					}
+					} else {
+						// TODO: I haven't yet found the right C# source code that will generate
+						// this code sequence and allow it to be validated.
+						if (arm_is_imm8 (inst->inst_offset) && arm_is_imm8 (inst->inst_offset + 1)) {
+							ARM_STRB_IMM (code, ARMREG_LR, inst->inst_basereg, inst->inst_offset);
+							// I don't think it's necessary to preserve LR here.
+							ARM_SHR_IMM (code, ARMREG_LR, ARMREG_LR, 8);
+							ARM_STRB_IMM (code, ARMREG_LR, inst->inst_basereg, inst->inst_offset + 1);
+						} else {
+							code = mono_arm_emit_load_imm (code, ARMREG_IP, inst->inst_offset);
+							ARM_ADD_REG_REG (code, ARMREG_IP, inst->inst_basereg, ARMREG_IP);
+							ARM_STRB_IMM (code, ARMREG_LR, ARMREG_IP, 0);
+							// I don't think it's necessary to preserve LR here.
+							ARM_SHR_IMM (code, ARMREG_LR, ARMREG_LR, 8);
+							ARM_STRB_IMM (code, ARMREG_LR, ARMREG_IP, 1);
+						}
+					}
 					break;
 				case 8:
 					if (arm_is_imm12 (inst->inst_offset)) {
@@ -5450,12 +5772,22 @@
 						code = mono_arm_emit_load_imm (code, ARMREG_IP, inst->inst_offset);
 						ARM_STR_REG_REG (code, ARMREG_LR, inst->inst_basereg, ARMREG_IP);
 					}
+#ifdef ENABLE_RISCOS_STACKFRAMES
+					// TODO: check the +8 is correct here (there's already a +4 below so
+					// another equals +8.
+					if (cfg->arch.stack_frame_needed) {
+						ARM_LDR_IMM (code, ARMREG_LR, ARMREG_R11, (ainfo->offset + 8));
+					} else {
+#endif
 					if (arm_is_imm12 (prev_sp_offset + ainfo->offset + 4)) {
 						ARM_LDR_IMM (code, ARMREG_LR, ARMREG_SP, (prev_sp_offset + ainfo->offset + 4));
 					} else {
 						code = mono_arm_emit_load_imm (code, ARMREG_IP, prev_sp_offset + ainfo->offset + 4);
 						ARM_LDR_REG_REG (code, ARMREG_LR, ARMREG_SP, ARMREG_IP);
 					}
+#ifdef ENABLE_RISCOS_STACKFRAMES
+					}
+#endif
 					if (arm_is_imm12 (inst->inst_offset + 4)) {
 						ARM_STR_IMM (code, ARMREG_LR, inst->inst_basereg, inst->inst_offset + 4);
 					} else {
@@ -5493,6 +5825,11 @@
 				if (ainfo->vtsize) {
 					/* FIXME: handle overrun! with struct sizes not multiple of 4 */
 					//g_print ("emit_memcpy (prev_sp_ofs: %d, ainfo->offset: %d, soffset: %d)\n", prev_sp_offset, ainfo->offset, soffset);
+#ifdef ENABLE_RISCOS_STACKFRAMES
+					if (cfg->arch.stack_frame_needed)
+						code = emit_memcpy (code, ainfo->vtsize * sizeof (gpointer), inst->inst_basereg, doffset, ARMREG_R11, ainfo->offset + 4);
+					else
+#endif
 					code = emit_memcpy (code, ainfo->vtsize * sizeof (gpointer), inst->inst_basereg, doffset, ARMREG_SP, prev_sp_offset + ainfo->offset);
 				}
 			} else if (ainfo->storage == RegTypeStructByAddr) {
@@ -5652,13 +5989,20 @@
 	}
 
 	if (method->save_lmf) {
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		int lmf_offset;
+#else
 		int lmf_offset, reg, sp_adj, regmask;
+#endif
 		/* all but r0-r3, sp and pc */
 		pos += sizeof (MonoLMF) - (MONO_ARM_NUM_SAVED_REGS * sizeof (mgreg_t));
 		lmf_offset = pos;
 
 		code = emit_restore_lmf (cfg, code, cfg->stack_usage - lmf_offset);
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		ARM_EMIT (code, ARM_DEF_MRT (0xABF0, ARMREG_R11, 1, 0, 0, 0, 1, ARMCOND_AL)); /* r4-r9,fp,sp,pc */
+#else
 		/* This points to r4 inside MonoLMF->iregs */
 		sp_adj = (sizeof (MonoLMF) - MONO_ARM_NUM_SAVED_REGS * sizeof (mgreg_t));
 		reg = ARMREG_R4;
@@ -5673,7 +6017,24 @@
 		code = emit_big_add (code, ARMREG_SP, cfg->frame_reg, cfg->stack_usage - lmf_offset + sp_adj);
 		/* restore iregs */
 		ARM_POP (code, regmask); 
+#endif
 	} else {
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		if (cfg->arch.stack_frame_needed) {
+			ARM_EMIT (code, ARM_DEF_MRT (cfg->used_int_regs | (1 << ARMREG_R11) | (1 << ARMREG_SP) | (1 << ARMREG_PC), ARMREG_R11, 1, 0, 0, 0, 1, ARMCOND_AL));
+		} else {
+			if (cfg->stack_usage != 0) {
+				if ((i = mono_arm_is_rotated_imm8 (cfg->stack_usage, &rot_amount)) >= 0) {
+					ARM_ADD_REG_IMM (code, ARMREG_SP, cfg->frame_reg, i, rot_amount);
+				} else {
+					code = mono_arm_emit_load_imm (code, ARMREG_IP, cfg->stack_usage);
+					ARM_ADD_REG_REG (code, ARMREG_SP, cfg->frame_reg, ARMREG_IP);
+				}
+			}
+
+			ARM_POP (code, cfg->used_int_regs | (1 << ARMREG_PC));
+		}
+#else
 		if ((i = mono_arm_is_rotated_imm8 (cfg->stack_usage, &rot_amount)) >= 0) {
 			ARM_ADD_REG_IMM (code, ARMREG_SP, cfg->frame_reg, i, rot_amount);
 		} else {
@@ -5690,6 +6051,7 @@
 		} else {
 			ARM_POP (code, cfg->used_int_regs | (1 << ARMREG_PC));
 		}
+#endif
 	}
 
 	cfg->code_len = code - cfg->native_code;
