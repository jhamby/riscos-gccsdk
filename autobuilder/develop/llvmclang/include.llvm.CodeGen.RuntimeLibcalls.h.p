--- include/llvm/CodeGen/RuntimeLibcalls.h.orig	2014-07-17 12:12:12.000000000 +0100
+++ include/llvm/CodeGen/RuntimeLibcalls.h	2014-11-15 20:59:52.107639596 +0000
@@ -389,6 +389,10 @@
     // Stack Protector Fail.
     STACKPROTECTOR_CHECK_FAIL,
 
+    ALLOCA,
+    ALLOCA_SAVE,
+    ALLOCA_RESTORE,
+
     UNKNOWN_LIBCALL
   };
 
