--- lib/Target/ARM/ARMInstrInfo.td.orig	2014-07-18 13:05:49.000000000 +0100
+++ lib/Target/ARM/ARMInstrInfo.td	2015-03-10 20:22:21.120457105 +0000
@@ -2010,8 +2010,9 @@
 
 // Address computation and loads and stores in PIC mode.
 let isNotDuplicable = 1 in {
+// RISCOS: Changed size of PICADD from 4 to 8 (two instructions generated).
 def PICADD  : ARMPseudoInst<(outs GPR:$dst), (ins GPR:$a, pclabel:$cp, pred:$p),
-                            4, IIC_iALUr,
+                            8/*4*/, IIC_iALUr,
                             [(set GPR:$dst, (ARMpic_add GPR:$a, imm:$cp))]>,
                             Sched<[WriteALU, ReadALU]>;
 
@@ -3419,7 +3420,8 @@
 def : ARMPat<(ARMadde GPR:$src, so_imm_not:$imm, CPSR),
              (SBCri   GPR:$src, so_imm_not:$imm)>;
 def : ARMPat<(ARMadde GPR:$src, imm0_65535_neg:$imm, CPSR),
-             (SBCrr   GPR:$src, (MOVi16 (imm_not_XFORM imm:$imm)))>;
+             (SBCrr   GPR:$src, (MOVi16 (imm_not_XFORM imm:$imm)))>,
+             Requires<[IsARM, HasV6T2]>;
 
 // Note: These are implemented in C++ code, because they have to generate
 // ADD/SUBrs instructions, which use a complex pattern that a xform function
