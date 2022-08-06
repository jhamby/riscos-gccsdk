--- Makefile.orig	2022-08-06 08:47:02.992456455 +0100
+++ Makefile	2022-08-06 08:47:14.788417890 +0100
@@ -35,7 +35,7 @@
 # install to install in a separate location.  This is useful for
 # packagers.
 
-BUILD_ITEMS := manual libqpdf zlib-flate libtests qpdf fuzz examples
+BUILD_ITEMS := manual libqpdf zlib-flate qpdf fuzz examples
 OUTPUT_DIR = build
 ALL_TARGETS =
 
