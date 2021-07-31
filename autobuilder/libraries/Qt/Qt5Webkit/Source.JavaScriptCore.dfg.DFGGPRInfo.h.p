--- Source/JavaScriptCore/dfg/DFGGPRInfo.h.orig	2015-06-29 21:11:04.000000000 +0100
+++ Source/JavaScriptCore/dfg/DFGGPRInfo.h	2016-01-15 19:36:56.458358385 +0000
@@ -393,14 +393,22 @@
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
     static const GPRReg regT1 = ARMRegisters::r1;
     static const GPRReg regT2 = ARMRegisters::r2;
     static const GPRReg regT3 = ARMRegisters::r4;
+#if OS(RISCOS)
+    static const GPRReg regT4 = ARMRegisters::r7;
+#else
     static const GPRReg regT4 = ARMRegisters::r8;
+#endif
     static const GPRReg regT5 = ARMRegisters::r9;
     static const GPRReg regT6 = ARMRegisters::r10;
     static const GPRReg regT7 = ARMRegisters::r11;
@@ -415,7 +423,11 @@
     static const GPRReg argumentGPR2 = ARMRegisters::r2; // regT2
     static const GPRReg argumentGPR3 = ARMRegisters::r3; // regT8
     static const GPRReg nonArgGPR0 = ARMRegisters::r4; // regT3
+#if OS(RISCOS)
+    static const GPRReg nonArgGPR1 = ARMRegisters::r7; // regT4
+#else
     static const GPRReg nonArgGPR1 = ARMRegisters::r8; // regT4
+#endif
     static const GPRReg nonArgGPR2 = ARMRegisters::r9; // regT5
     static const GPRReg returnValueGPR = ARMRegisters::r0; // regT0
     static const GPRReg returnValueGPR2 = ARMRegisters::r1; // regT1
@@ -424,7 +436,11 @@
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
 
@@ -432,7 +448,11 @@
     {
         ASSERT(static_cast<unsigned>(reg) != InvalidGPRReg);
         ASSERT(static_cast<unsigned>(reg) < 16);
+#if OS(RISCOS)
+        static const unsigned indexForRegister[16] = { 0, 1, 2, 6, 3, InvalidIndex, InvalidIndex, 4, InvalidIndex, 5, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex };
+#else
         static const unsigned indexForRegister[16] = { 0, 1, 2, 8, 3, InvalidIndex, InvalidIndex, InvalidIndex, 4, 5, 6, 7, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex };
+#endif
         unsigned result = indexForRegister[reg];
         ASSERT(result != InvalidIndex);
         return result;
