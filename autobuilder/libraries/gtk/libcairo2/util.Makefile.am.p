--- util/Makefile.am.orig	2013-08-26 16:07:21.000000000 +0100
+++ util/Makefile.am	2013-11-06 16:33:51.458038786 +0000
@@ -25,7 +25,7 @@
 if CAIRO_HAS_DLSYM
 if CAIRO_HAS_SCRIPT_SURFACE
 if CAIRO_HAS_TEE_SURFACE
-SUBDIRS += cairo-sphinx
+#SUBDIRS += cairo-sphinx
 endif
 endif
 endif
