--- mono/mini/tramp-arm.c.orig	2011-12-19 21:10:25.000000000 +0000
+++ mono/mini/tramp-arm.c	2012-05-26 17:33:34.000000000 +0100
@@ -157,8 +157,11 @@
 	 * regs on the stack (all but PC and SP). The original LR value has been
 	 * saved as sp + LR_OFFSET by the push in the specific trampoline
 	 */
+#ifdef ENABLE_RISCOS_STACKFRAMES
+#define LR_OFFSET (sizeof (mgreg_t) * 12)
+#else
 #define LR_OFFSET (sizeof (mgreg_t) * 13)
-
+#endif
 	// FIXME: Finish the unwind info, the current info allows us to unwind
 	// when the trampoline is not in the epilog
 
@@ -166,8 +169,11 @@
 	cfa_offset = 14 * sizeof (mgreg_t);
 	mono_add_unwind_op_def_cfa (unwind_ops, code, buf, ARMREG_SP, cfa_offset);
 	// PC saved at sp+LR_OFFSET
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	mono_add_unwind_op_offset (unwind_ops, code, buf, ARMREG_LR, -8);
+#else
 	mono_add_unwind_op_offset (unwind_ops, code, buf, ARMREG_LR, -4);
-
+#endif
 	ARM_MOV_REG_REG (code, ARMREG_V1, ARMREG_SP);
 	if (aot && tramp_type != MONO_TRAMPOLINE_GENERIC_CLASS_INIT) {
 		/* 
@@ -226,16 +232,25 @@
 		ARM_MOV_REG_IMM8 (code, ARMREG_R2, 0);
 		ARM_STR_IMM (code, ARMREG_R2, ARMREG_R1, G_STRUCT_OFFSET (MonoLMF, method));
 	}
-	/* Save sp into lmf->iregs, the eh code expects it to be at IP */
+	/* Save sp into lmf->iregs, the eh code expects it to be at IP-1 because we skip R10 & R13 */
 	ARM_ADD_REG_IMM8 (code, ARMREG_R2, ARMREG_SP, cfa_offset);
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	ARM_STR_IMM (code, ARMREG_R2, ARMREG_R1, G_STRUCT_OFFSET (MonoLMF, iregs) + ((ARMREG_IP - 1) * sizeof (mgreg_t)));
+#else
 	ARM_STR_IMM (code, ARMREG_R2, ARMREG_R1, G_STRUCT_OFFSET (MonoLMF, iregs) + (ARMREG_IP * sizeof (mgreg_t)));
+#endif
 	ARM_STR_IMM (code, ARMREG_SP, ARMREG_R1, G_STRUCT_OFFSET (MonoLMF, esp));
 	/* save the IP (caller ip) */
 	if (tramp_type == MONO_TRAMPOLINE_JUMP) {
 		ARM_MOV_REG_IMM8 (code, ARMREG_R2, 0);
 	} else {
 		/* assumes STACK == sizeof (MonoLMF) */
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		/* We don't save R10, so the offset is 1 less than the original.  */
+		ARM_LDR_IMM (code, ARMREG_R2, ARMREG_SP, (G_STRUCT_OFFSET (MonoLMF, iregs) + 12*4));
+#else
 		ARM_LDR_IMM (code, ARMREG_R2, ARMREG_SP, (G_STRUCT_OFFSET (MonoLMF, iregs) + 13*4));
+#endif
 	}
 	ARM_STR_IMM (code, ARMREG_R2, ARMREG_R1, G_STRUCT_OFFSET (MonoLMF, eip));
 
@@ -290,7 +305,12 @@
 	 * clobbered). This way we can just restore all the regs in one inst
 	 * and branch to IP.
 	 */
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	/* We don't save R10, so the offset is 1 less than the original.  */
+	ARM_STR_IMM (code, ARMREG_R0, ARMREG_V1, ((ARMREG_R12 - 1) * sizeof (mgreg_t)));
+#else
 	ARM_STR_IMM (code, ARMREG_R0, ARMREG_V1, (ARMREG_R12 * sizeof (mgreg_t)));
+#endif
 
 	/* Check for thread interruption */
 	/* This is not perf critical code so no need to check the interrupt flag */
@@ -334,7 +354,11 @@
 	 * Note that IP has been conveniently set to the method addr.
 	 */
 	ARM_ADD_REG_IMM8 (code, ARMREG_SP, ARMREG_SP, sizeof (MonoLMF) - sizeof (mgreg_t) * 14);
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	ARM_POP_NWB (code, 0x5bff); /* All but R10, R13 & PC.  */
+#else
 	ARM_POP_NWB (code, 0x5fff);
+#endif
 	if (tramp_type == MONO_TRAMPOLINE_RGCTX_LAZY_FETCH)
 		ARM_MOV_REG_REG (code, ARMREG_R0, ARMREG_IP);
 	/* do we need to set sp? */
@@ -420,8 +444,13 @@
 	 * method-literal
 	 * tramp-literal
 	 */
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	/* We save all the registers, except R10 and SP */
+	ARM_PUSH (code, 0xdbff);
+#else
 	/* We save all the registers, except PC and SP */
 	ARM_PUSH (code, 0x5fff);
+#endif
 	if (short_branch) {
 		constants = (gpointer*)code;
 		constants [0] = GUINT_TO_POINTER (short_branch | (1 << 24));
@@ -645,7 +674,13 @@
 		mono_marshal_find_bitfield_offset (MonoVTable, initialized, &byte_offset, &bitmask);
 
 	g_assert (arm_is_imm8 (byte_offset));
+	if (v5_supported) {
 	ARM_LDRSB_IMM (code, ARMREG_IP, MONO_ARCH_VTABLE_REG, byte_offset);
+	} else {
+		ARM_LDRB_IMM (code, ARMREG_IP, MONO_ARCH_VTABLE_REG, byte_offset);
+		ARM_SHL_IMM (code, ARMREG_IP, ARMREG_IP, 24);
+		ARM_SAR_IMM (code, ARMREG_IP, ARMREG_IP, 24);
+	}
 	imm8 = mono_arm_is_rotated_imm8 (bitmask, &rot_amount);
 	g_assert (imm8 >= 0);
 	ARM_AND_REG_IMM (code, ARMREG_IP, ARMREG_IP, imm8, rot_amount);
