--- lib/Target/ARM/InstPrinter/ARMInstPrinter.cpp.orig	2014-06-11 21:26:45.000000000 +0100
+++ lib/Target/ARM/InstPrinter/ARMInstPrinter.cpp	2014-11-05 21:32:38.021283968 +0000
@@ -20,6 +20,9 @@
 #include "llvm/MC/MCInstrInfo.h"
 #include "llvm/MC/MCRegisterInfo.h"
 #include "llvm/Support/raw_ostream.h"
+
+#include "llvm/MC/MCSymbol.h"
+
 using namespace llvm;
 
 #define DEBUG_TYPE "asm-printer"
@@ -1083,6 +1086,20 @@
 
   O << markup("<mem:") << "[";
   printRegName(O, MO1.getReg());
+  
+/*  if (STI.isTargetRISCOS())*/ {
+    if (MO2.isExpr()) {
+      const MCExpr *Expr = MO2.getExpr();
+      if (Expr->getKind() == MCExpr::SymbolRef) {
+	const MCSymbolRefExpr &Ref = cast<MCSymbolRefExpr>(*Expr);
+	const MCSymbol &Sym = Ref.getSymbol();
+	if (Sym.getName() == "__GOTT_INDEX__") {
+	  O << ", #__GOTT_INDEX__]";
+	  return;
+	}
+     }
+    }
+  }
 
   int32_t OffImm = (int32_t)MO2.getImm();
   bool isSub = OffImm < 0;
