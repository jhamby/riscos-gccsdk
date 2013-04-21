--- Makefile.in.orig	2012-05-26 20:50:54.000000000 +0100
+++ Makefile.in	2013-04-21 16:11:02.000000000 +0100
@@ -290,7 +290,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = pixman demos test
+SUBDIRS = pixman #demos test
 pkgconfigdir = $(libdir)/pkgconfig
 pkgconfig_DATA = pixman-1.pc
 GPGKEY = 6FF7C1A8
