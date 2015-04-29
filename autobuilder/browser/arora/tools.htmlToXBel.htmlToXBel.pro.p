--- tools/htmlToXBel/htmlToXBel.pro.orig	2014-04-06 19:17:48.000000000 +0100
+++ tools/htmlToXBel/htmlToXBel.pro	2015-04-29 16:38:44.888029660 +0100
@@ -37,3 +37,7 @@
     man-compress.depends = install_man
 }
 }
+
+*riscos* {
+    QMAKE_LFLAGS = -Wl,-rpath-link,$(WEBKITOUTPUTDIR)/Release/lib
+}
