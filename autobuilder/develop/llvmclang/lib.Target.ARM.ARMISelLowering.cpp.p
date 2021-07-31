--- lib/Target/ARM/ARMISelLowering.cpp.orig	2014-08-20 18:42:35.000000000 +0100
+++ lib/Target/ARM/ARMISelLowering.cpp	2014-12-09 21:10:20.796516382 +0000
@@ -714,7 +714,8 @@
     setExceptionSelectorRegister(ARM::R1);
   }
 
-  if (Subtarget->getTargetTriple().isWindowsItaniumEnvironment())
+  if (Subtarget->getTargetTriple().isWindowsItaniumEnvironment() ||
+      Subtarget->isTargetRISCOS())
     setOperationAction(ISD::DYNAMIC_STACKALLOC, MVT::i32, Custom);
   else
     setOperationAction(ISD::DYNAMIC_STACKALLOC, MVT::i32, Expand);
@@ -2445,7 +2446,9 @@
   SDLoc dl(Op);
   const GlobalValue *GV = cast<GlobalAddressSDNode>(Op)->getGlobal();
   if (getTargetMachine().getRelocationModel() == Reloc::PIC_) {
-    bool UseGOTOFF = GV->hasLocalLinkage() || GV->hasHiddenVisibility();
+    bool UseGOTOFF = (GV->hasLocalLinkage() || GV->hasHiddenVisibility()) &&
+                       !getSubtarget()->isTargetRISCOS();
+
     ARMConstantPoolValue *CPV =
       ARMConstantPoolConstant::Create(GV,
                                       UseGOTOFF ? ARMCP::GOTOFF : ARMCP::GOT);
@@ -2542,9 +2545,16 @@
   EVT PtrVT = getPointerTy();
   SDLoc dl(Op);
   unsigned PCAdj = Subtarget->isThumb() ? 4 : 8;
-  ARMConstantPoolValue *CPV =
-    ARMConstantPoolSymbol::Create(*DAG.getContext(), "_GLOBAL_OFFSET_TABLE_",
+  ARMConstantPoolValue *CPV;
+  if (Subtarget->isTargetRISCOS()) {
+    MCSymbol *Sym = MF.getContext().GetOrCreateSymbol(StringRef("__GOTT_BASE__"));
+    MCSymbolRefExpr::Create(Sym, MCSymbolRefExpr::VariantKind::VK_None, MF.getContext());
+    CPV = ARMConstantPoolSymbol::Create(*DAG.getContext(), "__GOTT_BASE__",
+                                  ARMPCLabelIndex, 0);
+  } else {
+    CPV = ARMConstantPoolSymbol::Create(*DAG.getContext(), "_GLOBAL_OFFSET_TABLE_",
                                   ARMPCLabelIndex, PCAdj);
+  }
   SDValue CPAddr = DAG.getTargetConstantPool(CPV, PtrVT, 4);
   CPAddr = DAG.getNode(ARMISD::Wrapper, dl, MVT::i32, CPAddr);
   SDValue Result = DAG.getLoad(PtrVT, dl, DAG.getEntryNode(), CPAddr,
@@ -6266,8 +6276,10 @@
   case ISD::SDIVREM:
   case ISD::UDIVREM:       return LowerDivRem(Op, DAG);
   case ISD::DYNAMIC_STACKALLOC:
-    if (Subtarget->getTargetTriple().isWindowsItaniumEnvironment())
+    if (Subtarget->getTargetTriple().isWindowsItaniumEnvironment() ||
+        Subtarget->isTargetRISCOS())
       return LowerDYNAMIC_STACKALLOC(Op, DAG);
+
     llvm_unreachable("Don't know how to custom lower this!");
   }
 }
@@ -10607,7 +10619,37 @@
 
 SDValue
 ARMTargetLowering::LowerDYNAMIC_STACKALLOC(SDValue Op, SelectionDAG &DAG) const {
-  assert(Subtarget->isTargetWindows() && "unsupported target platform");
+  assert((Subtarget->isTargetWindows() || Subtarget->isTargetRISCOS()) && "unsupported target platform");
+
+  if (Subtarget->isTargetRISCOS()) {
+    SDLoc DL(Op);
+    // Get the inputs.
+    SDValue Chain = Op.getOperand(0);
+    SDValue Size  = Op.getOperand(1);
+
+    TargetLowering::ArgListTy Args;
+    TargetLowering::ArgListEntry Entry;
+    EVT ArgVT = Size.getValueType();
+    Type *ArgTy = ArgVT.getTypeForEVT(*DAG.getContext());
+    Entry.Node = Size;
+    Entry.Ty = ArgTy;
+    Entry.isSExt = true;
+    Entry.isZExt = false;
+    Args.push_back(Entry);
+
+    Type *RetTy = Op.getValueType().getTypeForEVT(*DAG.getContext());
+
+    SDValue Callee = DAG.getExternalSymbol(getLibcallName(RTLIB::ALLOCA),
+                                           getPointerTy());
+    TargetLowering::CallLoweringInfo CLI(DAG);
+    CLI.setDebugLoc(SDLoc(Op)).setChain(Chain)
+      .setCallee(getLibcallCallingConv(RTLIB::ALLOCA), RetTy, Callee, std::move(Args), 0)
+      .setSExtResult(false).setZExtResult(true);
+
+    std::pair<SDValue, SDValue> CallInfo = LowerCallTo(CLI);
+    return CallInfo.first;
+  }
+
   SDLoc DL(Op);
 
   // Get the inputs.
