--- lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp.orig	2014-07-21 10:13:56.000000000 +0100
+++ lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp	2015-01-09 17:45:01.760493048 +0000
@@ -147,7 +147,9 @@
 
   // Mask = ~(1 << (Size-1))
   APInt API = APInt::getAllOnesValue(Size);
-  API.clearBit(Size-1);
+  API.clearBit(Size == 64 && DAG.getTarget().hasBigEndianDoubles() ?
+               (Size/2)-1 :
+               Size-1);
   SDValue Mask = DAG.getConstant(API, NVT);
   SDValue Op = GetSoftenedFloat(N->getOperand(0));
   return DAG.getNode(ISD::AND, SDLoc(N), NVT, Op, Mask);
