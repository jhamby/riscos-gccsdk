--- lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp.orig	2014-07-21 10:13:56.000000000 +0100
+++ lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp	2015-01-16 20:06:58.047413366 +0000
@@ -4648,14 +4648,26 @@
   case Intrinsic::vastart:  visitVAStart(I); return nullptr;
   case Intrinsic::vaend:    visitVAEnd(I); return nullptr;
   case Intrinsic::vacopy:   visitVACopy(I); return nullptr;
-  case Intrinsic::returnaddress:
-    setValue(&I, DAG.getNode(ISD::RETURNADDR, sdl, TLI->getPointerTy(),
+  case Intrinsic::returnaddress:{
+    Triple TT(TM.getTargetTriple());
+    if (TT.getOS() == Triple::RISCOS) {
+      return "__builtin_return_address";
+    } else {
+      setValue(&I, DAG.getNode(ISD::RETURNADDR, sdl, TLI->getPointerTy(),
                              getValue(I.getArgOperand(0))));
-    return nullptr;
-  case Intrinsic::frameaddress:
-    setValue(&I, DAG.getNode(ISD::FRAMEADDR, sdl, TLI->getPointerTy(),
+      return nullptr;
+    }
+  }
+  case Intrinsic::frameaddress: {
+    Triple TT(TM.getTargetTriple());
+    if (TT.getOS() == Triple::RISCOS) {
+      return "__builtin_frame_address";
+    } else {
+      setValue(&I, DAG.getNode(ISD::FRAMEADDR, sdl, TLI->getPointerTy(),
                              getValue(I.getArgOperand(0))));
-    return nullptr;
+      return nullptr;
+    }
+  }
   case Intrinsic::read_register: {
     Value *Reg = I.getArgOperand(0);
     SDValue RegName = DAG.getMDNode(cast<MDNode>(Reg));
@@ -5207,17 +5219,27 @@
     return nullptr;
   }
   case Intrinsic::stacksave: {
-    SDValue Op = getRoot();
-    Res = DAG.getNode(ISD::STACKSAVE, sdl,
-                      DAG.getVTList(TLI->getPointerTy(), MVT::Other), Op);
-    setValue(&I, Res);
-    DAG.setRoot(Res.getValue(1));
-    return nullptr;
+    Triple TT(TM.getTargetTriple());
+    if (TT.getOS() == Triple::RISCOS) {
+      return "__llvm_alloca_save";
+    } else {
+      SDValue Op = getRoot();
+      Res = DAG.getNode(ISD::STACKSAVE, sdl,
+                        DAG.getVTList(TLI->getPointerTy(), MVT::Other), Op);
+      setValue(&I, Res);
+      DAG.setRoot(Res.getValue(1));
+      return nullptr;
+    }
   }
   case Intrinsic::stackrestore: {
-    Res = getValue(I.getArgOperand(0));
-    DAG.setRoot(DAG.getNode(ISD::STACKRESTORE, sdl, MVT::Other, getRoot(), Res));
-    return nullptr;
+    Triple TT(TM.getTargetTriple());
+    if (TT.getOS() == Triple::RISCOS) {
+      return "__llvm_alloca_restore";
+    } else {
+      Res = getValue(I.getArgOperand(0));
+      DAG.setRoot(DAG.getNode(ISD::STACKRESTORE, sdl, MVT::Other, getRoot(), Res));
+      return nullptr;
+    }
   }
   case Intrinsic::stackprotector: {
     // Emit code into the DAG to store the stack guard onto the stack.
