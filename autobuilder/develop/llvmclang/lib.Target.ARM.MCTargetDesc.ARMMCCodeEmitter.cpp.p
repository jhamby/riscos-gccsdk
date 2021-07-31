--- lib/Target/ARM/MCTargetDesc/ARMMCCodeEmitter.cpp.orig	2014-06-19 07:10:58.000000000 +0100
+++ lib/Target/ARM/MCTargetDesc/ARMMCCodeEmitter.cpp	2014-11-10 19:20:51.786316926 +0000
@@ -28,6 +28,8 @@
 #include "llvm/Support/ErrorHandling.h"
 #include "llvm/Support/raw_ostream.h"
 
+#include "llvm/MC/MCSymbol.h"
+
 using namespace llvm;
 
 #define DEBUG_TYPE "mccodeemitter"
@@ -60,6 +62,10 @@
     Triple TT(STI.getTargetTriple());
     return TT.isOSBinFormatMachO();
   }
+  bool isTargetRISCOS(const MCSubtargetInfo &STI) const {
+    Triple TT(STI.getTargetTriple());
+    return TT.getOS() == Triple::RISCOS;
+  }
 
   unsigned getMachineSoImmOpValue(unsigned SoImm) const;
 
@@ -528,6 +534,8 @@
   } else if (MO.isFPImm()) {
     return static_cast<unsigned>(APFloat(MO.getFPImm())
                      .bitcastToAPInt().getHiBits(32).getLimitedValue());
+  } else if (MO.isExpr() && isTargetRISCOS(STI)) {
+    return 0;
   }
 
   llvm_unreachable("Unable to encode MCOperand!");
@@ -543,6 +551,19 @@
 
   Reg = CTX.getRegisterInfo()->getEncodingValue(MO.getReg());
 
+  if (MO1.isExpr() && isTargetRISCOS(STI)) {
+    const MCExpr *Expr = MO1.getExpr();
+    if (Expr->getKind() == MCExpr::SymbolRef) {
+      const MCSymbolRefExpr &Ref = cast<MCSymbolRefExpr>(*Expr);
+      const MCSymbol &Sym = Ref.getSymbol();
+      if (Sym.getName() == "__GOTT_INDEX__") {
+        Fixups.push_back(MCFixup::Create(0, Expr, MCFixupKind(ARM::fixup_riscos_gott_index)));
+        Imm = 0;
+        return true;
+      }
+    }
+  }
+
   int32_t SImm = MO1.getImm();
   bool isAdd = true;
 
