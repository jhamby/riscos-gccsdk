--- Source/JavaScriptCore/dfg/DFGGPRInfo.h.orig	2015-06-29 21:11:04.000000000 +0100
+++ Source/JavaScriptCore/dfg/DFGGPRInfo.h	2015-12-11 12:17:41.987699550 +0000
@@ -393,7 +393,11 @@
 class GPRInfo {
 public:
     typedef GPRReg RegisterType;
+#if OS(RISCOS)
+    static const unsigned numberOfRegisters = 7;
+#else
     static const unsigned numberOfRegisters = 9;
+#endif
 
     // Temporary registers.
     static const GPRReg regT0 = ARMRegisters::r0;
@@ -424,7 +428,11 @@
     static GPRReg toRegister(unsigned index)
     {
         ASSERT(index < numberOfRegisters);
+#if OS(RISCOS)
+        static const GPRReg registerForIndex[numberOfRegisters] = { regT0, regT1, regT2, regT3, regT4, regT5, regT8 };
+#else
         static const GPRReg registerForIndex[numberOfRegisters] = { regT0, regT1, regT2, regT3, regT4, regT5, regT6, regT7, regT8 };
+#endif
         return registerForIndex[index];
     }
 
@@ -432,7 +440,11 @@
     {
         ASSERT(static_cast<unsigned>(reg) != InvalidGPRReg);
         ASSERT(static_cast<unsigned>(reg) < 16);
+#if OS(RISCOS)
+        static const unsigned indexForRegister[16] = { 0, 1, 2, 8, 3, InvalidIndex, InvalidIndex, InvalidIndex, 4, 5, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex };
+#else
         static const unsigned indexForRegister[16] = { 0, 1, 2, 8, 3, InvalidIndex, InvalidIndex, InvalidIndex, 4, 5, 6, 7, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex };
+#endif
         unsigned result = indexForRegister[reg];
         ASSERT(result != InvalidIndex);
         return result;
