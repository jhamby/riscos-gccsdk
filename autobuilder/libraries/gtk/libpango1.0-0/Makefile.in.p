--- Makefile.in	2009-07-28 08:28:18.000000000 -0700
+++ Makefile.in.orig	2009-07-28 08:27:40.000000000 -0700
@@ -272,7 +272,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = pango modules pango-view examples docs tools tests build
+SUBDIRS = pango modules #pango-view examples docs tools tests build
 EXTRA_DIST = \
 	autogen.sh		\
 	pango.pc.in		\
