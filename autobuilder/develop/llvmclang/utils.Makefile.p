--- utils/Makefile.orig	2014-06-23 18:58:39.000000000 +0100
+++ utils/Makefile	2015-01-21 21:08:37.124352861 +0000
@@ -8,8 +8,8 @@
 ##===----------------------------------------------------------------------===##
 
 LEVEL = ..
-PARALLEL_DIRS := FileCheck TableGen PerfectShuffle count fpcmp llvm-lit not \
-                 unittest
+PARALLEL_DIRS := FileCheck TableGen PerfectShuffle count fpcmp llvm-lit not
+#                 unittest
 
 EXTRA_DIST := check-each-file codegen-diff countloc.sh \
               DSAclean.py DSAextract.py emacs findsym.pl GenLibDeps.pl \
