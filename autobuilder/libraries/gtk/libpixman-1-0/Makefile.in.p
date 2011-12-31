--- Makefile.in.orig	2009-07-28 15:20:44.000000000 -0700
+++ Makefile.in	2009-07-28 15:20:54.000000000 -0700
@@ -205,7 +205,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = pixman demos test
+SUBDIRS = pixman #demos test
 pkgconfigdir = $(libdir)/pkgconfig
 pkgconfig_DATA = pixman-1.pc
 GPGKEY = 6FF7C1A8
