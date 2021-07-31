--- tools/clang/lib/Driver/Driver.cpp.orig	2014-07-11 15:28:45.000000000 +0100
+++ tools/clang/lib/Driver/Driver.cpp	2014-11-30 16:25:47.277369606 +0000
@@ -2006,6 +2006,9 @@
         break;
       }
       break;
+    case llvm::Triple::RISCOS:
+      TC = new toolchains::RISCOS(*this, Target, Args);
+      break;
     default:
       // TCE is an OSless target
       if (Target.getArchName() == "tce") {
