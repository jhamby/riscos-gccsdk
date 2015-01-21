--- lib/Target/ARM/ARMSubtarget.cpp.orig	2014-07-16 21:13:31.000000000 +0100
+++ lib/Target/ARM/ARMSubtarget.cpp	2014-12-15 20:21:30.195075672 +0000
@@ -50,6 +50,16 @@
 UseFusedMulOps("arm-use-mulops",
                cl::init(true), cl::Hidden);
 
+static cl::opt<bool>
+mPokeFunctionName("embed-function-names",
+		  cl::Hidden,
+		  cl::desc("Precede each function with its name in object code"));
+
+static cl::opt<bool>
+mAPCSStackCheck("apcs-stack-check",
+		cl::Hidden,
+		cl::desc("Use stack size checking"));
+
 enum AlignMode {
   DefaultAlign,
   StrictAlign,
@@ -216,6 +226,8 @@
   Thumb2DSP = false;
   UseNaClTrap = false;
   UnsafeFPMath = false;
+  UseAPCSStackCheck = mAPCSStackCheck;
+  UseEmbededFunctionNames = mPokeFunctionName;
 }
 
 void ARMSubtarget::resetSubtargetFeatures(const MachineFunction *MF) {
@@ -307,6 +319,9 @@
     SupportsTailCall = !isThumb1Only();
   }
 
+  if (isTargetRISCOS())
+    SupportsTailCall = false;
+
   switch (Align) {
     case DefaultAlign:
       // Assume pre-ARMv6 doesn't support unaligned accesses.
@@ -340,6 +355,9 @@
       break;
   }
 
+  if (isTargetRISCOS() && !HasV4TOps)
+    AllowsUnalignedMem = false;
+
   switch (IT) {
   case DefaultIT:
     RestrictIT = hasV8Ops() ? true : false;
@@ -357,6 +375,11 @@
   if ((Bits & ARM::ProcA5 || Bits & ARM::ProcA8) && // Where this matters
       (Options.UnsafeFPMath || isTargetDarwin()))
     UseNEONForSinglePrecisionFP = true;
+
+  if (isTargetRISCOS()) {
+    UseAPCSStackCheck = mAPCSStackCheck;
+    UseEmbededFunctionNames = mPokeFunctionName;
+  }
 }
 
 /// GVIsIndirectSymbol - true if the GV will be accessed via an indirect symbol.
