--- lib/CodeGen/TargetLoweringBase.cpp.orig	2014-07-21 10:13:56.000000000 +0100
+++ lib/CodeGen/TargetLoweringBase.cpp	2014-11-15 21:00:02.799639735 +0000
@@ -409,6 +409,16 @@
     // These are generally not available.
     Names[RTLIB::STACKPROTECTOR_CHECK_FAIL] = nullptr;
   }
+
+  if (TT.getOS() == Triple::RISCOS) {
+    Names[RTLIB::ALLOCA] = "__llvm_alloca";
+    Names[RTLIB::ALLOCA_SAVE] = "__llvm_alloca_save";
+    Names[RTLIB::ALLOCA_RESTORE] = "__llvm_alloca_restore";
+  } else {
+    Names[RTLIB::ALLOCA] = nullptr;
+    Names[RTLIB::ALLOCA_SAVE] = nullptr;
+    Names[RTLIB::ALLOCA_RESTORE] = nullptr;
+  }
 }
 
 /// InitLibcallCallingConvs - Set default libcall CallingConvs.
