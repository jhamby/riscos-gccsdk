--- lib/Target/ARM/ARMInstrInfo.td.orig	2014-07-18 13:05:49.000000000 +0100
+++ lib/Target/ARM/ARMInstrInfo.td	2014-12-12 19:40:16.924838773 +0000
@@ -2010,8 +2010,9 @@
 
 // Address computation and loads and stores in PIC mode.
 let isNotDuplicable = 1 in {
+// RISCOS: Changed size of PICADD from 4 to 8 (two instructions generated).
 def PICADD  : ARMPseudoInst<(outs GPR:$dst), (ins GPR:$a, pclabel:$cp, pred:$p),
-                            4, IIC_iALUr,
+                            8/*4*/, IIC_iALUr,
                             [(set GPR:$dst, (ARMpic_add GPR:$a, imm:$cp))]>,
                             Sched<[WriteALU, ReadALU]>;
 
