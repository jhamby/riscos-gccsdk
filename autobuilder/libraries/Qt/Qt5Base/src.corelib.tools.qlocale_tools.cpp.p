--- src/corelib/tools/qlocale_tools.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qlocale_tools.cpp	2014-05-27 19:55:16.378522842 +0100
@@ -2157,7 +2157,12 @@
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
