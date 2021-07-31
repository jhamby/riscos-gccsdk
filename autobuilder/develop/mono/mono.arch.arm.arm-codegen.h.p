--- mono/arch/arm/arm-codegen.h.orig	2013-07-30 02:47:34.000000000 +0100
+++ mono/arch/arm/arm-codegen.h	2013-08-02 20:24:44.839899124 +0100
@@ -82,7 +82,11 @@
 	ARMREG_V6 = ARMREG_R9,
 	ARMREG_V7 = ARMREG_R10,
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	ARMREG_FP = ARMREG_R9,
+#else
 	ARMREG_FP = ARMREG_R11,
+#endif
 	ARMREG_IP = ARMREG_R12,
 	ARMREG_SP = ARMREG_R13,
 	ARMREG_LR = ARMREG_R14,
@@ -1107,6 +1111,10 @@
 #define ARM_UDIV_COND(p, rd, rn, rm, cond) ARM_EMIT (p, (((cond) << 28) | (0xe << 23) | (0x3 << 20) | ((rd) << 16) | (0xf << 12) | ((rm) << 8) | (0x0 << 5) | (0x1 << 4) | ((rn) << 0)))
 #define ARM_UDIV(p, rd, rn, rm) ARM_UDIV_COND ((p), (rd), (rn), (rm), ARMCOND_AL)
 
+#ifdef __riscos__
+#define ARM_EMIT_SWI_OSBREAKPT(p) ARM_EMIT(p, 0xEF000017)
+#endif
+
 #ifdef __cplusplus
 }
 #endif
