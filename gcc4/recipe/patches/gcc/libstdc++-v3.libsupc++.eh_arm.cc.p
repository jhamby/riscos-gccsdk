Index: libstdc++-v3/libsupc++/eh_arm.cc
===================================================================
--- libstdc++-v3/libsupc++/eh_arm.cc	(revision 172858)
+++ libstdc++-v3/libsupc++/eh_arm.cc	(working copy)
@@ -168,15 +168,35 @@
 "	bl\t_Unwind_Resume @ Never returns\n"
 "	.popsection\n");
 #else
+#ifdef __PIC__
 asm ("  .pushsection .text.__cxa_end_cleanup\n"
 "	.global __cxa_end_cleanup\n"
 "	.type __cxa_end_cleanup, \"function\"\n"
 "__cxa_end_cleanup:\n"
 "	stmfd\tsp!, {r1, r2, r3, r4}\n"
+"	ldr\tr7, 1f\n"
+"0:\n"
+"	add\tr7, pc, r7\n"
+"	ldmia\tr7, {r7, r8}\n"
+"	ldr\tr8, [r8, #0]\n"
+"	ldr\tr7, [r8, r7, lsl#4]\n"
 "	bl\t__gnu_end_cleanup\n"
 "	ldmfd\tsp!, {r1, r2, r3, r4}\n"
 "	bl\t_Unwind_Resume @ Never returns\n"
+"1:\n"
+"	.word _GLOBAL_OFFSET_TABLE_-(0b + 4)\n"
 "	.popsection\n");
+#else
+asm ("  .pushsection .text.__cxa_end_cleanup\n"
+"	.global __cxa_end_cleanup\n"
+"	.type __cxa_end_cleanup, \"function\"\n"
+"__cxa_end_cleanup:\n"
+"	stmfd\tsp!, {r1, r2, r3, r4}\n"
+"	bl\t__gnu_end_cleanup\n"
+"	ldmfd\tsp!, {r1, r2, r3, r4}\n"
+"	bl\t_Unwind_Resume @ Never returns\n"
+"	.popsection\n");
 #endif
+#endif
 
 #endif
