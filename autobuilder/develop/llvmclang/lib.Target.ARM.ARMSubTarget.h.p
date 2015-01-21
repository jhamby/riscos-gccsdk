--- lib/Target/ARM/ARMSubtarget.h.orig	2014-07-16 21:13:31.000000000 +0100
+++ lib/Target/ARM/ARMSubtarget.h	2014-12-19 19:33:16.383898075 +0000
@@ -205,6 +205,9 @@
   /// Target machine allowed unsafe FP math (such as use of NEON fp)
   bool UnsafeFPMath;
 
+  bool UseAPCSStackCheck;
+  bool UseEmbededFunctionNames;
+
   /// stackAlignment - The minimum alignment known to hold of the stack frame on
   /// entry to the function and which must be maintained by every function.
   unsigned stackAlignment;
@@ -349,6 +352,7 @@
   bool isTargetNaCl() const { return TargetTriple.isOSNaCl(); }
   bool isTargetNetBSD() const { return TargetTriple.getOS() == Triple::NetBSD; }
   bool isTargetWindows() const { return TargetTriple.isOSWindows(); }
+  bool isTargetRISCOS() const { return TargetTriple.isOSRISCOS(); }
 
   bool isTargetCOFF() const { return TargetTriple.isOSBinFormatCOFF(); }
   bool isTargetELF() const { return TargetTriple.isOSBinFormatELF(); }
@@ -374,7 +378,8 @@
             TargetTriple.getEnvironment() == Triple::GNUEABI ||
             TargetTriple.getEnvironment() == Triple::EABIHF ||
             TargetTriple.getEnvironment() == Triple::GNUEABIHF ||
-            TargetTriple.getEnvironment() == Triple::Android) &&
+            TargetTriple.getEnvironment() == Triple::Android ||
+            isTargetRISCOS()) &&
            !isTargetDarwin() && !isTargetWindows();
   }
 
@@ -444,6 +449,8 @@
   /// symbol.
   bool GVIsIndirectSymbol(const GlobalValue *GV, Reloc::Model RelocM) const;
 
+  bool APCSStackCheck() const { return UseAPCSStackCheck; }
+  bool PokeFunctionName() const { return UseEmbededFunctionNames; }
 };
 } // End llvm namespace
 
