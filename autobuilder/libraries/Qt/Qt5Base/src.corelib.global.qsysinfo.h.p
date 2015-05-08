--- src/corelib/global/qsysinfo.h.orig	2015-02-17 04:56:50.000000000 +0000
+++ src/corelib/global/qsysinfo.h	2015-05-08 12:35:48.996587940 +0100
@@ -167,6 +167,13 @@
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
