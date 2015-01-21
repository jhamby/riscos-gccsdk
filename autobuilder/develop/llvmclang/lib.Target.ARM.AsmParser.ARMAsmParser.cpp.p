--- lib/Target/ARM/AsmParser/ARMAsmParser.cpp.orig	2014-06-26 14:10:53.000000000 +0100
+++ lib/Target/ARM/AsmParser/ARMAsmParser.cpp	2014-11-10 20:08:29.678353960 +0000
@@ -274,6 +274,11 @@
     return STI.getFeatureBits() & ARM::FeatureMClass;
   }
 
+  bool isTargetRISCOS() const {
+    Triple TT(STI.getTargetTriple());
+    return TT.getOS() == Triple::RISCOS;
+  }
+
   /// @name Auto-generated Match Functions
   /// {
 
@@ -466,6 +471,7 @@
     // Offset is in OffsetReg or OffsetImm. If both are zero, no offset
     // was specified.
     const MCConstantExpr *OffsetImm;  // Offset immediate value
+    const MCSymbolRefExpr *OffsetExpr; // RISCOS: To record __GOTT_INDEX__ as immediate operand
     unsigned OffsetRegNum;    // Offset register num, when OffsetImm == NULL
     ARM_AM::ShiftOpc ShiftType; // Shift type for OffsetReg
     unsigned ShiftImm;        // shift for OffsetReg.
@@ -2223,7 +2229,10 @@
     // Otherwise, it's a normal memory reg+offset.
     int64_t Val = Memory.OffsetImm ? Memory.OffsetImm->getValue() : 0;
     Inst.addOperand(MCOperand::CreateReg(Memory.BaseRegNum));
-    Inst.addOperand(MCOperand::CreateImm(Val));
+    if (Memory.OffsetExpr)
+      Inst.addOperand(MCOperand::CreateExpr(Memory.OffsetExpr));
+    else
+      Inst.addOperand(MCOperand::CreateImm(Val));
   }
 
   void addMemTBBOperands(MCInst &Inst, unsigned N) const {
@@ -2678,6 +2687,7 @@
     auto Op = make_unique<ARMOperand>(k_Memory);
     Op->Memory.BaseRegNum = BaseRegNum;
     Op->Memory.OffsetImm = OffsetImm;
+    Op->Memory.OffsetExpr = nullptr;
     Op->Memory.OffsetRegNum = OffsetRegNum;
     Op->Memory.ShiftType = ShiftType;
     Op->Memory.ShiftImm = ShiftImm;
@@ -2689,6 +2699,27 @@
     return Op;
   }
 
+  // RISCOS: For adding a __GOTT_INDEX__ expression as an operand when
+  // parsing PIC.
+  static std::unique_ptr<ARMOperand>
+  CreateMem(unsigned BaseRegNum, const MCSymbolRefExpr *ExprImm,
+            SMLoc S,
+            SMLoc E, SMLoc AlignmentLoc = SMLoc()) {
+    auto Op = make_unique<ARMOperand>(k_Memory);
+    Op->Memory.BaseRegNum = BaseRegNum;
+    Op->Memory.OffsetImm = nullptr;
+    Op->Memory.OffsetExpr = ExprImm;
+    Op->Memory.OffsetRegNum = 0;
+    Op->Memory.ShiftType = ARM_AM::no_shift;
+    Op->Memory.ShiftImm = 0;
+    Op->Memory.Alignment = 0;
+    Op->Memory.isNegative = false;
+    Op->StartLoc = S;
+    Op->EndLoc = E;
+    Op->AlignmentLoc = AlignmentLoc;
+    return Op;
+  }
+
   static std::unique_ptr<ARMOperand>
   CreatePostIdxReg(unsigned RegNum, bool isAdd, ARM_AM::ShiftOpc ShiftTy,
                    unsigned ShiftImm, SMLoc S, SMLoc E) {
@@ -4565,6 +4596,19 @@
     if (getParser().parseExpression(Offset))
      return true;
 
+    if (Offset->getKind() == MCExpr::SymbolRef && isTargetRISCOS()) {
+      const MCSymbolRefExpr &SymRef = cast<MCSymbolRefExpr>(*Offset);
+      if (SymRef.getSymbol().getName() == "__GOTT_INDEX__") {
+        // Now we should have the closing ']'
+        if (Parser.getTok().isNot(AsmToken::RBrac))
+          return Error(Parser.getTok().getLoc(), "']' expected");
+        E = Parser.getTok().getEndLoc();
+        Parser.Lex(); // Eat right bracket token.
+        Operands.push_back(ARMOperand::CreateMem(BaseRegNum, &SymRef,S, E));
+        return false;
+      }
+    }
+
     // The expression has to be a constant. Memory references with relocations
     // don't come through here, as they use the <label> forms of the relevant
     // instructions.
