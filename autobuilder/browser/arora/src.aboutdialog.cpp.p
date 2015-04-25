--- src/aboutdialog.cpp.orig	2010-09-27 03:42:17.000000000 +0100
+++ src/aboutdialog.cpp	2015-04-23 20:38:09.172151837 +0100
@@ -27,7 +27,10 @@
 #include <qtextedit.h>
 #include <qtextstream.h>
 
-#if QT_VERSION >= 0x040600 || defined(WEBKIT_TRUNK)
+#if QT_VERSION >= 0x050000 || defined(WEBKIT_TRUNK)
+#include <qwebkitglobal.h>
+#include <qtwebkitversion.h>
+#elif QT_VERSION >= 0x040600
 #include <qwebkitversion.h>
 #endif
 
