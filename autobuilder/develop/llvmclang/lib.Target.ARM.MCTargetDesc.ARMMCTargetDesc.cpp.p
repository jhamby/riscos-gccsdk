--- lib/Target/ARM/MCTargetDesc/ARMMCTargetDesc.cpp.orig	2014-07-18 13:13:04.000000000 +0100
+++ lib/Target/ARM/MCTargetDesc/ARMMCTargetDesc.cpp	2015-01-18 14:37:54.655460601 +0000
@@ -25,6 +25,7 @@
 #include "llvm/MC/MCSubtargetInfo.h"
 #include "llvm/Support/ErrorHandling.h"
 #include "llvm/Support/TargetRegistry.h"
+#include "llvm/Support/ELF.h"
 
 using namespace llvm;
 
@@ -281,8 +282,18 @@
     assert(TheTriple.isOSWindows() && "non-Windows ARM COFF is not supported");
     return createARMWinCOFFStreamer(Ctx, MAB, *Emitter, OS);
   case Triple::ELF:
-    return createARMELFStreamer(Ctx, MAB, OS, Emitter, false, NoExecStack,
-                                TheTriple.getArch() == Triple::thumb);
+    if (TheTriple.getOS() == Triple::RISCOS) {
+      MCELFStreamer *S = createARMELFStreamer(Ctx, MAB, OS, Emitter, /*RelaxAll=*/false,
+					      NoExecStack, /*IsThumb=*/false);
+      // Make compatible with RISC OS GCCSDK GAS, ie, EABI unknown instead of version
+      // 5 and soft float by default.
+      S->getAssembler().setELFHeaderEFlags(ELF::EF_ARM_EABI_UNKNOWN |
+					   ELF::EF_ARM_SOFT_FLOAT);
+      return S;
+    }
+    else
+      return createARMELFStreamer(Ctx, MAB, OS, Emitter, false, NoExecStack,
+                                  TheTriple.getArch() == Triple::thumb);
   }
 }
 
