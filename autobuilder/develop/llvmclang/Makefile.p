--- Makefile.orig	2014-03-25 21:45:41.000000000 +0000
+++ Makefile	2015-01-24 11:37:47.179506004 +0000
@@ -31,7 +31,8 @@
   OPTIONAL_DIRS := tools/clang/utils/TableGen
 else
   DIRS := lib/Support lib/TableGen utils lib/IR lib tools/llvm-shlib \
-          tools/llvm-config tools docs cmake unittests
+          tools/llvm-config tools docs cmake
+#unittests
   OPTIONAL_DIRS := projects bindings
 endif
 
