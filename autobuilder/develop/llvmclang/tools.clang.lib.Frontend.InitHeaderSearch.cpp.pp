--- tools/clang/lib/Frontend/InitHeaderSearch.cpp.orig	2014-06-10 22:07:12.000000000 +0100
+++ tools/clang/lib/Frontend/InitHeaderSearch.cpp	2015-02-02 20:59:50.876396373 +0000
@@ -231,6 +231,7 @@
     case llvm::Triple::NetBSD:
     case llvm::Triple::OpenBSD:
     case llvm::Triple::Bitrig:
+    case llvm::Triple::RISCOS:
       break;
     default:
       // FIXME: temporary hack: hard-coded paths.
@@ -258,7 +259,11 @@
   StringRef CIncludeDirs(C_INCLUDE_DIRS);
   if (CIncludeDirs != "") {
     SmallVector<StringRef, 5> dirs;
+#ifdef __riscos__
+    CIncludeDirs.split(dirs, ",");
+#else
     CIncludeDirs.split(dirs, ":");
+#endif
     for (SmallVectorImpl<StringRef>::iterator i = dirs.begin();
          i != dirs.end();
          ++i)
@@ -451,6 +456,10 @@
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
