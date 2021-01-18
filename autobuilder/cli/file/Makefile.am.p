--- magic/Makefile.am.orig	2021-01-18 15:38:58.705043082 +0000
+++ magic/Makefile.am	2021-01-18 15:39:12.344773322 +0000
@@ -313,7 +313,7 @@
 # FIXME: Build file natively as well so that it can be used to compile
 # the target's magic file; for now we bail if the local version does not match
 if IS_CROSS_COMPILE
-FILE_COMPILE = file${EXEEXT}
+FILE_COMPILE = file
 FILE_COMPILE_DEP =
 else
 FILE_COMPILE = $(top_builddir)/src/file${EXEEXT}
