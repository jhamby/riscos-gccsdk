--- lib/Target/ARM/ARMAsmPrinter.cpp.orig	2014-07-07 06:18:30.000000000 +0100
+++ lib/Target/ARM/ARMAsmPrinter.cpp	2014-12-21 19:10:08.460301134 +0000
@@ -72,6 +72,18 @@
     OutStreamer.EmitThumbFunc(CurrentFnSym);
   }
 
+  if (Subtarget->isTargetRISCOS() && Subtarget->PokeFunctionName()) {
+    SmallString<256> terminated_name(CurrentFnSym->getName());
+
+    terminated_name += '\0';
+
+    int rounded_length = (terminated_name.size() + 3) & ~3;
+    
+    OutStreamer.EmitBytes(terminated_name);
+    OutStreamer.EmitValueToAlignment(4);
+    OutStreamer.EmitIntValue(0xff000000 + rounded_length, 4);
+  }
+
   OutStreamer.EmitLabel(CurrentFnSym);
 }
 
@@ -1074,11 +1086,19 @@
       RegList.push_back(SrcReg);
       break;
     }
-    if (MAI->getExceptionHandlingType() == ExceptionHandling::ARM)
+    if (MAI->getExceptionHandlingType() == ExceptionHandling::ARM) {
+      if (Subtarget->isTargetRISCOS()) {
+        if (RegList.back() == ARM::PC) {
+          RegList.pop_back();
+          ATS.emitPad(4);
+        }
+      }
+
       ATS.emitRegSave(RegList, Opc == ARM::VSTMDDB_UPD);
+    }
   } else {
     // Changes of stack / frame pointer.
-    if (SrcReg == ARM::SP) {
+    if (SrcReg == ARM::SP || (Subtarget->isTargetRISCOS() && SrcReg == ARM::R12)) {
       int64_t Offset = 0;
       switch (Opc) {
       default:
@@ -1124,7 +1144,7 @@
         if (DstReg == FramePtr && FramePtr != ARM::SP)
           // Set-up of the frame pointer. Positive values correspond to "add"
           // instruction.
-          ATS.emitSetFP(FramePtr, ARM::SP, -Offset);
+          ATS.emitSetFP(FramePtr, Subtarget->isTargetRISCOS() ? ARM::R12 : ARM::SP, -Offset);
         else if (DstReg == ARM::SP) {
           // Change of SP by an offset. Positive values correspond to "sub"
           // instruction.
@@ -1367,6 +1387,30 @@
     return;
   }
   case ARM::PICADD: {
+    if (Subtarget->isTargetRISCOS()) {
+      // This is a pseudo op for a double load sequence, which looks like:
+      //   LDR r0, [r0, #0]
+      //   LDR r0, [r0, #__GOTT_INDEX__]
+      // where r0 is the PIC register or whatever register was chosen by the compiler.
+      EmitToStreamer(OutStreamer, MCInstBuilder(ARM::LDRi12)
+        .addReg(MI->getOperand(1).getReg())
+        .addReg(MI->getOperand(1).getReg())
+        .addImm(0)
+        // Add predicate operands.
+        .addImm(MI->getOperand(3).getImm())
+        .addReg(MI->getOperand(4).getReg()));
+      MCSymbol *Sym = OutContext.GetOrCreateSymbol(StringRef("__GOTT_INDEX__"));
+      const MCExpr *Expr =
+          MCSymbolRefExpr::Create(Sym, MCSymbolRefExpr::VariantKind::VK_None, OutContext);
+
+      EmitToStreamer(OutStreamer, MCInstBuilder(ARM::LDRi12)
+        .addReg(MI->getOperand(0).getReg())
+        .addReg(MI->getOperand(1).getReg())
+        .addExpr(Expr)
+        // Add predicate operands.
+        .addImm(MI->getOperand(3).getImm())
+        .addReg(MI->getOperand(4).getReg()));
+    } else {
     // This is a pseudo op for a label + instruction sequence, which looks like:
     // LPC0:
     //     add r0, pc, r0
@@ -1387,6 +1431,7 @@
       .addReg(MI->getOperand(4).getReg())
       // Add 's' bit operand (always reg0 for this)
       .addReg(0));
+    }
     return;
   }
   case ARM::PICSTR:
