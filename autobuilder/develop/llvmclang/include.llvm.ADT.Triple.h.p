--- include/llvm/ADT/Triple.h.orig	2014-07-18 15:28:19.000000000 +0100
+++ include/llvm/ADT/Triple.h	2014-10-04 11:16:38.113489257 +0100
@@ -135,7 +135,8 @@
     Bitrig,
     AIX,
     CUDA,       // NVIDIA CUDA
-    NVCL        // NVIDIA OpenCL
+    NVCL,       // NVIDIA OpenCL
+    RISCOS
   };
   enum EnvironmentType {
     UnknownEnvironment,
@@ -414,6 +415,11 @@
     return getOS() == Triple::Linux;
   }
 
+  /// \brief Tests whether the OS is RISC OS
+  bool isOSRISCOS() const {
+    return getOS() == Triple::RISCOS;
+  }
+
   /// \brief Tests whether the OS uses the ELF binary format.
   bool isOSBinFormatELF() const {
     return getObjectFormat() == Triple::ELF;
