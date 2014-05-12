--- docs/reference/gio/Makefile.in.orig	2014-03-24 18:54:26.000000000 +0000
+++ docs/reference/gio/Makefile.in	2014-05-12 19:18:45.054048693 +0100
@@ -472,7 +472,8 @@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
 NULL = 
-SUBDIRS = gdbus-object-manager-example
+SUBDIRS =
+#gdbus-object-manager-example
 
 # The name of the module.
 DOC_MODULE = gio
