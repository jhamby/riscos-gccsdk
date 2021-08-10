--- include/llvm/ADT/Triple.h.orig	2021-06-28 09:23:38.000000000 -0700
+++ include/llvm/ADT/Triple.h	2021-08-09 13:02:18.558138606 -0700
@@ -198,7 +198,8 @@
     Hurd,       // GNU/Hurd
     WASI,       // Experimental WebAssembly OS
     Emscripten,
-    LastOSType = Emscripten
+    RISCOS,
+    LastOSType = RISCOS
   };
   enum EnvironmentType {
     UnknownEnvironment,
