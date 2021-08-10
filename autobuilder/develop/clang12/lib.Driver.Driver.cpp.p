--- lib/Driver/Driver.cpp.orig	2021-06-28 09:23:38.000000000 -0700
+++ lib/Driver/Driver.cpp	2021-08-09 17:31:09.830269553 -0700
@@ -40,6 +40,7 @@
 #include "ToolChains/OpenBSD.h"
 #include "ToolChains/PPCLinux.h"
 #include "ToolChains/PS4CPU.h"
+#include "ToolChains/RISCOS.h"
 #include "ToolChains/RISCVToolchain.h"
 #include "ToolChains/Solaris.h"
 #include "ToolChains/TCE.h"
@@ -5176,6 +5177,9 @@
     case llvm::Triple::Hurd:
       TC = std::make_unique<toolchains::Hurd>(*this, Target, Args);
       break;
+    case llvm::Triple::RISCOS:
+      TC = std::make_unique<toolchains::RISCOS>(*this, Target, Args);
+      break;
     case llvm::Triple::ZOS:
       TC = std::make_unique<toolchains::ZOS>(*this, Target, Args);
       break;
