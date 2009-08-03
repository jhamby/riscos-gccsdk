--- Makefile.in.orig	2009-08-02 19:53:01.000000000 -0700
+++ Makefile.in	2009-08-02 19:53:16.000000000 -0700
@@ -502,7 +502,7 @@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
 ACLOCAL_AMFLAGS = -I m4
-SUBDIRS = build-aux po runtime-po lib data src doc examples tests etc
+SUBDIRS = build-aux po runtime-po lib data src #doc examples tests etc
 aclocal_DATA = m4/bison-i18n.m4
 EXTRA_DIST = .prev-version .version cfg.mk maint.mk \
   OChangeLog PACKAGING \
