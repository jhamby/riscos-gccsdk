--- libgcc/config/arm/lib1funcs.S.orig	2018-01-03 10:03:58.000000000 +0000
+++ libgcc/config/arm/lib1funcs.S	2018-05-20 11:36:45.244185430 +0100
@@ -1556,7 +1556,8 @@
 	
 #endif /* L_dvmd_lnx */
 #ifdef L_clear_cache
-#if defined __ARM_EABI__ && defined __linux__
+#if defined __ARM_EABI__
+#if defined __linux__
 @ EABI GNU/Linux call to cacheflush syscall.
 	ARM_FUNC_START clear_cache
 	do_push	{r7}
@@ -1572,6 +1573,17 @@
 	do_pop	{r7}
 	RET
 	FUNC_END clear_cache
+#elif defined __riscos__
+@ EABI GNU/RISCOS call to cacheflush SWI.
+	ARM_FUNC_START clear_cache
+	bic	r2, r1, #3
+	sub	r1, r0, #1
+	bic	r1, r1, #3
+	mov	r0, #1
+	swi	0x2006e		@ XOS_SynchroniseCodeAreas
+	RET
+	FUNC_END clear_cache
+#endif
 #else
 #error "This is only for ARM EABI GNU/Linux"
 #endif
