--- Makefile.orig	2021-10-23 09:48:15.242622719 +0100
+++ Makefile	2021-10-23 09:48:22.974450371 +0100
@@ -35,7 +35,7 @@
 # install to install in a separate location.  This is useful for
 # packagers.
 
-BUILD_ITEMS := manual libqpdf zlib-flate libtests qpdf examples
+BUILD_ITEMS := manual libqpdf zlib-flate qpdf examples
 OUTPUT_DIR = build
 ALL_TARGETS =
 
