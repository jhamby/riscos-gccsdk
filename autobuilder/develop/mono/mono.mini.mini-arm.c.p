--- mono/mini/mini-arm.c.orig	2011-12-19 21:10:25.000000000 +0000
+++ mono/mini/mini-arm.c	2012-04-15 16:31:58.000000000 +0100
@@ -710,6 +710,15 @@
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
@@ -718,6 +727,7 @@
 	if (!(cfg->compile_aot || cfg->uses_rgctx_reg || COMPILE_LLVM (cfg)))
 		/* V5 is reserved for passing the vtable/rgctx/IMT method */
 		regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V5));
+#endif
 	/*regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V6));*/
 	/*regs = g_list_prepend (regs, GUINT_TO_POINTER (ARMREG_V7));*/
 
@@ -4687,6 +4697,10 @@
 	mono_register_jit_icall (__aeabi_read_tp, "__aeabi_read_tp", mono_create_icall_signature ("void"), TRUE);
 #endif
 #endif
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	mono_register_jit_icall (__rt_stkovf_split_small, "__rt_stkovf_split_small", NULL, FALSE);
+	mono_register_jit_icall (__rt_stkovf_split_big, "__rt_stkovf_split_big", NULL, FALSE);
+#endif
 }
 
 #define patch_lis_ori(ip,val) do {\
@@ -4730,6 +4744,11 @@
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
@@ -4775,6 +4794,114 @@
 
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
@@ -4804,6 +4931,10 @@
 	int lmf_offset = 0;
 	int prev_sp_offset, reg_offset;
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	gboolean stack_check_needed = FALSE;
+#endif
+
 	if (mono_jit_trace_calls != NULL && mono_trace_eval (method))
 		tracing = 1;
 
