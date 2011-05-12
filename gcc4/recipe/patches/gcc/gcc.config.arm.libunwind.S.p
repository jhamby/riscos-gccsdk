Index: gcc/config/arm/libunwind.S
===================================================================
--- gcc/config/arm/libunwind.S	(revision 172858)
+++ gcc/config/arm/libunwind.S	(working copy)
@@ -343,11 +343,23 @@
 	add sp, #72
 	bx r3
 #else
+#ifdef __PIC__
+	ldr r7, 1f
+0:
+	add r7, pc, r7
+	ldmia r7, {r7, r8}
+	ldr r8, [r8, #0]
+	ldr r7, [r8, r7, lsl#4]
+#endif
 	bl SYM (__gnu\name) __PLT__
 	ldr lr, [sp, #64]
 	add sp, sp, #72
 	RET
+#ifdef __PIC__
+1:
+	.word _GLOBAL_OFFSET_TABLE_-(0b + 4)
 #endif
+#endif
 	FUNC_END \name
 	UNPREFIX \name
 .endm
