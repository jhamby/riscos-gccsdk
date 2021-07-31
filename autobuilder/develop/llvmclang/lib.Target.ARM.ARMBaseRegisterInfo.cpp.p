--- lib/Target/ARM/ARMBaseRegisterInfo.cpp.orig	2014-07-16 21:13:31.000000000 +0100
+++ lib/Target/ARM/ARMBaseRegisterInfo.cpp	2014-11-29 18:23:29.692072633 +0000
@@ -64,6 +64,9 @@
                                 ? CSR_iOS_SaveList
                                 : CSR_AAPCS_SaveList;
 
+  if (STI.isTargetRISCOS())
+    RegList = CSR_RISCOS_APCS32_SaveList;
+
   if (!MF) return RegList;
 
   const Function *F = MF->getFunction();
@@ -95,6 +98,8 @@
   if (CC == CallingConv::GHC)
     // This is academic becase all GHC calls are (supposed to be) tail calls
     return CSR_NoRegs_RegMask;
+  if (STI.isTargetRISCOS())
+    return CSR_RISCOS_APCS32_REAL_RegMask;
   return (STI.isTargetIOS() && !STI.isAAPCS_ABI())
     ? CSR_iOS_RegMask : CSR_AAPCS_RegMask;
 }
@@ -117,6 +122,8 @@
   if (CC == CallingConv::GHC)
     // This is academic becase all GHC calls are (supposed to be) tail calls
     return nullptr;
+  if (STI.isTargetRISCOS())
+    return CSR_RISCOS_APCS32_ThisReturn_RegMask;
   return (STI.isTargetIOS() && !STI.isAAPCS_ABI())
     ? CSR_iOS_ThisReturn_RegMask : CSR_AAPCS_ThisReturn_RegMask;
 }
@@ -138,6 +145,8 @@
   // Some targets reserve R9.
   if (STI.isR9Reserved())
     Reserved.set(ARM::R9);
+  if (STI.isTargetRISCOS() && STI.APCSStackCheck())
+    Reserved.set(ARM::R10);
   // Reserve D16-D31 if the subtarget doesn't support them.
   if (!STI.hasVFP3() || STI.hasD16()) {
     assert(ARM::D31 == ARM::D16 + 15);
