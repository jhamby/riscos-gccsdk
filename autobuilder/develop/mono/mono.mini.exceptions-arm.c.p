--- mono/mini/exceptions-arm.c.orig	2013-01-08 18:41:05.000000000 +0000
+++ mono/mini/exceptions-arm.c	2013-02-24 18:27:06.000000000 +0000
@@ -474,7 +474,15 @@
 		 * produce the register state which existed at the time of the call which
 		 * transitioned to native call, so we save the sp/fp/ip in the LMF.
 		 */
+#ifdef ENABLE_RISCOS_STACKFRAMES
+		memcpy (&new_ctx->regs [0], &(*lmf)->iregs [0], sizeof (mgreg_t) * 10); // R0-R9
+		/* R10 & SP is skipped */
+		new_ctx->regs [ARMREG_R11] = (*lmf)->iregs [ARMREG_R11 - 1]; // Skip R10
+		new_ctx->regs [ARMREG_IP] = (*lmf)->iregs [ARMREG_IP - 1]; // Skip R10
+		new_ctx->regs [ARMREG_LR] = (*lmf)->iregs [ARMREG_LR - 2]; // Skip R10 & SP
+#else
 		memcpy (&new_ctx->regs [0], &(*lmf)->iregs [0], sizeof (mgreg_t) * 13);
+#endif
 		new_ctx->pc = (*lmf)->ip;
 		new_ctx->regs [ARMREG_SP] = (*lmf)->sp;
 		new_ctx->regs [ARMREG_FP] = (*lmf)->fp;
