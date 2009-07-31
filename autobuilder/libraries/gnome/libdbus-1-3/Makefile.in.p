--- Makefile.in.orig	2009-07-31 07:01:44.000000000 -0700
+++ Makefile.in	2009-07-31 07:01:50.000000000 -0700
@@ -347,7 +347,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = dbus bus doc tools test
+SUBDIRS = dbus #bus doc tools test
 DIST_SUBDIRS = dbus bus doc tools test
 pkgconfigdir = $(libdir)/pkgconfig
 pkgconfig_DATA = dbus-1.pc 
