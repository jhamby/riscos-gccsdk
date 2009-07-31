--- dbus/Makefile.in.orig	2009-07-31 07:11:04.000000000 -0700
+++ dbus/Makefile.in	2009-07-31 07:11:15.000000000 -0700
@@ -315,7 +315,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = . examples
+SUBDIRS = . #examples
 INCLUDES = \
 	-I$(top_srcdir)						\
 	-I$(top_builddir)					\
--- Makefile.in.orig	2009-07-31 07:11:35.000000000 -0700
+++ Makefile.in	2009-07-31 07:11:41.000000000 -0700
@@ -267,7 +267,7 @@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
 GLIB_PC = dbus-glib-1.pc
-SUBDIRS = dbus tools test doc
+SUBDIRS = dbus #tools test doc
 DIST_SUBDIRS = dbus tools test doc m4
 pkgconfigdir = $(libdir)/pkgconfig
 pkgconfig_DATA = $(GLIB_PC)
