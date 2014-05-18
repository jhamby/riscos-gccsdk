--- Makefile.in.orig	2014-03-18 01:02:20.000000000 +0000
+++ Makefile.in	2014-05-18 16:27:53.205110800 +0100
@@ -418,7 +418,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = pango modules pango-view examples docs tools tests build
+SUBDIRS = pango modules pango-view examples# docs tools tests build
 EXTRA_DIST = \
 	autogen.sh		\
 	pango.pc.in		\
