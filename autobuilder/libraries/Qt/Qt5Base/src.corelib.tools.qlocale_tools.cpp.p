--- src/corelib/tools/qlocale_tools.cpp.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/tools/qlocale_tools.cpp	2015-04-12 13:43:51.636657032 +0100
@@ -2149,7 +2149,12 @@
     feholdexcept(&envp);
 #endif
 
+#ifdef Q_OS_RISCOS
+    char *s = __dtoa (d, mode, ndigits, decpt, sign, rve);
+    *resultp = s;
+#else
     char *s = _qdtoa(d, mode, ndigits, decpt, sign, rve, resultp);
+#endif
 
 #ifdef Q_OS_WIN
     _clear87();
