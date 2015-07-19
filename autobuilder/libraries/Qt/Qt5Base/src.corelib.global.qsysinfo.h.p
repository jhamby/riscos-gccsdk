--- src/corelib/global/qsysinfo.h.orig	2015-06-29 21:04:58.000000000 +0100
+++ src/corelib/global/qsysinfo.h	2015-07-15 18:24:19.858735895 +0100
@@ -186,6 +186,13 @@
     static QString productType();
     static QString productVersion();
     static QString prettyProductName();
+
+#ifdef Q_OS_RISCOS
+    static bool requireRedBlueSwap();
+private:
+    static bool RedBlueSwapVarRead;
+    static bool RequireRedBlueSwap;
+#endif
 };
 
 QT_END_NAMESPACE
