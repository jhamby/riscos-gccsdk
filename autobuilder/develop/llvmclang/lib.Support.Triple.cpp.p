--- lib/Support/Triple.cpp.orig	2014-07-18 15:28:19.000000000 +0100
+++ lib/Support/Triple.cpp	2014-12-01 20:56:43.600532762 +0000
@@ -153,6 +153,7 @@
   case AIX: return "aix";
   case CUDA: return "cuda";
   case NVCL: return "nvcl";
+  case RISCOS: return "riscos";
   }
 
   llvm_unreachable("Invalid OSType");
@@ -332,6 +333,7 @@
     .StartsWith("aix", Triple::AIX)
     .StartsWith("cuda", Triple::CUDA)
     .StartsWith("nvcl", Triple::NVCL)
+    .StartsWith("riscos", Triple::RISCOS)
     .Default(Triple::UnknownOS);
 }
 
@@ -1028,6 +1030,8 @@
     default:
       return "strongarm";
     }
+  case llvm::Triple::RISCOS:
+    return "strongarm";
   default:
     switch (getEnvironment()) {
     case llvm::Triple::EABIHF:
