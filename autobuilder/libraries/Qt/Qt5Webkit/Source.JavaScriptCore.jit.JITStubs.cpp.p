--- Source/JavaScriptCore/jit/JITStubs.cpp.orig	2015-06-29 21:11:07.000000000 +0100
+++ Source/JavaScriptCore/jit/JITStubs.cpp	2016-01-24 19:49:57.001389769 +0000
@@ -726,13 +726,21 @@
 INLINE_ARM_FUNCTION(ctiTrampoline)
 SYMBOL_STRING(ctiTrampoline) ":" "\n"
     "stmdb sp!, {r1-r3}" "\n"
+#if OS(RISCOS)
+    "stmdb sp!, {r4-r7, r9-r11, lr}" "\n"
+#else
     "stmdb sp!, {r4-r6, r8-r11, lr}" "\n"
+#endif
     "sub sp, sp, #" STRINGIZE_VALUE_OF(PRESERVEDR4_OFFSET) "\n"
     "mov r5, r2" "\n"
     // r0 contains the code
     "blx r0" "\n"
     "add sp, sp, #" STRINGIZE_VALUE_OF(PRESERVEDR4_OFFSET) "\n"
+#if OS(RISCOS)
+    "ldmia sp!, {r4-r7, r9-r11, lr}" "\n"
+#else
     "ldmia sp!, {r4-r6, r8-r11, lr}" "\n"
+#endif
     "add sp, sp, #12" "\n"
     "bx lr" "\n"
 ".globl " SYMBOL_STRING(ctiTrampolineEnd) "\n"
@@ -756,7 +764,11 @@
 INLINE_ARM_FUNCTION(ctiOpThrowNotCaught)
 SYMBOL_STRING(ctiOpThrowNotCaught) ":" "\n"
     "add sp, sp, #" STRINGIZE_VALUE_OF(PRESERVEDR4_OFFSET) "\n"
+#if OS(RISCOS)
+    "ldmia sp!, {r4-r7, r9-r11, lr}" "\n"
+#else
     "ldmia sp!, {r4-r6, r8-r11, lr}" "\n"
+#endif
     "add sp, sp, #12" "\n"
     "bx lr" "\n"
 );
