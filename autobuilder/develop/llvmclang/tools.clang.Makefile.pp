--- tools/clang/Makefile.orig	2014-01-08 13:00:32.000000000 +0000
+++ tools/clang/Makefile	2015-01-24 10:58:48.831438945 +0000
@@ -14,7 +14,8 @@
 
 IS_TOP_LEVEL := 1
 CLANG_LEVEL := .
-DIRS := utils/TableGen include lib tools runtime docs unittests
+DIRS := utils/TableGen include lib tools runtime docs
+#unittests
 
 PARALLEL_DIRS :=
 
