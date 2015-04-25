--- tools/htmlToXBel/main.cpp.orig	2010-09-27 03:42:17.000000000 +0100
+++ tools/htmlToXBel/main.cpp	2015-04-24 15:12:06.023657548 +0100
@@ -18,7 +18,13 @@
  */
 
 #include <QtGui/QtGui>
+#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
+#include <QtWebKitWidgets/QtWebKitWidgets>
+#include <QtWidgets/QtWidgets>
+#else
 #include <QtWebKit/QtWebKit>
+#endif
+
 
 /*!
     A tool to convert html bookmark files into the xbel format.