@@ -4817,6 +4948,86 @@
 	pos = 0;
 	prev_sp_offset = 0;
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	stack_check_needed = mono_stack_check_needed (!(cfg->flags & MONO_CFG_HAS_CALLS), alloc_size);
+	cfg->arch.stack_frame_needed = mono_stack_frame_needed (!(cfg->flags & MONO_CFG_HAS_CALLS), alloc_size);
+
+	if (!method->save_lmf) {
+
+		if (cfg->arch.stack_frame_needed) {
+			ARM_MOV_REG_REG (code, ARMREG_IP, ARMREG_SP);
+			ARM_PUSH (code, cfg->used_int_regs | (1 << ARMREG_R11) | (1 << ARMREG_IP) | (1 << ARMREG_LR) | (1 << ARMREG_PC));
+			ARM_SUB_REG_IMM8 (code, ARMREG_R11, ARMREG_IP, 4);
+			prev_sp_offset += 16; // r11,ip,lr,pc
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_PC, -4);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_LR, -8);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_SP, -12);
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_R11, -16);
+		} else {
+			ARM_PUSH (code, cfg->used_int_regs | (1 << ARMREG_LR));
+			prev_sp_offset += 4;
+			mono_emit_unwind_op_offset (cfg, code, ARMREG_LR, -4);
+		}
+
+		for (i = 0; i < 16; ++i) {
+			if (cfg->used_int_regs & (1 << i))
+				prev_sp_offset += 4;
+		}
+		mono_emit_unwind_op_def_cfa_offset (cfg, code, prev_sp_offset);
+		reg_offset = 0;
+		for (i = 0; i < 10; ++i) {
+			if ((cfg->used_int_regs & (1 << i))) {
+				mono_emit_unwind_op_offset (cfg, code, i, (- prev_sp_offset) + reg_offset);
+				reg_offset += 4;
+			}
+		}
+
+
+		// align to MONO_ARCH_FRAME_ALIGNMENT bytes
+		if (alloc_size & (MONO_ARCH_FRAME_ALIGNMENT - 1)) {
+			alloc_size += MONO_ARCH_FRAME_ALIGNMENT - 1;
+			alloc_size &= ~(MONO_ARCH_FRAME_ALIGNMENT - 1);
+		}
+
+		/* the stack used in the pushed regs */
+		if (prev_sp_offset & 4)
+			alloc_size += 4;
+
+		if (stack_check_needed)
+			mono_arch_emit_stack_check (cfg, &code, alloc_size);
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
+
+		alloc_size += pos;
+
+		// align to MONO_ARCH_FRAME_ALIGNMENT bytes
+		if (alloc_size & (MONO_ARCH_FRAME_ALIGNMENT - 1)) {
+			alloc_size += MONO_ARCH_FRAME_ALIGNMENT - 1;
+			alloc_size &= ~(MONO_ARCH_FRAME_ALIGNMENT - 1);
+		}
+
+		/* the stack used in the pushed regs */
+		if (prev_sp_offset & 4)
+			alloc_size += 4;
+
+		if (stack_check_needed) {
+			mono_arch_emit_stack_check (cfg, &code, alloc_size);
+		}
+	}
+#else
 	if (!method->save_lmf) {
 		if (iphone_abi) {
 			/* 
@@ -4875,7 +5086,9 @@
 		pos += sizeof (MonoLMF) - prev_sp_offset;
 		lmf_offset = pos;
 	}
+
 	alloc_size += pos;
+
 	// align to MONO_ARCH_FRAME_ALIGNMENT bytes
 	if (alloc_size & (MONO_ARCH_FRAME_ALIGNMENT - 1)) {
 		alloc_size += MONO_ARCH_FRAME_ALIGNMENT - 1;
@@ -4885,6 +5098,7 @@
 	/* the stack used in the pushed regs */
 	if (prev_sp_offset & 4)
 		alloc_size += 4;
+#endif
 	cfg->stack_usage = alloc_size;
 	if (alloc_size) {
 		if ((i = mono_arm_is_rotated_imm8 (alloc_size, &rot_amount)) >= 0) {
@@ -4968,15 +5182,22 @@
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
@@ -5017,16 +5238,32 @@
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
@@ -5052,12 +5289,22 @@
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
@@ -5095,6 +5342,11 @@
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
@@ -5288,7 +5540,11 @@
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
@@ -5300,6 +5556,9 @@
 		ARM_LDR_IMM (code, ARMREG_LR, ARMREG_R2, G_STRUCT_OFFSET (MonoLMF, lmf_addr));
 		/* *(lmf_addr) = previous_lmf */
 		ARM_STR_IMM (code, ARMREG_IP, ARMREG_LR, G_STRUCT_OFFSET (MonoLMF, previous_lmf));
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		ARM_EMIT (code, ARM_DEF_MRT (0xABF0, ARMREG_R11, 1, 0, 0, 0, 1, ARMCOND_AL)); /* r4-r9,fp,sp,pc */
+#else
 		/* This points to r4 inside MonoLMF->iregs */
 		sp_adj = (sizeof (MonoLMF) - MONO_ARM_NUM_SAVED_REGS * sizeof (mgreg_t));
 		reg = ARMREG_R4;
@@ -5314,7 +5573,24 @@
 		ARM_ADD_REG_IMM8 (code, ARMREG_SP, ARMREG_R2, sp_adj);
 		/* restore iregs */
 		ARM_POP (code, regmask); 
+#endif
+	} else {
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		if (cfg->arch.stack_frame_needed) {
+			ARM_EMIT (code, ARM_DEF_MRT (cfg->used_int_regs | (1 << ARMREG_R11) | (1 << ARMREG_SP) | (1 << ARMREG_PC), ARMREG_R11, 1, 0, 0, 0, 1, ARMCOND_AL));
 	} else {
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
@@ -5331,6 +5607,7 @@
 		} else {
 			ARM_POP (code, cfg->used_int_regs | (1 << ARMREG_PC));
 		}
+#endif
 	}
 
 	cfg->code_len = code - cfg->native_code;
