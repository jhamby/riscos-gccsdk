--- docs/Makefile.in.orig	2008-01-20 12:35:45.000000000 -0800
+++ docs/Makefile.in	2008-01-20 12:35:53.000000000 -0800
@@ -239,7 +239,7 @@
 target_alias = @target_alias@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = reference
+SUBDIRS = #reference
 all: all-recursive
 
 .SUFFIXES:
