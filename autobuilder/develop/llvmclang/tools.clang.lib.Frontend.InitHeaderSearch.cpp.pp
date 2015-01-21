--- tools/clang/lib/Frontend/InitHeaderSearch.cpp.orig	2014-06-10 22:07:12.000000000 +0100
+++ tools/clang/lib/Frontend/InitHeaderSearch.cpp	2015-01-12 19:50:53.089659814 +0000
@@ -231,6 +231,7 @@
     case llvm::Triple::NetBSD:
     case llvm::Triple::OpenBSD:
     case llvm::Triple::Bitrig:
+    case llvm::Triple::RISCOS:
       break;
     default:
       // FIXME: temporary hack: hard-coded paths.
@@ -451,6 +452,10 @@
     AddGnuCPlusPlusIncludePaths("/opt/gcc4/include/c++/4.2.4",
                                 "i386-pc-solaris2.11", "", "", triple);
     break;
+  case llvm::Triple::RISCOS:
+    AddGnuCPlusPlusIncludePaths("GCCSDK_CROSS_C++_HEADERS/",
+                                "arm-unknown-riscos", "", "", triple);
+    break;
   default:
     break;
   }
