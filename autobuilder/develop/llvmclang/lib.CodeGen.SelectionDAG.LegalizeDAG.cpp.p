--- lib/CodeGen/SelectionDAG/LegalizeDAG.cpp.orig	2014-07-17 12:12:12.000000000 +0100
+++ lib/CodeGen/SelectionDAG/LegalizeDAG.cpp	2014-12-14 20:10:53.832415915 +0000
@@ -529,8 +529,14 @@
   unsigned IncrementSize = NumBits / 8;
   ISD::LoadExtType HiExtType = LD->getExtensionType();
 
+  // RISCOS: LSRSB doesn't work on an emulated StrongARM in RPCEmu (not sure about real hardware).
+  bool no_ldrsb = DAG.getTarget().getTargetCPU() == "strongarm" &&
+                      Alignment == 2 &&            // Halfword loads...
+                      IncrementSize == 1 &&        // ...using byte loads...
+                      HiExtType == ISD::SEXTLOAD;  // ...that are signed
+
   // If the original load is NON_EXTLOAD, the hi part load must be ZEXTLOAD.
-  if (HiExtType == ISD::NON_EXTLOAD)
+  if (HiExtType == ISD::NON_EXTLOAD || no_ldrsb)
     HiExtType = ISD::ZEXTLOAD;
 
   // Load the value in two parts
@@ -565,6 +571,15 @@
   SDValue Result = DAG.getNode(ISD::SHL, dl, VT, Hi, ShiftAmount);
   Result = DAG.getNode(ISD::OR, dl, VT, Result, Lo);
 
+  if (no_ldrsb) {
+    // Sign extend the 16bit result by shifting left 16bits and then arithmetically
+    // shifting back to the right 16bits.
+    SDValue ShiftAmount = DAG.getConstant(16,
+                                          TLI.getShiftAmountTy(Result.getValueType()));
+    Result = DAG.getNode(ISD::SHL, dl, VT, Result, ShiftAmount);
+    Result = DAG.getNode(ISD::SRA, dl, VT, Result, ShiftAmount);
+  }
+
   SDValue TF = DAG.getNode(ISD::TokenFactor, dl, MVT::Other, Lo.getValue(1),
                              Hi.getValue(1));
 
@@ -847,7 +862,8 @@
           if (!TLI.allowsUnalignedMemoryAccesses(ST->getMemoryVT(), AS)) {
             Type *Ty = ST->getMemoryVT().getTypeForEVT(*DAG.getContext());
             unsigned ABIAlignment= TLI.getDataLayout()->getABITypeAlignment(Ty);
-            if (ST->getAlignment() < ABIAlignment)
+            if (ST->getAlignment() < ABIAlignment ||
+	        (TM.getTargetCPU() == "strongarm" && ABIAlignment == 2))
               ExpandUnalignedStore(cast<StoreSDNode>(Node), DAG, TLI, this);
           }
           break;
@@ -1085,11 +1101,31 @@
             LD->getMemoryVT().getTypeForEVT(*DAG.getContext());
           unsigned ABIAlignment =
             TLI.getDataLayout()->getABITypeAlignment(Ty);
-          if (LD->getAlignment() < ABIAlignment){
-            ExpandUnalignedLoad(cast<LoadSDNode>(Node),
+          if (LD->getAlignment() < ABIAlignment ||
+	        (TM.getTargetCPU() == "strongarm" && ABIAlignment == 2)){
+	    ExpandUnalignedLoad(cast<LoadSDNode>(Node),
                                 DAG, TLI, Value, Chain);
           }
         }
+        if (MemVT.isSimple() &&
+            MemVT.getSimpleVT() == MVT::i8 &&
+            ExtType == ISD::SEXTLOAD &&
+            TM.getTargetCPU() == "strongarm") {
+	  // Replace LDRSB with an LDRB, a 24bit shift left and a 24bit shift right.
+          EVT VT = LD->getValueType(0);
+          SDValue Load = DAG.getExtLoad(ISD::ZEXTLOAD, dl, VT, LD->getChain(), Ptr,
+                           LD->getPointerInfo(),
+                           LD->getMemoryVT(), LD->isVolatile(),
+                           LD->isNonTemporal(), LD->getAlignment(), LD->getTBAAInfo());
+
+          SDValue ShiftAmount = DAG.getConstant(24,
+                                       TLI.getShiftAmountTy(Load.getValueType()));
+          SDValue Result = DAG.getNode(ISD::SHL, dl, VT, Load, ShiftAmount);
+          Result = DAG.getNode(ISD::SRA, dl, VT, Result, ShiftAmount);
+
+	  Value = Result;
+	  Chain = Load.getValue(1);
+        }
       }
       break;
     }
