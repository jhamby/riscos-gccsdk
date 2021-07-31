--- lib/CodeGen/SelectionDAG/DAGCombiner.cpp.orig	2014-08-12 06:41:11.000000000 +0100
+++ lib/CodeGen/SelectionDAG/DAGCombiner.cpp	2015-01-12 20:37:37.781740247 +0000
@@ -6268,7 +6268,17 @@
                                   N0.getOperand(0));
     AddToWorklist(NewConv.getNode());
 
-    APInt SignBit = APInt::getSignBit(VT.getSizeInBits());
+    APInt SignBit;
+
+    if (VT.getSizeInBits() == 64 &&
+        DAG.getTarget().hasBigEndianDoubles()) {
+      // RISCOS: The words of a 64bit double are swapped, so the sign bit is
+      // bit 31 not bit 63.
+      SignBit = APInt::getOneBitSet(VT.getSizeInBits(),
+                                    (VT.getSizeInBits()/2)-1);
+    } else
+      SignBit = APInt::getSignBit(VT.getSizeInBits());
+
     if (N0.getOpcode() == ISD::FNEG)
       return DAG.getNode(ISD::XOR, SDLoc(N), VT,
                          NewConv, DAG.getConstant(SignBit, VT));
@@ -7263,8 +7273,17 @@
     SDValue Int = N0.getOperand(0);
     EVT IntVT = Int.getValueType();
     if (IntVT.isInteger() && !IntVT.isVector()) {
+      APInt SignBit;
+      if (IntVT.getSizeInBits() == 64 &&
+          DAG.getTarget().hasBigEndianDoubles()) {
+        // RISCOS: The words of a 64bit double are swapped, so the sign bit is
+        // bit 31 not bit 63.
+        SignBit = APInt::getOneBitSet(IntVT.getSizeInBits(),
+                                      (IntVT.getSizeInBits()/2)-1);
+      } else
+        SignBit = APInt::getSignBit(IntVT.getSizeInBits());
       Int = DAG.getNode(ISD::XOR, SDLoc(N0), IntVT, Int,
-              DAG.getConstant(APInt::getSignBit(IntVT.getSizeInBits()), IntVT));
+              DAG.getConstant(SignBit, IntVT));
       AddToWorklist(Int.getNode());
       return DAG.getNode(ISD::BITCAST, SDLoc(N),
                          VT, Int);
@@ -7357,8 +7376,17 @@
     SDValue Int = N0.getOperand(0);
     EVT IntVT = Int.getValueType();
     if (IntVT.isInteger() && !IntVT.isVector()) {
+      APInt SignBit;
+      if (IntVT.getSizeInBits() == 64 &&
+          DAG.getTarget().hasBigEndianDoubles()) {
+        // RISCOS: The words of a 64bit double are swapped, so the sign bit is
+        // bit 31 not bit 63.
+        SignBit = APInt::getOneBitSet(IntVT.getSizeInBits(),
+                                      (IntVT.getSizeInBits()/2)-1);
+      } else
+        SignBit = APInt::getSignBit(IntVT.getSizeInBits());
       Int = DAG.getNode(ISD::AND, SDLoc(N0), IntVT, Int,
-             DAG.getConstant(~APInt::getSignBit(IntVT.getSizeInBits()), IntVT));
+             DAG.getConstant(~SignBit, IntVT));
       AddToWorklist(Int.getNode());
       return DAG.getNode(ISD::BITCAST, SDLoc(N),
                          N->getValueType(0), Int);
