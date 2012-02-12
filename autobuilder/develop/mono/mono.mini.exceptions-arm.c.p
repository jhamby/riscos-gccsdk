--- mono/mini/exceptions-arm.c.orig	2011-08-26 02:06:56.000000000 +0100
+++ mono/mini/exceptions-arm.c	2012-01-12 20:47:29.000000000 +0000
@@ -459,12 +459,21 @@
 		 * ARM_PUSH (code, 0x5ff0);
 		 * So it stores the register state as it existed at the caller.
 		 */
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		memcpy (&new_ctx->regs [0], &(*lmf)->iregs [0], sizeof (mgreg_t) * 10);
+		/* R10 & SP is skipped */
+		new_ctx->regs [ARMREG_R11] = (*lmf)->iregs [ARMREG_R11 - 1]; // Skip R10
+		new_ctx->regs [ARMREG_IP] = (*lmf)->iregs [ARMREG_IP - 1]; // Skip R10
+		new_ctx->regs [ARMREG_LR] = (*lmf)->iregs [ARMREG_LR - 2]; // Skip R10 & SP
+		new_ctx->esp = new_ctx->regs [ARMREG_IP];
+		new_ctx->eip = new_ctx->regs [ARMREG_LR];
+#else
 		memcpy (&new_ctx->regs [0], &(*lmf)->iregs [0], sizeof (mgreg_t) * 13);
 		/* SP is skipped */
 		new_ctx->regs [ARMREG_LR] = (*lmf)->iregs [ARMREG_LR - 1];
 		new_ctx->esp = (*lmf)->iregs [ARMREG_IP];
 		new_ctx->eip = new_ctx->regs [ARMREG_LR];
-
+#endif
 		/* Clear thumb bit */
 		new_ctx->eip &= ~1;
 
