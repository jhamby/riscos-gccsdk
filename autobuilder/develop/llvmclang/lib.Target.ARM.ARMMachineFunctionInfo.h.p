--- lib/Target/ARM/ARMMachineFunctionInfo.h.orig	2014-07-15 18:18:41.000000000 +0100
+++ lib/Target/ARM/ARMMachineFunctionInfo.h	2014-12-02 20:50:46.036462523 +0000
@@ -60,6 +60,9 @@
   /// enable far jump.
   bool LRSpilledForFarJump;
 
+  /// HasStackExtension - True if this function requires a stack extension.
+  bool HasStackExtension;
+
   /// FramePtrSpillOffset - If HasStackFrame, this records the frame pointer
   /// spill stack offset.
   unsigned FramePtrSpillOffset;
@@ -128,7 +131,7 @@
     isThumb(false),
     hasThumb2(false),
     ArgRegsSaveSize(0), HasStackFrame(false), RestoreSPFromFP(false),
-    LRSpilledForFarJump(false),
+    LRSpilledForFarJump(false), HasStackExtension(false),
     FramePtrSpillOffset(0), GPRCS1Offset(0), GPRCS2Offset(0), DPRCSOffset(0),
     GPRCS1Size(0), GPRCS2Size(0), DPRCSSize(0),
     NumAlignedDPRCS2Regs(0),
@@ -185,6 +188,9 @@
   unsigned getArgumentStackSize() const { return ArgumentStackSize; }
   void setArgumentStackSize(unsigned size) { ArgumentStackSize = size; }
 
+  bool hasStackExtension() const { return HasStackExtension; }
+  void setHasStackExtension(bool s) { HasStackExtension = s; }
+
   unsigned createJumpTableUId() {
     return JumpTableUId++;
   }
