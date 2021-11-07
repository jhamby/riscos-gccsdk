--- Makefile.am.orig	2021-11-07 15:02:02.158816081 +0000
+++ Makefile.am	2021-11-07 15:01:31.094958003 +0000
@@ -16,7 +16,7 @@
 
 DISTCLEANFILES = include/ares_build.h
 
-DIST_SUBDIRS = include src test docs
+DIST_SUBDIRS = include src docs
 
 SUBDIRS = @BUILD_SUBDIRS@
 
