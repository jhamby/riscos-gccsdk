--- Makefile.in.orig	2021-02-21 14:15:53.752135250 +0000
+++ Makefile.in	2021-02-21 14:16:32.628146789 +0000
@@ -577,7 +577,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = . gdk-pixbuf-loader tests tools doc build
+SUBDIRS = . tests tools doc build
 NULL = 
 BUILT_SOURCES = $(enum_sources)
 lib_LTLIBRARIES = librsvg-@RSVG_API_MAJOR_VERSION@.la
