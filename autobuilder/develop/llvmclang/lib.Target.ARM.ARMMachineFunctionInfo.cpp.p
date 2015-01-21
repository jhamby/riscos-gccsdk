--- lib/Target/ARM/ARMMachineFunctionInfo.cpp.orig	2014-06-18 23:48:09.000000000 +0100
+++ lib/Target/ARM/ARMMachineFunctionInfo.cpp	2014-12-02 21:08:33.596493138 +0000
@@ -17,7 +17,7 @@
     : isThumb(MF.getTarget().getSubtarget<ARMSubtarget>().isThumb()),
       hasThumb2(MF.getTarget().getSubtarget<ARMSubtarget>().hasThumb2()),
       StByValParamsPadding(0), ArgRegsSaveSize(0), HasStackFrame(false),
-      RestoreSPFromFP(false), LRSpilledForFarJump(false),
+      RestoreSPFromFP(false), LRSpilledForFarJump(false), HasStackExtension(false),
       FramePtrSpillOffset(0), GPRCS1Offset(0), GPRCS2Offset(0), DPRCSOffset(0),
       GPRCS1Size(0), GPRCS2Size(0), DPRCSSize(0), JumpTableUId(0),
       PICLabelUId(0), VarArgsFrameIndex(0), HasITBlocks(false),
