--- lib/Target/ARM/ARMCallingConv.td.orig	2014-05-09 15:01:47.000000000 +0100
+++ lib/Target/ARM/ARMCallingConv.td	2014-12-22 17:48:24.730392823 +0000
@@ -238,4 +238,12 @@
 // registers.
 def CSR_FIQ : CalleeSavedRegs<(add LR, R11, (sequence "R%u", 7, 0))>;
 
+// The APCS32 registers saved by RISC OS
+def CSR_RISCOS_APCS32 : CalleeSavedRegs<(add PC, LR, SP, R11, R9, R7,
+                                         R6, R5, R4)>;
+def CSR_RISCOS_APCS32_ThisReturn : CalleeSavedRegs<(add PC, LR, SP, R11, R9, R7,
+                                         R6, R5, R4, R0)>;
+
+def CSR_RISCOS_APCS32_REAL : CalleeSavedRegs<(add LR, R11, R9, R7,
+                                         R6, R5, R4)>;
 
