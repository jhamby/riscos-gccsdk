--- Source/JavaScriptCore/jit/JITStubs.cpp.orig	2014-05-15 18:12:36.000000000 +0100
+++ Source/JavaScriptCore/jit/JITStubs.cpp	2014-06-28 14:44:59.657016252 +0100
@@ -732,11 +732,20 @@
     "mov r5, r2" "\n"
     "mov r6, #512" "\n"
     // r0 contains the code
+#if OS(RISCOS)
+    "mov lr, pc" "\n"
+    "mov pc, r0" "\n"
+#else
     "blx r0" "\n"
+#endif
     "add sp, sp, #" STRINGIZE_VALUE_OF(PRESERVEDR4_OFFSET) "\n"
     "ldmia sp!, {r4-r6, r8-r11, lr}" "\n"
     "add sp, sp, #12" "\n"
+#if OS(RISCOS)
+    "mov pc, lr" "\n"
+#else
     "bx lr" "\n"
+#endif
 ".globl " SYMBOL_STRING(ctiTrampolineEnd) "\n"
 HIDE_SYMBOL(ctiTrampolineEnd) "\n"
 SYMBOL_STRING(ctiTrampolineEnd) ":" "\n"
@@ -760,7 +769,11 @@
     "add sp, sp, #" STRINGIZE_VALUE_OF(PRESERVEDR4_OFFSET) "\n"
     "ldmia sp!, {r4-r6, r8-r11, lr}" "\n"
     "add sp, sp, #12" "\n"
+#if OS(RISCOS)
+    "mov pc, lr" "\n"
+#else
     "bx lr" "\n"
+#endif
 );
 
 #elif COMPILER(RVCT) && CPU(ARM_THUMB2)
@@ -1324,6 +1337,22 @@
 
 #elif CPU(ARM_TRADITIONAL) && COMPILER(GCC)
 
+#if OS(RISCOS)
+#define DEFINE_STUB_FUNCTION(rtype, op) \
+    extern "C" { \
+        rtype JITStubThunked_##op(STUB_ARGS_DECLARATION); \
+    }; \
+    asm ( \
+        ".globl " SYMBOL_STRING(cti_##op) "\n" \
+        INLINE_ARM_FUNCTION(cti_##op) \
+        SYMBOL_STRING(cti_##op) ":" "\n" \
+        "str lr, [sp, #" STRINGIZE_VALUE_OF(THUNK_RETURN_ADDRESS_OFFSET) "]" "\n" \
+        "bl " SYMBOL_STRING(JITStubThunked_##op) "\n" \
+        "ldr lr, [sp, #" STRINGIZE_VALUE_OF(THUNK_RETURN_ADDRESS_OFFSET) "]" "\n" \
+        "mov pc, lr" "\n" \
+        ); \
+    rtype JITStubThunked_##op(STUB_ARGS_DECLARATION)
+#else
 #define DEFINE_STUB_FUNCTION(rtype, op) \
     extern "C" { \
         rtype JITStubThunked_##op(STUB_ARGS_DECLARATION); \
@@ -1338,6 +1367,7 @@
         "bx lr" "\n" \
         ); \
     rtype JITStubThunked_##op(STUB_ARGS_DECLARATION)
+#endif
 
 #elif (CPU(ARM_THUMB2) || CPU(ARM_TRADITIONAL)) && COMPILER(RVCT)
 
