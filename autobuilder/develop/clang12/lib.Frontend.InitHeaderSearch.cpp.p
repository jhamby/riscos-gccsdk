--- lib/Frontend/InitHeaderSearch.cpp.orig	2021-06-28 09:23:38.000000000 -0700
+++ lib/Frontend/InitHeaderSearch.cpp	2021-08-09 17:42:00.308857228 -0700
@@ -229,6 +229,7 @@
     case llvm::Triple::PS4:
     case llvm::Triple::ELFIAMCU:
     case llvm::Triple::Fuchsia:
+    case llvm::Triple::RISCOS:
       break;
     case llvm::Triple::Win32:
       if (triple.getEnvironment() != llvm::Triple::Cygnus)
@@ -338,6 +339,7 @@
   case llvm::Triple::NaCl:
   case llvm::Triple::ELFIAMCU:
   case llvm::Triple::Fuchsia:
+  case llvm::Triple::RISCOS:
     break;
   case llvm::Triple::PS4: {
     // <isysroot> gets prepended later in AddPath().
